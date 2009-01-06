// $Id$
//
//    File: dchit.h
// Created: Tues. Feb. 5, 2008
// Creator: davidl
//

#ifndef _dchit_
#define _dchit_

#include <TObject.h>
#include <TVector3.h>


class dchit:public TObject{

	public:
		int eventnumber;
		int wire;
		int layer;
		float t;
		float tof;
		float doca;
		float resi;
		float trk_chisq;
		float trk_Ndof;
		int LRthrown;
		int LRfit;
		TVector3 pos_doca;
		TVector3 pos_wire;

	private:
		ClassDef(dchit,1);

};

#endif // _dchit_

