// $Id$
//
//    File: DDIRCLut.h
//

#ifndef _DDIRCLut_
#define _DDIRCLut_

#include <JANA/JFactory.h>
#include <JANA/JObject.h>
using namespace jana;

#include <DANA/DApplication.h>
#include <PID/DDetectorMatches.h>
#include <DIRC/DDIRCGeometry.h>
#include <DIRC/DDIRCLutReader.h>
#include <DIRC/DDIRCTruthBarHit.h>
#include <DIRC/DDIRCPmtHit.h>

#include "TROOT.h"
#include "TVector3.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"

class DDIRCLut: public JObject {

public:
	
	JOBJECT_PUBLIC(DDIRCLut);

	DDIRCLut();
	~DDIRCLut(){};

	bool brun(JEventLoop *loop);
	bool CreateDebugHistograms();
	bool CalcLUT(TVector3 locProjPos, TVector3 locProjMom, const vector<const DDIRCPmtHit*> locDIRCHits, double locFlightTime, Particle_t locPID, shared_ptr<DDIRCMatchParams>& locDIRCMatchParams, const vector<const DDIRCTruthBarHit*> locDIRCBarHits, map<shared_ptr<const DDIRCMatchParams>, vector<const DDIRCPmtHit*> >& locDIRCTrackMatchParams) const;
	vector<pair<double,double>> CalcPhoton(const DDIRCPmtHit *locDIRCHit, double locFlightTime, TVector3 posInBar, TVector3 momInBar, map<Particle_t, double> locExpectedAngle, double locAngle, Particle_t locPID, map<Particle_t, double> &logLikelihoodSum, int &nPhotonsThetaC, double &meanThetaC, double &meanDeltaT, bool &isGood) const;
	vector<pair<double,double>> CalcPhoton(const DDIRCPmtHit *locDIRCHit, double locFlightTime, TVector3 posInBar, TVector3 momInBar, map<Particle_t, double> locExpectedAngle, double locAngle, Particle_t locPID, map<Particle_t, double> &logLikelihoodSum) const;
	double CalcLikelihood(double locExpectedThetaC, double locThetaC) const;
	double CalcAngle(TVector3 momInBar, double locMass) const;
	map<Particle_t, double> CalcExpectedAngles(TVector3 momInBar) const;
	
private:
	DApplication *dapp;
	DDIRCLutReader *dDIRCLutReader;
	const DDIRCGeometry *dDIRCGeometry;
	
	bool DIRC_DEBUG_HISTS;
	bool DIRC_TRUTH_BARHIT;
	bool DIRC_TRUTH_PIXELTIME;

	double DIRC_CUT_TDIFFD;
	double DIRC_CUT_TDIFFR;
	double DIRC_SIGMA_THETAC;
	double DIRC_LIGHT_V;

	int dMaxChannels;
	double dCriticalAngle, dIndex;

	TH1I *hDiff, *hDiffT, *hDiffD, *hDiffR, *hTime, *hCalc, *hNph, *hNphC;
	TH2I *hDiff_Pixel[2];
	deque<Particle_t> dFinalStatePIDs;
	map<Particle_t, TH1I*> hDeltaThetaC;
	map<Particle_t, TH2I*> hDeltaThetaC_Pixel;
};

#endif // _DDIRCLut_

