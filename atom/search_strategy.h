#ifndef SEARCH_STRATEGY_H
#define SEARCH_STRATEGY_H

#include "pdb_atom.h"
#include "debug_and_support.h"
#include "atom_list.h"
#include "../atom/figure_param.h"

#include <vector>
#include <algorithm>
#include <map>

class search_strategy
{
typedef search_strategy Myt;
typedef pdb_atom target_type;

// get common stuff from somewhere
typedef target_type Stuff;
typedef Stuff::coord_type coord_type;
typedef Stuff::count_type count_type;

typedef target_type TgtTy;
typedef const TgtTy * TgtPtr;
typedef std::vector<TgtTy> TgtListTy;
typedef TgtListTy::iterator TgtItorTy;
typedef TgtListTy::const_iterator TgtCItorTy;
typedef std::vector<TgtPtr> PtrListTy;
typedef PtrListTy::iterator PtrItorTy;
typedef PtrListTy::const_iterator PtrCItorTy;

class compare_x { public:  int operator () (const TgtPtr & i, const TgtPtr & j) 
const {return (*i).x()<(*j).x();}};
class compare_y { public:  int operator () (const TgtPtr & i, const TgtPtr & j) 
const {return (*i).y()<(*j).y();}};
class compare_z { public:  int operator () (const TgtPtr & i, const TgtPtr & j) 
const {return (*i).z()<(*j).z();}};

public:

search_strategy(): m_tgt_list(NULL ) {}


void LoadLists(const TgtListTy & li);

PtrItorTy m_zi,m_zf,m_xi,m_xf,m_yi,m_yf;
bool sorting_x,sorting_y;
void InitialRestrict(const coord_type z, const coord_type radius);
void UpdateComposite(const bool inc,const coord_type z, const coord_type radius);
void Restricty(const bool inc, const coord_type y, const coord_type radius);
void Restrictx(const bool inc, const coord_type x, const coord_type radius);
void IncrementalRestrict(const coord_type z, const coord_type radius);
void IncrementalRestrict(const coord_type x,const coord_type z, const coord_type radius);
void BuildPointList( pdb_atom& probe,     AtomLiTy & dest           //PointListTy & point_list 
	,const build_point_list_param &bplp= build_point_list_param());
void BuildPointListMap( pdb_atom& probe
	,const build_point_list_param &bplp= build_point_list_param());

inline bool FitCheckShell(const PtrListTy & al, const pdb_atom & probe, const coord_type sh, coord_type & naive_delta)
{	coord_type cl=1000000;
	for ( PtrCItorTy i=al.begin(); i!=al.end(); ++i)
	{ // The minimum amount worth checking again on failure is -c(>0)	
		coord_type c=probe.clearance(**i); if (c<0) { naive_delta=-c; return false;	}
		if (c<cl) {cl=c;  m_closest_hit=(*i); }
		}
	// if we aren't close enough, the minimum amount worth trying again
	// is cl(>0)-sh
	naive_delta=cl-sh;
	return (cl<sh);;
} 
TgtListTy * m_tgt_list;
PtrListTy m_x,m_y,m_z,m_composite,m_composite_x,m_composite_y;


const pdb_atom * m_closest_hit;
typedef map<const pdb_atom*,int> TestMapTy;
typedef TestMapTy::const_iterator TestMapItorTy;
void get_boundary(AtomLiTy & dest)
{TestMapItorTy ii=m_test_map.begin(), jj=m_test_map.end();
	while(ii!=jj) dest.push_back(*(*(ii++)).first);
AtomLiTy::iterator ia=dest.begin(), ea=dest.end();
std::sort (ia,ea,pdb_atom::compare_serial());	
	}

TestMapTy m_test_map;

};





#endif
