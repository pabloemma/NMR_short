//============================================================================
// Name        : ReadNMR.cpp
// Author      : andi klein
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#define ReadFile_cxx  // first define the cxx part


#include <unistd.h>  // to get gnu getoption
#include <ctime> // to manipulate time
#include <stdlib.h>
#include  <vector>

#include <iostream>
#include "ReadFile.h"
#include "ReadNMR.h"
#include "Analysis.h"
#include "TString.h"
#include "TApplication.h"



#ifdef __APPLE__
 std::string OS = "OSX";
#elif __linux__
 std::string OS = "OSX";
 #elif _WIN64
 std::string OS = "WIN";
 // add windows includes
#elif _WIN32
 std::string OS = "WIN";
 // add windows includes
#endif





ReadFile RF; // instantiate the ReadFile Class for the regular spectrum
ReadFile Bck; // instantiate for the background
Analysis Ana; // instantiate the analysis class


using namespace std;

#ifdef __APPLE__
int main(Int_t argc,char **argv) {
#endif

#ifdef __linux__
int main(Int_t argc,char **argv) {
#endif

#ifdef __WIN64
int main(Int_t argc,char *argv[],char *envp[] ) {
#endif

#ifdef __WIN32
int main(Int_t argc,char *argv[],char *envp[] ) {
#endif



	// root needs this to run interactively
//	TApplication *theApp = new TApplication("theApp",argc,argv);
//	theApp->GetOptions(&argc,argv);

    cout<<"argument count"<<argc<<"\n";
	cout << "NMR program" << endl; // prints !!!Hello World!!!
	// default stuff
	signal_sign=1; //we assume positive signal
	Double_t scale = 1.; // assume no scaling
	globalArgs.outfile = "";




	if(OS.compare("OSX")==0) { // Start OSX block
        int c;


    //Do not use b option since that suppresses root application drawing
	while((   c = getopt(argc,argv,"f:t:k:i:z:y:s:u:o:h")) != -1){
		cout<<char(c)<<"\n";
		switch (c)

		{
			case 'f' :
				cout<<"optarg"<<string(optarg)<<"\n";
				globalArgs.FitFunction = string(optarg) ;
				cout<<" FitFunction chosen  " << globalArgs.FitFunction <<"\n";


				break;
			case 't' :
				globalArgs.title = optarg ;
				cout<<" Title "<<globalArgs.title<<"\n";

				break;

			case 'i' :
				globalArgs.input = optarg ;
				datestring = globalArgs.input;
				cout<<" Input file "<<globalArgs.input<<"\n";

				break;
			case 'z' :
				globalArgs.background = string(optarg) ;
				bck = true;
				cout<<" Background run "<<globalArgs.background<<"\n";
				break;
			case 'y' :
				globalArgs.uvafile = string(optarg) ;
				UVa = true;
				cout<<" UVafile "<<globalArgs.uvafile<<"\n";

				break;
			case 's' :
				globalArgs.sign = string(optarg) ;
				cout<<" sign "<<globalArgs.sign<<"\n"; // can be either pos or neg
				if(globalArgs.sign == "neg") signal_sign=-1;

				break;
			case 'k' :
				cout<<"test option \n";
				globalArgs.data_directory = optarg ;
				cout<<" Data Directory "<<globalArgs.data_directory<<"   opt arg "<<optarg<<"\n";

				break;
			case 'o' :
				cout<<"outputfile \n";
				globalArgs.outfile = optarg ;
				cout<<" outputfile "<<globalArgs.outfile<<"   opt arg "<<optarg<<"\n";

				break;
			case 'u' :
				cout<<"scale \n";
				globalArgs.scale = optarg ;
				cout<<"scale "<<globalArgs.scale<<"   opt arg "<<optarg<<"\n";
				scale = std::stod(globalArgs.scale); //convert from string to double
				break;

			case '?' :
				fprintf(stderr,"unknown option `\\x%x'.\n",optopt);
				fprintf(stderr,"use -f  gaus,lor\n");
				break;
			default:
				globalArgs.FitFunction = "gaus";
				break;
		}

	}

	for(Int_t index = optind; index < argc ; index++){
		printf ("argument %s\n",argv[index]);
	}


    if(globalArgs.data_directory != "") {
   	directory = "/"+globalArgs.data_directory; // since getopt cannot handle forward slash
    }
    else {
    	 directory = "/Users/klein/NMR/pol_measurements/";
    }

    TString file_start ="NMR";

     in_src = directory+file_start+globalArgs.input;

     outputfilename = in_src+ outputend;
     inputfilename = in_src + inputend;
     rootfile = in_src + rootend; //this is the rootfile containg the tree

	 input_NMR_filename =inputfilename;





    } // end OSX block

if(OS =="WIN") {
    int count;

    // Display each command-line argument.
    cout << "\nCommand-line arguments:\n";
    for( count = 0; count < argc; count++ )
    {
    	cout << "  argv[" << count << "]   "
                << argv[count] << "\n";
    	if(argv[count] == "-i") input_NMR_filename = argv[count+1];
       	if(argv[count] == "-f") globalArgs.FitFunction = argv[count+1];
       	if(argv[count] == "-s") globalArgs.sign= argv[count+1];

    }



}// end WIN

// deal with the global arguments
		if(globalArgs.sign == "neg") signal_sign=-1;




	//input_NMR_filename = "/Users/klein/NMR/pol_measurements/NMR3533139761.xls"; // read in the string, stupid C++ needs double quote
        Int_t *argc1;
		TApplication *theApp = new TApplication("theApp",argc1,argv);  // problem with the two commandline args getting in conflict
		theApp->GetOptions(&argc,argv);
		if(MYDEBUG){
				cout <<"this is the input file    "<<input_NMR_filename<<" \n";
				cout<<" this is the rootfile for the tree  "<<rootfile<<"\n\n\n";
		}

	RF.OpenFile(input_NMR_filename,rootfile); // pass filename as a const char *

    RF.GetDate(datestring);  // returns exact date of run from name

    RF.ReadData(signal_sign);  // read the different spectra and put them into ROOT histo

    RF.DrawHisto("Foreground"); // draw histo from ReadDAta

    RF.CloseFile();

    //theApp->Run();

    theApp->Run();

    theApp->Terminate();
    return 0;

;
}
