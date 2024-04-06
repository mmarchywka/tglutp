#ifndef ATOM_LIST_HH_H
#define ATOM_LIST_HH_H

#include "../view/display_tree.h"
#include "own_vector.h"
#include "../view/glut_commons.h"
#include "../atom/pdb_atom.h"

#include <vector>
#include <map>
#include <string>
#include <list>


typedef vector<pdb_atom> ParentAtomLiTy;

//
class atom_container : public ParentAtomLiTy
{
	typedef atom_container Myt;
	typedef ParentAtomLiTy Super;
	typedef ParentAtomLiTy::iterator MyItorTy;
	typedef myPoint3 MyPtTy;
	typedef int index_type;
	// a list of >
	
public:
//	typedef int flag_type;
	typedef pdb_atom atom_type;	
	typedef atom_type::coord_type coord_type;
	typedef atom_type::flag_type flag_type;
	typedef atom_type::pdb_entry string_type;
	typedef display_tree DispTy;
private:
	typedef list<atom_type*> ChildIndTy;
	typedef ChildIndTy::iterator ChildItorTy;
	enum {LTYPES=3,ATOM=0,HETERO=1,OTHER=2};
	ChildIndTy m_all[LTYPES];
	GLUquadricObj* xxx;
public:
atom_container(const string_type nm):Super(),m_name(nm) {xxx=gluNewQuadric();}
atom_container(const string_type nm,const Myt & that) :Super(),m_name(nm)
 {(*this)=that; m_name=nm;xxx=gluNewQuadric();}
atom_container():Super(),m_name("default") {xxx=gluNewQuadric();}
~atom_container() { //delete xxx; 
}
const string_type & name() const { return m_name;}
//template <class Ty> string_type  mprintf(const Ty & v) const{Ty cc=v; OsStreamTy os; os<<cc	return os.str(); } 
void create_display_tree(DispTy * root);

bool topo(const atom_type & p, const flag_type flag) const;

void find_hull();
void draw();
template < typename Thing, typename Ts,typename Ct > 
bool model_find( display_tree & root,const Ct target, Thing & thing, Ts& is )
{ 	typedef Ct count_type; 
	count_type i=0; is>>i;
	//return (*this)[i]->model_find(root.get_ref(i),target,thing,is);  
	count_type k=0;
	for ( index_type j=0; j<LTYPES; ++j)
 	{
	if (m_all[j].size()!=0)
 	{	
 		if (i==k)
 	 	{      count_type roff=target-root.get_ref(i).m_idx_min;
 	 		ChildItorTy ii=m_all[j].begin(), ie=m_all[j].end();
 	 		while (roff) 
 	 		{   ++ii; if(ii==ie){cout<<"problem finding "<<target<<"\n";
 	 			root.get_ref(i).lineage(std::cout,0);}
 	 		--roff;
 	 		}
 	 		if (roff==0) {thing.push_back(**ii); return true;}
 		}
 	k++; 
	}
}
std::cout<<"Got here but no hits with i="<<i<<" k="<<k<<"\n";
return false;
}
	
	
void draw(DispTy & root);
void DrawSpheres( DispTy & disp, ChildIndTy & li);

void DrawDisks( DispTy & disp, ChildIndTy & li);
void set_to_atom(const atom_type & p,const display_settings& ds) ;
void DrawPoints( DispTy & disp, ChildIndTy & li);
void DrawEnd();
void DrawBegin(const display_settings & ds); 

	
	coord_type m_x,m_y,m_z,m_X,m_Y,m_Z;
	MyPtTy m_cg;
	string_type m_name;
	
};




typedef atom_container AtomLiTy;
typedef AtomLiTy::iterator AtomLiTyItor;
typedef AtomLiTy::const_iterator AtomLiTyItorConst;
//typedef point_list_container PointListTy;






typedef own_vector< AtomLiTy * > AtomLiSTy; 





#endif
