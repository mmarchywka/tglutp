#ifndef DISPLAY_TREE_H__H
#define DISPLAY_TREE_H__H

#include "../view/display_settings.h"
#include <iostream>
#include <math.h>
#include <map>
#include <vector>
#include <iostream>



//class display_tree_list;
class display_tree: public display_tr
{
	typedef display_tree Myt;
	typedef Myt * MyPtr;
	
	// The map is slow and nice, the vector fast and cryptic.
	typedef map<name_type,index_type> IndexTy;
	typedef vector<MyPtr> TreeTy;
	typedef TreeTy::iterator TreeItorTy;
	typedef IndexTy::const_iterator IndexItorTy;
	
public:
	display_tree(const string_type &nm="Root")
	:m_name(nm),m_attributes(0),m_mom(0), m_idx(),m_tree(),m_prune(false),m_priority(0){}
	// User only makes roots of calls to make a parented child.
	private:
	display_tree(const MyPtr mom,const string_type &nm):m_name(nm),m_attributes(0),
	m_mom(mom), m_idx(),m_tree(0),m_prune(false),m_priority(0) {}
	
	public:
	~display_tree()
	{ if (m_tree.size()) cout<<"Display tree not delted "<<m_name<<"\n";
	//	delete m_tree;
	}
const MyPtr mom() const { return m_mom; }
void adopt(const string_type & nm) { Adopt(nm);  }
string_type name() const { return m_name; }
Myt& mode(const disp_attr_type & m) { m_attributes=m; return *this; }
disp_attr_type mode() const { return m_attributes; }
const display_settings & settings() const { return m_set;} 
bool hidden() const{return m_set.hidden(); }// { return m_attributes==-1; }
index_type priority() const { return m_priority; }
bool use_instance_settings() const { return m_priority>16; }
count_type size() const { return m_idx.size(); }
bool prune() const { return m_prune; }
// doesn't normally propogate
Myt & prune(const bool p) {m_prune=p; return * this; }
template < class Ty> void traverse(Ty & ss,const count_type lvl) const
	{ Traverse(ss,lvl);	}
template < class Ty> void lineage(Ty & ss,const count_type lvl) const
	{ Lineage(ss,lvl);	}
	// if you insist on name calling, expect a little indirection 
MyPtr get_child( const string_type & name) { Adopt(name); return m_tree[m_idx[name]];}
MyPtr get_child( const string_type & name, const display_settings & ds) 
{ if ( Adopt(name))  m_tree[m_idx[name]]->m_set=ds;   ;  return m_tree[m_idx[name]];}
private:
//MyPtr Find(const index_type i) const
//{} 
public:

MyPtr operator[] (const index_type i) const
	{ if ( i<m_tree.size())
	{  MyPtr ch= m_tree[i];	
		// The min goes downward and max is always on root
		// after bubbling back up. 
		ch->m_idx_min=m_idx_max; 
		//This should be ok since the max bubbles up to root
		ch->m_idx_max=ch->m_idx_min;
		return ch;
	}
		std::cout<<" Tree index "<<i<<" greater than size "<<m_tree.size()<<"\n";
		//traverse(std::cout,0);
		Lineage(std::cout,0);
		return 0;
	}
	void sprout_max() 
	{ MyPtr x=m_mom; while (x) {x->m_idx_max=m_idx_max; x=x->m_mom; } }
	// not sure which way to go yet, right now just make sure they are consistent...
bool xassert(const string_type & name, const index_type i)
	{	
	bool r=(m_idx[name]==i);
	if (r) return r;
	std::cout<<"Name and index wrong "<<name<<" idx="<<i<<"\n";
	traverse(std::cout,0);
	return false;
	}
void remove(const string_type & name) {Remove(name); }
void clean() 
	{
	{	TreeItorTy ii=m_tree.begin(), ie=m_tree.end();
		while (ii!=ie) { (*ii)->clean();  delete (*ii);   ++ii;  }
	}
}	

bool ssets(const string_type & nm, const  string_type & os,const count_type prop)
{  ConvStreamTy p(os);
return sset(nm,p,prop);
}
template <class Ty>bool sset(const string_type & nm, Ty& os,const count_type prop)
{  OsStreamTy oss;
//std::cout<<" have a prop of "<<prop<<"\n";
	if (nm=="prune") {os>>m_prune; std::cout<<" setting prune ";}
	else if (nm=="priority"){ os>>m_priority; oss<<m_priority; 
           std::cout<<" setting priority ";}
         else if (nm=="attr"){ os>>m_attributes; oss<<m_attributes;
           std::cout<<" setting attr ";}
	else if (m_set.sset(nm,os,prop,oss))
	{}
	else {std::cout<<" or try priority,attr,prune\n"; return false;} 
//	std::cout<<" now have a prop of "<<prop<<"\n";
	if (prop==1) {
//		std::cout<<" going up hierachy\n";
		if (m_mom) {ConvStreamTy is(oss.str()); m_mom->sset(nm,is,prop);} }
	if (prop==2) { 
//		std::cout<<"going down hierarchy\n";
		TreeItorTy ii=m_tree.begin(), ie=m_tree.end();
	while (ii!=ie) { ConvStreamTy is(oss.str());  (*(ii++))->sset(nm,is,prop); }}

	return true;
}

Myt&  get_ref(const count_type i ) const { return *(m_tree[i]);}

template <class Tx,class Ty>  Myt & set ( Tx (Myt:: *thing), const Ty & val, const bool prop)
{	this->*thing=val; if (prop) Pgate(thing,val);return *this; }

private:
//template <class Tx,class Ty> void Pgate( Tx  *thing, const Ty & val);
//template <class Tx,class Ty> void display_tree::Pgate( Tx (Myt:: *thing), const Ty & val)
template <class Tx,class Ty> void Pgate( Tx (Myt:: *thing), const Ty & val)
{
	this->*thing=val;
//	std::cout<<"propogating to children...\n";
	TreeItorTy ii=m_tree.begin(), ie=m_tree.end();
	while (ii!=ie) { (*(ii++))->Pgate(thing,val); }
}

private:
bool Adopt(const string_type &name)
{
	if (m_idx.find(name)==m_idx.end()) 
	{	m_idx[name]= m_tree.size();
		m_tree.push_back(new Myt(this,name));  
		return true;}
	return false;
}
template < class Ty> void Traverse(Ty & ss,const count_type lvl) const
{
	ss<<m_name<<" "<<m_prune<<" "<<m_attributes<<" "<<m_priority<<" : ";m_set.dump(ss);
	const string_type indent=string_type().append(lvl*3+3,' ');
	IndexItorTy ii=m_idx.begin(), ie=m_idx.end(); //count_type i=0;
//	while (ii!=ie) { ss<<indent<<(i++)<<" ";  
//	m_tree[(*ii).second]->traverse(ss,lvl+1); ++ii; }
	while (ii!=ie) { ss<<indent<<(*ii).second<<" ";  
	m_tree[(*ii).second]->traverse(ss,lvl+1); ++ii; }
	
}
template < class Ty> void Lineage(Ty & ss,const count_type lvl) const
{
	ss<<"Mom is "<< m_name<<" "<<m_prune<<" "<<m_attributes<<" "<<m_priority<<" : ";m_set.dump(ss);
	const string_type indent=string_type().append(lvl*3+3,' ');
	if (m_mom) m_mom->Lineage(ss,lvl+1);
}

void Remove(const string_type & name)
	{	
		if (m_idx.find(name)==m_idx.end()) return; 
		MyPtr p=m_tree[m_idx[name]];
		p->clean();
		// I'm not sure this works right- free's resource and doesn't effect 
		// vector but does remove from map. 
		p->prune(true); // leave the debris on the ground for now
		m_idx.erase(m_idx.find(name)); 	
	}


	string_type m_name; 
	disp_attr_type m_attributes;
	
	MyPtr m_mom;
	IndexTy m_idx;
	TreeTy m_tree;
	
	bool m_prune;
	index_type m_priority; 
	display_settings m_set;
public:
	index_type m_idx_min,m_idx_max;


template<typename Ty , typename Tc> void find_picked(const index_type idx, Ty& item, Tc & branch)
	{
		index_type jj=0,ii=0, ie=m_idx.size();
//		std::cout<<"looking for "<<idx<<" at "<<string_type(branch.str())<<" with "<<m_idx_min<<" "<<m_idx_max<<"\n";
		// max can not be relied on- only root max is meaningful...
	//	if ((idx>m_idx_max)||(idx<m_idx_min)) return;
	if (ii==ie) return;
	jj=ie-1;// if there is non, thedn the lawt is it.
	MyPtr x=NULL;
	while (ii!=ie) 
	{
		x=m_tree[ii];
//		std::cout<<" recurse "<<branch.str()<<" ii=" <<ii<<" min="<<x->m_idx_min<<"\n";
		// min is sent down, max ubbles up for next min. 
		if(!x->settings().hidden()) { if (idx<x->m_idx_min) break;jj=ii; }
		ii++;
	}
	// probalby unsigned .....
	if(ii<1) {std::cout<<" bad sign looking for "<<idx<<" with min of "<<x->m_idx_min<<"\n"; 
	return;  }
	
	branch<<(jj)<<" "; m_tree[jj]->find_picked(idx,item,branch); 
	return ; 	
	}
};

//template < >bool display_tree::sset<>(const string_type & nm,  string_type & os,const bool prop)
//{  ConvStreamTy p(os);
//return sset(nm,p,prop);
//}



///////////////////////////////////////

#endif
