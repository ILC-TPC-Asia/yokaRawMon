#define CHECK_DIST
#include <iomanip>
#include "../Runinfo.h"

void MomResol(Double_t p = 5.0)
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
  Double_t xdata[kNp], ydata[kNp], dxdata[kNp], dydata[kNp];
  //       dlen [cm]      5     10     15     20     25     30     35     40     45     50
  Int_t kRun[kNp] = { 17319, 17367, 17324, 17364, 17327, 17359, 17331, 17356, 17340, 17352}; // B=1T

  Double_t x, y, dx, dy;
  Int_t n = 0;
  for (Int_t i=0; i<kNp; i++) {
    Process(kRun[i], x, y, dx, dy, p);
    xdata[n] = x; ydata[n] = y; dxdata[n] = dx; dydata[n] = dy; n++; 
  }

  TCanvas *c1 = new TCanvas("c1","",800,500);
  c1->cd();

  TGraphErrors *grp = new TGraphErrors(n,xdata,ydata,dxdata,dydata);
  grp->GetHistogram()->GetXaxis()->SetLimits(0.,550.);
  grp->GetHistogram()->GetXaxis()->SetTitle("Drift Length: z [mm]");
  grp->GetHistogram()->GetYaxis()->SetTitle("#sigma_{#kappa} [1/GeV]");
  grp->Draw("ap");
  grp->SetMarkerColor(1);
  grp->SetMarkerStyle(21);
  grp->SetMinimum(0.005);
#if 1
  grp->SetMaximum(0.020);
#else
  grp->SetMaximum(0.80);
#endif
  TF1 fun("fun","sqrt([0]**2+[1]**2*x)");
  fun.SetLineStyle(2);
  fun.SetLineColor(2);
  grp->Fit(&fun,"","",10.,550.);
  Double_t sigma0        = fun.GetParameter(0);             // [1/GeV]
  Double_t dsigma0       = fun.GetParError (0);             // [1/GeV]
  Double_t cdbyrootneff  = fun.GetParameter(1) * sqrt(10.);  // [1/GeV/#sqrt{cm}]
  Double_t dcdbyrootneff = fun.GetParError (1) * sqrt(10.);  // [1/GeV/#sqrt{cm}]

  TPaveText *pt = new TPaveText(330,0.016, 530.,0.018);

  stringstream titlestr;
  titlestr << "Momentum Resolutin (P" << p << "[GeV])" << ends;
  grp->SetTitle(titlestr.str().data());
  pt->SetTextFont(132);
  pt->SetTextAlign(12);
  stringstream stext;
  stext << "#sigma_{0} = " << setw(6) << setprecision(3) << sigma0
        << " #pm " << setw(4) << setprecision(2) << dsigma0
        << " [#mum]" << ends;
  pt->AddText(stext.str().data());
  stext.str("");
  stext.clear();
  stext << "Slope = " << setw(6) << setprecision(3) << cdbyrootneff 
        << " #pm " <<  setw(4) << setprecision(2) << dcdbyrootneff 
        << " [1/GeV/#sqrt{cm}]" << ends;
  pt->AddText(stext.str().data());
  pt->Draw();

  TLatex *fitfun = new TLatex;
  fitfun->SetTextFont(132);
  fitfun->SetTextAlign(12);
  fitfun->DrawLatex(100,0.016, "#sigma_{#kappa} = #sqrt{#sigma_{0}^{2}+slope^{2} z}");

  // save plot as eps file
  stringstream ofile;
  ofile << "MomResol_P" << p << "GeV" << ".eps"<< ends; 
  c1->Print(ofile.str().data());
}

void Process(Int_t run, Double_t &x, Double_t &y, Double_t &dx, Double_t &dy, Double_t p)
{
  // ---------------
  // Reset Run Info.
  // ---------------
  Runinfo &rinfo = *Runinfo::GetInstancePtr();

  stringstream finstr;
  finstr << "10k/p." << p << "_z." << rinfo.GetDlength(run) << "_B." << rinfo.GetBfield(run) << ".root" << ends;
  TFile *hfp = new TFile(finstr.str().data());
  TNtupleD *hTracks = static_cast<TNtupleD *>(gROOT->FindObject("hTracks"));
#ifdef CHECK_DIST
  TDirectory *last = gDirectory;
  stringstream hstr;
  hstr << "c" << "_" << rinfo.GetDlength(run) << ends;
  TCanvas *cp = new TCanvas(hstr.str().data(),"",400,400);
  cp->cd();
#endif
  stringstream cut;
  ///// Cuts ////////////////////////
#if 0
  const Int_t    kNdfCut    =  28;
  const Double_t kChi2Cut   = 4000.;
  const Double_t kCpaMinCut =  -2.0;
  const Double_t kCpaMaxCut =  2.5;
#else
  const Int_t    kNdfCut    =  140;
  const Double_t kChi2Cut   = 40000.;
  const Double_t kCpaMinCut =  0.25;
  const Double_t kCpaMaxCut =  0.4;
#endif
  ///////////////////////////////////
  cut << "ndf>"  << kNdfCut    << "&&"
      << "chi2<" << kChi2Cut   << "&&"
      << "cpa>"  << kCpaMinCut << "&&"
      << "cpa<"  << kCpaMaxCut << ends;

  hTracks->Draw("cpa", cut.str().data());
  htemp->Rebin(2);
  htemp->Fit("gaus","","", -1.2,2.5);
  Double_t mu  = htemp->GetFunction("gaus")->GetParameter(1);
  Double_t sg  = htemp->GetFunction("gaus")->GetParameter(2);
  htemp->Fit("gaus","","", -0.2,mu+0.5*sg);
           mu  = htemp->GetFunction("gaus")->GetParameter(1);
           sg  = htemp->GetFunction("gaus")->GetParameter(2);
  htemp->Fit("gaus","","",mu-5.0*sg,mu+0.5*sg);
           mu  = htemp->GetFunction("gaus")->GetParameter(1);
           sg  = htemp->GetFunction("gaus")->GetParameter(2);
  htemp->Fit("gaus","","",mu-4.0*sg,mu+0.5*sg);
           mu  = htemp->GetFunction("gaus")->GetParameter(1);
           sg  = htemp->GetFunction("gaus")->GetParameter(2);
  htemp->Fit("gaus","","",mu-3.0*sg,mu+0.5*sg);
           mu  = htemp->GetFunction("gaus")->GetParameter(1);
           sg  = htemp->GetFunction("gaus")->GetParameter(2);
  htemp->Fit("gaus","","",mu-3.0*sg,mu+0.6*sg);
           mu  = htemp->GetFunction("gaus")->GetParameter(1);
           sg  = htemp->GetFunction("gaus")->GetParameter(2);
  htemp->Fit("gaus","","",mu-3.0*sg,mu+0.6*sg);
           mu  = htemp->GetFunction("gaus")->GetParameter(1);
           sg  = htemp->GetFunction("gaus")->GetParameter(2);
  htemp->Fit("gaus","","",mu-4.5*sg,mu+0.6*sg);
#if 1
  cerr << "===============" << endl;
  cerr << "  mu = " << mu << endl;
  cerr << "===============" << endl;
#endif

  x  = rinfo.GetDlength(run) * 10;
  y  = htemp->GetFunction("gaus")->GetParameter(2);
  dx = 0;
  dy = htemp->GetFunction("gaus")->GetParError(2);
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
