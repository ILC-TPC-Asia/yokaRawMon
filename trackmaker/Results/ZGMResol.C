#define CHECK_DIST
#include <iomanip>
#include "../Runinfo.h+"

void ZGMResol(Int_t b = 1, Int_t layer = 48, Double_t p = 5.0)
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
  //const Int_t kNp = 10;
  const Int_t kNp = 12;
  Double_t xdata[2][kNp], ydata[2][kNp], dxdata[2][kNp], dydata[2][kNp];
  //           dlen [cm]      5     10     15     20     25     30     35     40     45     50
  //Int_t kRun[2][kNp] = {{  6953,  6953,  6957,  6958,  6972,  6973,  6974,  6975,  6976,  6977},  // B=0T
  Int_t kRun[2][kNp] = {{  19616,  19611,  6957,  6958,  6972,  6973,  6974,  6975,  6976,  6977, 1111, 1112},  // B=0T
//                        { 17319, 17367, 17324, 17364, 17327, 17359, 17331, 17356, 17340, 17352}}; // B=1T
//                        { 18241, 18238, 18218, 18221, 18224, 18226, 18228, 18230, 18233, 18243}}; // B=1T
//                        { 18412, 18413, 18414, 18415, 18417, 18418, 18419, 18420, 18421, 18423, 18424}}; // B=1T
//                        { 18564, 18563, 18562, 18561, 18552, 18551, 18550, 18549, 18548, 18547}}; // B=1T
                       // { 18609, 18606, 18605, 18601, 18600, 18598, 18597, 18595, 18594, 18592, 18591}}; // B=1T
                        //{ 19648, 19647, 19646, 19645, 19644, 19638, 19639, 19640, 19654, 19653, 19655, 19656, 19657}}; // B=1T
                        { 19678, 19677, 19676, 19675, 19674, 19673, 19672, 19671, 19670, 19669, 19668, 19667}}; // B=1T
#endif
#endif

#if 0
  const Char_t *kInOut[2] = { "in", "out" };
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
      xdata[io][n] = x; ydata[io][n] = y; dxdata[io][n] = dx; dydata[io][n] = dy; n++; 
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
  grp->GetHistogram()->GetYaxis()->SetTitle("#sigma_{z} [mm]");
  grp->Draw("ap");
  grp->SetMarkerColor(1);
  grp->SetMarkerStyle(21);
  double ymin = 0.0;
  //double ymax = 0.8;
  double ymax = 1.0;
  grp->SetMinimum(ymin);
  grp->SetMaximum(ymax);
  TF1 fun("fun","sqrt([0]**2+[1]**2*x)");
  fun.SetLineStyle(2);
  fun.SetLineColor(2);
  //grp->Fit(&fun,"","",100.,600.);
  grp->Fit(&fun,"","",150.,600.);
  Double_t sigma0        = fun.GetParameter(0) * 1.e3;             // [#mum]
  Double_t dsigma0       = fun.GetParError (0) * 1.e3;             // [#mum]
  Double_t cdbyrootneff  = fun.GetParameter(1) * sqrt(10.) * 1e3;  // [#mum/#sqrt{cm}]
  Double_t dcdbyrootneff = fun.GetParError (1) * sqrt(10.) * 1e3;  // [#mum/#sqrt{cm}]
  Double_t chi2          = fun.GetChisquare();
  Int_t    ndf           = fun.GetNDF();

  TPaveText *pt = new TPaveText(330,4*ymax/5, 530.,4.9*ymax/5);

  stringstream titlestr;
  titlestr << "GM Resolutin (Row" << layer << ")" << ends;
  grp->SetTitle(titlestr.str().data());
  pt->SetFillColor(0);
  pt->SetTextFont(132);
  pt->SetTextAlign(12);
  stringstream stext;
  stext << "#chi^{2}/ndf = " << setprecision(3) << chi2 << "/" << ndf << ends;
  pt->AddText(stext.str().data());
  stext.str("");
  stext.clear();
  pt->Draw();
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
  fitfun->DrawLatex(100,4*ymax/5, "#sigma_{z} = #sqrt{#sigma_{0}^{2}+(C_{D}^{2}/N_{eff}) z}");

#if 0
  // save plot as eps file
  stringstream ofile;
  ofile << "GMResol_Row" << layer << "_B" << b << "T" << ".eps"<< ends; 
  c1->Print(ofile.str().data());
#endif
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
  TNtupleD *hResZin = static_cast<TNtupleD *>(gROOT->FindObject("hResZin"));
  stringstream item;
  item << "dz" << inout << setw(2) << setfill('0') << layer << ends;
  stringstream cut;

  ///// Cuts ////////////////////////
#if 0
#if 0
  const Double_t kResZmin   = -0.1;
  const Double_t kResZmax   = +0.1;
  const Int_t    kNdfCut    =  28;
  const Double_t kChi2Cut   = 9000.;
  const Double_t kCpaMinCut = -0.8;
  const Double_t kCpaMaxCut =  0.4;
  const Double_t kPhi0MinCut = 6.22;
  const Double_t kPhi0MaxCut = 6.28;
  const Double_t kTanlMinCut = -0.02;
  const Double_t kTanlMaxCut = +0.02;
#else
  const Double_t kResZmin   = -0.10;
  const Double_t kResZmax   = +0.10;
  //const Double_t kResZmin   = -0.15;
  //const Double_t kResZmax   = +0.15;
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
  const Int_t    kNdfCut    =  155;
  const Double_t kChi2Cut   = 300.;
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
  const Double_t kPhi0MinCut = 6.218;
  const Double_t kPhi0MaxCut = 6.314;
  const Double_t kTanlMinCut = -0.02;
  const Double_t kTanlMaxCut = +0.02;
  const Double_t kPkPhMinCut =  10.;
  const Double_t kPkPhMaxCut =  900.;
#endif
#else
  const Double_t kResZmin   = -0.50;
  const Double_t kResZmax   = +0.50;
#if 0
  const Int_t    kNdfCut    = 100; //117;
  const Double_t kChi2Cut   = 5000.;
  //const Double_t kCpaMinCut = -0.2;
  //const Double_t kCpaMaxCut =  1.2;
  //const Double_t kCpaMinCut =  -1.5; //-0.2;
  //const Double_t kCpaMaxCut =   2.5; // 0.8;
  const Double_t kCpaMinCut =  -0.2;
  const Double_t kCpaMaxCut =   0.8;
  const Double_t kPhi0MinCut =  6.20; // 6.24; //+6.218;
  const Double_t kPhi0MaxCut =  6.31; // 6.27; // 6.314;
  const Double_t kTanlMinCut = -0.02;
  const Double_t kTanlMaxCut = +0.02;
  const Double_t kPkPhMinCut =  10.;
  const Double_t kPkPhMaxCut =  900.;
#else
  const Int_t    kNdfCut    = 80; //117;
  //const Int_t    kNdfCut    = 34; //117;
  const Double_t kChi2Cut   = 40000.;
  //const Double_t kCpaMinCut = -0.2;
  //const Double_t kCpaMaxCut =  1.2;
  //const Double_t kCpaMinCut =  -1.5; //-0.2;
  //const Double_t kCpaMaxCut =   2.5; // 0.8;
  const Double_t kCpaMinCut =  -2.;
  const Double_t kCpaMaxCut =   8.;
  const Double_t kPhi0MinCut = -99999; // 6.24; //+6.218;
  const Double_t kPhi0MaxCut = +99999; // 6.27; // 6.314;
  const Double_t kTanlMinCut = -99999;
  const Double_t kTanlMaxCut = +99999;
  const Double_t kPkPhMinCut =  10.;
  const Double_t kPkPhMaxCut =  900.;
#endif	
#endif
  ///////////////////////////////////
  cut << item.str().data() << ">" << kResZmin << "&&"
      << item.str().data() << "<" << kResZmax << "&&"
      << "ndf>"  << kNdfCut    << "&&"
      << "chi2<" << kChi2Cut   << "&&"
      << "tnl>"  << kTanlMinCut << "&&"
      << "tnl<"  << kTanlMaxCut << "&&"
      << "pk" << setw(2) << setfill('0') << layer << ">" << kPkPhMinCut << "&&"
      << "pk" << setw(2) << setfill('0') << layer << "<" << kPkPhMaxCut;
  if (rinfo.GetBfield(run)) {
    cut << "&&"
      //  << "xbar"  << layer << ">2442&&"
      //  << "xbar"  << layer << "<2450&&"
      //  << "xbar"  << layer << ">2440&&"
      //  << "xbar"  << layer << "<2470&&"
        << "cpa>"  << kCpaMinCut << "&&"
        << "cpa<"  << kCpaMaxCut;
  }
  cut << ends;
  stringstream target;
  //target << item.str().data() << ">>htemp(100," << kResZmin << ", " << kResZmax << ")" << ends;
  target << item.str().data() << ">>htemp(50," << kResZmin << ", " << kResZmax << ")" << ends;
  hResZin->Draw(target.str().data(), cut.str().data());

  // -----------------
  // Fit residuals
  // -----------------
  htemp->Rebin(1);
  htemp->Fit("gaus","RI","",-0.3,0.3);
  Double_t mu  = htemp->GetFunction("gaus")->GetParameter(1);
  Double_t sg  = htemp->GetFunction("gaus")->GetParameter(2);
  htemp->Fit("gaus","RI","",mu-2.5*sg,mu+2.5*sg);

  x  = rinfo.GetDlength(run) + rinfo.GetDLOffset(run);
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
