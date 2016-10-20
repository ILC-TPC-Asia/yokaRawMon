#ifndef JMONHIST_H
#define JMONHIST_H

#include "JMonEvent.h"
#include "TObjArray.h"
#include <string>

class TH1F;
class TTree;
class TFile;

class JPed;
class JREvent;

class JMonHist : public TObjArray {

  public:
    JMonHist();
    JMonHist(const string &outs);
    virtual ~JMonHist();

    void SetPedestals(int nrcu, JPed **jped);
    void ClearAll();
    void FillEvent(CLUSTER *ad);
    void WriteAll(int ev = 0);
    void CloseFile();

  private:
    //TH1F *htime[8192];   // timedist
    TTree *fTree; // tree
    JREvent *fREvent; // event
    TFile   *fFile; // file
};
#endif
