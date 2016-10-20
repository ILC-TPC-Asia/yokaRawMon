#define CHECK_DIST
//#define PED_CORR
#include <iomanip>
#include "../Runinfo.h"

void Process(Int_t run, Int_t layer, Double_t &x, Double_t &y, Double_t &dx, Double_t &dy, Double_t p);

void PadRes(Int_t b = 1, Int_t layer = 48, Double_t p = 5.0)
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

#if 0
  const Int_t kNp = 9;
  Double_t xdata[kNp], ydata[kNp], dxdata[kNp], dydata[kNp];

  //           dlen [cm]    10    15    20    25    30    35    40    45    50
  Int_t kRun[2][kNp] = {{ 6953, 6957, 6958, 6972, 6973, 6974, 6975, 6976, 6977},  // B=0T
                        { 7049, 7046, 7051, 7053, 7059, 7061, 7065, 7020, 7023}}; // B=1T
#else
#if 0
  const Int_t kNp = 11;
  Double_t xdata[kNp], ydata[kNp], dxdata[kNp], dydata[kNp];
  //           dlen [cm]      5     10     15     20     25     30     35     40     45     50     55
  Int_t kRun[2][kNp] = {{  6953,  6953,  6957,  6958,  6972,  6973,  6974,  6975,  6976,  6977,  6977},  // B=0T
                        { 17319, 17367, 17324, 17364, 17327, 17359, 17331, 17356, 17340, 17352, 17344}}; // B=1T
#else
  const Int_t kNp = 10;
  Double_t xdata[kNp], ydata[kNp], dxdata[kNp], dydata[kNp];
  //           dlen [cm]      5     10     15     20     25     30     35     40     45     50
  Int_t kRun[2][kNp] = {{  6953,  6953,  6957,  6958,  6972,  6973,  6974,  6975,  6976,  6977},  // B=0T
                        //{ 17319, 17367, 17324, 17364, 17327, 17359, 17331, 17356, 17340, 17352}}; // B=1T
                        { 18564, 18563, 18562, 18561, 18552, 18551, 18550, 18549, 18548, 18547}}; // B=1T
#endif
#endif

  Double_t x, y, dx, dy;
  Int_t n = 0;
  for (Int_t i=0; i<kNp; i++) {
    Process(kRun[b][i], layer, x, y, dx, dy, p);
    xdata[n] = x; ydata[n] = y*y; dxdata[n] = dx; dydata[n] = dy; n++; 
  }

  TCanvas *c1 = new TCanvas("c1","",800,500);
  c1->cd();

  TGraphErrors *grp = new TGraphErrors(n,xdata,ydata,dxdata,dydata);
  grp->GetHistogram()->GetXaxis()->SetLimits(0.,550.);
  grp->GetHistogram()->GetXaxis()->SetTitle("Drift Length: z [mm]");
  grp->GetHistogram()->GetYaxis()->SetTitle("#sigma_{PR}(0)^{2} [mm^{2}]");
  grp->Draw("ap");
  grp->SetMarkerColor(1);
  grp->SetMarkerStyle(21);
  grp->SetMinimum(0.);
  Double_t ymax = 2.;
  grp->SetMaximum(ymax);
  TF1 fun("fun","[0]**2+[1]**2*x");
  fun.SetLineStyle(2);
  fun.SetLineColor(2);
  grp->Fit(&fun);
  grp->Fit(&fun);
  grp->Fit(&fun);
  Double_t sigma0  = fun.GetParameter(0) * 1.e3;             // [#mum]
  Double_t dsigma0 = fun.GetParError (0) * 1.e3;             // [#mum]
  Double_t cd      = fun.GetParameter(1) * sqrt(10.) * 1e3;  // [#mum/#sqrt{cm}]
  Double_t dcd     = fun.GetParError (1) * sqrt(10.) * 1e3;  // [#mum/#sqrt{cm}]

  TPaveText *pt = new TPaveText(330,8*ymax/10, 530.,8*ymax/10+1.5*ymax/10);

  stringstream titlestr;
  titlestr << "Pad Response (Row" << layer << ")" << ends;
  grp->SetTitle(titlestr.str().data());

  pt->SetTextFont(132);
  pt->SetTextAlign(12);
  stringstream stext;
  stext << "#sigma_{PR}(0) = " << setw(6) << setprecision(4) << sigma0 
        << " #pm "             << setw(3) << setprecision(2) << dsigma0 
        << " [#mum]"           << ends;
  pt->AddText(stext.str().data());
  stext.str("");
  stext.clear();
  stext << "C_{D} = " << setw(6) << setprecision(4) << cd
        << " #pm "    << setw(3) << setprecision(2) << dcd
        << " [#mum/#sqrt{cm}]" << ends;
  pt->AddText(stext.str().data());
  pt->Draw();

  TLatex *fitfun = new TLatex;
  fitfun->SetTextFont(132);
  fitfun->SetTextAlign(12);
  fitfun->DrawLatex(50.,ymax/2, "#sigma_{PR}^{2} = #sigma_{PR}(0)^{2}+(C_{D}^{2}) z");

  // save plot as eps file
  stringstream ofile;
  ofile << "PadRes_Row" << layer << "_B" << b << "T" << ".eps"<< ends; 
  c1->Print(ofile.str().data());
}

void Process(Int_t run, Int_t layer, Double_t &x, Double_t &y, Double_t &dx, Double_t &dy, Double_t p)
{
  // ---------------
  // Reset Run Info.
  // ---------------
  Runinfo &rinfo = *Runinfo::GetInstancePtr();

  stringstream finstr;
  finstr << "10k/p." << p << "_z." << rinfo.GetDlength(run) << "_B." << rinfo.GetBfield(run) << ".root" << ends;
  TFile *hfp = new TFile(finstr.str().data());

  stringstream hstr;
  hstr << "qx" << setw(2) << setfill('0') << layer << ends;
#ifdef CHECK_DIST
  stringstream padresstr;
  padresstr << "c" << hstr.str().data() << "_" << rinfo.GetDlength(run) << ends;
  TCanvas *cp = new TCanvas(padresstr.str().data(),"",400,400);
  cp->cd();
#endif
  TH2D *hq = static_cast<TH2D *>(gROOT->FindObject(hstr.str().data()));
  hq->Draw(hstr.str().data());
  hq->FitSlicesY();

  stringstream hslicestr;
  hslicestr << hstr.str().data() << "_1" << ends;
  TH1D *hslice = static_cast<TH1D *>(gROOT->FindObject(hslicestr.str().data()));

#ifdef PED_CORR
  TF1 func("user","[0]*exp(-pow((x-[1])/[2],2)/2.)/sqrt(2*3.141592*[2])+[3]",-8.,+8.);
  func.SetParameter(0,1.);
  func.SetParameter(1,0.);
  func.SetParameter(2,1.);
  func.SetParameter(3,0.);
  hslice->Rebin(1);
  hslice->Fit("user","","",-3.,+3.);
  y  = hslice->GetFunction("user")->GetParameter(2);
  hslice->Fit("user","","",-5*y,5*y);

  x  = rinfo.GetDlength(run);
  y  = hslice->GetFunction("user")->GetParameter(2);
  dx = 0;
  dy = hslice->GetFunction("user")->GetParError(2);
  x *= 10;
#else
  hslice->Rebin(1);
  hslice->Fit("gaus","","",-4.0,4.0);
  y  = hslice->GetFunction("gaus")->GetParameter(2);
  if (rinfo.GetBfield(run) == 1) {
    hslice->Fit("gaus","","",-3*y,3*y);
  }
  if (rinfo.GetBfield(run) == 0) {
    hslice->Fit("gaus","","",-5*y,5*y);
  }

  x  = rinfo.GetDlength(run);
  y  = hslice->GetFunction("gaus")->GetParameter(2);
  dx = 0;
  dy = hslice->GetFunction("gaus")->GetParError(2);
  x *= 10;
#endif
#ifdef CHECK_DIST
  hslice->SetMinimum(-0.1);
  hslice->SetMaximum(1.2);
  hslice->SetMarkerStyle(4);
  hslice->SetMarkerSize(0.7);
  hslice->SetMarkerColor(2);
  hslice->SetLineColor(2);
  hslice->Draw("pe");
#endif
}
