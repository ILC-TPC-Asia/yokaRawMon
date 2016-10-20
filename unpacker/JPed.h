#ifndef __JPed
#define __JPed



#define CHANNELS_RCU 4096
#define MAX_PED_RMS  3
using namespace std;


typedef struct {
  int val[CHANNELS_RCU];
  int anz[CHANNELS_RCU];
  int sum[CHANNELS_RCU];
  double sumq[CHANNELS_RCU];
  float mean[CHANNELS_RCU];
  float rms[CHANNELS_RCU];
  int rcuid;
} PEDCALC;

class JPed 
{
 private:
  PEDCALC pc;
  PEDCALC ps;

 public:
  JPed(int rcuid);
  virtual ~JPed();
  void copyPedestalValues();
  void clearPedestals();
  void makePedestalCalc();
  void fillPedestalCalc(CLUSTER *cl);
  int writePedestalFile(const char *fname);
  int writePedestalFile(const char *fname, const char *comment);
  int readPedestalFile(const char *fname, int rcuid);
  int GetPedestal(int ch); 
};
#endif
