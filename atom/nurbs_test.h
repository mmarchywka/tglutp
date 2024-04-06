#ifndef NURBS_TEST_HH__H
#define NURBS_TEST_HH__H

#include "../atom/pdb_atom.h"
#include "../math/various_searches.h"
#include "../atom/connectivity_list.h" // really shouldn't use, just have tempateld function...
//#include "../data_and_config.h"
#include "../debug_and_support.h"
#include <GL/glut.h>
#include <vector>
#include <algorithm>
#include <map>


class nurbs_test
{
typedef nurbs_test Myt;
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

nurbs_test(const TgtListTy * tlt)
{ LoadLists(*tlt);
cout<<"Nurbs "<<m_x.size()<<" "<<m_y.size()<<" "<<m_z.size()<<"\n";
}


void LoadLists(const TgtListTy & li);


typedef searches::Restrictor<coord_type,PtrItorTy,TgtTy,PtrListTy> Restrictor;
void add_triangles( connectivity_list * cage, const coord_type rad, const bool debuggin);

void add_neighbors( connectivity_list * cage, const coord_type rad, const bool debuggin);




//void add_x_neighbors( connectivity_list * cage, const coord_type rad, const bool debuggin)
//{ add_something<&Myt::add_something_delegate>(cage,rad,debuggin); 	 }
//typedef void (Myt:: *DelTy) (connectivity_list *,PtrItorTy&,PtrItorTy,PtrItorTy,const coord_type, const bool);

template <class coord_type,class PtrItorTy,class TgtTy,class PtrListTy>
class rti
{
	typedef rti Myt;
	typedef typename searches::Restrictor<coord_type,PtrItorTy,TgtTy,PtrListTy> Res;
	
	//template <p_type pp,bool tp_sort,bool inc,class So > 
typedef  void ( Res:: * rcomp_x_ptr)  //< &TgtTy::z,true,true,nurbs_test::compare_x >
( const coord_type, const coord_type, PtrListTy &, PtrListTy & ,  const 
nurbs_test::compare_x &  );

//const  rcomp_x_ptr z_one,z_two;
	
	Res rz,rx,ry;
	PtrItorTy  ii,ie;
	
	PtrListTy & m_z;
	const coord_type radius;
	//typedef typename TgtTy::z rtd;
	//typedef typename Res::restrict<>  asdf;
	PtrListTy m_composite,m_composite_x,m_composite_y;
	public:
	rti( PtrListTy & z,const coord_type rad_ ):
	//z_one(Restrictor::restrict< &TgtTy::z,true,true,nurbs_test::compare_x >  ),
//	z_one(Res::restrict),
//	z_two(&Res::restrict), //<int,true,false,nurbs_test::compare_x>),
	m_z(z), radius(rad_) 
		{
			Init(); if (valid()) Top(); } 
	
	PtrItorTy & begin() { return ry.m_yi; }
	PtrItorTy & end()  { return ry.m_yf; }
	const TgtTy & atom() { return **ii; }
	bool valid() { return (ii!=ie); }
	bool inc() {  Bottom(); if (valid()) Top(); return valid(); }
	
	private:
	
	bool Init() {
	ii=m_z.begin(), ie=m_z.end(); 
	cout<<"AddNay1\n"; cout.flush();
	if (ii==ie) return false;
	rz.init_itors(ii,ie);
	rz.restrict<&TgtTy::z,true,true > ( (**ii).z(),radius,m_z,m_composite,compare_x());
//	(rz.*z_one) ( (**ii).z(),radius,m_z,m_composite,compare_x());
	return true;
}
	void Top() {
	rx.init_itors(rz);
		rx.restrict<&TgtTy::x,true,true>
		( (**ii).x(),radius,m_composite,m_composite_x,compare_y());
		ry.init_itors(rx);
		pdb_atom::compare_distance::center(*ii);
		pdb_atom::compare_distance fack;
		ry.restrict<&TgtTy::y,true,true>
		((**ii).y(),radius,m_composite_x,m_composite_y,fack);
		}
	void Bottom()
	{
		++ii;
		if (ii!=ie) //       (rz.*z_two)        
		rz.restrict<&TgtTy::z,true,false> 
		( (**ii).z(),radius,m_z,m_composite,compare_x());
	}
};




// clean this up and put it somewhere...

#include "build_neighbor_map.h"

TgtListTy * m_tgt_list;
PtrListTy m_x,m_y,m_z,m_composite,m_composite_x,m_composite_y;


const pdb_atom * m_closest_hit;
typedef map<const pdb_atom*,int> TestMapTy;
typedef TestMapTy::const_iterator TestMapItorTy;
void get_boundary(AtomLiTy & dest)
{TestMapItorTy ii=m_test_map.begin(), jj=m_test_map.end();
	while(ii!=jj) dest.push_back(*(*(ii++)).first);
	
	}

TestMapTy m_test_map;

};





#endif
