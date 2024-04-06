#ifndef CONNECTIVITY_LIST_H_HH
#define CONNECTIVITY_LIST_H_HH

#include "pdb_chain_maker.h"
#include "../view/glut_commons.h"
#include "../math/various_searches.h"
#include "../view/display_tree.h"
#include "../atom/atom_list.h"
#include "../atom/molecule_view_list.h"
//#include "../base/points_base_classes.h"

#include <vector>
#include <map>
#include <string>
#include <list>
#include <sstream>
#include <GL/glut.h>

class connectivity_list
{
	
typedef connectivity_list Myt;
typedef int count_type;
typedef int index_type;
typedef float coord_type;
typedef index_type IdxTy;
typedef char ChTy;
typedef basic_string<ChTy> string_type;
typedef pdb_atom atom_type;
typedef const atom_type * AtomPtrTy;
typedef int flag_type;
typedef AtomLiTy AtomListTy;
typedef string_type ContigNameTy;
typedef display_tree DispTy;



typedef annotated_list<atom_type>  ChainListTy;

typedef map<ContigNameTy,ChainListTy> ChainMapTy;
typedef ChainMapTy::const_iterator ChMapItorTy;
typedef ChainMapTy::iterator MapItorTy;
typedef pair<index_type,index_type> BondTy;
typedef list<BondTy> BondStackTy;

enum {NCHAINS=3,CHAIN=0, SIDE=1,HETERO=2};
static const ChTy** chain_names()
	{
	static const ChTy* chain_names[]={"main chain","side chains","hetero and etc"};
	return chain_names; 
	}

static index_type & inst() { static index_type inst=0; return inst; }
index_type mangled_list;
mutable bool m_compile,m_compiled;
typedef index_type(atom_type:: *fs)()const; 
typedef index_type  (*FP)(const AtomListTy & , const index_type,fs);
const FP FindFunc ;//=searches::FindNear<index_type,AtomListTy,atom_type>;

public:
// turn off compiling until I have a way to determine staleness
connectivity_list(const AtomListTy & atom_list) :m_compile(false),m_compiled(false),
FindFunc(searches::FindNear<index_type,AtomListTy,atom_type>),
m_atom_list(atom_list) ,m_try_unsorted(false),m_bond_stack_name("hetero")
{mangled_list=(++inst());}
const string_type & name() const { return m_atom_list.name(); }
 void add_bond(const index_type s, const index_type d)
{m_bond_stack.push_back(BondTy(s,d));
//std::cout<<"Adding a bond "<<s<<" "<<d<<"         \r";
}
index_type size() const { return m_bond_stack.size(); }
void add_bond(const index_type s, const index_type d,const index_type r)
{m_bond_stack.push_back(BondTy(s,d));
m_bond_stack.push_back(BondTy(d,r));

// a point and a normal, something new :)
//typedef point3_template<coord_type> PointTy ;
//void add_bond(const PointTy & p, const PointTy & n

//std::cout<<"Adding a bond "<<s<<" "<<d<<" "<<r<<"         \r";
}
template < class Thing, class Ts > 
	bool model_find( display_tree & root,const count_type target, Thing & thing, Ts& is )
{ count_type i_=0; is>>i_;
if (root.hidden()) return false;
//display_type  rp=&root;
index_type j=0; //,k=0;
for (index_type i=0; i<NCHAINS; ++i){
	if (m_all[i].size()) { 
		if (i_==j) 
		{
			//display_tree & root2=root.get_ref(j);			
			MapItorTy ii=m_all[i].begin(); //, ie=m_all[i].end();
			is>>i_; if (((unsigned int)(i_))>=m_all[i].size()) 
			std::cout<<"Size wrong in conn list\n";
			display_tree & root2=root.get_ref(j).get_ref(i_);
			while (i_) {++ii; --i_; }
			return (*ii).second.model_find(root2,target,thing,is);
		}// i==j
		j++;
		}
	
	
}//nchains
std::cout<<"Connlist model_find fails with "<<i_<<" and "<<j<<"\n";
return false;
}

void draw(DispTy & root);

typedef std::ostringstream OsStreamTy;
template <class Ty> string_type  mprintf(const Ty & v) const{Ty cc=v; OsStreamTy os; os<<cc;	return os.str(); } 
void create_display_tree(DispTy * root);

void make_chains() { MakeChains();  make_bonds_only();}
void make_bonds_only(){ MakeThings(2,m_bond_stack_name);}
void make_triangles(){  MakeThings(3,m_bond_stack_name); }




void set_bond_name( const string_type & name ) {m_bond_stack_name=name; }
void set_bond_color(const count_type v, const coord_type r, const coord_type g, const coord_type b)
{ m_all[HETERO][m_bond_stack_name]=ChainAttrTy(v,r,g,b);  }
	
/*
template <class Ty> Ty & dump_chains(Ty & ss) const
{ //ChainMapTy::const_iterator 
ChMapItorTy i=m_chain_map.begin(), f=m_chain_map.end();
  while (i!=f){ 
	ss<<"Chain"<<(*i).first<<" Length of "<<(*i).second.size() <<"\n";
  ++i; }
	return ss;
	
}
*/
private:
void MakeChains();

index_type FindSequential(const index_type target);

void MakeThings(const index_type v, const string_type &name);

public:
// source list for connectivity stuff
const AtomListTy & m_atom_list;
// map of lists
ChainMapTy m_all[NCHAINS]; //chain, side,hetero

// waiting to be added.
BondStackTy m_bond_stack;
bool m_try_unsorted;

string_type m_bond_stack_name;

DispTy m_disp; 

};

typedef connectivity_list ConListTy;
typedef own_vector<  ConListTy * > ConnLiSTy;


#endif
