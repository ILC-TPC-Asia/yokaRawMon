#include "JSimpleTrackFinder.h"
#include "JHitLayer.h"
#include "JPadGeom.h"
#include "JTrack.h"
#include "JTrackMaker.h"
#include "JLock.h"

#include "TMath.h"

ClassImp(JSimpleTrackFinder)

JSimpleTrackFinder::JSimpleTrackFinder(JTrackMaker *tmp)
                  : JVTrackFinder(tmp)
{
}

JSimpleTrackFinder::~JSimpleTrackFinder()
{
}

void JSimpleTrackFinder::FindTracks(TObjArray *hitlayersptr)
{
#if 1
  cerr << " FindTracks called " << endl;
#endif
  //--
  // Check if we have enough numbers of layers
  //--
  /////// VERY TEMPORARY TREATMENT /////////
  static const Int_t kMinNlayers = 10;
  static const Int_t kMinNhits   =  4;
  static const Int_t kMaxMissing =  4;
  //////////////////////////////////////////
#if 1
  Int_t kNTracks = 0;
#endif
  if (hitlayersptr->GetEntries() < kMinNlayers) return; 
  JHitLayer::SetSortBy(JHitLayer::kDet);
  hitlayersptr->Sort();
  //--
  // Loop over layers to find tracks
  //--
  Int_t layertolookat = GetTrackMakerPtr()->GetLayerToLookAt();
  TIter nextlayer1(hitlayersptr);
  JHitLayer *l1p;
  while ((l1p = static_cast<JHitLayer *>(nextlayer1()))) {
    TIter nexthit(l1p);
    JHit *h1p;
    while ((h1p = static_cast<JHit *>(nexthit()))) { // 1st hit
      Int_t missing = 0;
      JTrack *curtrkptr = new JTrack(h1p);
      TIter nextlayer(nextlayer1);
      nextlayer();
      JHitLayer *lp;
      while ((lp = static_cast<JHitLayer *>(nextlayer()))) {
        Int_t layer = lp->GetLayerID();
        Int_t ok = 0;
        TIter next(lp);
        JHit *hp;
        while ((hp = static_cast<JHit *>(next()))) {
          if (hp->TestBit(BIT(kUsed))) continue;
          // if hp touches the last, add hp to the current track
          if (curtrkptr->Append(hp)) {
            ok = 1;
            break;
          }
        }
        if (!ok && layer != layertolookat) {
          missing++;
          if (missing > kMaxMissing) {
            delete curtrkptr;
	    curtrkptr = 0;
            break;
          }
        }
      }
      if (curtrkptr && curtrkptr->GetEntries()>kMinNhits) {
#if 1
        kNTracks++;
#endif
        GetTrackMakerPtr()->Add(curtrkptr);
        TIter next(curtrkptr);
        JHit *hp;
        while ((hp = static_cast<JHit *>(next()))) {
          hp->SetBit(BIT(kUsed),kTRUE);
        }
      } else {
        delete curtrkptr;
      }
    }
  }
#if 1
  cerr << "kNTracks = " << kNTracks << endl;
#endif
}

Bool_t JSimpleTrackFinder::AppendHit (JTrack *tp, JHit *hp)
{
  if (!tp->GetEntries()) {
    tp->Add(hp);
    return kTRUE;
  } else {
    JHit *lasthp = static_cast<JHit *>(tp->Last());
    JCluster *lastclp = static_cast<JCluster *>(lasthp->At(0));
    JCluster *lastcrp = static_cast<JCluster *>(lasthp->Last());
    Int_t lastlayer   = lastclp->GetLayerID();
    Int_t lastpadl    = lastclp->GetPadID();
    Int_t lastpadr    = lastcrp->GetPadID();
    TVector2 lastxyl  = JPadGeom::GetInstance()
                        ->GetLocalPosition(lastlayer, lastpadl);
    TVector2 lastxyr  = JPadGeom::GetInstance()
                        ->GetLocalPosition(lastlayer, lastpadr);
    Double_t lastfil  = TMath::ATan2(lastxyl.Y(),lastxyl.X());
    Double_t lastfir  = TMath::ATan2(lastxyr.Y(),lastxyr.X());
    Double_t lastw    = lastfir - lastfil;
#if 0
    cerr << "---" << endl;
    cerr << " lastfil = " << lastfil << " lastfir = " << lastfir << endl;
#endif

    JCluster *clp = static_cast<JCluster *>(hp->At(0));
    JCluster *crp = static_cast<JCluster *>(hp->Last());
    Int_t layer   = clp->GetLayerID();
    Int_t padl    = clp->GetPadID();
    Int_t padr    = crp->GetPadID();
    TVector2 xyl  = JPadGeom::GetInstance()
                    ->GetLocalPosition(layer, padl);
    TVector2 xyr  = JPadGeom::GetInstance()
                    ->GetLocalPosition(layer, padr);
    Double_t fil  = TMath::ATan2(xyl.Y(),xyl.X());
    Double_t fir  = TMath::ATan2(xyr.Y(),xyr.X());
    Double_t w    = fir - fil;
    if ((TMath::Max(lastfir,fir)-TMath::Min(lastfil,fil)) 
        < (lastw + w)) {
      tp->Add(hp);
      return kTRUE;
    } else {
      return kFALSE;
    }
  }
}
