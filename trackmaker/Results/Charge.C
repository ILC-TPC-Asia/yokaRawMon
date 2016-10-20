#define CHECK_DIST
//#define TIME_DEP
#include <iomanip>
#include "../Runinfo.h+"

void Process(Int_t run, Int_t layer, Double_t &x, Double_t &y, Double_t &dx, Double_t &dy, Double_t p);

void Charge(Int_t b = 1, Int_t layer = 44, Double_t p = 5.0)
//void Charge(Int_t b = 1, Int_t layer = 48, Double_t p = 2.0)
{
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
  Double_t xdata[kNp], ydata[kNp], dxdata[kNp], dydata[kNp], dlen[kNp];
#ifndef TIME_DEP
  //           dlen [cm]      5     10     15     20     25     30     35     40     45     50
  Int_t kRun[2][kNp] = {{  6953,  6953,  6957,  6958,  6972,  6973,  6974,  6975,  6976,  6977},  // B=0T
//                        { 17319, 17367, 17324, 17364, 17327, 17359, 17331, 17356, 17340, 17352}}; // B=1T
//                        { 18241, 18238, 18218, 18221, 18224, 18226, 18228, 18230, 18233, 18243}}; // B=1T
//                        { 18412, 18413, 18414, 18415, 18417, 18418, 18419, 18420, 18421, 18423, 18424}}; // B=1T
                        { 18564, 18563, 18562, 18561, 18552, 18551, 18550, 18549, 18548, 18547}}; // B=1T
//                        { 18609, 18606, 18605, 18601, 18600, 18598, 18597, 18595, 18594, 18592, 18591}}; // B=1T
#else
  Int_t kRun[2][kNp] = {{  6953,  6953,  6957,  6958,  6972,  6973,  6974,  6975,  6976,  6977},  // B=0T
//                        { 17319, 17324, 17327, 17331, 17340, 17352, 17356, 17359, 17364, 17367}}; // B=1T
//                        { 18241, 18238, 18218, 18221, 18224, 18226, 18228, 18230, 18233, 18243}}; // B=1T
//                        { 18241, 18238, 18218, 18221, 18224, 18226, 18228, 18230, 18233, 18243}}; // B=1T
//                        { 18412, 18413, 18414, 18415, 18417, 18418, 18419, 18420, 18421, 18423, 18424}}; // B=1T
                        { 18564, 18563, 18562, 18561, 18552, 18551, 18550, 18549, 18548, 18547}}; // B=1T
//                        { 18609, 18606, 18605, 18601, 18600, 18598, 18597, 18595, 18594, 18592, 18591}}; // B=1T
#endif
#endif
#endif
  // ---------------
  // Reset Run Info.
  // ---------------
  Runinfo &rinfo = *Runinfo::GetInstancePtr();

  Double_t x, y, dx, dy;
  Int_t n = 0;
  for (Int_t i=0; i<kNp; i++) {
      Process(kRun[b][i], layer, x, y, dx, dy, p);
#ifndef TIME_DEP
      xdata[n] = x; ydata[n] = y; dxdata[n] = dx; dydata[n] = dy; n++; 
#else
      xdata[n] = i; ydata[n] = y; dxdata[n] = dx; dydata[n] = dy;
      dlen[n] = rinfo.GetDlength(kRun[b][i]);
      n++; 
#endif
  }

  TCanvas *c1 = new TCanvas("c1","",600,500);
  c1->cd();

  // ---------
  // Graph
  // ---------
  TGraphErrors *grp = new TGraphErrors(n,xdata,ydata,dxdata,dydata);
#ifndef TIME_DEP
  Double_t xmin =   0.;
  Double_t xmax = 550.;
  grp->GetHistogram()->GetXaxis()->SetTitle("Drift Length [mm]");
#else
  Double_t xmin = -1;
  Double_t xmax = 10;
  grp->GetHistogram()->GetXaxis()->SetTitle("Sequence");
#endif
  grp->GetHistogram()->GetXaxis()->SetLimits(xmin,xmax);
  grp->GetHistogram()->GetYaxis()->SetTitle("Charge: MP");
  grp->GetHistogram()->GetYaxis()->SetTitleOffset(1.3);
  grp->Draw("ap");
  grp->SetMarkerColor(1);
  grp->SetMarkerStyle(21);
  Double_t ymin = 0.;
  Double_t ymax = 2000.;
  grp->SetMinimum(ymin);
  grp->SetMaximum(ymax);
  stringstream titlestr;
  titlestr << "Charge_Row" << layer << ends;
  grp->SetTitle(titlestr.str().data());

  // ---------
  // Fit
  // ---------
  TF1 fun("fun","[0]+[1]*x");
  fun.SetLineStyle(2);
  fun.SetLineColor(2);
  grp->Fit(&fun,"","",xmin,xmax);
  Double_t y0    = fun.GetParameter(0);
  Double_t dy0   = fun.GetParError (0);
  Double_t dydx  = fun.GetParameter(1);
  Double_t ddydx = fun.GetParError (1);
  Double_t chi2  = fun.GetChisquare();
  Int_t    ndf   = fun.GetNDF();

  TPaveText *pt = new TPaveText(xmin+6*(xmax-xmin)/10,ymin+4*(ymax-ymin)/5, 
		                xmin+9.8*(xmax-xmin)/10,ymin+4.9*(ymax-ymin)/5);
  pt->SetTextFont(132);
  pt->SetTextAlign(12);
  stringstream stext;
  stext << "#chi^{2}/ndf = " << setprecision(3) << chi2 << "/" << ndf << ends;
  pt->AddText(stext.str().data());
  stext.str("");
  stext.clear();
  pt->Draw();
  stext << "y0      = " << setw(6) << setprecision(3) << y0
        << " #pm "      << setw(4) << setprecision(2) << dy0
	<< ends;
  pt->AddText(stext.str().data());
  stext.str("");
  stext.clear();
  stext << "dy/dx = " << setw(6) << setprecision(3) << dydx
        << " #pm "    << setw(4) << setprecision(2) << ddydx
        << ends;
  pt->AddText(stext.str().data());

  // ---------
  // Info.
  // ---------
  for (Int_t i=0; i<kNp; i++) {
      stext.str("");
      stext.clear();
#ifndef TIME_DEP 
      stext << "r" << kRun[b][i] << ends;
#else
      stext << "z=" << dlen[i] << "cm" << ends;
#endif
      TLatex *runno = new TLatex;
      runno->SetTextFont(132);
      runno->SetTextAlign(12);
      Double_t yoff = (2*(i%2)-1)*(ymax-ymin)/7;
      runno->DrawLatex(xdata[i],ydata[i]+yoff,stext.str().data());
      Double_t xoff = (xmax-xmin)/kNp/10;
      TLine *line = new TLine(xdata[i]+xoff,ydata[i]+yoff,xdata[i],ydata[i]);
      line->SetLineStyle(3);
      line->Draw();
  }
}

static int lastrun = -1;

void Process(Int_t run, Int_t layer, Double_t &x, Double_t &y, Double_t &dx, Double_t &dy, Double_t p)
{
  // ---------------
  // Reset Run Info.
  // ---------------
  Runinfo &rinfo = *Runinfo::GetInstancePtr();

  if (run != lastrun) {
    stringstream finstr;
    finstr << "10k/p." << p << "_z." << rinfo.GetDlength(run) << "_B." << rinfo.GetBfield(run) << ".root" << ends;
    TFile *hfp = new TFile(finstr.str().data());
  }
  TNtupleD *hResXin = static_cast<TNtupleD *>(gROOT->FindObject("hResXin"));
#ifdef CHECK_DIST
  stringstream hstr;
  hstr << "c" << "_" << rinfo.GetDlength(run) << ends;
  TCanvas *cp = new TCanvas(hstr.str().data(),"",400,400);
  cp->cd();
#endif
  stringstream item;
  item << "ch" << setw(2) << setfill('0') << layer << ends;

  stringstream dxstr;
  dxstr << "dxin" << setw(2) << setfill('0') << layer << ends;

  ///// Cuts ////////////////////////
#if 1
  const Double_t kResXmin   = -0.3;
  const Double_t kResXmax   = +0.3;
  const Int_t    kNdfCut    =  40;
  const Double_t kChi2Cut   = 10000.;
  const Double_t kCpaMinCut = -0.8;
  const Double_t kCpaMaxCut =  1.0;
  const Double_t kQMaxCut   =  7000.;
  const Double_t kPkPhMinCut =  10.;
  const Double_t kPkPhMaxCut =  900.;
#else
  const Double_t kResXmin   = -0.3;
  const Double_t kResXmax   = +0.3;
  const Int_t    kNdfCut    =  100;
  //const Int_t    kNdfCut    =  32;
  //const Int_t    kNdfCut    =  24;
  const Double_t kChi2Cut   = 4000.;
  const Double_t kCpaMinCut = -0.2;
  const Double_t kCpaMaxCut =  0.8;
  const Double_t kQMaxCut   =  3000.;
  //const Double_t kQMaxCut   =  700.;
  const Double_t kPkPhMinCut =  10.;
  const Double_t kPkPhMaxCut =  900.;
#endif
  ///////////////////////////////////
  stringstream cut;
  cut << dxstr.str().data() << ">" << kResXmin << "&&"
      << dxstr.str().data() << "<" << kResXmax << "&&"
      << "ndf>"  << kNdfCut    << "&&"
      << "chi2<" << kChi2Cut   << "&&"
      << item.str().data() << "<" << kQMaxCut << "&&"
      << "pk" << layer << ">" << kPkPhMinCut << "&&"
      << "pk" << layer << "<" << kPkPhMaxCut;
  if (rinfo.GetBfield(run)) {
    cut << "&&"
        << "cpa>"  << kCpaMinCut << "&&"
        << "cpa<"  << kCpaMaxCut;
  }
  cut << ends;

  hResXin->Draw(item.str().data(), cut.str().data());
  cerr << "hResXin = " << hResXin << endl;
  TH1* htemp = static_cast<TH1 *>(gROOT->FindObject("htemp"));
  cerr << "htemp= " << htemp << endl;
  htemp->Rebin(1);
  htemp->Fit("landau","","",0.0,10000.);
  htemp->Fit("landau","","",0.0,10000.);
  Double_t mu  = htemp->GetFunction("landau")->GetParameter(1);
  Double_t sg  = htemp->GetFunction("landau")->GetParameter(2);

  x  = rinfo.GetDlength(run);
  y  = htemp->GetFunction("landau")->GetParameter(1);
  dx = 0;
  dy = htemp->GetFunction("landau")->GetParError(1);
  x  *= 10.;
  dx *= 10.;
#ifdef CHECK_DIST
  htemp->SetMinimum(0.);
  htemp->SetMarkerStyle(4);
  htemp->SetMarkerSize(0.7);
  htemp->SetMarkerColor(2);
  htemp->SetLineColor(2);
  htemp->Draw("pe");
#endif
}
