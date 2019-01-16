// $Id$
//
//    File: DEventProcessor_led_dirc.h
// Created: Thu Oct  4 18:32:51 EDT 2018
// Creator: yunjiey (on Linux yunjiey-VirtualBox 4.15.0-34-generic x86_64)
//

#ifndef _DEventProcessor_led_dirc_
#define _DEventProcessor_led_dirc_


//#include <ANALYSIS/DEventWriterROOT.h>
//#include <HDDM/DEventWriterREST.h>
//#include <ANALYSIS/DHistogramActions.h>

#include <iostream>
#include <vector>

#include <JANA/JFactory.h>
#include <JANA/JEventProcessor.h>
#include <JANA/JEventLoop.h>
#include <JANA/JApplication.h>
#include <DANA/DApplication.h>
#include <HDGEOMETRY/DGeometry.h>

#include <TRACKING/DMCThrown.h>
#include <TRACKING/DMCTrackHit.h>
#include <PID/DKinematicData.h>
#include <PID/DBeamPhoton.h>
#include <DIRC/DDIRCTruthBarHit.h>
#include <DIRC/DDIRCTruthPmtHit.h>
#include "DIRC/DDIRCGeometry.h"
#include <DIRC/DDIRCPmtHit.h>

#include <TMath.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TDirectoryFile.h>
#include <TThread.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TROOT.h>
#include <TClonesArray.h>

#include <TRandom3.h>
#include <TGraph.h>

#include <ANALYSIS/DAnalysisAction.h>

using namespace jana;
using namespace std;

class DEventProcessor_led_dirc : public jana::JEventProcessor
{
	public:
		const char* className(void){return "DEventProcessor_led_dirc";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop* locEventLoop, int32_t locRunNumber);	///< Called every time a new run number is detected.
		jerror_t evnt(jana::JEventLoop* locEventLoop, uint64_t locEventNumber);	///< Called every event.
		jerror_t erun(void);						///< Called every time run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

	const DDIRCGeometry* dDIRCGeometry;

	const double res_PmtTime = 1.;

	TH1F *hTruthBarHitBar;
	TH2F *hTruthBarHitXY;
	TH2F *hTruthPmtHitZY_North, *hTruthPmtHitZY_South;
	TH2F *hTruthPmtHit_North, *hTruthPmtHit_South;
	TH2F *hTruthPixelHit_North, *hTruthPixelHit_South;
	TH2F *hPixelHit_North, *hPixelHit_South;
	//TH2F *hTruthPixelHitTime;

	const int nPMTs   = 108;
	const int nPixels = 64;

	TH1F *hPixelHitTime_t_North[108][64];
	TH1F *hPixelHitTime_t_South[108][64];

	//TH1F *hPixelHitTime_t_fixed_North[108][64];
	//TH1F *hPixelHitTime_t_fixed_South[108][64];

	TH1F *hSmearAmount_HitTime;
	TH1F *hPixelHitTime_HitTimeSmeared_North[108][64];
	TH1F *hPixelHitTime_HitTimeSmeared_South[108][64];

	double t_rise = 0.84;
	double t_FWHM = 1.5;
	double LED_t_range;


	TGraph *LEDPulseShape;
	TH1F *hSmearAmount_LED;
	TH1F *hPixelHitTime_LEDSmeared_North[108][64];
	TH1F *hPixelHitTime_LEDSmeared_South[108][64];

	TH1F *hPixelHitTime_FinalSmeared_North[108][64];
	TH1F *hPixelHitTime_FinalSmeared_South[108][64];

		//For non-custom reaction-independent histograms, it is recommended that you simultaneously run the monitoring_hists plugin instead of defining them here

		//For making EventStore skims
		int Get_FileNumber(JEventLoop* locEventLoop) const;
		ofstream dEventStoreSkimStream;
};

#endif // _DEventProcessor_led_dirc_

