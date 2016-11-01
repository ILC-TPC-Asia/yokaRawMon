#ifndef Runinfo_H
#define Runinfo_H
#include <iostream>

class Runinfo {
public:
  ~Runinfo()
  {
    if (kDlength) delete [] kDlength;
    if (kBfield)  delete [] kBfield;
  }

  inline double GetDlength(int run) const { return run < kNruns ? kDlength[run] : 1.e10; } 
  inline int GetBfield (int run) const { return run < kNruns ? kBfield [run] : 1.e10; } 
  inline double GetDLOffset(int run) const { return kDLoff; }

  static Runinfo *GetInstancePtr()
  {
    if (!gInstancePtr) gInstancePtr = new Runinfo;
    return gInstancePtr;
  }

private:
#if 0
  Runinfo() : kNruns(100000), kDlength(0), kBfield(0), kDLoff(2.8)
#else
  //Runinfo() : kNruns(100000), kDlength(0), kBfield(0), kDLoff(-0.6)
  Runinfo() : kNruns(100000), kDlength(0), kBfield(0), kDLoff(-1.0)
#endif
  {
    kDlength = new double[kNruns];
    kBfield  = new int[kNruns];
    for (Int_t i=0; i<kNruns; i++) {
      kDlength[i] = 0;
      kBfield [i] = 0;
    }
    kDlength[6953] = 10;
    kDlength[6957] = 15;
    kDlength[6958] = 20;
    kDlength[6972] = 25;
    kDlength[6973] = 30;
    kDlength[6974] = 35;
    kDlength[6975] = 40;
    kDlength[6976] = 45;
    kDlength[6977] = 50;

    kDlength[7049] = 10;
    kDlength[7046] = 15;
    kDlength[7051] = 20;
    kDlength[7052] = 25;
    kDlength[7053] = 25;
    kDlength[7059] = 30;
    kDlength[7061] = 35;
    kDlength[7065] = 40;
    kDlength[7020] = 45;
    kDlength[7023] = 50;

    kDlength[17319] =  5;
    kDlength[17324] = 15;
    kDlength[17327] = 25;
    kDlength[17331] = 35;
    kDlength[17340] = 45;
    kDlength[17344] = 55;
    kDlength[17352] = 50;
    kDlength[17356] = 40;
    kDlength[17359] = 30;
    kDlength[17364] = 20;
    kDlength[17367] = 10;

    kDlength[18241] = 2.5;
    kDlength[18238] = 5;
    kDlength[18218] = 7.5;
    kDlength[18221] = 10;
    kDlength[18224] = 15;
    kDlength[18226] = 20;
    kDlength[18228] = 25;
    kDlength[18230] = 30;
    kDlength[18233] = 40;
    kDlength[18243] = 50;

    kDlength[18412] = 3.5;
    kDlength[18413] = 6;
    kDlength[18414] = 7.5;
    kDlength[18415] = 10;
    kDlength[18417] = 12.5;
    kDlength[18418] = 15;
    kDlength[18419] = 20;
    kDlength[18420] = 25;
    kDlength[18421] = 30;
    kDlength[18423] = 40;
    kDlength[18424] = 50;

    kDlength[18564] = 5;
    kDlength[18563] = 6.5;
    kDlength[18562] = 9;
    kDlength[18561] = 11.5;
    kDlength[18552] = 14;
    kDlength[18551] = 19;
    kDlength[18550] = 24;
    kDlength[18549] = 29;
    kDlength[18548] = 39;
    kDlength[18547] = 49;
    
    kDlength[18609] = 2.5;
    kDlength[18606] = 5;
    kDlength[18605] = 7.5;
    kDlength[18601] = 10;
    kDlength[18600] = 12.5;
    kDlength[18598] = 15;
    kDlength[18597] = 20;
    kDlength[18595] = 25;
    kDlength[18594] = 30;
    kDlength[18592] = 40;
    kDlength[18591] = 50;

    kDlength[19611] = 25; //temporary
    kDlength[19616] = 10; //temporary
   
    kDlength[19636] = 23.5; //temporary
    kDlength[19637] = 28.5; //temporary
    kDlength[19638] = 33.5; //temporary
    kDlength[19639] = 38.5; //temporary
    kDlength[19640] = 43.5; //temporary
    kDlength[19641] = 48.5; //temporary
    kDlength[19643] = 55; //temporary

    kBfield[7049] = 1;
    kBfield[7046] = 1;
    kBfield[7051] = 1;
    kBfield[7052] = 1;
    kBfield[7053] = 1;
    kBfield[7059] = 1;
    kBfield[7061] = 1;
    kBfield[7065] = 1;
    kBfield[7020] = 1;
    kBfield[7023] = 1;

    kBfield[17319] = 1;
    kBfield[17324] = 1;
    kBfield[17327] = 1;
    kBfield[17331] = 1;
    kBfield[17340] = 1;
    kBfield[17344] = 1;
    kBfield[17352] = 1;
    kBfield[17356] = 1;
    kBfield[17359] = 1;
    kBfield[17364] = 1;
    kBfield[17367] = 1;

    kBfield[18241] = 1;
    kBfield[18238] = 1;
    kBfield[18218] = 1;
    kBfield[18221] = 1;
    kBfield[18224] = 1;
    kBfield[18226] = 1;
    kBfield[18228] = 1;
    kBfield[18230] = 1;
    kBfield[18233] = 1;
    kBfield[18243] = 1;

    kBfield[18412] = 1;
    kBfield[18413] = 1;
    kBfield[18414] = 1;
    kBfield[18415] = 1;
    kBfield[18417] = 1;
    kBfield[18418] = 1;
    kBfield[18419] = 1;
    kBfield[18420] = 1;
    kBfield[18421] = 1;
    kBfield[18423] = 1;
    kBfield[18424] = 1;

    kBfield[18564] = 1;
    kBfield[18563] = 1;
    kBfield[18562] = 1;
    kBfield[18561] = 1;
    kBfield[18552] = 1;
    kBfield[18551] = 1;
    kBfield[18550] = 1;
    kBfield[18549] = 1;
    kBfield[18548] = 1;
    kBfield[18547] = 1;

    kBfield[18609] = 1;
    kBfield[18606] = 1;
    kBfield[18605] = 1;
    kBfield[18601] = 1;
    kBfield[18600] = 1;
    kBfield[18598] = 1;
    kBfield[18597] = 1;
    kBfield[18595] = 1;
    kBfield[18594] = 1;
    kBfield[18592] = 1;
    kBfield[18591] = 1;
    
    kBfield[19611] = 0;
    kBfield[19616] = 0;
   
    kBfield[19636] = 1;
    kBfield[19637] = 1;
    kBfield[19638] = 1;
    kBfield[19639] = 1;
    kBfield[19640] = 1;
    kBfield[19641] = 1;
    kBfield[19643] = 1;
  }

private:
  const  int       kNruns;
         double   *kDlength;
         int      *kBfield;
  const  double    kDLoff;
  static Runinfo  *gInstancePtr;
};

Runinfo *Runinfo::gInstancePtr = 0;
#endif
