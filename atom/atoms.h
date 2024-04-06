#ifndef ATOMS_H
#define ATOMS_H
#include <GL/glut.h>
#include <list>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include "pdb_atom.h"
#include "atom_list.h"


#include "figure_param.h"

#include "search_strategy.h"




class atom_list_proximity
{
	typedef atom_list_proximity Myt;
public:


typedef float distance_type;
typedef pdb_atom::coord_type coord_type;
typedef int count_type;

typedef ::figure_param figure_param;
typedef ::build_point_list_param build_point_list;
typedef figure_param FpTy;
typedef build_point_list_param BPLPTy;

atom_list_proximity ( const AtomLiTy & atom_list): m_list(atom_list),m_dists(NULL),m_sz(0),m_array(0) {}	
~atom_list_proximity() { delete [] m_dists;  }	
//http://mathworld.wolfram.com/SphericalCoordinates.html

Myt &  figure( const FpTy & fp, const BPLPTy &bplp,AtomLiTy & dest);

AtomLiTy & answer() { return m_cavity_list; }	;

coord_type MinimizeAgainstTestList(pdb_atom &probe, const pdb_atom & center, const coord_type r);

//bool ContinueProbing() {  return (m_cavity_list.size()<50); }
coord_type Energy(const pdb_atom & probe);
count_type MakeTestList(const pdb_atom& center, const coord_type r);


const AtomLiTy & m_list;	
AtomLiTyItor m_itor;	
	
AtomLiTy  m_cavity_list;
AtomLiTy m_test_list,m_z_list;
distance_type * m_dists;	
count_type m_sz,m_array;
	};




#endif


