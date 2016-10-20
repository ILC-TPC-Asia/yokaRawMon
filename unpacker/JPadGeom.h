#ifndef JPADGEOM_H
#define JPADGEOM_H
#include "TVector2.h"
#include <string>

class TVector2;

class JPadGeom {
  private:
    JPadGeom(); // JPadGeom is a singleton!

  public:
    ~JPadGeom();

    TVector2 GetPosition     (int module, int layer, int pad);
    TVector2 GetPosition     (int module, const TVector2 &xv);
    TVector2 GetLocalPosition(int layer, int pad);
    static Int_t    GetNpads        (int seg);
    static Double_t GetDphi         (int seg);
    static Int_t    GetNrow         () { return fgNrow;   }
    static Double_t GetDr           () { return fgDr;     }
    static Double_t GetRmin         () { return fgRmin;   }
    static Double_t GetGap          () { return fgGap;    }
    static Double_t GetPhiMin       (int module);
    static Double_t GetPhiMax       (int module);
    static TVector2 GetXc           (int module);
    static Double_t GetR            (int layer);
    static Double_t GetLength       () { return fgLength; }

    static JPadGeom *GetInstance();

  private:
    static JPadGeom    *fgInstancePtr;    // the singleton pointer
    static std::string  fgGeomFile;       // geometry file
    static Double_t     fgPhi0[7];        // Phi0
    static Double_t     fgR0  [7];        // R0
    static Int_t        fgNpad[2];        // # pads per row
    static Double_t     fgDphi[2];        // delta phi per pad
    static Int_t        fgNrow;           // # pad rows per module
    static Double_t     fgDr;             // delta r per pad row
    static Double_t     fgRmin;           // Radius of the innermost pad row
    static Double_t     fgGap;            // pad to pad gap 
    static Double_t     fgLength;         // maximum drift length
};
#endif
