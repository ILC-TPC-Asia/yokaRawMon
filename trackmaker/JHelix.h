#ifndef JHELIX_H
#define JHELIX_H

#include "TObject.h"
#include "TVector3.h"

class TPolyLine3D;

class JHelix : public TObject {
public:
  JHelix(Double_t drho   = 0.,    // drho  [cm]
         Double_t phi0   = 0.,    // phi0
         Double_t rhoinv = 1.e-3, // 1/rho [cm]
         Double_t dz     = 0.,    // dz    [cm]
         Double_t tanl   = 0.,    // tan(lambda)
         Double_t x0     = 0.,    // X0
         Double_t y0     = 0.,    // Y0
         Double_t z0     = 0.,    // Z0
         Double_t phi1st = 0.,    // phi first
         Double_t philst = 0.);   // phi last

  inline Double_t GetDrho    () { return fDrho;     }
  inline Double_t GetPhi0    () { return fPhi0;     }
  inline Double_t GetRhoInv  () { return fRhoInv;   }
  inline Double_t GetDz      () { return fDz;       }
  inline Double_t GetTanL    () { return fTanL;     }
  inline TVector3 GetPivot   () { return fPivot;    }
  inline Double_t GetPhiFirst() { return fPhiFirst; }
  inline Double_t GetPhiLast () { return fPhiLast;  }

  inline void SetHelixParameters(Double_t drho,
                                 Double_t phi0,
                                 Double_t rhoinv,
                                 Double_t dz,
                                 Double_t tanl,
                                 Double_t x0,
                                 Double_t y0,
                                 Double_t z0,
                                 Double_t phi1st = 0.,
                                 Double_t philst = 0.)
  {
    fDrho   = drho;
    fPhi0   = phi0;
    fRhoInv = rhoinv;
    fDz     = dz;
    fTanL   = tanl;
    fPivot.SetXYZ(x0,y0,z0);
    fPhiFirst = phi1st;
    fPhiLast  = philst;
  }

  TVector3 XvAt(Double_t phi);

  virtual void DebugPrint(const Char_t *opt = "");

  virtual void Draw(const Char_t *opt="");
  virtual void Draw(Int_t  color, const Char_t *opt="");

  Int_t DistancetoPrimitive(Int_t px, Int_t py); // object picker

private:
  Double_t        fDrho;       // drho
  Double_t        fPhi0;       // phi0
  Double_t        fRhoInv;     // Q/rho
  Double_t        fDz;         // dz 
  Double_t        fTanL;       // tan(lambda)
  TVector3        fPivot;      // pivot 
  Double_t        fPhiFirst;   // start phi
  Double_t        fPhiLast;    // end phi
  TPolyLine3D    *fPl3dPtr;    //! graphical track

  ClassDef(JHelix, 1)  // Raw Data class
};
#endif
