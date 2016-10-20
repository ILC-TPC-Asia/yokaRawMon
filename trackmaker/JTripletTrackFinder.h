#ifndef JTRIPLETTRACKFINDER_H
#define JTRIPLETTRACKFINDER_H
#include "JVTrackFinder.h"

class JTripletTrackFinder : public JVTrackFinder {
public:
  JTripletTrackFinder(JTrackMaker *tmp = 0);
  virtual ~JTripletTrackFinder();

  virtual void   FindTracks  (TObjArray *hitlayers);
  virtual Bool_t AppendHit   (JTrack *tp, JHit *hp);
  virtual Bool_t AppendHitLoc(JTrack *tp, JHit *hp);

  static Int_t    GetMinNlayers () { return fgMinNlayers;  }
  static Int_t    GetMinNhits   () { return fgMinNhits;    }
  static Int_t    GetMaxMissing () { return fgMaxMissing;  }
  static Double_t GetCosThetaCut() { return fgCosThetaCut; }
  static Double_t GetZdiffCut   () { return fgZdiffCut;    }

  static void SetMinNlayers (Int_t    n) { fgMinNlayers  = n; }
  static void SetMinNhits   (Int_t    n) { fgMinNhits    = n; }
  static void SetMaxMissing (Int_t    n) { fgMaxMissing  = n; }
  static void SetCosThetaCut(Double_t x) { fgCosThetaCut = x; }
  static void SetZdiffCut   (Double_t x) { fgZdiffCut    = x; }

private:
  static Int_t    fgMinNlayers;    //! minimum # of layers
  static Int_t    fgMinNhits;      //! minimum # of hits
  static Int_t    fgMaxMissing;    //! maximum # of missing hit layer
  static Double_t fgCosThetaCut;   //! criterion for track-finding
                                   //  maximum cos-theta
  static Double_t fgZdiffCut;      //! z differene cut
 
  ClassDef(JTripletTrackFinder, 1) // abstract base for track finder
};
#endif
