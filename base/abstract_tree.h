#ifndef ABSTRACT_TREE_H__
#define ABSTRACT_TREE_H__
#include <iostream>
#include <math.h>
#include <map>
#include <vector>
#include <iostream>

// pickup tr for now
#include "../view/display_settings.h"

class abstract_tree: public display_tr
{
	typedef abstract_tree Myt;
	typedef Myt * MyPtr;
// The map is slow and nice, the vector fast and cryptic.
	typedef map<name_type,index_type> IndexTy;
	typedef vector<MyPtr> TreeTy;
	typedef TreeTy::iterator TreeItorTy;
	typedef IndexTy::const_iterator IndexItorTy;

public:
	abstract_tree(const string_type &nm="Root")
	:m_name(nm),m_mom(0), m_idx(),m_tree(){}
	// User only makes roots of calls to make a parented child.
private:
	abstract_tree(const MyPtr mom,const string_type &nm):m_name(nm),
	m_mom(mom), m_idx(),m_tree(0) {}
	
	public:
	~abstract_tree()
	{ if (m_tree.size()) cout<<"Display tree not delted "<<m_name<<"\n";}
const MyPtr mom() const { return m_mom; }
void adopt(const string_type & nm) { Adopt(nm);  }
string_type name() const { return m_name; }
count_type size() const { return m_idx.size(); }

template < class Ty> void traverse(Ty & ss,const count_type lvl) const
	{ Traverse(ss,lvl);	}
template < class Ty> void lineage(Ty & ss,const count_type lvl) const
	{ Lineage(ss,lvl);	}
	// if you insist on name calling, expect a little indirection 
MyPtr get_child( const string_type & name) { Adopt(name); return m_tree[m_idx[name]];}
Myt&  get_ref(const count_type i ) const { return *(m_tree[i]);}
MyPtr operator[] (const index_type i) const
	{ if ( i<m_tree.size())
	{  MyPtr ch= m_tree[i];	
		// The min goes downward and max is always on root
		// after bubbling back up. 
		ch->m_idx_min=m_idx_max; 
		ch->m_idx_max=ch->m_idx_min;
		return ch;
	}
		std::cout<<" Tree index "<<i<<" greater than size "<<m_tree.size()<<"\n";
		Lineage(std::cout,0);
		return 0;
	}
	void sprout_max() 
	{ MyPtr x=m_mom; while (x) {x->m_idx_max=m_idx_max; x=x->m_mom; } }
	// not sure which way to go yet, right now just make sure they are consistent...
	void remove(const string_type & name) {Remove(name); }
	void clean() 
	{
	{	TreeItorTy ii=m_tree.begin(), ie=m_tree.end();
		while (ii!=ie) { (*ii)->clean();  delete (*ii);   ++ii;  }
	}
	}	

bool ssets(const string_type & nm, const  string_type & os,const count_type prop)
{  ConvStreamTy p(os);	return sset(nm,p,prop);
}
template <class Ty>bool sset(const string_type & nm, Ty& os,const count_type prop)
{  OsStreamTy oss;
//std::cout<<" have a prop of "<<prop<<"\n";
//	if (nm=="prune") {os>>m_prune; std::cout<<" setting prune ";}
//	else if (nm=="priority"){ os>>m_priority; oss<<m_priority; 
  //         std::cout<<" setting priority ";}
    //     else if (nm=="attr"){ os>>m_attributes; oss<<m_attributes;
     //      std::cout<<" setting attr ";}
//	else 
	if (m_set.sset(nm,os,prop,oss))
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



template <class Tx,class Ty>  Myt & set ( Tx (Myt:: *thing), const Ty & val, const bool prop)
{	this->*thing=val; if (prop) Pgate(thing,val);return *this; }

private:

template <class Tx,class Ty> void Myt::Pgate( Tx (Myt:: *thing), const Ty & val)
{
	this->*thing=val;
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
	MyPtr m_mom;
	IndexTy m_idx;
	TreeTy m_tree;
public:
	index_type m_idx_min,m_idx_max;


template<typename Ty , typename Tc> void find_picked(const index_type idx, Ty& item, Tc & branch)
{
	index_type jj=0,ii=0, ie=m_idx.size();
	if (ii==ie) return;
	jj=ie-1;
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

};


sample_tree : public abstract_tree
{	typedef sample_tree Myt;
	typedef abstract_tree Super;
	
	typedef DrawTy;
	typedef OperTy;
	
public :
	sample_tree(const string_type &nm="Root") :Super(nm){}
	
	virtual void draw()=0;	
	virtual void do_something()=0; 
protected:
	DrawTy m_settings;
	OperTy m_operand;
	
};
template <class Ty> sample_tree_of : public sample_tree
{	typedef sample_tree_of Myt;
	typedef sample_tree Super;
	typedef Super::Super Base;	
	typedef Ty MyTy;
	typedef MyTy * MyPtr;
	
public:
	sample_tree(MyPtr tgt, const string_type &nm="Root") :Super(nm),m_ptr(tgt){}
	void draw(){m_ptr->draw(m_settings); }
	void do_something(){m_ptr->do_something(m_operand); } 
	
private :
	TyPtr m_ptr;	
	
};


#endif
