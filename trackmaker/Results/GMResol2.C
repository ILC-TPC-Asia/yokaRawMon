#define CHECK_DIST
#include <iomanip>
#include "../Runinfo.h+"

void GMResol2(Int_t b = 1, Int_t layer = 48, Double_t p = 5.0)
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
  Double_t xdata[2][kNp], ydata[2][kNp], dxdata[2][kNp], dydata[2][kNp];
  Double_t x, y, dx, dy;
  Int_t n = 0;

  //           dlen [cm]    10    15    20    25    30    35    40    45    50
  Int_t kRun[2][kNp] = {{ 6953, 6957, 6958, 6972, 6973, 6974, 6975, 6976, 6977},  // B=0T
                        { 7049, 7046, 7051, 7053, 7059, 7061, 7065, 7020, 7023}}; // B=1T
#else
#if 0
  const Int_t kNp = 11;
  Double_t xdata[2][kNp], ydata[2][kNp], dxdata[2][kNp], dydata[2][kNp];
  //           dlen [cm]      5     10     15     20     25     30     35     40     45     50     55
  Int_t kRun[2][kNp] = {{  6953,  6953,  6957,  6958,  6972,  6973,  6974,  6975,  6976,  6977,  6977},  // B=0T
                        { 17319, 17367, 17324, 17364, 17327, 17359, 17331, 17356, 17340, 17352, 17344}}; // B=1T
#else
  const Int_t kNp = 12;
  Double_t xdata[2][kNp], ydata[2][kNp], dxdata[2][kNp], dydata[2][kNp];
  //           dlen [cm]      5     10     15     20     25     30     35     40     45     50
  Int_t kRun[2][kNp] = {{  6953,  6953,  6957,  6958,  6972,  6973,  6974,  6975,  6976,  6977, 1111, 1112},  // B=0T
                       // { 17319, 17367, 17324, 17364, 17327, 17359, 17331, 17356, 17340, 17352}}; // B=1T
                        { 19782, 19783, 19784, 19786, 19787, 19788, 19789, 19790, 19791, 19792, 19793, 19794}}; // B=1T
#endif
#endif

#if 0
  const Char_t *2] = { "in", "out" };
#else
  const Char_t *kInOut[2] = { "in", "ot" };
#endif

  // ---------------
  // Loop over runs
  // ---------------
  Double_t x, y, dx, dy;
  Int_t n = 0;
  for (Int_t io=0; io<2; io++) {
    n = 0;
    for (Int_t i=0; i<kNp; i++) {
      Process(kRun[b][i], layer, x, y, dx, dy, kInOut[io], p);
      xdata[io][n] = x; ydata[io][n] = y*y; dxdata[io][n] = dx; dydata[io][n] = 2*y*dy; n++; 
    }
  }

  // ------------------------
  // Fill z v.s. sigma_x plot
  // ------------------------
  Double_t xdat[kNp], ydat[kNp], dxdat[kNp], dydat[kNp];
  for (Int_t i=0; i<kNp; i++) {
    xdat [i] = xdata[0][i];
    dxdat[i] = dxdata[0][i];
    ydat [i] = TMath::Sqrt(ydata[0][i]*ydata[1][i]);
    dydat[i] = (dydata[0][i] + dydata[1][i])/2.;
  }

  TCanvas *c1 = new TCanvas("c1","",800,500);
  c1->cd();

  TGraphErrors *grp = new TGraphErrors(n,xdat,ydat,dxdat,dydat);
  grp->GetHistogram()->GetXaxis()->SetLimits(0.,550.);
  grp->GetHistogram()->GetXaxis()->SetTitle("Drift Length: z [mm]");
  grp->GetHistogram()->GetYaxis()->SetTitle("#sigma_{x}^{2} [mm^2]");
  grp->Draw("ap");
  grp->SetMarkerColor(1);
  grp->SetMarkerStyle(21);
  double ymin = 0.0;
  double ymax = 0.1;
  grp->SetMinimum(ymin);
  grp->SetMaximum(ymax);
  TF1 fun("fun","[0]**2+[1]**2*x");
  fun.SetLineStyle(2);
  fun.SetLineColor(2);
  grp->Fit(&fun,"","",100.,600.);
  Double_t sigma0        = fun.GetParameter(0) * 1.e3;             // [#mum]
  Double_t dsigma0       = fun.GetParError (0) * 1.e3;             // [#mum]
  Double_t cdbyrootneff  = fun.GetParameter(1) * sqrt(10.) * 1e3;  // [#mum/#sqrt{cm}]
  Double_t dcdbyrootneff = fun.GetParError (1) * sqrt(10.) * 1e3;  // [#mum/#sqrt{cm}]

  TPaveText *pt = new TPaveText(330,4*ymax/5, 530.,4.9*ymax/5);

  stringstream titlestr;
  titlestr << "GM Resolutin (Row" << layer << ")" << ends;
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
  stext << "C_{D}/#sqrt{N_{eff}} = " << setw(6) << setprecision(3) << cdbyrootneff 
        << " #pm " <<  setw(4) << setprecision(2) << dcdbyrootneff 
        << " [#mum/#sqrt{cm}]" << ends;
  pt->AddText(stext.str().data());
  pt->Draw();

  TLatex *fitfun = new TLatex;
  fitfun->SetTextFont(132);
  fitfun->SetTextAlign(12);
  fitfun->DrawLatex(100,4*ymax/5, "#sigma_{x}^{2} = #sigma_{0}^{2}+(C_{D}^{2}/N_{eff}) z");

  // save plot as pdf file
  stringstream ofile;
  ofile << "GMResol_Row" << layer << "_B" << b << "T" << ".pdf"<< ends; 
  c1->Print(ofile.str().data());
}

void Process(Int_t run, Int_t layer, Double_t &x, Double_t &y, Double_t &dx, Double_t &dy, const Char_t *inout, Double_t p)
{
  // ---------------
  // Reset Run Info.
  // ---------------
  Runinfo &rinfo = *Runinfo::GetInstancePtr();

#ifdef CHECK_DIST
  stringstream hstr;
  hstr << "c" << "_" << rinfo.GetDlength(run) << "_" << inout << ends;
  TCanvas *cp = new TCanvas(hstr.str().data(),hstr.str().data(),400,400);
  cp->cd();
#endif

  // ---------------
  // Open input file
  // ---------------
  stringstream finstr;
  finstr << "10k/p." << p << "_z." << rinfo.GetDlength(run) << "_B." << rinfo.GetBfield(run) << ".root" << ends;
  cerr << "opening " << finstr.str().data() << endl;
  TFile *hfp = new TFile(finstr.str().data());

  // -----------------
  // Get residual data
  // -----------------
  TNtupleD *hResXin = static_cast<TNtupleD *>(gROOT->FindObject("hResXin"));
  stringstream item;
  item << "dx" << inout << setw(2) << setfill('0') << layer << ends;
  stringstream cut;

  ///// Cuts ////////////////////////
#if 1
  const Double_t kResXmin   = -0.1;
  const Double_t kResXmax   = +0.1;
  const Int_t    kNdfCut    =  28;
  const Double_t kChi2Cut   = 9000.;
  const Double_t kCpaMinCut = -0.1;
  const Double_t kCpaMaxCut =  0.9;
#else
  const Double_t kResXmin   = -0.1;
  const Double_t kResXmax   = +0.1;
  const Int_t    kNdfCut    =  80;
  //const Int_t    kNdfCut    =  155;
  //const Int_t    kNdfCut    =  140;
  //const Int_t    kNdfCut    =  100;
  //const Double_t kChi2Cut   = 40000.;
  const Double_t kChi2Cut   = 40000.;
  //const Double_t kCpaMinCut = -0.2;
  //const Double_t kCpaMaxCut =  0.8;
  const Double_t kCpaMinCut = -0.2;
  const Double_t kCpaMaxCut =  0.5;
  //const Double_t kCpaMinCut =  0.25;
  //const Double_t kCpaMaxCut =  0.4;
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

  // -----------------
  // Fit residuals
  // -----------------
  htemp->Rebin(1);
  htemp->Fit("gaus","","",-0.3,0.3);
  Double_t mu  = htemp->GetFunction("gaus")->GetParameter(1);
  Double_t sg  = htemp->GetFunction("gaus")->GetParameter(2);
  htemp->Fit("gaus","","",mu-2.5*sg,mu+2.5*sg);

  x  = rinfo.GetDlength(run);
  y  = htemp->GetFunction("gaus")->GetParameter(2); // sigma
  dx = 0;
  dy = htemp->GetFunction("gaus")->GetParError(2);  // its error
  x  *= 10.; // [cm] to [mm]
  y  *= 10.; // [cm] to [mm]
  dx *= 10.; // [cm] to [mm]
  dy *= 10.; // [cm] to [mm]
#ifdef CHECK_DIST
  htemp->SetMinimum(-0.1);
  htemp->SetMarkerStyle(4);
  htemp->SetMarkerSize(0.7);
  htemp->SetMarkerColor(2);
  htemp->SetLineColor(2);
  htemp->Draw("pe");
#endif
}
