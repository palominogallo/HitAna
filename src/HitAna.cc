#ifndef HITANA_CC
#define HITANA_CC

#include "HitAna.hh"

//=================================================================
// Constructor
//=================================================================
HitAna::HitAna( TTree *tree )
{
	mReader.SetTree(tree);
	mNevents = mReader.GetEntries( kTRUE );
}

//=================================================================
// Destructor
//=================================================================
HitAna::~HitAna()
{
}

//=================================================================
// GetEntry
//=================================================================
Int_t HitAna::GetEntry( Long64_t entry )
{
	return mReader.SetEntry( entry );
}

#endif