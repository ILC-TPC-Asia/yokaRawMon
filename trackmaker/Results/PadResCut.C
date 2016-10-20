#define CHECK_DIST
#define PED_CORR
#include <iomanip>
#include "../Runinfo.h"

void Process(Int_t , Int_t , Double_t &, Double_t &, Double_t &, Double_t &, Double_t );

void PadResCut(Int_t b = 1, Int_t layer = 44, Double_t p = 5.0)
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
  //const Int_t kNp = 11;
  Double_t xdata[kNp], ydata[kNp], dxdata[kNp], dydata[kNp];
  //           dlen [cm]      5     10     15     20     25     30     35     40     45     50
  Int_t kRun[2][kNp] = {{  6953,  6953,  6957,  6958,  6972,  6973,  6974,  6975,  6976,  6977},  // B=0T
//                        { 17319, 17367, 17324, 17364, 17327, 17359, 17331, 17356, 17340, 17352}}; // B=1T
//                        { 18241, 18238, 18218, 18221, 18224, 18226, 18228, 18230, 18233, 18243}}; // B=1T
//                        { 18412, 18413, 18414, 18415, 18417, 18418, 18419, 18420, 18421, 18423, 18424}}; // B=1T
                        { 18564, 18563, 18562, 18561, 18552, 18551, 18550, 18549, 18548, 18547}}; // B=1T
//                        { 18609, 18606, 18605, 18601, 18600, 18598, 18597, 18595, 18594, 18592, 18591}}; // B=1T
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
  fun.SetParameter(0,0.5);
  fun.SetParameter(1,0.02);
  grp->Fit(&fun);
  //grp->Fit(&fun,"","",350.,600.);
  grp->Fit(&fun,"","",250.,600.);
  //grp->Fit(&fun,"","",100.,600.);
  //grp->Fit(&fun);
  //grp->Fit(&fun);
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

#if 0
  // save plot as eps file
  stringstream ofile;
  ofile << "PadRes_Row" << layer << "_B" << b << "T" << ".eps"<< ends; 
  c1->Print(ofile.str().data());
#endif
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

  TNtupleD *hResXin = static_cast<TNtupleD *>(gROOT->FindObject("hResXin"));
  stringstream hstr;
  hstr << "qx" << setw(2) << setfill('0') << layer << ends;
#ifdef CHECK_DIST
  stringstream padresstr;
  padresstr << "c" << hstr.str().data() << "_" << rinfo.GetDlength(run) << ends;
  TCanvas *cp = new TCanvas(padresstr.str().data(),"",400,400);
  cp->cd();
#endif
  stringstream cut;
  ///// Cuts ////////////////////////
#if 0
  const Double_t kResXmin   = -0.1;
  const Double_t kResXmax   = +0.1;
  const Int_t    kNdfCut    =  28;
  const Double_t kChi2Cut   = 9000.;
  const Double_t kCpaMinCut = -0.8;
  const Double_t kCpaMaxCut =  0.4;
  const Double_t kPhi0MinCut = 6.22;
  const Double_t kPhi0MaxCut = 6.28;
  const Double_t kTanlMinCut = -0.02;
  const Double_t kTanlMaxCut = +0.02;
#else
  const Double_t kResXmin   = -0.10;
  const Double_t kResXmax   = +0.10;
  //const Double_t kResXmin   = -0.15;
  //const Double_t kResXmax   = +0.15;
#if 0
  //const Int_t    kNdfCut    =   155;
  const Int_t    kNdfCut    =  130;
  //const Int_t    kNdfCut    =  140;
  //const Int_t    kNdfCut    =  100;
  //const Double_t kChi2Cut   = 80000.;
  //const Double_t kChi2Cut   = 40000.;
  //const Double_t kChi2Cut   = 10000.;
  const Double_t kChi2Cut   = 40000.;
  const Double_t kCpaMinCut =  0.3;
  const Double_t kCpaMaxCut =  0.5;
#else
  //const Int_t    kNdfCut    =  32;
  //const Int_t    kNdfCut    =  28;
  //const Int_t    kNdfCut    =  24;
  //const Int_t    kNdfCut    =  140;
  //const Int_t    kNdfCut    =  155;
  const Int_t    kNdfCut    =  100;
  const Double_t kChi2Cut   = 10000.;
  //const Double_t kCpaMinCut = -0.2;
  //const Double_t kCpaMaxCut =  0.8;
  const Double_t kCpaMinCut = -4.;
  const Double_t kCpaMaxCut =  4.;
#endif
  //const Double_t kPhi0MinCut = 6.245;
  //const Double_t kPhi0MaxCut = 6.275;
  //const Double_t kPhi0MinCut = 6.24;
  //const Double_t kPhi0MaxCut = 6.27;
  //const Double_t kPhi0MinCut = 6.22;
  //const Double_t kPhi0MaxCut = 6.28;
  //const Double_t kPhi0MinCut = 6.00;
  //const Double_t kPhi0MaxCut = 6.50;
  //
#if 0
  const Double_t kPhi0MinCut = 6.24; // 6.218;
  const Double_t kPhi0MaxCut = 6.28; // 6.314;
  const Double_t kTanlMinCut = -0.02;
  const Double_t kTanlMaxCut = +0.02;
  const Double_t kPkPhMinCut = 10.;
  const Double_t kPkPhMaxCut = 900.;
#else
  const Double_t kPhi0MinCut = 6.00; // 6.218;
  const Double_t kPhi0MaxCut = 6.60; // 6.314;
  const Double_t kTanlMinCut = -0.2;
  const Double_t kTanlMaxCut = +0.2;
  const Double_t kPkPhMinCut = 10.;
  const Double_t kPkPhMaxCut = 900.;
#endif
#endif
  ///////////////////////////////////
  cut << "ndf>"  << kNdfCut    << "&&"
      << "chi2<" << kChi2Cut   << "&&"
      << "((fi0>"<< kPhi0MinCut << "&&"
      << "fi0<"  << kPhi0MaxCut << ")||"
      << "(fi0+2*TMath::Pi()>"  << kPhi0MinCut << "&&"
      << "fi0+2*TMath::Pi()<"   << kPhi0MaxCut << "))&&"
      << "tnl>"  << kTanlMinCut << "&&"
      << "tnl<"  << kTanlMaxCut << "&&"
      << "pk" << layer << ">" << kPkPhMinCut << "&&"
      << "pk" << layer << "<" << kPkPhMaxCut;
  if (rinfo.GetBfield(run)) {
    cut << "&&"
        << "cpa>"  << kCpaMinCut << "&&"
        << "cpa<"  << kCpaMaxCut;
  }
  cut << ends;
  stringstream target;
  target << "chm5" << layer << ":xpm5" << layer << ">>htempm5(60,-3.0,3.0,100,0.,1.)" << ends;
  hResXin->Draw(target.str().data(), cut.str().data());
  target.str("");
  target.clear();
  target << "chm4" << layer << ":xpm4" << layer << ">>htempm4(60,-3.0,3.0,100,0.,1.)" << ends;
  hResXin->Draw(target.str().data(), cut.str().data());
  target.str("");
  target.clear();
  target << "chm3" << layer << ":xpm3" << layer << ">>htempm3(60,-3.0,3.0,100,0.,1.)" << ends;
  hResXin->Draw(target.str().data(), cut.str().data());
  target.str("");
  target.clear();
  target << "chm2" << layer << ":xpm2" << layer << ">>htempm2(60,-3.0,3.0,100,0.,1.)" << ends;
  hResXin->Draw(target.str().data(), cut.str().data());
  target.str("");
  target.clear();
  target << "chm1" << layer << ":xpm1" << layer << ">>htempm1(60,-3.0,3.0,100,0.,1.)" << ends;
  hResXin->Draw(target.str().data(), cut.str().data());
  target.str("");
  target.clear();
  target << "chm0" << layer << ":xpm0" << layer << ">>htempm0(60,-3.0,3.0,100,0.,1.)" << ends;
  hResXin->Draw(target.str().data(), cut.str().data());
  target.str("");
  target.clear();
  target << "chp1" << layer << ":xpp1" << layer << ">>htempp1(60,-3.0,3.0,100,0.,1.)" << ends;
  hResXin->Draw(target.str().data(), cut.str().data());
  target.str("");
  target.clear();
  target << "chp2" << layer << ":xpp2" << layer << ">>htempp2(60,-3.0,3.0,100,0.,1.)" << ends;
  hResXin->Draw(target.str().data(), cut.str().data());
  target.str("");
  target.clear();
  target << "chp3" << layer << ":xpp3" << layer << ">>htempp3(60,-3.0,3.0,100,0.,1.)" << ends;
  hResXin->Draw(target.str().data(), cut.str().data());
  target.str("");
  target.clear();
  target << "chp4" << layer << ":xpp4" << layer << ">>htempp4(60,-3.0,3.0,100,0.,1.)" << ends;
  hResXin->Draw(target.str().data(), cut.str().data());
  target.str("");
  target.clear();
  target << "chp5" << layer << ":xpp5" << layer << ">>htempp5(60,-3.0,3.0,100,0.,1.)" << ends;
  hResXin->Draw(target.str().data(), cut.str().data());

  TH2F *htempm0 = static_cast<TH2F *>(gROOT->FindObject("htempm0"));
  TH2F *htempm1 = static_cast<TH2F *>(gROOT->FindObject("htempm1"));
  TH2F *htempm2 = static_cast<TH2F *>(gROOT->FindObject("htempm2"));
  TH2F *htempm3 = static_cast<TH2F *>(gROOT->FindObject("htempm3"));
  TH2F *htempm4 = static_cast<TH2F *>(gROOT->FindObject("htempm4"));
  TH2F *htempm5 = static_cast<TH2F *>(gROOT->FindObject("htempm5"));
  TH2F *htempp1 = static_cast<TH2F *>(gROOT->FindObject("htempp1"));
  TH2F *htempp2 = static_cast<TH2F *>(gROOT->FindObject("htempp2"));
  TH2F *htempp3 = static_cast<TH2F *>(gROOT->FindObject("htempp3"));
  TH2F *htempp4 = static_cast<TH2F *>(gROOT->FindObject("htempp4"));
  TH2F *htempp5 = static_cast<TH2F *>(gROOT->FindObject("htempp5"));

  TH2F *hq = new TH2F(hstr.str().data(),"",60,-3.0,3.0,100,0.,1);
#if 0
  *hq = (*htempm5) + (*htempm4) + (*htempm3) + (*htempm2) + (*htempm1) + (*htempm0)
      + (*htempp1) + (*htempp2) + (*htempp3) + (*htempp4) + (*htempp5);
#else
  *hq = (*htempm5);
  *hq = *hq + (*htempm4);
  *hq = *hq + (*htempm3);
  *hq = *hq + (*htempm2);
  *hq = *hq + (*htempm1);
  *hq = *hq + (*htempm0);
  *hq = *hq + (*htempp1);
  *hq = *hq + (*htempp2);
  *hq = *hq + (*htempp3);
  *hq = *hq + (*htempp4);
  *hq = *hq + (*htempp5);
#endif
  hq->SetName(hstr.str().data());
  hq->Draw(hstr.str().data());
  //hq->FitSlicesY();
  hq->FitSlicesY(0,0,-1,0,"QNRI");

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

  x  = rinfo.GetDlength(run) + rinfo.GetDLOffset(run);
  y  = hslice->GetFunction("user")->GetParameter(2);
  dx = 0;
  dy = hslice->GetFunction("user")->GetParError(2);
  x *= 10;
#else
  hslice->Rebin(1);
  //hslice->Fit("gaus","RI","",-4.0,4.0);
  hslice->Fit("gaus","RI","",-2.0,2.0);
  y  = hslice->GetFunction("gaus")->GetParameter(2);
  if (rinfo.GetBfield(run) == 1) {
    hslice->Fit("gaus","RI","",-3*y,3*y);
  }
  if (rinfo.GetBfield(run) == 0) {
    hslice->Fit("gaus","RI","",-5*y,5*y);
  }

  x  = rinfo.GetDlength(run) + rinfo.GetDLOffset(run);
  y  = hslice->GetFunction("gaus")->GetParameter(2);
  dx = 0;
  dy = hslice->GetFunction("gaus")->GetParError(2);
  x *= 10;
#endif
#ifdef CHECK_DIST
  //hq->Draw();
  hslice->SetMinimum(-0.1);
  hslice->SetMaximum(1.2);
  hslice->SetMarkerStyle(4);
  hslice->SetMarkerSize(0.7);
  hslice->SetMarkerColor(2);
  hslice->SetLineColor(2);
  hslice->Draw("pe same");
#endif
}
