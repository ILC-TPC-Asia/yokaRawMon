#include "JPadGeom.h"

#include "TMath.h"

#include <iostream>
#include <cstdlib>

JPadGeom   *JPadGeom::fgInstancePtr = 0;
std::string JPadGeom::fgGeomFile    = "geomfile.dat";
//module numbering from pad surface
//       outer
//       5     6
//    2     3     4
//        0     1
#if 0
Double_t JPadGeom::fgPhi0[] = {0.12485582, -0.021577304,
                               0.2208493 , 0.07441581, -0.07184279,
                               0.15976241, 0.013329281 };
Double_t JPadGeom::fgR0  [] = {1676.115 , 1676.115,
                               1503.615 , 1503.615, 1503.615,
                               1332.000 , 1332.000 };
//                            ---------------------------
//                            innner half    outer half
//                             ---------------------------
Int_t    JPadGeom::fgNpad[] = {        176,         192}; // # pads per row
//                            ---------------------------
//                            innner half    outer half
//                            ---------------------------
Double_t JPadGeom::fgDphi[] = {0.000821484, 0.000753027}; // delta phi per pad

Int_t    JPadGeom::fgNrow   =      28;
Double_t JPadGeom::fgDr     =    5.36;
Double_t JPadGeom::fgRmin   = 1440.01;
Double_t JPadGeom::fgGap    =    0.10;
Double_t JPadGeom::fgLength =  600.00;
#else
//2012/12/16
//      look from outside 
//       6     5
//    4     3     2
//        1     0
//
#if 0
Double_t JPadGeom::fgPhi0[] = {0.12557643968099, -0.02085668456134,
                               0.22156954854069,  0.07513642429837, -0.07112216701877,
                               0.16048302472087,  0.01404990047855};
#else
#if 0
Double_t JPadGeom::fgPhi0[] = {0.12548480989526, -0.020948314347062,
                               0.22147791875495,  0.075044794512626, -0.071213796804499,
                               0.16039139493515,  0.013958270692825};
#else
Double_t JPadGeom::fgPhi0  [] = {0.12555020738233, -0.020830452262675,
                                 0.22149085164471, 0.075110191999698, -0.071270467645311,
                                 0.16045679242222, 0.014076132777212};
#endif
#endif
Double_t JPadGeom::fgR0  [] = {1676.115 , 1676.115,
                               1503.615 , 1503.615, 1503.615,
                               1332.000 , 1332.000 };
//                            ---------------------------
//                            innner half    outer half
//                             ---------------------------
Int_t    JPadGeom::fgNpad[] = {        176,         192}; // # pads per row
//                            ---------------------------
//                            innner half    outer half
//                            ---------------------------
//Double_t JPadGeom::fgDphi[] = {0.000821484, 0.000753027}; // delta phi per pad
//Double_t JPadGeom::fgDphi[] = {0.00082296704149, 0.00075763327885}; // delta phi per pad
//Double_t JPadGeom::fgDphi[] = {0.00082390388954424, 0.00075578323952206}; // delta phi per pad
Double_t JPadGeom::fgDphi[] = {0.00082390388954424, 0.00075576971366224}; // delta phi per pad

Int_t    JPadGeom::fgNrow   =      28;
//Double_t JPadGeom::fgDr     =   5.3543197516891; 
Double_t JPadGeom::fgDr     = 5.3570988356489;
Double_t JPadGeom::fgRmin   = 1440.;
Double_t JPadGeom::fgGap    =    0.10;
Double_t JPadGeom::fgLength =  600.00;
#endif

JPadGeom::JPadGeom()
{
}

JPadGeom::~JPadGeom()
{
}

JPadGeom *JPadGeom::GetInstance()
{
  if (!fgInstancePtr) fgInstancePtr = new JPadGeom;
  return fgInstancePtr;
}

Int_t JPadGeom::GetNpads(Int_t seg)
{
  using namespace std;
  if (seg >= 0 && seg < 2) return fgNpad[seg];
  else {
    cerr << ":::: ERROR: JPadGeom::GetNpad :::::::::::::::::::::::::" << endl
         << " Bad segment = " << seg                                  << endl
         << " Abort!"                                                 << endl;
    ::abort();
  }
}

Double_t JPadGeom::GetDphi(Int_t seg)
{
  using namespace std;
  if (seg >= 0 && seg < 2) return fgDphi[seg];
  else {
    cerr << ":::: ERROR: JPadGeom::GetDphi :::::::::::::::::::::::::" << endl
         << " Bad segment = " << seg                                  << endl
         << " Abort!"                                                 << endl;
    ::abort();
  }
}

Double_t JPadGeom::GetPhiMin(Int_t module)
{
#if 1
  return - fgPhi0[module];
#else
  return TMath::Pi()/2 - fgPhi0[module];
#endif
}

Double_t JPadGeom::GetPhiMax(Int_t module)
{
#if 1
  return - fgPhi0[module] + fgNpad[0]*fgDphi[0];
#else
  return TMath::Pi()/2 - fgPhi0[module] + fgNpad[0]*fgDphi[0];
#endif
}

TVector2 JPadGeom::GetXc(Int_t module)
{
  return  TVector2(0.,-fgR0[module]);
}

Double_t JPadGeom::GetR(Int_t layer)
{
  return fgRmin + fgDr*(layer+0.5) - 0.5*fgGap;
}

TVector2 JPadGeom::GetLocalPosition(int layer, int pad)
{
  using namespace std;

  int iseg = layer < fgNrow/2 ? 0 : 1;
  if( pad >= GetNpads(iseg) || layer >= fgNrow) {
    cerr << ":::: ERROR: JPadGeom::GetPosition :::::::::::::::::::::" << endl
         << " Bad pad address: layer = " << layer << " pad = " << pad << endl
         << " Abort!"                                                 << endl;
    ::abort();
  }

  double r      = fgRmin + fgDr*(layer+0.5) - 0.5*fgGap;
  double offset = layer%2 ? 0.5 : 0.;
#if 0
  TVector2 xv(-0.5*fgGap, r);
  double phi = GetDphi(iseg)*(pad + offset + 0.5);
#else
#if 1
  TVector2 xv(0., r);
  double phi = GetDphi(iseg)*(pad + offset + 0.5) + 0.5*fgGap/r;
#else /* 20120610 KF */
  TVector2 xv(0., r);
  double phi = GetDphi(iseg)*(pad + offset + 0.5) + 0.5*fgGap/fgRmin;
#endif
#endif
  xv = xv.Rotate(phi);

  return xv;
}

TVector2 JPadGeom::GetPosition(int module, int layer, int pad)
{
  TVector2 xv = GetLocalPosition(layer, pad);
  return GetPosition(module, xv);
}

TVector2 JPadGeom::GetPosition(int module, const TVector2 &xv)
{
  return xv.Rotate(-fgPhi0[module]) - TVector2(0.,fgR0[module]);
}
