#ifdef __CINT__
typedef struct {
  int rcuid;
  int channel;
  int nb;
  int length;
  int time;
  int *data;
} CLUSTER;
#else
#include "JCluster.h"
#endif

void clusterTest() 
{
#ifdef __CINT__
  gSystem->Load("libGraf3d");
  gSystem->Load("libPhysics");
  gSystem->Load("libJREvent");
#endif

 //-- 
 // Prepare dummy raw data for a mutipeak cluster.
 //--
  CLUSTER cl;
  int data[] = {0,3,16,40,170,30,0,0,30,70,140,50,0,0,30,60,90,50,30};
  cl.data    = data;
  cl.length  = sizeof(data)/sizeof(int);
  cerr << "cl.length = " << cl.length << endl;

  cl.rcuid   = 0; // RCU ID
  cl.channel = 0; // channel number
  cl.nb      = 0; // ??
  cl.time    = 0; // start time of pulse

 //-- 
 // Test splitting a multi-peak cluster.
 //--
  JREvent   ev;                        // array to store all clusters
  TObjArray acs;                       // array to store trailing clusters
  if (cl.length > 0) ev.Add(new JCluster(&cl, &acs)); // create a cluster
  TIter nextacl(&acs,kIterBackward);                  // backward iterator
  JCluster* cp;
  while ((cp = static_cast<JCluster *>(nextacl()))) ev.Add(cp); // sorting

 //-- 
 // Print out
 //--
  cerr << "No. of mini-clusters in this pluse = " << ev.GetEntries() << endl;
  TIter next(&ev);
  while ((cp = static_cast<JCluster *>(next()))) {
	  cp->DebugPrint("detail");
  }
}
