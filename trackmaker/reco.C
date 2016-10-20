//#define __DEBUG__
#include <iomanip>
#include "Runinfo.h"
#ifndef __CINT__
#include <sstream>
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TNtupleD.h"
#include "TH2.h"
#include "TTree.h"

#include "EXTPCKaLDetector.h"
#include "TKalDetCradle.h"

#include "JREvent.h"
#include "JCluster.h"
#include "JHitMaker.h"
#include "JHitCorrector.h"
#include "JTrackMaker.h"
#include "JSimpleTrackFitter.h"
#include "JTripletTrackFinder.h"
#include "JTrack.h"
#endif
//-----------------------------------//
//  Pulse shape (height/width) cuts  //
//-----------------------------------//
///////////////////////////////////////
//const double kCosThetaCut = 0.98;
//const int    kMaxMissing  =  15;
//const int    kMinNhits    =  10;
//const int    kZdiffCut    = 2.0;
const double kCosThetaCut = 0.98;
//const double kCosThetaCut = 0.97;
const int    kMaxMissing  =  15;
//const int    kMinNhits    =  20;
//const int    kMinNhits    =  10;
const int    kMinNhits    =  22;
const int    kZdiffCut    = 5.0;
//const int    kZdiffCut    = 2.0;
//const int    kZdiffCut    = 3.0;
///////////////////////////////////////

void reco(Int_t run = 17319, Double_t p = 5., const Char_t *delxfile = "Results/deltax0.dat")
//void reco(Int_t run = 17324, Double_t p = 5., const Char_t *delxfile = "Results/deltax0.dat")
//void reco(Int_t run = 17327, Double_t p = 5., const Char_t *delxfile = "Results/deltax0.dat")
//void reco(Int_t run = 17331, Double_t p = 5., const Char_t *delxfile = "Results/deltax0.dat")
//void reco(Int_t run = 17340, Double_t p = 5., const Char_t *delxfile = "Results/deltax0.dat")
//void reco(Int_t run = 17344, Double_t p = 5., const Char_t *delxfile = "Results/deltax0.dat")
//void reco(Int_t run = 17352, Double_t p = 5., const Char_t *delxfile = "Results/deltax0.dat")
//void reco(Int_t run = 17356, Double_t p = 5., const Char_t *delxfile = "Results/deltax0.dat")
//void reco(Int_t run = 17359, Double_t p = 5., const Char_t *delxfile = "Results/deltax0.dat")
//void reco(Int_t run = 17364, Double_t p = 5., const Char_t *delxfile = "Results/deltax0.dat")
//void reco(Int_t run = 17367, Double_t p = 5.0, const Char_t *delxfile = "Results/deltax0.dat")
{
#ifdef __CINT__
  gSystem->Load("libTree");
  gSystem->Load("libGraf3d");
  gSystem->Load("libPhysics");
  gSystem->Load("$(KALTESTROOT)/lib/libS4Utils");
  gSystem->Load("$(KALTESTROOT)/lib/libS4Kalman");
  gSystem->Load("$(KALTESTROOT)/lib/libS4Geom");
  gSystem->Load("$(KALTESTROOT)/lib/libS4KalTrack");
  gSystem->Load("../kalman/lib/libEXKern");
  gSystem->Load("../unpacker/libJREvent");
  gSystem->Load("../kalman/lib/libEXTPC");
  gSystem->Load("../hitmaker/libJHits");
  gSystem->Load("libJTracks");
#endif

#if 0
  gInterpreter->AddIncludePath("../../KalTest/include");
  gInterpreter->AddIncludePath("../unpacker");
  gInterpreter->AddIncludePath("../hitmaker");
  gInterpreter->AddIncludePath("../trackmaker");
#endif

  // ---------------
  // Reset Run Info.
  // ---------------
  Runinfo &rinfo = *Runinfo::GetInstancePtr();

  // ---------------
  // Open input file
  // ---------------
  stringstream finstr;
  finstr << "../unpacker/root/readout-" << run << "_0.root" << ends;
  cerr << "Infile: " << finstr.str().data() << endl;
  TFile *fp = new TFile(finstr.str().data());
  TFile &f  = *fp;
  TTree *T  = static_cast<TTree *>(f.Get("Event"));
  JREvent *event = 0;
  T->SetBranchAddress("Event", &event);
  Int_t nentries = (Int_t)T->GetEntries();
  cerr << "nentries = " << nentries << endl;

  // ----------------
  // Open output file
  // ----------------
  stringstream dirname;
  dirname << "Results/10k" << ends;
  //dirname << "Results/1k" << ends;
  if (gSystem->AccessPathName(dirname.str().data())){
    gSystem->mkdir(dirname.str().data(),kTRUE);
    cerr << "Create new directory: " << dirname.str().data() << endl;
  }
  stringstream foutstr;
  foutstr << dirname.str().data() << "/p." << p << "_z." << rinfo.GetDlength(run) << "_B." << rinfo.GetBfield(run) << ".root" << ends;
  cerr << "Outfile: " << foutstr.str().data() << endl;
  TFile *hfp = new TFile(foutstr.str().data(),"RECREATE","Marilin");

  // --------------------
  // Create Reco. classes
  // --------------------
#if 0
  JSimpleTrackFitter::SetLayerToLookAt(layertolookat);
  if (inout == "out") {
    JSimpleTrackFitter::SetIsOutFlag();
    JTrackMaker::SetLayerToLookAt(layertolookat);
  }
#endif
  cerr << "distortion correction file = " << delxfile << endl;
  (void *) JHitCorrector::GetInstancePtr(delxfile);
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
#if 0
#if 0
  trackmaker.KillModule(1);
  trackmaker.KillModule(6);
#else
  trackmaker.KillModule(0);
  //trackmaker.KillModule(3);
  trackmaker.KillModule(5);
#if 0
  // Exclude row 44 from the fit
  Int_t layertolookat = 16;
  JSimpleTrackFitter::SetIsOutFlag();
  JSimpleTrackFitter::SetLayerToLookAt(layertolookat);
#endif
#endif
#endif
  // --------------------
  // Now loop over events
  // --------------------
  const Int_t kMaxEvt = 10000;
  //const Int_t kMaxEvt = 2000;
  //const Int_t kMaxEvt = 1000;

  for (Int_t ev=0; ev<nentries && ev<kMaxEvt; ev++) {
      T->GetEntry(ev);
      if (!(ev%100)) {
        cerr << "ev: " << ev << " evno: " << event->GetEventNumber() << " #cls: " << event->GetEntries() << endl;
      }
      cerr << " -------<Event " << ev << " >----------------" << endl;
      JCluster::SetSortBy(JCluster::kDet); // set sorting policy
      event->Sort(); // sort clusters in ascending order of layers
      hitmaker.MakeHits(event);
      //hitmaker.DebugPrint("detail");
      trackmaker.MakeTracks(&hitmaker);
      trackmaker.DebugPrint("detail");
  } 
  // -----------------------
  // Now output tree headers
  // -----------------------
  TNtupleD *hTracks = static_cast<TNtupleD *>(gROOT->FindObject("hTracks"));
  TNtupleD *hResXin = static_cast<TNtupleD *>(gROOT->FindObject("hResXin"));
  TNtupleD *hResZin = static_cast<TNtupleD *>(gROOT->FindObject("hResZin"));

  const Int_t kNmodules = 3;
  const Int_t kNlayers  = 28;
  TH2D *hPadRes[kNmodules*kNlayers];
  for (Int_t i=0; i<kNmodules*kNlayers; i++) {
    stringstream name;
    name << "qx" << setw(2) << setfill('0') << i << ends;
    hPadRes[i] = static_cast<TH2D *>(gROOT->FindObject(name.str().data()));
  }

  hfp->cd();
  hTracks->Write();
  hResXin->Write();
  hResZin->Write();
  for (Int_t i=0; i<kNmodules*kNlayers; i++) { 
     if (hPadRes[i]) hPadRes[i]->Write();
  }
  hfp->Write();
}
