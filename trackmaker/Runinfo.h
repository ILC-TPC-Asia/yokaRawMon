#ifndef Runinfo_H
#define Runinfo_H
#include <iostream>

class Runinfo {
public:
  ~Runinfo()
  {
    if (kDlength) delete [] kDlength;
    if (kBfield)  delete [] kBfield;
    if (kAngle)  delete [] kAngle;
  }

  inline double GetDlength(int run) const { return run < kNruns ? kDlength[run] : 1.e10; } 
  inline int GetBfield (int run) const { return run < kNruns ? kBfield [run] : 1.e10; } 
  inline int GetAngle (int run) const { return run < kNruns ? kANgle [run] : 1.e10; } 
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
  Runinfo() : kNruns(100000), kDlength(0), kBfield(0), kAngle(0) ,kDLoff(-1.0)
#endif
  {
    kDlength = new double[kNruns];
    kBfield  = new int[kNruns];
    kAngle  = new int[kNruns];
    for (Int_t i=0; i<kNruns; i++) {
      kDlength[i] = 0;
      kBfield [i] = 0;
      kAngle [i] = 0;
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
   
    kDlength[19636] = 11.5; //modified
    kDlength[19637] = 16.5; 
    kDlength[19638] = 21.5; 
    kDlength[19639] = 26.5; 
    kDlength[19640] = 31.5; 
    kDlength[19641] = 36.5; 
    kDlength[19643] = 39; 
    kDlength[19644] = 16.5; 
    kDlength[19645] = 11.5; 
    kDlength[19646] = 6.5; 
    kDlength[19647] = 4.5; 
    kDlength[19648] = 2.5; 
    kDlength[19649] = 0.5; 
    kDlength[19651] = 36.5; 
    kDlength[19652] = 41.5; 
    kDlength[19653] = 41.5; 
    kDlength[19654] = 36.5; 
    kDlength[19655] = 46.5; 
    kDlength[19656] = 51.5; 
    kDlength[19657] = 56.5; 

    kDlength[19666] = 56.8; //modified
    kDlength[19667] = 55; 
    kDlength[19668] = 50; 
    kDlength[19669] = 45; 
    kDlength[19670] = 40; 
    kDlength[19671] = 35; 
    kDlength[19672] = 30; 
    kDlength[19673] = 25; 
    kDlength[19674] = 19.6; 
    kDlength[19675] = 14.6; 
    kDlength[19676] = 9.6; 
    kDlength[19677] = 4.6;  
    kDlength[19678] = 2.6;  
    kDlength[19680] = 0;  

    kDlength[19687] = 2.5;  
    kDlength[19692] = 5.0;  
    kDlength[19693] = 10;  
    kDlength[19696] = 15;  
    kDlength[19697] = 20;  
    kDlength[19698] = 25;  
    kDlength[19699] = 30;  
    kDlength[19700] = 35;  
    kDlength[19701] = 40;  
    kDlength[19702] = 45;  
    kDlength[19703] = 50;  
    kDlength[19704] = 55;

    kDlength[19717] = 2.5;  
    kDlength[19718] = 5;  
    kDlength[19719] = 10;  
    kDlength[19720] = 15;  
    kDlength[19721] = 20;  
    kDlength[19722] = 25;  
    kDlength[19723] = 30;  
    kDlength[19724] = 35;  
    kDlength[19725] = 40;  
    kDlength[19726] = 45;  
    kDlength[19727] = 50;  
    kDlength[19728] = 55;  

    kDlength[19732] = 55;  
    kDlength[19733] = 50;  
    kDlength[19734] = 45;  
    kDlength[19735] = 40;  
    kDlength[19736] = 35;  
    kDlength[19737] = 30;  
    kDlength[19738] = 25;  
    kDlength[19739] = 20;  
    kDlength[19740] = 15;  
    kDlength[19741] = 10;  
    kDlength[19742] = 5;  
    kDlength[19743] = 2.5;  

    kDlength[19746] = 2.5;  
    kDlength[19747] = 5;  
    kDlength[19748] = 10;  
    kDlength[19749] = 15;  
    kDlength[19750] = 20;  
    kDlength[19752] = 25;  
    kDlength[19753] = 30;  
    kDlength[19754] = 35;  
    kDlength[19755] = 40;  
    kDlength[19756] = 45;  
    kDlength[19757] = 50;  
    kDlength[19758] = 55;  

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
    kBfield[19644] = 1;
    kBfield[19645] = 1;
    kBfield[19646] = 1;
    kBfield[19647] = 1;
    kBfield[19648] = 1;
    kBfield[19649] = 1;
    kBfield[19650] = 1;
    kBfield[19651] = 1;
    kBfield[19652] = 1;
    kBfield[19653] = 1;
    kBfield[19654] = 1;
    kBfield[19655] = 1;
    kBfield[19656] = 1;
    kBfield[19657] = 1;

    kBfield[19666] = 1;
    kBfield[19667] = 1;
    kBfield[19668] = 1;
    kBfield[19669] = 1;
    kBfield[19670] = 1;
    kBfield[19671] = 1;
    kBfield[19672] = 1;
    kBfield[19673] = 1;
    kBfield[19674] = 1;
    kBfield[19675] = 1;
    kBfield[19676] = 1;
    kBfield[19677] = 1;
    kBfield[19678] = 1;
    kBfield[19679] = 1;

    kBfield[19687] = 0;
    kBfield[19692] = 0;
    kBfield[19693] = 0;
    kBfield[19696] = 0;
    kBfield[19697] = 0;
    kBfield[19698] = 0;
    kBfield[19699] = 0;
    kBfield[19700] = 0;
    kBfield[19701] = 0;
    kBfield[19702] = 0;
    kBfield[19703] = 0;
    kBfield[19704] = 0;

    kBfield[19717] = 1;
    kBfield[19718] = 1;
    kBfield[19719] = 1;
    kBfield[19720] = 1;
    kBfield[19721] = 1;
    kBfield[19722] = 1;
    kBfield[19723] = 1;
    kBfield[19724] = 1;
    kBfield[19725] = 1;
    kBfield[19726] = 1;
    kBfield[19727] = 1;
    kBfield[19728] = 1;

    kBfield[19732] = 1;
    kBfield[19733] = 1;
    kBfield[19734] = 1;
    kBfield[19735] = 1;
    kBfield[19736] = 1;
    kBfield[19737] = 1;
    kBfield[19738] = 1;
    kBfield[19739] = 1;
    kBfield[19740] = 1;
    kBfield[19741] = 1;
    kBfield[19742] = 1;
    kBfield[19743] = 1;

    kBfield[19746] = 1;
    kBfield[19747] = 1;
    kBfield[19748] = 1;
    kBfield[19749] = 1;
    kBfield[19750] = 1;
    kBfield[19752] = 1;
    kBfield[19753] = 1;
    kBfield[19754] = 1;
    kBfield[19755] = 1;
    kBfield[19756] = 1;
    kBfield[19757] = 1;
    kBfield[19758] = 1;

    kAngle[19717] = 0;
    kAngle[19718] = 0;
    kAngle[19719] = 0;
    kAngle[19720] = 0;
    kAngle[19721] = 0;
    kAngle[19722] = 0;
    kAngle[19723] = 0;
    kAngle[19724] = 0;
    kAngle[19725] = 0;
    kAngle[19726] = 0;
    kAngle[19727] = 0;
    kAngle[19728] = 0;
    
    kAngle[19743] = 10;
    kAngle[19742] = 10;
    kAngle[19741] = 10;
    kAngle[19739] = 10;
    kAngle[19738] = 10;
    kAngle[19737] = 10;
    kAngle[19736] = 10;
    kAngle[19735] = 10;
    kAngle[19734] = 10;
    kAngle[19733] = 10;
    kAngle[19732] = 10;

    kAngle[19746] = 20;
    kAngle[19747] = 20;
    kAngle[19748] = 20;
    kAngle[19749] = 20;
    kAngle[19750] = 20;
    kAngle[19752] = 20;
    kAngle[19753] = 20;
    kAngle[19754] = 20;
    kAngle[19755] = 20;
    kAngle[19756] = 20;
    kAngle[19757] = 20;
    kAngle[19758] = 20;

  }

private:
  const  int       kNruns;
         double   *kDlength;
         int      *kBfield;
         int      *kAngle;
  const  double    kDLoff;
  static Runinfo  *gInstancePtr;
};

Runinfo *Runinfo::gInstancePtr = 0;
#endif
