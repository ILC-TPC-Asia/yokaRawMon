#ifndef JHITLAYER_H
#define JHITLAYER_H
#include "JHit.h"

#include "TObjArray.h"

class TTUBS;

class JHitLayer : public TObjArray {
public:
  JHitLayer();

  inline Int_t GetModuleID () const { return fModule;      }
  inline Int_t GetLayerID  () const { return fLayer;       }


  JHit *operator()(Int_t i) 
  { 
    return static_cast<JHit *>(TObjArray::operator[](i));
  }
  
  void SetClusters(TObjArray *jclsptr);
  void MakeHits(TObjArray *jclsptr = 0);

  virtual void DebugPrint(const Char_t *opt = "");

  virtual void Draw(const Char_t *opt="");
  virtual void Draw(Int_t  color, const Char_t *opt="");
#if 1
  Bool_t IsSortable() const { return kTRUE; }
  Int_t  Compare(const TObject *obj) const;
  enum ESort { kDet, kLayer };
  static void SetSortBy(ESort type) { fgSortBy = type; }
#endif

private:
  void Update();

  Int_t DistancetoPrimitive(Int_t px, Int_t py); // object picker

private:
  Int_t           fModule;      // module ID
  Int_t           fLayer;       // layer ID
  TObjArray      *fClustersPtr; // pointer to cluster array
  Bool_t          fDone;        //! 1 if fXY set, 0 otherwise

  TTUBS          *fTubsPtr;     //! marker pointer
#if 1
  static ESort    fgSortBy;     //! sort type
#endif
  ClassDef(JHitLayer, 1)  // Raw Data class
};
#endif
