//
// A very primitive pad monitor for ALTRO raw data
//
// by Katsumasa Ikematsu (KEK)
//
// $Id: HitMonitor.C,v 1.3 2009/06/10 13:10:43 fujiik Exp $
//
#ifndef __CINT__
#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TFile.h"
#include "TH1.h"
#include "TText.h"
#include "TCanvas.h"
#include "TColor.h"
#include "../GEMDisplay.h"
#include "../GEMHist.h"
#include "../../unpacker/JREvent.h"
#include "../../unpacker/JCluster.h"
#include <sstream>
#endif

const int kMaxWidth  = 50;
const int kMinWidth  = 14;
const int kThreshold = 10;

void HitMonitor(Int_t /* event = 1 */, Int_t /* run = 68 */, Int_t /* part = 0 */, Bool_t reset = true) {
  if (reset) {
    gROOT->LoadMacro("SetGlobalStyle.C");
    gROOT->ProcessLine("SetGlobalStyle()");
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    gStyle->SetPadBottomMargin(1.);
    gStyle->SetPadTopMargin(1.);
    gStyle->SetPadLeftMargin(1.);
    gStyle->SetPadRightMargin(1.);
    gStyle->SetPalette(1);
    Int_t   color  = TColor::GetColor(0,0,0);
    gStyle->SetPadColor(color);
  }

  Int_t kNRow = 28*3;
  Int_t kNPad = 48;

  GEMDisplay *gemdisplay = GEMDisplay::GetInstance();
  gemdisplay->GetCurFile()->cd();
  gemdisplay->GetCanvas()->Clear();
  gemdisplay->GetCanvas()->Divide(kNPad, kNRow, 0, 0);

  JREvent *ep = gemdisplay->GetCurEvent();
  JCluster::SetSortBy(JCluster::kDet);
  ep->Sort();
  TIter next(ep);
  JCluster *cp;
  while ((cp = static_cast<JCluster *>(next()))) {
#if 0 
    int phmax = cp->GetMaximum();
    int width = cp->GetLength();
    if (phmax < kThreshold || width < kMinWidth || width > kMaxWidth) continue;
#endif
    Int_t module = cp->GetModuleID();
    Int_t layer  = cp->GetLayerID();
    Int_t pad    = cp->GetPadID();
    Int_t row    = module == 1 ? layer : (module == 3 ? layer+28 : layer + 56);
    Int_t col    = module == 1 ? pad : (module == 3 ? pad - 128 : pad - 48);
#if 0
    GEMDisplay::GetInstance()->GetCanvas()->cd((col+1)+(row*kNPad));
#else
    GEMDisplay::GetInstance()->GetCanvas()->cd((col+1)+((kNRow-row-1)*kNPad));
#endif
    Int_t charge = cp->GetCharge();
    Int_t   color  = TColor::GetColor((float)(charge/2000.),(float)(charge/2000.),(float)0.);
    //Int_t   color  = TColor::GetColor(charge*255/1024.,0,0);
    //Int_t   color  = TColor::GetColor((ULong_t)(charge*16770000./1000000.));
    gPad->SetFillColor(color);
    gPad->Draw();
  }
  gemdisplay->GetCanvas()->Update();
}
