#include "JTripletTrackFinder.h"
#include "JHitLayer.h"
#include "JPadGeom.h"
#include "JTrack.h"
#include "JTrackMaker.h"
#include "JLock.h"

#include "TMath.h"

//#define TIME_OVERLAP

Int_t    JTripletTrackFinder::fgMinNlayers  =  10;
Int_t    JTripletTrackFinder::fgMinNhits    =   4;
Int_t    JTripletTrackFinder::fgMaxMissing  =   4;
Double_t JTripletTrackFinder::fgCosThetaCut = 0.95;
Double_t JTripletTrackFinder::fgZdiffCut    = 3.0;

ClassImp(JTripletTrackFinder)

JTripletTrackFinder::JTripletTrackFinder(JTrackMaker *tmp)
                  : JVTrackFinder(tmp)
{
#if 1
  cerr << " ::::::: JTripletTrackFinder Created!" << endl;
#endif
}

JTripletTrackFinder::~JTripletTrackFinder()
{
}

void JTripletTrackFinder::FindTracks(TObjArray *hitlayersptr)
{
#if 1
  cerr << " FindTracks called " << endl;
#endif
  //--
  // Check if we have enough numbers of layers
  //--
#if 1
  Int_t nTracks = 0;
#endif
  if (hitlayersptr->GetEntries() < fgMinNlayers) return; 
  JHitLayer::SetSortBy(JHitLayer::kDet);
  hitlayersptr->Sort();
  //--
  // Loop over layers to find tracks
  //--
  Int_t moduletolookat = GetTrackMakerPtr()->GetModuleToLookAt();
  Int_t layertolookat  = GetTrackMakerPtr()->GetLayerToLookAt();
  TIter nextlayer1(hitlayersptr);
  JHitLayer *l1p;
  while ((l1p = static_cast<JHitLayer *>(nextlayer1()))) {
    TIter nexthit(l1p);
    JHit *h1p;
    while ((h1p = static_cast<JHit *>(nexthit()))) { // 1st hit
      if (h1p->TestBit(BIT(kUsed))) continue;
      Int_t missing    = 0;
      Int_t lastmodule = -1;
      JTrack *curtrkptr = new JTrack(h1p);
      TIter nextlayer(nextlayer1);
      JHitLayer *lp;
      while ((lp = static_cast<JHitLayer *>(nextlayer()))) {
        Int_t module = lp->GetModuleID();
        Int_t layer  = lp->GetLayerID();
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
#if 0
        if (!ok && ( module != moduletolookat || layer != layertolookat ))  missing++;
#else
        if (!ok && ( module != moduletolookat || layer != layertolookat )) {
          if (curtrkptr->GetEntries() > 50) {
            cerr << "missing: module=" << module << " layer=" << layer << " missing=" << missing << endl;
          }
          missing++;
        }
#endif
        if (missing > fgMaxMissing) {
          if (curtrkptr->GetEntries() <= fgMinNhits) {
#if 0
            cerr << "(Nhits,Nmis)=(" << curtrkptr->GetEntries() << "," << missing << ")" << endl;
#endif
            delete curtrkptr;
            curtrkptr = 0;
          } 
#if 1
	  if (curtrkptr) {
            cerr << "(Nhits,Nmis)=(" << curtrkptr->GetEntries() << "," << missing << ")" << endl;
	  }
#endif
          if (module != lastmodule && ok) missing = 0;
          else                            break;
        }
      }
      if (curtrkptr && curtrkptr->GetEntries() > fgMinNhits) {
#if 1
	cerr << "track #" << nTracks << ": nhits = " << curtrkptr->GetEntries() << endl;
        nTracks++;
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
  cerr << "nTracks = " << nTracks << endl;
#endif
}

Bool_t JTripletTrackFinder::AppendHit (JTrack *tp, JHit *hp)
{
  if (tp->GetEntries() < 2) {
    return AppendHitLoc(tp, hp);
  } else {
#if 0
    JHit *prevh1p = static_cast<JHit *>(tp->At(tp->GetLast()-1));
#else
    JHit *prevh1p = static_cast<JHit *>(tp->At(0));
#endif
    JHit *prevh2p = static_cast<JHit *>(tp->At(tp->GetLast()));
    TVector3 xv1 = prevh1p->GetPosition();
    TVector3 xv2 = prevh2p->GetPosition();
    TVector3 xv  = hp->GetPosition();
    TVector2 xy1(xv1.X(),xv1.Y()); // projection to XY plane
    TVector2 xy2(xv2.X(),xv2.Y()); // projection to XY plane
    TVector2 xy(xv.X(),xv.Y());    // projection to XY plane
    Double_t cs = (xy2 - xy1).Unit() * (xy - xy2).Unit(); // cos theta
    if (hp->GetModuleID() == prevh2p->GetModuleID() &&
        TMath::Abs(hp->GetLayerID() - prevh2p->GetLayerID()) <= 2) {
      return AppendHitLoc(tp, hp);
    } else {
      if (TMath::Abs(cs) > fgCosThetaCut) { 
        tp->Add(hp);
        return kTRUE;
      } else {
        return kFALSE;
      }
    }
  }
}

Bool_t JTripletTrackFinder::AppendHitLoc(JTrack *tp, JHit *hp)
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
#ifdef TIME_OVERLAP
    Double_t lasttw   = lastclp->GetLength();
    Double_t lastt0   = lastclp->GetStartTime();
    Double_t lastt1   = lastt0 + lasttw - 1;
#else
    Double_t lastz    = lasthp->GetPosition().Z();
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
#ifdef TIME_OVERLAP
    Double_t tw   = clp->GetLength();
    Double_t t0   = clp->GetStartTime();
    Double_t t1   = t0 + tw - 1;
    if ((TMath::Max(lastfir,fir)-TMath::Min(lastfil,fil)) < (lastw + w) &&
        (TMath::Max(lastt1,t1)-TMath::Min(lastt0,t0)) <  (lasttw + tw)) {
#else
    //static Double_t kEps = 0.0;
    //static Double_t kEps = 0.1;
    static Double_t kEps = 0.2;
    //static Double_t kEps = 0.3;
    Double_t z    = hp->GetPosition().Z();
    if ((TMath::Max(lastfir,fir)-TMath::Min(lastfil,fil)) < (lastw + w)*(1+kEps)
         && TMath::Abs(z-lastz) < fgZdiffCut) {
#endif
      tp->Add(hp);
      return kTRUE;
    } else {
      return kFALSE;
    }
  }
}
