#ifndef ATOM_CMD_HANDLER_H__
#define ATOM_CMD_HANDLER_H__



#include "../atom/atom_display_tr.h"
#include "../view/text_commands_extension.h"
#include "atom_lists.h"

#include "../atom/nurbs_test.h"

#include <math.h>
#include <cmath>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>

class atom_display;
class atom_cmd_handler : public atom_display_tr, public text_commands_extension
{
	typedef atom_cmd_handler Myt;
	typedef text_commands_extension Super;
	// what the fucxk changed???
	typedef Super::string_type string_type;
public:
	atom_cmd_handler(atom_io * aio,config_type & cf);
	
	void cmd(text_commands * src, const console_type & s) ;
private:
	void Nurbs(string_parse_type & p);
	void Cage(string_parse_type & p);
	void Cave(string_parse_type & p);
	void Cavity(string_parse_type & p);
	void Tri(string_parse_type & p);
	void select_help(string_parse_type & p);
	void Select(string_parse_type & p);
	void SelPoints(string_parse_type & p);
	//void Iso(string_parse_type & p);
	void Pot(string_parse_type & p);
	void Pointel(string_parse_type & p, return_values_type & r);
	void Caving(string_parse_type & p, return_values_type & r);
	void Properties(string_parse_type & p, return_values_type & r);
	void Modify(string_parse_type & p, return_values_type & r);
	void ModPoints(string_parse_type & p, return_values_type & r);
	void Expo(string_parse_type & p, return_values_type & r);
	void Align(string_parse_type & p, return_values_type & r);
	atom_io & get_io() { return m_io; } 
config_type & m_config;
atom_io& m_io; 
atom_display * m_ad;
//bool m_script_started, m_script_more;
//config_type::value_iterator m_si;
};

#endif
