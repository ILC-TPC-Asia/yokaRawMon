#ifndef JHITCORRECTOR_H
#define JHITCORRECTOR_H
#include <vector>

class JHitCorrector {
private:
  JHitCorrector(const char *delxfile = 0);
public:
  virtual ~JHitCorrector();

  virtual double GetDeltaX(double x,
                           int layer,
                           int module,
                           int run);

  virtual void   DebugPrint(const char *opt = "");
  static  JHitCorrector *GetInstancePtr(const char *delxfile = 0);

public:
  static const int  kNb =  2;
  static const int  kNm =  7;
  static const int  kNl = 28;

private:
  static double fgDataDX[kNb][kNm][kNl];
  static JHitCorrector *fgInstancePtr;
};
#endif
