#define AVOID_EDGE
#include "JSimpleTrackFitter.h"
#include "JTrack.h"
#include "JHit.h"
#if 1
#include "JHitCorrector.h"
#endif
#include "JPadGeom.h"

#include "EXTPCKalDetector.h"
#include "EXTPCMeasLayer.h"
#include "EXTPCHit.h"
#include "EXHYBTrack.h"

#include "TKalTrackSite.h"
#include "TKalTrackState.h"
#include "THelicalTrack.h"

#include "TVector3.h"
#include "TMath.h"
#include "TNtupleD.h"
#include "TH2.h"
#include "TH1.h"

#include <sstream>
#include <iomanip>


static const Bool_t gkDir = kIterBackward;
//static const Bool_t gkDir = kIterForward;

Bool_t JSimpleTrackFitter::fgIsOut         = kFALSE;
#if 1
// use in estimation of tracking efficiency
Bool_t JSimpleTrackFitter::fgIsUpperHit    = kFALSE;
Bool_t JSimpleTrackFitter::fgIsLowerHit    = kFALSE;
Bool_t JSimpleTrackFitter::fgIsMidHit      = kFALSE;
#endif
Int_t  JSimpleTrackFitter::fgLayerToLookAt = 14;

ClassImp(JSimpleTrackFitter)

JSimpleTrackFitter::JSimpleTrackFitter(TVKalDetector *kaldp)
: JVTrackFitter(kaldp)
{
}

JSimpleTrackFitter::~JSimpleTrackFitter()
{
}

void JSimpleTrackFitter::Fit(JTrack   *trkp,
  JHelix   &helix,
  Double_t &chi2,
  Int_t    &ndf)
  {

    cerr << "JSimpleTrackFitter CALLED!!!!!!!!!!!!!!!!!" << endl;


    const Double_t kmm2cm = 0.1;
    //--
    // First prepare kalhits
    //--
    TObjArray kalhits;
    kalhits.SetOwner();
    TIter next(trkp);
    JHit *hp;
    while ((hp = static_cast<JHit *>(next()))) {
      TVector3 xv     = kmm2cm * hp->GetPosition();
      Int_t    layer  = hp->GetLayerID ();
      Int_t    module = hp->GetModuleID();
      EXTPCKalDetector *tpckaldp
      = dynamic_cast<EXTPCKalDetector *>(GetKalDetector());
      if (!tpckaldp) {
        cerr << "::::: ERROR in JSimpleTrackFitter ::::::" << endl
        << " Failed to cast KalDet to TPCKalDet!"     << endl
        << " Abort!"                                  << endl;
        ::abort();
      }
      ////// VERY TEMPORARY ////////////////////////////////////////
      Int_t superlayer = (module < 2 ? 0 : (module < 5 ? 1 : 2));
      Int_t layerser   = superlayer * JPadGeom::GetNrow() + layer;
      ////// VERY TEMPORARY ////////////////////////////////////////
      EXTPCMeasLayer *measlayerp
      = static_cast<EXTPCMeasLayer *>(tpckaldp->At(layerser));
      if (measlayerp->GetModuleID() != module || measlayerp->GetLayerID() != layer) {
        cerr << "::::: ERROR in JSimpleTrackFitter :::::::::::::::::::::::::" << endl
        << " meas layer ID inconsistent for ml = " << layerser           << endl
        << " ht: (module,layer) = (" << module << "," << layer    << ")" << endl
        << " ml: (module,layer) = (" << measlayerp->GetModuleID() << ","
        <<                              measlayerp->GetLayerID () << ")" << endl
        << " Abort!"                                                     << endl;
        ::abort();
      }

      Double_t meas [2];
      Double_t dmeas[2];
      Int_t    side = +1;
      TKalMatrix mv = measlayerp->XvToMv(xv, side);
      Double_t d = mv(1,0);
      meas [0] = mv(0,0);
      #if 1
      Double_t deltax = JHitCorrector::GetInstancePtr()->GetDeltaX(meas[0],layer,module,0);
      meas [0] += deltax;
      //cerr << " module = " << module << " layer = " << layer << " deltax = " << deltax << endl;
      #else
      // No distortion correction
      #endif
      meas [1] = d;
      #if 0
      cerr << "xv = (" << xv.X() << ", " << xv.Y() << ", " << xv.Z() <<  ")" << endl;
      #endif
      dmeas[0] = measlayerp->GetSigmaX(d);
      dmeas[1] = measlayerp->GetSigmaZ(d);
      //--
      // Exclude layer to look at from the fit if requested
      //--
      if (fgIsOut && layer == fgLayerToLookAt) {
        dmeas[0] *= 100.;
        dmeas[1] *= 100.;
      }
      #ifdef AVOID_EDGE
      //--
      // Exclude Edge rows
      //--
      ////////////// VERY TEMPORARY ///////////
      //static const Int_t kEdge           = 6;
      //static const Int_t kEdge           = 1;
      static const Int_t kEdge           = 2;
      //static const Int_t kModuleToLookAt = 3;
      /////////////////////////////////////////
      #if 0
      if ((layer < kEdge) || (layer >= (JPadGeom::GetNrow() - kEdge)) || module != kModuleToLookAt) {
      #else
      #if 1
      if ((layer < kEdge) || (layer >= (JPadGeom::GetNrow() - kEdge))) {
        #else
        if ((layer < kEdge) || (layer >= (JPadGeom::GetNrow() - kEdge)) ||
        (layer == 7) || (layer == 13)  || (layer == 14) || (layer == 20)) {
          #endif
          #endif
          dmeas[0] *= 1000.;
          dmeas[1] *= 1000.;
        }
        #endif
        Double_t b    = EXTPCKalDetector::GetBfield();
        Double_t v    = EXTPCKalDetector::GetVdrift();
        kalhits.Add(new EXTPCHit(*measlayerp, meas, dmeas, side, v, hp, b));
      }
      //--
      // Set start helix parameters.
      //--
      if (kalhits.GetEntries() < 5) {
        cerr << "<<<<<< Shortage of Hits! nhits = "
        << kalhits.GetEntries() << " >>>>>>>" << endl;
        return;
      }

      Int_t i1, i2, i3; // (i1,i2,i3) = (1st,mid,last) hit to filter
      if (gkDir == kIterBackward) {
        #if 1
        #if 1
        i3 = 1;                        // avoid edge
        i1 = kalhits.GetEntries() - 2; // avoid edge
        #else
        i3 = 0;                        // avoid edge
        i1 = kalhits.GetEntries() - 1; // avoid edge
        #endif
        i2 = i1 / 2;
        #else
        #if 0
        i3 = 3;                        // avoid edge
        i1 = kalhits.GetEntries() - 4; // avoid edge
        i2 = i1 / 2;
        #else
        i3 = 6;                        // avoid edge
        i1 = kalhits.GetEntries() - 7; // avoid edge
        i2 = i1 / 2;
        #endif
        #endif
      } else {
        i1 = 1;                        // avoid edge
        i3 = kalhits.GetEntries() - 2; // avoid edge
        i2 = i3 / 2;
      }
      // ---------------------------
      //  Create a dummy site: sited
      // ---------------------------
      TVTrackHit *ht1p = static_cast<TVTrackHit *>(kalhits.At(i1));
      TVTrackHit *htdp = new EXTPCHit(*static_cast<EXTPCHit *>(ht1p));
      TVTrackHit &hitd = *htdp;

      hitd(0,1) = 1.e6;   // give a huge error to d
      hitd(1,1) = 1.e6;   // give a huge error to z

      TKalTrackSite &sited = *new TKalTrackSite(hitd);
      sited.SetHitOwner();// site owns hit
      sited.SetOwner();   // site owns states

      // ---------------------------
      //  Create initial helix
      // ---------------------------
      TVTrackHit &h1 = *static_cast<TVTrackHit *>(kalhits.At(i1)); // first hit
      TVTrackHit &h2 = *static_cast<TVTrackHit *>(kalhits.At(i2)); // middle hit
      TVTrackHit &h3 = *static_cast<TVTrackHit *>(kalhits.At(i3)); // last hit
      TVector3    x1 = h1.GetMeasLayer().HitToXv(h1);
      TVector3    x2 = h2.GetMeasLayer().HitToXv(h2);
      TVector3    x3 = h3.GetMeasLayer().HitToXv(h3);
      THelicalTrack helstart(x1, x2, x3, h1.GetBfield(), gkDir); // initial helix

      // ---------------------------
      //  Set dummy state to sited
      // ---------------------------

      static TKalMatrix svd(kSdim,1);
      svd(0,0) = 0.;                        // dr
      svd(1,0) = helstart.GetPhi0();        // phi0
      svd(2,0) = helstart.GetKappa();       // kappa
      svd(3,0) = 0.;                        // dz
      svd(4,0) = helstart.GetTanLambda();   // tan(lambda)
      if (kSdim == 6) svd(5,0) = 0.;        // t0

      static TKalMatrix C(kSdim,kSdim);
      for (Int_t i=0; i<kSdim; i++) {
        C(i,i) = 1.e4;   // dummy error matrix
      }

      sited.Add(new TKalTrackState(svd,C,sited,TVKalSite::kPredicted));
      sited.Add(new TKalTrackState(svd,C,sited,TVKalSite::kFiltered));

      // ---------------------------
      //  Add sited to the kaltrack
      // ---------------------------

      EXHYBTrack kaltrack;   // a track is a kal system
      kaltrack.SetOwner();   // kaltrack owns sites
      kaltrack.Add(&sited);  // add the dummy site to this track

      // ---------------------------
      //  Prepare hit iterrator
      // ---------------------------

      TIter nextsite(&kalhits, gkDir); // come in to IP, if gkDir = kIterBackward

      // ---------------------------
      //  Start Kalman Filter
      // ---------------------------

      TVTrackHit *hitp = 0;
      while ((hitp = dynamic_cast<TVTrackHit *>(nextsite()))) {
        #if 0
        JHit *jhp = static_cast<JHit *>(const_cast<TObject *>(static_cast<EXTPCHit *>(hitp)->GetHitPtr()));
        Int_t    layer  = jhp->GetLayerID ();
        if (layer < 5 || layer > 22) continue;
        #endif
        TKalTrackSite  &site = *new TKalTrackSite(*hitp); // new site
        if (!kaltrack.AddAndFilter(site)) {               // filter it
          cerr << " site discarded!" << endl;
          delete &site;                        // delete it if failed
          #if 1
        }else{

          // cerr << " hoge" << endl;


        }
        #else
      }
      site.DebugPrint();
      static_cast<JHit *>(const_cast<TObject *>(static_cast<EXTPCHit *>(hitp)->GetHitPtr()))->DebugPrint();
      static_cast<TKalTrackState &>(site.GetCurState()).DebugPrint();
      TVector3 xv = hitp->GetMeasLayer().HitToXv(*hitp);
      cerr << " xvhit = (" << xv.X() << "," << xv.Y() << "," << xv.Z() << ")" << endl;
      #endif
    } // end of Kalman filter

    // ---------------------------
    //  Save it as a JHelix
    // ---------------------------

    ndf  = kaltrack.GetNDF();
    chi2 = kaltrack.GetChi2();
    Double_t cl   = TMath::Prob(chi2, ndf);
    Double_t dr   = kaltrack.GetCurSite().GetCurState()(0, 0);
    Double_t fi0  = kaltrack.GetCurSite().GetCurState()(1, 0);
    Double_t cpa  = kaltrack.GetCurSite().GetCurState()(2, 0);
    Double_t dz   = kaltrack.GetCurSite().GetCurState()(3, 0);
    Double_t tnl  = kaltrack.GetCurSite().GetCurState()(4, 0);
    #if 0
    Double_t cs   = tnl/TMath::Sqrt(1.+tnl*tnl);
    Double_t t0   = kaltrack.GetCurSite().GetCurState()(5, 0);
    #endif
    Double_t rho  = static_cast<TKalTrackState &>(kaltrack.GetCurSite().GetCurState()).GetHelix().GetRho();
    TVector3 xv0  = static_cast<TKalTrackSite &>(kaltrack.GetCurSite()).GetPivot();
    Double_t phi1st = 0.;
    Double_t philst = - (x3 - x1).Perp()/rho;
    helix.SetHelixParameters(dr,fi0,1/rho,dz,tnl,xv0.X(),xv0.Y(),xv0.Z(),
    phi1st,philst);
    #if 1
    helix.DebugPrint();
    cerr << "(chi2, ndf) = (" << chi2 << "," << ndf << ")" << endl;
    #endif
    // ---------------------------
    //  Skip the rest if ndf<0
    // ---------------------------
    if (ndf <= 0) return;

    // ---------------------------
    //  Smooth the track
    // ---------------------------

    kaltrack.SmoothBackTo(1);

    #if 1
    // ============================================================
    //  Monitor Fit Result
    // ============================================================
    //
    static TNtupleD *hTracks    = 0;
    if (!hTracks) {
      stringstream list;
      list << "ndf:chi2:cl:dr:fi0:cpa:dz:tnl" << ends;
      hTracks = new TNtupleD("hTracks","",list.str().data());
    }
    static const Int_t kNdata = 10000;
    Double_t data[kNdata];
    data[0] = ndf;
    data[1] = chi2;
    data[2] = cl;
    data[3] = dr;
    data[4] = fi0;
    data[5] = cpa;
    data[6] = dz;
    data[7] = tnl;


    // const   char *  ty = typeid(hp).name();
    // cerr << "type of hp =" << ty << endl;
    //
    // TVector3 vec = hp->GetPosition();
    //
    // printf("x: %f\n", vec.X());



    // JTrack track = *trkp;
    //
    // printf("GetEntries: %d\n", track.GetEntriesFast());
    //
    //
    // const   char *  ty = typeid(track.GetLast()).name();
    //
    //
    // cerr << "type of GetLast =" << ty << endl;
    //
    // if (track.IsEmpty()) {
    //   cerr << "Empty ="<< endl;
    // }




    //
    // const   char *  ty = typeid(track).name();
    // cerr << "type of track=" << ty << endl;//P6JTrack
    //
    // JHit jhit = track.At(0);
    //
    // const   char *  ty4 = typeid(track.At(0)).name();
    // cerr << "type of track.At(0)=" << ty4 << endl;//4JHit

    // const   char *  ty2 = typeid(jhit).name();
    // cerr << "type of jhit=" << ty2 << endl;//P6JTrack

    // const   char *  ty2 = typeid(*trkp).name();
    // cerr << "type of *trkp=" << ty2 << endl;//6JTrack
    //
    // const   char *  ty3 = typeid(*(trkp->At(0))).name();
    // cerr << "type of *(trkp->At(0))=" << ty3 << endl;//4JHit
    //
    // const   char *  ty4 = typeid((trkp->At(0))).name();
    // cerr << "type of (trkp->At(0))=" << ty4 << endl;//4JHit


    // printf("sizeof(*trkp) : %lu\n", sizeof(*trkp));//8

    //  for (int i = 0; i < sizeof(*trkp); i++) {
    //    const   char *  ty3 = typeid(*trkp->At(i)).name();
    //    cerr << "type of *trkp->At(" << i << ")=" << ty3 << endl;//4JHit
    //  }




    //    // const   char *  ty7 = typeid(*(trkp->At(0))).name();
    //    // cerr << "type of *(trkp->At(0))=" << ty7 << endl;//4JHit
    //
    //    printf("sizeof(*(trkp->At(0))) : %lu\n", sizeof(*(trkp->At(0))));//16
    //
    //    int sizeOfJTrackArr = sizeof(*trkp);
    //
    //    for (int i = 0; i < sizeOfJTrackArr; i++) {
    //
    //      int sizeOfJHitArr = sizeof(*trkp->At(i));
    //
    //      for (int m = 0; m < sizeOfJHitArr; m++) {
    //
    //        //  JHit *hp  = static_cast<JHit*>(*trkp->At(i));
    //        // TObject hit = static_cast<TObject>(*trkp->At(i));
    //
    //        JHit *hit = new JHit();
    //
    //        hit = static_cast<JHit*>(trkp->At(i));
    //
    ////        const   char *  ty11 = typeid(hit).name();
    ////        cerr << "type of hit=" << ty11 << endl;//4JHit
    //
    //
    //
    //         TVector3 vec = hit->GetPosition();
    //
    //
    //         printf("x: %f\n", vec.X());
    //         printf("y: %f\n", vec.Y());
    //         printf("z: %f\n", vec.Z());
    //
    //
    //
    //      }
    //
    //    }








    hTracks->Fill(data);
    #if 0
    cerr << "tuple: (chi2,ndf)=(" << data[1] << ", " << data[0] << ")" << endl;
    #endif

    /////////// TEMPORARY TREATMENT /////////////////////////
    static const Int_t kOffset   = 11; // ndf, chi2, ...
    static const Int_t kNmodules =  3; // # modules
    static const Int_t kNlayers  = 28; // # layers / module
    /////////// TEMPORARY TREATMENT /////////////////////////
    static TNtupleD *hResXin = 0;
    if (!hResXin) {
      stringstream list;
      list << "ndf:chi2:cl:dr:fi0:cpa:dz:tnl";
      list << ":fgIsUpperHit:fgIsMidHit:fgIsLowerHit";
      for (Int_t i=0; i<kNlayers*kNmodules; i++) {
        list << ":dxin" << setw(2) << setfill('0') << i;
        list << ":dxot" << setw(2) << setfill('0') << i;
      }
      for (Int_t i=0; i<kNlayers*kNmodules; i++) {
        list << ":ch" << setw(2) << setfill('0') << i;
      }
      for (Int_t i=0; i<kNlayers*kNmodules; i++) {
        list << ":pk" << setw(2) << setfill('0') << i;
      }
      for (Int_t i=0; i<kNlayers*kNmodules; i++) {
        list << ":xpm5" << setw(2) << setfill('0') << i;
        list << ":chm5" << setw(2) << setfill('0') << i;
        list << ":xpm4" << setw(2) << setfill('0') << i;
        list << ":chm4" << setw(2) << setfill('0') << i;
        list << ":xpm3" << setw(2) << setfill('0') << i;
        list << ":chm3" << setw(2) << setfill('0') << i;
        list << ":xpm2" << setw(2) << setfill('0') << i;
        list << ":chm2" << setw(2) << setfill('0') << i;
        list << ":xpm1" << setw(2) << setfill('0') << i;
        list << ":chm1" << setw(2) << setfill('0') << i;
        list << ":xpm0" << setw(2) << setfill('0') << i;
        list << ":chm0" << setw(2) << setfill('0') << i;
        list << ":xpp1" << setw(2) << setfill('0') << i;
        list << ":chp1" << setw(2) << setfill('0') << i;
        list << ":xpp2" << setw(2) << setfill('0') << i;
        list << ":chp2" << setw(2) << setfill('0') << i;
        list << ":xpp3" << setw(2) << setfill('0') << i;
        list << ":chp3" << setw(2) << setfill('0') << i;
        list << ":xpp4" << setw(2) << setfill('0') << i;
        list << ":chp4" << setw(2) << setfill('0') << i;
        list << ":xpp5" << setw(2) << setfill('0') << i;
        list << ":chp5" << setw(2) << setfill('0') << i;
      }
      for (Int_t i=0; i<kNlayers*kNmodules; i++) {
        list << ":xbar" << setw(2) << setfill('0') << i;
      }
      for (Int_t i=0; i<kNlayers*kNmodules; i++) {
        list << ":fi0" << setw(2) << setfill('0') << i;
      }
      list << ends;
      hResXin = new TNtupleD("hResXin","",list.str().data());
    }
    for (Int_t i=kOffset; i<kNdata; i++) data[i] = 99999.;

    TIter nextlayer(&kaltrack);
    nextlayer();
    TKalTrackSite *sitep;

    fgIsMidHit   = kFALSE;
    fgIsUpperHit = kFALSE;
    fgIsLowerHit = kFALSE;

    while ((sitep = static_cast<TKalTrackSite *>(nextlayer()))) {
      #if 0
      Int_t index = sitep->GetHit().GetMeasLayer().GetIndex();
      #else
      ////// VERY TEMPORARY ////////////////////////////////////////
      const JHit  *hp    = static_cast<const JHit *>(static_cast<const EXTPCHit &>(sitep->GetHit()).GetHitPtr());
      Int_t module = hp->GetModuleID();
      Int_t layer  = hp->GetLayerID();
      Int_t superlayer = (module < 2 ? 0 : (module < 5 ? 1 : 2));
      Int_t index = superlayer * JPadGeom::GetNrow() + layer;
      ////// VERY TEMPORARY ////////////////////////////////////////
      #endif

      Int_t CenterPadOfCluster = hp->GetPadID();
      #if 0
      Int_t MaxReadoutPad = 175; // for Module3 (channelmap_090324.dat)
      Int_t MinReadoutPad = 112; // for Module3 (channelmap_090324.dat)
      Int_t PadOffset     = 15;
      #else
      Int_t MaxReadoutPad = 111; // for Module3 (channelmap_090324.dat)
      Int_t MinReadoutPad =   0; // for Module3 (channelmap_090324.dat)
      Int_t PadOffset     = 15;
      #endif
      //select the hits which are near enough the center of readout region
      if (CenterPadOfCluster < MaxReadoutPad-PadOffset && CenterPadOfCluster > MinReadoutPad + PadOffset) {
        if      (index == fgLayerToLookAt)     fgIsMidHit   = kTRUE;
        else if (index == fgLayerToLookAt + 1) fgIsUpperHit = kTRUE;
        else if (index == fgLayerToLookAt - 1) fgIsLowerHit = kTRUE;
      }
      data[8]  = fgIsUpperHit;
      data[9]  = fgIsMidHit;
      data[10] = fgIsLowerHit;

      Double_t dxin = sitep->GetResVec()(0,0);
      Double_t dxot = 999999.;
      sitep->InvFilter();
      dxot = sitep->GetResVec(TVKalSite::kInvFiltered)(0,0);
      data[kOffset+2*index  ]        = dxin;
      data[kOffset+2*index+1]        = dxot;

      Double_t charge = hp->GetCharge();
      data[kOffset+index+2*kNlayers*kNmodules] = charge;

      Double_t peakph = hp->GetMaximum();
      data[kOffset+index+3*kNlayers*kNmodules] = peakph;

      JHit *hpp      = const_cast<JHit *>(hp);
      TVector3 xv    = hpp->GetLocalPosition();
      Double_t rc    = xv.Perp();
      Double_t phic  = TMath::ATan2(xv.Y(), xv.X());
      Double_t rphic = rc*phic;
      TIter nextpad(hpp);
      Double_t drphi[1000], fprf[1000];
      Int_t    ipad = 0;
      Int_t    ipek = 0;
      Double_t chmx = -9999;
      JCluster *cp;
      while ((cp = static_cast<JCluster *>(nextpad()))) {
        TVector2 xy = cp->GetLocalPosition();
        Double_t r    = xy.Mod();
        Double_t phi  = TMath::ATan2(xy.Y(), xy.X());
        Double_t rphi = r*phi;
        #if 1
        Double_t ch   = cp->GetCharge();
        #else
        static Double_t kPed = 0.5;
        Double_t ch   = cp->GetCharge() - cp->GetLength()*kPed;
        #endif
        drphi[ipad] = rphic-rphi;
        fprf [ipad] = ch/charge;
        if (ch > chmx) {
          ipek = ipad;
          chmx = ch;
        }
        ipad++;
      }
      for (Int_t i=ipek-5; i<=ipek+5; i++) {
        Int_t j = i - (ipek-5);
        data[kOffset+4*kNlayers*kNmodules+22*index+2*j  ] = 999999.;
        data[kOffset+4*kNlayers*kNmodules+22*index+2*j+1] = 0;
        if (i>=0 && i<ipad) {
          data[kOffset+4*kNlayers*kNmodules+22*index+2*j  ] = drphi[i];
          data[kOffset+4*kNlayers*kNmodules+22*index+2*j+1] = fprf [i];
          //cerr << "j = " << j << " drphi = " << drphi[i] << " fprf = " << fprf[i] << endl;
        }
      }
      Double_t phi0c  = static_cast<TKalTrackState &>(sitep->GetCurState()).GetHelix().GetPhi0();
      data[kOffset+26*kNlayers*kNmodules+index] = rphic;
      data[kOffset+27*kNlayers*kNmodules+index] = phi0c;
    }
    hResXin->Fill(data);

    static TNtupleD *hResZin = 0;
    if (!hResZin) {
      stringstream list;
      list << "ndf:chi2:cl:dr:fi0:cpa:dz:tnl";
      list << ":dummy1:dummy2:dummy3";
      for (Int_t i=0; i<kNlayers*kNmodules; i++) {
        list << ":dzin" << setw(2) << setfill('0') << i;
        list << ":dzot" << setw(2) << setfill('0') << i;
      }
      for (Int_t i=0; i<kNlayers*kNmodules; i++) {
        list << ":ch" << setw(2) << setfill('0') << i;
      }
      for (Int_t i=0; i<kNlayers*kNmodules; i++) {
        list << ":pk" << setw(2) << setfill('0') << i;
      }
      list << ends;
      hResZin = new TNtupleD("hResZin","",list.str().data());
    }
    for (Int_t i=kOffset; i<kNdata; i++) data[i] = 99999.;
    nextlayer.Reset();
    nextlayer();
    while ((sitep = static_cast<TKalTrackSite *>(nextlayer()))) {
      #if 0
      Int_t index = sitep->GetHit().GetMeasLayer().GetIndex();
      #else
      ////// VERY TEMPORARY ////////////////////////////////////////
      const JHit  *hp    = static_cast<const JHit *>(static_cast<const EXTPCHit &>(sitep->GetHit()).GetHitPtr());
      Int_t module = hp->GetModuleID();
      Int_t layer  = hp->GetLayerID();
      Int_t superlayer = (module < 2 ? 0 : (module < 5 ? 1 : 2));
      Int_t index = superlayer * JPadGeom::GetNrow() + layer;
      ////// VERY TEMPORARY ////////////////////////////////////////
      #endif
      Double_t charge = hp->GetCharge();
      Double_t peakph = hp->GetMaximum();
      //cerr << "index = " << index << endl;
      Double_t dzin = sitep->GetResVec()(1,0);
      Double_t dzot = 999999.;
      dzot = sitep->GetResVec(TVKalSite::kInvFiltered)(1,0);
      data[kOffset+2*index  ] = dzin;
      data[kOffset+2*index+1] = dzot;
      data[kOffset+2*kNlayers*kNmodules+index] = charge;
      data[kOffset+3*kNlayers*kNmodules+index] = peakph;
    }
    hResZin->Fill(data);

    static TH2D **hPadRes = 0;
    if (!hPadRes) {
      hPadRes = new TH2D* [kNlayers*kNmodules];
      for (Int_t i=0; i<kNlayers*kNmodules; i++) {
        stringstream name;
        name << "qx" << setw(2) << setfill('0') << i << ends;
        hPadRes[i] = new TH2D(name.str().data(),"",200,-10.,10.,100,0.,1.);
      }
    }

    //トラックにアソシエイトされたパッドごのとチャージの分布(Q vs. X)
    static TH2D **hPadCharge = 0;
    if (!hPadCharge) {
      hPadCharge = new TH2D* [kNlayers*kNmodules];
      for (Int_t i=0; i<kNlayers*kNmodules; i++) {
        stringstream name;
        name << "XQhist" << setw(2) << setfill('0') << i << ends;
        hPadCharge[i] = new TH2D(name.str().data(),"",100,-250.,250.,100,0.,7000.);
      }
    }


    //パッドごのと解像度(σ vs. X),レジデュアル
    static TH2D **hXRes = 0;
    if (!hXRes) {
      hXRes = new TH2D* [kNlayers*kNmodules];
      for (Int_t i=0; i<kNlayers*kNmodules; i++) {
        stringstream name;
        name << "XReshist" << setw(2) << setfill('0') << i << ends;
        hXRes[i] = new TH2D(name.str().data(),"",100,-250.,250.,100,-10.,10.);
      }
    }



    next.Reset();
    while ((hp = static_cast<JHit *>(next()))) {
      Int_t module   = hp->GetModuleID();

      Int_t layer    = hp->GetLayerID();
      Int_t superlayer = (module < 2 ? 0 : (module < 5 ? 1 : 2));
      Int_t index    = superlayer * JPadGeom::GetNrow() + layer;
      Double_t Q     = hp->GetCharge();
      TVector3 xv    = hp->GetLocalPosition();
      Double_t rc    = xv.Perp();
      Double_t phic  = TMath::ATan2(xv.Y(), xv.X());
      Double_t rphic = rc*phic;



      hPadCharge[index]->Fill(xv.X(), Q);




      TIter nextpad(hp);
      JCluster *cp;
      while ((cp = static_cast<JCluster *>(nextpad()))) {
        TVector2 xy = cp->GetLocalPosition();
        Double_t r    = xy.Mod();
        Double_t phi  = TMath::ATan2(xy.Y(), xy.X());
        Double_t rphi = r*phi;
        hPadRes[index]->Fill(rphic-rphi, cp->GetCharge()/Q);

        hXRes[index]->Fill(xv.X(), rphic-rphi);


      }
    }
    #endif
  }
