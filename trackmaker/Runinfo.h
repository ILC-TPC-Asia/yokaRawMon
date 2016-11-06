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
  inline int GetAngle (int run) const { return run < kNruns ? kAngle [run] : 1.e10; } 
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
  //Runinfo() : kNruns(100000), kDlength(0), kBfield(0) ,kDLoff(-1.0)
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

    kDlength[19751] = 20;

    kDlength[19761] = 55;  
    kDlength[19762] = 50;  
    kDlength[19763] = 45;  
    kDlength[19764] = 40;  
    kDlength[19765] = 35;  
    kDlength[19766] = 30;  
    kDlength[19767] = 25;  
    kDlength[19768] = 20;  
    kDlength[19769] = 15;  
    kDlength[19770] = 10;  
    kDlength[19771] = 5;  
    kDlength[19772] = 2.5; 

    kDlength[19782] = 2.5; 
    kDlength[19783] = 5; 
    kDlength[19784] = 10; 
    kDlength[19786] = 15; 
    kDlength[19787] = 20; 
    kDlength[19788] = 25; 
    kDlength[19789] = 30; 
    kDlength[19790] = 35; 
    kDlength[19791] = 40; 
    kDlength[19792] = 45; 
    kDlength[19793] = 50; 
    kDlength[19794] = 55; 

    kDlength[19785] = 10;//Vcath=350 V,Vanode=315 V 

    kDlength[19800] = 50; 
    kDlength[19801] = 45; 
    kDlength[19802] = 40; 
    kDlength[19803] = 35; 
    kDlength[19804] = 30; 
    kDlength[19805] = 25; 
    kDlength[19806] = 20; 
    kDlength[19807] = 15; 
    kDlength[19808] = 10; 
    kDlength[19809] = 5; 
    kDlength[19810] = 2.5; 
   
    kDlength[19816] = 2.5; 
    kDlength[19817] = 5; 
    kDlength[19818] = 10; 
    kDlength[19819] = 15; 
    kDlength[19820] = 20; 
    kDlength[19821] = 25; 
    kDlength[19822] = 30; 
    kDlength[19824] = 35; 
    kDlength[19825] = 40; 
    kDlength[19826] = 45; 
    kDlength[19827] = 50; 
    kDlength[19828] = 55; 

    kDlength[19823] = 35; //Vcath=350 V,Vanode=315 V 

    kDlength[19830] = 55;  
    kDlength[19831] = 50;  
    kDlength[19832] = 45;  
    kDlength[19833] = 40;  
    kDlength[19834] = 35;  
    kDlength[19835] = 30;  
    kDlength[19836] = 25;  
    kDlength[19837] = 20;  
    kDlength[19838] = 15;  
    kDlength[19839] = 10;  
    kDlength[19840] = 5;  
    kDlength[19841] = 2.5;    

    kDlength[19848] = 25;  
    kDlength[19849] = 25;  
    kDlength[19850] = 25;  
    kDlength[19851] = 25;  
    kDlength[19852] = 15;  
    kDlength[19853] = 15;  
    kDlength[19854] = 35;  
    kDlength[19855] = 35;  
   
    kDlength[19860] = 35;  
    kDlength[19861] = 25;  
    kDlength[19862] = 20;  
    kDlength[19863] = 15;    
    kDlength[19864] = 10;    
    kDlength[19865] = 5;    
    kDlength[19866] = 2.5;    
    kDlength[19867] = 1.25;    
    kDlength[19868] = 0.75;    
    kDlength[19869] = 7.5;    
    kDlength[19870] = 12.5;    
    kDlength[19871] = 30;    
    kDlength[19872] = 40;    
    kDlength[19873] = 50;    
    kDlength[19874] = 55;    
    kDlength[19875] = 55;       

    kDlength[19881] = 55;    
    kDlength[19882] = 50;    
    kDlength[19883] = 45;    
    kDlength[19884] = 40;    
    kDlength[19885] = 35;    
    kDlength[19886] = 30;    
    kDlength[19887] = 25;    
    kDlength[19888] = 20;    
    kDlength[19889] = 15;    
    kDlength[19890] = 12.5;    
    kDlength[19891] = 10;    
    kDlength[19892] = 7.5;    
    kDlength[19893] = 5;    
    kDlength[19894] = 2.5;    
    kDlength[19895] = 1.25;    

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

    kBfield[19751] = 1;

    kBfield[19761] = 1;  
    kBfield[19762] = 1;  
    kBfield[19763] = 1;  
    kBfield[19764] = 1;  
    kBfield[19765] = 1;  
    kBfield[19766] = 1;  
    kBfield[19767] = 1;  
    kBfield[19768] = 1;  
    kBfield[19769] = 1;  
    kBfield[19770] = 1;  
    kBfield[19771] = 1;  
    kBfield[19772] = 1; 

    kBfield[19782] = 0; 
    kBfield[19783] = 0; 
    kBfield[19784] = 0; 
    kBfield[19786] = 0; 
    kBfield[19787] = 0; 
    kBfield[19788] = 0; 
    kBfield[19789] = 0; 
    kBfield[19790] = 0; 
    kBfield[19791] = 0; 
    kBfield[19792] = 0; 
    kBfield[19793] = 0; 
    kBfield[19794] = 0; 

    kBfield[19785] = 0;//Vcath=350 V,Vanode=315 V 

    kBfield[19800] = 0; 
    kBfield[19801] = 0; 
    kBfield[19802] = 0; 
    kBfield[19803] = 0; 
    kBfield[19804] = 0; 
    kBfield[19805] = 0; 
    kBfield[19806] = 0; 
    kBfield[19807] = 0; 
    kBfield[19808] = 0; 
    kBfield[19809] = 0; 
    kBfield[19810] = 0; 
   
    kBfield[19816] = 0; 
    kBfield[19817] = 0; 
    kBfield[19818] = 0; 
    kBfield[19819] = 0; 
    kBfield[19820] = 0; 
    kBfield[19821] = 0; 
    kBfield[19822] = 0; 
    kBfield[19824] = 0; 
    kBfield[19825] = 0; 
    kBfield[19826] = 0; 
    kBfield[19827] = 0; 
    kBfield[19828] = 0; 

    kBfield[19823] = 0; //Vcath=350 V,Vanode=315 V 

    kBfield[19830] = 0;  
    kBfield[19831] = 0;  
    kBfield[19832] = 0; 
    kBfield[19833] = 0;  
    kBfield[19834] = 0;  
    kBfield[19835] = 0;  
    kBfield[19836] = 0;  
    kBfield[19837] = 0;  
    kBfield[19838] = 0;  
    kBfield[19839] = 0;  
    kBfield[19840] = 0;  
    kBfield[19841] = 0;    

    kBfield[19848] = 0;  
    kBfield[19849] = 0;  
    kBfield[19850] = 0;  
    kBfield[19851] = 0;  
    kBfield[19852] = 0;  
    kBfield[19853] = 0;  
    kBfield[19854] = 0;  
    kBfield[19855] = 0;  
   
    kBfield[19860] = 1;  
    kBfield[19861] = 1;  
    kBfield[19862] = 1;  
    kBfield[19863] = 1;    
    kBfield[19864] = 1;    
    kBfield[19865] = 1;    
    kBfield[19866] = 1;    
    kBfield[19867] = 1;    
    kBfield[19868] = 1;    
    kBfield[19869] = 1;    
    kBfield[19870] = 1;    
    kBfield[19871] = 1;    
    kBfield[19872] = 1;    
    kBfield[19873] = 1;    
    kBfield[19874] = 1;    
    kBfield[19875] = 1;       

    kBfield[19881] = 1;    
    kBfield[19882] = 1;    
    kBfield[19883] = 1;    
    kBfield[19884] = 1;    
    kBfield[19885] = 1;    
    kBfield[19886] = 1;    
    kBfield[19887] = 1;    
    kBfield[19888] = 1;    
    kBfield[19889] = 1;    
    kBfield[19890] = 1;    
    kBfield[19891] = 1;    
    kBfield[19892] = 1;    
    kBfield[19893] = 1;    
    kBfield[19894] = 1;    

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

    kAngle[19732] = 10;  
    kAngle[19733] = 10;  
    kAngle[19734] = 10;  
    kAngle[19735] = 10;  
    kAngle[19736] = 10;  
    kAngle[19737] = 10;  
    kAngle[19738] = 10;  
    kAngle[19739] = 10;  
    kAngle[19740] = 10;  
    kAngle[19741] = 10;  
    kAngle[19742] = 10;  
    kAngle[19743] = 10;  

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

    kAngle[19751] = 20;

    kAngle[19761] = 20;  
    kAngle[19762] = 20;  
    kAngle[19763] = 20;  
    kAngle[19764] = 20;  
    kAngle[19765] = 20;  
    kAngle[19766] = 20;  
    kAngle[19767] = 20;  
    kAngle[19768] = 20;  
    kAngle[19769] = 20;  
    kAngle[19770] = 20;  
    kAngle[19771] = 20;  
    kAngle[19772] = 20; 

    kAngle[19782] = 20; 
    kAngle[19783] = 20; 
    kAngle[19784] = 20; 
    kAngle[19786] = 20; 
    kAngle[19787] = 20; 
    kAngle[19788] = 20; 
    kAngle[19789] = 20; 
    kAngle[19790] = 20; 
    kAngle[19791] = 20; 
    kAngle[19792] = 20; 
    kAngle[19793] = 20; 
    kAngle[19794] = 20; 

    kAngle[19785] = 20;//Vcath=350 V,Vanode=315 V 

    kAngle[19800] = 10; 
    kAngle[19801] = 10; 
    kAngle[19802] = 10; 
    kAngle[19803] = 10; 
    kAngle[19804] = 10; 
    kAngle[19805] = 10; 
    kAngle[19806] = 10; 
    kAngle[19807] = 10; 
    kAngle[19808] = 10; 
    kAngle[19809] = 10; 
    kAngle[19810] = 10; 

    kAngle[19816] = 1; 
    kAngle[19817] = 1; 
    kAngle[19818] = 1; 
    kAngle[19819] = 1; 
    kAngle[19820] = 1; 
    kAngle[19821] = 1; 
    kAngle[19822] = 1; 
    kAngle[19823] = 1; 
    kAngle[19824] = 1; 
    kAngle[19825] = 1; 
    kAngle[19826] = 1; 
    kAngle[19827] = 1; 
    kAngle[19828] = 1; 

    kAngle[19830] = 20; 
    kAngle[19831] = 20; 
    kAngle[19832] = 20; 
    kAngle[19833] = 20; 
    kAngle[19834] = 20; 
    kAngle[19835] = 20; 
    kAngle[19836] = 20; 
    kAngle[19837] = 20; 
    kAngle[19838] = 20; 
    kAngle[19839] = 20; 
    kAngle[19840] = 20; 
    kAngle[19841] = 20; 

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
