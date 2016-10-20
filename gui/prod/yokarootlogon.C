{
#ifndef __CINT__
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
}
