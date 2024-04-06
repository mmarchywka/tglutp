#ifndef SPATIAL_PROPERTIES_H__
#define SPATIAL_PROPERTIES_H__


#include "../data_and_config.h"
#include "atom_list.h"
//#include "../atom/raw_points_list.h"
#include "../atom/energy_tr.h"
#include "../base/basic_symbol_store.h"
#include "../atom/atom_selection.h"
//#include "../base/cheap_greps.h"
#include <sstream>
#include <string>
#include <iostream>

#include <map>

class spatial_properties : public energy_tr
{
	typedef spatial_properties Myt;
	typedef AtomLiTy SrcTy;
	typedef string_parse_type StParse;
	typedef basic_symbol_store StoreTy;
	typedef map<string_type,string_type> LutTy;
	typedef LutTy::iterator LutItor;
public:

	spatial_properties (config_type & cf, StParse & p, const string_type nm)
	{ Configure(cf,p,nm); }
	void calculate(SrcTy* src);
	void centroid(SrcTy* src, const string_type & nm);
	void axes(SrcTy* src, const string_type & nm);
	void minmax(SrcTy* src, const string_type & nm);
	void selected(SrcTy* src, const string_type & nm);
	const StoreTy & results() const
	{ return m_store; }
private:

	void Configure(config_type & cf, StParse& p, const string_type nm);
	
	void AddCmd(config_type & cf, const string_type & nm,const string_type &val)
	{	
		string_type dest="";
		cf.parse(nm,val,dest);
		if (dest.length()!=0) m_lut[val]=dest;
	}
StoreTy m_store;
LutTy m_lut;
atom_selection m_sel;
};
#endif
