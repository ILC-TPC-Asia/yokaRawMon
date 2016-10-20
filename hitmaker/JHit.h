#ifndef JHIT_H
#define JHIT_H
#include "JCluster.h"

#include "TObjArray.h"
#include "TVector3.h"


class TPolyMarker3D;

class JHit : public TObjArray {
public:
  JHit(JCluster *cp = 0);
  JHit(const JHit &h);
  ~JHit();

  inline Double_t GetTime     () const { return fTime;        }
  inline Int_t    GetMaximum  () const { return fPhMax;       }
  inline Int_t    GetCharge   () const { return fCharge;      }

  inline Int_t    GetModuleID () const { return fModule;      }
  inline Int_t    GetLayerID  () const { return fLayer;       }
  inline Int_t    GetPadID    () const { return fPad;         }

  virtual TVector3  GetPosition();
  virtual TVector3 &GetLocalPosition();

  JCluster *operator()(Int_t i) 
  { 
    return static_cast<JCluster *>(TObjArray::operator[](i));
  }

  virtual void DebugPrint(const Char_t *opt = "");

  virtual void Draw(const Char_t *opt="");
  virtual void Draw(Int_t  color, const Char_t *opt="");
#if 1
  Bool_t IsSortable() const { return kTRUE; }
  Int_t  Compare(const TObject *obj) const;
  enum ESort { kPad, kTime };
  static void SetSortBy(ESort type) { fgSortBy = type; }
#endif

private:
  void Update();

  Int_t DistancetoPrimitive(Int_t px, Int_t py); // object picker

private:
  Double_t        fTime;       // time
  Int_t           fPhMax;      // maximum pulse height
  Int_t           fCharge;     // charge
  Int_t           fModule;     // module ID
  Int_t           fLayer;      // layer ID
  Int_t           fPad;        // pad ID
  Bool_t          fDone;       //! 1 if fXY set, 0 otherwise
  TVector3        fXYZ;        // local position

  TPolyMarker3D  *fPm3dPtr;    //! marker pointer
#if 1
  static ESort    fgSortBy;    //! sort type
#endif
  ClassDef(JHit, 1)  // Raw Data class
};
#endif
