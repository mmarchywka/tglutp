#ifndef ATOM_LISTS_H
#define ATOM_LISTS_H

#include "../view/glut_mol_gui.h"
#include "pdb_atom.h"
#include "atom_list.h"
#include "../data_and_config.h"
#include "connectivity_list.h"
#include "../atom/raw_points_list.h"
//#include "./base/point_base_classes.h"
#include "./view/display_tree.h"

#include "../atom/pdb_reader.h"

#include <GL/glut.h>
#include <list>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <math.h>
#include <vector>

typedef data_and_config::input_type IsTy;

// using Tr...
#include "../view/draw_from_tree.h"




class atom_io : public draw_from_tree_TR
{
	typedef atom_io Myt;
public:
	static Myt*  r_one;
	pdb_reader m_pr;
	
protected:
	AtomLiSTy atom_lists;
	ConnLiSTy conn_lists; 
	RawLiSTy point_lists;
	
	display_tree m_root;
	display_tree *m_atom_branch,* m_conn_branch, *m_point_branch;
public:

atom_io() {
		r_one=this; 
	m_atom_branch=m_root.get_child("atom lists");
	m_conn_branch=m_root.get_child("conn lists");
	m_point_branch=m_root.get_child("point lists");
}

	display_tree & tree() { return m_root; } 
	void model_draw(display_tree& root); 
	
	const AtomLiSTy & get_list() { return atom_lists; }
	//const 
	enum {SIGNBIT=~((-1)>>1)};
	AtomLiTy *  get_list(const count_type i) 
	{ 	if (((i&SIGNBIT)!=0)||(i>=count_type(atom_lists.size()))) 
		{	std::cout<<"Note that the request failed sign bit test :"<<i<<"\n";
		return NULL;}
		return atom_lists[i]; 
	}
	const ConnLiSTy & get_conn() { return conn_lists; }
	const ConListTy * get_conn(const count_type i) 
	{ 	if ((i<0)||(i>=count_type(conn_lists.size()))) return NULL;
		return conn_lists[i]; 
	}
	static string_type serial_name()
	{ static int serial=0; serial++; OsStreamTy r; r<<serial; return r.str(); } 
	void push_atom( AtomLiTy *x ) 
	{	atom_lists.push_back(x);
		x->create_display_tree(m_atom_branch);
	}
	void push_conn( ConListTy *x ) 
	{	conn_lists.push_back(x);
		x->create_display_tree(m_conn_branch);
	}
	const RawListTy * get_raw(const count_type i) { 
		if ((i<0)||(i>=count_type(point_lists.size()))) return NULL;
		return point_lists[i]; 
	}
	RawListTy * get_raw_rw(const count_type i) { 
		if ((i<0)||(i>=count_type(point_lists.size()))) return NULL;
		return point_lists[i]; 
	}
	void push_point( RawListTy *x ) 
	{	point_lists.push_back(x);
		x->create_display_tree(m_point_branch);
	}
	
	template < class Thing, class Ts > 
	bool model_find( display_tree & root,const count_type target, Thing & thing, Ts& is )
	{ count_type br=0; is>>br;   
	// note we haven't []'ed the tree yet as this will mess up the min things... 
		if (br==2) return 
			point_lists.model_find(*m_point_branch,target,thing,is);
		if (br==1) return conn_lists.model_find(*m_conn_branch,target,thing,is);
		return atom_lists.model_find(*m_atom_branch,target,thing,is);
	}
	
	static void drawSphereList();
    	static void drawSelectionSphereList(int x,int y );

	typedef GLuint gl_count_type;
	typedef data_and_config DaCoTy;
	// typedef DaCoTy::flag_type flag_type;	
	// typedef DaCoTy::coord_type coord_type; 
	typedef DaCoTy::count_type count_type;
	

IsTy & loadSphereList(AtomLiTy & atom_list, IsTy & is,const config_type & cf,ConListTy * cl=NULL)
{
	return m_pr.loadSphereList(atom_list,is,cf,cl);		
}

private:
vector<pdb_atom> m_pick_history; 


};

#endif

