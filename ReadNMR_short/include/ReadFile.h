/*
 * ReadFile.h
 *
 *  Created on: Dec 17, 2015
 *      Author: klein
 */

#ifndef ReadFile_h
#define ReadFile_h
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <TROOT.h>
#include <TFile.h>
#include <TH1D.h>
#include <TString.h>
#include <TCanvas.h>
#include <TTree.h>

#define MYDEBUG 0   // setting this to one results also in the spectrum written to the root file
using namespace std;



class ReadFile
{
public:
	char filename[80];
	TString inputfilename;
	FILE *fp;
	// Read the firts block
	// according to Pat the format is
	char *line[100];
	char timec1[5];
	char timec2[5];
	std::string time1;
	std::string time2; // for time stamp. is the sum of both.
	std::string timell; // exact time from data file, concatenation of time 1 and time 2
	Long64_t timel;
	Double_t FreqCenter ;   // center frequency in MHz
	Double_t FreqStep;      // step size in MHz
	Double_t ScanPoints;      // number of points
	Int_t IntScanPoints;
	Double_t ScanNumber;      // scan number
	Double_t Amplitude;  // this is the value read from the file
	Double_t Temperature; // T of crate
	Double_t ControllerV; // 3.3 V
	Double_t TuneV; // tune voltage
	Double_t Offset; // linear offset
	Double_t Norm ; // Normlization factor
	Double_t scale; // scale for background
	std::vector<Double_t> array;



	Double_t MaxAmp;  // maximum amplitude
	Int_t MaxBin;    // Bin with max in it
	Double_t MaxFreqHist; // Bin center of max bin
	Int_t FitLimit ; //    number of FreqStep below and above frequency center
	Double_t BaseLineOffset;

	Int_t  NumberOfSpectra; // number of spectra in file
	TH1D * NMR1;
	TCanvas * myCanvas ;
	TFile *RootFile; // this file contains the tree
	TTree *NMRtree; // the NMR tree
	// Double_t *NMR_DATA; // should hold the NMR data



ReadFile();
virtual ~ReadFile();
//virtual ~ReadFile();

//virtual int OpenFile(const char * );
virtual int OpenFile(TString , TString);
virtual int CloseFile();
virtual int ReadData(Int_t );
TString GetDate(TString ); // determines exact date from file name according to Labview
virtual int DrawHisto(TString);
virtual int ScaleHisto(Double_t);
virtual int WriteROOT(TString);
//virtual int MakeTree(TTree *, Double_t *, Int_t);
virtual int MakeTree();


};


#endif /* class ifdef */


#ifdef ReadFile_cxx

ReadFile::ReadFile(){
}


//int ReadFile::OpenFile(const char * input_NMR_filename){

	int ReadFile::OpenFile(TString input_NMR_filename, TString root_tree_file){
		// the root_tree_file contains the tree of the data
		// this is different from the histogram file
		//it gets automatically created and filled and has the same name
		// as the input but with different extension .root
      cout<<"opening file "<<input_NMR_filename<<"\n";
	  fp = fopen(input_NMR_filename,"r");
	   if(fp ==NULL){
		   cout<<" Cannot find file "<<input_NMR_filename<<"  will exit \n";
		   return 0;
	   }
	   cout <<"opening new root file   "<<root_tree_file<<"\n";
	  // RootFile = new TFile(root_tree_file,"RECREATE"); //overwrite existing file
	   RootFile = new TFile(root_tree_file,"RECREATE"); //overwrite existing file
	   return 1;

}

ReadFile::~ReadFile()
{
	// check if file is still open

	   if(fp != NULL){
		   fclose(fp);
		   cout<<" closed file and exiting \n";
	   }
	   else{
		   cout<<"bye, bye now \n";
	   }
}

int ReadFile::CloseFile(){
fclose(fp);
// close tree file

RootFile->Close();
	return 1;
}

//int ReadFile::MakeTree(TTree *NMRtree ,Double_t *array, Int_t IntScanPoints){
int ReadFile::MakeTree(){
	// this creates a tree for the data

	 	 NMRtree = new TTree("NMRtree","NMR");



	 	 NMRtree->Branch("FreqCenter",&FreqCenter,"FreqCenter/D");


	 	 NMRtree->Branch("FreqStep",&FreqStep,"FreqStep/D");


	 	 NMRtree->Branch("Temperature",&Temperature,"Temperature/D");

	 	 NMRtree->Branch("ScanPoints",&ScanPoints,"ScanPoints/D");

	 	 NMRtree->Branch("TuneV",&TuneV,"TuneV/D");

	 	 NMRtree->Branch("Offset",&Offset,"Offset/D");

	 	 NMRtree->Branch("ControllerV",&ControllerV,"ControllerV/D");

	 	 //NMRtree->Branch("NMR_array",NMR_array,"NMR_array[IntScanPoints]/D");



	 	 NMRtree->Branch("array",&array);

	 	 NMRtree->Branch("timel",&timel,"timel/L");

	 	 NMRtree->Branch("IntScanPoints",&IntScanPoints,"IntScanPoints/I"); // needed for variable size array


	return 1;
}


int ReadFile::ReadData(Int_t sign){
	// reads over data and fills it into histograms

	FitLimit = 10; //    number of FreqStep below and above frequency center

	NumberOfSpectra =0; // number of spectra in file

    fscanf(fp,"%s",timec1);
    fscanf(fp,"%s",timec2);
    time1=string(timec1);
    time2=string(timec2);
    //cout<< timec1<<"   "<<timec2<<" times \n";
    // now remove the . if there is one, concatenate the times, we have now down to 100 musecs involved
    Int_t point_pos2 = time2.find(".");
    Int_t point_pos1 = time1.find(".");
    time2.erase(0,point_pos2+1);
    time1.erase(point_pos1); // problem due to stupid string read in stripped it at 10 position
    std::string time_help= time1+time2;
    timel = std::stol(time_help)-2082844800;  // get the time base onto UNIX time


    fscanf(fp,"%lf ",&FreqCenter);

 // read the first header to get all the pertinent info
 //   cout<<FreqCenter<<"\n";

    fscanf(fp,"%lf ",&FreqStep);
    fscanf(fp,"%lf ",&ScanPoints);
    IntScanPoints = int(ScanPoints); // need this for the memory allocation of array
    fscanf(fp,"%lf ",&ScanNumber);
// additional data block
    fscanf(fp,"%lf ",&Temperature);
    fscanf(fp,"%lf ",&ControllerV);
    fscanf(fp,"%lf ",&TuneV);
    fscanf(fp,"%lf ",&Offset);

    cout<< FreqCenter <<"  "<< FreqStep << " "<< ScanPoints <<" "<< ScanNumber << "\n";

// histo limits
 Double_t MinFreq = FreqCenter - (ScanPoints-1)/2 * FreqStep;
 Double_t MaxFreq = FreqCenter + (ScanPoints-1)/2 * FreqStep;

 // Create tree and histos


 	 MakeTree();



  	  NMR1 = new TH1D("NMR1","first NMR spectrum",ScanPoints,MinFreq,MaxFreq);

// Now scan over the ascii file until we hit the end
 rewind(fp);  // get to the beginning of the file

 //$$$$$$$
 rewind(fp);  // get to the beginning of the file
 while (1) {
	      //Int_t ncols = fscanf(fp,"%[^\n]\n",line);
	      Int_t ncols = fscanf(fp,"%s",timec1);
	      if (ncols < 0) break; // only the first
	      NumberOfSpectra++;
	      fscanf(fp,"%s",timec2);
	      time1=string(timec1);
	      time2=string(timec2);
	      // now remove the . if there is one, concatenate the times, we have now down to 100 musecs involved
	      Int_t point_pos2 = time2.find(".");
	      Int_t point_pos1 = time1.find(".");
	      time2.erase(0,point_pos2+1);
	      time1.erase(point_pos1); // problem due to stupid string read in stripped it at 10 position
	      std::string time_help= time1+time2;
	      timel = std::stol(time_help)-2082844800; // Time base converted to UNIX time
	      timel = timel - 2082844800;
	      //cout<<timel<<" timel  \n";





       fscanf(fp,"%lf ",&FreqCenter);
       FreqCenter = FreqCenter;
    // three more headerlines
      fscanf(fp,"%lf ",&FreqStep);
      fscanf(fp,"%lf ",&ScanPoints);
      fscanf(fp,"%lf ",&ScanNumber);
      IntScanPoints = int(ScanPoints); // need this for the memory allocation of array

//	        printf("%lf %d \n",ScanNumber,NumberOfSpectra);
	      fscanf(fp,"%lf ",&Temperature);
	      fscanf(fp,"%lf ",&ControllerV);
	      fscanf(fp,"%lf ",&TuneV);
	      fscanf(fp,"%lf ",&Offset);
	      // cout<< FreqCenter <<"  "<< FreqStep << " "<< ScanPoints <<" "<< ScanNumber << "\n";

      for(Int_t loop = 0; loop<ScanPoints ; loop++){
      	fscanf(fp," %lf",&Amplitude);

      	array.push_back(Amplitude);  //put signal into vector      	cout<<array.at(loop)<<" arra \n";

      	Amplitude = Amplitude * sign; // to take care of negative signal

      	NMR1->Fill(MinFreq+loop*FreqStep,Amplitude+BaseLineOffset);  // add an offset to correct for negative values
      	}
        NMRtree->Fill();
  } // end of while
 // free up the space of array

if(!MYDEBUG){
 NMRtree->Write();
}
else
{
 RootFile->Write();
}
// RootFile->Close();








// Consistency check
 if (NumberOfSpectra != ScanNumber) {
	   cout << "read in spectra :"<<NumberOfSpectra << "    stored spectra :"<< ScanNumber<<"\n";
 }
 Norm = FreqStep/NumberOfSpectra;
 cout<<"Norm "<<Norm<<"\n";
 // here we average spectrum over number of sweeps
 cout<<"averaging spectra \n";

 NMR1->Scale(1./NumberOfSpectra);

 return NumberOfSpectra;
}

TString ReadFile::GetDate(TString input) {


	TString timestring = input;
    time_t time_test = timestring.Atoi()-2082844800; // calculated with offset since stupid labview uses jan-1-1904
    tm *ltm = localtime(&time_test);          //and unix uses jan-1-1970

    cout<<" \n \n ******************************************\n\n";
    cout << "Year: "<< 1900 + ltm->tm_year << endl;
       cout << "Month: "<< 1 + ltm->tm_mon<< endl;
       cout << "Day: "<<  ltm->tm_mday << endl;
       cout << "Time: "<< 1 + ltm->tm_hour << ":";
       cout << 1 + ltm->tm_min << ":";
       cout << 1 + ltm->tm_sec << endl;

      cout<<asctime(ltm)<< " \n";
      cout<<" \n \n ******************************************\n\n";

      return  asctime(ltm);
}
int ReadFile::DrawHisto(TString HistoTitle){
	// this routine should handle the drawings
	myCanvas = new TCanvas("myCanvas","NMR",100,100,800,600);
	NMR1->SetTitle(HistoTitle);
	NMR1->Draw();
	myCanvas->Update();
	return 0;
}
int ReadFile::ScaleHisto(Double_t scale){
	// to scale histo grams, this will allow for changing of Qcurve subtraction
	NMR1->Scale(scale);
			return 0;
}
int ReadFile::WriteROOT(TString outfile){
	//this opens a rootfile and writes content to this file
	TFile *rofile = new TFile(outfile,"RECREATE");
	NMR1->Write();
	rofile->Close();
	return 0;
}







#endif /* ReadFile_cxx */
