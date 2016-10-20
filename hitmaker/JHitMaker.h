#ifndef JHITMAKER_H
#define JHITMAKER_H
#include "JCluster.h"

#include "TObjArray.h"

class JHitMaker : public TObjArray {
public:
  JHitMaker();

  void SetClusters(TObjArray *jclsptr);
  void MakeHits   (TObjArray *jclsptr = 0);

  virtual void DebugPrint(const Char_t *opt = "");

  virtual void Draw(const Char_t *opt="");
  virtual void Draw(Int_t  color, const Char_t *opt="");

private:
  void Update();

private:
  TObjArray  *fClustersPtr; //  pointer to cls array
  Bool_t      fDone;        //! 1 if fXY set, 0 otherwise

  ClassDef(JHitMaker, 1)  // Hit Maker class
};
#endif
