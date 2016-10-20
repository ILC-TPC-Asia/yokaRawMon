#ifndef JTRACKMAKER_H
#define JTRACKMAKER_H
#include "JTrack.h"
#include "JVTrackFinder.h"

#include "TObjArray.h"
#include <vector>
using std::vector;

class JTrackMaker : public TObjArray {
public:
  JTrackMaker();
  virtual ~JTrackMaker();

  void SetHits       (TObjArray     *jhitlayersptr);
  void SetTrackFinder(JVTrackFinder *trackfinderptr = 0);

  static void SetLayerToLookAt(Int_t layer)   { fgLayerToLookAt = layer; }
  static Int_t GetLayerToLookAt()             { return fgLayerToLookAt;  }

  static void SetModuleToLookAt(Int_t module) { fgModuleToLookAt = module; }
  static Int_t GetModuleToLookAt()            { return fgModuleToLookAt;   }

  void MakeTracks(TObjArray *jhitlayersptr = 0);
  void KillModule(Int_t module = 6);

  virtual void DebugPrint(const Char_t *opt = "");

  virtual void Draw(const Char_t *opt="");
  virtual void Draw(Int_t  color, const Char_t *opt="");

private:
  void Update();

private:
  TObjArray     *fHitLayersPtr;    //  pointer to hitlayers array,
  Bool_t         fDone;            //! 1 if track finding done, 0 otherwise
  vector<Int_t>  fKill;            //! list of modules to kill
  JVTrackFinder *fTrackFinderPtr;  //! pointer to track finder
  static Int_t   fgLayerToLookAt;  //! layer to look at
  static Int_t   fgModuleToLookAt; //! module to look at

  ClassDef(JTrackMaker, 1)  // Raw Data class
};
#endif
