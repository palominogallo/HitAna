#include "hitana/HitAna.hh"
#include <iostream>

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
	TString partstring, inpartstring;

	// Loop
	for ( Int_t index = start; index < end; index++)
	{
		hitana->GetEntry(index);

// Primaries
		cout << hitana->Event->Primaries.size()  << endl;
		for ( auto vert: hitana->Event->Primaries )
		{
			
			for ( auto invert: vert.Informational )
      {	
				inpartstring.Clear();
				for ( auto inpart: invert.Particles )
				{
        	inpartstring += inpart.Name + Form( ":%d:%.2f; ", inpart.TrackId,inpart.Momentum.E() );
				}
				cout << inpartstring.Data() << endl;
      }
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
#define PRINTINFO_TRAJECTORIES 
#ifdef PRINTINFO_TRAJECTORIES
		cout << endl << "** Printing Trajectories **" << endl;
		for ( auto traj: hitana->Event->Trajectories )
		{
			cout << " Part = " << setw(10) << traj.Name;
			cout << "\t TrackId = " << setw(3) << traj.TrackId;
			cout << "\t ParentId = " << setw(3) << traj.ParentId;
			cout << "\t Process = " << setw(2) << traj.Points[0].Process;
			cout << "\t E = " << setw(9) << Form( "%.2F", traj.InitialMomentum.E() );
			//cout << "\t X = " << setw(10) << Form( "%.3F", traj.Points[0].Position.X() );
			//cout << "\t Y = " << setw(10) << Form( "%.3F", traj.Points[0].Position.Y() );
			//cout << "\t Z = " << setw(10) << Form( "%.3F", traj.Points[0].Position.Z() );
			cout << endl;
		}
#endif

// Segments
#define PRINTINFO_SEGMENTS
#ifdef PRINTINFO_SEGMENTS
		cout << endl << "** Printing SegmentDetectors **" << endl;
		for ( auto sd: hitana->Event->SegmentDetectors )
		{
			//cout << sd.first << endl;
			for ( auto hit: sd.second )
			{
				cout << " Contrib Id = " << hit.Contrib[0] <<  " Primary Id = " << hit.PrimaryId;
				cout << " Energy deposit = " << hit.GetEnergyDeposit() << endl;
			}
		}
#endif

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
