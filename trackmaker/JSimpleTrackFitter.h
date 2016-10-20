#ifndef JSIMPLETRACKFITTER_H
#define JSIMPLETRACKFITTER_H
#include "JVTrackFitter.h"

class JSimpleTrackFitter : public JVTrackFitter {
public:
  JSimpleTrackFitter(TVKalDetector *kaldp = 0);
  virtual ~JSimpleTrackFitter();

  virtual void Fit(JTrack   *trkp,
		   JHelix   &hlix,
		   Double_t &chi2,
		   Int_t    &Ndf);

  static Bool_t GetIsOutFlag    () { return fgIsOut;         }

  static Int_t  GetLayerToLookAt() { return fgLayerToLookAt; }

  static void   SetIsOutFlag    (Bool_t isout = kTRUE) { fgIsOut         = isout; }
  static void   SetLayerToLookAt(Int_t  layer = 14)    { fgLayerToLookAt = layer; }

private:
  static Bool_t fgIsOut;         //! flag to tell if the point in question should be excluded
#if 1
  static Bool_t fgIsUpperHit;     //! flag to tell if there is hit on upper-row of row to look at   
  static Bool_t fgIsLowerHit;     //! flag to tell if there is hit on lower-row of row to look at   
  static Bool_t fgIsMidHit;       //! flag to tell if there is hit on row to look at   
#endif
  static Int_t  fgLayerToLookAt; //! layer to look at

  ClassDef(JSimpleTrackFitter, 1) // simple track fitter class
};
#endif
