#ifndef OWN_VECTOR_H__
#define OWN_VECTOR_H__
#include <vector>
#include <map>
#include <string>
#include <list>
#include "../view/display_tree.h"
#include "../debug_and_support.h"

template <class Ty> class own_vector : public vector<Ty>
{
	typedef vector<Ty> Super;
	typedef display_tree DispTy;
// fix this...
public : ~own_vector() {  Clear(); delete (Super*)(this); }
// 2008-03-18 oh well, didn't work on new compiler...
//template <class Tx> void erase(const Tx& x,const Tx&y=x ) { throw x; }

void clear() { Clear(); Super::clear(); } 

// testing templates...
//template <class index_type > 
void draw(DispTy & root)
{	typedef int index_type;
 	if (root.hidden()) return; 
 	index_type j=Super::size();
 	for (index_type i=0; i<j; ++i) 
 	{ 
 		DispTy * p=root[i];
 		if (p!=0) ((*this)[i])->draw(*(root[i])); 
 		else {mmdd::dout(MMDDLOC)<<"Report missing children- shouldnt happen\n";}
 	}
	
}

template < typename Thing, typename Ts,typename Ta,typename count_type > 
	bool model_find( Ta & root,const count_type target, Thing & thing, Ts& is )
{ count_type i=0; is>>i;
return (*this)[i]->model_find(root.get_ref(i),target,thing,is);  }

template <class Tx > Tx& names(Tx& os)const {
	typename Super::const_iterator ii= Super::begin(),jj=Super::end();
	size_t i=0; while (ii!=jj){os<<i<<" "<<(*ii)->name()<<"\n";++ii; ++i;}
return os; }

//DispTy m_disp; 
private:	
void Clear() {size_t i=Super::size(); while (i) delete (*this)[--i];}  
};



#endif

