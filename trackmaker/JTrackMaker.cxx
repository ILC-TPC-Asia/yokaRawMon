#include "JTrackMaker.h"
#include "JHitLayer.h"
#include "JLock.h"
#include "JSimpleTrackFinder.h"

#include "TPad.h"
#include "TString.h"
#include "TColor.h"

#include <cstdlib>
#include <iostream>
#include <iomanip>
using namespace std;

ClassImp(JTrackMaker)

Int_t JTrackMaker::fgLayerToLookAt  = 14;
//Int_t JTrackMaker::fgModuleToLookAt =  3;
Int_t JTrackMaker::fgModuleToLookAt =  -3;

JTrackMaker::JTrackMaker()
        : fHitLayersPtr  (0), // pointer to hits 
	  fDone     (kFALSE), // update flag
          fTrackFinderPtr(0)  // pointer to track finder
{
  SetOwner();
}

JTrackMaker::~JTrackMaker()
{
  if (fTrackFinderPtr->TestBit(kMyOwn)) delete fTrackFinderPtr;
}

void JTrackMaker::DebugPrint(const Char_t *opt)
{
  if (!fDone) Update();
  cerr << "Number of tracks =  " << GetEntries() << endl;
  TString opts(opt);
  opts.ToLower();
  if (opts.Contains("detail")) {
    cerr << "------" << endl;
    Int_t ntrks = 0;
    TIter next(this);
    JTrack *tp;
    while ((tp = static_cast<JTrack *>(next()))) {
      cerr << "---<hit " << ntrks++ << ">-----" << endl;
      tp->DebugPrint(opt);
    }
  }
}

void JTrackMaker::SetHits(TObjArray *jhitlayersptr)
{
  Clear();
  fHitLayersPtr = jhitlayersptr;
  fDone         = kFALSE;
}

void JTrackMaker::SetTrackFinder(JVTrackFinder *trackfinderptr)
{
  if (fTrackFinderPtr && fTrackFinderPtr->TestBit(kMyOwn)) delete fTrackFinderPtr;
  if (!trackfinderptr) {
    fTrackFinderPtr = new JSimpleTrackFinder(this);
    fTrackFinderPtr->SetBit(BIT(kMyOwn),kTRUE);
#if 1
    cerr << " TrackFinder created" << endl;
#endif
  } else {
    fTrackFinderPtr = trackfinderptr;
  }
  fDone           = kFALSE;
  JTrack::SetTrackFinder(fTrackFinderPtr);
}

void JTrackMaker::MakeTracks(TObjArray *jhitlayersptr)
{
  SetHits(jhitlayersptr);
  Update();
}

void JTrackMaker::Update()
{
  if (fDone) return;
  if (!fHitLayersPtr) {
    cerr << ":::::: ERROR in JTrackMaker::Update ::::::" << endl
	 << " Hits not yet set! abort."                  << endl;   
    ::abort();
  }	  
  if (!fHitLayersPtr->GetEntries()) {
    cerr << ":::::: WARNING in JTrackMaker::Update ::::" << endl
	 << " No Hits in array! skip."                   << endl;   
    return;
  }	  
  fDone = kTRUE;
  //--
  // Select layers to use 
  //--
  TObjArray activelayers; // layers to use
  TIter next(fHitLayersPtr);
  JHitLayer *hlp;
  while ((hlp = static_cast<JHitLayer *>(next()))) {
    Int_t module = hlp->GetModuleID();
    vector<Int_t>::iterator iter;
    for (iter=fKill.begin(); iter != fKill.end(); iter++) {
      if ((*iter) == module) hlp->SetBit(BIT(kLock),kTRUE);
    }
    if (!hlp->TestBit(BIT(kLock))) activelayers.Add(hlp);
  }
#if 1
  cerr << "active layers = " << activelayers.GetEntries() << endl;
#endif
  //--
  // Find tracks
  //--
  if (!fTrackFinderPtr) SetTrackFinder();
  fTrackFinderPtr->FindTracks(&activelayers);
}

void JTrackMaker::KillModule(Int_t module)
{
   fKill.push_back(module);
}

void JTrackMaker::Draw(const Char_t *opt)
{
  Int_t color = 10; 
  Draw(color, opt);
}

void JTrackMaker::Draw(Int_t color, const Char_t *opt)
{
  if (!gPad)   return;
  if (!fDone) Update();
  TIter next(this);
  JTrack *tp;
  while ((tp = static_cast<JTrack *>(next()))) {
    tp->Draw(color, opt);  
  }
}
