#ifndef DATA_AND_CONFIG_H
#define DATA_AND_CONFIG_H
//#include <GL/glut.h>
#include <list>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include <sstream>
#include <ios>

namespace kluge
{
	// the first thing to put here is a define...
#define MM_MAX_LINE_LENGTH 4096
extern char nl; //='\n';
	
};

//namespace 
class data_and_config
{
	public:
typedef char Ch;
typedef std::basic_istream<Ch>  input_type;
typedef std::basic_ostream<Ch>  output_type;
typedef std::basic_string<Ch> string_type;
typedef string_type name_type;

typedef int count_type;
typedef int flag_type;
typedef input_type IsTy;
typedef float coord_type;
typedef std::istringstream ConvStreamTy;
typedef std::ostringstream OsStreamTy;
	

static IsTy * open(const name_type & nm);

typedef string_type config_key_type;
typedef std::vector<string_type> config_value_type;
typedef count_type config_idx_type;
typedef std::map<config_key_type,config_value_type> config_base_type;
// The base class needs to be protected as we are counting accesses to let
// users determine if they have the most recent version- this 
// allows cached lookups but also a cheap consistency test
// Public unchecked access to the base map makes this impossible
class Config : public config_base_type
{
	typedef config_base_type Super;
	typedef Config Myt;
	public: 
	typedef config_key_type KeyTy;
	typedef string_type ValItemTy;
	typedef config_value_type ValTy;
	typedef ValTy::const_iterator value_iterator; 
	typedef iterator MyItor;
	typedef const_iterator MyCItor;
	public:
	
	Config(): m_prefix(),m_version(0),m_debug_mode(false),m_fail_mode(false),m_script_source(NULL) {}
	KeyTy m_prefix;
	~Config() { delete m_script_source; }
	const volatile count_type * version() const { return &m_version; } 
	template <class Ty> void debug (const string_type & c, const Ty & v) const
	{	
		if (m_debug_mode&&!m_fail_mode) 
		{Ty cc=v; OsStreamTy os; os<<cc;std::cout<<c<<os.str()<<".\n";
		std::cout.flush(); 
	} }
	template <class Ty> void faile (const string_type & c, const Ty & v) const
	{	
		if (m_fail_mode||m_debug_mode) 
		{Ty cc=v; OsStreamTy os; os<<cc;std::cout<<c<<os.str()<<".\n";
		std::cout.flush(); 
	} }
	bool has ( const KeyTy & key, const ValItemTy & val )  const ;
	bool has ( const KeyTy & key )  const ;
	const ValTy * get_vector(const KeyTy & key )  const ;
	const ValItemTy & get(const KeyTy & key )  const;
	value_iterator  get_all(const KeyTy & key )  const;
	value_iterator  get_end(const KeyTy & key )  const;
	template <class Ty> bool parse(const KeyTy & _str,const KeyTy & key, Ty& val) const
	{  
		debug("looking for entry ",_str);
		debug("   with key value =",key);
		debug("   default value is ",Ty(val));
		const_iterator crap=find(_str);
		if (crap==end()) {faile("   no entry for key ",_str ); return false; }
		debug("   found key ",_str);
		const ValTy& v=(*crap).second; 
		//KeyTy str=_str;
		for ( ValTy::const_iterator  i=v.begin(); i!=v.end(); i++)
		{
		ValItemTy vv=(*i); debug("      found value entry ",vv);
		ValItemTy::size_type x=vv.find("=");
		if(x!=string_type::npos) {// return false;
		debug("      testing against entry ",vv.substr(0,x));
		if (key==vv.substr(0,x)) {vv=vv.substr(x+1);
		ConvStreamTy ss(vv); ss>>val;  
		// template was looking for ref????
		Ty stupid=val; OsStreamTy os; os<<stupid;
		debug("      returning a value of ",os.str()); 
		return true; }}
		else { if (key==vv) {debug("      found entry no value",key);  return true;}
		}
	} 
	faile("   did find ",_str);
	faile("   but no entry for ", key);
		return false; 
		}
	template <class Ty> bool get(const KeyTy & key, Ty& val) const
	{
		MyCItor f=find(key);
		debug("trying to get key ",key);
		debug("   default value is ",val);
		if ( f==end()) { faile("   no entry for ",key); return false; }
		ValItemTy x =((*f).second)[0];
		ConvStreamTy str(x);
		str>>val;
		debug("   found a value of ",val);
		return true;
	}
	
	void load(int argc, char *argv[]);
	typedef void (  HelpTy )(); 
	int load(int argc, char *argv[], HelpTy * phelp, HelpTy * ahelp );
	void load(const name_type & x);
	void unload(const name_type & x);
	IsTy &  load(IsTy & is);

void add(const KeyTy & key, const ValItemTy & val) ;

	input_type & script_source();

	private:
	void Inc() { ++m_version; } 
volatile count_type m_version;
bool m_debug_mode,m_fail_mode; 	
	input_type * m_script_source;
	};

	
};
typedef data_and_config::Config config_type;
typedef data_and_config::OsStreamTy pr_stream_type;


#endif
