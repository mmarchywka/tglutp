#ifndef POT_ENERGY_CONTOUR_H__
#define POT_ENERGY_CONTOUR_H__

#include "../data_and_config.h"
#include "atom_list.h"
#include "../atom/raw_points_list.h"
#include "../atom/energy_tr.h"


#include <sstream>
#include <string>
#include <iostream>



class pot_energy_contour : public energy_tr
{
	typedef pot_energy_contour Myt;
	typedef AtomLiTy SrcTy;
	typedef RawListTy  DestTy;
	typedef string_parse_type StParse;
public:

	pot_energy_contour
		(config_type & cf, StParse & p, const string_type nm,  SrcTy * ap,DestTy*& cage);
private:

	void Configure(config_type & cf, StParse& p, const string_type nm);
	
	
};

#endif
