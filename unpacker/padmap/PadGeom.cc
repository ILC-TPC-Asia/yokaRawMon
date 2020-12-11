// This code correspond Hardware channel to pad and position
// This code is based on the code writen by A.Sugiyama at Feb. 2009


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

#include "PadGeom.h"

using namespace std;

extern vector< vector<int> > v_Data;    // All hadware data -- global parameter


int PadGeom::GetPadInfo(int drorc, int rcu, int fec, int ch, int *Mod, int *Lay, int *Pad)
{
  int module, layer, pad, parity;
  int cable = ch/32;
  int pin = ch%32;

  for(vector<vector<int> >::const_iterator p=v_Data.begin(); p != v_Data.end();
      ++p){
    if(drorc == p->at(6) && rcu == p->at(5) && fec == p->at(4) && 
       cable == p->at(3)){
      for(int j=0; j < p->at(1); j++){
	module = p->at(0);
	parity = p->at(2);
	switch (parity) {
	case 0:
	  if(pin >= 0 && pin <= 7) {
	    layer = p->at(8);   // innerlayer
	    pad = p->at(9) + 15 - pin;
	  } else if(pin >= 8 && pin <= 15) {
	    layer = p->at(8);
	    pad = p->at(9)  + pin - 8;
	  } else if(pin >= 16 && pin <= 23) {
	    layer = p->at(7);   // outerlayer
	    pad = p->at(9) + pin - 16;
	  } else if(pin >= 24 && pin <= 31) {
	    layer = p->at(7);
	    pad = p->at(9) + 39 - pin;
	  }
	  break;
	case 1:
	  if(pin >= 0 && pin <= 7) {
	    layer = p->at(7);   // outerlayer
	    pad = p->at(9) + pin;
	  } else if(pin >= 8 && pin <= 15) {
	    layer = p->at(7);
	    pad = p->at(9) + 23 - pin;
	  } else if(pin >=16 && pin <= 23) {
	    layer = p->at(8);  // innerlayer
	    pad = p->at(9) +31 - pin;
	  } else if(pin >= 24 && pin <= 31) {
	    layer = p->at(8);
	    pad = p->at(9) + pin - 24;
	  }
	}

	*Mod = module;
	*Lay = layer;
	*Pad = pad;

	return 0;
      }
    }
  }
return 1;

}

int PadGeom::GetPadPost(int module, int layer, int pad, double *Xc, double *Yc)
{
  //module numbering from pad surface outer
  // left            right
  //       5     6
  //    2     3     4
  //        0     1
  //down
  //

  // set module parameters
 double offset_angle[7]={0.12485582,-0.021577304,0.2208493,0.07441581,-0.07184279, 0.15976241,0.013329281};
  double center_displace[7]={1676.115,1676.115,1503.615,1503.615,1503.615,1332.00,1332.00};
  double layer_min=1440.01;
  double local_x, local_y, temp_x, temp_y, new_x, new_y;
  //at the begining get local coordinate from layer/pad
  if(module > 6) return 10;

  if(PadGeom::GetPadGeom(layer, pad, &local_x, &local_y)) return 10;
  //local module has (0.0) at layer 0 pad 0 low_left edge
  //all coordinate must be rotated by offset_angle and late on shifted by
  //according to center dispacement
  temp_x = local_x;
  temp_y = local_y + layer_min;
  // rotate_r=sqrt(temp_x*temp_x+temp_y*temp_y);
  new_x = temp_x * cos(offset_angle[module]) - 
          temp_y * sin(offset_angle[module]);
  new_y = temp_y * cos(offset_angle[module]) +
          temp_x * sin(offset_angle[module]);

  // get (X, Y) on the center of a pad
  *Xc = new_x;
  *Yc = new_y - center_displace[module];

  return 0;
}

int PadGeom::GetPadGeom(int Layer, int Pad, double *xc, double *yc)
{
  // This function returns (X,Y) on the center of the pad, which is specified
  // by the input, layer and pad.
  // Caution: output (X,Y) is local coordinate of a module.

  //Set pad parameters
  int npad[2] = {176, 192};
  double effective_angle_padpitch[2]={0.000821484, 0.000753027};
  double layer_drpitch=5.36;
  double layer_min=1440.01;
  double space=0.1;

  // Define layer center
  double layer_center = layer_min + layer_drpitch * (Layer+0.5) - 0.5*space;
 
  // Define segment (upper, lower)
  int iseg = -1;
  if( Layer < 14 ) iseg = 0;
  else if( Layer >= 14 ) iseg = 1;
  //  cout << "Pad = " << Pad << ", Layer = " << Layer << endl;
  if( Pad >= npad[iseg] || Layer > 27 ) return 10;  // out of segments

  // Define pad offset for each module  
  double offset = Layer%2 ? 0.5 : 0.0;  // if(Layer%2)offset=0.5 else offset=0

  // Determine (X,Y) of the pad
  *xc = layer_center * sin(effective_angle_padpitch[iseg]*(Pad+offset+0.5))
        - 0.5 * space * cos(effective_angle_padpitch[iseg]*(Pad+offset+0.5));
  *yc = layer_center * (cos(effective_angle_padpitch[iseg]*(Pad+offset+0.5)))
        + 0.5 * space * sin(effective_angle_padpitch[iseg]*(Pad+offset+0.5))
        - layer_min;

  return 0;
}
