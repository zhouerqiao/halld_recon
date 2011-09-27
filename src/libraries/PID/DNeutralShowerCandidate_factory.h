// $Id$
//
//    File: DNeutralShowerCandidate_factory.h
// Created: Tue Aug  9 14:29:24 EST 2011
// Creator: pmatt (on Linux ifarml6 2.6.18-128.el5 x86_64)
//

#ifndef _DNeutralShowerCandidate_factory_
#define _DNeutralShowerCandidate_factory_

#include <JANA/JFactory.h>
#include <PID/DNeutralShowerCandidate.h>
#include <PID/DChargedTrack.h>
#include <PID/DChargedTrackHypothesis.h>
#include <FCAL/DFCALShower.h>
#include <BCAL/DBCALShower.h>

class DNeutralShowerCandidate_factory:public jana::JFactory<DNeutralShowerCandidate>{
	public:
		DNeutralShowerCandidate_factory(){};
		~DNeutralShowerCandidate_factory(){};

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *locEventLoop, int runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *locEventLoop, int eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		int USE_KLOE;
};

#endif // _DNeutralShowerCandidate_factory_

