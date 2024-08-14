// Converter from binary to root for TPX3Cam event mode data
// The binary data (from exportEvent) can be viewed with od
// od -w48 -tf8 filename
// i.e. six eight-byte values (x, y, time, photon count, PSD parameter, null)

int bin2root(TString filename, int debug=0){
	
	// Print extra info
	bool debugOn = 0;
	
	// Append .root to the output file name
	TString outfilename = filename;
	outfilename.Append(".root");
	
	// Create the output file
	TFile *outfile = new TFile(outfilename,"RECREATE");
	TTree *eventdata = new TTree("eventdata","Event data from the TPX3Cam");

	
	// Event variables
	Float_t xpos;
	Float_t ypos;
	Double_t time;
	Short_t nP;
	Float_t PSD;

	// Setup branches and link to variables
	eventdata->Branch("xpos", &xpos, "xpos/F");
	eventdata->Branch("ypos", &ypos, "ypos/F");
	eventdata->Branch("time", &time, "time/D");
	eventdata->Branch("nP", &nP, "nP/S");	// Replace with short int
	eventdata->Branch("PSD", &PSD, "PSD/F");

	// Open input file
	ifstream infile(filename, ios::in | ios::binary);
	if (!filename){
		cerr << "Error opening file: " << filename << endl;
		return 1;
	}
	
	// Get file size
	infile.seekg(0, ios::end);
	streamsize fileSize = infile.tellg();
	infile.seekg(0, ios::beg);
	// Get number of doubles in file
	size_t numValues = fileSize / sizeof(double);

	cout << "file size = " << fileSize << endl;
	cout << "number of values = " << numValues << endl;
	cout << "number of events = " << numValues/6 << endl;

	// Read all values into a vecotr of doubles
	vector<double> values(numValues);
	if (infile.read(reinterpret_cast<char*>(values.data()), fileSize)) {
		for (size_t i = 0; i < numValues; i=i+6) {
			xpos = Float_t(values[i]);
			ypos = Float_t(values[i+1]);
			time = values[i+2];
			nP = Short_t(values[i+3]);
			PSD = Float_t(values[i+4]);
		
			eventdata->Fill();
			// Print event for debugging //
			if(debugOn){
				cout << "xpos: " << xpos << endl;
				cout << "ypos: " << ypos << endl;
				cout << "time: " << time << endl;
				cout << "nP: " << nP << endl;
				cout << "PSD: " << PSD << endl;
			}	
		}
	}

	// Close files
	outfile->Write();
	infile.close();
	outfile->Close();

	return 0;
}
