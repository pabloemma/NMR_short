/*
 * ReadNMR.h
 *
 *  Created on: Jan 19, 2016
 *      Author: klein
 */

#ifndef READNMR_H_
#define READNMR_H_

	std::string input_file_str;  // the original NMR file
	const char *input_file ; // character array for file open




	struct globalArgs_t {
		std::string FitFunction; /* -f option selects peak signal function*/
		std::string title; /* -t gives title */
		std::string input; /* -i gives input filename (only the digits)*/
		std::string data_directory; /* data direcory string */
		std::string uvafile; // if we deal with a UVa run: give the full path
		std::string sign; // if we deal with absorpion signal
		std::string background ; /* indicating his to be a backgroun run */
		std::string scale ;
		std::string outfile ;  //root format outputfile
		std::string dummy; // for batch argument

	}globalArgs ; // currently only the gaus or lorentzian, defaults to gauss

	Int_t c;
	Int_t loop;
	Int_t read_control;
	Bool_t bck;
	Bool_t batch; // for batch operation
	Bool_t UVa; // to control that we have a UVa file
	Int_t signal_sign; // positive or negative
	TString datestring; // part of the nmr file name which contains the time

	// TString block which has to deal with the filename manipulation
	//TString inputfile ="C:\\NMR" ; // beginning of filename
	TString inputfile ="NMR" ; // beginning of filename
	TString directory; // string for data directory
	TString inputdate ; // the part of the inputfile which is 10 charactesr and conatins the date and time
	TString inputend =".csv";
	TString inputfilename; // inpiut for foreground spectrum
	TString in_src = ""; // the filename without extension but directory etc
	TString inputfilename_bck; // inpiut for background spectrum

	TString outputfilename;
	TString outputend =".dat"; //file with result parameter from this fitting
	TString rootend = ".root"; //Root file of the NMR spectra
	TString rootfile ;
	TString input_NMR_filename ;
	TString output_root_filename ;
	TString output_NMR_filename ;


	TString FitForm= "gaus";





#endif /* READNMR_H_ */
