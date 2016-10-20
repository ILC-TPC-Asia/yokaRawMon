//
// A very primitive pad monitor for ALTRO raw data
//
// by Katsumasa Ikematsu (KEK)
//
// $Id: PadMonitor3.C,v 1.3 2009/06/04 14:02:10 fujiik Exp $
//
#ifndef __CINT__
#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TFile.h"
#include "TH1.h"
#include "TText.h"
#include "TCanvas.h"
#include "../GEMDisplay.h"
#include "../GEMHist.h"
#include "../../unpacker/JREvent.h"
#include "../../unpacker/JCluster.h"
#include <sstream>
#endif

const Int_t kMinXHisto = 0;
const Int_t kMaxXHisto = 512;
const Int_t kMinYHisto = -20;
const Int_t kMaxYHisto = 200;

const Int_t kThresholdPH = 20;

void PadMonitor3(Int_t /* event = 1 */, Int_t /* run = 68 */, Int_t /* part = 0 */, Bool_t reset = true) {
  if (reset) {
    gROOT->LoadMacro("SetGlobalStyle.C");
    gROOT->ProcessLine("SetGlobalStyle()");
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    gStyle->SetPadBottomMargin(0.);
    gStyle->SetPadTopMargin(0.);
    gStyle->SetPadLeftMargin(0.);
    gStyle->SetPadRightMargin(0.);
    gStyle->SetPalette(1);
  }
#if 1
  Int_t kNRow = 28;
  Int_t kNPad = 48;

  /////////////////////////
  Int_t kModule = 3;
  Int_t kOffset = 128;
  /////////////////////////
#endif
  GEMDisplay *gemdisplay = GEMDisplay::GetInstance();
  gemdisplay->GetCurFile()->cd();
  gemdisplay->GetCanvas()->Clear();
  gemdisplay->GetCanvas()->Divide(kNPad, kNRow, 0, 0);

  JREvent *ep = gemdisplay->GetCurEvent();
  JCluster::SetSortBy(JCluster::kDet);
  ep->Sort();
  GEMHist *gemhist = GEMHist::GetInstance();
  gemhist->ClearAll();
  gemhist->FillEvent(ep);

  for (Int_t iRow = 0; iRow < kNRow; iRow++) {
    for (Int_t iPad = 0; iPad < kNPad; iPad++) {
#if 0
      GEMDisplay::GetInstance()->GetCanvas()->cd((iPad+1)+(iRow*kNPad));
#else
      GEMDisplay::GetInstance()->GetCanvas()->cd((iPad+1)+((kNRow-iRow-1)*kNPad));
#endif
      stringstream ostr;
      ostr << "HTIME_" << kModule << "_" << iRow << "_" << iPad + kOffset << ends; 
      TH1F *hPulse = static_cast<TH1F *>(gROOT->FindObject(ostr.str().data()));
      hPulse->SetAxisRange(kMinXHisto, kMaxXHisto);
      hPulse->SetMinimum(kMinYHisto);
      hPulse->SetMaximum(kMaxYHisto);
      hPulse->UseCurrentStyle();
      hPulse->Draw();
      Int_t    peaktime = hPulse->GetMaximumBin();
      Double_t peakph   = hPulse->GetBinContent(peaktime);

      TText t1(0, 0, "");
      t1.SetTextFont(132);
      t1.SetTextSize(0.4);
      t1.SetTextColor(4);

      ostr.str("");
      ostr.clear();
      ostr << peaktime << ends;
      if (peakph > kThresholdPH) t1.DrawText(peaktime, 0.5 * kMaxYHisto, ostr.str().data());

      TText t2(0, 0, "");
      t2.SetTextFont(132);
      t2.SetTextSize(0.3);
      Double_t xt2 = kMinXHisto + 0.05 * kMaxXHisto;
      ostr.str("");
      ostr.clear();
      ostr << iPad + 1 << ends;
      if (iRow == kNRow -1) t2.DrawText(xt2 + 0.1 * kMaxXHisto, 0.1 * kMaxYHisto, ostr.str().data());
      ostr.str("");
      ostr.clear();
      ostr << iRow + 1 << ends;
      if (!iPad) t2.DrawText(xt2, 0.7 * kMaxYHisto, ostr.str().data());
    }
  }
  gemdisplay->GetCanvas()->Update();
}
