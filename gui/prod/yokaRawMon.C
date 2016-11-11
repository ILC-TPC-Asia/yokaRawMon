//
// $Id: yokaRawMon.C,v 1.8 2012/05/03 09:41:47 fujiik Exp $
//

class GEMDisplay;

//int yokaRawMon(int run = 7049, int part = 0)
//int yokaRawMon(int run = 17180, int part = 0)
//int yokaRawMon(int run = 18594, int part = 0)
int yokaRawMon(int run = 19667, int part = 0)
{
#ifdef __CINT__
  gSystem->Load("libPhysics.so");
  gSystem->Load("libGraf3d.so");
  gSystem->Load("libTree.so");
  gSystem->Load("$(KALTESTROOT)/lib/libS4Utils");
  gSystem->Load("$(KALTESTROOT)/lib/libS4Kalman");
  gSystem->Load("$(KALTESTROOT)/lib/libS4Geom");
  gSystem->Load("$(KALTESTROOT)/lib/libS4KalTrack");
  gSystem->Load("../../kalman/lib/libEXKern");
  gSystem->Load("../../unpacker/libJREvent");
  gSystem->Load("../../kalman/lib/libEXTPC");
  gSystem->Load("../../hitmaker/libJHits");
  gSystem->Load("../../trackmaker/libJTracks");
  gSystem->Load("libGEMDisplay");
#endif

  GEMDisplay *gemdisplay = GEMDisplay::GetInstance();
  gemdisplay->Add("TrackMonitor3D");
#ifdef __CINT
  gemdisplay->Add("HitMonitor3D");
#if 0
  gemdisplay->Add("PadMonitor1");
  gemdisplay->Add("PadMonitor3");
  gemdisplay->Add("PadMonitor6");
  gemdisplay->Add("HitMonitor");
#endif
#endif
  gemdisplay->SetInputDir("../../unpacker/root/");

  gemdisplay->Initialize();
  gemdisplay->GoToEvent(0, run, part);

  return 0;
}
