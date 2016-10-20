#ifndef JTRACK_H
#define JTRACK_H
#include "JHelix.h"

#include "TObjArray.h"

class JHit;
class JVTrackFinder;
class JVTrackFitter;

class JTrack : public TObjArray {
public:
  JTrack(JHit *hp = 0);
  JTrack(const JTrack &tk);
  virtual ~JTrack();

  Double_t GetChi2  () { if (!fDone) Update(); return fChi2;  }
  Double_t GetNdf   () { if (!fDone) Update(); return fNdf;   }
  JHelix  &GetHelix () { if (!fDone) Update(); return fHelix; }

  Bool_t   Append(JHit *hp);

  static  void SetTrackFinder(JVTrackFinder *trackfinderptr);
  static  void SetTrackFitter(JVTrackFitter *trackfitterptr);

  virtual void DebugPrint(const Char_t *opt = "");

  virtual void Draw(const Char_t *opt="");
  virtual void Draw(Int_t  color, const Char_t *opt="");
#if 1
  Bool_t IsSortable() const { return kTRUE; }
  Int_t  Compare(const TObject *obj) const;
  enum ESort { kChi2 };
  static void SetSortBy(ESort type) { fgSortBy = type; }
#endif

private:
  void Update();

  Int_t DistancetoPrimitive(Int_t px, Int_t py); // object picker

private:
  Double_t        fChi2;       // chi2
  Int_t           fNdf;        // ndf
  JHelix          fHelix;      // helix
  Bool_t          fDone;       //! 1 if fXY set, 0 otherwise
#if 1
  static ESort    fgSortBy;    //! sort type
#endif
  static JVTrackFinder *fgTrackFinderPtr; //! pointer to finder
  static JVTrackFitter *fgTrackFitterPtr; //! pointer to fitter

  ClassDef(JTrack, 1)  // Raw Data class
};
#endif
