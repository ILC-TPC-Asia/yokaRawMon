#include "JHitCorrector.h"

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

JHitCorrector *JHitCorrector::fgInstancePtr = 0;
double         JHitCorrector::fgDataDX[JHitCorrector::kNb][JHitCorrector::kNm][JHitCorrector::kNl];

JHitCorrector::JHitCorrector(const char *delxfile)
{
   stringstream filename;
   if (delxfile) filename << delxfile             << ends;
   else          filename << "Results/deltax0.dat" << ends;
   ifstream in(filename.str().data());
   static double kmm2cm = 0.1;
   int np = 0;
   for (int b=1; b<kNb; b++) {
      for (int m=0; m<kNm; m++) {
         if (m != 0 && m != 3 && m!= 5) continue;
         for (int l=0; l<kNl; l++) {
	    int    ls;
            double dx;
            in >> ls >> dx;
            fgDataDX[b][m][l] = -dx*kmm2cm;
#if 1
            cerr << "np=" << np++ << " m=" << m << " l=" << l << " dx=" << fgDataDX[b][m][l] << endl;
#endif
         }    
      }
   }
}

JHitCorrector::~JHitCorrector()
{
}

JHitCorrector *JHitCorrector::GetInstancePtr(const char *delxfile)
{
   if (!fgInstancePtr) {
      fgInstancePtr = new JHitCorrector(delxfile);
   }
   return fgInstancePtr;
}

double JHitCorrector::GetDeltaX(double x,
                                int    layer,
                                int    module,
                                int    run)
{
   int b  = 1;
   return fgDataDX[b][module][layer];
}

void JHitCorrector::DebugPrint(const char *opt)
{
}
