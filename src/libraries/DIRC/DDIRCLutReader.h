// $Id$
//
//    File: DDIRCLutReader.h
//

#ifndef _DDIRCLutReader_
#define _DDIRCLutReader_

#include <JANA/jerror.h>

#include <DANA/DApplication.h>

#include "TROOT.h"
#include "TVector3.h"
#include "TFile.h"
#include "TTree.h"

class DDIRCLutReader{

public:
	
	DDIRCLutReader(JApplication *japp, unsigned int run_number);
	virtual ~DDIRCLutReader();

	uint GetLutPixelAngleSize(int bar, int pixel) const;
	uint GetLutPixelTimeSize(int bar, int pixel) const;
	uint GetLutPixelPathSize(int bar, int pixel) const;
	TVector3 GetLutPixelAngle(int bar, int pixel, int entry) const;
	Double_t GetLutPixelTime(int bar, int pixel, int entry) const;
	Long64_t GetLutPixelPath(int bar, int pixel, int entry) const;
	vector<TVector3> lutNodeAngle[48][10864];
		

private:

	pthread_mutex_t mutex;

	//vector<TVector3> lutNodeAngle[48][10864];
	vector<Double_t> lutNodeTime[48][10864];
	vector<Long64_t> lutNodePath[48][10864];
};

#endif // _DDIRCLutReader_
