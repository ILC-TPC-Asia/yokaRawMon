#ifndef JCLUSTER_H
#define JCLUSTER_H
#include "JMonEvent.h"

#include "TObject.h"
#include "TVector2.h"

#include <vector>
using std::vector;

class TObjArray;
class TPolyMarker3D;

class JCluster : public TObject {
public:
  JCluster(CLUSTER *cp = 0, TObjArray *ap = 0);
  inline Int_t    GetRCUID    () const { return fRCUID;       }
  inline Int_t    GetChannelID() const { return fChannelID;   }
  inline Int_t    GetLength   () const { return fData.size(); }
  inline Int_t    GetStartTime() const { return fStartTime;   }
  inline Int_t    GetMaximum  () const { return fPhMax;       }
  inline Int_t    GetCharge   () const { return fCharge;      }

  inline Int_t    GetModuleID () const { return fModule;      }
  inline Int_t    GetLayerID  () const { return fLayer;       }
  inline Int_t    GetPadID    () const { return fPad;         }
  inline Double_t GetTime     () const { return fTime;        }

  virtual TVector2  GetPosition();
  virtual TVector2 &GetLocalPosition();

  Int_t operator[](size_t i) { return fData[i];     }

  virtual void DebugPrint(const Char_t *opt = "");

  virtual void Draw(const Char_t *opt="");
  virtual void Draw(Int_t  color, const Char_t *opt="");

  Bool_t IsSortable() const { return kTRUE; }
  Int_t  Compare(const TObject *obj) const;
  enum ESort { kEle, kDet, kTime };
  static void SetSortBy(ESort type) { fgSortBy = type; }

  static void SetPre      (Int_t pre ) { fgPre       = pre;  }
  static void SetPost     (Int_t post) { fgPost      = post; }
  static void SetThreshold(Int_t thre) { fgThreshold = thre; }

  static Double_t GetVdrift   () { return fgVdrift;    }
  static Double_t GetTbinWidth() { return fgTbinWidth; }

private:
  void Update();

  Int_t DistancetoPrimitive(Int_t px, Int_t py); // object picker

private:
  Int_t           fRCUID;      // rcu ID
  Int_t           fChannelID;  // channel ID
  Int_t           fStartTime;  // starting time
  Double_t        fTime;       // starting time
  vector<Int_t>   fData;       // ADC data

  Int_t           fPhMax;      // maximum pulse height
  Int_t           fCharge;     // charge
  Int_t           fModule;     // module ID
  Int_t           fLayer;      // layer ID
  Int_t           fPad;        // pad ID
  Bool_t          fDone;       //! 1 if fXY set, 0 otherwise
  TVector2        fXY;         //! local position on module

  TPolyMarker3D  *fPm3dPtr;    //! marker pointer

  static ESort    fgSortBy;    //! sort type
  static Int_t    fgPre;       //! pre
  static Int_t    fgPost;      //! post
  static Int_t    fgThreshold; //! threshold
  static Double_t fgVdrift;    //! drift velocity [mm/ns]
  static Double_t fgTbinWidth; //! time bin width [ns]

  ClassDef(JCluster, 1)  // Raw Data class
};
#endif
