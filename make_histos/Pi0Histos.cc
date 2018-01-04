#include "hitana/HitAna.hh"
std::vector<pair<int,float>> getGammasVec( std::vector<int> pi0idvec, HitAna *hitana);
TVector3 getPairProPosition( int id,  HitAna *hitana );
bool isFV( TVector3 vertex );

//=================================================================
// Pi0Histos
//
//=================================================================
int Pi0Histos( TString infile, TString outfile )
{
	// Create file to store histograms
	TFile *rootfile = new TFile( outfile,"RECREATE");
	TH1F *hvtx = new TH1F("hvtx","Vertex;X (cm)", 400,-2000, 2000);
	TH1F *hvty = new TH1F("hvty","Vertex;Y (cm)", 400,-1500, 2500);
	TH1F *hvtz = new TH1F("hvtz","Vertex;Z (cm)", 400, 9000,13000);
	enum Color{ kAll, kRES, kQES, kDIS };
	TString inter[4] = {"All","RES","QES","DIS"};
	TH2F *hcont1[4];
	TH2F *hcont2[4];
	for ( int i = 0; i < 4; i++ )
	{
	 hcont1[i] = new TH2F(Form("hcont1%d",i),inter[i]+" - Containment > 0.;N^{0} rad len;Angle",100,0,20,90,0,180);
	 hcont2[i] = new TH2F(Form("hcont2%d",i),inter[i]+" - Containment > 0.95;N^{0} rad len;Angle",100,0,20,90,0,180);
	}

	// TChain to retrieve data
	TChain *chitana = new TChain("EDepSimEvents");
	chitana->Add(infile);
	HitAna *hitana = new HitAna( chitana );
	Long64_t nevents = hitana->GetEntries();
	TString partstring;
	std::vector<int> pi0idvec;
	std::vector<pair<int,float>> gammasvec; // to store id and energy
	TVector3 vertex;
	TVector3 beamdirection(0,-76,749); //double check this information
	beamdirection = beamdirection.Unit();
	double angle;

	// Loop
	for ( Int_t index = 0; index < nevents; index++)
	//for ( Int_t index = 0; index < 20000; index++)
	//for ( Int_t index = 38; index < 39; index++)
	//for ( Int_t index = 0; index < 100; index++)
	{
		hitana->GetEntry(index);
		if ( index % 10000 == 0 ) cout << " Running " << index << " events " << endl;
		vertex = hitana->Event->Primaries[0].Position.Vect();
		TString reaction = TString(hitana->Event->Primaries[0].Reaction);
		hvtx->Fill( vertex.X() );
		hvty->Fill( vertex.Y() );
		hvtz->Fill( vertex.Z() );
		if ( isFV( vertex ) ) continue;
		//cout << " vtx X = " << vertex.X()  << " vtx Y = " << vertex.Y() << " vtx Z =  " << vertex.Z() << endl;

		partstring.Clear();
		pi0idvec.clear();
		gammasvec.clear();
		for ( auto part: hitana->Event->Primaries[0].Particles )
		{
				if ( part.Name  == "pi0" ) pi0idvec.push_back(part.TrackId);		
				partstring += part.Name + Form( ":%d:%.2f; ", part.TrackId,part.Momentum.E() );
		}
		
		if ( !pi0idvec.size() )  continue;
		//cout << "Entry = " << index << "; Reaction = " << hitana->Event->Primaries[0].Reaction << endl;
		//cout << "  " << partstring.Data() << endl;
		gammasvec = getGammasVec( pi0idvec, hitana );
		for ( auto gamma: gammasvec )
		{
			Float_t totalE = 0, distance = 0;
			
			TVector3 pairpropos = getPairProPosition( gamma.first, hitana );	
			TVector3 gammadirection = pairpropos - vertex;
			gammadirection = gammadirection.Unit();

			for ( auto hit: hitana->Event->SegmentDetectors["3DScint"] )
			{
				if ( gamma.first != hit.PrimaryId ) continue;
				totalE += hit.EnergyDeposit;
				float temp = (hit.Stop.Vect() - pairpropos).Dot(gammadirection);
				if ( distance < temp ) distance = temp;
			}
			//cout << totalE/gamma.second << " " << distance << endl;
			if ( totalE/gamma.second > 0.) 
			{
				angle = 180*beamdirection.Angle(gammadirection)/TMath::Pi();
				hcont1[kAll]->Fill( distance/430, angle ); 
				if ( reaction.Contains("QES;") ) 
					hcont1[kQES]->Fill( distance/430, angle ); 
				if ( reaction.Contains("RES;") ) 
					hcont1[kRES]->Fill( distance/430, angle ); 
				if ( reaction.Contains("DIS;") ) 
					hcont1[kDIS]->Fill( distance/430, angle ); 
				
				if ( totalE/gamma.second > 0.95 ) 
				{
					hcont2[kAll]->Fill(distance/430, angle);
					if ( reaction.Contains("QES;") ) 
						hcont2[kQES]->Fill( distance/430, angle ); 
					if ( reaction.Contains("RES;") ) 
						hcont2[kRES]->Fill( distance/430, angle ); 
					if ( reaction.Contains("DIS;") ) 
						hcont2[kDIS]->Fill( distance/430, angle ); 
				}
			}

		}

	}
	rootfile->Write();
	rootfile->Close();

	return 0;

}

//=================================================================
// getGammaIdVec
//=================================================================
std::vector<pair<int,float>> getGammasVec( std::vector<int> pi0idvec, HitAna *hitana)
{
	std::vector<pair<int,float>> gammasvec;
	for ( auto pi0id: pi0idvec) 
	{
		int count = 0;
		for ( auto traj: hitana->Event->Trajectories )
		{
			if ( pi0id != traj.ParentId ) continue;
			pair<int,float> temp(traj.TrackId,traj.InitialMomentum.E());
			gammasvec.push_back(temp) ;
			count++;
			if ( count == 2 ) break;
		}
	}

	return gammasvec;
}

//=================================================================
// getShowerPosition
//=================================================================
TVector3 getPairProPosition( int id,  HitAna *hitana )
{
	TVector3 pairpropos;
	for ( auto traj: hitana->Event->Trajectories )
  {
    if ( id != traj.ParentId ) continue;
		pairpropos = traj.Points[0].Position.Vect();
  }
	return pairpropos;
}

//=================================================================
// isFV
//=================================================================
bool isFV( TVector3 vertex )
{
	if ( !(vertex.X() > -1200+430) ) return false;
	if ( !(vertex.X() <  1200-430) ) return false;
	if ( !(vertex.Y() > -680 +430) ) return false;
	if ( !(vertex.Y() <  1720-430) ) return false;
	if ( !(vertex.Z() >  9560+430) ) return false;
	if ( !(vertex.Z() < 11560-430) ) return false;
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
	return Pi0Histos(infile, outfile);
}
