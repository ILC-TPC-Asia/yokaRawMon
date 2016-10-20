#include "JTrack.h"
#include "JHit.h"
#include "JCluster.h"
#include "JPadGeom.h"
#include "JVTrackFinder.h"
#include "JVTrackFitter.h"
#include "JSimpleTrackFitter.h"
#include "JLock.h"

#include "TMath.h"
#include "TVector2.h"
#include "TPad.h"
#include "TString.h"
#include "TColor.h"
#include "TPolyLine3D.h"

#include <cstdlib>
#include <iomanip>
#if 1
JTrack::ESort  JTrack::fgSortBy = kChi2;     //! sort type
#endif
JVTrackFinder *JTrack::fgTrackFinderPtr = 0; // pointer to track Finder
JVTrackFitter *JTrack::fgTrackFitterPtr = 0; // pointer to track Fitter

ClassImp(JTrack)

JTrack::JTrack(JHit *hp)
        : fChi2     (0),
          fNdf      (0),
	  fDone     (kFALSE) // update flag
{
  if (hp) {
    Add(hp);
  }
}

JTrack::JTrack(const JTrack &orig)
        : //TObjArray(orig),
          fNdf      (orig.fNdf),
          fHelix    (orig.fHelix),
	  fDone     (orig.fDone) // update flag
{
  SetOwner(kFALSE);
}

JTrack::~JTrack()
{
  if (IsOwner()) Clear();
}

Bool_t JTrack::Append(JHit *hp)
{
  if (!fgTrackFinderPtr) {
    cerr << "::::: ERROR in JTrak ::::::::::" << endl
         << " No track finder available!"     << endl
         << " Abort!"                         << endl;
    ::abort();
  } else {
    return fgTrackFinderPtr->AppendHit(this, hp);
  }
}

void JTrack::SetTrackFinder(JVTrackFinder *trackfinderptr)
{
  fgTrackFinderPtr = trackfinderptr;
}

void JTrack::SetTrackFitter(JVTrackFitter *trackfitterptr)
{
#if 0
  if (fgTrackFitterPtr && 
      fgTrackFitterPtr->TestBit(BIT(kMyOwn))) delete fgTrackFitterPtr;
#endif
  fgTrackFitterPtr = trackfitterptr;
}

#if 1
Int_t JTrack::Compare(const TObject *obj) const
{
  const JTrack *cp = static_cast<const JTrack *>(obj);
  switch (fgSortBy) {
    case kChi2:
	 return fChi2 < cp->fChi2 ? -1 : (fChi2 > cp->fChi2 ? +1 : 0);
    default:
         cerr << " Error::::: JTrack::Compare :::::::::::::::::"   << endl
              << "   ESort = " << fgSortBy << " is not supported." << endl
	      << "   Abort!                                      " << endl;
	 ::abort();
  }
}
#endif

void JTrack::DebugPrint(const Char_t *opt)
{
  if (!fDone) Update();
  cerr << "--------" << endl;
  cerr << "Nhits = " << GetEntries() << endl;
  cerr << "(chi2,ndf)=(" << fChi2  << "," << fNdf << ")" << endl;
  fHelix.DebugPrint(opt);
  TString opts(opt);
  opts.ToLower();
  if (opts.Contains("detailed")) {
    cerr << "---" << endl;
    TIter next(this);
    JHit *hp;
    while ((hp = static_cast<JHit *>(next()))) {
      hp->DebugPrint(opt);
    }
  }
}

void JTrack::Update()
{
  if (fDone) return;
  if (!GetEntries()) {
    cerr << ":::::: ERROR in JTrack::Update :::::::" << endl
         << " No hit set! abort! "                   << endl;
    ::abort();
  }
  fDone = kTRUE;
  //
  // Track Fiting should be done here.
  //
  if (!fgTrackFitterPtr) {
    fgTrackFitterPtr = new JSimpleTrackFitter();
    fgTrackFitterPtr->SetBit(BIT(kMyOwn),kTRUE);
  }
  fgTrackFitterPtr->Fit(this,fHelix,fChi2,fNdf);
}

void JTrack::Draw(const Char_t *opt)
{
  static Int_t color = 1;
  color %= 20;
  color++;
  Draw(color, opt);
}

void JTrack::Draw(Int_t color, const Char_t *opt)
{
   if (!gPad)   return;
   gPad->cd();

   if (!fDone) Update();
   JTrack *clonep = new JTrack(*this);     // must clone it since pickable

   clonep->AppendPad(opt);                 // will be killed by gPad
   clonep->fHelix.SetBit(TObject::kCannotPick);

   TIter next(this);
   JHit *hp;
   while ((hp = static_cast<JHit *>(next()))) {
      hp->Draw(color+1, opt);
   }

   clonep->fHelix.Draw(color, opt);
   gPad->Update();
   cerr << "Called JTrack::Draw()" << endl;
}

//--------------------------------------------------------------------------
//====================================
// JTrack::DistancetoPrimitive(...)
//====================================
// Object picker for 3D display
//
Int_t JTrack::DistancetoPrimitive(Int_t px, Int_t py)
{
  return fHelix.DistancetoPrimitive(px,py);
}
