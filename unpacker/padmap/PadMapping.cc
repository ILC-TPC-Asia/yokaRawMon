// This code makes the pad mapping file and pad postion file

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

#include "PadGeom.h"
#include "Data.h"

using namespace std;


int main(int argc, char *argv[])
{
  int channel, FEC, CHA, DRORC, RCU;
  int mod, lay, pad;
  double xpos, ypos;

  // determine the out files
  ofstream op1, op2;
  op1.open(argv[3]); 
  //op1.open("./channelmap_090713-test.txt");
  if(!op1) {
    cout << "Can't open channelmap output file" << endl;
  }
  // Header of channelmap output file
  op1 << "#/ Mapping list of hardware channel to soft ware channnel." << endl;
  op1 << "#/ soft ware channels are asigned by anti-clockwise from surface." << endl;
  op1 << "#/ designed for the data for Jul.08." << endl;
  op1 << "#/ \t RCU \t channel module layer \t pad" << endl;

  op2.open(argv[4]);
  //  op2.open("./ch2pos_090713-test.txt");
  if(!op2) {
    cout << "Can't open ch2pos output file" << endl;
  }
  //Header of ch2pos output file
  op2 << "#/ (X,Y) postion of each channel." << endl;
  op2 << "#/ (X,Y) position is on the global coordinate." << endl;
  op2 << "#/ designed for the data for Jul.08." << endl;
  op2 << "#/ \t RCU \t channel X-position \t Y-position" << endl;

  // data in Hardware txt file and connector txt file is combined a vector 
  Data theA;  
  theA.GetData(argv[1], argv[2]);

  PadGeom theB;
  // loop over the RCU
  for( int k=0; k <4; k++)
    {
      RCU = k;
      //  loop over maximum channel
      for( int i = 0; i < 50000; i++)
	{
	  channel = i;
	  FEC = channel/128;
	  CHA = channel%128;
	  DRORC = 0;

	  // Get connector parameters
	  int a = theB.GetPadInfo(DRORC, RCU, FEC, CHA, &mod, &lay, &pad);
	  if(a) continue;  // not match data
	  int b = theB.GetPadPost(mod, lay, pad, &xpos, &ypos);
	  if(b) continue;  // not match data

	  // write the data to the files
	  op1 << "\t " << RCU << "\t " << channel << "\t  " << mod << "\t  " << lay << "\t  " << pad << endl;
	  op2 << "\t " << RCU << "\t " << channel << "\t " << xpos << " \t " << ypos << endl;

	  /* for debug
	  cout << "mod = " << mod << ", Lay = " << lay << ", pad = " << pad  
	  << ", channel = " << channel << ", xpos = " << xpos 
	  << ", ypos = " << ypos << " , FEC= " << FEC << ", CHA = " << CHA << endl;
	  */
	  // Connetor parametor data to be stored on the files

	}
    }

  op1.close();
  op2.close();
  return 0;

}
