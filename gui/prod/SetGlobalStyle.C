//
// $Id: SetGlobalStyle.C,v 1.1.1.1 2009/03/08 01:15:50 fujiik Exp $
//

void SetGlobalStyle() {

  // See $ROOTSYS/include/TStyle.h
  // (TStyle *gStyle;)
  // gROOT->SetStyle(const char *stylename="Default");

  gStyle->SetFitFormat("5.4g");
  //gStyle->SetHeaderPS(const char *header);
  //gStyle->SetTitlePS(const char *pstitle);
  //gStyle->SetLineScalePS(Float_t scale=3);
  //gStyle->SetLineStyleString(Int_t i, const char *text);
  //gStyle->SetNdivisions(Int_t n=510, Option_t *axis="X");

  // Axis & Label

  gStyle->SetAxisColor(1, "X");
  gStyle->SetAxisColor(1, "Y");
  gStyle->SetLabelColor(1, "X");
  gStyle->SetLabelColor(1, "Y");
  gStyle->SetLabelFont(22, "X"); // Times-bold-r-normal
  gStyle->SetLabelFont(22, "Y"); // Times-bold-r-normal
  gStyle->SetLabelOffset(0.005, "X");
  gStyle->SetLabelOffset(0.005, "Y");
  gStyle->SetLabelSize(0.04, "X");
  gStyle->SetLabelSize(0.04, "Y");

  //gStyle->SetLegoInnerR(Float_t rad=0.5);
  //gStyle->SetScreenFactor(Float_t factor=1);

  gStyle->SetTickLength(0.03, "X");
  gStyle->SetTickLength(0.03, "Y");
  gStyle->SetTitleOffset(1.0, "X"); // set axis title offset
  gStyle->SetTitleOffset(1.3, "Y");
  gStyle->SetTitleSize(0.060, "X"); // set axis title size
  gStyle->SetTitleSize(0.060, "Y");

  // Option

  gStyle->SetOptDate(0); // date is printed in the bottom/left corner.
  //gStyle->SetOptDate(3); // date is printed in the top/right corner.
  gStyle->SetOptFile(1);
  //gStyle->SetOptFit(0111); // Why this causes segmentation violation ??
  gStyle->SetOptFit(0000);
  // The parameter mode can be = pcev  (default = 0111)
  //    v = 1;  print name/values of parameters
  //    e = 1;  print errors (if e=1, v must be 1)
  //    c = 1;  print Chisquare/Number of degress of freedom
  //    p = 1;  print Probability
  gStyle->SetOptLogx(0);
  gStyle->SetOptLogy(0);
  gStyle->SetOptLogz(0);
  gStyle->SetOptStat(1001100);
  //gStyle->SetOptStat(Option_t *stat);
  gStyle->SetOptTitle(1);

  // Bar

  gStyle->SetBarOffset(0.1);
  gStyle->SetBarWidth(0.9);

  // Date

  gStyle->SetDateX(0.015);
  gStyle->SetDateY(0.015);

  // Error

  // Canvas

  gStyle->SetDrawBorder(1);
  gStyle->SetCanvasColor(30);
  gStyle->SetCanvasBorderSize(2);
  gStyle->SetCanvasBorderMode(1);
  gStyle->SetCanvasDefH(400);
  gStyle->SetCanvasDefW(400);
  //gStyle->SetCanvasDefX(10);
  //gStyle->SetCanvasDefY(10);

  // Pad

  gStyle->SetPadColor(30);
  gStyle->SetPadBorderSize(2);
  gStyle->SetPadBorderMode(1);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadTopMargin(0.15);
  gStyle->SetPadLeftMargin(0.18);
  gStyle->SetPadRightMargin(0.12);
  gStyle->SetPadGridX(kTRUE);
  gStyle->SetPadGridY(kTRUE);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);

  // Function

  gStyle->SetFuncStyle(1);
  gStyle->SetFuncColor(50);
  gStyle->SetFuncWidth(4);

  // Grid

  gStyle->SetGridStyle(3);
  gStyle->SetGridColor(1);
  gStyle->SetGridWidth(1);

  // Frame

  gStyle->SetFrameFillColor(29);
  gStyle->SetFrameLineColor(1);
  gStyle->SetFrameFillStyle(1001);
  gStyle->SetFrameLineStyle(1);
  gStyle->SetFrameLineWidth(1);
  gStyle->SetFrameBorderSize(3);
  gStyle->SetFrameBorderMode(-1);

  // Histogram

  gStyle->SetHistFillColor(5);
  gStyle->SetHistLineColor(1);
  gStyle->SetHistFillStyle(1001);
  gStyle->SetHistLineStyle(1);
  gStyle->SetHistLineWidth(1);

  // PaintTextFormat

  //gStyle->SetPaintTextFormat(const char *format="g");

  // Paper

  //gStyle->SetPaperSize(kA4);
  //gStyle->SetPaperSize(Float_t xsize=20, Float_t ysize=26);

  // Statistics

  gStyle->SetStatColor(19);
  gStyle->SetStatTextColor(1);
  gStyle->SetStatStyle(1001);
  gStyle->SetStatBorderSize(3);
  //gStyle->SetStatFont(62); // Helvetica-bold-r-normal
  //gStyle->SetStatFont(32); // Times-bold-i-normal
  //gStyle->SetStatFont(22); // Times-bold-r-normal
  gStyle->SetStatFont(12); // Times-medium-i-normal
  //gStyle->SetStatFontSize(0);
  gStyle->SetStatFormat("6.4g");
  //gStyle->SetStatX(0.9); // % from left-bottom edge
  //gStyle->SetStatY(0.9); // % from left-bottom edge
  gStyle->SetStatW(0.35); // % from stats edge
  gStyle->SetStatH(0.35);

  // Strip

  gStyle->SetStripDecimals(kTRUE);

  // Time

  //gStyle->SetTimeOffset(Double_t toffset);

  // Title

  gStyle->SetTitleColor(1, "X");
  gStyle->SetTitleColor(1, "Y");
  gStyle->SetTitleTextColor(1);
  gStyle->SetTitleStyle(1001);
  //gStyle->SetTitleFont(62); // Helvetica-bold-r-normal
  //gStyle->SetTitleFont(32); // Times-bold-i-normal
  gStyle->SetTitleFont(22, "X"); // Times-bold-r-normal
  gStyle->SetTitleFont(22, "Y"); // Times-bold-r-normal
  gStyle->SetTitleFont(22, "Z"); // Times-bold-r-normal
  gStyle->SetTitleFont(22, ""); // Times-bold-r-normal
  //gStyle->SetTitleFontSize(0);
  gStyle->SetTitleBorderSize(2);
  //gStyle->SetTitleXOffset(Float_t offset=1) {SetTitleOffset(offset,"X");}
  //gStyle->SetTitleXSize(Float_t size=0.02) {SetTitleSize(size,"X");}
  //gStyle->SetTitleYOffset(Float_t offset=1) {SetTitleOffset(offset,"Y");}
  //gStyle->SetTitleYSize(Float_t size=0.02) {SetTitleSize(size,"Y");}
  //gStyle->SetTitleX(0); // % from left-bottom edge
  gStyle->SetTitleX(0.02);
  //gStyle->SetTitleY(0.985); // % from left-bottom edge
  gStyle->SetTitleY(0.98);
  //gStyle->SetTitleW(0); // % from title edge
  //gStyle->SetTitleH(0); // % from title edge

  //ToggleEventStatus();
  //SetPalette(Int_t ncolors=0, Int_t *colors=0);
}
