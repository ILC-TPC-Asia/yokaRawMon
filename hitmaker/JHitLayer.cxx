#include "JHitLayer.h"

#include "TPad.h"
#include "TTUBS.h"
#include "TString.h"
#include "TColor.h"

#include <cstdlib>
#include <iomanip>
#if 1
JHitLayer::ESort  JHitLayer::fgSortBy = kDet;     //! sort type
#endif

ClassImp(JHitLayer)

JHitLayer::JHitLayer()
        : fModule      (-1),     // module ID
	  fLayer       (-1),     // layer ID
	  fClustersPtr (0),      // pointer to cluster
	  fDone        (kFALSE), // update flag
	  fTubsPtr     (0)       // pointer to tubs 
{
  SetOwner();
}

#if 1
Int_t JHitLayer::Compare(const TObject *obj) const
{
  const JHitLayer *cp = static_cast<const JHitLayer *>(obj);
  switch (fgSortBy) {
    case kDet:
         if (fModule < cp->fModule) return -1;
         if (fModule > cp->fModule) return +1;
         if (fModule == cp->fModule) {
           return fLayer < cp->fLayer ? -1 : (fLayer > cp->fLayer ? +1 : 0);
	 }
    case kLayer:
	 return fLayer < cp->fLayer ? -1 : (fLayer > cp->fLayer ? +1 : 0);
    default:
         cerr << " Error::::: JHitLayer::Compare :::::::::::::::::" << endl
              << "   ESort = " << fgSortBy << " is not supported." << endl
	      << "   Abort!                                      " << endl;
	 ::abort();
  }
}
#endif

void JHitLayer::DebugPrint(const Char_t *opt)
{
  if (!fDone) Update();
  cerr << "(mod,lyr)=(" << fModule << "," << fLayer << ") " << endl;
  TString opts(opt);
  opts.ToLower();
  if (opts.Contains("detail")) {
    cerr << "------" << endl;
    Int_t nhits = 0;
    TIter next(this);
    JHit *hp;
    while ((hp = static_cast<JHit *>(next()))) {
      cerr << "---<hit " << nhits++ << ">-----" << endl;
      hp->DebugPrint(opt);
    }
  }
}

void JHitLayer::SetClusters(TObjArray *jclsptr)
{
  fClustersPtr = jclsptr;
  fDone        = kFALSE;
}

void JHitLayer::MakeHits(TObjArray *jclsptr)
{
  SetClusters(jclsptr);
  Update();
}

void JHitLayer::Update()
{
  if (fDone) return;
  if (!fClustersPtr) {
    cerr << ":::::: ERROR in JHitLayer::Update ::::::" << endl
	 << " Clusters not yet set! abort."            << endl;   
    ::abort();
  }	  
  if (!fClustersPtr->GetEntries()) {
    cerr << ":::::: WARNING in JHitLayer::Update ::::" << endl
	 << " No clusters in array! skip."             << endl;   
    return;
  }	  
  fDone = kTRUE;
  
  JCluster::SetSortBy(JCluster::kDet);
  fClustersPtr->Sort();
  TIter next(fClustersPtr);

  JCluster *pcp = static_cast<JCluster *>(next());
        fModule = pcp->GetModuleID();
        fLayer  = pcp->GetLayerID();
  JHit     *hp  = new JHit(pcp);
  JCluster *cp;
  while ((cp = static_cast<JCluster *>(next()))) {
    Int_t ppad    = pcp->GetPadID(); 
    Int_t ptstart = pcp->GetStartTime();
    Int_t plength = pcp->GetLength();
    Int_t ptend   = plength + ptstart;
 
    Int_t pad     = cp->GetPadID(); 
    Int_t tstart  = cp->GetStartTime();
    Int_t length  = cp->GetLength();
    Int_t tend    = length + tstart;
 
#if 1
    if ((ppad + 1) == pad &&
        (TMath::Max(ptend,tend) - TMath::Min(ptstart,tstart))
       	< (plength + length) ) {
#else
    static const double kTimeDiff = 4.;
    if ((ppad + 1) == pad &&
        TMath::Abs(tstart-ptstart)
       	< kTimeDiff ) {
#endif
      hp->Add(cp);
    } else {
      Add(hp);
      hp = new JHit(cp);
    }	    
    pcp = cp;
  }
  Add(hp);
}

void JHitLayer::Draw(const Char_t *opt)
{
  Int_t color = 10; 
  Draw(color, opt);
}

void JHitLayer::Draw(Int_t color, const Char_t *opt)
{
   if (!gPad)   return;
   if (!fDone) Update();
   gPad->cd();
   TIter next(this);
   JHit *hp;
   while ((hp = static_cast<JHit *>(next()))) {
      hp->Draw(color, opt);
   }
}

//--------------------------------------------------------------------------
//====================================
// JHitLayer::DistancetoPrimitive(...)
//====================================
// Object picker for 3D display
//
Int_t JHitLayer::DistancetoPrimitive(Int_t px, Int_t py)
{
  if (fTubsPtr) return fTubsPtr->DistancetoPrimitive(px,py);
  else          return 9999999;
}
