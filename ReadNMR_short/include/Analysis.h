/*
 * Analysis.h
 *
 *  Created on: Jan 26, 2016
 *      Author: klein
 *      * this Class manipulates the different spectra from the NMR
 */

#ifndef ANALYSIS_H_
#define ANALYSIS_H_

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>

#include <TROOT.h>
#include <TFile.h>
#include <TH1D.h>
#include <TF1.h>
#include <TString.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TSpectrum.h>
#include <TLegend.h>
#include <TMath.h>

using namespace std;

class Analysis
{
public:
	TH1D * DiffSpec;
	TH1D *Spectrum;  // the histogram we want to find the spectrum of
	TSpectrum *spec;
	TF1 *FitPeak;  // function to fit the peak forund by FindPeak
	TF1 *FitFcn; //      combined FitFunction
	TF1 *BackGround;//
	TF1 *Signal;
	// spectrum finder
	Int_t npeaks;	//number of peaks to find
	Double_t *xpeaks; // xpositions of peaks
	Float_t sigma; // sigma for peak search
	Double_t amplitude;// amplitude from peak serach
	Double_t par[3];
	Double_t PeakFitPar[3]; // the parametes of the peak fitteing
	Float_t fit_low_range; // in the findpeak routine
	Float_t fit_high_range; // range for fitting routine, determined by peak and sigma
	Float_t fit_low_overall; // in main fitting routine
	Float_t fit_high_overall;
	Float_t Offset; // Offset which is added so that we do not have any negative counts
					// this offset gets determned from the loweest histogram entry and then added if that one is negative
	Float_t NMRArea; // fitted area of NMR signal
	Int_t NumberOfSpectra; // how many spectra are in the NMR sweep.
	Int_t NparFit; // numbers of ft aparmeter fr the full function

Analysis();
virtual ~Analysis();
//virtual ~ReadFile();
virtual TH1D *SubtractHisto(TH1D* , TH1D*);
int FitSpectrum(TH1D *, Int_t ,TString );
int FitSpectrum_tanh(TH1D *, Int_t ,TString );

// Double_t Background(Double_t *, Double_t *) {return (par[0] + par[1]*x[0] + par[2]*x[0]*x[0]);}
static  Double_t Background(Double_t *, Double_t *) ;
static Double_t Background_tanh(Double_t *, Double_t *);

static  Double_t LorentzianPeak(Double_t *, Double_t *);
static Double_t FitFunction(Double_t *, Double_t *);
static Double_t FitFunctionL(Double_t *, Double_t *);
static Double_t FitFunction_T(Double_t *, Double_t *);
static Double_t FitFunctionL_T(Double_t *, Double_t *);
 //Double_t GausPeak(Double_t *, Double_t *);
static Double_t GausPeak(Double_t *, Double_t *);

 void FindPeak(TH1D * , TString );


private:
	// Limits for background fitting;
 Bool_t peakfind;



};

Analysis::Analysis(){
	cout<<"initializing Analysis Package \n";
	npeaks = 1 ; // number of peaks for peak fitting routine.
	sigma = .04;

}

Analysis::~Analysis(){
	cout<<"Done with analysis \n";
}

TH1D * Analysis::SubtractHisto(TH1D *histo1 , TH1D *histo2){
	// this routine subtracts two histograms and returns the difference;
	// we will take the difference of histo1 - histo2
	//
	// Clone histo1 into histo 2;
	DiffSpec = (TH1D*)histo1->Clone("DiffSpec");
	DiffSpec->Add(histo2,-1.);
	DiffSpec->SetTitle("Difference spectrum");
	// now take the difference of this minus histo2
	return DiffSpec;
}



// Block of fit functions

// Quadratic background function
Double_t Analysis::Background(Double_t *x, Double_t *par) {
	// new version with point rejection, the idea being that
	// I will not fit background in the peak area but on left and right side of it
	// see fit descrition in Root reference manual
	/*if(x[0]>212.884  && x[0] < 213.093){
	     TF1::RejectPoint();
	 }*/

   return (par[0] + par[1]*x[0] + par[2]*x[0]*x[0]);
   //return par[0] + par[1]*x[0] ;

}

Double_t Analysis::Background_tanh(Double_t *x, Double_t *par){
	// Background using a sum of tanh and quadratic
	   //cout<<"x0 &&  "<<212.982-x[0] <<"\n";
	x[0]=(x[0]-212.982)*20.;
	   Double_t test = ((par[0] + par[1]*x[0] + par[2]*x[0]*x[0])*.1 +par[3]*TMath::TanH((x[0]-212.982)*par[4]));
       x[0] =x[0]/20.+212.982;
       return test;
}

// Different signal functions block
// Lorenzian Peak function
Double_t Analysis::LorentzianPeak(Double_t *x, Double_t *par) {
  return (0.5*par[0]*par[1]/TMath::Pi()) /
    TMath::Max( 1.e-10,(x[0]-par[2])*(x[0]-par[2])
   + .25*par[1]*par[1]);
}

// Sum of background and peak function
Double_t Analysis::FitFunction(Double_t *x, Double_t *par) {
  return Background(x,par) + GausPeak(x,&par[3]);
}

// Sum of background and peak function
Double_t Analysis::FitFunctionL(Double_t *x, Double_t *par) {
  return Background(x,par) + LorentzianPeak(x,&par[3]);
}
// Sum of background and peak function
Double_t Analysis::FitFunction_T(Double_t *x, Double_t *par) {
  return Background_tanh(x,par) + GausPeak(x,&par[3]);
}

// Sum of background and peak function
Double_t Analysis::FitFunctionL_T(Double_t *x, Double_t *par) {
  return Background_tanh(x,par) + LorentzianPeak(x,&par[3]);
}


Double_t Analysis::GausPeak(Double_t *x, Double_t *par){
	Double_t arg = 0;
	if(par[2] !=0) arg = (x[0]-par[1])/par[2];
	return par[0]*TMath::Exp(-.5*arg*arg);
}

void Analysis::FindPeak(TH1D * Spectrum,TString FitForm){
	// this is from peaks.C at Cern
	// Create new spectrum,weonly assume on peak for the moment

	// Determine if any of the cnannels is negative and calculate the offset
	Offset = Spectrum->GetBinContent(Spectrum->GetMinimumBin());
	if (Offset < 0.){
		cout<<"Add offset to spectrum  "<< Offset<<"   "<<Spectrum->GetNbinsX()<<"\n";
		for (Int_t k = 0;  k< Spectrum->GetNbinsX();k++){
			cout<<"adding bin"<<k<<"\n";
			Spectrum->AddBinContent(k,-Offset);  // add offset
		}
	}
    peakfind = true;
	spec= new TSpectrum(npeaks,1.);
	Int_t nfound = spec->Search(Spectrum,sigma,"nobackground new",.01);
	cout<<"\n\n\n*******************************************\n";
	cout<<"Number of peaks found "<<nfound<<"\n";
	// fill array with peak posistions
	xpeaks = (spec->GetPositionX());
	if(*xpeaks < 212.982 - .018 || *xpeaks > 212.982 + .018 ){
		cout<< "TSpectrum failed, assign peak to 212.98 \n";
		*xpeaks = 212.982;
		sigma = .018;
		peakfind = false;
	}
	for (Int_t k=0 ; k<nfound; k++ ){
		cout<<"position of peak found is  " <<setprecision(8)<<xpeaks[k]<<"\n";
		Int_t xbin = Spectrum->GetXaxis()->FindBin(xpeaks[k]); // get bin number
		amplitude = Spectrum->GetBinContent(xbin);
		// fill parameter for Gauss fit
		if (FitForm.Contains("gaus"))	{
			par[0] = amplitude;
			par[1] = xpeaks[k];
			par[2] = sigma;
			fit_low_range = xpeaks[k]-1.2*sigma;
			fit_high_range = xpeaks[k]+1.2*sigma;
			FitPeak =  new TF1("FitPeak",GausPeak,fit_low_range,fit_high_range,3);
		}

		else	{
			cout<<" amplitude"<< amplitude <<"    xpeaks[k] "<<xpeaks[k]<<"     sigma "<<sigma<<"\n";
			par[0] = amplitude;
			par[2] = xpeaks[k];
			sigma = sigma*2.354; // relation between full width and sigma
			par[1] = sigma;
			fit_low_range = xpeaks[k]-1.2*sigma;
			fit_high_range = xpeaks[k]+1.2*sigma;
			FitPeak =  new TF1("FitPeak",LorentzianPeak,fit_low_range,fit_high_range,3);
		}

		FitPeak->SetParameters(par);
		FitPeak->SetNpx(1000);
		Spectrum->Fit(FitPeak,"R");
		//FitPeak->Draw();
		// Now return the fit parameters
		PeakFitPar[0] = FitPeak->GetParameter(0);
		PeakFitPar[1] = FitPeak->GetParameter(1);
		PeakFitPar[2] = FitPeak->GetParameter(2);
	}


	cout<<"\n\n\n*******************************************\n";




}

int Analysis::FitSpectrum(TH1D * Spectrum,Int_t NumberOfSpectra,TString FitF){
	// function to fit spectrum with this needs to be called after Findpeak
	// since it relies on finding the parameters.


	if(FitF.Contains("gaus")){
		fit_low_overall = PeakFitPar[1]-(4.*sigma);
		fit_high_overall = (PeakFitPar[1]+4.*sigma);
	FitFcn =  new TF1("FitFcn",FitFunction,fit_low_overall,fit_high_overall,6);
	}
	else {
		FitFcn =  new TF1("FitFcn",FitFunctionL,fit_low_overall,fit_high_overall,6);
		fit_low_overall = PeakFitPar[2]-(2.*sigma);
		fit_high_overall = (PeakFitPar[2]+2.*sigma);
	}
	FitFcn->SetNpx(1000);
	FitFcn->SetLineWidth(4);
	FitFcn->SetLineColor(kMagenta);
	if(FitF.Contains("gaus")){

	FitFcn->SetParameters(1.,1.,1.,PeakFitPar[0],PeakFitPar[1],PeakFitPar[2]);
	FitFcn->FixParameter(4,PeakFitPar[1]);
	FitFcn->FixParameter(5,PeakFitPar[2]);
	}
	else {
		FitFcn->SetParameters(1.,1.,1.,PeakFitPar[0],PeakFitPar[1],PeakFitPar[2]);
		FitFcn->FixParameter(4,PeakFitPar[1]);
		FitFcn->FixParameter(5,PeakFitPar[2]);
	}



	Spectrum->Fit("FitFcn","+REM","C");
	//Spectrum->Draw();
	//FitFcn->Draw("SAME");

	// Now get the single fits for background and signal

	   // improve the picture:
	   BackGround = new TF1("BackGround",Background,fit_low_overall,fit_high_overall,3);
	   BackGround->SetLineColor(kRed);
		if(FitF.Contains("gaus"))
		{
			Signal = new TF1("Signal",GausPeak,fit_low_overall,fit_high_overall,3);
		}
		else
		{
			Signal = new TF1("Signal",LorentzianPeak,fit_low_overall,fit_high_overall,3);

		}
	   Signal->SetLineColor(kBlue);
	   Signal->SetNpx(500);
	   Double_t par[6];
	   // writes the fit results into the par array
	   FitFcn->GetParameters(par);
	   BackGround->SetParameters(par);
	   BackGround->Draw("same");
	   Float_t low =0.;
	   Float_t high = 0.;
	   if(FitF.Contains("gaus")){

			Signal->SetParameters(&par[3]);
			Signal->Draw("same");
			 low = Signal->GetParameter(1)-3*(Signal->GetParameter(2));
			 high = Signal->GetParameter(1)+3*(Signal->GetParameter(2));
		}
		else
		{
			   Signal->SetParameters(&par[3]);
			   Signal->Draw("same");
			    low = Signal->GetParameter(2)-1.2*(Signal->GetParameter(1));
			    high = Signal->GetParameter(2)+1.2*(Signal->GetParameter(1));
		}

	   cout<< "\n\n\n $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n\n\n";
	   cout<<"low integration limit  "<<low<<"   high integration limit  "<<high<<"   \n";
	   NMRArea = Signal->Integral(low,high);
       cout<<"integral of fit  corrected histogram  "<<NMRArea/NumberOfSpectra<<" \n";
	   cout<< "\n\n\n $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n\n\n";
	   //fprintf(fp_out,"%lf \n", NMRArea/NumberOfSpectra);

	   // Now integrate the whole peak to get the NMR value
	   //

	   // draw the legend
	   TLegend *legend=new TLegend(0.6,0.65,0.88,0.85);
	   legend->SetTextFont(72);
	   legend->SetTextSize(0.04);
	   legend->AddEntry(Spectrum,"NMR corrected","lpe");
	   legend->AddEntry(BackGround,"Background fit","l");
	   legend->AddEntry(Signal,"Signal fit","l");
	   legend->AddEntry(FitFcn,"Global Fit","l");
	   legend->Draw();







	return 1;
}
int Analysis::FitSpectrum_tanh(TH1D * Spectrum,Int_t NumberOfSpectra,TString FitF){
	// function to fit spectrum with this needs to be called after Findpeak
	// since it relies on finding the parameters.


	if(FitF.Contains("gaus")){
		fit_low_overall = PeakFitPar[1]-(8.*sigma);
		fit_high_overall = (PeakFitPar[1]+8.*sigma);
	FitFcn =  new TF1("FitFcn",FitFunction_T,fit_low_overall,fit_high_overall,8);
	}
	else {
		fit_low_overall = PeakFitPar[2]-(8.*sigma);
		fit_high_overall = (PeakFitPar[2]+8.*sigma);
		FitFcn =  new TF1("FitFcn",FitFunctionL_T,fit_low_overall,fit_high_overall,8);
	}
	FitFcn->SetNpx(1000);
	FitFcn->SetLineWidth(4);
	FitFcn->SetLineColor(kMagenta);
	if(FitF.Contains("gaus")){

	FitFcn->SetParameters(1.,.2,.2,.001,100.,PeakFitPar[0],PeakFitPar[1],PeakFitPar[2]);
//	FitFcn->FixParameter(3,1.); // ude of tanh
	FitFcn->SetParLimits(2,.1,.3);
//	FitFcn->FixParameter(4,1.); // fix apmlitude of tanh
	FitFcn->FixParameter(6,PeakFitPar[1]);
	FitFcn->FixParameter(7,PeakFitPar[2]);
	}
	else {
		FitFcn->SetParameters(1.,.2,.2,.001,100.,PeakFitPar[0],PeakFitPar[1],PeakFitPar[2]);
//		FitFcn->FixParameter(3,1.); // fix apmlitude of tanh
//		FitFcn->SetParLimits(3,.8,1.1);
//		FitFcn->FixParameter(4,1.); // fix apmlitude of tanh
		FitFcn->FixParameter(6,PeakFitPar[1]);
		FitFcn->FixParameter(7,PeakFitPar[2]);
	}



	Spectrum->Fit("FitFcn","+REM","C");
	//Spectrum->Draw();
	//FitFcn->Draw("SAME");

	// Now get the single fits for background and signal

	   // improve the picture:
	   BackGround = new TF1("BackGround",Background_tanh,fit_low_overall,fit_high_overall,5);
	   BackGround->SetLineColor(kRed);
		if(FitF.Contains("gaus"))
		{
			Signal = new TF1("Signal",GausPeak,fit_low_overall,fit_high_overall,3);
		}
		else
		{
			Signal = new TF1("Signal",LorentzianPeak,fit_low_overall,fit_high_overall,3);

		}
	   Signal->SetLineColor(kBlue);
	   Signal->SetNpx(500);
	   Double_t par[8];
	   // writes the fit results into the par array
	   FitFcn->GetParameters(par);
	   BackGround->SetParameters(par);
	   BackGround->Draw("same");
	   Float_t low =0.;
	   Float_t high = 0.;
	   if(FitF.Contains("gaus")){

			Signal->SetParameters(&par[5]);
			Signal->Draw("same");
			 low = Signal->GetParameter(1)-3*(Signal->GetParameter(2));
			 high = Signal->GetParameter(1)+3*(Signal->GetParameter(2));
		}
		else
		{
			   Signal->SetParameters(&par[5]);
			   Signal->Draw("same");
			    low = Signal->GetParameter(2)-1.2*(Signal->GetParameter(1));
			    high = Signal->GetParameter(2)+1.2*(Signal->GetParameter(1));
		}

	   cout<< "\n\n\n $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n\n\n";
	   cout<<"low integration limit  "<<low<<"   high integration limit  "<<high<<"   \n";
	   NMRArea = Signal->Integral(low,high);
       cout<<"integral of fit  corrected histogram  "<<NMRArea/NumberOfSpectra<<" \n";
	   cout<< "\n\n\n $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n\n\n";
	   //fprintf(fp_out,"%lf \n", NMRArea/NumberOfSpectra);

	   // Now integrate the whole peak to get the NMR value
	   //

	   // draw the legend
	   TLegend *legend=new TLegend(0.6,0.65,0.88,0.85);
	   legend->SetTextFont(72);
	   legend->SetTextSize(0.04);
	   legend->AddEntry(Spectrum,"NMR corrected","lpe");
	   legend->AddEntry(BackGround,"Background fit","l");
	   legend->AddEntry(Signal,"Signal fit","l");
	   legend->AddEntry(FitFcn,"Global Fit","l");
	   legend->Draw();







	return 1;
}


#endif /* ANALYSIS_H_ */
