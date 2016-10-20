#include "JHelix.h"
#include "JPadGeom.h"

#include "TPad.h"
#include "TPolyLine3D.h"
#include "TString.h"
#include "TColor.h"

#include "TMath.h"
#include <cstdlib>
#include <iostream>
#include <iomanip>
using namespace std;

ClassImp(JHelix)

JHelix::JHelix(Double_t drho,   // drho  [cm]
               Double_t phi0,   // phi0
               Double_t rhoinv, // 1/rho [cm]
               Double_t dz,     // dz    [cm]
               Double_t tanl,   // tan(lambda)
               Double_t x0,     // X0
               Double_t y0,     // Y0
               Double_t z0,     // Z0
               Double_t phi1st, // phi first
               Double_t philst) // phi last
        : fDrho    (drho),     // drho
          fPhi0    (phi0),     // phi0
          fRhoInv  (rhoinv),   // Q/rho
	  fDz      (dz),       // dz
	  fTanL    (tanl),     // tan(lambda)
	  fPivot   (x0,y0,z0), // pivot
          fPhiFirst(phi1st),   // phi first
          fPhiLast (philst),   // phi last
	  fPl3dPtr (0)         // polyline 3D 
{
}

TVector3 JHelix::XvAt(Double_t phi)
{
  Double_t cosf0  = TMath::Cos(fPhi0);
  Double_t cosf0f = TMath::Cos(fPhi0 + phi);
  Double_t sinf0  = TMath::Sin(fPhi0);
  Double_t sinf0f = TMath::Sin(fPhi0 + phi);
  Double_t x = fPivot.X() + fDrho*cosf0
	      + (1/fRhoInv)*(cosf0 - cosf0f);
  Double_t y = fPivot.Y() + fDrho*sinf0
	      + (1/fRhoInv)*(sinf0 - sinf0f);
  Double_t z = fPivot.Z() + fDz
	      - (1/fRhoInv)*fTanL*phi;
  TVector3 xv(x,y,z);
  return xv;
}

void JHelix::DebugPrint(const Char_t * /* opt */)
{
  cerr << "(drho,phi0,kappa,dz,tanL)=("
       << fDrho << "," << fPhi0 << ","
       << fRhoInv << "," << fDz  << "," << fTanL << ") "
       << "pivot = (" << fPivot.X() << "," << fPivot.Y() << "," << fPivot.Z() << ")"
       << endl;
}

void JHelix::Draw(const Char_t *opt)
{
  static Int_t color = 1;
  color %= 20;
  color++;
  Draw(color, opt);
}

void JHelix::Draw(Int_t color, const Char_t *opt)
{
   if (!gPad)   return;
   gPad->cd();

   Int_t npts = 100;
   TPolyLine3D *pl3dp = new TPolyLine3D(npts);
   pl3dp->SetBit(kCanDelete);
#if 0
   pl3dp->SetLineStyle(1);
#endif

   fPl3dPtr = pl3dp;

   pl3dp->SetBit(TObject::kCannotPick);    // maker should not be pickable!
   JHelix *clonep = new JHelix(*this);     // must clone it since pickable
   clonep->AppendPad(opt);                 // will be killed by gPad

   pl3dp->SetLineColor(color);
   pl3dp->SetLineWidth(2);
   
   Double_t phi1st = fPhiFirst;
   Double_t dphi   = fPhiLast - fPhiFirst;
   const Double_t kDelta = 0.05;
            phi1st = - kDelta*dphi;
            dphi   = (fPhiLast + 2*kDelta*dphi - phi1st)/(npts-1);
   if (dphi == 0.) {
     Double_t l    = 17.; // [cm] track length to draw
     dphi = l*fRhoInv/npts; 
     dphi = fRhoInv > 0. ? -dphi : +dphi;
   }
   const Double_t kCmToMm = 10.;
   for (Int_t ipt=0; ipt<npts; ipt++) {
     Double_t phi = phi1st + ipt*dphi;
     TVector3 xv = XvAt(phi);
     pl3dp->SetPoint(ipt, kCmToMm*xv.X(), kCmToMm*xv.Y(), kCmToMm*xv.Z());
   }
   pl3dp->Draw();
   gPad->Update();
}

//--------------------------------------------------------------------------
//====================================
// JHelix::DistancetoPrimitive(...)
//====================================
// Object picker for 3D display
//
Int_t JHelix::DistancetoPrimitive(Int_t px, Int_t py)
{
  if (fPl3dPtr) return fPl3dPtr->DistancetoPrimitive(px,py);
  else          return 9999999;
}
