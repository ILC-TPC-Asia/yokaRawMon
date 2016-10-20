#include <signal.h>
#include <cstdlib>

#include "JMonFile.h"
#include "JMonHist.h"
#include "JMonEvent.h"
#include "JPed.h"
#include "JChMapper.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <getopt.h>
#include "TFile.h"
#include "TROOT.h"

#define MAX_RUC_ID 3

int signal_stoprun;

int pedrun        = 0;
int eventmon      = 1;
int reversed      = 0;
int unpackchannel = 1;
//int maxevents     = 99999999;
int maxevents     = 10000;
int format        = 0;

JPed *jped[MAX_RCU_ID+1] = {0,0,0,0};

int ilcGetParameters(int argc, char **argv);
int sourceFile();

string indirname("rawdata/");
string outdirname("root/");
string infilename("readout.dat");
string outfilename("readout.root");
//string mapfilename("map/channelmap_090228.txt");
string mapfilename("map/channelmap_090304.txt");

/***********************************************************
 * Signal handler
 ***********************************************************/
void signal_handler(int signum)
{
  cout << "SIGNAL" << signum << endl << flush;
  switch(signum) {
  case SIGQUIT:
  case SIGTERM:
  case SIGINT: 
    signal_stoprun = 1;
    break;
  }
}


/* Decode input parameters */
int ilcGetParameters(int argc, char **argv)
{
  /*
    -i, --infile   : input file in the input directory
    -I, --indir    : input data directory  overrides default
    -O, --outdir   : output data directory  overrides default
    -p, --pedestal : pedestal calculation
    -e, --events   : make rootfile for each event (only for file)
    -r, --reversed : reversed data format
    -m, --mapfile  : map file name
    -F, --format   : data format version
  */
  int c;
  int option_index = 0;
  const char *short_options = "aeI:i:OopP:w:";
  const struct option long_options[]=
  {
    {"events",0,0,'e'},
    {"indir",1,0,'I'},
    {"infile",1,0,'i'},
    {"outdir",1,0,'O'},
    {"pedestal",0,0,'d'},
    {"reversed",0,0,'r'},
    {"mapfile",1,0,'m'},
    {"format",1,0,'F'}
  };
  if (argc > 1) {
    while ((c = getopt_long(argc, argv, short_options,
                                        long_options, &option_index)) != EOF) {
      switch (c) {
      case 'd':
	pedrun   = 1;
	eventmon = 0;
	break;
      case 'e':
	eventmon = 1;
	break;
      case 'r':
	reversed = 1;
	break;
      case 'i':
        infilename = optarg; 	
        cout << "Input file: " << infilename << endl;
	break;
      case 'I':
        indirname  = optarg; 	
        cout << "Input dir: " << indirname << endl;
        break;
      case 'O':
        outdirname = optarg; 	
        cout << "Output dir: " << outdirname << endl;
        break;
      case 'm':
        mapfilename = optarg; 	
        cout << "Map file: " << mapfilename << endl;
	break;
      case 'F':
        format = atoi(optarg); 	
	break;
      default:
	cout << "Unknow option: " << optarg << endl
	     << "Abort" << endl;
	abort();
      }
    }
  }
  size_t sl   = infilename.rfind("/",infilename.length());
  if (sl != string::npos) infilename = infilename.substr(sl+1);

  size_t dot  = infilename.find(".dat",0);
  outfilename = infilename;
  outfilename.replace(dot, 5, ".root");
  cout << "Output file:" << outfilename << endl;

  return 0;
}
int sourceFile()
{
  SORHEADER sor;
  BOFHEADER bof;
  EOFHEADER eof;
  EORHEADER eor;
  EVTHEADER evt;
  CLUSTER    *cl;
  CHANNEL    *ch;
  RCUTRAILER *rcutrailer;
  int nevt            = 0;
  int noclusterevents = 0;
  int nbclusters      = 0;
  int nbclusterevents = 0;
  int filenb          = 0;
  int runnb           = 0;
  int all             = 1;
  int blockid;
  int rcuid;

  // Create histograms
  stringstream outstr;
  outstr << outdirname << outfilename.data() << ends;
  JMonHist *jhist = new JMonHist(outstr.str());
  JChMapper::SetMapFile(mapfilename.data());

  /* get file and event handling */
  JMonFile  *jfile  = new JMonFile();
  JMonEvent *jevent = new JMonEvent();

  /* set data format */
  jevent->setFormatVersion(format);

  // Create pedestal objects
  for (int j = 0; j <= MAX_RCU_ID; j++) jped[j] = new JPed(j);

  JHeader *jhead;
  while (filenb >=0) {
    stringstream ostr;
    ostr << indirname << infilename.data() << ends;
    if (jfile->OpenFile(ostr.str().data()) == JMonFileOK) {
    /* Everything should start with a header:
       start of run, end of run, event, beginning of file, end of file */
      int status;
      while(!signal_stoprun && ((status = jfile->ReadRunHeader()) == JMonFileOK)) {
	jhead = jfile->GetRunHeader();
	blockid = jevent->getBlockId(jhead);
	if (blockid != 0x22222222) {
	  jfile->DumpRunHeader(jhead);
	  cout << "BLOCKID " << blockid << endl;
	}
	if (blockid == 0x11111111)  { // SOR
	  jevent->getHeader(&sor,jhead);
	  runnb = sor.runnb;
	  filenb = -1;
	  cout << "SOR runnb: "  << runnb << endl;
          if (eventmon) { // read in pedestals
            for (int j = 0; j <= MAX_RCU_ID; j++) {
              stringstream pedfilename;
              pedfilename << outdirname << "pedestals-" << runnb << "-" << j << ".new" << ends;
              jped[j]->readPedestalFile(pedfilename.str().data(),j);
	    }
          }
	  jhist->SetPedestals(MAX_RCU_ID, jped);
	}
	else if (blockid == 0x33333333)  { // EOR
	  jevent->getHeader(&eor,jhead);
	  filenb = -1;
	  cout << "EOR runnb: "  << runnb << endl;
	  cout << "Events processed " << nevt << endl;
	  cout << "No cluster found in: " << noclusterevents << endl;
	}
        else if (blockid == 0x11112222)  { // POR
          jevent->getHeader(&eor,jhead);
	  cout << "POR events: "  << eor.events << endl;
        }
        else if (blockid == 0x11113333)  { // COR
          jevent->getHeader(&eor,jhead);
          cout << "COR events: "  << eor.events << endl;
        }
	else if (blockid == 0x44444444)  { // BOF
	  jevent->getHeader(&bof,jhead);
	  runnb = bof.runnb;
	  filenb = -1;
	  cout << "BOF runnb: " << runnb << endl;
	}
	else if (blockid == 0x55555555)  { // EOF
	  jevent->getHeader(&eof,jhead);
	  if (all) {
	    filenb = eof.filenb + 1;
	    stringstream outs;
	    outs << "readout-" << runnb << "-" << filenb << ends;
	    infilename = outs.str();
	  }
	  cout << "Events processed: " << nevt << endl;
	  cout << "EOF next filenb: " << filenb << endl;
	}
	else if (blockid == 0x22222222)  { // EVENT
	  nevt++;
	  jevent->getHeader(&evt,jhead);
	  jhead->total_length -= jhead->header_length;
	  jhead->total_length--; /* exclusive header length */
	  /* loop over RCUs */
	  nbclusters = 0;
	  jhist->ClearAll();
	  while (jhead->total_length > 0) {
	    jfile->ReadRcuEvent();
            JEvent *jrcu = jfile->GetRcuEvent();
	    rcuid = jevent->getRcuId(jrcu);
	    if ((rcuid >= 0) && (rcuid <= MAX_RCU_ID)) {
	      /* get the RCU header */
	      jevent->getRcuHeader(rcuid,jrcu);
	      rcutrailer = jevent->getRcuTrailer(rcuid,2);

	      int cont = 1;
	      /* loop over ALTRO channels in the data */
              if (reversed) {
                ch = jevent->getChannelReversed(rcuid);
                if (ch->trailer == 0x2AEE) {
                  unpackchannel = 0;
                } 
                else {
                  unpackchannel = 1;
                }
              } 
              else {
                ch = jevent->getChannel(rcuid);
                unpackchannel = 1;
              }
#if 1
              while (cont && ch) {
#else
              if (ch<=0) continue;
              while (cont && ch>0) {
#endif
                if (ch->length > 0) {
                  /* get pointers to all clusters for the channel */
                  if (reversed) {
                    cl = jevent->getClustersReversed();
                  }
                  else {
                    cl = jevent->getClusters();
		  }
		  if (cl && unpackchannel == 1) {
                    /* Fill histograms for the clusters */
                    do {
                      nbclusters++;
                      jhist->FillEvent(cl);
                      if (pedrun) jped[rcuid]->fillPedestalCalc(cl);
                    } while(cl++->nb > 0);
		  }
		  else {
		    cont = 1;
		  }
		}
		if (reversed) {
                  ch = jevent->getChannelReversed(rcuid);
#if 1
		  if (ch) {
#else
		  if (ch>0) {
#endif
                    if (ch->trailer == 0x2AEE) {
                      unpackchannel = 0;
                    } 
                    else {
                      unpackchannel = 1;
                    }
		  }
		}
		else {
                  ch = jevent->getChannel(rcuid);
                  unpackchannel = 1;
		}
		if (((rcutrailer->err_reg2) & 0x3FFFFF)) {
                  cout << "Clusters found " << nbclusters << endl;
		}
	      }
	    }
	    /* Update the remaining length  */
	    jhead->total_length -= jrcu->total_length;
	    jhead->total_length--; /* exclusive header length */
	  }
	  if (eventmon) {
	      if ((nbclusterevents < maxevents) && nbclusters) {
#if 0
		  stringstream evtfilename;
		  evtfilename << outdirname << "evt-" << nevt << "-" << outfilename.data() << ends;
		  TFile evtfile(evtfilename.str().data(),"RECREATE");
		  evtfile.cd();  			      
		  jhist->WriteAll();
		  evtfile.Close();
#else
		  jhist->WriteAll(nevt);
#endif
	      }
	  }
	  if ((nevt % 100) == 0)  {
            cout << "Events processed " << nevt << endl;
	  }
	  if (nbclusters == 0) noclusterevents++;
	  else nbclusterevents++;
	}
      }
      jfile->CloseFile();
      if (filenb == 0) filenb = -1;
    }
    else filenb = -1;
  }
  if (pedrun) {
    for (int j = 0; j <= MAX_RCU_ID; j++)  {
      jped[j]->makePedestalCalc();
      stringstream pedfilename;
      pedfilename << outdirname << "pedestals-" << runnb << "-" << j << ".new" << ends;
      jped[j]->writePedestalFile(pedfilename.str().data());
    }
    for (int j = 0; j <= MAX_RCU_ID; j++)  delete jped[j];
  }
#if 0
#else
  jhist->CloseFile();
#endif

  delete jevent;
  delete jfile;
  delete jhist;

  cout << "ENDFILE" << endl << flush;

  return(0);
}

int main(int argc, char **argv)
{
  signal_stoprun = 0;
  ilcGetParameters(argc,argv);
  TROOT simple("simple","Example of creation of a tree");

  // Install signal handler
  signal(SIGINT,signal_handler);
  signal(SIGQUIT,signal_handler);
  signal(SIGTERM,signal_handler);

  cout << "MAIN START" << endl << flush;
  sourceFile();
  cout << "MAIN EXIT" << endl << flush;

  return 0;
}
