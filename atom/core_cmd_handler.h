#ifndef CORE_CMD_HANDLER_H__
#define CORE_CMD_HANDLER_H__


#include "../atom/atom_display_tr.h"
#include "../view/text_commands.h"
#include "atom_lists.h"

//#include "../atom/nurbs_test.h"

#include <math.h>
#include <cmath>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>

class atom_display;
class core_cmd_handler : public atom_display_tr, public text_commands_extension
{
	typedef core_cmd_handler Myt;
	typedef text_commands_extension Super;
	typedef atom_display_tr::string_type string_type;
public:
	core_cmd_handler(atom_io * aio,config_type & cf);
	
	void cmd(text_commands * src, const console_type & s) ;
private:
	void Script(const string_type & rcmd,text_commands * src,string_parse_type & p);
	void Vtree(string_parse_type & p);
	void Stree(string_parse_type & p);
	//void Screen(string_parse_type & p, return_values_type & r);
	void Screen(string_parse_type & p, text_commands * src);
	void Mark(string_parse_type & p, text_commands * src);
	void Cf(string_parse_type & p);
	static	void List(atom_io & io,string_parse_type & p);

public:
AtomLiTy * Load(string_parse_type &p); 
	static AtomLiTy * Load(atom_io & io,string_type s,config_type & cf);
	static 	AtomLiTy * Load(atom_io & io,string_parse_type & p, config_type & cf);
	void Cavity(AtomLiTy* al,const string_type & plp, const string_type & str) ;
	static void Cavity(atom_io & io,AtomLiTy* al,config_type & cf,
			const string_type & plp, const string_type & str);

	void Cavity(AtomLiTy* al,const string_type & str) ;
	static void Cavity(atom_io & io,AtomLiTy* al,config_type & cf,
			const string_type & str);
	
	atom_io & get_io() { return m_io; } 

	AtomLiTy * LoadParameters(const string_type & cf,
		 const string_type & rf, const string_type & pf) 
{      
	IsTy * cmis=DaCoTy::open(m_config.get(cf));
	IsTy * rmis=DaCoTy::open(m_config.get(rf));
	if ( (cmis!=0)&&(rmis!=0)) m_io.m_pr.load(cmis,rmis); 
	else std::cout<<"Cant load color and radius info\n";
	delete cmis; delete rmis; 
	return Load(m_io,m_config.get(pf),m_config);
}
// doh
int LoadParameters(const string_type & cf,
		 const string_type & rf) 
{      
	IsTy * cmis=DaCoTy::open(m_config.get(cf));
	IsTy * rmis=DaCoTy::open(m_config.get(rf));
	if ( (cmis!=0)&&(rmis!=0)) m_io.m_pr.load(cmis,rmis); 
	else { std::cout<<"Cant load color and radius info\n"; return -1; }
	delete cmis; delete rmis; 
	return 0;
}

config_type & m_config;
atom_io m_io; 
atom_display * m_ad;
bool m_script_started, m_script_more;
config_type::value_iterator m_si;
	
	
};

#endif
