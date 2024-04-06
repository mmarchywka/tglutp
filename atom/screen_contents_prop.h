#ifndef SCREEN_CONTENTS_PROP_H__
#define SCREEN_CONTENTS_PROP_H__

#include "../data_and_config.h"
#include "../atom/energy_tr.h"
#include "../base/basic_symbol_store.h"
//#include "../base/cheap_greps.h"
#include <sstream>
#include <string>
#include <iostream>

#include <map>


class screen_contents_prop : public energy_tr
{
	typedef screen_contents_prop Myt;
//	typedef AtomLiTy SrcTy;
	typedef string_parse_type StParse;
	typedef basic_symbol_store StoreTy;
	typedef std::map<string_type,string_type> LutTy;
	typedef LutTy::iterator LutItor;
public:

	screen_contents_prop(config_type & cf, StParse & p, const string_type nm)
	{ Configure(cf,p,nm); }
	typedef int index_type; // almost didn't need it :)
	void calculate(index_type w, index_type h);
	const StoreTy & results() const
	{ return m_store; }
private:
	void Bounds(const string_type &nm,index_type w, index_type h);
	void Percent(const string_type &nm,index_type w, index_type h);
	void Zstats(const string_type &nm,index_type w, index_type h);
	void Configure(config_type & cf, StParse& p, const string_type nm);
	void AddCmd(config_type& cf, const string_type& nm, const string_type & val)
	{	string_type dest="";
		cf.parse(nm,val,dest);
		if (dest.length()!=0) m_lut[val]=dest;
	}
StoreTy m_store;
LutTy m_lut;
//atom_selection m_sel;
	
	
};


/*

class spatial_properties : public energy_tr
{
	

	void calculate(SrcTy* src);
	void centroid(SrcTy* src, const string_type & nm);
	void axes(SrcTy* src, const string_type & nm);
	void minmax(SrcTy* src, const string_type & nm);
	void selected(SrcTy* src, const string_type & nm);
	const StoreTy & results() const
	{ return m_store; }

};

*/

#endif
