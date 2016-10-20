#ifndef JVTRACKFITTER_H
#define JVTRACKFITTER_H
#include "TObject.h"

class JTrack;
class JHelix;
class TVKalDetector;

class JVTrackFitter : public TObject {
public:
  JVTrackFitter(TVKalDetector *kaldp = 0);
  virtual ~JVTrackFitter();

  TVKalDetector *GetKalDetector() { return fKalDetPtr; }

  virtual void Fit(JTrack   *trkp,
		   JHelix   &helix,
		   Double_t &chi2,
		   Int_t    &Ndf) = 0;

private:
  TVKalDetector *fKalDetPtr; //! pointer to kaldetector

  ClassDef(JVTrackFitter, 1) // abstract base for track finder
};
#endif
