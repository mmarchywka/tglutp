#ifndef BASIC_POINTELISM_H__
#define BASIC_POINTELISM_H__

#include "../data_and_config.h"
//#include "atom_list.h"
#include "../atom/raw_points_list.h"
#include "../atom/energy_tr.h"


class basic_pointelism : public energy_tr
{
	typedef basic_pointelism Myt;
	typedef RawListTy  DestTy;
	typedef RawListTy SrcTy;
	typedef string_parse_type StParse;
public:

	basic_pointelism
		(config_type & cf, StParse & p, const string_type nm,
		const SrcTy* red, const SrcTy * blue, DestTy*& dest);
private:

	void Configure(config_type & cf, StParse& p, const string_type nm);
	
	
};

#endif
