// $Id: DEventProcessor_trackeff_hists.cc 2774 2007-07-19 15:59:02Z davidl $
//
//    File: DEventProcessor_trackeff_hists.cc
// Created: Sun Apr 24 06:45:21 EDT 2005
// Creator: davidl (on Darwin Harriet.local 7.8.0 powerpc)
//

#include <iostream>
#include <cmath>
using namespace std;

#include <TThread.h>

#include "DEventProcessor_trackeff_hists.h"

#include <JANA/JApplication.h>
#include <JANA/JEventLoop.h>

#include <DANA/DApplication.h>
#include <TRACKING/DMCThrown.h>
#include <TRACKING/DTrackCandidate.h>
#include <TRACKING/DTrack.h>
#include <FDC/DFDCGeometry.h>
#include <DVector2.h>

// The executable should define the ROOTfile global variable. It will
// be automatically linked when dlopen is called.
//extern TFile *ROOTfile;

// Routine used to create our DEventProcessor
extern "C"{
void InitPlugin(JApplication *app){
	InitJANAPlugin(app);
	app->AddProcessor(new DEventProcessor_trackeff_hists());
}
} // "C"


//------------------
// DEventProcessor_trackeff_hists
//------------------
DEventProcessor_trackeff_hists::DEventProcessor_trackeff_hists()
{
	leaf_ptr = &leaf;
	
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&rt_mutex, NULL);
}

//------------------
// ~DEventProcessor_trackeff_hists
//------------------
DEventProcessor_trackeff_hists::~DEventProcessor_trackeff_hists()
{
	delete ref;
}

//------------------
// init
//------------------
jerror_t DEventProcessor_trackeff_hists::init(void)
{
	// Create TRACKING directory
	TDirectory *dir = new TDirectory("TRACKING","TRACKING");
	dir->cd();

	// Create Tree
	trkeff = new TTree("trkeff","Tracking Efficiency");
	trkeff->Branch("F","TrkEff_Leaf",&leaf_ptr);

	dir->cd("../");
	
	MAX_HIT_DIST_CDC = 1.0; // cm
	MAX_HIT_DIST_FDC = 5.0; // cm

	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t DEventProcessor_trackeff_hists::brun(JEventLoop *loop, int runnumber)
{
	DApplication* dapp = dynamic_cast<DApplication*>(loop->GetJApplication());
	bfield = dapp->GetBfield(); // temporary until new geometry scheme is worked out
	ref = new DReferenceTrajectory(bfield);
	
	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t DEventProcessor_trackeff_hists::erun(void)
{

	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t DEventProcessor_trackeff_hists::fini(void)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t DEventProcessor_trackeff_hists::evnt(JEventLoop *loop, int eventnumber)
{
	CDChitv cdctrackhits;
	FDChitv fdctrackhits;
	vector<const DTrackCandidate*> trackcandidates;
	vector<const DTrack*> tracks;
	vector<const DMCThrown*> mcthrowns;
	
	loop->Get(cdctrackhits);
	loop->Get(fdctrackhits);
	loop->Get(trackcandidates);
	loop->Get(tracks);
	loop->Get(mcthrowns);
	
	// Lock mutex
	pthread_mutex_lock(&mutex);

	// Fill map associating FDC hits with truth points
	FindFDCTrackNumbers(loop);
		
	// Get hit list for all candidates
	vector<CDChitv> cdc_candidate_hits;
	vector<FDChitv> fdc_candidate_hits;
	for(unsigned int i=0; i<trackcandidates.size(); i++){
		CDChitv cdc_outhits;
		GetCDCHits(trackcandidates[i], cdctrackhits, cdc_outhits);
		cdc_candidate_hits.push_back(cdc_outhits);

		FDChitv fdc_outhits;
		GetFDCHits(trackcandidates[i], fdctrackhits, fdc_outhits);
		fdc_candidate_hits.push_back(fdc_outhits);
	}

	// Get hit list for all fit tracks
	vector<CDChitv> cdc_fit_hits;
	vector<FDChitv> fdc_fit_hits;
	for(unsigned int i=0; i<tracks.size(); i++){
		CDChitv cdc_outhits;
		GetCDCHits(tracks[i], cdctrackhits, cdc_outhits);
		cdc_fit_hits.push_back(cdc_outhits);

		FDChitv fdc_outhits;
		GetFDCHits(tracks[i], fdctrackhits, fdc_outhits);
		GetFDCHitsFromTruth(i+1, fdc_outhits);
		fdc_fit_hits.push_back(fdc_outhits);
	}

	// Get hit list for all throwns
	for(unsigned int i=0; i<mcthrowns.size(); i++){
		const DMCThrown *mcthrown = mcthrowns[i];
		
		// if this isn't a charged track, then skip it
		if(fabs(mcthrowns[i]->charge())==0.0)continue;

		CDChitv cdc_thrownhits;
		FDChitv fdc_thrownhits;
		GetCDCHits(mcthrowns[i], cdctrackhits, cdc_thrownhits);
		//GetFDCHits(mcthrowns[i], fdctrackhits, fdc_thrownhits);
		GetFDCHitsFromTruth(i+1, fdc_thrownhits);

		
		leaf.status_can = 0;
		leaf.status_fit = 0;
		if(cdc_thrownhits.size()<5 && fdc_thrownhits.size()<5){
			leaf.status_can--;
			leaf.status_fit--;
		}
		
		leaf.pthrown = mcthrown->momentum();
		leaf.z_thrown = mcthrown->position().Z();
		leaf.ncdc_hits_thrown = cdc_thrownhits.size();
		leaf.nfdc_hits_thrown = fdc_thrownhits.size();
		leaf.ncdc_hits = cdctrackhits.size();
		leaf.nfdc_hits = GetNFDCWireHits(fdctrackhits);
		
		//========= CANDIDATE ========
		// Look for a candidate track that matches this thrown one
		CDChitv cdc_matched_hits;
		FDChitv fdc_matched_hits;
		unsigned int icdc_can = FindMatch(cdc_thrownhits, cdc_candidate_hits, cdc_matched_hits);
		unsigned int ifdc_can = FindMatch(fdc_thrownhits, fdc_candidate_hits, fdc_matched_hits);
		
		// Initialize values for when no matching candidate is found
		leaf.pcan.SetXYZ(0.0, 0.0, 0.0);
		leaf.z_can = -1000.0;
		leaf.ncdc_hits_can = 0;
		leaf.nfdc_hits_can = 0;
		leaf.ncdc_hits_thrown_and_can = 0;
		leaf.nfdc_hits_thrown_and_can = 0;
		leaf.cdc_chisq_can = 1.0E6;
		leaf.fdc_chisq_can = 1.0E6;
		
		// CDC
		const DTrackCandidate *cdc_can = NULL;
		if(icdc_can>=0 && icdc_can<trackcandidates.size()){
			cdc_can = trackcandidates[icdc_can];
			
			leaf.ncdc_hits_can = cdc_candidate_hits[icdc_can].size();
			leaf.ncdc_hits_thrown_and_can = cdc_matched_hits.size();
			leaf.cdc_chisq_can = 0.0;
		}
		
		// FDC
		const DTrackCandidate *fdc_can = NULL;
		if(ifdc_can>=0 && ifdc_can<trackcandidates.size()){
			fdc_can = trackcandidates[ifdc_can];
			
			leaf.nfdc_hits_can = fdc_candidate_hits[ifdc_can].size();
			leaf.nfdc_hits_thrown_and_can = fdc_matched_hits.size();
			leaf.fdc_chisq_can = 0.0;
		}
		
		// Figure out which candidate (if any) I should match this with
		const DTrackCandidate* can = NULL;
		if(cdc_can!=NULL && fdc_can!=NULL){
			can = cdc_matched_hits.size()>fdc_matched_hits.size() ? cdc_can:fdc_can;
		}else{
			can = cdc_can!=NULL ? cdc_can:fdc_can;
		}
		
		if(can!=NULL){
			leaf.pcan = can->momentum();
			leaf.z_can = can->position().Z();
		}

		//========= FIT TRACK ========
		// Look for a fit track that matches this thrown one
		unsigned int icdc_fit = FindMatch(cdc_thrownhits, cdc_fit_hits, cdc_matched_hits);
		unsigned int ifdc_fit = FindMatch(fdc_thrownhits, fdc_fit_hits, fdc_matched_hits);
		
		// Initialize values for when no matching fitdidate is found
		leaf.pfit.SetXYZ(0.0, 0.0, 0.0);
		leaf.z_fit = -1000.0;
		leaf.ncdc_hits_fit = 0;
		leaf.nfdc_hits_fit = 0;
		leaf.ncdc_hits_thrown_and_fit = 0;
		leaf.nfdc_hits_thrown_and_fit = 0;
		leaf.cdc_chisq_fit = 1.0E6;
		leaf.fdc_chisq_fit = 1.0E6;
		
		// CDC
		const DTrack *cdc_fit = NULL;
		if(icdc_fit>=0 && icdc_fit<tracks.size()){
			cdc_fit = tracks[icdc_fit];
			
			leaf.ncdc_hits_fit = cdc_fit_hits[icdc_fit].size();
			leaf.ncdc_hits_thrown_and_fit = cdc_matched_hits.size();
			leaf.cdc_chisq_fit = 0.0;
		}
		
		// FDC
		const DTrack *fdc_fit = NULL;
		if(ifdc_fit>=0 && ifdc_fit<tracks.size()){
			fdc_fit = tracks[ifdc_fit];
			
			leaf.nfdc_hits_fit = fdc_fit_hits[ifdc_fit].size();
			leaf.nfdc_hits_thrown_and_fit = fdc_matched_hits.size();
			leaf.fdc_chisq_fit = 0.0;
		}
		
		// Figure out which fitdidate (if any) I should match this with
		const DTrack* fit = NULL;
		if(cdc_fit!=NULL && fdc_fit!=NULL){
			fit = cdc_matched_hits.size()>fdc_matched_hits.size() ? cdc_fit:fdc_fit;
		}else{
			fit = cdc_fit!=NULL ? cdc_fit:fdc_fit;
		}
		
		if(fit!=NULL){
			leaf.pfit = fit->momentum();
			leaf.z_fit = fit->position().Z();
		}
		
		trkeff->Fill();
	}
	
	// Unlock mutex
	pthread_mutex_unlock(&mutex);
	
	return NOERROR;
}

//------------------
// GetCDCHits
//------------------
void DEventProcessor_trackeff_hists::GetCDCHits(const DKinematicData *p, CDChitv &inhits, CDChitv &outhits)
{
	// In case we run with multiple threads
	pthread_mutex_lock(&rt_mutex);
	
	// Re-swim the reference trajectory using the parameters in p
	ref->Swim(p->position(), p->momentum(), p->charge());
	
	// Loop over hits in the "inhits" vector and find the DOCA of the R.T.
	// for each.
	outhits.clear();
	for(unsigned int i=0; i<inhits.size(); i++){
		double doca = ref->DistToRT(inhits[i]->wire);
		if(doca < MAX_HIT_DIST_CDC)outhits.push_back(inhits[i]);
	}
	
	pthread_mutex_unlock(&rt_mutex);
}

//------------------
// GetFDCHits
//------------------
void DEventProcessor_trackeff_hists::GetFDCHits(const DKinematicData *p, FDChitv &inhits, FDChitv &outhits)
{
	// In case we run with multiple threads
	pthread_mutex_lock(&rt_mutex);
	
	// Re-swim the reference trajectory using the parameters in p
	ref->Swim(p->position(), p->momentum(), p->charge());
	
	// Loop over hits in the "inhits" vector and find the DOCA of the R.T.
	// for each.
	outhits.clear();
	for(unsigned int i=0; i<inhits.size(); i++){
		const DFDCHit* hit = inhits[i];
		if(hit->type != 0)continue; // filter out cathode hits
		const DFDCWire *wire = DFDCGeometry::GetDFDCWire(hit->gLayer, hit->element);
		if(!wire)continue;
		double doca = ref->DistToRT(wire);
		if(doca < MAX_HIT_DIST_FDC)outhits.push_back(hit);
	}
	
	pthread_mutex_unlock(&rt_mutex);
}

//------------------
// GetFDCHitsFromTruth
//------------------
void DEventProcessor_trackeff_hists::GetFDCHitsFromTruth(int trackno, FDChitv &outhits)
{
	// Loop over all entried in the fdclink map and find the ones
	// corresponding to the given track number
	outhits.clear();
	map<const DFDCHit*, const DMCTrackHit*>::iterator iter;
	for(iter=fdclink.begin(); iter!=fdclink.end(); iter++){
		if((iter->second)->track==trackno)outhits.push_back(iter->first);
	}
}

//------------------
// GetNFDCWireHits
//------------------
unsigned int DEventProcessor_trackeff_hists::GetNFDCWireHits(FDChitv &inhits)
{
	unsigned int N=0;
	for(unsigned int i=0; i<inhits.size(); i++){
		if(inhits[i]->type==0)N++;
	}
	return N;
}

//------------------
// FindMatch
//------------------
unsigned int DEventProcessor_trackeff_hists::FindMatch(CDChitv &thrownhits, vector<CDChitv> &candidate_hits, CDChitv &matched_hits)
{
	// Loop through all of the candidate hits and look for the one that best
	// matches this thrown's hits. The algorithm simply looks for the candidate
	// that has the most hits in common with the thrown.
	//
	// If the best match shares less than half its hits with the thrown,
	// then it is considered not to match and -1 is returned.

	unsigned int ibest=(unsigned int)-1;
	CDChitv my_matched;
	matched_hits.clear();
	for(unsigned int i=0; i<candidate_hits.size(); i++){
		CDChitv &canhits = candidate_hits[i];
		
		// Loop over thrown hits
		my_matched.clear();
		for(unsigned int j=0; j<thrownhits.size(); j++){
			// Loop over candidate hits
			for(unsigned int k=0; k<canhits.size(); k++){
				if(canhits[k] == thrownhits[j])my_matched.push_back(thrownhits[j]);
			}
		}
		
		// Check if this candidate is a better match
		if(my_matched.size() > matched_hits.size()){
			matched_hits = my_matched;
			ibest = i;
		}
	}
	
	// Is the best good enough?
	if(matched_hits.size() >= (thrownhits.size()/2))return ibest;
	
	return (unsigned int)-1;
}

//------------------
// FindMatch
//------------------
unsigned int DEventProcessor_trackeff_hists::FindMatch(FDChitv &thrownhits, vector<FDChitv> &candidate_hits, FDChitv &matched_hits)
{
	// Loop through all of the candidate hits and look for the one that best
	// matches this thrown's hits. The algorithm simply looks for the candidate
	// that has the most hits in common with the thrown.
	//
	// If the best match shares less than half its hits with the thrown,
	// then it is considered not to match and -1 is returned.

	unsigned int ibest=(unsigned int)-1;
	FDChitv my_matched;
	matched_hits.clear();
	for(unsigned int i=0; i<candidate_hits.size(); i++){
		FDChitv &canhits = candidate_hits[i];
		
		// Loop over thrown hits
		my_matched.clear();
		for(unsigned int j=0; j<thrownhits.size(); j++){
			// Loop over candidate hits
			for(unsigned int k=0; k<canhits.size(); k++){
				if(canhits[k] == thrownhits[j])my_matched.push_back(thrownhits[j]);
			}
		}
		
		// Check if this candidate is a better match
		if(my_matched.size() > matched_hits.size()){
			matched_hits = my_matched;
			ibest = i;
		}
	}
	
	// Is the best good enough?
//_DBG_<<"matched_hits.size()="<<matched_hits.size()<<"  thrownhits.size()="<<thrownhits.size()<<endl;
	if(matched_hits.size() >= (thrownhits.size()/2))return ibest;
	
	return (unsigned int)-1;
}

//------------------
// FindMatch
//------------------
void DEventProcessor_trackeff_hists::FindFDCTrackNumbers(JEventLoop *loop)
{
	vector<const DFDCHit*> fdchits;
	vector<const DMCTrackHit*> mchits;
	loop->Get(fdchits);
	loop->Get(mchits);

	fdclink.clear();

	// Loop over all FDC wire hits
	for(unsigned int i=0; i<fdchits.size(); i++){
		const DFDCHit *fdchit = fdchits[i];
		if(fdchit->type!=0)continue; // only look for wires
		const DFDCWire *wire = DFDCGeometry::GetDFDCWire(fdchit->gLayer, fdchit->element);
		if(!wire)continue;
		
		// Loop over FDC truth points
		for(unsigned int j=0; j<mchits.size(); j++){
			const DMCTrackHit *mchit = mchits[j];
			if(mchit->system != SYS_FDC)continue;
			
			// Check if this hit is "on" this wire
			if(fabs(mchit->z - wire->origin.Z())>0.01)continue;
			DVector2 A(wire->origin.X(), wire->origin.Y());
			DVector2 Adir = A/A.Mod();
			DVector2 udir(wire->udir.X(), wire->udir.Y());
			DVector2 f(mchit->r*cos(mchit->phi), mchit->r*sin(mchit->phi));
			double delta = udir*(f-A);
			double k = udir*Adir;
			double beta = Adir*(f - A - delta*udir)/(1.0 - k*Adir*udir);
			
			// Truth point is somewhere in the cell. We give some tolerance
			// for the exact cell size because the track can go through at
			// an angle such that the truth point is outside the cell. 
			// Not also that the same truth point can be associated with 2
			// FDC hits.
			bool match = (fabs(beta)-1.116/2.0)<0.01;
			if(!match)continue;
			
			// Check if this is already in the map
			map<const DFDCHit*, const DMCTrackHit*>::iterator iter = fdclink.find(fdchit);
			if(iter!=fdclink.end()){
				//_DBG_<<"More than 1 match for FDC hit!"<<endl;
				// If a link for this hit already exists, delete it so neither
				// one is used. This will of couse only work for 0,1, or 2
				// hits matched to a given truth point. 3 or more hits will
				// cause the algorithm to fail.
				fdclink.erase(iter);
			}else{
				fdclink[fdchit] = mchit;
			}
		}
	}
}

