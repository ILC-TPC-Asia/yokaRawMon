#ifndef JVTRACKFINDER_H
#define JVTRACKFINDER_H
#include "TObject.h"

class TObjArray;
class JTrackMaker;
class JTrack;
class JHit;

class JVTrackFinder : public TObject {
public:
  JVTrackFinder(JTrackMaker *tmp = 0);
  virtual ~JVTrackFinder();

  virtual void     FindTracks(TObjArray *hitlayers) = 0; 
  virtual Bool_t   AppendHit (JTrack *tp, JHit *hp) = 0;

  inline JTrackMaker *GetTrackMakerPtr() { return fTrackMakerPtr; }

private:
  JTrackMaker *fTrackMakerPtr; //! pointer to track maker

  ClassDef(JVTrackFinder, 1) // abstract base for track finder
};
#endif
