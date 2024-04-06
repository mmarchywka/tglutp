#ifndef BASIC_CAVING_H__
#define BASIC_CAVING_H__
#include "../debug_and_support.h"
#include "../data_and_config.h"
//#include "atom_list.h"
//#include "../atom/raw_points_list.h"
#include "../atom/energy_tr.h"
#include "atom_list.h"

class basic_caving : public energy_tr,public caving_tr
{
	typedef basic_caving Myt;
	typedef TgtListTy  DestTy;
	typedef TgtListTy SrcTy;
	typedef string_parse_type StParse;
	
	typedef std::vector<pdb_atom> xx;
	typedef atom_container AtomContainerTy;
public:

	basic_caving
		(config_type & cf, StParse & p, const string_type nm,
		const AtomContainerTy* src, AtomContainerTy*& dest);
private:

	void Configure(config_type & cf, StParse& p, const string_type nm);
	
	
};

#endif
