//============================================================================
///============================================================================
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
#include <stdio.h>
// this is git test


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
	while((   c = getopt(argc,argv,"f:t:d:i:z:y:s:u:b:o:h")) != -1){
		cout<<char(c)<<"\n";
		switch (c)

		{


				break;
			case 't' :
				globalArgs.title = optarg ;
				cout<<" Title "<<globalArgs.title<<"\n";

				break;

			case 'i' :
				globalArgs.input = optarg ;
				//datestring = globalArgs.input;
				cout<<" Input file "<<globalArgs.input<<"\n";

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
			case 'd' :
				cout<<"test option \n";
				globalArgs.data_directory = optarg ;
				cout<<" Data Directory "<<globalArgs.data_directory<<"   opt arg "<<optarg<<"\n";

				break;
			case 'z' :
				cout<<"sleep time \n";
				globalArgs.sleep_time = optarg ;
				cout<<" sleep time "<<globalArgs.sleep_time<<"   opt arg "<<optarg<<"\n";
				sleepy = atoi(globalArgs.sleep_time.c_str());

				break;
			case 'o' :
				cout<<"outputfile \n";
				globalArgs.outfile = optarg ;
				cout<<" outputfile "<<globalArgs.outfile<<"   opt arg "<<optarg<<"\n";

				break;
			case 'b' :
				cout<<"batch operation \n";
				globalArgs.dummy = optarg;
				batch = true ;
				cout<<" We are doing batch operation \n";

				break;
			case 'u' :
				cout<<"scale \n";
				globalArgs.scale = optarg ;
				cout<<"scale "<<globalArgs.scale<<"   opt arg "<<optarg<<"\n";
				scale = std::stod(globalArgs.scale); //convert from string to double
				break;

			case '?' :
				////fprintf(stderr,"unknown option `\\x%x'.\n",optopt);
				//fprintf(stderr,"use -f  gaus,lor\n");
				cout<<" You probaly want to use -d for the input data directory and -i for the file "<<endl;
				cout<<" I leave now so you can figure it out"<<endl;
				exit(1);
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

     //TString file_start ="TEQ";

    //in_src = directory+file_start+globalArgs.input;
    in_src = directory+globalArgs.input;

     outputfilename = in_src+ outputend;
     inputfilename = in_src + inputend;
     rootfile = directory+"/root/"+globalArgs.input + rootend; //this is the rootfile containg the tree

	 input_NMR_filename =inputfilename;





    } // end OSX block


// deal with the global arguments
		if(globalArgs.sign == "neg") signal_sign=-1;




	//input_NMR_filename = "/Users/klein/NMR/pol_measurements/NMR3533139761.xls"; // read in the string, stupid C++ needs double quote
        Int_t *argc1;

// set batch to true
        batch=true;

        TApplication *theApp = new TApplication("theApp",&argc,argv);  // problem with the two commandline args getting in conflict


//		theApp->GetOptions(&argc,argv);

		if(MYDEBUG){
				cout <<"this is the input file    "<<input_NMR_filename<<" \n";
				cout<<" this is the rootfile for the tree  "<<rootfile<<"\n\n\n";
		}

	RF.OpenFile(input_NMR_filename,rootfile); // pass filename as a const char *


	// Create date string from input name in globalArgs.input
	// strip first three characters:
	datestring = globalArgs.input.erase(0,3); //erase 3 characters starting at 0


    RF.GetDate(datestring);  // returns exact date of run from name
    RF.ReadData(signal_sign);  // read the different spectra and put them into ROOT histo


/*    switch (read_control) {
    case 0:
    RF.ReadData(signal_sign);  // read the different spectra and put them into ROOT histo
    break;
    case 1:
    RF.ReadData(signal_sign);  // read the different spectra and put them into ROOT histo
    break;
    }
*/
    if(sleepy != 0){
       RF.DrawHisto("Foreground"); // draw histo from ReadDAta
       // now wait for 5 seconds, to give a chance for the plot
       sleep(sleepy);
    }
    //if(!batch)RF.DrawHisto("Foreground"); // draw histo from ReadDAta

    //RF.CloseFile();

    //theApp->Run();
    if(batch)cout<< "at end \n";
 if(!batch){
	 theApp->Run();
     theApp->Terminate();
 }

 	 RF.CloseFile();
    return 0;

;
}
