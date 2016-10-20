#ifndef GEMHIST_H
#define GEMHIST_H
#include "TObjArray.h"

class TH1F;
class JREvent;

class GEMHist : public TObjArray {
  private:
    GEMHist();
  public:
    virtual ~GEMHist();

    void ClearAll();
    void FillEvent(JREvent *event);
    static GEMHist *GetInstance();

  private:
    static GEMHist *fgInstancePtr;  //! singleton ptr
    TH1F    **htime;                //  timedist
    JREvent  *fREvent;              //! event

    ClassDef(GEMHist, 1) // histogram container
};
#endif
