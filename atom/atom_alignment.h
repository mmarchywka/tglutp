#ifndef ATOM_ALIGNMENT_H__
#define ATOM_ALIGNMENT_H__
#include "../math/move_around.h"
#include "../debug_and_support.h"
#include "../data_and_config.h"
#include "atom_list.h"
#include "../atom/energy_tr.h"
#include "atom_list.h"

#include "../atom/atom_selection.h"
#include <map>

class atom_alignment : public energy_tr,public caving_tr
{
	typedef atom_alignment Myt;
	typedef string_parse_type StParse;
	typedef atom_container AtomContainerTy;
	typedef std::map<string_type,string_type> LutTy;
public:

	atom_alignment
		(config_type & cf, StParse & p, const string_type nm,
		AtomContainerTy* src);//{}
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

#include "../atom/raw_points_list.h"

class atom_exposure : public energy_tr,public caving_tr
{
	typedef atom_alignment Myt;
	typedef string_parse_type StParse;
	typedef atom_container AtomContainerTy;
	typedef std::map<string_type,string_type> LutTy;
public:

	atom_exposure
		(config_type & cf, StParse & p, const string_type nm,
		AtomContainerTy* src,AtomContainerTy* dest);//{}
	atom_exposure
		(config_type & cf, StParse & p, const string_type nm,
		AtomContainerTy* src,RawListTy* dest);//{}
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
