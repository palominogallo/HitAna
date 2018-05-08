#include "hitana/HitAna.hh"
#include <TGeoManager.h>
#include <TGeoNode.h>
#include <TGeoVolume.h>

bool muIsContained( TGeoManager *gm,  std::vector<TG4TrajectoryPoint> muPoints, TString volname );

//=================================================================
// MuContainedHistos
//
//=================================================================
int MuContainedHistos( TString infile, TString outfile )
{
	// Create file to store histograms
	TFile *rootfile = new TFile( outfile,"RECREATE");
	TH1F *htest = Utils1::bookTH1F( "htest", "Muons;Deposit Energy / Kinetic Energy;Events / ;units", 40, 0, 2, true);

	// TChain to retrieve data
	TChain *chitana = new TChain("EDepSimEvents");
	chitana->Add(infile);
	HitAna *hitana = new HitAna( chitana );
	Long64_t nevents = hitana->GetEntries();

	TFile *readFile = new TFile(infile);
	TGeoManager* gm = (TGeoManager*)readFile->Get("EDepSimGeometry");
  //gm->Import(infile);

	Int_t muId = 0;
	Double_t muE = 0, muEk, TotalE;
	TString muName;
	std::vector<TG4TrajectoryPoint> muPoints;

	// Loop
	for ( Int_t index = 0; index < nevents; index++)
	{
		hitana->GetEntry(index);

		TString reaction( hitana->Event->Primaries[0].GetReaction() );
		if ( !reaction.Contains("[CC]") ) continue;

		TLorentzVector lposition = hitana->Event->Primaries[0].GetPosition();
		TGeoNode* node = gm->FindNode(lposition.X(), lposition.Y(), lposition.Z() );
		if ( TString(node->GetName()) != "volLArActive_PV_0" ) continue;


		for ( auto traj: hitana->Event->Trajectories )
    {
			muName = TString(traj.Name);
			if ( !muName.Contains("mu") ) continue;
			if ( traj.GetParentId() != -1 ) continue; 
			muId = traj.TrackId;
			muE = traj.InitialMomentum.E();
			muEk = muE - 105.6583745;
			muPoints = traj.Points;
			break;
    }

		if ( hitana->Event->SegmentDetectors.size() > 1 ) continue;

		if ( !muIsContained( gm, muPoints, "volArgonCubeActive_PV" ) ) continue;
		
		TotalE = 0;
		for ( auto sd: hitana->Event->SegmentDetectors )
    {
      for ( auto hit: sd.second )
      {
				if ( hit.GetPrimaryId() !=  muId ) continue;
				TotalE += hit.GetEnergyDeposit();
      }
    }

		cout << muName << "; RATIO " << TotalE / muEk << "; Total E deposit = " << TotalE << " " << index << endl;

	}

	rootfile->Write();
	rootfile->Close();

	return 0;

}

//=================================================================
// muIsContained
//=================================================================
bool muIsContained( TGeoManager *gm,  std::vector<TG4TrajectoryPoint> muPoints, TString volname )
{
	TString pathname;
  pathname = "/volWorld_PV_1/volDetEnclosure_PV_0/volArgonCubeDetector_PV_0/volArgonCubeCryostat_PV_0/";
	pathname += "volReinforcedConcrete_PV_0/volMoistureBarrier_PV_0/volInsulationBoard2_PV_0/";
	pathname += "volGREBoard2_PV_0/volInsulationBoard1_PV_0/volGREBoard1_PV_0/volFireproofBoard_PV_0/";
	pathname += "volSSMembrane_PV_0/volArgonCubeService_PV_0/volArgonCube_PV_0/volArgonCubeActive_PV_0";	

	//if ( gm->CheckPath(pathname) ) 
	gm->cd(pathname);

	TGeoVolume *vol = gm->GetVolume(volname);	
	TGeoMatrix *active = gm->GetCurrentMatrix();
	for ( auto point: muPoints )
	{
		TLorentzVector pointPos = point.GetPosition();
  	TGeoNode *pointNode = gm->FindNode(pointPos.X(), pointPos.Y(), pointPos.Z() );	
			//cout << pointNode->GetName() << " " << gm->IsSameLocation( pointPos.X(), pointPos.Y(), pointPos.Z() ) << endl;
		double local_muon[3]= { 0, 0, 0 };
		double master_muon[3] = { pointPos.X(), pointPos.Y(), pointPos.Z() };
		active->MasterToLocal(master_muon,local_muon);
			//cout << pointNode->GetName() << " " << vol->Contains(local_muon) << endl;	
		if( !vol->Contains(local_muon) ) return false;
	}
	return true;

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
	return MuContainedHistos(infile, outfile);
}
