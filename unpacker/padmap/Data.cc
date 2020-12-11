// This code stores the Hardware and geometry data to vector

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

#include "Data.h"

using namespace std;

vector< vector<int> > v_Data;    // All hadware data -- global parameter


int Data::GetData(char *HwCt, char *CtLP)
{

  vector< vector<int> > v_HwCt;  //HardwareConnect
  vector< vector<int> > v_CtLP;  //Connector-LayerPa

  // Open Hardware connection files 
  ifstream fp1, fp2;
  fp1.open(HwCt);
  //fp1.open("./HardwareConnect_20090713.txt");
  if (!fp1) {
    cout << "Can't open HardwareConnect.txt" << endl;
    return 10;
  }
  
  fp2.open(CtLP);
  //fp2.open("./Connector-LayerPad.txt");
  if (!fp2) {
    cout << "Can't open Connector-LayerPad.txt" << endl;
    return 10;
  }

  // Define local vector
  vector<int> v_lpara;

  // Read the data of  HardwareConnect file
  //read first two line which shows a number of connector
  string fline;
  getline(fp1, fline);
  getline(fp1, fline);

  stringstream firstline(fline);
  int Nconnector, i;
  firstline >> Nconnector;
  cout << "Number of connector = " << Nconnector << endl;

// read all lines in the file
  i = 0;
  while (i < Nconnector)
    {
      // read the next line from the file
      string line;
      getline(fp1, line);

      // the line as a stream so we can stream into the variables
      stringstream linestream(line);

      // check if the next line is a comment
      if(linestream.peek() == '/')
	{
	  continue;
	}

      // stream the data into the variables
      i++;
      v_lpara.clear();
      int kmodule, kconnector, kparity, kcable, kfec, krcu, kdrorc;
      linestream >> kmodule >> kconnector >> kparity >> kcable >> kfec >> krcu >> kdrorc;
      v_lpara.push_back(kmodule);
      v_lpara.push_back(kconnector);
      v_lpara.push_back(kparity);
      v_lpara.push_back(kcable);
      v_lpara.push_back(kfec);
      v_lpara.push_back(krcu);
      v_lpara.push_back(kdrorc);
      v_HwCt.push_back(v_lpara);
      //      cout << kmodule << " " <<  kconnector << " " <<  kparity << " " <<  kcable << " " <<  kfec << " "  << krcu << " "  <<  kdrorc << endl;
    }
  //cout << "Size = " << v_HwCt.size() << endl;
  //for (vector<vector<int> >::const_iterator p = v_HwCt.begin(); p != v_HwCt.end(); ++p) { 
  //vector< vector<int> >::const_reference p = v_HwCt.at(2);
  //  vector<int>::const_reference q = p.at(0);
  //cout << q << endl;
      
  //for (vector<int>::const_iterator q = p->begin(); q != p->end(); ++q) cout << *q << " "; cout << endl;
      
  //}
     // Read the data on Connector-LayerPad file
  //vector<haddat:condat> v_connect
  while(!fp2.eof())
	{
	  // read the next line from the file
	  string cline;
	  getline(fp2, cline);
           // the line as a stream so we can stream into the variables
          stringstream linestream2(cline);

          // check if the next line is a comment
          if(linestream2.peek() == '/')
	    {
	      continue;
	    }
	  v_lpara.clear();
          int connector, outerlayer, innerlayer, leftpad, rightpad;
	  linestream2 >> connector >> outerlayer >> innerlayer >> leftpad >> rightpad;
	  v_lpara.push_back(connector);
	  v_lpara.push_back(outerlayer);
	  v_lpara.push_back(innerlayer);
	  v_lpara.push_back(leftpad);
	  v_lpara.push_back(rightpad);
	  v_CtLP.push_back(v_lpara);
	  //cout << connector << " " <<  outerlayer << " " <<  innerlayer << " " <<  leftpad << " " <<  rightpad << endl;
	}

  //
  // Combine two vectors to one vector
  //
  vector<int> v_total;

    for (vector< vector<int> >::const_iterator p1 = v_HwCt.begin(); 
         p1 != v_HwCt.end(); ++p1) 
     {
       v_total.clear();
       v_total.push_back(p1->at(0));  // module #
       v_total.push_back(p1->at(1));  // connector #
       v_total.push_back(p1->at(2));  // parity #
       v_total.push_back(p1->at(3));  // Cable #
       v_total.push_back(p1->at(4));  // FEC #
       v_total.push_back(p1->at(5));  // RCU #
       v_total.push_back(p1->at(6));  // DRORC #
       int q1 = p1->at(1);
       for (vector< vector<int> >::const_iterator p2 = v_CtLP.begin();
            p2 != v_CtLP.end(); ++p2)
        {
	    if( q1 == p2->at(0))     // Same connector#
	      {
	      //cout << "This is same: " << q1 << " , " << p2->at(0) << endl;
		v_total.push_back(p2->at(1));  // outer layar #
		v_total.push_back(p2->at(2));  // inner layer #
		v_total.push_back(p2->at(3));  // left pad #
		v_total.push_back(p2->at(4));  // right pad #
	    break;
	      }
        }
       v_Data.push_back(v_total);
     }  
    /* for debug
    cout << v_Data.size() << endl;
    for (vector<vector<int> >::const_iterator k = v_Data.begin(); k != v_Data.end(); ++k) { 
    for (vector<int>::const_iterator kq = k->begin(); kq != k->end(); ++kq) cout << *kq << " "; cout << endl;
     }
    */
     
  return 0;
}
