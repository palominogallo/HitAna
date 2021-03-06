#ifndef HITANA_HH
#define HITANA_HH
#include <EDepSim/TG4Event.h>
#include <EDepSim/TG4Trajectory.h>
#include "ROOTinclude.hh"

class HitAna
{

 	public:
		//! Default Constructor
		HitAna( TTree *tree );

		//! Destructor
		virtual ~HitAna();

	public:
		TTreeReader			mReader;
		Long64_t			mNevents;

		// List of branches
		TTreeReaderValue<TG4Event> Event = {mReader, "Event" };

		Int_t GetEntry( Long64_t entry );
		inline Long64_t GetEntries(){ return mNevents; };
};

#endif
