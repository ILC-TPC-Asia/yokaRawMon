void phi0()
{
  double phiwidth    = 8.386994; // degrees
  //double phioffset[] = { 3.0, -5.39, 8.5, 0.11, -8.27, 5.0, -3.39 };
  double phioffset[] = { 3.0, -5.386994, 8.496994, 0.11, -8.276994, 5.0, -3.386994 };
  double deg2rad     = TMath::Pi()/180.;
  cerr << "Double_t JPadGeom::fgPhi0  [] = {"
       << setw(14)
       << setprecision(14)
       << (phioffset[0]+phiwidth/2)*deg2rad << ", " 
       << (phioffset[1]+phiwidth/2)*deg2rad << "," << endl
       << "                                 "
       << setw(14)
       << setprecision(14)
       << (phioffset[2]+phiwidth/2)*deg2rad << ", " 
       << (phioffset[3]+phiwidth/2)*deg2rad << ", " 
       << (phioffset[4]+phiwidth/2)*deg2rad << "," << endl
       << "                                 "
       << setw(14)
       << setprecision(14)
       << (phioffset[5]+phiwidth/2)*deg2rad << ", " 
       << (phioffset[6]+phiwidth/2)*deg2rad << "};" << endl;
  // Survey data
  // lup           rup
  //   3 4          3 4
  //  1 2          1 2
  //
  //   ldown     rdown
  //    3 4       3 4
  //   1 2       1 2
  //
  double xylup[4][4][2]={{{-115.301,152.946}, // pad 1
                          {-114.269,153.006},
                          {-114.935,147.780},
                          {-113.898,147.842}},
                         {{-114.114,153.018}, // pad 2
                          {-113.075,153.115},
                          {-113.742,147.857},
                          {-112.708,147.948}},
                         {{-115.117,158.334}, // pad 3
			  {-114.056,158.399},
                          {-114.719,153.150},
                          {-113.677,153.243}},
			 {{-113.895,158.408}, // pad 4
                          {-112.859,158.509},
                          {-113.529,153.251},
                          {-112.490,153.314}}};
  // 
  double xyrup[4][4][2]={{{ 112.529,153.137}, // pad 1
                          { 113.568,153.070},
                          { 112.154,147.972},
                          { 113.196,147.913}},
                         {{ 113.721,153.069}, // pad 2
                          { 114.763,153.009},
                          { 113.351,147.896},
                          { 114.391,147.836}},
                         {{ 113.508,158.466}, // pad 3
			  { 114.549,158.410},
                          { 113.135,153.272},
                          { 114.178,153.207}},
			 {{ 114.703,158.374}, // pad 4
                          { 115.769,158.296},
                          { 114.331,153.195},
                          { 115.367,153.104}}};
  //
  double xyldn[4][4][2]={{{-105.110, 14.029}, // pad 1
                          {-104.077, 14.091},
                          {-104.756,  8.861},
                          {-103.707,  8.923}},
                         {{-103.908, 14.104}, // pad 2
                          {-102.909, 14.196},
                          {-103.552,  8.930},
                          {-102.543,  9.031}},
                         {{-104.913, 19.413}, // pad 3
                          {-103.874, 19.483},
                          {-104.540, 14.246},
                          {-103.503, 14.311}},
			 {{-103.719, 19.493}, // pad 4
                          {-102.679, 19.581},
                          {-103.350, 14.320},
                          {-102.311, 14.410}}};
  //
  double xyrdn[4][4][2]={{{ 102.360, 14.245}, // pad 1
                          { 103.373, 14.184},
                          { 101.986,  9.059},
                          { 103.008,  9.000}},
                         {{ 103.556, 14.168}, // pad 2
                          { 104.568, 14.108},
                          { 103.149,  8.988},
                          { 104.205,  8.922}},
                         {{ 103.325, 19.522}, // pad 3
                          { 104.370, 19.491},
                          { 102.955, 14.375},
                          { 103.995, 14.312}},
			 {{ 104.520, 19.479}, // pad 4
                          { 105.561, 19.386},
                          { 104.150, 14.302},
                          { 105.192, 14.205}}};
  //
  double rframe = 9.5;
  int    nrows  = 28;
#if 0
  double dgap   = 0.1;
  double dxrud42 = xyrup[3][1][0] - xyrdn[1][3][0];
  double dyrud42 = xyrup[3][1][1] - xyrdn[1][3][1];
  double dxyrud42 = sqrt(dxrud42*dxrud42+dyrud42*dyrud42);
  cerr << "dxyrud42=" << dxyrud42 << endl;
  double deltaR = dxyrud42+rframe*2;
  double dr = (dxyrud42+dgap)/nrows;
#else
  double dxruu21 = (xyrup[3][3][0] + xyrup[2][2][0])/2
	         - (xyrup[1][1][0] + xyrup[1][0][0])/2;
  double dyruu21 = (xyrup[3][3][1] + xyrup[2][2][1])/2
	         - (xyrup[1][1][1] + xyrup[1][0][1])/2;
  double dgap    = sqrt(dxruu21*dxruu21+dyruu21*dyruu21);

  double dxrud44 = xyrup[3][1][0] - xyrdn[3][1][0];
  double dyrud44 = xyrup[3][1][1] - xyrdn[3][1][1];
  double dxyrud44 = sqrt(dxrud44*dxrud44+dyrud44*dyrud44);
  cerr << "dxyrud44=" << dxyrud44 << endl;
  double dr = dxyrud44/(nrows-2);
  double deltaR = dxyrud44+2*dr-dgap+rframe*2;
#endif
  cerr << "deltaR=" << deltaR << endl;
  cerr << "dr=" << dr << endl << endl;

  double rin     = 1430. + rframe;
  double rout    = rin + deltaR - 2*rframe;
  cerr << "rin =" << rin << endl;
  cerr << "rout=" << rout << endl;
  int    npadsup = 192;
  int    npadsdn = 176;

  double dxurl43 = xyrup[3][1][0] - xylup[2][1][0];
  double dyurl43 = xyrup[3][1][1] - xylup[2][1][1];
  double dxyurl43 = sqrt(dxurl43*dxurl43+dyurl43*dyurl43);
  cerr << "dxyurl43=" << dxyurl43 << endl;
  double dphiup = TMath::ATan(dxyurl43/2/rout)*2/(npadsup-1);
  cerr << "dphiup=" << dphiup << endl << endl;

  double dxdrl21 = xyrdn[1][3][0] - xyldn[0][3][0];
  double dydrl21 = xyrdn[1][3][1] - xyldn[0][3][1];
  double dxydrl21 = sqrt(dxdrl21*dxdrl21+dydrl21*dydrl21);
  cerr << "dxydrl21=" << dxydrl21 << endl;
  double dphidn = TMath::ATan(dxydrl21/2/rin)*2/(npadsdn-1);
  cerr << "dphidn=" << dphidn << endl;
}
