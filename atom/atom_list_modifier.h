#ifndef ATOM_LIST_MODIFIER_H__
#define ATOM_LIST_MODIFIER_H__
#include "../debug_and_support.h"
#include "../data_and_config.h"
#include "atom_list.h"
//#include "../atom/raw_points_list.h"
#include "../atom/energy_tr.h"
#include "atom_list.h"

#include "../atom/atom_selection.h"
#include <map>

#include "../atom/raw_points_list.h"

class atom_list_modifier : public energy_tr,public caving_tr
{
	typedef atom_list_modifier Myt;
//	typedef TgtListTy  DestTy;
//	typedef TgtListTy SrcTy;
	typedef string_parse_type StParse;
	
//	typedef std::vector<pdb_atom> xx;
	typedef atom_container AtomContainerTy;
	typedef std::map<string_type,string_type> LutTy;
public:

	atom_list_modifier
		(config_type & cf, StParse & p, const string_type nm,
		AtomContainerTy* src);//{}
		atom_list_modifier
		(config_type & cf, StParse & p, const string_type nm,
		RawListTy* src);//{}
private:

	void Configure(config_type & cf, StParse& p, const string_type nm);//{}
	void AddCmd(config_type & cf, const string_type & nm,const string_type &val)
	{	
		string_type dest="";
		cf.parse(nm,val,dest);
		if (dest.length()!=0) m_lut[val]=dest;
	}
	
atom_selection m_sel;
	LutTy m_lut;
};

#endif
