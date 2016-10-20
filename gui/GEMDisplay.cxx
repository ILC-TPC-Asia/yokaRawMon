//
// $Id: GEMDisplay.cxx,v 1.2 2011/12/25 01:18:27 fujiik Exp $
//
#include "GEMDisplay.h"
#include "JREvent.h"

#include "TSystem.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TBrowser.h"
#include "TTree.h"

#include <iostream>
#include <sstream>

using namespace std;

const int kNbuttons = 5;

GEMDisplay *GEMDisplay::fgInstancePtr = 0;

ClassImp(GEMDisplay)

GEMDisplay *GEMDisplay::GetInstance()
{
  if (!fgInstancePtr) fgInstancePtr = new GEMDisplay;
  return fgInstancePtr;
}

GEMDisplay::GEMDisplay()
          : TGMainFrame(gClient->GetRoot(),1040, 820, kHorizontalFrame), 
	    fCanvasDirectory(0),
	    fCanvas         (0),
            fCont           (0),
            fBottomButtons  (0),
	    fEventEntry     (0),
	    fStatusBar      (0),
            fStatus         (0),
	    fMenuBar        (0),
            fToolMenu       (0),
            fMonitorMenu    (0),
            fInputDir       ("./root/"),
            fCurFile        (0),
            fCurTree        (0),
            fCurJREvent     (0),
            fCurEvent       (-1),
            fCurRun         (-1),
            fCurPart        (0),
            fCurType        (0),
            fScripts        (1),
	    fIsInitialized  (false),
	    fReset          (true)
{
  fButtons = new TGTextButton* [kNbuttons];
  for (int i=0; i<kNbuttons; i++) fButtons[i] = 0;
}

GEMDisplay::~GEMDisplay()
{
  delete fCont;
  delete [] fButtons;
  TIter next(&fScripts);
  TObject *obj;
  while ((obj = next())) delete obj;
}

void GEMDisplay::SetMonitorType(int type)
{
  fCurType = type;
}

void GEMDisplay::SetRun(int run)
{
  fCurRun = run;
}

void GEMDisplay::SetInputDir(const char *indir)
{
  fInputDir = indir;
}

void GEMDisplay::Add(const char *script)
{
  TObjString *objstr = new TObjString(script);
  if (objstr->String().Length()) {
    fScripts.Add(objstr);
    stringstream outs;
#if 0
    outs << script << ".C" << ends;
#else
    outs << script << ".C+" << ends;
#endif
    gROOT->LoadMacro(outs.str().data());
    cout << ".... Loaded " << outs.str().data() << endl;
    objstr->SetBit(kScriptLoaded);
  } else {
    delete objstr;
  }
}


void GEMDisplay::Initialize()
{
  SetWindowName("yokaRawMon - Yet anOther Kantan Altro RAW data MONitor");

  TGLayoutHints *lhC          = new TGLayoutHints(kLHintsCenterX|kLHintsExpandX|kLHintsExpandY,2,2,2,2);
  TGLayoutHints *lhTL         = new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX,1,1,1,1);
  TGLayoutHints *lhTR         = new TGLayoutHints(kLHintsTop|kLHintsRight|kLHintsExpandX,1,1,1,1);
  TGLayoutHints *lhTLnoexpand = new TGLayoutHints(kLHintsTop|kLHintsLeft,1,1,1,1);
  TGLayoutHints *lhTRnoexpand = new TGLayoutHints(kLHintsTop|kLHintsRight,1,1,1,1);
  TGLayoutHints *lhItem       = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 1, 0, 0);

  // ------------------
  //  World frame
  // ------------------
  fCont       = new TGCompositeFrame(this,1024,800,kVerticalFrame);

  // ------------------
  //  Status Bar
  // ------------------
  fStatusBar  = new TGCompositeFrame(fCont,100,5,kHorizontalFrame);

  // ------------------
  //  Status Bar Items
  // ------------------
  fStatus     = new TGLabel(fStatusBar);
  stringstream outs;
  outs << "Initializing                                  "
       << "                                              "
       << ends;
  fStatus->SetText(outs.str().data());
  fStatus->SetTextJustify(kTextLeft);

  fStatusBar->AddFrame(fStatus,lhTLnoexpand);

  fMenuBar    = new TGMenuBar(fStatusBar, 1, 1, kHorizontalFrame);

  fToolMenu   = new TGPopupMenu(fClient->GetRoot());
  fToolMenu->Associate(this);
  fToolMenu->AddEntry("New Canvas", 9001); 		// Canvas
  fToolMenu->AddEntry("Open Browser", 9002);		// Browser
  fMenuBar->AddPopup("Tools", fToolMenu, lhItem);    

  fMonitorMenu = new TGPopupMenu(fClient->GetRoot());
  fMonitorMenu->Associate(this);
  for (int itemno = 0; itemno < fScripts.GetEntries(); itemno++) {
    TObjString *item = static_cast<TObjString *>(fScripts.At(itemno));
    fMonitorMenu->AddEntry(item->String().Data(), 100000+itemno);
  }
  fMenuBar->AddPopup("Monitors", fMonitorMenu, lhItem);    

  fStatusBar->AddFrame(fMenuBar,lhTRnoexpand);
  fCont->AddFrame(fStatusBar,lhTL);

// ------------------
//  Display Area
// ------------------
  fCanvas = new TRootEmbeddedCanvas("DisplayArea",fCont,1024,768);
  fCont->AddFrame(fCanvas,lhC);

// ------------------
//  Bottom Buttons
// ------------------
  fBottomButtons = new TGCompositeFrame(fCont,10,10,kHorizontalFrame);
  fButtons[0] = new TGTextButton(fBottomButtons, "Prev",     8001);
  fButtons[1] = new TGTextButton(fBottomButtons, "Next",     8000);
  fButtons[2] = new TGTextButton(fBottomButtons, "Goto",     8002);
  fButtons[3] = new TGTextButton(fBottomButtons, "Print",    8003);
  fButtons[4] = new TGTextButton(fBottomButtons, "Quit",     8004);

  for (int i=0; i<kNbuttons; i++) {
    if (i == 4) { // quit
      fBottomButtons->AddFrame(fButtons[i],lhTR);
    } else if (i == 2) { // goto event
      fBottomButtons->AddFrame(fButtons[i],lhTLnoexpand);
      fButtons[i]->SetLeftMargin(15);
      fButtons[i]->SetRightMargin(15);
      fButtons[i]->Resize(80);
      fEventEntry = new TGTextEntry(fBottomButtons, new TGTextBuffer(20),8002);
      //fEventEntry->Resize(50, fEventEntry->GetDefaultHeight());
      fBottomButtons->AddFrame(fEventEntry, lhTLnoexpand);
      fEventEntry->Associate(this);
    } else {
      fBottomButtons->AddFrame(fButtons[i],lhTL);
    }
    fButtons[i]->Associate(this);
  }
  fBottomButtons->Layout();
  fCont->AddFrame(fBottomButtons,lhTL);

  AddFrame(fCont,lhC);
  fCont->Layout();
  fCanvasDirectory = gDirectory;

  MapSubwindows();
  Resize(GetDefaultSize());
  Layout();
  MapWindow();
#if 1 // This is essentail to display frame.
  gSystem->ProcessEvents();
#endif
    
  fIsInitialized = true;
}

bool GEMDisplay::Display()
{
  bool ret = false;
  if (!fIsInitialized) {
    Initialize();
  } else {
    fCanvas->GetCanvas()->cd();
    gPad->Clear();
    fCanvas->Clear();
  } 
  TDirectory *olddir = gDirectory;
  fCanvasDirectory->cd();

  fCanvas->GetCanvas()->cd();
  if (GetScript().String().Length()) {
    if (fCurEvent >= 0 && fCurRun > 0 && fCurPart >= 0) {
      stringstream cmd;
      cmd << GetScript().String().Data()
          << "(" << fCurEvent + 1
          << "," << fCurRun
          << "," << fCurPart
          << "," << (fReset ? true : false)
          <<  ");" << ends;
      cerr << cmd.str().data() << endl;
      gROOT->ProcessLine(cmd.str().data());
      ret = true;
    }
  }
  if (fCanvasDirectory != olddir) olddir->cd();
  fReset = kFALSE;
  return ret;
}

TCanvas *GEMDisplay::GetCanvas()
{
  if (!fIsInitialized) {
    Initialize();
  }
  return fCanvas->GetCanvas();
}

void GEMDisplay::PrintDisplay()
{
  stringstream outs;
  outs << GetScript().String().Data()
       << ".evt"  << fCurEvent + 1
       << ".run" << fCurRun
       << "_"   << fCurPart 
#if 0
       << ".pdf"
       << ends;
  GetCanvas()->Print(outs.str().data());
#else
       << ".C"
       << ends;
  GetCanvas()->SaveSource(outs.str().data());
#endif
}

void GEMDisplay::ProcessButton(Long_t parm1)
{
  switch(parm1) {
    case 8004: { // Quit
      CloseWindow();   // this also terminates theApp
      gROOT->ProcessLine(".q;");
      break;
    }
    case 8003: { // Print
      PrintDisplay();
      break;
    }
    case 8002: { // Goto
      // cerr << "Goto button is pressed" << endl;
      stringstream ins(fEventEntry->GetBuffer()->GetString());
      int nevt;
      ins >> nevt;
      nevt -= 1;
      if (nevt >= 0) {
        GoToEvent(nevt,fCurRun,fCurPart);
      } else {
        stringstream outs;
        outs << "Failed to get event #" << nevt + 1 << ends;
	UpdateStatus(outs.str().data());
      }
      break;
    }
    case 8001: { // Prev
      // cerr << "Prev button is pressed" << endl;
      int nevt = fCurEvent - 1;
      if (nevt >= 0) {
        GoToEvent(nevt,fCurRun,fCurPart);
      } else {
        stringstream outs;
        outs << "Failed to get event #" << nevt + 1 << ends;
	UpdateStatus(outs.str().data());
      }
      break;
    }
    case 8000: { // Next
      // cerr << "Next button is pressed" << endl;
      int nevt = fCurEvent + 1;
      if (nevt >= 0) {
        GoToEvent(nevt,fCurRun,fCurPart);
      } else {
        stringstream outs;
        outs << "Failed to get event #" << nevt + 1 << ends;
	UpdateStatus(outs.str().data());
      }
      break;
    }
    default:
      break;
  }
}

void GEMDisplay::ProcessMenu(Long_t parm1)
{
  if (parm1 < 100000) return;
  fCurType = parm1 % 100000;
  fReset   = kTRUE;
  GoToEvent(fCurEvent, fCurRun, fCurPart);
}

Bool_t GEMDisplay::ProcessMessage(Long_t msg, Long_t parm1, Long_t)
{
  switch (GET_MSG(msg)) {
    case kC_COMMAND:
      switch (GET_SUBMSG(msg)) {
        case kCM_BUTTON:
             if (parm1 >= 8000) ProcessButton(parm1);
             break;
        case kCM_MENUSELECT:
             break;
        case kCM_MENU:
             switch (parm1) {
               case 9001: {
                    new TCanvas();
                    break;
               }
               case 9002: {
                    new TBrowser();
                    break;
               }
               default:
                    if (parm1 >= 100000) ProcessMenu(parm1);
                    break;
             }
	     break;
         default:
             break;
       }
       break;
    default:
       break;
  }
  return kTRUE;
}

void GEMDisplay::UpdateStatus(const char *message)
{
  stringstream outs;
  if (message) {
    outs << message << ends;
    //cerr << outs.str().data() << endl;
    fStatus->SetTextColor(kRed);
  } else {
    outs << "Run " << fCurRun << "  Event " << fCurEvent+1 << ends;
    fStatus->SetTextColor(kBlack);
  }
  fStatus->SetText(TGString(outs.str().data()));
}

bool GEMDisplay::GoToEvent(int evt, int run, int part)
{
  stringstream infile;
  infile << fInputDir << "readout-" << run << "_" << part <<".root";
  if (gSystem->AccessPathName(infile.str().data(),kFileExists)) {
    stringstream outs;
    outs << infile.str().data() << " does not exist. skip!" << ends;
    UpdateStatus(outs.str().data());
    return false;
  } else {
    if (!fCurFile || run != fCurRun || part != fCurPart) {
      cerr << "Opening " << infile.str().data() << endl;
      fCurFile  = new TFile(infile.str().data());
      delete fCurTree;
      fCurTree = static_cast<TTree *>(fCurFile->Get("Event"));
      fCurTree->SetBranchAddress("Event", &fCurJREvent);
    }
  }
  fCurTree->GetEntry(evt);
  fCurEvent = evt;
  fCurRun   = run;
  fCurPart  = part;
  UpdateStatus();
  return Display();
}

bool GEMDisplay::NextEvent()
{
  fCurEvent += 1;
  return  GoToEvent(fCurEvent, fCurRun, fCurPart);
}

bool GEMDisplay::PreviousEvent()
{
  fCurEvent -= 1;
  return  GoToEvent(fCurEvent, fCurRun, fCurPart);
}

TObjString & GEMDisplay::GetScript()
{
  if (!fScripts.At(fCurType)) {
    fScripts.AddAt(new TObjString, fCurType);
    cerr << " fCurType = " << fCurType << " empty!" << endl;
  }
  return *static_cast<TObjString *>(fScripts.At(fCurType));
}
