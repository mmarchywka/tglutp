#ifndef CHEAP_GREPS_H__
#define CHEAP_GREPS_H__
#include <sstream>
#include <iostream>
#include <list>
#include <map>
//#include <pair>

namespace greps
{
class Traits
{
	
	public: 
typedef char Ch;
typedef std::basic_istream<Ch>  input_type;
typedef std::basic_string<Ch> string_type;
typedef string_type StTy;
typedef string_type name_type;

typedef unsigned int count_type;
typedef int flag_type;
typedef input_type IsTy;
typedef float coord_type;
typedef std::istringstream ConvStreamTy;
typedef std::ostringstream OsStreamTy;
typedef ConvStreamTy string_parse_type;
enum {MAX_LINE=4096	 };
	
};


class cmd_line_form : public Traits
{	typedef cmd_line_form Myt;
	typedef Traits Tr; 
public:
	typedef std::list<name_type> HitTy;
	typedef HitTy::iterator ItorTy;
	// what the heck, a few levels of copy so what
	ItorTy begin() { return m_hits.begin(); }
	ItorTy end() { return m_hits.end(); }
	HitTy m_hits;
	
	template <typename Ts, typename Tstr> cmd_line_form(Ts & s,  Tstr & p)
	{
		typedef std::list<name_type> CriteriaTy;
		typedef CriteriaTy::iterator CrItor;
		CriteriaTy li;
		string_type qual="";
		//string_parse_type p(cmd);
		while (!p.eof()&&p.good())
		{  p>>qual; li.push_back(qual); }
		Ch stupid[Tr::MAX_LINE];
		string_type x="";
		while (!s.eof())
		{
		s.getline(stupid,Tr::MAX_LINE,'\n');
		x=name_type(stupid);
		CrItor ii=li.begin(), ie=li.end();
		while (ii!=ie)
		{
		qual=(*ii);
		bool pol=true;
		if (qual.find("-")==0) {pol=false; qual=qual.substr(1);}
		bool ma=(x.find(qual)!=string_type::npos);
		if (ma&&!pol||!ma&&pol)  break;//cout<<x<<"\n";		
		++ii;
		}
		if (ii==ie) m_hits.push_back(x);
		}		
	}
	
}; //class

class eqn_subs : public Traits
{	typedef eqn_subs Myt;
	typedef Traits Tr; 
public:
//	typedef list<name_type> HitTy;
//	typedef HitTy::iterator ItorTy;
	// what the heck, a few levels of copy so what
//	ItorTy begin() { return m_hits.begin(); }
//	ItorTy end() { return m_hits.end(); }
//	HitTy m_hits;
	string_type m_proc;
	string_type operator()() const { return m_proc; }	
	
	template <typename Ty > static bool ky_split(const Ty & s2, Ty & k,Ty& v)
	{
		//count_type pe=s2.find("=");
		std::size_t pe=s2.find("=");
		if ((pe!=string_type::npos)&&((pe+1)<s2.length())&&(pe>0)) 
		{	k=s2.substr(0,pe);v=s2.substr(pe+1); return true; }
		return false;
	}
	template <typename Tx, typename Ty > static void map_split(const Tx & s2, Ty & m)
	{
		//count_type pe=s2.find("=");
		std::size_t pe=s2.find("=");
		if ((pe!=string_type::npos)&&((pe+1)<s2.length())&&(pe>0)) 
			m[s2.substr(0,pe)]=s2.substr(pe+1);
		else if (s2.length()>0) m[s2]=""; 
	}
	template <typename TgtTy, typename Tm > static void map_from_fields(Tm & m)
	{count_type i=0;
		while (1)
	{
		string_type	key=TgtTy::field_name(i);
		if (key.length()==0 ) break; 
	//	std::cout<<"Adding a field:"<<key<<".\n";
		m[key]=(i++);
	}
		
		}
	template <typename Cf, typename Ty,typename Tx,typename TgtTy > static void cf_field_pairs
	(const Cf & cf, const Ty & str, Tx& parsed, Tx& parsed_negated, TgtTy )
	{
	typedef count_type IdxTy;
	typedef std::map<string_type,IdxTy> Mt;
	typedef std::pair<IdxTy,string_type> P;
	Mt m=Mt();
	//count_type i=0;
	map_from_fields<TgtTy,Mt>(m);
	
	string_type k="",v="";
	typename Cf::value_iterator vi=cf.get_all(str),ve=cf.get_end(str);
	while (vi!=ve) 
	{ //std::cout<<"parsing :"<<*vi<<"\n";
		if (ky_split(*vi,k,v) ) 
		{ 	Tx * p=&parsed; 
		//	std::cout<<"k="<<k<<".v="<<v<<".\n";
			if (k.substr(0,1)=="-") {k=k.substr(1); p=&parsed_negated; }
			if (m.find(k)!=m.end())  { p->push_back(P(m[k],v));  }
	  	}
		++vi;
	} 
	
	
	}
	template < typename Tmap, typename Ts>static void loadMap
	(Tmap &m, Ts& s_)
	{
		while (s_.length()!=0)
		{ 
		//	std::cout<<"s is ."<<s_<<".\n";
			string_type s2=s_;
			//count_type pp=s_.find(" ");
			std::size_t pp=s_.find(" ");
			if ( pp!=string_type::npos) 
			{s2=s_.substr(0,pp); if (pp+1>=s_.length()) {pp=s_.length()-1;}	
			 s_=s_.substr(pp+1);} else s_="" ;
			if (s_==" " ) s_="";	
			map_split(s2,m);

		}
		
	}
	template < typename Tmap, typename Ts> static void unloadMap
	(Tmap &m, Ts& proc)
	{ typedef typename Tmap::iterator foo;
		foo ii=m.begin();
		foo ie=m.end();
		while (ii!=ie)
		{ if (proc.length()>0) proc=proc+" "+(*ii).first; 
			else  proc=(*ii).first;
		 if ((*ii).second.length()>0) proc=proc+"="+(*ii).second; 
		++ii;
		}
	} 
	
	template < typename Tmap > static void mergeMap
	(Tmap &d, const Tmap& s)
	{ typedef typename Tmap::const_iterator foo;
		foo ii=s.begin(); foo ie=s.end();
		while (ii!=ie) { d[(*ii).first]=(*ii).second; ++ii; }
	} 
	
	
	/////
//	count_type pe=s2.find("=");
	//	if ((pe!=string_type::npos)&&((pe+1)<s2.length())&&(pe>0)) 
//			m[s2.substr(0,pe)]=s2.substr(pe+1);
	//	else if (s2.length()>0) m[s2]=""; 
	/////
	template < typename Tmap, typename Ts>static  void orideMap
	(Tmap &m, Ts& p)
	{	string_type qual="";	
		while (!p.eof()&&p.good())
		{  p>>qual;  
		map_split(qual,m);
//		count_type pe=qual.find("=");
	//		if ((pe!=string_type::npos)&&((pe+1)<qual.length())) 
		//		m[qual.substr(0,pe)]=qual.substr(pe+1);
			//	else m[qual]=""; 
		}
	}
	template <typename Ts, typename Tstr> eqn_subs(Ts & s,  Tstr & p)
	{ // return string s containing either original name=valu pairs
	// or those newer ones contained in p.

		typedef std::map<string_type,string_type> MapTy;
		MapTy m=MapTy();
		string_type s_=s;
		loadMap(m,s_);
		orideMap(m,p);
		m_proc="";
		unloadMap(m,m_proc);
	s=m_proc;
	}
	

template < class Tc,typename Ts >static  void cf_default(Tc &cf, const Ts & rcmd, Ts &def)
{
		typedef typename Tc::value_iterator foo;
		foo si=cf.get_all(rcmd);
		foo sf=cf.get_end(rcmd);
		while (si!=sf)
		{
			string_type s=(*si);
			++si;	
			def=s+" "+def;
	//		std::cout<<__FILE__<<__LINE__<<" gettinf "<< s<<" for "<<rcmd<<'\n';
		}
		
	}
	
	template < class Tc,typename Tp, typename Ts >static  
		Ts netMap(Tc &cf, Tp& p,Ts & hard_coded_defaults, Ts &default_key)
{
	typedef std::map<string_type,string_type> MapTy;
		MapTy m=MapTy(),m_user=MapTy();
		//the names are bad- this loads from a stream
		orideMap(m_user,p);
		// first put in hard coded value 
		loadMap(m,hard_coded_defaults);
		// then get the values at the hard coded OR user defined config location
		if (m_user.find("DEF")!=m_user.end()) { default_key=m_user["DEF"];	}
		Ts config_defaults="";
		cf_default(cf,default_key,config_defaults);
	//	std::cout<<__FILE__<<__LINE__<<" defaults is "<<config_defaults<<'\n';
		loadMap(m,config_defaults);
		// combine user input with net defaults
		mergeMap(m,m_user);
		// unload the map into a string temp so we can examine it
		Ts resultant="";
		unloadMap(m,resultant);
	//	std::cout<<__FILE__<<__LINE__<<" resultant is "<<resultant<<'\n';
		return resultant;
}// netmap
		
}; //class


class config_like_string : public Traits
{
public:

template <typename CfTy,class Ty,class Tx >
	void pass_cf(CfTy & cf,	const Tx& generic, const Ty & key, const StTy& def,string_parse_type & p)
	{
//		string_type str="figuretwo",str_tar="figtarget";
		string_type figtarget=def;
//		"probe_radius=1 neighbor_radius=15 center_index=20 center=0,0,0 make_shell=1";
		greps::eqn_subs msub=greps::eqn_subs(figtarget,p);
		figtarget=string_type(key)+" "+figtarget;
		if (cf.has("DEBUG")) std::cout<<" using string \n"<<figtarget<<"\n";
		cf.unload(string_type(generic)); cf.unload(string_type(key));
		cf.load(string_type(generic)+" "+string_type(key));
		cf.load(figtarget);
}	
};




}; // greps
#endif

