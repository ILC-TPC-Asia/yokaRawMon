#ifndef Runinfo_H
#define Runinfo_H
#include <iostream>
#include <map>

using namespace std;

class Runinfo {
public:
  // ----------
  // Getters
  // ----------
  static Runinfo *GetInstancePtr()
  {
    if (!gInstancePtr) gInstancePtr = new Runinfo;
    return gInstancePtr;
  }

  inline double GetDlength  (int run)
  {
    if (fRunMap.find(run) == fRunMap.end()) return 1.e10;
    else                                    return fRunMap[run].getDlength();
  }

  inline double GetBfield   (int run)
  {
    if (fRunMap.find(run) == fRunMap.end()) return 1.e10;
    else                                    return fRunMap[run].getBfield();
  }

  inline double GetMomentum(int run)
  {
    if (fRunMap.find(run) == fRunMap.end()) return 1.e10;
    else                                    return fRunMap[run].getMomentum();
  }

  inline double GetAnglephi(int run)
  {
    if (fRunMap.find(run) == fRunMap.end()) return 1.e10;
    else                                    return fRunMap[run].getAnglephi();
  }
  
  inline double GetAngletheta(int run)
  {
    if (fRunMap.find(run) == fRunMap.end()) return 1.e10;
    else                                    return fRunMap[run].getAngletheta();
  }

  // ---------
  // Utility
  // ---------
  inline void   RegisterRun(int run, int b, double p, double d, double phi, double theta )
  {
    if (fRunMap.find(run) == fRunMap.end()) {
      fRunMap[run] = RunCond(b,p,d,phi,theta);
    } else {
      cerr << "run" << run << " already there!" << endl;;
    }
  }


private:
  Runinfo()
  {
    //--
    // 2008 data
    //--
    //(Run#)
/*    RegisterRun(6953, 0, 5, 10);
    RegisterRun(6957, 0, 5, 15);
    RegisterRun(6958, 0, 5, 20);
    RegisterRun(6972, 0, 5, 25);
    RegisterRun(6973, 0, 5, 30);
    RegisterRun(6974, 0, 5, 35);
    RegisterRun(6975, 0, 5, 40);
    RegisterRun(6976, 0, 5, 45);
    RegisterRun(6977, 0, 5, 50);

    RegisterRun(7049, 1, 5, 10);
    RegisterRun(7046, 1, 5, 15);
    RegisterRun(7051, 1, 5, 20);
    RegisterRun(7052, 1, 5, 25);
    RegisterRun(7053, 1, 5, 25);
    RegisterRun(7059, 1, 5, 30);
    RegisterRun(7061, 1, 5, 35);
    RegisterRun(7065, 1, 5, 40);
    RegisterRun(7020, 1, 5, 45);
    RegisterRun(7023, 1, 5, 50);

    //--
    // 2010 data
    //--
    RegisterRun(17319, 1, 5,  5);
    RegisterRun(17324, 1, 5, 15);
    RegisterRun(17327, 1, 5, 25);
    RegisterRun(17331, 1, 5, 35);
    RegisterRun(17375, 1, 5, 35);
    RegisterRun(17340, 1, 5, 45);
    RegisterRun(17344, 1, 5, 55);
    RegisterRun(17352, 1, 5, 50);
    RegisterRun(17356, 1, 5, 40);
    RegisterRun(17359, 1, 5, 30);
    RegisterRun(17364, 1, 5, 20);
    RegisterRun(17367, 1, 5, 10);

    //--
    // 2012 data
    //--
    RegisterRun(18169, 1, 5, 52);
    RegisterRun(18180, 1, 5, 52);
    RegisterRun(18184, 1, 5, 52);
    RegisterRun(18185, 1, 5, 52);
    RegisterRun(18192, 1, 5, 0);
    RegisterRun(18193, 1, 5, 0.1);
    RegisterRun(18194, 1, 5, 10);
    RegisterRun(18195, 1, 5, 20);
    RegisterRun(18196, 1, 5, 30);
    RegisterRun(18197, 1, 5, 40);
    RegisterRun(18198, 1, 5, 50);
    RegisterRun(18199, 1, 5, 0.1);
    RegisterRun(18201, 1, 5, 0.1);
    RegisterRun(18205, 1, 5, 0.1);
    RegisterRun(18208, 1, 5, 0.1);
    RegisterRun(18214, 1, 5, 2.5);
    RegisterRun(18215, 1, 5, 5.);
    RegisterRun(18218, 1, 5, 7.5);
    RegisterRun(18221, 1, 5, 10);
    RegisterRun(18224, 1, 5, 15);
    RegisterRun(18226, 1, 5, 20);
    RegisterRun(18228, 1, 5, 25);
    RegisterRun(18230, 1, 5, 30);
    RegisterRun(18233, 1, 5, 40);
    RegisterRun(18235, 1, 5, 55);
    RegisterRun(18238, 1, 5, 5);
    RegisterRun(18241, 1, 5, 2.5);
    RegisterRun(18243, 1, 5, 50);

    // gas changed
    RegisterRun(18247, 1, 5, 50);
    
    // angle phi=10
    RegisterRun(18252, 1, 5, 50);
    RegisterRun(18253, 1, 5, 40);
    RegisterRun(18254, 1, 5, 30);
    RegisterRun(18255, 1, 5, 25);
    RegisterRun(18259, 1, 5, 20);
    RegisterRun(18260, 1, 5, 15);
    RegisterRun(18263, 1, 5, 10);
    RegisterRun(18264, 1, 5, 7.5);
    RegisterRun(18266, 1, 5, 5);
    RegisterRun(18267, 1, 5, 2.5);

    // angle phi=-5
    RegisterRun(18275, 1, 5, 5);
    RegisterRun(18278, 1, 5, 7.5);
    RegisterRun(18280, 1, 5, 10);
    RegisterRun(18282, 1, 5, 15);
    RegisterRun(18284, 1, 5, 20);
    RegisterRun(18286, 1, 5, 25);
    RegisterRun(18288, 1, 5, 30);
    RegisterRun(18290, 1, 5, 40);
    RegisterRun(18295, 1, 5, 50);
    RegisterRun(18297, 1, 5, 40);
    RegisterRun(18300, 1, 5, 30);
    RegisterRun(18302, 1, 5, 25);
 
    // P=3GeV 
    RegisterRun(18308, 1, 5, 20);
    RegisterRun(18310, 1, 5, 15);
    RegisterRun(18312, 1, 5, 15);

    // HV noise removed
    RegisterRun(18356, 1, 5, 5);
    RegisterRun(18362, 1, 5, 50);
    RegisterRun(18367, 1, 5, 40);
    RegisterRun(18368, 1, 5, 30);
    RegisterRun(18369, 1, 5, 25);
    RegisterRun(18370, 1, 5, 20);
    RegisterRun(18372, 1, 5, 15);
    RegisterRun(18373, 1, 5, 12.5);
    RegisterRun(18374, 1, 5, 10);
    RegisterRun(18375, 1, 5, 7.5);
    RegisterRun(18377, 1, 5, 2.5);

    // HV increased
    RegisterRun(18381, 1, 5, 50);
    RegisterRun(18382, 1, 5, 20);
    RegisterRun(18385, 1, 5, 50);
    RegisterRun(18387, 1, 5, 50);
    // z scan with high gain cathode VGEM=350V
    RegisterRun(18389, 1, 5, 49);
    RegisterRun(18390, 1, 5, 39);
    RegisterRun(18391, 1, 5, 29);
    RegisterRun(18394, 1, 5, 24);
    RegisterRun(18395, 1, 5, 19);
    RegisterRun(18396, 1, 5, 14);
    RegisterRun(18398, 1, 5, 11.5);
    RegisterRun(18399, 1, 5, 9);
    RegisterRun(18400, 1, 5, 6.5);
    RegisterRun(18403, 1, 5, 5);
    RegisterRun(18404, 1, 5, 2.5);
    // z scan with high gain cathode VGEM=355V
    RegisterRun(18412, 1, 5, 2.5);
    RegisterRun(18413, 1, 5, 5);
    RegisterRun(18414, 1, 5, 6.5);
    RegisterRun(18415, 1, 5, 9);
    RegisterRun(18417, 1, 5, 11.5);
    RegisterRun(18418, 1, 5, 14);
    RegisterRun(18419, 1, 5, 19);
    RegisterRun(18420, 1, 5, 24);
    RegisterRun(18421, 1, 5, 29);
    RegisterRun(18423, 1, 5, 39);
    RegisterRun(18424, 1, 5, 49);

    // HV 360
    RegisterRun(18429, 1, 5, 49);
  
    // shaper 60ns HV355
    RegisterRun(18432, 1, 5, 49);
    RegisterRun(18433, 1, 5, 39);
    RegisterRun(18434, 1, 5, 29);
    RegisterRun(18435, 1, 5, 24);
    RegisterRun(18438, 1, 5, 19);
    RegisterRun(18439, 1, 5, 14);
    RegisterRun(18441, 1, 5, 9);
    RegisterRun(18443, 1, 5, 6.5);
    RegisterRun(18444, 1, 5, 5);

    // Threshold=3 z-scan B=ON shaper=120 HV=355
    RegisterRun(18450, 1, 5, 49);
    RegisterRun(18451, 1, 5, 39);
    RegisterRun(18453, 1, 5, 29);
    RegisterRun(18454, 1, 5, 24);
    RegisterRun(18457, 1, 5, 19);
    RegisterRun(18458, 1, 5, 14);
    RegisterRun(18461, 1, 5, 11.5);
    RegisterRun(18462, 1, 5, 9);
    RegisterRun(18464, 1, 5, 6.5);
    RegisterRun(18465, 1, 5, 5);
    RegisterRun(18466, 1, 5, 2.5);

    // Threshold=3 z-scan B=0 shaper=120 HV=355
    RegisterRun(18508, 1, 5, 49);
    RegisterRun(18507, 1, 5, 39);
    RegisterRun(18506, 1, 5, 29);
    RegisterRun(18504, 1, 5, 24);
    RegisterRun(18503, 1, 5, 19);
    RegisterRun(18502, 1, 5, 14);
    RegisterRun(18501, 1, 5, 11.5);
    RegisterRun(18498, 1, 5, 9);
    RegisterRun(18497, 1, 5, 6.5);
    RegisterRun(18495, 1, 5, 5);
    RegisterRun(18494, 1, 5, 2.5);

    // Threshold=3 z-scan B=1 shaper=120 HV=330,355 transfer up
    RegisterRun(18510, 1, 5, 49);
    RegisterRun(18511, 1, 5, 39);
    RegisterRun(18512, 1, 5, 29);
    RegisterRun(18514, 1, 5, 24);
    RegisterRun(18515, 1, 5, 19);
    RegisterRun(18516, 1, 5, 14);
    RegisterRun(18518, 1, 5, 11.5);
    RegisterRun(18519, 1, 5, 9);
    RegisterRun(18520, 1, 5, 6.5);
    RegisterRun(18524, 1, 5, 5);
    RegisterRun(18525, 1, 5, 2.5);

    // Threshold=3 z-scan B=1 shaper=120 HV=330,340 low transfer&induction
    RegisterRun(18545, 1, 5, 49);
    RegisterRun(18544, 1, 5, 39);
    RegisterRun(18543, 1, 5, 29);
    RegisterRun(18542, 1, 5, 24);
    RegisterRun(18540, 1, 5, 19);
    RegisterRun(18539, 1, 5, 14);
    RegisterRun(18538, 1, 5, 11.5);
    RegisterRun(18537, 1, 5, 9);
    RegisterRun(18530, 1, 5, 6.5);
    RegisterRun(18529, 1, 5, 5);
    RegisterRun(18528, 1, 5, 2.5);

    // Threshold=3 z-scan B=1 shaper=120 HV=330,355 high transfer&induction
    RegisterRun(18547, 1, 5, 49);
    RegisterRun(18548, 1, 5, 39);
    RegisterRun(18549, 1, 5, 29);
    RegisterRun(18550, 1, 5, 24);
    RegisterRun(18551, 1, 5, 19);
    RegisterRun(18552, 1, 5, 14);
    RegisterRun(18561, 1, 5, 11.5);
    RegisterRun(18562, 1, 5, 9);
    RegisterRun(18563, 1, 5, 6.5);
    RegisterRun(18564, 1, 5, 5);

    // HV 360 360
    RegisterRun(18570, 1, 5, 49);
    RegisterRun(18572, 1, 5, 11.5);

    // HV:330,355;Shaper:120;Threshold:3;high EF in transfer and induction gap
    RegisterRun(18582, 1, 5, 50);
    RegisterRun(18583, 1, 5, 40);
    RegisterRun(18585, 1, 5, 30);
    RegisterRun(18586, 1, 5, 20);
    RegisterRun(18588, 1, 5, 10);

    // Threshold=3 z-scan B=1 shaper=120 HV=330,355 20000event sametarnsfer & indaction gap 18547~18564
    RegisterRun(18591, 1, 5, 50);
    RegisterRun(18592, 1, 5, 40);
    RegisterRun(18594, 1, 5, 30);
    RegisterRun(18595, 1, 5, 25);
    RegisterRun(18597, 1, 5, 20);
    RegisterRun(18598, 1, 5, 15);
    RegisterRun(18600, 1, 5, 12.5);
    RegisterRun(18601, 1, 5, 10);
    RegisterRun(18605, 1, 5, 7.5);
    RegisterRun(18606, 1, 5, 5);
    RegisterRun(18609, 1, 5, 2.5);

    RegisterRun(18721, 1, 5, 50);

    RegisterRun(18739, 1, 5, 2.5);
    RegisterRun(18740, 1, 5, 10);
    RegisterRun(18741, 1, 5, 20);
    RegisterRun(18742, 1, 5, 30);
    RegisterRun(18743, 1, 5, 40);
    RegisterRun(18744, 1, 5, 50);
*/
    //-------------
    //2016 data
    //-------------

    RegisterRun(19782, 1, 5, 2.5, 0, 20);
    RegisterRun(19783, 1, 5, 5, 0, 20);
    RegisterRun(19784, 1, 5, 10, 0, 20);
    RegisterRun(19786, 1, 5, 15, 0, 20);
    RegisterRun(19787, 1, 5, 20, 0, 20);
    RegisterRun(19788, 1, 5, 25, 0, 20);
    RegisterRun(19789, 1, 5, 30, 0, 20);
    RegisterRun(19790, 1, 5, 35, 0, 20);
    RegisterRun(19791, 1, 5, 40, 0, 20);
    RegisterRun(19792, 1, 5, 45, 0, 20);
    RegisterRun(19793, 1, 5, 50, 0, 20);
    RegisterRun(19794, 1, 5, 55, 0, 20);

/*
    RegisterRun(19881, 1, 5, 55);
    RegisterRun(19882, 1, 5, 50);
    RegisterRun(19883, 1, 5, 45);
    RegisterRun(19884, 1, 5, 40);
    RegisterRun(19885, 1, 5, 35);
    RegisterRun(19886, 1, 5, 30);
    RegisterRun(19887, 1, 5, 25);
    RegisterRun(19888, 1, 5, 20);
    RegisterRun(19889, 1, 5, 15);
    RegisterRun(19890, 1, 5, 12.5);
    RegisterRun(19891, 1, 5, 10);
    RegisterRun(19892, 1, 5, 7.5);
    RegisterRun(19893, 1, 5, 5);
    RegisterRun(19894, 1, 5, 2.5);
    RegisterRun(19895, 1, 5, 1.25);
*/

    
}

//private:
public:
  // -------------
  // private class
  // -------------
  class RunCond {
  private:
    int    _bfield;
    double _momentum;
    double _dlength;
    double _anglephi;
    double _angletheta;

  public:
    RunCond(int b = 1, double p = 5., double d = 0., double phi = 0.,double theta = 0.)
           : _bfield(b), _momentum(p), _dlength(d), _anglephi(phi), _angletheta(theta)  {}

    inline int    getBfield  () const { return _bfield;   } // [T]
    inline double getDlength () const { return _dlength;  } // [cm]
    inline double getMomentum() const { return _momentum; } // [GeV]
    inline double getAnglephi() const { return _anglephi; } // [°]
    inline double getAngletheta() const { return _angletheta; } // [°]
  };

private:
  // -------------
  // Data members
  // -------------
  map<int, RunCond>  fRunMap;      // run map
  static Runinfo    *gInstancePtr; // singleton pointer
};

Runinfo *Runinfo::gInstancePtr = 0;
#endif
