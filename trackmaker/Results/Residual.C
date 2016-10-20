#define CHECK_DIST
#include <iomanip>
#include "../Runinfo.h"

void Process(Int_t run, Int_t layer, Double_t &x, Double_t &y, Double_t &dx, Double_t &dy, Double_t p);
void Residual(Int_t layer = 48, Int_t b = 1, Double_t p = 5.0)
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

  const Int_t kNp = 10;
  //const Int_t kNp = 11;
  Double_t xdata[kNp], ydata[kNp], dxdata[kNp], dydata[kNp];
  //           dlen [cm]      5     10     15     20     25     30     35     40     45     50
  Int_t kRun[2][kNp] = {{  6953,  6953,  6957,  6958,  6972,  6973,  6974,  6975,  6976,  6977},  // B=0T
//                        { 17319, 17367, 17324, 17364, 17327, 17359, 17331, 17356, 17340, 17352}}; // B=1T
//                        { 18241, 18238, 18218, 18221, 18224, 18226, 18228, 18230, 18233, 18243}}; // B=1T
//                        { 18412, 18413, 18414, 18415, 18417, 18418, 18419, 18420, 18421, 18423, 18424}}; // B=1T
                        { 18564, 18563, 18562, 18561, 18552, 18551, 18550, 18549, 18548, 18547}}; // B=1T
//                        { 18609, 18606, 18605, 18601, 18600, 18598, 18597, 18595, 18594, 18592, 18591}}; // B=1T

  Double_t x, y, dx, dy;
  Int_t n = 0;
  n = 0;
  for (Int_t i=0; i<kNp; i++) {
    Process(kRun[b][i], layer, x, y, dx, dy, p);
    xdata[n] = x; ydata[n] = y; dxdata[n] = dx; dydata[n] = dy; n++; 
  }

  TCanvas *c1 = new TCanvas("c1","",800,500);
  c1->cd();

  TGraphErrors *grp = new TGraphErrors(n,xdata,ydata,dxdata,dydata);
  grp->GetHistogram()->GetXaxis()->SetLimits(0.,550.);
  grp->GetHistogram()->GetXaxis()->SetTitle("Drift Length: z [mm]");
  grp->GetHistogram()->GetYaxis()->SetTitle("#Delta x [mm]");
  grp->Draw("ap");
  grp->SetMarkerColor(1);
  grp->SetMarkerStyle(21);
  Double_t ymin = -3.0;
  Double_t ymax = +3.0;
  grp->SetMinimum(ymin);
  grp->SetMaximum(ymax);
  TF1 fun("fun","[0]+[1]*x");
  fun.SetLineStyle(2);
  fun.SetLineColor(2);
  grp->Fit(&fun,"","",10.,600.);
  grp->Draw("same");
  Double_t p0  = fun.GetParameter(0) * 1.e3; // [#mum]
  Double_t dp0 = fun.GetParError (0) * 1.e3; // [#mum]
  Double_t p1  = fun.GetParameter(1) * 1.e4; // [#mum/cm]
  Double_t dp1 = fun.GetParError (1) * 1.e4; // [#mum/cm]

  TPaveText *pt = new TPaveText(330,4*ymax/5, 530.,(4.9*ymax)/5);

  stringstream titlestr;
  titlestr << "Residual (Row" << layer << ")" << ends;
  grp->SetTitle(titlestr.str().data());
  pt->SetTextFont(132);
  pt->SetTextAlign(12);
  stringstream stext;
  stext << "#Delta x_{0} = " << setw(6) << setprecision(3) << p0 
        << " #pm " << setw(4) << setprecision(2) << dp0
        << " [#mum]" << ends;
  pt->AddText(stext.str().data());
  stext.str("");
  stext.clear();
  stext << "Slope = " << setw(6) << setprecision(3) << p1
        << " #pm " <<  setw(4) << setprecision(2) << dp1 
        << " [#mum/cm]" << ends;
  pt->AddText(stext.str().data());
  pt->Draw();

  TLatex *fitfun = new TLatex;
  fitfun->SetTextFont(132);
  fitfun->SetTextAlign(12);
  fitfun->DrawLatex(100,ymax/5, "#Delta x = #Delta x_{0}+Slope * z");

  // save plot as eps file
  stringstream ofile;
  ofile << "Residual_Row" << layer << "_B" << b << "T" << ".eps"<< ends; 
  c1->Print(ofile.str().data());
}

void Process(Int_t run, Int_t layer, Double_t &x, Double_t &y, Double_t &dx, Double_t &dy, Double_t p)
{
  // ---------------
  // Reset Run Info.
  // ---------------
  Runinfo &rinfo = *Runinfo::GetInstancePtr();

#ifdef CHECK_DIST
  TDirectory *last = gDirectory;
  stringstream hstr;
  hstr << "c" << "_" << rinfo.GetDlength(run) << ends;
  TCanvas *cp = new TCanvas(hstr.str().data(),"",400,400);
  cp->cd();
#endif
  stringstream finstr;
  finstr << "10k/p." << p << "_z." << rinfo.GetDlength(run) << "_B." << rinfo.GetBfield(run) << ".root" << ends;
  TFile *hfp = new TFile(finstr.str().data());

  TNtupleD *hResXin = static_cast<TNtupleD *>(gROOT->FindObject("hResXin"));
  stringstream item;
  item << "dxin" << setw(2) << setfill('0') << layer << ends;
  stringstream cut;

  ///// Cuts ////////////////////////
#if 0
  const Double_t kResXmin   = -0.3;
  const Double_t kResXmax   = +0.3;
  const Int_t    kNdfCut    =  40;
  const Double_t kChi2Cut   = 10000.;
  const Double_t kCpaMinCut = -0.8;
  const Double_t kCpaMaxCut =  0.0;
#else
  const Double_t kResXmin   = -0.3;
  const Double_t kResXmax   = +0.3;
  //const Int_t    kNdfCut    =  140;
  //const Int_t    kNdfCut    =  32;
  //const Int_t    kNdfCut    =  28;
  //const Int_t    kNdfCut    =  24;
  const Int_t    kNdfCut    =  100;
  const Double_t kChi2Cut   = 10000.;
  //const Double_t kCpaMinCut = -0.2;
  //const Double_t kCpaMaxCut =  0.8;
  const Double_t kCpaMinCut = -4.;
  const Double_t kCpaMaxCut =  4.;
#endif
  ///////////////////////////////////
  cut << item.str().data() << ">" << kResXmin << "&&"
      << item.str().data() << "<" << kResXmax << "&&"
      << "ndf>"  << kNdfCut    << "&&"
      << "chi2<" << kChi2Cut;
  if (rinfo.GetBfield(run)) {
    cut << "&&"
        << "cpa>"  << kCpaMinCut << "&&"
        << "cpa<"  << kCpaMaxCut;
  }
  cut << ends;

    hResXin->Draw(item.str().data(), cut.str().data());
    TH1D *htemp = static_cast<TH1D *>(gROOT->FindObject("htemp"));

    htemp->Rebin(1);
    htemp->Fit("gaus","","",-0.3,0.3);
    Double_t mu  = htemp->GetFunction("gaus")->GetParameter(1);
    Double_t sg  = htemp->GetFunction("gaus")->GetParameter(2);
    htemp->Fit("gaus","","",mu-2.5*sg,mu+2.5*sg);

    x  = rinfo.GetDlength(run);
    y  = htemp->GetFunction("gaus")->GetParameter(1);
    dx = 0;
    dy = htemp->GetFunction("gaus")->GetParError(1);
    x  *= 10.;
    y  *= 10.;
    dx *= 10.;
    dy *= 10.;
    cerr << x << "+/-" << dx << " " << y << "+/-" << dy << endl;
#ifdef CHECK_DIST
    htemp->SetMinimum(-0.1);
    htemp->SetMarkerStyle(4);
    htemp->SetMarkerSize(0.7);
    htemp->SetMarkerColor(2);
    htemp->SetLineColor(2);
    htemp->Draw("pe");
    last->cd();
#endif
  
}
