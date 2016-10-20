
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "JMonEvent.h"
#include "JMonFile.h"

//#define __DEBUG_EVENT


// Constructor dummy one
JMonEvent::JMonEvent(u_int *data)
{
  int j;
//  cldata = 0;
//  chdata.data = 0;
  bodbuf = data;
  FormatVersion = 0;
  rcut.fwv = 0;
  for (j = 0; j <= MAX_RCU_ID; j++) hrcu[j].id = -1;
}

JMonEvent::JMonEvent()
{
  int j;
//  cldata = 0;
//  chdata.data = 0;
  bodbuf = 0;
  FormatVersion = 0;
  for (j = 0; j <= MAX_RCU_ID; j++) hrcu[j].id = -1;
}

JMonEvent::~JMonEvent()
{
//  if (cldata)   delete cldata;
//  cldata = 0;
//  chdata.data = 0;
}

int JMonEvent::getBlockId(JHeader *jhead)
{
  return jhead->data[0];
}

int JMonEvent::getRcuId(JEvent *jevt)
{
  return jevt->data[0];
}

void JMonEvent::Close()
{
//  if (cldata) {
    //    puts("CLOSE DEL CLDATA");
//    delete cldata;
//    cldata = 0;
//  }
//  if (chdata.data) {
    //    printf("CLOSE DEL CHDATA %d\n", chdata.channel);
//    chdata.data = 0;
//  }
}

void JMonEvent::setBuffer(u_int *data)
{
  bodbuf = data;
}

void JMonEvent::setFormatVersion(int format) 
{
    FormatVersion = format; 
    cerr << "FORMAT VERSION = " << FormatVersion << endl;
}


JHeader *JMonEvent::getRunHeader()
{
  evtdata = bodbuf;
  rh.total_length = evtdata[0];
  rh.header_length = evtdata[1];
  rh.data = &evtdata[2];
  return &rh;
}

void JMonEvent::dumpRunHeader(JHeader *jh)
{
  int j;
  printf("TOTLEN %d HEADLEN %d\n",jh->total_length, jh->header_length);
  for (j = 0; j <= jh->header_length; j++) {
    printf("%04d: %08X\n",j,jh->data[j]);
  }
}


RCUHEADER *JMonEvent::getRcuHeader(int rcuid,JEvent *evt)
{
  RCUHEADER *rcu;
  u_int *ptr = (u_int *) evt->data;
  rcu = &hrcu[rcuid];
  rcu->length = evt->total_length;
  if ((rcuid<0) || (rcuid>MAX_RCU_ID)) return NULL;
  rcu->id = *ptr++;
  rcu->blength = *ptr++;
  rcu->evtid1 = *ptr & 0xFFF;
  rcu->l1type = (*ptr >> 16) & 0xFF;
  rcu->format = (*ptr++ >> 24) & 0xFF;
  rcu->evtid2 = *ptr++ & 0xFFFFFF;
  rcu->subdet = *ptr & 0xFFFFFF;
  rcu->block = (*ptr++ >> 24) & 0xFF;
  rcu->bunch = *ptr & 0xFFF;
  rcu->status = (*ptr++ >> 12) & 0xFFFF;
  rcu->tclassl = *ptr++;
  rcu->tclassh = *ptr & 0x3FFFF;
  rcu->roil = (*ptr++ >> 28) & 0xF;
  rcu->roih = *ptr++;
  rcu->data = ptr;
  rcu->length -= 9;
  clf.chfirst = 1;
  return rcu;
}

/* Get the RCU firmware trailers for version fwv */
RCUTRAILER *JMonEvent::getRcuTrailerNew(int rcuid, int fwv)
{
  RCUHEADER *rcu;
  u_int j;
  u_int code;
  u_int data;
  u_int *ptr;
  rcut.length = 0;
  rcut.fwv = fwv;
  rcu = &hrcu[rcuid];
  if ((rcuid<0) || (rcuid>MAX_RCU_ID)) return NULL;
  if (fwv == 2) {
    ptr = rcu->data + rcu->length  - 1; // Points code 9 = length
    rcut.length = 9;
    for (j = 0; j < rcut.length; j++) {
      data = *ptr--;
      code = (data >> 26) & 0xF;
      switch (code) {
      case 0x00: rcut.paylength = data & 0x3FFFFF; break;
      case 0x01: rcut.err_reg1 = data & 0x3FFFFF; break;
      case 0x02: rcut.err_reg2 = data & 0x3FFFFF; break;
      case 0x03: rcut.err_reg3 = data & 0x3FFFFF; break;
//      case 0x04: rcut.err_reg4 = data & 0x3FFFFF; break;
      case 0x04: rcut.fecroa = data & 0x3FFFFF; break;
      case 0x05: rcut.fecrob = data & 0x3FFFFF; break;
      case 0x06: rcut.rdocfg1 = data & 0x3FFFFF; break;
      case 0x07: rcut.rdocfg2 = data & 0x3FFFFF; break;
      case 0x08: rcut.rcuid = data & 0x3FFFFF; break;
     }
    }
// 20090123 Dump trailer if readout error
    if (((rcut.err_reg2 & 0x3FFFFF) != 0) ||  ((rcut.err_reg1 & 0x3FFFFF) != 0)) {
        printf("RCU TRAILER for event %d\n",thisevent);
        dumpRcuTrailer(rcuid);
    }
  }
  else {
    return NULL;
  }
  rcu->length -= rcut.length;
  return &rcut;
}


RCUTRAILER *JMonEvent::getRcuTrailer(int rcuid, int fwv)
{
    RCUTRAILER *trail; 
    trail = NULL; 
    
    if (FormatVersion == 0) trail = getRcuTrailerOld(rcuid,fwv);
    else if (FormatVersion == 1) trail = getRcuTrailerNew(rcuid,fwv);
    return trail;
}

/* Get the RCU firmware trailers for version fwv */
RCUTRAILER *JMonEvent::getRcuTrailerOld(int rcuid, int fwv)
{
  RCUHEADER *rcu;
  u_int j;
  u_int code;
  u_int data;
  u_int *ptr;
  rcut.length = 0;
  rcut.fwv = fwv;
  rcu = &hrcu[rcuid];
  if ((rcuid<0) || (rcuid>MAX_RCU_ID)) return NULL;
  if (fwv == 2) {
    ptr = rcu->data + rcu->length  - 1; // Points code 9 = length
    data = *ptr--;
    code = (data >> 26) & 0x3F;
    if (code == 0x2A) rcut.length = data & 0x3F;
    for (j = 1; j < rcut.length; j++) {
      data = *ptr--;
      code = (data >> 26) & 0x3F;
      switch (code) {
      case 0x00: rcut.paylength = data & 0x3FFFFF; break;
      case 0x01: rcut.err_reg1 = data & 0x3FFFFF; break;
      case 0x02: rcut.err_reg2 = data & 0x3FFFFF; break;
      case 0x03: rcut.err_reg3 = data & 0x3FFFFF; break;
      case 0x04: rcut.err_reg4 = data & 0x3FFFFF; break;
      case 0x05: rcut.fecroa = data & 0x3FFFFF; break;
      case 0x06: rcut.fecrob = data & 0x3FFFFF; break;
      case 0x07: rcut.rdocfg1 = data & 0x3FFFFF; break;
      case 0x08: rcut.rdocfg2 = data & 0x3FFFFF; break;
     }
    }
// 20090123 Dump trailer if readout error
    if (((rcut.err_reg2 & 0x3FFFFF) != 0) ||  ((rcut.err_reg1 & 0x3FFFFF) != 0)) {
	printf("RCU TRAILER for event %d\n",thisevent);
	dumpRcuTrailer(rcuid);
    }
  }
  else {
    return NULL;
  }
  rcu->length -= rcut.length;
  return &rcut;
}

void JMonEvent::dumpRcuTrailer(int rcuid)
{
  if ((rcuid<0) || (rcuid>MAX_RCU_ID)) return;
  printf("RCU TRAILER DUMP for RCU ID    %3d\n",rcuid);
  printf("==================================\n");
  printf("FIRMWARE VERSION                  %d\n",rcut.fwv);
  printf("PAYLOAD LENGTH                    %d\n",rcut.paylength);
  printf("ERR_REG1                          %08X\n",rcut.err_reg1);
  printf("ERR_REG2                          %08X\n",rcut.err_reg2);
  printf("ERR_REG3                          %08X\n",rcut.err_reg3);
  printf("ERR_REG4                          %08X\n",rcut.err_reg4);
  printf("FEC_RO_A                          %08X\n",rcut.fecroa);
  printf("FEC_RO_B                          %08X\n",rcut.fecrob);
  printf("RDO_CFG1                          %08X\n",rcut.rdocfg1);
  printf("RDO_CFG2                          %08X\n",rcut.rdocfg2);
  printf("TRAILER LENGTH                    %d\n",rcut.length);
}

void JMonEvent::dumpRcuHeader(int rcuid)
{
  RCUHEADER *rcu;
  if ((rcuid<0) || (rcuid>MAX_RCU_ID)) return;
  rcu = &hrcu[rcuid];
  printf("RCU HEADER DUMP for RCU ID     %3d\n",rcuid);
  printf("==================================\n");
  printf("RCU ID                         %d\n",rcu->id);
  printf("DATA LENGTH                    %d\n",rcu->length);
  printf("BLOCK LENGTH                   %d\n",rcu->blength);
  printf("Event ID 1 (bunch crossing)    %d\n",rcu->evtid1);
  printf("L1 trigger message             %d\n",rcu->l1type);
  printf("Format version                 %d\n",rcu->format);
  printf("Event ID 2 (orbit number)      %d\n",rcu->evtid2);
  printf("Participating sub-detectors    %d\n",rcu->subdet);
  printf("Block attributes               %d\n",rcu->block);
  printf("Mini-event ID (bunch crossing) %d\n",rcu->bunch);
  printf("Status & error bits            %X\n",rcu->status);
  printf("Trigger classes low            %d\n",rcu->tclassl);
  printf("Trigger classes high           %d\n",rcu->tclassh);
  printf("ROI low                        %d\n",rcu->roil);
  printf("ROI high                       %d\n",rcu->roih);
  printf("=================================\n");
}

JEvent *JMonEvent::getRcuEvent()
{
  u_int *ptr = evtdata;
  if (rcunb == 0) {
    ptr =  evtdata + rh.header_length + 2;
  }
  rcunb++;
  je.total_length = *ptr++;
  je.data = ptr;
  // evtdata = ptr + je.total_length + 1;
  evtdata = ptr + je.total_length;
  return &je;
}

void JMonEvent::dumpBlock(JEvent *je)
{
    int j,max;
    u_int *ptr = je->data;
    printf("LENGTH :%d\n",je->total_length);
    max = je->total_length;
    if (je->total_length > 100) max = 100;
    for(j=1; j<= max;j++) {
	printf("%08X ",*ptr++);
	if ((j % 8) == 0) printf("\n");
    }
    printf("\n");
}

void *JMonEvent::getHeader(MONHEADER *mon,JHeader *jhead)
{
  mon->totlen = jhead->total_length;
  mon->length = jhead->header_length;
  mon->blockid = jhead->data[0];
  mon->status = jhead->data[1];
  mon->error = jhead->data[2];
  mon->runnb = jhead->data[3];
  mon->runmode = jhead->data[4];
  mon->pcasettings = jhead->data[5];
  return mon;
}

void *JMonEvent::getHeader(SORHEADER *sor,JHeader *jhead)
{
  sor->totlen = jhead->total_length;
  sor->length = jhead->header_length;
  sor->blockid = jhead->data[0];
  sor->format = jhead->data[1];
//  FormatVersion = jhead->data[1];
  if (sor->format >= 1) {
    sor->runnb = jhead->data[2];
    sor->rundate = jhead->data[3];
    sor->runtime = jhead->data[4];
    sor->pcasettings = jhead->data[5];
  }
  return sor;
}
void JMonEvent::dumpHeader(SORHEADER *sor)
{
  printf("TOTLEN %d\n",sor->totlen);
  printf("LENGTH %d\n",sor->length);
  printf("BLOCKID %d\n",sor->blockid);
  printf("FORMAT %d\n",sor->format);
  printf("RUNNB %d\n",sor->runnb);
  printf("RUNDATE %04d-%02d-%02d\n",
	 ((sor->rundate >> 16) & 0xFFFF),
	 ((sor->rundate >> 8) & 0xFF),
	 ((sor->rundate) & 0xFF));
  printf("RUNTIME %02d-%02d-%02d\n",
	 ((sor->runtime >> 16) & 0xFF),
	 ((sor->runtime >> 8) & 0xFF),
	 ((sor->runtime) & 0xFF));
  printf("PCA SHIFTREG %X DAC %d\n",(sor->pcasettings >> 16),
	 (sor->pcasettings & 0xFFF));
  return;
}

void *JMonEvent::getHeader(EORHEADER *eor,JHeader *jhead)
{
  eor->totlen = jhead->total_length;
  eor->length = jhead->header_length;
  eor->blockid = jhead->data[0];
  if (FormatVersion >= 1) {
    eor->events = jhead->data[1];
    eor->rundate = jhead->data[2];
    eor->runtime = jhead->data[3];
  } 
  return eor;
}

void *JMonEvent::getHeader(EVTHEADER *evt,JHeader *jhead)
{
  evt->totlen = jhead->total_length;
  evt->length = jhead->header_length;
  evt->blockid = jhead->data[0];
  evt->event = jhead->data[1];
  thisevent = jhead->data[1];
  evt->dboxtime = jhead->data[2];
  evt->dboxevtn = jhead->data[3];
  evt->tluevtn = jhead->data[4];
  rcunb = 0;
  return evt;
}

void JMonEvent::dumpHeader(EVTHEADER *evt)
{
  printf("TOTLEN %d\n",evt->totlen);
  printf("LEN %d\n",evt->length);
  printf("BLOCKID %X\n",evt->blockid);
  printf("EVENTNB %d (%X)\n",evt->event,evt->event);
  printf("DBOXTIME %X\n",evt->dboxtime);
  printf("DBOXEVTN %X\n",evt->dboxevtn);
  printf("TLUEVTN %X\n",evt->tluevtn);
  return;
}

void *JMonEvent::getHeader(BOFHEADER *bof,JHeader *jhead)
{
  bof->totlen = jhead->total_length;
  bof->length = jhead->header_length;
  bof->blockid = jhead->data[0];
  bof->format = jhead->data[1];
//  FormatVersion = jhead->data[1];
  if (bof->format >= 1) {
    bof->runnb = jhead->data[2];
    bof->rundate = jhead->data[3];
    bof->runtime = jhead->data[4];
    bof->filenb = jhead->data[5];
    bof->eventnb = jhead->data[6];
  }

  return bof;
}
void *JMonEvent::getHeader(EOFHEADER *eof,JHeader *jhead)
{
  eof->totlen = jhead->total_length;
  eof->length = jhead->header_length;
  eof->blockid = jhead->data[0];
  if (FormatVersion >= 1) {
    eof->eventnb = jhead->data[1];
    eof->rundate = jhead->data[2];
    eof->runtime = jhead->data[3];
    eof->filenb = jhead->data[4];
  } 
  return eof;
}


void *JMonEvent::getFirstChannel(void)
{
  clf.index0 = *clf.eob--;
  clf.index1 = *clf.eob--;
#ifdef __DEBUG_EVENT
  printf("%s: INDEX0 %08X INDEX1 %08X\n",__FUNCTION__,clf.index0,clf.index1);
#endif
  clf.length -= 2;
  clf.first = 1;
  clf.altro_length = 0;
  if (clf.mod5 == 0) {
      clf.altro_address = ((clf.index0 & 0xF) << 8)| ((clf.index1 >> 24) & 0xFF);
    clf.altro_length = (clf.index0 >> 8) & 0x3FF;
    clf.pos = 1;
  }
  else if (clf.mod5 == 2) {
    // 20090121 - correct FF -> FFF
    clf.altro_address = clf.index1 & 0xFFF;
    clf.altro_length = (clf.index1 >> 16) & 0x3FF;
    clf.pos = 0;
  }
  else if (clf.mod5 == 3) {
    clf.altro_address = (clf.index1 >> 8) & 0xFFF;
    clf.altro_length = ((clf.index0 & 0x3) << 8) | ((clf.index1 >> 24) & 0xFF);
    clf.pos = 3;
  }
  else if (clf.mod5 == 4) {
    clf.altro_address = (clf.index1 >> 16) & 0xFFF;
    clf.altro_length = clf.index0 & 0x3FF;
    clf.index0 = clf.index1;
    clf.pos = 2;
  }
  chdata.rcuid = clf.rcud->id;
  chdata.channel = clf.altro_address;
  chdata.length = clf.altro_length;
#ifdef _DEBUG_EVENT
  printf("ALTROADDR %d ALTROLEN %d pos %d\n",clf.altro_address,clf.altro_length,clf.pos);
#endif
  clf.chfirst = 0;
  fflush(stdout);
  return NULL;
}

void *JMonEvent::getFirstChannelReversed(void)
{
  // sob points to next location from the start of buffer
  clf.index0 = *clf.sob++;
  clf.index1 = *clf.sob++;
#ifdef __DEBUG_EVENT
  printf("%s: INDEX0 %08X INDEX1 %08X\n",__FUNCTION__,clf.index0,clf.index1);
#endif
  clf.length -= 2;
  clf.first = 1;
  clf.altro_length = 0;
  clf.altro_trailer = ((clf.index0 >> 26) & 0x3F) | ((clf.index1 & 0xFF) << 6); // 0x2AAA or 0x2AEE
  clf.altro_address = clf.index0 & 0xFFF;
  clf.altro_length = (clf.index0 >> 16) & 0x3FF;
  clf.pos = 1;
  chdata.rcuid = clf.rcud->id;
  chdata.trailer = clf.altro_trailer;
  chdata.channel = clf.altro_address;
  chdata.length = clf.altro_length;
#ifdef __DEBUG_EVENT
  printf("ALTROTRAILER = %X ALTROADDR %d ALTROLEN %d pos %d\n",clf.altro_trailer,clf.altro_address,clf.altro_length,clf.pos);
#endif
  clf.chfirst = 0;
  fflush(stdout);
  return NULL;
}

void *JMonEvent::getFirstChannelNew(void)
{
  // sob points to next location from the start of buffer
  clf.index0 = *clf.sob++;
#ifdef __DEBUG_EVENT
  printf("%s: INDEX0 %08X\n",__FUNCTION__,clf.index0);
#endif
  clf.length--;
  clf.first = 1;
  clf.altro_length = 0;
  clf.altro_trailer = 0x2AAA;
  if (clf.index0 & 0x20000000) clf.altro_trailer = 0x2AEE; // error bit is set
  clf.altro_address = clf.index0 & 0xFFF;
  clf.altro_length = (clf.index0 >> 16) & 0x3FF;
  clf.pos = 1;
  chdata.rcuid = clf.rcud->id;
  chdata.trailer = clf.altro_trailer;
  chdata.channel = clf.altro_address;
  chdata.length = clf.altro_length;
#ifdef __DEBUG_EVENT
  printf("%s: EVENT %d ALTROTRAILER = %X ALTROADDR %d ALTROLEN %d pos %d\n",__FUNCTION__,thisevent,clf.altro_trailer,clf.altro_address,clf.altro_length,clf.pos);
#endif
  clf.chfirst = 0;
  fflush(stdout);
  return NULL;
}


int JMonEvent::getChannelData()
{
  int j,k,l;
  unsigned int tword[4];
  int mlength = clf.altro_length;
  int clength = 0;
//  if (chdata.data) {
    //    printf("DELETE CHDATA.DATA %d ch %d\n",clf.altro_length, chdata.channel);
//    chdata.data = 0;
//  }
//  chdata.data = &altrobuffer;
//  if (chdata.data == 0) {
//    printf("%s new CHDATA buffer exceeded\n",__FUNCTION__);
//    return -1;
//  }
  while(clf.altro_length) {
    if (clf.length < 0 ) {
      printf("%s %d *ERROR* cluster length < 0\n",__FUNCTION__,thisevent);
      return -1;
    }
    switch(clf.pos) {
    case 0:
      clf.index0 = *clf.eob--;
      clf.index1 = *clf.eob--;
      tword[0] = (clf.index0 >> 22) & 0x3FF;
      tword[1] = (clf.index0 >> 12) & 0x3FF;
      tword[2] = (clf.index0 >> 2) & 0x3FF;
      tword[3] = ((clf.index0 & 0x3) << 8) | ((clf.index1 >> 24) & 0xFF);
      clf.pos = 1;
      clf.length -= 2;
      break;
    case 1:
      clf.index0 = *clf.eob--;
      tword[0] = (clf.index1 >> 14) & 0x3FF;
      tword[1] = (clf.index1 >> 4) & 0x3FF;
      tword[2] = ((clf.index1 & 0xF) << 6) | ((clf.index0 >> 26) & 0x3F);
      tword[3] = (clf.index0 >> 16) & 0x3FF;
      clf.pos = 2;
      clf.length--;
      break;
    case 2:
      clf.index1 = *clf.eob--;
      tword[0] = (clf.index0 >> 6) & 0x3FF;
      tword[1] = ((clf.index0 & 0x3F) << 4) | ((clf.index1 >> 28) & 0xF);
      tword[2] = (clf.index1 >> 18) & 0x3FF;
      tword[3] = (clf.index1 >> 8) & 0x3FF;
      clf.pos = 3;
      clf.length--;
      break;
    case 3:
      clf.index0 = *clf.eob--;
      tword[0] = ((clf.index1 & 0xFF) << 2) | ((clf.index0 >> 30) & 0x3);
      tword[1] = (clf.index0 >> 20) & 0x3FF;
      tword[2] = (clf.index0 >> 10) & 0x3FF;
      tword[3] = clf.index0 & 0x3FF;
      clf.pos = 0;
      clf.length--;
      break;
    }
    j = 0;
    if (clf.first) {
#ifdef __DEBUG_EVENT
      for(j=0;tword[j] == 0x2AA;j++) printf("%X",tword[j]);
      printf("\n%s J= %d ALTROLENGTH %d\n",__FUNCTION__,j,clf.altro_length);
      fflush(stdout);
#else 
      for(j=0;tword[j] == 0x2AA;j++);
#endif
      clf.first = 0;
    }
    k = 4;
    if (clf.altro_length < 4) k = clf.altro_length;
    //	printf("J= %d K= %d ALTROLENGHT %d\n",j,k,altro_length);
    for (l=j; l < k; l++) {
      chdata.data[clength++] = tword[l];
      if (clength > mlength) printf("CLENGTH %d ALTRO %d\n",clength,clf.altro_length);
      clf.altro_length--;
    }
#ifdef __DEBUG_EVENT
    printf("%s ALTROLENGTH %d POS %d\n",__FUNCTION__,clf.altro_length,clf.pos);
    fflush(stdout);
#endif
  }
  return 0;
}


int JMonEvent::getChannelDataReversed(void)
{
  int j,k,l;
  unsigned int tword[4];
  int mlength = clf.altro_length;
  int clength = 0;
//  if (chdata.data) {
    //    printf("DELETE CHDATA.DATA %d ch %d\n",clf.altro_length, chdata.channel);
//    chdata.data = 0;
//  }
//  chdata.data = &altrobuffer;
//  if (chdata.data == 0) {
//    printf("%s new CHDATA buffer exceeded\n",__FUNCTION__);
//    return -1;
//  }
  while(clf.altro_length) {
    if (clf.length < 0 ) {
      printf("%s %d *ERROR* cluster length < 0\n",__FUNCTION__,thisevent);
      return -1;
    }
    switch(clf.pos) {
    case 0:
      clf.index0 = *clf.sob++;
      clf.index1 = *clf.sob++;
      tword[3] = clf.index0 & 0x3FF;
      tword[2] = (clf.index0 >> 10) & 0x3FF;
      tword[1] = (clf.index0 >> 20) & 0x3FF;
      tword[0] = ((clf.index0 >> 30) & 0x3) | ((clf.index1 & 0xFF) << 2);
//      printf("%s I1 %X  I0 %X I2 %X POS %d 0 = %X 1 = %X 2 = %X 3 = %X\n",__FUNCTION__,clf.index1,clf.index0,*(clf.sob+1),clf.pos,tword[0],tword[1],tword[2],tword[3]);
      clf.pos = 1;
      clf.length -= 2;
      break;
    case 1:
      clf.index0 = *clf.sob++;
      tword[3] = (clf.index1 >> 8) & 0x3FF;
      tword[2] = (clf.index1 >> 18) & 0x3FF;
      tword[1] = ((clf.index1 >> 28) & 0xF) | ((clf.index0 & 0x3F) << 4);
      tword[0] = (clf.index0 >> 6) & 0x3FF;
//      printf("%s I1 %X  I0 %X I2 %X POS %d 0 = %X 1 = %X 2 = %X 3 = %X\n",__FUNCTION__,clf.index1,clf.index0,*(clf.sob+1),clf.pos,tword[0],tword[1],tword[2],tword[3]);
      clf.pos = 2;
      clf.length--;
      break;
    case 2:
      clf.index1 = *clf.sob++;
      tword[3] = ((clf.index0 >> 16) & 0x3FF);
      tword[2] = ((clf.index0 >> 26) & 0x3F) | ((clf.index1 & 0xF) << 6);
      tword[1] = (clf.index1 >> 4) & 0x3FF;
      tword[0] = (clf.index1 >> 14) & 0x3FF;
//      printf("%s I1 %X  I0 %X I2 %X POS %d 0 = %X 1 = %X 2 = %X 3 = %X\n",__FUNCTION__,clf.index1,clf.index0,*(clf.sob+1),clf.pos,tword[0],tword[1],tword[2],tword[3]);
      clf.pos = 3;
      clf.length--;
      break;
    case 3:
      clf.index0 = *clf.sob++;
      tword[3] = ((clf.index1  >> 24 ) & 0xFF) | ((clf.index0 & 0x3) << 8);
      tword[2] = (clf.index0 >> 2) & 0x3FF;
      tword[1] = (clf.index0 >> 12) & 0x3FF;
      tword[0] = (clf.index0 >> 22) & 0x3FF;
//      printf("%s I1 %X  I0 %X I2 %X POS %d 0 = %X 1 = %X 2 = %X 3 = %X\n",__FUNCTION__,clf.index1,clf.index0,*(clf.sob+1),clf.pos,tword[0],tword[1],tword[2],tword[3]);
      clf.pos = 0;
      clf.length--;
      break;
    }
    j = 0;
    if (clf.first) {
#ifdef __DEBUG_EVENT
      for(j=0;(tword[j] == 0x2AA);j++) printf("%X",tword[j]);
      printf("\n%s J= %d ALTROLENGTH %d\n",__FUNCTION__,j,clf.altro_length);
      fflush(stdout);
#else 
      for(j=0;tword[j] == 0x2AA;j++);
#endif
      clf.first = 0;
    }
    k = 4;
    if (clf.altro_length < 4) k = clf.altro_length;
    //	printf("J= %d K= %d ALTROLENGHT %d\n",j,k,altro_length);
    for (l=j; l < k; l++) {
      chdata.data[clength++] = tword[l];
      if (clength > mlength) printf("CLENGTH %d ALTRO %d\n",clength,clf.altro_length);
      clf.altro_length--;
    }
#ifdef __DEBUG_EVENT
    printf("%s ALTROLENGTH %d POS %d\n",__FUNCTION__,clf.altro_length,clf.pos);
    fflush(stdout);
#endif
  }
  return 0;
}

int JMonEvent::getChannelDataNew(void)
{ 
  int j,k,l;
  unsigned int tword[4];
  int mlength = clf.altro_length;
  int clength = 0;
  while(clf.altro_length) {
    if (clf.length < 0 ) {
      printf("%s %d *ERROR* cluster length < 0\n",__FUNCTION__,thisevent);
      return -1;
    }
    clf.index0 = *clf.sob++;
    tword[2] = clf.index0 & 0x3FF;
    tword[1] = (clf.index0 >> 10) & 0x3FF;
    tword[0] = (clf.index0 >> 20) & 0x3FF;
//      printf("%s I1 %X  I0 %X I2 %X POS %d 0 = %X 1 = %X 2 = %X 3 = %X\n",__FUNCTION__,clf.index1,clf.index0,*(clf.sob+1),clf.pos,tword[0],tword[1],tword[2],tword[3]);
    clf.length--;
    j = 0;
    if (clf.first) {
/*
#ifdef __DEBUG_EVENT
      for(j=0;(tword[j] == 0x2AA);j++) printf("%s: %X",__FUNCTION__,tword[j]);
      printf("\n%s J= %d ALTROLENGTH %d\n",__FUNCTION__,j,clf.altro_length);
      fflush(stdout);
#else 
      for(j=0;tword[j] == 0x2AA;j++) {
          clf.altro_length--;
          printf("%s: EVNT %d  %d 2AA CH %d\n",__FUNCTION__,thisevent,j,clf.altro_address);
      }
#endif
*/
      clf.first = 0;
    }
    k = 3;
    if (clf.altro_length < 3) k = clf.altro_length;
    //  printf("J= %d K= %d ALTROLENGHT %d\n",j,k,altro_length);
    for (l=j; l < k; l++) {
      chdata.data[clength++] = tword[l];
      if (clength > mlength) printf("CLENGTH %d ALTRO %d\n",clength,clf.altro_length);
      clf.altro_length--;
    }
#ifdef __DEBUG_EVENT
    printf("%s ALTROLENGTH %d POS %d\n",__FUNCTION__,clf.altro_length,clf.pos);
    fflush(stdout);
#endif
  }
  return 0;
}


void *JMonEvent::getNextChannel()
{
      clf.first = 1;
#ifdef __DEBUG_EVENT
      printf("%s: START LENGTH %d ALENGTH %d pos %d\n",__FUNCTION__,clf.altro_length,clf.length,clf.pos);
#endif
      switch(clf.pos) {
      case 0:
	clf.index0 = *clf.eob--;
	clf.index1 = *clf.eob--;
#ifdef __DEBUG_EVENT
	printf("%s INDEX %08X %08X\n",__FUNCTION__,clf.index0, clf.index1);
#endif
	clf.altro_address = ((clf.index0 & 0xF) << 8)| ((clf.index1 >> 24) & 0xFF);
	clf.altro_length = (clf.index0 >> 8) & 0x3FF;
	clf.pos = 1;
	clf.length -= 2;
	break;
      case 1:
	clf.index0 = *clf.eob--;
	clf.altro_address = (clf.index0 >> 16) & 0xFFF;
	clf.altro_length = clf.index1 & 0x3FF;
	clf.pos = 2;
	clf.length--;
	break;
      case 2:
	clf.index1 = *clf.eob--;
	clf.altro_address = (clf.index1 >> 8) & 0xFFF;
	clf.altro_length = ((clf.index0 & 0x3) << 8) | ((clf.index1 >> 24) & 0xFF);
	clf.pos = 3;
	clf.length--;
	break;
      case 3:
	clf.index0 = *clf.eob--;
	clf.altro_address = clf.index0 & 0xFFF;
	clf.altro_length = (clf.index0 >> 16) & 0x3FF;	
	clf.pos = 0;
	clf.length--;
	break;
      }
      chdata.rcuid = clf.rcud->id;
      chdata.channel = clf.altro_address;
      chdata.length = clf.altro_length;
      //      printf("%s CHANNEL %d LENGTH %d\n",__FUNCTION__,chdata.channel,chdata.length);
#ifdef __DEBUG_EVENT
      printf("%s ALTROADDR %d ALTROLEN %d pos %d\n",__FUNCTION__,clf.altro_address, clf.altro_length,clf.pos);
      printf("%s RCUID %d ALTROCH %d ALTROLEN %d\n",__FUNCTION__,chdata.rcuid, chdata.channel,chdata.length);
      fflush(stdout);
#endif
  return NULL;
}

void *JMonEvent::getNextChannelNew()
{
      clf.first = 1;
#ifdef __DEBUG_EVENT
      printf("%s: START LENGTH %d ALENGTH %d pos %d\n",__FUNCTION__,clf.altro_length,clf.length,clf.pos);
#endif
      clf.index0 = *clf.sob++;
#ifdef __DEBUG_EVENT
      printf("%s INDEX %08X %08X\n",__FUNCTION__,clf.index0, clf.index1);
#endif
      clf.altro_address = clf.index0 & 0xFFF;
      clf.altro_length = (clf.index0 >> 16) & 0x3FF;
      clf.altro_trailer = 0x2AAA;
      if (clf.index0 & 0x20000000) {
          clf.altro_trailer = 0x2AEE; // error bit is set
          printf("%s: ERRORBIT EVNT %d INDEX0 %08X DATA %08X %08X\n",__FUNCTION__,thisevent,clf.index0,*clf.sob,*(clf.sob-2));
      }
      if ((clf.index0 & 0xC0000000) != 0x40000000) {
          printf("%s: *ERROR* EXPECTED EVNT %d ALTRO HEADER FOUND %08X\n",__FUNCTION__,thisevent,clf.index0);
      }
      clf.pos = 1;
      clf.length--;
      chdata.rcuid = clf.rcud->id;
      chdata.trailer = clf.altro_trailer;
      chdata.channel = clf.altro_address;
      chdata.length = clf.altro_length;
//      printf("%s EVENT %d CHANNEL %d LENGTH %d\n",__FUNCTION__,thisevent,chdata.channel,chdata.length);
#ifdef __DEBUG_EVENT
      printf("%s ALTROADDR %d ALTROLEN %d pos %d\n",__FUNCTION__,clf.altro_address, clf.altro_length,clf.pos);
      printf("%s RCUID %d ALTROCH %d ALTROLEN %d\n",__FUNCTION__,chdata.rcuid, chdata.channel,chdata.length);
      fflush(stdout);
#endif
  return NULL;
}


void *JMonEvent::getNextChannelReversed()
{
      clf.first = 1;
#ifdef __DEBUG_EVENT
      printf("%s: START LENGTH %d ALENGTH %d pos %d\n",__FUNCTION__,clf.altro_length,clf.length,clf.pos);
#endif
      switch(clf.pos) {
      case 0:
	clf.index0 = *clf.sob++;
	clf.index1 = *clf.sob++;
#ifdef __DEBUG_EVENT
	printf("%s INDEX %08X %08X\n",__FUNCTION__,clf.index0, clf.index1);
#endif
	clf.altro_address = clf.index0 & 0xFFF;
	clf.altro_length = (clf.index0 >> 16) & 0x3FF;
	clf.altro_trailer = ((clf.index0 >> 26) & 0x3F) | ((clf.index1 & 0xFF) << 6); // 0x2AAA or 0x2AEE
	clf.pos = 1;
	clf.length -= 2;
	break;
      case 1:
	clf.index0 = *clf.sob++;
	clf.altro_address = (clf.index1 >> 8) & 0xFFF;
	clf.altro_length = ((clf.index1 >> 24) & 0xFF) | ((clf.index0 & 0x3) << 8);
	clf.altro_trailer = (clf.index0 >> 2) & 0x3FFF; // 0x2AAA or 0x2AEE
	clf.pos = 2;
	clf.length--;
	break;
      case 2:
	clf.index1 = *clf.sob++;
	clf.altro_address = (clf.index0 >> 16) & 0xFFF;
	clf.altro_length = clf.index1 & 0x3FF;
	clf.altro_trailer = (clf.index1 >> 10) & 0x3FFF; // 0x2AAA or 0x2AEE
	clf.pos = 3;
	clf.length--;
	break;
      case 3:
	clf.index0 = *clf.sob++;
	clf.altro_address = ((clf.index1 >> 24) & 0xFF) | ((clf.index0 & 0xF) << 8); 
	clf.altro_length = (clf.index0 >> 8) & 0x3FF;	
	clf.altro_trailer = (clf.index0 >> 18) & 0x3FFF; // 0x2AAA or 0x2AEE
	clf.pos = 0;
	clf.length--;
      break;
      }
      chdata.rcuid = clf.rcud->id;
      chdata.trailer = clf.altro_trailer;
      chdata.channel = clf.altro_address;
      chdata.length = clf.altro_length;
      //      printf("%s CHANNEL %d LENGTH %d\n",__FUNCTION__,chdata.channel,chdata.length);
#ifdef __DEBUG_EVENT
      printf("%s ALTROADDR %d ALTROLEN %d pos %d\n",__FUNCTION__,clf.altro_address, clf.altro_length,clf.pos);
      printf("%s RCUID %d ALTROCH %d ALTROLEN %d\n",__FUNCTION__,chdata.rcuid, chdata.channel,chdata.length);
      fflush(stdout);
#endif
  return NULL;
}


CHANNEL *JMonEvent::getChannel(int rcuid)
{
  if ((rcuid<0) || (rcuid>MAX_RCU_ID)) return NULL;
  if (hrcu[rcuid].id < 0) return NULL;
  clf.rcud = &hrcu[rcuid];
  if (clf.chfirst) {
    clf.length = clf.rcud->length;
    clf.eob = clf.rcud->data + clf.length - 1;
    if (rcut.fwv == 0) {
      clf.length--; /* DTSW word expected */
#ifdef __DEBUG_EVENT
      printf("DTSW = %X %d\n",*clf.eob,*clf.eob);
      fflush(stdout);
#endif
      clf.eob--;
    }
    clf.mod5 = clf.length % 5;
    clf.index = (clf.length / 5) * 4;
    clf.nbclusters = 0;
#ifdef __DEBUG_EVENT
      printf("MOD5 = %d %d LENGTH %d\n",clf.mod5,clf.index,clf.length);
      fflush(stdout);
#endif
    getFirstChannel();
  }
  else if (clf.length == 0) {
    return NULL;
  }
  else {
    getNextChannel();
  }
  if (getChannelData() < 0 ) return NULL;
  return &chdata;
}

/*
Events are read in reversed order
TRAILER length time data ....
*/
CHANNEL *JMonEvent::getChannelNew(int rcuid)
{
  if ((rcuid<0) || (rcuid>MAX_RCU_ID)) return NULL;
  if (hrcu[rcuid].id < 0) return NULL;
  clf.rcud = &hrcu[rcuid];
  if (clf.chfirst) {
    clf.length = clf.rcud->length;
    clf.eob = clf.rcud->data +  clf.length - 1;
    clf.sob = clf.rcud->data;
    clf.mod5 = 0; // reverse orde first word is altro(31:0)
    clf.index = (clf.length / 5) * 4; // NOT USED ??
    clf.nbclusters = 0;
#ifdef __DEBUG_EVENT
      printf("MOD5 = %d %d LENGTH %d\n",clf.mod5,clf.index,clf.length);
      fflush(stdout);
#endif
    getFirstChannelNew();
  }
  else if (clf.length == 0) {
    return NULL;
  }
  else {
    getNextChannelNew();
  }
  if (getChannelDataNew() < 0 ) return NULL;
  if (chdata.trailer == 0x2AEE) {
      printf("%s event %d ALTRO TRAILER : %X channel %d length %d\n",__FUNCTION__,thisevent,chdata.trailer,chdata.channel,chdata.length);
  }
  return &chdata;
}

CHANNEL *JMonEvent::getChannelReversed(int rcuid)
{
    CHANNEL *ch;
    ch = NULL; 
    if (FormatVersion == 0) ch = getChannelReversedOld(rcuid);
    else if (FormatVersion == 1) ch = getChannelNew(rcuid);
    return ch;
}

CHANNEL *JMonEvent::getChannelReversedOld(int rcuid)
{
  if ((rcuid<0) || (rcuid>MAX_RCU_ID)) return NULL;
  if (hrcu[rcuid].id < 0) return NULL;
  clf.rcud = &hrcu[rcuid];
  if (clf.chfirst) {
    clf.length = clf.rcud->length;
    clf.eob = clf.rcud->data +  clf.length - 1;
    clf.sob = clf.rcud->data;
    clf.mod5 = 0; // reverse orde first word is altro(31:0)
    clf.index = (clf.length / 5) * 4; // NOT USED ??
    clf.nbclusters = 0;
#ifdef __DEBUG_EVENT
      printf("MOD5 = %d %d LENGTH %d\n",clf.mod5,clf.index,clf.length);
      fflush(stdout);
#endif
    getFirstChannelReversed();
  }
  else if (clf.length == 0) {
    return NULL;
  }
  else {
    getNextChannelReversed();
  }
  if (getChannelDataReversed() < 0 ) return NULL;
  if (chdata.trailer == 0x2AEE) {
      printf("%s ALTRO TRAILER : %X channel %d length %d\n",__FUNCTION__,chdata.trailer,chdata.channel,chdata.length);
  }
  return &chdata;
}

/* Get all clusters that exist in chdata, returned
   as pointers into the data */
CLUSTER *JMonEvent::getClusters(void)
{
  int length = chdata.length;
  int *ptr = chdata.data;
  int nbclusters = 0;
  int cluster_length = 0;
  int cluster_time = 0;
//  if (cldata) delete cldata;
//  cldata = 0;
  while (length > 0) {
    cluster_length = ptr[0];
    cluster_time = ptr[1];
    if (cluster_length == 0) {
      printf("%s: %d *ERROR* length %d (%d) T %d clusters CH %d\n",__FUNCTION__,thisevent,cluster_length,length,cluster_time,chdata.channel);
      return NULL;
    }
    length -= cluster_length;
    ptr += cluster_length;
    nbclusters++;
  }
  if (length < 0) {
    printf("%s: %d *ERROR* buffer exceeded CHANNEL %d dlength %d clength %d time %d nbclusters %d\n",__FUNCTION__,thisevent,chdata.channel,chdata.length,cluster_length,cluster_time,nbclusters);
    fflush(stdout);
    return NULL;
  }
#ifdef __DEBUG_EVENT
  cout << thisevent << "CLUSTERS = " << nbclusters << endl; fflush(stdout);
#endif
  if (nbclusters == 0 ) {
    printf("%s: E length %d clusters CH %d\n",__FUNCTION__,chdata.length,chdata.channel);
    fflush(stdout);
    return NULL;
  }
//  cldata = new CLUSTER[nbclusters];
//  if (cldata == 0) {
//    printf("%s new CLDATA buffer exceeded\n",__FUNCTION__);
//  }
  length = chdata.length;
  ptr = chdata.data;
  int nb = 0;
  while ((length > 0) && (nbclusters)) {
    nbclusters--;
    cluster_length = *ptr++;
    cldata[nbclusters].nb = nb++;
    cldata[nbclusters].rcuid = chdata.rcuid;
    cldata[nbclusters].channel = chdata.channel;
    cldata[nbclusters].length = cluster_length - 2;
    cldata[nbclusters].time = *ptr++;
    cldata[nbclusters].data = ptr;
    length -= cluster_length;
    ptr += cldata[nbclusters].length;
    //    printf("CLUSTER %d CL_L %d CH %d time %d LENGTH %d\n",nbclusters,cluster_length, cldata[nbclusters].channel,cldata[nbclusters].time,cldata[nbclusters].length);
  }
  if (length) {
    printf("%s: *ERROR* length %d clusters %d\n",__FUNCTION__,length,nbclusters);
    fflush(stdout);
    return NULL;
  }
  return cldata;
}


CLUSTER *JMonEvent::getClustersReversed(void)
{
  int length = chdata.length;
  int *ptr = chdata.data;
  int nbclusters = 0;
  int cluster_length = 0;
  int cluster_time = 0;
//  if (cldata) delete cldata;
//  cldata = 0;
  while (length > 0) {
    cluster_length = ptr[0];
    cluster_time = ptr[1];
//    printf("%s LENGTH %d TIME %d\n", __FUNCTION__,cluster_length,cluster_time);
    if (cluster_length == 0) {
      printf("%s: %d *ERROR* length %d (%d) T %d clusters CH %d\n",__FUNCTION__,thisevent,cluster_length,length,cluster_time,chdata.channel);
      return NULL;
    }
    length -= cluster_length;
    ptr += cluster_length;
    nbclusters++;
  }
  if (length < 0) {
    printf("%s: %d *ERROR* buffer exceeded CHANNEL %d dlength %d clength %d time %d nbclusters %d\n",__FUNCTION__,thisevent,chdata.channel,chdata.length,cluster_length,cluster_time,nbclusters);
    fflush(stdout);
    return NULL;
  }
#ifdef __DEBUG_EVENT
  cout << thisevent << "CLUSTERS = " << nbclusters << endl; fflush(stdout);
#endif
  if (nbclusters == 0 ) {
    printf("%s: E length %d clusters CH %d\n",__FUNCTION__,chdata.length,chdata.channel);
    fflush(stdout);  
    return NULL;
  }
//  cldata = new CLUSTER[nbclusters];
//  if (cldata == 0) {
//    printf("%s new CLDATA buffer exceeded\n",__FUNCTION__);
//  }
  length = chdata.length;
  ptr = chdata.data;
  int nb = 0;
  while ((length > 0) && (nbclusters)) {
    nbclusters--;
    cluster_length = *ptr++;
    cldata[nbclusters].nb = nb++;
    cldata[nbclusters].rcuid = chdata.rcuid;
    cldata[nbclusters].channel = chdata.channel;
    cldata[nbclusters].length = cluster_length - 2;
    cldata[nbclusters].time = *ptr++;
    cldata[nbclusters].data = ptr;
    length -= cluster_length;
    ptr += cldata[nbclusters].length;
    //    printf("CLUSTER %d CL_L %d CH %d time %d LENGTH %d\n",nbclusters,cluster_length, cldata[nbclusters].channel,cldata[nbclusters].time,cldata[nbclusters].length);
  }
  if (length) {
    printf("%s: *ERROR* length %d clusters %d\n",__FUNCTION__,length,nbclusters);
    fflush(stdout);
    return NULL;
  }
  return cldata;
}


void JMonEvent::CloseEvent()
{
//  if (cldata) delete cldata;
//  cldata = 0;
//  chdata.data = 0;
}
