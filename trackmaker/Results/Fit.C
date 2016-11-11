#include <iomanip>
#include "../Runinfo.h"

void Fit(Int_t layer = 44, const Char_t *inout = "in", int b = 1, int p = 5)
{
  const Int_t kNp = 12;
  Double_t xdata[kNp], ydata[kNp], dxdata[kNp], dydata[kNp];
  Double_t x, y, dx, dy;
  Int_t n = 0;

  ////           dlen [cm]    10    15    20    25    30    35    40    45    50
  //Int_t kRun[2][kNp] = {{ 6953, 6957, 6958, 6972, 6973, 6974, 6975, 6976, 6977},  // B=0T
  //                      { 7049, 7046, 7051, 7053, 7059, 7061, 7065, 7020, 7023}}; // B=1T
  //           dlen [cm]      5     10     15     20     25     30     35     40     45     50
  Int_t kRun[2][kNp] = {{  6953,  6953,  6957,  6958,  6972,  6973,  6974,  6975,  6976,  6977,1111,1112},  // B=0T
                        //{ 17319, 17367, 17324, 17364, 17327, 17359, 17331, 17356, 17340, 17352}}; // B=1T
                        { 19678, 19677, 19676, 19675, 19674, 19673, 19672, 19671, 19670, 19669, 19668, 19667}}; // B=1T

  for (Int_t i=0; i<kNp; i++) {
    cerr << kRun[b][i] << endl;
    Process(kRun[b][i], layer, x, y, dx, dy, inout, p);
    xdata[n] = x; ydata[n] = y; dxdata[n] = dx; dydata[n] = dy; n++; 
  }

  TGraphErrors *grp = new TGraphErrors(n,xdata,ydata,dxdata,dydata);
  grp->Draw("ap");
  grp->SetMarkerColor(1);
  grp->SetMarkerStyle(21);
  grp->SetMinimum(0.);
  grp->SetMaximum(0.25);
  TF1 fun("fun","[0]**2+[1]**2*x");
  fun.SetLineStyle(2);
  fun.SetLineColor(2);
  grp->Fit(&fun);
  grp->Fit(&fun);
}

void Process(Int_t run, Int_t layer, Double_t &x, Double_t &y, Double_t &dx, Double_t &dy, const Char_t *inout, Double_t p)
{
  // ---------------
  // Reset Run Info.
  // ---------------
  Runinfo &rinfo = *Runinfo::GetInstancePtr();

  stringstream finstr;
  finstr << "10k/p." << p << "_z." << rinfo.GetDlength(run) << "_B." << rinfo.GetBfield(run) << ".root" << ends;
  TFile *hfp = new TFile(finstr.str().data());
  TNtupleD *hResXin = static_cast<TNtupleD *>(gROOT->FindObject("hResXin"));

  stringstream item;
  item << "dx" << inout << setw(2) << setfill('0') << layer << ends;

  stringstream cut;
  cut << "abs(" << item.str().data() << ")<0.4&&ndf>150" << ends;

  hResXin->Draw(item.str().data(), cut.str().data());
  htemp->Fit("gaus");

  x  = rinfo.GetDlength(run);
  y  = htemp->GetFunction("gaus")->GetParameter(2);
  dx = 0;
  dy = htemp->GetFunction("gaus")->GetParError(2);
  x  *= 10.;
  y  *= 10.;
  dx *= 10.;
  dy *= 10.;
}
