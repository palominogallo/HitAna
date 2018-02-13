#include "hitana/HitAna.hh"

//=================================================================
// BasicHistos
//
//=================================================================
int BasicHistos( TString infile, TString outfile )
{
	// Create file to store histograms
	TFile *rootfile = new TFile( outfile,"RECREATE");
	TH1F *htest = Utils1::bookTH1F( "htest", "Muons;Deposit Energy / Kinetic Energy;Events / ;units", 40, 0, 2, true);

	// TChain to retrieve data
	TChain *chitana = new TChain("EDepSimEvents");
	chitana->Add(infile);
	HitAna *hitana = new HitAna( chitana );
	Long64_t nevents = hitana->GetEntries();

	// Loop
	for ( Int_t index = 0; index < nevents; index++)
	//for ( Int_t index = 0; index < 10; index++)
	{
		hitana->GetEntry(index);
		TLorentzVector lvector = hitana->Event->Trajectories[0].InitialMomentum;

//#define printBasic
#ifdef printBasic
		cout << hitana->Event->Trajectories[0].PDGCode << " Momentum 0 = "; 
		cout << sqrt(pow(lvector.E(),2) - pow(105.6583745,2)) << endl;
		cout << "Momentum 1: " << sqrt( pow(lvector.Px(),2)+pow(lvector.Py(),2)+pow(lvector.Pz(),2) ) << endl;
		cout << " Energy : " << lvector.E() << endl; 
		cout << " Kin Energy : " << lvector.E() - 105.6583745 << endl; 
#endif

		double totalE = 0;
		double kinEnergy = lvector.E() - 105.6583745;
		for ( auto sd: hitana->Event->SegmentDetectors )
		{
			for ( auto hitsegment: sd.second )
			{
				totalE += hitsegment.EnergyDeposit;
			}
		}
		htest->Fill( totalE / kinEnergy );

	}

	rootfile->Write();
	rootfile->Close();

	return 0;

}

//=================================================================
// showHelp
//=================================================================
void showHelp( TString program_name )
{
	cout<<"Usage:		"<< program_name <<" [-option] [argument]" << endl;
	cout<<"option:		"<<"-i input directory " << endl;
	cout<<"option:		"<<"-o output directory "<< endl;
	cout<<"example:		"<< program_name <<"-i <input directory> -o <plot output directoy >" <<endl;
	exit(-1);
}

//=================================================================
// main
//
//=================================================================
int main(int argc, char **argv)
{
	TString infile;
	TString outfile;

	if ( argc == 1 ) showHelp( TString(argv[0]) );
	int opt;
	while ( ( opt = getopt( argc, argv, "i:o:") ) != -1 )
	{
		switch(opt)
		{
 			case 'i': infile = TString(optarg); break;
			case 'o': outfile = TString(optarg); break;
			default  : showHelp(argv[0]);
		}
	}
	return BasicHistos(infile, outfile);
}
