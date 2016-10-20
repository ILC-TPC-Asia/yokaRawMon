//// VERY TEMPORARY ///
//#define AVOID_EDGE
///////////////////////
#include "JHit.h"
#include "JCluster.h"
#include "JPadGeom.h"

#include "TPad.h"
#include "TPolyMarker3D.h"
#include "TString.h"
#include "TColor.h"

#include <cstdlib>
#include <iomanip>
#if 1
JHit::ESort  JHit::fgSortBy = kPad;     //! sort type
#endif
#ifdef AVOID_EDGE
#include "JLock.h"
#endif

ClassImp(JHit)

JHit::JHit(JCluster *cp)
        : fTime     (0.),     // starting time
          fPhMax    (-1),     // maximum pulse height
          fCharge   ( 0),     // charge
	  fModule   (-1),     // module ID
	  fLayer    (-1),     // layer ID
	  fPad      (-1),     // pad ID
	  fDone     (kFALSE), // update flag
	  fPm3dPtr  (0)       // update flag
{
  if (cp) {
    fModule = cp->GetModuleID();
    fLayer  = cp->GetLayerID();
    Add(cp);
#ifdef AVOID_EDGE
    //static const Int_t kEdge = 4;
    static const Int_t kEdge = 6;
    if ((fLayer < kEdge) || (fLayer >= (JPadGeom::GetNrow() - kEdge))) {
      SetBit(BIT(kUsed),kTRUE);
    }
#endif
  }
}

JHit::JHit(const JHit &h)
        : // TObjArray(h),
          fTime     (h.fTime),   // starting time
          fPhMax    (h.fPhMax),  // maximum pulse height
          fCharge   (h.fCharge), // charge
	  fModule   (h.fModule), // module ID
	  fLayer    (h.fLayer),  // layer ID
	  fPad      (h.fPad),    // pad ID
	  fDone     (h.fDone),   // update flag
	  fPm3dPtr  (0)          // update flag
{
  SetOwner(kFALSE);
}

JHit::~JHit()
{
  if (IsOwner()) Clear();
}

#if 1
Int_t JHit::Compare(const TObject *obj) const
{
  const JHit *cp = static_cast<const JHit *>(obj);
  switch (fgSortBy) {
    case kPad:
	 return fPad < cp->fPad ? -1 : (fPad > cp->fPad ? +1 : 0);
    case kTime:
	 return fTime < cp->fTime ? -1 : (fTime > cp->fTime ? +1 : 0);
    default:
         cerr << " Error::::: JHit::Compare :::::::::::::::::" << endl
              << "   ESort = " << fgSortBy << " is not supported." << endl
	      << "   Abort!                                      " << endl;
	 ::abort();
  }
}
#endif

void JHit::DebugPrint(const Char_t *opt)
{
  if (!fDone) Update();
  static const Double_t kmm2cm = 0.1;
#if 0
  TVector3 xv = kmm2cm * fXYZ;
#else
  TVector3 xv = kmm2cm * GetPosition();
#endif
  cerr << "(mod,lyr,pad)=(" 
       << fModule   << "," 
       << fLayer    << "," 
       << fPad      << ")"
       << " Time="  << fTime 
       << " Phmax=" << fPhMax 
       << " Q="     << fCharge 
       << " (x,y,z)=("
       << xv.X()    << ","
       << xv.Y()    << ","
       << xv.Z()    << ")"
       << endl;
  TString opts(opt);
  opts.ToLower();
  if (opts.Contains("detailed")) {
    cerr << "---" << endl;
    TIter next(this);
    JCluster *cp;
    while ((cp = static_cast<JCluster *>(next()))) {
      cp->DebugPrint(opt);
    }
  }
}

TVector3 &JHit::GetLocalPosition()
{
  if (!fDone) Update();
  return fXYZ;
}

TVector3 JHit::GetPosition()
{
  if (!fDone) Update();
  TVector2 xy(fXYZ.X(),fXYZ.Y());
  xy = JPadGeom::GetInstance()->GetPosition(fModule,xy);
#if 1
  return TVector3(xy.X(),xy.Y(),fXYZ.Z());
#else
  Double_t z = JPadGeom::GetLength() - fXYZ.Z();
  return TVector3(-xy.X(),xy.Y(),z);
#endif
}

void JHit::Update()
{
  if (fDone) return;
  if (!GetEntries()) {
    cerr << ":::::: ERROR in JHit::Update :::::::" << endl
         << " No clusters set! abort! "            << endl;
    ::abort();
  }
  fDone = kTRUE;
  Double_t x = 0;
  Double_t y = 0;
  JCluster *pkcp = 0;
  TIter next(this);
  JCluster *cp;
  while ((cp = static_cast<JCluster *>(next()))) {
#if 1
    Double_t q = cp->GetCharge();
#else
    static Double_t kPed = 0.5;
    Double_t q = cp->GetCharge() - cp->GetLength()*kPed;
#endif
    x       += q * cp->GetLocalPosition().X();
    y       += q * cp->GetLocalPosition().Y();
    fTime   += q * cp->GetTime();
    fCharge += q;
    if (cp->GetMaximum() > fPhMax) {
      fPhMax = cp->GetMaximum();
      pkcp   = cp;
    }
  }
  fTime /= fCharge;
  x     /= fCharge;
  y     /= fCharge;

  fModule = pkcp->GetModuleID();
  fLayer  = pkcp->GetLayerID();
  fPad    = pkcp->GetPadID();

  Double_t vd    = JCluster::GetVdrift();
  Double_t tbinw = JCluster::GetTbinWidth();
  Double_t z     = vd*fTime*tbinw;

  fXYZ.SetXYZ(x,y,z);
}

void JHit::Draw(const Char_t *opt)
{
  static const Float_t kNorm = 8000.;
  Float_t charge = fCharge/kNorm;
  Float_t zero   = 0.;
  Int_t color = TColor::GetColor(charge, zero, zero);
  Draw(color, opt);
}

void JHit::Draw(Int_t color, const Char_t *opt)
{
   if (!gPad)   return;
   if (!fDone) Update();
   Int_t charge = GetCharge();
   if (!charge) return;
   gPad->cd();

   Int_t npts = 1;
   TPolyMarker3D *pm3dp = new TPolyMarker3D(npts);
   pm3dp->SetBit(kCanDelete);
   pm3dp->SetMarkerColor(color);
   pm3dp->SetMarkerStyle(20);

   fPm3dPtr = pm3dp;
   pm3dp->SetBit(TObject::kCannotPick);    // marker should not be pickable!
   JHit *clonep = new JHit(*this);         // must clone it since pickable
   clonep->AppendPad(opt);                 // will be killed by gPad
#if 0
   pm3dp->SetMarkerSize (TMath::Min(0.001*charge,1.));
#else
   pm3dp->SetMarkerSize (0.4);
#endif
   TVector2 xy(fXYZ.X(),fXYZ.Y());
   xy = JPadGeom::GetInstance()->GetPosition(fModule,xy);
   TVector3 xv(xy.X(), xy.Y(), fXYZ.Z());
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
// JHit::DistancetoPrimitive(...)
//====================================
// Object picker for 3D display
//
Int_t JHit::DistancetoPrimitive(Int_t px, Int_t py)
{
  if (fPm3dPtr) return fPm3dPtr->DistancetoPrimitive(px,py);
  else          return 9999999;
}
