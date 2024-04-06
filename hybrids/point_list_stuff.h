#ifndef POINT_LIST_STUFF_H__
#define POINT_LIST_STUFF_H__

#include "../atom/pdb_atom.h"
#include "pdb_hybrid_typedefs.h"
#include <vector>
#include <map>

class point_list : public std::vector<pdb_atom>, public pdb_typedefs
{
	typedef point_list Myt;
	typedef std::vector<pdb_atom> Super;
	typedef Super::value_type V;
	typedef Super::const_iterator CI;
	public :
	string_type name() const { return m_name;}
	Myt & name( const string_type & nm) 
	{ m_name=nm; return *this; } 

	template <class Ty,class Ts > static void mutate(Myt& d, const Ts & s, const Ty & f)
	{
		V v;
		typedef typename Ts::const_iterator Cis;
		//CI 
		Cis ii=s.begin(), ie=s.end();
		while (ii!=ie) { f(v,*ii); d.push_back(v); ++ii; }
		
	}
	private:
	string_type m_name;
};


class point_ptr_list : public std::vector<pdb_atom* >, public pdb_typedefs
{
	typedef point_ptr_list Myt;
	typedef point_list SrTy;
	public :
	point_ptr_list():m_mom(0) {}
	SrTy const *  mom() const { return m_mom; } 
	string_type name() const { return m_name;}
	Myt & name( const string_type & nm) 
	{ m_name=nm; return *this; } 
	Myt & operator=( SrTy & rhs)
	{
		m_mom=&rhs; 
		SrTy::iterator ii=rhs.begin(), ie=rhs.end();
		while (ii!=ie) {  push_back(&(*ii));   ++ii; }	
		name(rhs.name()); 
		return *this;
	} 
	private:
	string_type m_name;
	SrTy const * m_mom;

};




class attr_catalog : public std::map<std::string, int>
{
	typedef attr_catalog Myt;
	typedef std::map<std::string, int> Super;
public:

template <class Os>  bool dump (Os & os,const int len)
{
	typedef typename Super::iterator I;
	I ii=begin(), ie=end();int i=0;
	bool rv=true;
	while (ii!=ie) 
	{    os<<(*ii).first<<" "<<(*ii).second<<"\t";         
		++i; rv=( (i%len)==0); if (rv)  os<<'\n'; ++ii;
	}
	if (!rv) os<<'\n';
	return rv; 
}	
	
};
class point_list_operations: public pdb_typedefs
{
typedef point_list_operations Myt;
public:
	point_list_operations() { make(); }
	
template <class Os,class Ty >	void list(Os& os,Ty& s)
{	// output a list of atoms that are obscured less than the visible threshold.
	typedef typename Ty::value_type V;
	typedef typename Ty::iterator 	I;
	I ii=s.begin(), ie=s.end();
	while (ii!=ie)
	{
		V & p=(*ii);
		os<<"RAW "<<p.radius()<<" "<<p.x()<<" "<<p.y()<<" "<<p.z()<<" "<<p.m_r
					<<" "<<p.m_g<<" "<<p.m_b<<" "<<"\n";
		++ii; 
	}

}
	
// this "should" allow for const Ty &s but not sure :)
template <class Ty> void catalog(Ty& s)
{
	typedef typename Ty::value_type V;
	typedef typename Ty::iterator 	I;
	attr_catalog rec,atom,residue,chain,elem;
	I ii=s.begin(), ie=s.end();
	while (ii!=ie)
	{
		V & v=(*ii);
		rec[v.field(0)]++;
		atom[v.field(2)]++;
		residue[v.field(4)]++;
		chain[v.field(5)]++;
		elem[v.field(13)]++;
		++ii;	
	}
	OsTy & os=dos();
	os<<" Catalog of "<<s.name()<<"\n";
	os<<"recs:\n" ; rec.dump(os,4);
	os<<"atoms:\n" ;atom.dump(os,5);
	os<<"resd:\n" ;residue.dump(os,5);
	os<<"chan:\n" ;chain.dump(os,4);
	os<<"elem:\n" ;elem.dump(os,8);
	
}
int add(const Chr* nm, const Chr* v ) 
{ 
	if ( m_map.find(St(nm))== m_map.end()) return -1;
	(*m_map[St(nm)])[St(v)]; 
	if (debug()) eos()<<MM_MARK<<" Adding "<<v <<" to "<< nm<<" for total size of "<<
		(*m_map[St(nm)]).size()<<'\n';
	return 0;
}
int operate(const Chr* nm, const Chr*  v ) 
{ 
	if ( strcmp(nm,"clear")==0)
	{
		if ( m_map.find(St(v))== m_map.end())
		{ 
			if (St(v)=="all")
			{    m_rec.clear(); m_atom.clear(); m_residue.clear(); m_chain.clear(); m_elem.clear();      
				return 0; 
			}
			return -1;
		}
		(*m_map[St(nm)]).clear();
		return 0; 
	}
	
	eos()<<MM_MARK<<" PLO commands are clear  followed by type,atom,residue,chain,elem, or all\n";
	return -1;
}
template <class Ty> void assign_some(Ty&d, Ty& s)
{
	typedef typename Ty::value_type V;
	typedef typename Ty::iterator 	I;
	I ii=s.begin(), ie=s.end();
	while (ii!=ie)
	{
		V & v=(*ii);
		if ( m_rec.find(v.field(0))==m_rec.end())
		if ( m_atom.find(v.field(2))==m_atom.end())
		if ( m_residue.find(v.field(4))==m_residue.end())
		if (m_chain.find(v.field(5))==m_chain.end())
		if (m_elem.find(v.field(13))==m_elem.end())
		d.push_back(v);
		
		++ii;	
	}
	
}
void make()
{
	m_map["type"]=&m_rec; 
	m_map["atom"]=&m_atom; 
	m_map["residue"]=&m_residue; 
	m_map["chain"]=&m_chain; 
	m_map["elem"]=&m_elem; 
		
}

private:
std::map<string_type, attr_catalog * > m_map;
attr_catalog m_rec,m_atom,m_residue,m_chain,m_elem;

};
#endif
