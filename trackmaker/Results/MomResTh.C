// Theoretical cureve for momentum resolution.
//
TGraph *MomResTh()
{
   const int kNp = 101;
   double x[kNp];
   double y[kNp];

   double dlmin = 0.;     // [cm]
   double dlmax = 60.;    // [cm]
   double ddl   = (dlmax-dlmin)/(kNp-1);

   double Cd    = 94.e-6; // [m/sqrt(cm)]
   double sgx0  = 70.e-6; // [m]
   double neff  = 29.;    // n_effective
   double B     = 1.;     // B-field [T]
   double nrow  = 28;     // #pad raws / module
   double nmod  = 3;      // #modules
   double gap   = 10.e-3; // frame width [cm]
   double padl  = 5.36e-3;  // pad length [m]
   double n     = nrow*nmod; // #sample points
   double L     = (padl*nrow)*nmod+gap*(2*nmod-2); // lever arm length [m]
   double X0    = 110.;   // radiation length of Ar [m]
   double p     = 5.;     // momentum [GeV]

   int i = 0;
   for (int i=0; i<kNp; i++) {
      double dl  = dlmin + i*ddl;
      double sgx = sqrt(sgx0*sgx0 + (Cd*Cd/neff)*dl); // [m]
      double sgkm2  = pow(sgx/(0.3*B*L*L),2)*(720./(n+4));
#if 1 // more accurate formula
      double sgkms2 = pow(0.05/(B*L*p),2)*((10./7)*(n*(n*n*n+112*n*n+135*n-362)/((n-1)*(n+1)*(n+2)*(n+3)))*(L/X0));
#else
      double sgkms2 = pow(0.05/(B*L*p),2)*((10./7)*(L/X0));
#endif
      double sgk    = sqrt(sgkm2+sgkms2);
#if 0
      cerr << "dl=" << dl << "sgx=" << sgx << endl;
#endif
      x[i] = dl*1.e1; // [mm]
      y[i] = sgk;
   }
   TGraph *gp = new TGraph(i, x, y);
   //gp->Draw("apl");
   gp->SetMinimum(0.);
   //gp->SetMaximum(0.8);
#if 0
   TF1 *fp = new TF1("fp","pol1",120.,250.);
   gp->Fit(fp, "R");
#endif
   return gp;
}
