//
// A very primitive pad monitor for ALTRO raw data
//
// by Katsumasa Ikematsu (KEK)
//
// $Id: TrackMonitor3D.C,v 1.5 2011/01/30 18:09:35 fujiik Exp $
//
#ifndef __CINT__
#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TView.h"
#include "TTUBE.h"
#include "TTUBS.h"
#include "TMath.h"
#include "TVector3.h"
#include "TClassMenuItem.h"
#include "../GEMDisplay.h"
#include "../../unpacker/JREvent.h"
#include "../../unpacker/JCluster.h"
#include "TKalDetCradle.h"
#include "TVKalDetector.h"
#include "TAttDrawable.h"
#include "EXVKalDetector.h"
#include "EXTPCKalDetector.h"
#include "JHitMaker.h"
#include "JTrack.h"
#include "JTrackMaker.h"
#include "JSimpleTrackFitter.h"
#include "JTripletTrackFinder.h"
#include "JHitCorrector.h"

#include <sstream>
#endif
#include "TNode.h"

//#define __MOVIE__
#define SEP_2010

///////////////////////////////////////
//-----------------------------------//
//  Pulse shape (height/width) cuts  //
//-----------------------------------//
///////////////////////////////////////
#if 1
//const int kMaxWidth  = 200;
const int kMaxWidth  = 60;
const int kMinWidth  =  7;
const int kThreshold =  8;
//const int kThreshold =  5;
#else
const int kMaxWidth  = 15;
const int kMinWidth  =  5;
const int kThreshold =  90;
#endif
const double kCosThetaCut = 0.98;
const int    kMaxMissing  =  15;
//const int    kMinNhits    =  10;
const int    kMinNhits    =  22;
const int    kZdiffCut    = 5.0;
///////////////////////////////////////

TNode *gWorldNodePtr = 0;
TNode *gDetNodePtr   = 0;
TNode *gMnNodePtr[7] = {0, 0, 0, 0, 0, 0, 0};

#if 0
void TrackMonitor3D(Int_t /* event = 1 */, Int_t /* run = 68 */, Int_t /* part = 0*/, Bool_t reset = true) {
#else
void TrackMonitor3D(Int_t event = 1, Int_t run = 68, Int_t part = 0, Bool_t reset = true) {
#endif
  if (reset) {
    gROOT->LoadMacro("SetGlobalStyle.C");
    gROOT->ProcessLine("SetGlobalStyle()");
  }

  GEMDisplay *gemdisplay = GEMDisplay::GetInstance();
  gemdisplay->GetCurFile()->cd();
  gemdisplay->GetCanvas()->Clear();
  //--
  // Create a 3D viewer
  //--
  TView   *vwp = TView::CreateView(1,0,0);
  const Double_t kRangeXY = 230.;
  vwp->SetRange(-kRangeXY, -kRangeXY*1024./768.,-5.,+kRangeXY, +kRangeXY*1024./768., +595.);
  Int_t ierr;
#if 0
  vwp->SetView(190.,80.,80.,ierr); // 3D view
#else
   vwp->SetView(0.,0.,0.,ierr); // Top view
  //vwp->SetView(0.,90.,0.,ierr); // Side view
#endif
  //--
  // Create a crude LP1 model
  //--
  if (!gWorldNodePtr) {
    const Double_t kRout  = 370.;
    const Double_t kHalfL = 300.;
    new TTUBE("World","World","void",300.,300.,300.);
    new TTUBE("Det","Det","void", kRout, kRout, kHalfL);
    gWorldNodePtr = new TNode("WNode","WNode","World",0.,0.,0.);
    gWorldNodePtr->cd();
    gDetNodePtr   = new TNode("DNode","DNode","Det",0.,0., kHalfL);
    gWorldNodePtr->SetVisibility(0);

#if 0
    const Double_t kDphi[2]  = { 0.000821484, 0.000753027}; // delta phi per pad
    const Int_t    kNpad[2]  = {        176,         192}; // # pads per row
#endif
    const Int_t    kNrow     = 28;
    const Double_t kDr       = 5.36;
    const Double_t kRmin     = 1440.01;
    const Double_t kGap      = 0.10;
    const Double_t kRad2Deg  = 180./TMath::Pi();
    const Double_t kPhiWidth = 0.14464384*kRad2Deg;
    const Double_t kRmax     = kRmin + kNrow*kDr - kGap;
    const Double_t kZ        = 0.;
    const Double_t kPhi0[7]  = {0.12485582, -0.021577304,
                                0.2208493 , 0.07441581, -0.07184279,
                                0.15976241, 0.013329281 };
    const Double_t kR0  [7]  = {1676.115 , 1676.115,
                                1503.615, 1503.615, 1503.615,
                                1332.000 , 1332.000 };
    const Char_t  *kName[7]  = {"M0", "M1", "M2", "M3", "M4", "M5", "M6"};

    for (Int_t module=0; module<7; module++) {
      new TTUBS(kName[module], kName[module],
               "void", kRmin, kRmax, kZ, 
               90. - kPhi0[module]*kRad2Deg,
               90. - kPhi0[module]*kRad2Deg + kPhiWidth);
      string rotname("rot");
      rotname += kName[module];
      new TRotMatrix(rotname.data(), rotname.data(),
                     90., 0.,
                     90.,90.,
                      0., 0.);
      TVector3 xv(0.,-kR0[module],0.);
      string nodename("N");
      nodename += kName[module];
      gMnNodePtr[module] =  new TNode(nodename.data(), nodename.data(), kName[module],
                                      xv.X(),xv.Y(),xv.Z(),
                                      rotname.data());
#ifdef SEP_2010
      if (module != 0 && module != 3 && module != 5) { // active modules are 0, 3, and 5 now.
//#else 
  //    if (module != 1 && module != 3 && module != 6) { // active modules are 1, 3, and 6 now.
#else 
      if (module != 0 && module != 3) { // active modules are 0 and 3 now.
#endif
        gMnNodePtr[module]->SetLineStyle(2); // dummy module with dashed lines
      }
    }
    //--
    // Add "Draw Pulse" as a Menu Item for the "JCluster" Object
    //--
    TClass *classp = gROOT->GetClass("JCluster");
    TList *listp = classp->GetMenuList();
    Int_t ptype = 2; // the (TObject *) pointer passed to the function "DrawPulse" is the selected object
    TClassMenuItem *it1p = new TClassMenuItem(TClassMenuItem::kPopupUserFunction,classp,
                                              "Draw Pulse","DrawPulse",0,"TObject *",ptype); 
    listp->AddFirst(it1p); // add this as the top item
    //--
    // Add "Draw Track Param" as a Menu Item for the "JTrack" Object
    //--
    TClass *class2p = gROOT->GetClass("JTrack");
    listp = class2p->GetMenuList();
    TClassMenuItem *it2p = new TClassMenuItem(TClassMenuItem::kPopupUserFunction,class2p,
                                              "Print Track Param","PrintTrackParam",0,"TObject *",ptype); 
    listp->AddFirst(it2p); // add this as the top item
  }
  gWorldNodePtr->Draw("pad same");

  //--
  // Loop over clusters and draw them on the 3D canvas
  //--
  JREvent *ep = gemdisplay->GetCurEvent();
  JCluster::SetSortBy(JCluster::kDet);
  ep->Sort(); // sort clusters from upstream to downstream
#ifdef __MOVIE__
  Int_t ncp = 0;
  gSystem->Unlink("evt.gif");
#endif
  TIter next(ep);
  JCluster *cp;
  while ((cp = static_cast<JCluster *>(next()))) {
    if (cp->GetLength() < kMinWidth || 
        cp->GetLength() > kMaxWidth ||
        cp->GetMaximum() < kThreshold) continue;
    cp->Draw(); 
#ifdef __MOVIE__
    if (ncp%5 == 0) {
      gemdisplay->GetCanvas()->Modified();
      gemdisplay->GetCanvas()->Update();
      gemdisplay->GetCanvas()->Print("evt.gif+");
    }
    ncp++;
#endif
  }
  //JHitCorrector::GetInstancePtr("../../trackmaker/Results/deltax2.dat");
  JHitCorrector::GetInstancePtr("../../trackmaker/Results/deltax0.dat");
  JHitMaker        hitmaker;
  TKalDetCradle    lp1;
  EXTPCKalDetector tpcdet;
  lp1.Install(tpcdet);
  //lp1.SwitchOffMS();

  JSimpleTrackFitter fitter(&tpcdet);
  JTrack::SetTrackFitter(&fitter);

  JTrackMaker trackmaker;
  trackmaker.SetTrackFinder(new JTripletTrackFinder(&trackmaker));
  JTripletTrackFinder::SetMinNhits   (kMinNhits);
  JTripletTrackFinder::SetMaxMissing (kMaxMissing);
  JTripletTrackFinder::SetCosThetaCut(kCosThetaCut);
  JTripletTrackFinder::SetZdiffCut(kZdiffCut);
  //trackmaker.KillModule(1);
  //trackmaker.KillModule(3);
  //trackmaker.KillModule(6);
  trackmaker.SetModuleToLookAt(-1);
  trackmaker.SetLayerToLookAt(-1);

  hitmaker.MakeHits(ep);
  //hitmaker.DebugPrint("detail");
  trackmaker.MakeTracks(&hitmaker);
#if 1
  //trackmaker.DebugPrint("detailed");
  hitmaker.Draw(5);
  trackmaker.Draw(3);
#endif
  gemdisplay->GetCanvas()->Modified();
  gemdisplay->GetCanvas()->Update();
}

//-------------------------------------------------------------------------------------------
//========================
// DrawPulse(Tobject *obj)
//========================
//(Function)
//  Create a pule shape histogram for the selected cluster.
//
void DrawPulse(TObject *obj)
{
  JCluster *cp = static_cast<JCluster *>(obj);
  Int_t module = cp->GetModuleID();
  Int_t layer  = cp->GetLayerID();
  Int_t pad    = cp->GetPadID();

  cerr << " module = " << module 
       << " layer  = " << layer 
       << " pad    = " << pad
       << " rcu    = " << cp->GetRCUID()
       << " ch     = " << cp->GetChannelID()
       << " charge = " << cp->GetCharge()
       << " phmax  = " << cp->GetMaximum()
       << " length = " << cp->GetLength()
       << endl;

  TCanvas *cphp = new TCanvas("cph","",500,500);
  //--
  // Create a pulse shape histogram
  //--
  stringstream hname;
  hname.str("");
  hname.clear();
  hname << "hPulse" << module << "_" << layer << "_" << pad << ends;
  TH1F *hp = new TH1F(hname.str().data(),hname.str().data(),1024,0.,1024.);
  hp->SetMinimum(-10.);
  hp->SetMaximum(1090.);
  hp->SetBit(TObject::kCanDelete);
  hp->GetXaxis()->SetTitle("Time bin");
  hp->GetYaxis()->SetTitle("ADC count");
  //--
  // Fill the histogram
  //--
  Int_t time = cp->GetStartTime();
  for (Int_t i=0; i<cp->GetLength(); i++) {
    hp->Fill(i+time, cp->operator[](i));
  }
  hp->Draw();
  cphp->Update();
}
//-------------------------------------------------------------------------------------------
//==============================
// PrintTrackParam(Tobject *obj)
//==============================
//(Function)
//  Print out track parameters
//
void PrintTrackParam(TObject *obj)
{
  JTrack *tp = static_cast<JTrack *>(obj);
  tp->DebugPrint();
}
