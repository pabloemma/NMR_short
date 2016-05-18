/*
 * vector_eample.cpp
 *example of crating a vector and iterating over it
 *example
 *  Created on: Feb 12, 2016
 *      Author: klein
 */


	// define vector of char
	std::vector<std::string> coll;
	std::string RXBUFFER;
	char testchar[50]={"andi"};
	testchar[4]= '\n';
	cout<<testchar[0]<<testchar[3]<<testchar[4];
	cout<<testchar[0]<<testchar[3];


// little vector test
		for(Int_t k=0;k<10;k++){
			RXBUFFER = "asdfghjklq";

			coll.push_back(RXBUFFER);
		}
		// now check the length of the vector
		cout<<"vector size "<<coll.size()<<"\n";
		// now rpint the vecor out
		for (std::vector<std::string>::iterator pos=coll.begin(); pos!= coll.end();pos++) cout<<"vector "<<*pos<<"\n";
		// empty vector
		coll.clear();
		cout<<"vector size after clear "<<coll.size()<<"\n";

		//Loop over character array until we find linefeed
		for(int l =0; l<50;l++){
			cout <<testchar[l];
			if(testchar[l]=='\n') break;
		}

