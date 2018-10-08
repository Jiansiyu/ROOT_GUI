#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"

void efficiency(){

	TFile *file = new TFile("../../results/test.root","r");
	file->ls();
	TH2F * real = (TH2F *) file->Get("real");
	real->GetXaxis()->SetRangeUser(-20,20);
	real->GetYaxis()->SetRangeUser(-25,125);
	//real->GetXaxis()->Set(20,-20,20);
	//real->GetYaxis()->Set(75,-25,125);
	TH2F *real_copy=(TH2F *)real->Clone();
	real_copy->SetTitle("Real Data");

	TH2F *predicted = (TH2F *) file->Get("predicted");
	predicted->GetXaxis()->SetRangeUser(-20,20);
	predicted->GetYaxis()->SetRangeUser(-25,125);
	//predicted->GetXaxis()->Set(20,-20,20);
	//predicted->GetYaxis()->Set(75,-25,125);


	real->Divide(predicted);
	TCanvas *canvas = new TCanvas("INFN GEM","infn gem",1000,1000);
	canvas->Divide(3,1);
	canvas->cd(1);
	real_copy->Draw("colz");
	canvas->cd(2);
	predicted->Draw("colz");
	canvas->cd(3);
	real->SetTitle("GEM efficiency(real/predicted)");
	real->Draw("colz");
	canvas->Modified();
	canvas->Update();
	canvas->Draw();

}
