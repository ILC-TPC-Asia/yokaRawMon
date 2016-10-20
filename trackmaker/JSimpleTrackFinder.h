#ifndef JSIMPLETRACKFINDER_H
#define JSIMPLETRACKFINDER_H
#include "JVTrackFinder.h"

class JSimpleTrackFinder : public JVTrackFinder {
public:
  JSimpleTrackFinder(JTrackMaker *tmp = 0);
  virtual ~JSimpleTrackFinder();

  virtual void   FindTracks(TObjArray *hitlayers);
  virtual Bool_t AppendHit (JTrack *tp, JHit *hp);

  ClassDef(JSimpleTrackFinder, 1) // abstract base for track finder
};
#endif
