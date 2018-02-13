#include "hitana/HitAna.hh"

//=================================================================
// PrintInfoHistos
//
//=================================================================
int PrintInfoHistos( TString infile, Int_t start, Int_t end )
{
	// TChain to retrieve data
	TChain *chitana = new TChain("EDepSimEvents");
	chitana->Add(infile);
	HitAna *hitana = new HitAna( chitana );
	Long64_t nevents = hitana->GetEntries();
	TString partstring;

	// Loop
	for ( Int_t index = start; index < end; index++)
	{
		hitana->GetEntry(index);
		if ( index % 10000 == 0 ) cout << " Running " << index << " events " << endl;

// Primaries
		for ( auto vert: hitana->Event->Primaries )
		{
			//if ( !TString(vert.Reaction).Contains("nu:12") ) continue;
			//if ( !TString(vert.Reaction).Contains("COH;") ) continue;
			//if ( !TString(vert.Reaction).Contains("nu:14") ) continue;
			//if ( !TString(vert.Reaction).Contains("NuEEL;") ) continue;
			//cout << " Primaries = " << vert.Reaction << endl;
			partstring.Clear();
			for ( auto part: vert.Particles )
			{		
				partstring += part.Name + Form( ":%d:%.2f; ", part.TrackId,part.Momentum.E() );
			}
//			if ( partstring.Contains("pi0") ) 
//			{
				cout << "Entry = " << index << "; Reaction = " << vert.Reaction << endl;
				cout << "  " << partstring.Data() << endl;
//			}	

		}		

// Trajectories
/*
		for ( auto traj: hitana->Event->Trajectories )
		{
			cout << " Particle = " << traj.Name << " TrackId = " << traj.TrackId << " ParentId = ";
      cout << traj.ParentId << " Process = " << traj.Points[0].Process << " E = ";
      cout << traj.InitialMomentum.E() <<  " X = " << traj.Points[0].Position.X() << " Y = ";
      cout << traj.Points[0].Position.Y() <<  " Z = " << traj.Points[0].Position.Z() << endl;
		}


// Segments

		for ( auto sd: hitana->Event->SegmentDetectors )
		{
			//cout << sd.first << endl;
			for ( auto hit: sd.second )
			{
				cout << " Contrib Id = " << hit.Contrib[0] <<  " Primary Id = " << hit.PrimaryId << endl;
			}
		}
*/

	}

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
	Int_t start, end;

	if ( argc == 1 ) showHelp( TString(argv[0]) );
	int opt;
	while ( ( opt = getopt( argc, argv, "i:s:e:") ) != -1 )
	{
		switch(opt)
		{
 			case 'i': infile = TString(optarg); break;
			case 's': start = atoi(optarg); break;
			case 'e': end = atoi(optarg); break;
			default  : showHelp( argv[0] );
		}
	}
	cout << infile << " "<< start << " " << end << endl;
	return PrintInfoHistos( infile, start, end );
}
