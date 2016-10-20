#ifndef JREVENT_H
#define JREVENT_H
#include "TObjArray.h"

class JREvent : public TObjArray {
public:
  JREvent();
  inline Int_t GetEventNumber()           { return fEventNo; }
  inline void  SetEventNumber(Int_t evno) { fEventNo = evno; }

private:
  Int_t fEventNo; // event number

  ClassDef(JREvent, 1)  // Raw Data class
};
#endif
