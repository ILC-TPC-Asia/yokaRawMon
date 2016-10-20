#include <cstdlib>
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cmath>
#include "JMonEvent.h"
#include "JPed.h"



JPed::JPed(int rcuid)
{
  memset(&pc,0,sizeof(PEDCALC));
  memset(&ps,0,sizeof(PEDCALC));
  pc.rcuid = rcuid;
}


JPed::~JPed()
{
}

void JPed::clearPedestals()
{
  int rcuid;
  rcuid = pc.rcuid;
  memset(&pc,0,sizeof(PEDCALC));
  pc.rcuid = rcuid;
}

void JPed::fillPedestalCalc(CLUSTER *cl)
{
  int j;
  int adtime;
  adtime = cl->time;
  for ( j = 0; j < cl->length; j++,adtime--) {
    if (!ps.rms[cl->channel]) {
	if (adtime > 50) {
	    pc.sum[cl->channel] += cl->data[j];
	    pc.sumq[cl->channel] += (cl->data[j]*cl->data[j]);
	    pc.val[cl->channel]++;
	}
    }
    else if (abs(cl->data[j] - (int)ps.mean[j]) < (MAX_PED_RMS * ps.rms[j])) {
      pc.sum[cl->channel] += cl->data[j];
      pc.sumq[cl->channel] += (cl->data[j]*cl->data[j]);
      pc.val[cl->channel]++;
    }
  }
}


void JPed::makePedestalCalc()
{
  int j;
  for (j = 0; j < CHANNELS_RCU; j++) {
    if (pc.val[j]) {
      pc.mean[j] = (float) pc.sum[j]/(float)pc.val[j];
      pc.rms[j] = sqrt((pc.sumq[j]/(float)pc.val[j]) - (pc.mean[j]*pc.mean[j]));
    }
    else {
      pc.mean[j] = 1023.0;
      pc.rms[j]  = 0.0;
    }
  }
}


/* Write the pedestal file
format:
# RCUID <id>
channel    <meaninteger> <meanfloat> <rmsfloat>
*/
int JPed::writePedestalFile(const char *fname)
{
  int j;
  char outbuf[512];
  ofstream fout;
  fout.open(fname,ios::out);
  if (!fout) {
    cerr << "Cannot open pedestal file: " << fname << endl;
    return -1;
  }
  sprintf(outbuf,"#RCUID %d\n",pc.rcuid);
  fout.write(outbuf,strlen(outbuf));
  for (j = 0; j < CHANNELS_RCU; j++) {
      sprintf(outbuf,"%4d %4d %10.3f %10.3f %d\n",j,(int)pc.mean[j],pc.mean[j],pc.rms[j],pc.val[j]);
      fout.write(outbuf,strlen(outbuf));
  }
  fout.close();
  return 0;
}

int JPed::writePedestalFile(const char *fname, const char *comment)
{
  int j;
  char outbuf[512];
  ofstream fout;
  fout.open(fname,ios::out);
  if (!fout) {
    cerr << "Cannot open pedestal file: " << fname << endl;
    return -1;
  }
  sprintf(outbuf,"#RCUID %d\n",pc.rcuid);
  fout.write(outbuf,strlen(outbuf));
  fout.write(comment,strlen(comment));
  for (j = 0; j < CHANNELS_RCU; j++) {
      sprintf(outbuf,"%4d %4d %10.3f %10.3f %d\n",j,(int)pc.mean[j],pc.mean[j],pc.rms[j],pc.val[j]);
      fout.write(outbuf,strlen(outbuf));
  }
  fout.close();
  return 0;
}

/* read the pedestal file into storage buffer */
int JPed::readPedestalFile(const char *fname, int rcuid)
{
  int l,ch,value;
  float mean,rms;
  ifstream fin;
  char indata[1024];
  ps.rcuid = rcuid;
  fin.open(fname,ios::in);
  if (!fin) {
    cerr << "Cannot open pedestal file: " << fname << endl;
    return -1;
  }
  cout << "Reading pedestal file:" << fname << endl;
  l = 1;
  while (!fin.eof()) {
    fin.getline(indata,sizeof(indata));
    if (indata[0] != '#') {
      stringstream in(indata);
      in >> ch >> value >> mean >> rms;
      if ((ch >= 0) && (ch < CHANNELS_RCU)) {
	ps.val [ch] = value;
	ps.mean[ch] = mean;
	ps.rms [ch] = rms;  
      }
      else return l;
    }
    l++;
  }
  fin.close();
  cout << "Read pedestal file:" << fname << endl;

  return 0;
}

/* Copy values calculated to storage buffer */
void JPed::copyPedestalValues()
{
  memcpy(&ps,&pc,sizeof(PEDCALC));
}

int JPed::GetPedestal(int ch)
{
  return ps.val[ch];
}
