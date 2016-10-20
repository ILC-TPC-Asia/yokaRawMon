#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>
#include "JMonFile.h"
//#define __DEBUG_FILE

// Constructor dummy one
JMonFile::JMonFile()
{
  JrawEvent.data = NULL;
  JrawEvent.total_length = 0;
  JrunHeader.data = NULL;
  JrunHeader.total_length = 0;
  JrunHeader.header_length = 0;
}

JMonFile::~JMonFile()
{
  if (JinFile) {
    JinFile.close();
  }
  delete JrawEvent.data;
  delete JrunHeader.data;
  cout << "DELETE JMONFILE" << endl;
}



int JMonFile::OpenFile(const char *filename)
{
  JinFile.open(filename,ios::in);
  if (!JinFile) {
    cerr << "Cannot open: " << filename << " for input" << endl;
    return(JMonFileOPEN);
  }
  return JMonFileOK;
}

void JMonFile::CloseFile()
{
  cerr << "Close" << endl;
  JinFile.close();
}

JHeader *JMonFile::GetRunHeader()
{
#ifdef __DEBUG_FILE
  cout << "RUNEHADER" << endl;
#endif
  return(&JrunHeader);
}

int JMonFile::ReadRunHeader()
{
  int status = JMonFileOK;
  JrunHeader.total_length = 0;
  JrunHeader.header_length = 0;
  if (!JinFile) {
    cerr << "FILE NOT OK" << endl;
    return  JMonFileEOF;
  }
    JinFile.read((char *)&JrunHeader.total_length,sizeof(int));
#ifdef __BIG_ENDIAN__
    unsigned int tmp = 0;
    tmp |= (unsigned int)JrunHeader.total_length << 24;
    tmp |= (unsigned int)((JrunHeader.total_length >> 8) << 24) >> 8;
    tmp |= (unsigned int)((JrunHeader.total_length << 8) >> 24) << 8;
    tmp |= (unsigned int)JrunHeader.total_length >> 24;
    JrunHeader.total_length = tmp;
#endif
    if (JinFile.eof()) {
      cerr << "EOF 0" << endl;
      return  JMonFileEOF;
    }
    JinFile.read((char *)&JrunHeader.header_length,sizeof(int));
#ifdef __BIG_ENDIAN__
    tmp = 0;
    tmp |= (unsigned int)JrunHeader.header_length << 24;
    tmp |= (unsigned int)((JrunHeader.header_length >> 8) << 24) >> 8;
    tmp |= (unsigned int)((JrunHeader.header_length << 8) >> 24) << 8;
    tmp |= (unsigned int)JrunHeader.header_length >> 24;
    JrunHeader.header_length = tmp;
#endif
    if (JinFile.eof()) {
      cerr << "EOF 1" << endl;
      return  JMonFileEOF;
    }
#ifdef __DEBUG_FILE
    printf("%s: TOT %d(%X) LEN %d(%X)\n",__FUNCTION__,JrunHeader.total_length,JrunHeader.total_length,
	   JrunHeader.header_length,JrunHeader.header_length);
    fflush(stdout);
#endif
    if (JrunHeader.header_length) {
      if (JrawEvent.data) {
	delete JrawEvent.data;
	JrawEvent.data = NULL;
      }
      if (JrunHeader.data) delete JrunHeader.data;
      JrunHeader.data = new u_int [JrunHeader.header_length + 10];
      if (JrunHeader.data == NULL ) {
	cerr << "EXHAUST" << endl;
	return JMonFileEOF;
      }
      JinFile.read((char *) JrunHeader.data,JrunHeader.header_length << 2);
#ifdef __BIG_ENDIAN__
      for (int i=0; i<JrunHeader.header_length; i++) {
        tmp = 0;
        tmp |= JrunHeader.data[i] << 24;
        tmp |= ((JrunHeader.data[i] >> 8) << 24) >> 8;
        tmp |= ((JrunHeader.data[i] << 8) >> 24) << 8;
        tmp |= JrunHeader.data[i] >> 24;
        JrunHeader.data[i] = tmp;
      }
#endif
      if (JinFile.eof()) {
	fprintf(stderr,"%s: EOF 1 LENGTH %d (%X) %d (%X)\n",__FUNCTION__,JrunHeader.header_length,
		JrunHeader.header_length,JrunHeader.total_length,JrunHeader.total_length);
	return  JMonFileEOF;
      }
    }
    else { 
      status = JMonFileLENGTH;
    }
  return(status);
}

void JMonFile::DumpRunHeader(JHeader *jh)
{
  int j;
  printf("TOTLEN %d HEADLEN %d\n",jh->total_length, jh->header_length);
  for (j = 0; j < jh->header_length; j++) {
    printf("%04d: %08X\n",j,jh->data[j]);
  }
}

JEvent *JMonFile::GetRcuEvent()
{
  return(&JrawEvent);
}

int JMonFile::ReadRcuEvent()
{
  int status = JMonFileOK;
  if (JinFile) {
    JinFile.read((char *) &JrawEvent.total_length,sizeof(JrawEvent.total_length));
#ifdef __DEBUG_FILE
    printf("RDEVT LENGTH %d (%X)\n",JrawEvent.total_length,JrawEvent.total_length);
#endif
#ifdef __BIG_ENDIAN__
    unsigned int tmp = 0;
    tmp |= (unsigned int)JrawEvent.total_length << 24;
    tmp |= (unsigned int)((JrawEvent.total_length >> 8) << 24) >> 8;
    tmp |= (unsigned int)((JrawEvent.total_length << 8) >> 24) << 8;
    tmp |= (unsigned int)JrawEvent.total_length >> 24;
    JrawEvent.total_length = tmp;
#endif
    if (JrawEvent.data) delete JrawEvent.data;
    if (JrawEvent.total_length) {
      JrawEvent.data = new u_int [JrawEvent.total_length + 10];
      if (JrawEvent.data == NULL) {
	cerr << "EXHAUSTMEM" << endl;
	return JMonFileEOF;
      }
      JinFile.read((char *) JrawEvent.data,JrawEvent.total_length << 2);
#ifdef __BIG_ENDIAN__
      for (int i=0; i<JrawEvent.total_length; i++) {
        tmp = 0;
        tmp |= JrawEvent.data[i] << 24;
        tmp |= ((JrawEvent.data[i] >> 8) << 24) >> 8;
        tmp |= ((JrawEvent.data[i] << 8) >> 24) << 8;
        tmp |= JrawEvent.data[i] >> 24;
        JrawEvent.data[i] = tmp;
      }
#endif
    }
    else { 
      status = JMonFileLENGTH;
    }
  }
  else {
    status = JMonFileOPEN;
  }
  return(status);
}
