// $Id$
//
//    File: DEventProcessor_led_dirc.cc
// Created: Thu Oct  4 18:32:51 EDT 2018
// Creator: yunjiey (on Linux yunjiey-VirtualBox 4.15.0-34-generic x86_64)
//

#include "DEventProcessor_led_dirc.h"

// Routine used to create our DEventProcessor

extern "C"
{
	void InitPlugin(JApplication *locApplication)
	{
		InitJANAPlugin(locApplication);
		locApplication->AddProcessor(new DEventProcessor_led_dirc()); //register this plugin
	}
} // "C"

//------------------
// init
//------------------
jerror_t DEventProcessor_led_dirc::init(void)
{
  string locOutputFileName = "hd_root.root";
  if(gPARMS->Exists("OUTPUT_FILENAME"))
    gPARMS->GetParameter("OUTPUT_FILENAME", locOutputFileName);

  TFile* locFile = (TFile*)gROOT->FindObject(locOutputFileName.c_str());
  if(locFile != NULL)
    locFile->cd("");
  else
    gDirectory->Cd("/");

  int nChannels = 108*64;
  //hTruthPixelHitTime = new TH2F("hTruthPixelHitTime", "; Pixel Channel # ; #Delta t (ns)", nChannels, 0, nChannels, 200, -100, 100);

  hTruthBarHitXY = new TH2F("hTruthBarHitXY", "; Bar Hit X (cm); Bar Hit Y (cm)", 200, -100, 100, 200, -100, 100);
  hTruthBarHitBar = new TH1F("hTruthBarHitBar", "; Bar #", 48, 0.5, 47.5);
  hTruthPmtHitZY_North = new TH2F("hTruthPmtHitZY_North", "North Box; PMT Hit Z (cm); PMT Hit Y (cm)", 100, 525, 560, 110, 0., 110.);
  hTruthPmtHitZY_South = new TH2F("hTruthPmtHitZY_South", "South Box; PMT Hit Z (cm); PMT Hit Y (cm)", 100, 525, 560, 110, -110., 0.);
 
  hTruthPmtHit_North = new TH2F("hTruthPmtHit_North", "North Box; Pmt Hit Column ; Pixel Hit Row", 6, 0, 6, 18, 0, 18);
  hTruthPmtHit_South = new TH2F("hTruthPmtHit_South", "South Box; Pmt Hit Column ; Pixel Hit Row", 6, 0, 6, 18, 0, 18);
  hTruthPixelHit_North = new TH2F("hTruthPixelHit_North", "North Box; Pixel Hit X ; Pixel Hit Y", 144, 0, 144, 48, 0, 48);
  hTruthPixelHit_South = new TH2F("hTruthPixelHit_South", "South Box; Pixel Hit X ; Pixel Hit Y", 144, 0, 144, 48, 0, 48);

  hPixelHit_North = new TH2F("hPixelHit_North", "North Box; Pixel Hit X ; Pixel Hit Y", 144, 0, 144, 48, 0, 48);
  hPixelHit_South = new TH2F("hPixelHit_South", "South Box; Pixel Hit X ; Pixel Hit Y", 144, 0, 144, 48, 0, 48);


  LED_t_range = t_FWHM + t_rise;

  double LED_x[]={0,t_rise,t_FWHM,LED_t_range};
  double LED_y[]={0,1,1,0};
  LEDPulseShape= new TGraph(4,LED_x,LED_y);

  hSmearAmount_HitTime = new TH1F("hSmearAmount_HitTime","Hit Time Smear ; PMT Hit-time smear [ns] ; ",100,-5,5);
  hSmearAmount_LED     = new TH1F("hSmearAmount_LED","LED time smear; LED smear [ns] ; ",600,-1.,5.);

  double MinTime = -10.;
  double MaxTime = 50.;
  int    NbinsTime = 60; 
 
	for (int iPmt = 0 ; iPmt < nPMTs ; iPmt++)
	{
		for (int iPixel = 0; iPixel < nPixels ; iPixel++)
		{
                	int pixel_x = (17 - int(iPmt%18))*8 + (8 - int(iPixel%8)) ;
               		int pixel_y = (5  - int(iPmt/18))*8 + (8 - int(iPixel/8)) ;
			string pixel_xy = "PixelX"+std::to_string(pixel_x)+"_PixelY"+std::to_string(pixel_y);

			hPixelHitTime_t_South[iPmt][iPixel] = new TH1F(Form("hTimePmtHit_t_South_PMT%d_Pixel%d_%s",iPmt,iPixel,pixel_xy.c_str()),
								Form("hTimePmtHit_t_South_PMT%d_Pixel%d; hit time (ns);",iPmt,iPixel),NbinsTime,MinTime,MaxTime);
			//hPixelHitTime_t_fixed_South[iPmt][iPixel] = new TH1F(Form("hTimePmtHit_t_fixed_South_PMT%d_Pixel%d_%s",iPmt,iPixel,pixel_xy.c_str()),
			//					Form("hTimePmtHit_t_fixed_South_PMT%d_Pixel%d; hit time (ns);",iPmt,iPixel),NbinsTime,MinTime,MaxTime);
			hPixelHitTime_HitTimeSmeared_South[iPmt][iPixel] = new TH1F(Form("hTimePmtHit_HitTimeSmeared_South_PMT%d_Pixel%d_%s",iPmt,iPixel,pixel_xy.c_str()),
								Form("hTimePmtHit_HitTimeSmeared_South_PMT%d_Pixel%d; hit time (ns);",iPmt,iPixel),NbinsTime,MinTime,MaxTime);
			hPixelHitTime_LEDSmeared_South[iPmt][iPixel] = new TH1F(Form("hTimePmtHit_LEDSmeared_South_PMT%d_Pixel%d_%s",iPmt,iPixel,pixel_xy.c_str()),
								Form("hTimePmtHit_LEDSmeared_South_PMT%d_Pixel%d; hit time (ns);",iPmt,iPixel),NbinsTime,MinTime,MaxTime);
			hPixelHitTime_FinalSmeared_South[iPmt][iPixel] = new TH1F(Form("hTimePmtHit_FinalSmeared_South_PMT%d_Pixel%d_%s",iPmt,iPixel,pixel_xy.c_str()),
								Form("hTimePmtHit_FinalSmeared_South_PMT%d_Pixel%d; hit time (ns);",iPmt,iPixel),NbinsTime,MinTime,MaxTime);

		}
	}
	for (int iPmt = 0 ; iPmt < nPMTs ; iPmt++)
	{
                for (int iPixel = 0; iPixel < nPixels ; iPixel++)
                {

                	int pixel_x = (17 - int(iPmt%18))*8 + (8 - int(iPixel%8)) ;
               		int pixel_y = (5  - int(iPmt/18))*8 + (8 - int(iPixel/8)) ;
			string pixel_xy = "PixelX"+std::to_string(pixel_x)+"_PixelY"+std::to_string(pixel_y);

                        hPixelHitTime_t_North[iPmt][iPixel] = new TH1F(Form("hTimePmtHit_t_North_PMT%d_Pixel%d_%s",iPmt,iPixel,pixel_xy.c_str()),
                                                                Form("hTimePmtHit_t_North_PMT%d_Pixel%d; hit time (ns);",iPmt,iPixel),NbinsTime,MinTime,MaxTime);
                        //hPixelHitTime_t_fixed_North[iPmt][iPixel] = new TH1F(Form("hTimePmtHit_t_fixed_North_PMT%d_Pixel%d_%s",iPmt,iPixel,pixel_xy.c_str()),
                        //                                        Form("hTimePmtHit_t_fixed_North_PMT%d_Pixel%d; hit time (ns);",iPmt,iPixel),NbinsTime,MinTime,MaxTime);
                        hPixelHitTime_HitTimeSmeared_North[iPmt][iPixel] = new TH1F(Form("hTimePmtHit_HitTimeSmeared_North_PMT%d_Pixel%d_%s",iPmt,iPixel,pixel_xy.c_str()),
                                                                Form("hTimePmtHit_HitTimeSmeared_North_PMT%d_Pixel%d; hit time (ns);",iPmt,iPixel),NbinsTime,MinTime,MaxTime);
                        hPixelHitTime_LEDSmeared_North[iPmt][iPixel] = new TH1F(Form("hTimePmtHit_LEDSmeared_North_PMT%d_Pixel%d_%s",iPmt,iPixel,pixel_xy.c_str()),
                                                                Form("hTimePmtHit_LEDSmeared_North_PMT%d_Pixel%d; hit time (ns);",iPmt,iPixel),NbinsTime,MinTime,MaxTime);
                        hPixelHitTime_FinalSmeared_North[iPmt][iPixel] = new TH1F(Form("hTimePmtHit_FinalSmeared_North_PMT%d_Pixel%d_%s",iPmt,iPixel,pixel_xy.c_str()),
                                                                Form("hTimePmtHit_FinalSmeared_North_PMT%d_Pixel%d; hit time (ns);",iPmt,iPixel),NbinsTime,MinTime,MaxTime);

                }
	}

  return NOERROR;

}

//------------------
// brun
//------------------
jerror_t DEventProcessor_led_dirc::brun(jana::JEventLoop* loop, int32_t locRunNumber)
{
	// This is called whenever the run number changes
  vector<const DDIRCGeometry*> locDIRCGeometry;
  loop->Get(locDIRCGeometry);
  dDIRCGeometry = locDIRCGeometry[0];

  return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t DEventProcessor_led_dirc::evnt(jana::JEventLoop* loop, uint64_t locEventNumber)
{
  vector<const DBeamPhoton*> beam_photons;
  vector<const DMCThrown*> mcthrowns;
  vector<const DMCTrackHit*> mctrackhits;
  vector<const DDIRCTruthBarHit*> dircBarHits;
  vector<const DDIRCTruthPmtHit*> dircPmtHits;
  vector<const DDIRCPmtHit*> dircRecoPmtHits;
  
  loop->Get(beam_photons);
  loop->Get(mcthrowns);
  loop->Get(mctrackhits);
  loop->Get(dircPmtHits);
  loop->Get(dircBarHits);
  loop->Get(dircRecoPmtHits);

  if(mcthrowns.size()<1) return NOERROR;
  if(dircPmtHits.size()!=1) return NOERROR;

  for (unsigned int j = 0; j < dircBarHits.size(); j++){

    double x = dircBarHits[j]->x;
    double y = dircBarHits[j]->y;
    int bar = dircBarHits[j]->bar;

    japp->RootWriteLock(); //ACQUIRE ROOT LOCK
    hTruthBarHitXY->Fill(x, y);
    hTruthBarHitBar->Fill(bar);
    japp->RootUnLock();
  }

  TRandom3 rand3(0);

  for (unsigned int h = 0; h < dircPmtHits.size(); h++){
	
     int ch=dircPmtHits[h]->ch;
    
     double x = dircPmtHits[h]->x;
     double y = dircPmtHits[h]->y;
     double z = dircPmtHits[h]->z;
     double t = dircPmtHits[h]->t;
     //double t_fixed = dircPmtHits[h]->t_fixed;

     // get PMT labels
     int pmt_id     = dDIRCGeometry->GetPmtID(ch);
     int pmt_column = dDIRCGeometry->GetPmtColumn(ch); 
     int pmt_row    = dDIRCGeometry->GetPmtRow(ch);


     // get pixel labels
     int pixel_id = dDIRCGeometry->GetPixelID(ch);
     int pixel_x  = dDIRCGeometry->GetPixelX(ch);
     int pixel_y  = dDIRCGeometry->GetPixelY(ch);

     TVector3 pos =  TVector3(mcthrowns[0]->position().X(),
			      mcthrowns[0]->position().Y(),
			      mcthrowns[0]->position().Z());

     japp->RootWriteLock(); //ACQUIRE ROOT LOCK
     //hTruthPixelHitTime->Fill(ch, t-t_fixed);

     double t_HitTimeSmearAmount = rand3.Gaus(0.,res_PmtTime);

     hSmearAmount_HitTime -> Fill(t_HitTimeSmearAmount);

     double t_HitTimeSmeared = t + t_HitTimeSmearAmount;

     double t_rand1 = rand3.Rndm();
     double t_rand2 = rand3.Rndm();
     double t_LEDSmearAmount = t_rand1*LED_t_range;	
     double pulse_val = LEDPulseShape->Eval(t_LEDSmearAmount);

     while(t_rand2>pulse_val)
     {
	t_rand1 = rand3.Rndm();
	t_rand2 = rand3.Rndm();
     	t_LEDSmearAmount = t_rand1*LED_t_range;	
	pulse_val = LEDPulseShape->Eval(t_LEDSmearAmount);
	if (t_rand2 <= pulse_val)
		break;
     }

     hSmearAmount_LED -> Fill(t_LEDSmearAmount);
     double t_LEDSmeared = t + t_LEDSmearAmount;

     //Hard-coded feedthrough nominal positions -- to be fixed
     vector<double> South_FDTH_Y = {-23.6267,-55.4268,-87.2268};
     vector<double> North_FDTH_Y = {23.6267,55.4268,87.2268};
     double South_FDTH_X = -325.14;
     double North_FDTH_X = 325.14;

     double t_CableDelay = 0.;
     double dist_xy = 0.;
     if (x<0.)
     {
	for (int loc_i = 0; loc_i < 3; loc_i++)
	{
		dist_xy = (pos.X() - South_FDTH_X)*(pos.X() - South_FDTH_X) 
			+ (pos.Y() - South_FDTH_Y[loc_i])*(pos.Y() - South_FDTH_Y[loc_i]);
		if (dist_xy < 8.)
		{
			if (loc_i == 0)
				t_CableDelay = 20.;
			if (loc_i == 1)
				t_CableDelay = 10.;
			if (loc_i == 2)
				t_CableDelay = 0.;
		}

	}
     }
     else
     {
        for (int loc_i = 0; loc_i < 3; loc_i++)
        {
                dist_xy = (pos.X() - North_FDTH_X)        * (pos.X() - North_FDTH_X) 
                        + (pos.Y() - North_FDTH_Y[loc_i]) * (pos.Y() - North_FDTH_Y[loc_i]);
                if (dist_xy < 8.)
                {
                        if (loc_i == 0)
                                t_CableDelay = 20.;
                        if (loc_i == 1)
                                t_CableDelay = 10.;
                        if (loc_i == 2)
                                t_CableDelay = 0.;
                }

        }
     }
     double t_FinalSmeared = t + t_HitTimeSmearAmount + t_LEDSmearAmount + t_CableDelay;


     if(x < 0.) {
	hTruthPmtHitZY_South->Fill(z, y);
	hTruthPmtHit_South->Fill(pmt_column, pmt_row);
	hTruthPixelHit_South->Fill(pixel_x, pixel_y);
	hPixelHitTime_t_South[pmt_id][pixel_id]->Fill(t);
	//hPixelHitTime_t_fixed_South[pmt_id][pixel_id]->Fill(t_fixed);
	hPixelHitTime_HitTimeSmeared_South[pmt_id][pixel_id]->Fill(t_HitTimeSmeared);
	hPixelHitTime_LEDSmeared_South[pmt_id][pixel_id]->Fill(t_LEDSmeared);
	hPixelHitTime_FinalSmeared_South[pmt_id][pixel_id]->Fill(t_FinalSmeared);
     }
     else {
	hTruthPmtHitZY_North->Fill(z, y);
	hTruthPmtHit_North->Fill(pmt_column, pmt_row);
	hTruthPixelHit_North->Fill(pixel_x, pixel_y);
	hPixelHitTime_t_North[pmt_id][pixel_id]->Fill(t);
	//hPixelHitTime_t_fixed_North[pmt_id][pixel_id]->Fill(t);
	hPixelHitTime_HitTimeSmeared_North[pmt_id][pixel_id]->Fill(t_HitTimeSmeared);
	hPixelHitTime_LEDSmeared_North[pmt_id][pixel_id]->Fill(t_LEDSmeared);
	hPixelHitTime_FinalSmeared_North[pmt_id][pixel_id]->Fill(t_FinalSmeared);
     }
     japp->RootUnLock();
  }

  for (unsigned int h = 0; h < dircRecoPmtHits.size(); h++){
	  
	  int ch=dircRecoPmtHits[h]->ch;
	  //double t = dircRecoPmtHits[h]->t;
	  
	  // get pixel labels
	  int pixel_x = dDIRCGeometry->GetPixelX(ch);
	  int pixel_y = dDIRCGeometry->GetPixelY(ch);
	  

	  japp->RootWriteLock(); //ACQUIRE ROOT LOCK
	  if(ch < 108*64) {
		  hPixelHit_South->Fill(pixel_x, pixel_y);
	  }
	  else {
		  hPixelHit_North->Fill(pixel_x, pixel_y);
	  }
	  japp->RootUnLock();
  }



  return NOERROR;
}

int DEventProcessor_led_dirc::Get_FileNumber(JEventLoop* locEventLoop) const
{
	//Assume that the file name is in the format: *_X.ext, where:
		//X is the file number (a string of numbers of any length)
		//ext is the file extension (probably .evio or .hddm)

	//get the event source
	JEventSource* locEventSource = locEventLoop->GetJEvent().GetJEventSource();
	if(locEventSource == NULL)
		return -1;

	//get the source file name (strip the path)
	string locSourceFileName = locEventSource->GetSourceName();

	//find the last "_" & "." indices
	size_t locUnderscoreIndex = locSourceFileName.rfind("_");
	size_t locDotIndex = locSourceFileName.rfind(".");
	if((locUnderscoreIndex == string::npos) || (locDotIndex == string::npos))
		return -1;

	size_t locNumberLength = locDotIndex - locUnderscoreIndex - 1;
	string locFileNumberString = locSourceFileName.substr(locUnderscoreIndex + 1, locNumberLength);

	int locFileNumber = -1;
	istringstream locFileNumberStream(locFileNumberString);
	locFileNumberStream >> locFileNumber;

	return locFileNumber;
}

//------------------
// erun
//------------------
jerror_t DEventProcessor_led_dirc::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t DEventProcessor_led_dirc::fini(void)
{
	// Called before program exit after event processing is finished.
	if(dEventStoreSkimStream.is_open())
		dEventStoreSkimStream.close();
	return NOERROR;
}

