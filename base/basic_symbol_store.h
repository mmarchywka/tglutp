#ifndef BASIC_SYMBOL_STORE_H__
#define BASIC_SYMBOL_STORE_H__

#include "../debug_and_support.h"

#include <map>
#include <sstream>
#include <string>
#include <ios>
#include <iostream>
class basic_symbol_store
{
	typedef basic_symbol_store Myt;
	typedef std::istringstream string_parse_type;
	typedef std::ostringstream conv_type;
	typedef char Ch;
	typedef std::basic_string<Ch> name_type;
	typedef std::map<name_type, name_type> MapTy;
	typedef MapTy::const_iterator MapItor;
public:
	basic_symbol_store():m_map() {}	

	template <class Ty> void store(const name_type & nm, const Ty & val)
	{conv_type  p; //= new conv_type("",ios_base::ate);
	 p<<val;
	 m_map[nm]=p.str();
	 }
	template <class Ty> void load(const name_type & nm, Ty& val)
	{string_parse_type p(m_map[nm]); p>>val; }
	bool has(const name_type & nm) const
	{ 	MapItor ii=m_map.find(nm), ie=m_map.end();
		return (ii!=ie); 
	}
	const name_type & get(const name_type & nm)
	{ return m_map[nm]; } 
	template <class Os> void dump(Os& os) const
	{ 	MapItor ii=m_map.begin(), ie=m_map.end();
		while (ii!=ie) { os<<(*ii).first<<"="<<(*ii).second<<".\n"; ++ii; }
	}
	operator const MapTy&() const { return m_map;}
	Myt& operator +=(const MapTy & that) 
	{ MapTy::const_iterator ii=that.begin(),ie=that.end();
		while (ii!=ie) {m_map[(*ii).first]=(*ii).second; ++ii; }
	return *this;
}
Myt& operator &=(const MapTy & that) 
	{ MapTy::iterator ii=m_map.begin(),ie=m_map.end();
		while (ii!=ie) 
		{if(that.find((*ii).first)!=that.end())
			m_map[(*ii).first]	=(*(that.find((*ii).first))).second; 
			++ii; 
		}
	return *this;
}
void clear() { m_map.clear();}
void erase(const name_type &nm) {   if (has(nm)) m_map.erase(m_map.find(nm));  };
MapTy::iterator begin() {  return m_map.begin();}
const MapTy::iterator end() {  return m_map.end();}
const MapTy::iterator find(const name_type & nm ) {  return m_map.find(nm);}
name_type operator[](const name_type & nm)const  { return (*(m_map.find(nm))).second; }
private:
	MapTy m_map;	
	
};

#endif
