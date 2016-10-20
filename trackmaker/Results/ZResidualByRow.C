#define CHECK_DIST
#include <iomanip>
#include "../Runinfo.h"

  //const Int_t kNdl = 10;
  const Int_t kNdl = 11;
  //           dlen [cm]      5     10     15     20     25     30     35     40     45     50
  Int_t kRun[2][kNdl] = {{  6953,  6953,  6957,  6958,  6972,  6973,  6974,  6975,  6976,  6977},  // B=0T
//                        { 17319, 17367, 17324, 17364, 17327, 17359, 17331, 17356, 17340, 17352}}; // B=1T
//                        { 18241, 18238, 18218, 18221, 18224, 18226, 18228, 18230, 18233, 18243}}; // B=1T
//                        { 18412, 18413, 18414, 18415, 18417, 18418, 18419, 18420, 18421, 18423, 18424}}; // B=1T
//                        { 18564, 18563, 18562, 18561, 18552, 18551, 18550, 18549, 18548, 18547}}; // B=1T
                        { 18609, 18606, 18605, 18601, 18600, 18598, 18597, 18595, 18594, 18592, 18591}}; // B=1T

void ZResidualByRow(Int_t idl = 0, Int_t b= 1, Double_t p = 5.0)
{
//--Set Style
  // Axis
  gStyle->SetLabelFont(22,"X");
  gStyle->SetLabelFont(22,"Y");
  // Title offset
  gStyle->SetTitleOffset(1.0, "X");
  gStyle->SetTitleOffset(1.2, "Y");
  // Pad grid
  gStyle->SetPadGridX(kTRUE);
  gStyle->SetPadGridY(kTRUE);
  // Title
  gStyle->SetTitleFont(22, "X");
  gStyle->SetTitleFont(22, "Y");
  gStyle->SetTitleFont(22, "");

  const Int_t kNp = 84;
  Double_t xdata[kNp], ydata[kNp], dxdata[kNp], dydata[kNp];

  Double_t x, y, dx, dy;
  Int_t n = 0;
#if 0
  for (Int_t i=0; i<kNdl; i++) {
    Process(kRun[b][i], layer, x, y, dx, dy, p);
    xdata[n] = x; ydata[n] = y; dxdata[n] = dx; dydata[n] = dy; n++; 
  }
#else
  ofstream outfile("deltax.temp.dat");
  for (Int_t layer=0; layer<kNp; layer++) {
#if 0
    int slayer = layer/28;
    int row    = layer%28;
    if (slayer != 1 || row < 5 || row > 22) continue;
#endif
    Process(kRun[b][idl], layer, x, y, dx, dy, p);
    xdata[n] = x; ydata[n] = y; dxdata[n] = dx; dydata[n] = dy; n++; 
    outfile << x << " " << y << endl;
  }
  outfile.close();
#endif

  // ---------------
  // Reset Run Info.
  // ---------------
  Runinfo &rinfo = *Runinfo::GetInstancePtr();

  stringstream canvasstr;
  canvasstr << "c" << rinfo.GetDlength(kRun[b][idl]) << ends;
  TCanvas *c1 = new TCanvas(canvasstr.str().data(),"",800,500);
  c1->cd();

  TGraphErrors *grp = new TGraphErrors(n,xdata,ydata,dxdata,dydata);
  grp->GetHistogram()->GetXaxis()->SetLimits(0.,84.);
  grp->GetHistogram()->GetXaxis()->SetTitle("Layer Serial No.");
  grp->GetHistogram()->GetYaxis()->SetTitle("#Delta z [mm]");
  grp->Draw("ap");
  grp->SetMarkerColor(1);
  grp->SetMarkerStyle(21);
#if 1
  Double_t ymin = -3.0;
  Double_t ymax = +3.0;
#else
  Double_t ymin = -0.02;
  Double_t ymax = +0.02;
#endif
  grp->SetMinimum(ymin);
  grp->SetMaximum(ymax);

  TPaveText *pt = new TPaveText(330,4*ymax/5, 530.,(4.9*ymax)/5);

  stringstream titlestr;
  titlestr << "Residual (Drift Length " << rinfo.GetDlength(kRun[b][idl]) << "[cm])" << ends;
  grp->SetTitle(titlestr.str().data());
  pt->SetTextFont(132);
  pt->SetTextAlign(12);

#if 0
  // save plot as eps file
  stringstream ofile;
  ofile << "Residual_DL" << rinfo.GetDlength(kRun[b][idl]) << "_B" << b << "T" << ".eps"<< ends; 
  c1->Print(ofile.str().data());
#endif
}

TFile *hfp = 0;
static int lastrun = -1;

void Process(Int_t run, Int_t layer, Double_t &x, Double_t &y, Double_t &dx, Double_t &dy, Double_t p)
{
  // ---------------
  // Reset Run Info.
  // ---------------
  Runinfo &rinfo = *Runinfo::GetInstancePtr();
#ifdef CHECK_DIST
  stringstream hstr;
  hstr << "c" << "_" << layer << ends;
  TCanvas *cp = new TCanvas(hstr.str().data(),hstr.str().data(),400,400);
  cp->cd();
#endif

  if (lastrun != run) {
    stringstream finstr;
    finstr << "10k/p." << p << "_z." << rinfo.GetDlength(run) << "_B." << rinfo.GetBfield(run) << ".root" << ends;
    hfp = new TFile(finstr.str().data());
  }

  TNtupleD *hResXin = static_cast<TNtupleD *>(gROOT->FindObject("hResXin"));
  stringstream item;
  item << "dzin" << setw(2) << setfill('0') << layer << ends;
  stringstream cut;

  ///// Cuts ////////////////////////
#if 0
  const Double_t kResZmin   = -0.1;
  const Double_t kResZmax   = +0.1;
  const Int_t    kNdfCut    =  28;
  const Double_t kChi2Cut   = 1000.;
  const Double_t kCpaMinCut =  0.2;
  const Double_t kCpaMaxCut =  0.8;
#else
#if 0
  const Double_t kResZmin   = -0.3;
  const Double_t kResZmax   = +0.3;
  //const Int_t    kNdfCut    =  140;
  //const Int_t    kNdfCut    =  32;
  const Int_t    kNdfCut    =  20;
  //const Double_t kChi2Cut   = 10000.;
  const Double_t kChi2Cut   = 300.;
  //const Double_t kCpaMinCut = -0.2;
  //const Double_t kCpaMaxCut =  0.8;
  const Double_t kCpaMinCut = -4.;
  const Double_t kCpaMaxCut =  4.;
  const Double_t kPhi0MinCut = 6.22;
  const Double_t kPhi0MaxCut = 6.28;
  const Double_t kTanlMinCut = -0.02;
  const Double_t kTanlMaxCut = +0.02;
#else
  //const Double_t kResZmin   = -0.3;
  //const Double_t kResZmax   = +0.3;
  const Double_t kResZmin   = -2;
  const Double_t kResZmax   = +2;
  const Int_t    kNdfCut    =  80;
  //const Int_t    kNdfCut    =  34;
  //const Int_t    kNdfCut    =  30;
  const Double_t kChi2Cut   = 10000.;
  const Double_t kCpaMinCut = -4.;
  const Double_t kCpaMaxCut =  4.;
  //const Double_t kPhi0MinCut = 6.22;
  //const Double_t kPhi0MaxCut = 6.28;
  const Double_t kPhi0MinCut = 6.00;
  const Double_t kPhi0MaxCut = 6.50;
  const Double_t kTanlMinCut = -0.02;
  const Double_t kTanlMaxCut = +0.02;
#endif
#endif
  ///////////////////////////////////
  cut << item.str().data() << ">" << kResZmin << "&&"
      << item.str().data() << "<" << kResZmax << "&&"
      << "ndf>"  << kNdfCut    << "&&"
      << "chi2<" << kChi2Cut   << "&&"
      << "fi0>"  << kPhi0MinCut << "&&"
      << "fi0<"  << kPhi0MaxCut << "&&"
      << "tnl>"  << kTanlMinCut << "&&"
      << "tnl<"  << kTanlMaxCut;
  if (rinfo.GetBfield(run)) {
    cut << "&&"
        << "cpa>"  << kCpaMinCut << "&&"
        << "cpa<"  << kCpaMaxCut;
  }
  cut << ends;

  stringstream target;
  target << item.str().data() << ">>htemp(100," << kResZmin << ", " << kResZmax << ")" << ends;
  hResZin->Draw(target.str().data(), cut.str().data());
  TH1 *htemp = static_cast<TH1 *>(gROOT->FindObject("htemp"));
  Double_t y, dy;
  if (htemp && htemp->GetEntries()) {
    htemp->Rebin(1);
    htemp->Fit("gaus","","",-0.3,0.3);
    Double_t mu  = htemp->GetFunction("gaus")->GetParameter(1);
    Double_t sg  = htemp->GetFunction("gaus")->GetParameter(2);
    htemp->Fit("gaus","","",mu-2.5*sg,mu+2.5*sg);

    y  = htemp->GetFunction("gaus")->GetParameter(1);
    dy = htemp->GetFunction("gaus")->GetParError(1);
    y  *= 10.;
    dy *= 10.;

#ifdef CHECK_DIST
    htemp->SetMinimum(-0.1);
    htemp->SetMarkerStyle(4);
    htemp->SetMarkerSize(0.7);
    htemp->SetMarkerColor(2);
    htemp->SetLineColor(2);
    htemp->DrawCopy("pe");
#endif
  } else {
    y  = 0;
    dy = 0;
  }
  x  = layer;
  dx = 0;
  cerr << x << "+/-" << dx << " " << y << "+/-" << dy << endl;
  lastrun = run;
}
