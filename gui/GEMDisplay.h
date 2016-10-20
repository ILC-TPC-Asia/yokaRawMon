//
// $Id: GEMDisplay.h,v 1.1.1.1 2009/03/08 01:15:50 fujiik Exp $
//
#ifndef GEMDISPLAY_H
#define GEMDISPLAY_H

#include "TROOT.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TRootEmbeddedCanvas.h"
#include "TGButton.h"
#include "TGTextEntry.h"
#include "TGLabel.h"
#include "TGMenu.h"
#include "TDirectory.h"
#include "TTree.h"

class JREvent;

class GEMDisplay : public TGMainFrame {
private:
            GEMDisplay();
public:
  virtual  ~GEMDisplay();

  void      SetMonitorType(int type = 0);
  void      SetRun        (int run);
  void      SetInputDir   (const char *indir);
  int       GetEventNumber() { return fCurEvent;   }
  int       GetRunNumber  () { return fCurRun;     }
  TFile    *GetCurFile    () { return fCurFile;    }
  JREvent  *GetCurEvent   () { return fCurJREvent; }

  bool      GoToEvent(int evt, int run, int part);
  bool      NextEvent();
  bool      PreviousEvent();
  void      Add(const char *script);

  void      Initialize();
  void      ProcessButton(Long_t parm1);
  void      ProcessMenu  (Long_t parm1);
  bool      Display();

  TCanvas  *GetCanvas();
  void      PrintDisplay();

  // Base class method to override
  bool      ProcessMessage(Long_t msg, Long_t parm1, Long_t);

  static GEMDisplay *GetInstance();

private:
  TObjString & GetScript();
  void         UpdateStatus(const char* message = 0);

  enum EStatusBits { kScriptLoaded = BIT(15), kFileOpened = BIT(16) };

private:
  TDirectory          *fCanvasDirectory;	//!
  TRootEmbeddedCanvas *fCanvas;			//!
  TGCompositeFrame    *fCont;                   //!
  TGCompositeFrame    *fBottomButtons;          //!
  TGTextEntry         *fEventEntry;             //!
  TGCompositeFrame    *fStatusBar;              //!
  TGLabel             *fStatus;                 //!
  TGTextButton       **fButtons;                //!
  TGMenuBar           *fMenuBar;                //!
  TGPopupMenu         *fToolMenu;               //!
  TGPopupMenu         *fMonitorMenu;            //!
  const char          *fInputDir;               //! input file directory
  TFile               *fCurFile;                //! current input file
  TTree               *fCurTree;                //! current input tree
  JREvent             *fCurJREvent;             //! current input tree
  int                  fCurEvent;               //! current event
  int                  fCurRun;                 //! current run
  int                  fCurPart;                //! current part
  int                  fCurType;                //! current monitor type
  TObjArray            fScripts;                //!
  bool                 fIsInitialized;          //!
  bool                 fReset   ;               //!
  static GEMDisplay   *fgInstancePtr;           //!

  ClassDef(GEMDisplay, 1)   // A class to display event
};
#endif
