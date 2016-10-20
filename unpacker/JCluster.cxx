#include "JCluster.h"
#include "JChMapper.h"
#include "JMonEvent.h"
#include "JPadGeom.h"

#include "TObjArray.h"
#include "TPad.h"
#include "TPolyMarker3D.h"
#include "TVector3.h"
#include "TString.h"
#include "TColor.h"

#include <cstdlib>
#include <iomanip>

//JCluster::ESort  JCluster::fgSortBy = kEle;     //! sort type
JCluster::ESort  JCluster::fgSortBy = kDet;     //! sort type

//////////////////////////////////////////////////////////////////
#if 1
Int_t    JCluster::fgPre       = 1;      //! pre
Int_t    JCluster::fgPost      = 3;      //! post
#else
#if 0
Int_t    JCluster::fgPre       = 3;      //! pre
Int_t    JCluster::fgPost      = 7;      //! post
#else
Int_t    JCluster::fgPre       = 1;      //! pre
Int_t    JCluster::fgPost      = 7;      //! post
#endif
#endif
#if 1
Int_t    JCluster::fgThreshold = 6;     //! threshold
//Int_t    JCluster::fgThreshold = 5;     //! threshold
#else // 2012/06/28
Int_t    JCluster::fgThreshold = 3;     //! threshold
#endif
//Int_t    JCluster::fgThreshold = 20;     //! threshold
Double_t JCluster::fgVdrift    = 0.0768; //! drift velocity [mm/ns] Magboltz V8.5
Double_t JCluster::fgTbinWidth = 50.;    //! time bin width [ns]
//////////////////////////////////////////////////////////////////

ClassImp(JCluster)

JCluster::JCluster(CLUSTER *cp, TObjArray *ap)
        : fRCUID    (-1),     // rcu ID
          fChannelID(-1),     // channel ID
          fStartTime(-1),     // starting time
          fTime     (0.),     // starting time
          fPhMax    (-1),     // maximum pulse height
          fCharge   ( 0),     // charge
	  fModule   (-1),     // module ID
	  fLayer    (-1),     // layer ID
	  fPad      (-1),     // pad ID
	  fDone     (kFALSE), // update flag
	  fPm3dPtr  (0)       // update flag
{
  if (cp) {
    fRCUID     = cp->rcuid;
    fChannelID = cp->channel;
    fStartTime = cp->time - cp->length + 1;
    JChMapper::GetInstance()->GetPadAddress(fRCUID, fChannelID, fModule, fLayer, fPad);
    Bool_t start = kFALSE;
    static const Int_t kMax = 100;
    Int_t pre [kMax];
    Int_t npost = 0;
    Int_t imax  = -1;
    for (Int_t i=0; i<cp->length; i++) {
      Int_t ii  = cp->length-i-1;
      Int_t adc = cp->data[ii]; 
      pre[i%kMax] = adc;
      if (!start) {
        if (adc < fgThreshold) continue;
	else {
          start = kTRUE;
          for (Int_t j=fgPre; j>=0; j--) {
            if (ii+j >= cp->length) continue;
            fData.push_back(cp->data[ii+j]);
            fCharge += cp->data[ii+j];
	  }
	}
      } else {
        if (adc >= fgThreshold) {
	  if (ap && npost) {
	     CLUSTER nextc = *cp;
	     nextc.length = cp->length - i;
	     ap->Add(new JCluster(&nextc, ap));
	     break;
	  }
          fData.push_back(adc);
          fCharge += adc;
	} else {
          if (npost < fgPost) {
            fData.push_back(adc);
            fCharge += adc;
	  } else {
            break;
	  }
          npost++;
	}
      }
      if (adc > fPhMax) {
        fPhMax = adc;
	imax   = i;
      }
    }
#if 1
    Double_t qsum = 0;
//    for (Int_t i=imax-1; i<imax+2; i++) {
    for (Int_t i=imax-2; i<imax+3; i++) {
      Int_t ii  = cp->length-i-1;
      if (ii < 0 || ii >= cp->length) continue;
      fTime   += cp->data[ii]*(fStartTime+i);
      qsum    += cp->data[ii];
    }
    if (qsum) fTime /= qsum;
    else      fTime  = -1;
#else
    fTime = imax + fStartTime;
#endif
  }
}

Int_t JCluster::Compare(const TObject *obj) const
{
  const JCluster *cp = static_cast<const JCluster *>(obj);
  switch (fgSortBy) {
    case kEle:
         if (fRCUID < cp->fRCUID) return -1;
         if (fRCUID > cp->fRCUID) return +1;
         if (fRCUID == cp->fRCUID) {
           return fChannelID < cp->fChannelID ? -1 : (fChannelID > cp->fChannelID ? +1 : 0);
         }
    case kDet:
         if (fModule < cp->fModule) return -1;
         if (fModule > cp->fModule) return +1;
         if (fModule == cp->fModule) {
           if (fLayer < cp->fLayer) return -1;
           if (fLayer > cp->fLayer) return +1;
           return fPad < cp->fPad ? -1 : (fPad > cp->fPad ? +1 : 0);
         }
    case kTime:
	 return fStartTime < cp->fStartTime ? -1 : (fStartTime > cp->fStartTime ? +1 : 0);
    default:
         cerr << " Error::::: JCluster::Compare :::::::::::::::::" << endl
              << "   ESort = " << fgSortBy << " is not supported." << endl
	      << "   Abort!                                      " << endl;
	 ::abort();
  }
}

void JCluster::DebugPrint(const Char_t *opt)
{
  cerr << "(rcu,ch)=(" << fRCUID << "," << fChannelID << ") "
       << "(mod,lyr,pad)=(" << fModule << "," << fLayer << "," << fPad << ")"
       << " StartTime=" << fStartTime 
       << " Time="      << fTime 
       << " Phmax="     << fPhMax
       << " Q="         << fCharge
       << endl;
  TString opts(opt);
  opts.ToLower();
  if (opts.Contains("detail")) {
    cerr << "---" << endl;
    vector<Int_t>::iterator iter;
    for (iter=fData.begin(); iter!=fData.end(); iter++) {
      cerr << (*iter) << " ";
    }
    cerr << endl;
  }
}

TVector2 JCluster::GetPosition()
{
  if (!fDone) Update();
  return JPadGeom::GetInstance()->GetPosition(fModule,fXY);
}

TVector2 &JCluster::GetLocalPosition()
{
  if (!fDone) Update();
  return fXY;
}

void JCluster::Update()
{
  if (fDone) return;
  fDone = kTRUE;
  fXY = JPadGeom::GetInstance()->GetLocalPosition(fLayer,fPad);
}

void JCluster::Draw(const Char_t *opt)
{
  static const Float_t kNorm = 2000.;
  Float_t charge = fCharge/kNorm;
  Float_t zero   = 0.;
  Int_t color = TColor::GetColor(charge, zero, zero);
  Draw(color, opt);
}

void JCluster::Draw(Int_t color, const Char_t *opt)
{
   if (!gPad)   return;
   Int_t charge = GetCharge();
   if (!charge) return;
   gPad->cd();

   Int_t npts = 1;
   TPolyMarker3D *pm3dp = new TPolyMarker3D(npts);
   pm3dp->SetBit(kCanDelete);
   pm3dp->SetMarkerColor(color);
   pm3dp->SetMarkerStyle(20);

   fPm3dPtr = pm3dp;
   pm3dp->SetBit(TObject::kCannotPick);    // maker should not be pickable!
   JCluster *clonep = new JCluster(*this); // must clone it since pickable
   clonep->AppendPad(opt);                 // will be killed by gPad
#if 0
   pm3dp->SetMarkerSize (TMath::Min(0.001*charge,1.));
#else
   pm3dp->SetMarkerSize (0.4);
#endif
#if 0
   Double_t z = fgVdrift*fStartTime*fgTbinWidth;
#else
   Double_t z = fgVdrift*fTime*fgTbinWidth;
#endif

   if (!fDone) Update();
   TVector2 xy = JPadGeom::GetInstance()->GetPosition(fModule,fXY);
   TVector3 xv(xy.X(), xy.Y(), z);
   pm3dp->SetPoint(npts, xv.X(), xv.Y(), xv.Z());
#if 0
   cerr << "(X,Y,Z)=(" << setprecision(8)
        << xv.X() << ", "
        << xv.Y() << ", "
        << xv.Z() << ") " << endl;
#endif
   pm3dp->Draw();
   gPad->Update();
}

//--------------------------------------------------------------------------
//====================================
// JCluster::DistancetoPrimitive(...)
//====================================
// Object picker for 3D display
//
Int_t JCluster::DistancetoPrimitive(Int_t px, Int_t py)
{
  if (fPm3dPtr) return fPm3dPtr->DistancetoPrimitive(px,py);
  else          return 9999999;
}
