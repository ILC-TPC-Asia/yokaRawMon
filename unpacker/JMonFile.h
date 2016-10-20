#ifndef __JMon
#define __JMon

#include <fstream>
#include <iostream>
using namespace std;


#define JMonFileOK 0
#define JMonFileOPEN 1
#define JMonFileLENGTH 2
#define JMonFileEOF 3

typedef unsigned int u_int;

typedef struct {
  u_int *data;
  int total_length;
  int header_length;  
} JHeader;

typedef struct {
  u_int *data;
  int total_length;
} JEvent;


class JMonFile {
 private:
  ifstream JinFile;
  JHeader JrunHeader;
  JEvent JrawEvent;

 public:
  JMonFile();
  ~JMonFile();
  int OpenFile(const char *);
  void CloseFile();
  JHeader *GetRunHeader();
  int ReadRunHeader();
  void DumpRunHeader(JHeader *jh);
  JEvent *GetRcuEvent();
  int ReadRcuEvent();

};

#endif
