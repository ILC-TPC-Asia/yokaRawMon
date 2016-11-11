

void test2()
{
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    gStyle->SetTickLength(0.0,"xy");
    gStyle->SetPadGridX(0);
    gStyle->SetPadGridY(0);
    gStyle->SetOptStat(0);

    std::ifstream fdata("outdata.dat");
	
    TH2F* hInn = new TH2F("hInn","hInn", 9, 0, 9, 5, 0, 5);
    TH2F* hOut = new TH2F("hOut","hOut", 10, 0, 10, 5, 5, 10);

	double data[10]={};
	while ( fdata >> data[0] >> data[1] >> data[2] ) {
		std::cerr << data[0] << " " << data[1] << " " << data[2] << std::endl;
	
		// ----- inner part
		if ( data[0]==12 ) hInn->SetBinContent(1,1,data[2]);
		if ( data[0]==11 ) hInn->SetBinContent(1,2,data[2]);
		if ( data[0]==10 ) hInn->SetBinContent(1,3,data[2]);
		if ( data[0]== 9 ) hInn->SetBinContent(1,4,data[2]);
		if ( data[0]== 8 ) hInn->SetBinContent(1,5,data[2]);

        if ( data[0]==24 ) hInn->SetBinContent(2,1,data[2]);
        if ( data[0]==23 ) hInn->SetBinContent(2,2,data[2]);
        if ( data[0]==22 ) hInn->SetBinContent(2,3,data[2]);
        if ( data[0]==21 ) hInn->SetBinContent(2,4,data[2]);
        if ( data[0]==20 ) hInn->SetBinContent(2,5,data[2]);

        if ( data[0]==36 ) hInn->SetBinContent(3,1,data[2]);
        if ( data[0]==35 ) hInn->SetBinContent(3,2,data[2]);
        if ( data[0]==34 ) hInn->SetBinContent(3,3,data[2]);
        if ( data[0]==33 ) hInn->SetBinContent(3,4,data[2]);
        if ( data[0]==32 ) hInn->SetBinContent(3,5,data[2]);

        if ( data[0]==48 ) hInn->SetBinContent(4,1,data[2]);
        if ( data[0]==47 ) hInn->SetBinContent(4,2,data[2]);
        if ( data[0]==46 ) hInn->SetBinContent(4,3,data[2]);
        if ( data[0]==45 ) hInn->SetBinContent(4,4,data[2]);
        if ( data[0]==44 ) hInn->SetBinContent(4,5,data[2]);

        if ( data[0]==60 ) hInn->SetBinContent(5,1,data[2]);
        if ( data[0]==59 ) hInn->SetBinContent(5,2,data[2]);
        if ( data[0]==58 ) hInn->SetBinContent(5,3,data[2]);
        if ( data[0]==57 ) hInn->SetBinContent(5,4,data[2]);
        if ( data[0]==56 ) hInn->SetBinContent(5,5,data[2]);

        if ( data[0]==72 ) hInn->SetBinContent(6,1,data[2]);
        if ( data[0]==71 ) hInn->SetBinContent(6,2,data[2]);
        if ( data[0]==70 ) hInn->SetBinContent(6,3,data[2]);
        if ( data[0]==69 ) hInn->SetBinContent(6,4,data[2]);
        if ( data[0]==68 ) hInn->SetBinContent(6,5,data[2]);

        if ( data[0]==84 ) hInn->SetBinContent(7,1,data[2]);
        if ( data[0]==83 ) hInn->SetBinContent(7,2,data[2]);
        if ( data[0]==82 ) hInn->SetBinContent(7,3,data[2]);
        if ( data[0]==81 ) hInn->SetBinContent(7,4,data[2]);
        if ( data[0]==80 ) hInn->SetBinContent(7,5,data[2]);

        if ( data[0]==96 ) hInn->SetBinContent(8,1,data[2]);
        if ( data[0]==95 ) hInn->SetBinContent(8,2,data[2]);
        if ( data[0]==94 ) hInn->SetBinContent(8,3,data[2]);
        if ( data[0]==93 ) hInn->SetBinContent(8,4,data[2]);
        if ( data[0]==92 ) hInn->SetBinContent(8,5,data[2]);

        if ( data[0]==108) hInn->SetBinContent(9,1,data[2]);
        if ( data[0]==107) hInn->SetBinContent(9,2,data[2]);
        if ( data[0]==106) hInn->SetBinContent(9,3,data[2]);
        if ( data[0]==105) hInn->SetBinContent(9,4,data[2]);
        if ( data[0]==104) hInn->SetBinContent(9,5,data[2]);

        // ----- outer part
        if ( data[0]== 6 ) hOut->SetBinContent(1,1,data[2]);
        if ( data[0]== 5 ) hOut->SetBinContent(1,2,data[2]);
        if ( data[0]== 4 ) hOut->SetBinContent(1,3,data[2]);
        if ( data[0]== 3 ) hOut->SetBinContent(1,4,data[2]);
        if ( data[0]== 2 ) hOut->SetBinContent(1,5,data[2]);

        if ( data[0]==18 ) hOut->SetBinContent(2,1,data[2]);
        if ( data[0]==17 ) hOut->SetBinContent(2,2,data[2]);
        if ( data[0]==16 ) hOut->SetBinContent(2,3,data[2]);
        if ( data[0]==15 ) hOut->SetBinContent(2,4,data[2]);
        if ( data[0]==14 ) hOut->SetBinContent(2,5,data[2]);

        if ( data[0]==30 ) hOut->SetBinContent(3,1,data[2]);
        if ( data[0]==29 ) hOut->SetBinContent(3,2,data[2]);
        if ( data[0]==28 ) hOut->SetBinContent(3,3,data[2]);
        if ( data[0]==27 ) hOut->SetBinContent(3,4,data[2]);
        if ( data[0]==26 ) hOut->SetBinContent(3,5,data[2]);

        if ( data[0]==42 ) hOut->SetBinContent(4,1,data[2]);
        if ( data[0]==41 ) hOut->SetBinContent(4,2,data[2]);
        if ( data[0]==40 ) hOut->SetBinContent(4,3,data[2]);
        if ( data[0]==39 ) hOut->SetBinContent(4,4,data[2]);
        if ( data[0]==38 ) hOut->SetBinContent(4,5,data[2]);

        if ( data[0]==54 ) hOut->SetBinContent(5,1,data[2]);
        if ( data[0]==53 ) hOut->SetBinContent(5,2,data[2]);
        if ( data[0]==52 ) hOut->SetBinContent(5,3,data[2]);
        if ( data[0]==51 ) hOut->SetBinContent(5,4,data[2]);
        if ( data[0]==50 ) hOut->SetBinContent(5,5,data[2]);

        if ( data[0]==66 ) hOut->SetBinContent(6,1,data[2]);
        if ( data[0]==65 ) hOut->SetBinContent(6,2,data[2]);
        if ( data[0]==64 ) hOut->SetBinContent(6,3,data[2]);
        if ( data[0]==63 ) hOut->SetBinContent(6,4,data[2]);
        if ( data[0]==62 ) hOut->SetBinContent(6,5,data[2]);

        if ( data[0]==78 ) hOut->SetBinContent(7,1,data[2]);
        if ( data[0]==77 ) hOut->SetBinContent(7,2,data[2]);
        if ( data[0]==76 ) hOut->SetBinContent(7,3,data[2]);
        if ( data[0]==75 ) hOut->SetBinContent(7,4,data[2]);
        if ( data[0]==74 ) hOut->SetBinContent(7,5,data[2]);

        if ( data[0]==90 ) hOut->SetBinContent(8,1,data[2]);
        if ( data[0]==89 ) hOut->SetBinContent(8,2,data[2]);
        if ( data[0]==88 ) hOut->SetBinContent(8,3,data[2]);
        if ( data[0]==87 ) hOut->SetBinContent(8,4,data[2]);
        if ( data[0]==86 ) hOut->SetBinContent(8,5,data[2]);

        if ( data[0]==102) hOut->SetBinContent(9,1,data[2]);
        if ( data[0]==101) hOut->SetBinContent(9,2,data[2]);
        if ( data[0]==100) hOut->SetBinContent(9,3,data[2]);
        if ( data[0]==99 ) hOut->SetBinContent(9,4,data[2]);
        if ( data[0]==98 ) hOut->SetBinContent(9,5,data[2]);

        if ( data[0]==114) hOut->SetBinContent(10,1,data[2]);
        if ( data[0]==113) hOut->SetBinContent(10,2,data[2]);
        if ( data[0]==112) hOut->SetBinContent(10,3,data[2]);
        if ( data[0]==111) hOut->SetBinContent(10,4,data[2]);
        if ( data[0]==110) hOut->SetBinContent(10,5,data[2]);
	}

	//hOut->Draw("col text");
	//return;	

	/*
    for (int i=0; i<20000; i++) {
        hInn->Fill(gRandom->Gaus(4,8),gRandom->Gaus(4,8),81) ;
        hOut->Fill(gRandom->Gaus(5,8),gRandom->Gaus(4,8),81) ;
    }
	*/

    TCanvas *c3 = new TCanvas("c3","c3",900,500);

    TPad* tpad = new TPad("tpad", "tpad", 0., 0., 1., 0.5);
    tpad->SetRightMargin(0.2);
    tpad->SetTopMargin(0.0);
    tpad->SetFillStyle(0);
    tpad->Draw();
    tpad->cd();

    hInn->GetZaxis()->SetRangeUser(1500,5000);
    hInn->SetMarkerSize(3);
    hInn->Draw("col text20");

    c3->cd();

    TPad* tpad2 = new TPad("tpad2", "tpad2", 0., 0.5, 1., 1);
    tpad2->SetRightMargin(0.2);
    tpad2->SetBottomMargin(0.);
    tpad2->SetFillStyle(0);
    tpad2->Draw();
    tpad2->cd();

    hOut->GetZaxis()->SetRangeUser(1500,5000);
    hOut->SetMarkerSize(3);
    hOut->Draw("colz text20");

	#if 0 
    TCanvas *away = new TCanvas("away","",100,100);
    hOut->Draw("colz");
    away->Update();
    TPaletteAxis *palette = (TPaletteAxis*) hOut->GetListOfFunctions()->FindObject("palette");
    //palette->SetY2NDC(0.5);
	
	c3->cd();
    TPad* tpad3 = new TPad("tpad2", "tpad2", 0.7, 0.0, 1., 1);
   	tpad3->SetLeftMargin(0.98);
    tpad3->Draw();
    tpad3->cd();
    palette->Draw();
	#endif



	// ----- cross section 
    TGraph *tgInnRow5 = new TGraph();
    TGraph *tgOutRow10= new TGraph();
	for (int i=0; i<9; i++) tgInnRow5 ->SetPoint(tgInnRow5 ->GetN(),i+1,hInn->GetBinContent(i+1,3));
	for (int i=0; i<10;i++) tgOutRow10->SetPoint(tgOutRow10->GetN(),i+1,hOut->GetBinContent(i+1,3));
	

	TCanvas *c4 = new TCanvas("c4","c4",900,500);
	tgInnRow5 ->Draw("ap*");
	tgOutRow10->Draw("ap*");

}
