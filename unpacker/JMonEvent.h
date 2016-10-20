#ifndef __JMonEvent
#define __JMonEvent

#include <fstream>
#include <iostream>
#include "JMonFile.h"

using namespace std;

#define MAX_RCU_ID 3
#define ALTRO_SIZE 1024

typedef struct {
  u_int *data;
  int total_length;
  int header_length;
} RUNHEADER;

typedef struct {
  int totlen;
  int length;
  int blockid;
  int status;
  int error;
  int runnb;
  int runmode;
  unsigned int pcasettings;
  unsigned int runsettings;
} MONHEADER;


typedef struct {
  int totlen;
  int length;
  int blockid;
  int format;
  int runnb;
  int rundate;
  int runtime;
  unsigned int pcasettings;
} SORHEADER;

typedef struct {
  int totlen;
  int length;
  int blockid;
  int events;
  int rundate;
  int runtime;
} EORHEADER;

typedef struct {
  int totlen;
  int length;
  int blockid;
  int format;
  int runnb;
  int rundate;
  int runtime;
  int filenb;
  int eventnb;
} BOFHEADER;

typedef struct {
  int totlen;
  int length;
  int blockid;
  int eventnb;
  int rundate;
  int runtime;
  int filenb;
} EOFHEADER;

typedef struct {
  int length;
  int id;
  u_int blength;
  u_int format;
  u_int l1type;
  u_int evtid1;
  u_int evtid2;
  u_int subdet;
  u_int block;
  u_int status;
  u_int bunch;
  u_int tclassl;
  u_int tclassh;
  u_int roil;
  u_int roih;
  u_int *data;
} RCUHEADER;

typedef struct {
  int fwv;
  u_int paylength;
  u_int err_reg1;
  u_int err_reg2;
  u_int err_reg3;
  u_int err_reg4;
  u_int fecroa;
  u_int fecrob;
  u_int rdocfg1;
  u_int rdocfg2;
  u_int rcuid;
  u_int length;
} RCUTRAILER;

typedef struct {
  int totlen;
  int length;
  int blockid;
  int event;
  int dboxtime;
  int dboxevtn;
  int tluevtn;
} EVTHEADER;

typedef struct {
  int rcuid;
  int channel;
  int trailer;
  int length;
  int data[2048];
} CHANNEL;

typedef struct {
  int rcuid;
  int channel;
  int nb;
  int length;
  int time;
  int *data;
} CLUSTER;


typedef struct {
  RCUHEADER *rcud;
  u_int *eob;
  u_int *sob;
  int length;
  u_int mod5;
  int index;
  int first;
  int chfirst;
  int index0;
  int index1;
  int altro_trailer;
  int altro_address;
  int altro_length;
  int pos;
  int nbclusters;  
} CLFINDER;

class JMonEvent {
 private:
  CLFINDER clf;
  u_int *bodbuf; /* beginning of user data buffer */
  u_int *evtdata;
  int FormatVersion;
  int rcunb;
  CHANNEL chdata;
  CLUSTER cldata[1024];
  RCUHEADER hrcu[MAX_RCU_ID+1];
  RCUTRAILER rcut;
  JHeader rh;
  JEvent je;
  int thisevent;
  void *getFirstChannel(void);
  void *getFirstChannelReversed(void);
  void *getNextChannel(void);
  void *getNextChannelReversed(void);
  int getChannelData(void);
  int getChannelDataReversed(void);
  void *getFirstChannelNew(void);
  void *getNextChannelNew(void);
  int getChannelDataNew(void);
  RCUTRAILER *getRcuTrailerNew(int rcuid, int fwv);
  RCUTRAILER *getRcuTrailerOld(int rcuid, int fwv);

 public:
   JMonEvent(u_int *data);
  JMonEvent();
  ~JMonEvent();
  void Close();
  void CloseEvent();
  void setBuffer(u_int *data);
  void *getHeader(MONHEADER *mon,JHeader *jhead);
  void *getHeader(SORHEADER *sor,JHeader *jhead);
  void dumpHeader(SORHEADER *sor);
  void *getHeader(EORHEADER *eor,JHeader *jhead);
  void *getHeader(EVTHEADER *evt,JHeader *jhead);
  void dumpHeader(EVTHEADER *evt);
  void *getHeader(BOFHEADER *bof,JHeader *jhead);
  void *getHeader(EOFHEADER *eof,JHeader *jhead);
  void setFormatVersion(int version);
  JHeader *getRunHeader();
  void dumpRunHeader(JHeader *jh);
  void dumpBlock(JEvent *je);
  int getBlockId(JHeader *jhead);
  JEvent *getRcuEvent();
  RCUHEADER *getRcuHeader(int rcuid,JEvent *evt);
  void dumpRcuHeader(int rcuid);
  RCUTRAILER *getRcuTrailer(int rcuid, int fwv);
  void dumpRcuTrailer(int rcuid);
  int getRcuId(JEvent *evt);
  CHANNEL *getChannel(int rcuid);
  CLUSTER *getClusters(void);
  CHANNEL *getChannelReversed(int rcuid);
  CLUSTER *getClustersReversed(void);
  CHANNEL *getChannelNew(int rcuid);
  CHANNEL *getChannelReversedOld(int rcuid);
};

#endif
