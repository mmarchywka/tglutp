#ifndef ATOM_DISPLAY_H
#define ATOM_DISPLAY_H

#include "../atom/atom_display_tr.h"
#include "../atom/core_cmd_handler.h"
#include "../atom/atom_cmd_handler.h"





class atom_display : public atom_display_tr
{
//public:
	typedef atom_display Myt;
	
	static Myt * r_one;
atom_display():cch(0),ach(0) {}

static void rone() {  static Myt x=Myt(); if (r_one==NULL) r_one=&x;  }

core_cmd_handler * cch;
atom_cmd_handler * ach;

public:	
// The theory is that when rone() gets deleted by the runtime, these get called...
~atom_display() { delete cch; delete ach; }

template <class Ty> static int display(int argc, char *argv[],Ty & global_config)
{
	rone();
	name_type rf="radius_file", pf="pdb_file",cf="color_file",
		fig="figure_param", plp="point_list_param";
	mmdd::dout(MMDDLOC)<<"Reading config file "<<argv[2]<<kluge::nl; 
if (argc>=3) {
	DaCoTy::input_type * is=DaCoTy::open(name_type(argv[2]));
	if (!is->good()) {  
		cout<<"Problem with config file "<<name_type(argv[2])<<"\n"; cout.flush(); 
	return 65;}
	//std::cout<<"asdfasdfasfasdfasdf\n";
	global_config.load(*is);delete is;
	global_config.load(argc,argv); 
}
	// who deletes this? the commander has multiple pointers to same object?
	core_cmd_handler *  cch= new core_cmd_handler(0,global_config);
	atom_io & aio=(*cch).get_io();
	atom_cmd_handler * ach= new atom_cmd_handler(&aio,global_config);
	r_one->cch=cch; r_one->ach=ach;
	if (global_config.has(pf)) 
	{
		AtomLiTy * ax=cch->LoadParameters(cf,rf,pf);
		if (!ax) 
		{
		cout<<"Need to load at least one pdb file...\n";	
		return -1;
		}
		std::cout<<" list has "<<(*aio.get_list(0)).size()<<" elements \n";
		cout<<"Making connection lists\n";
		string_type str="figure";
		if ( ax&& global_config.get(fig,str)) cch->Cavity(ax,plp,str);
	}
	else
	{
		int x=cch->LoadParameters(cf,rf);
		if (x!=0) 
		{
		cout<<"Need to load colr and radii files...\n";	
		return -1;
		}
		
	}
	ggb->set_df(atom_io::drawSphereList);
	ggb->set_ds(atom_io::drawSelectionSphereList);
	return 0;	
	}

};
	
	
#endif
