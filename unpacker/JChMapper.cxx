#include "JChMapper.h"
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

JChMapper *JChMapper::fgInstancePtr = 0;
string     JChMapper::fgMapFile("map/channelmap_090307.txt");

JChMapper::JChMapper()
{
  for (int rcu=0; rcu<4; rcu++) {
    for (int ch=0; ch<8192; ch++) {
      fModule[rcu][ch] = -1;
      fLayer [rcu][ch] = -1;
      fPad   [rcu][ch] = -1;
    }
  }
  const int kMaxBuf = 1024;
  char   dummy[kMaxBuf];
  ifstream in(fgMapFile.data());
  while (!in.eof()) {
    in.getline(dummy,kMaxBuf);  
    if (dummy[0] == '#') continue;
    stringstream ins(dummy);
    int rcu, ch, module, layer, pad;
    ins >> rcu >> ch >> module >> layer >> pad;
    fModule[rcu][ch] = module;
    fLayer [rcu][ch] = layer;
    fPad   [rcu][ch] = pad;
  }
}

JChMapper::~JChMapper()
{
}

JChMapper *JChMapper::GetInstance()
{
  if (!fgInstancePtr) fgInstancePtr = new JChMapper;
  return fgInstancePtr;
}

#if 0
int JChMapper::GetLayer(int fec, int ch, int rcu)
{
  int chip = ch/16;
  int layer = 999999;
  if (fec == 1) {
    layer = chip;
  } else if (fec == 3) {
    layer = chip + 8;
  }
  return layer;
}

int JChMapper::GetPad(int fec, int ch, int rcu)
{
  int chip = ch/16;
  int pad  = ch%16;
  pad   = chip % 2 ? pad : (pad - 8 < 0 ? pad + 8 : pad - 8);
  pad   = pad < 8 ? pad : 23 - pad;
  int layer = 999999;
  if (fec == 1) {
    layer = chip;
  } else if (fec == 3) {
    layer = chip + 8;
  }
  if (layer < 4) {
    pad   = pad - 8 < 0 ? pad + 8 : pad - 8;
  }

  return pad;
}
#endif
int JChMapper::GetLayer(int fec, int ch, int rcu)
{
  //cerr << " fec: " << fec << " ch: " << ch << " rcu: " << rcu << endl;
  int ach = 128*fec + ch;

  return fLayer[rcu][ach];
}

int JChMapper::GetPad(int fec, int ch, int rcu)
{
  int ach = 128*fec + ch;
#if 1
  if ((rcu != 1 && rcu !=2) || (ach < 0 || ach > 3967)) {
	  cerr << "ERRER ::::::::::::::::::::::::::::::" << endl
	       << "RCU=" << rcu << " FEC=" << fec <<" Ch=" << ch << endl;
#endif
  
  }
  return fPad[rcu][ach];
}

void JChMapper::GetPadAddress(int rcu, int ch, int &module, int &layer, int &pad)
{
  if (rcu >= 4 || ch >= 8192) {
    module = -1;
    layer  = -1;
    pad    = -1;
    cerr << " ERROR >>>>>>>> "
         << " rcu = " << rcu 
         << " ch  = " << ch
         << " nonexistent!!" << endl;
  } else {
    module = fModule[rcu][ch];
    layer  = fLayer [rcu][ch];
    pad    = fPad   [rcu][ch];
  }
}
