#ifndef PDB_LOADER_H__
#define PDB_LOADER_H__

#include "pdb_atom.h"
#include "atom_list.h"
#include "connectivity_list.h"
#include "../data_and_config.h"
#include "../debug_and_support.h"
// using Tr...
#include "../view/draw_from_tree.h"

#include <iostream>
////////////////////////////////////////////////////////////
typedef data_and_config::input_type IsTy;
class myRadMapTy : public RadMapTy{
public:
IsTy & load(IsTy & is)
{ 	name_type x=""; RadTy y=0;
 	while (!is.eof()) { is>>x;  is>>y; (*this)[x]=y; }
	return is;
}

}; //  radius_map;

class myColMapTy : public ColMapTy
{
	public:
	
IsTy & load(IsTy & is)
{ 	name_type x=""; Point3 y; float r,g,b;
	while (!is.eof()) { is>>x;   is>>r; is>>g; is>>b; (*this)[x]=Point3(r,g,b);}
	return is;
}
	} ;// color_map;
////////////////////////////////////////////////////////////////


class pdb_reader : public draw_from_tree_TR
{
	myRadMapTy radius_map;
	myColMapTy color_map; 
	
	public:
	typedef data_and_config::input_type IsTy;
	
	void load (IsTy * cmis, IsTy * rmis)
	{color_map.load(*cmis); radius_map.load(*rmis); 
		
		}
	
	IsTy & loadSphereList(AtomLiTy & atom_list, IsTy & is,const config_type & cf,ConListTy * cl=NULL)
{
	
	
	Point3 sigma; 	char stupid[MM_MAX_LINE_LENGTH];
	name_type x="";
//	cout<<"HARDCODED RADIUS RESTRICTIONFOR TB PROTEASOME!!!!!\n\n";
	pdb_atom rcenter;
	bool center_restrict=false;
	bool hetok=true;
	string_type str="restrict_input";
	if (cf.has(str))
	{
		std::cout<<"Restricting input range,,,\n";
		center_restrict=true; 
		float rad;
		myPoint3 mp;
		cf.parse(str,"radius",rad);
		cf.parse(str,"center",mp);
		cf.parse(str,"hetok",hetok);
	rcenter.radius(rad); rcenter.move_to(mp.x(),mp.y(),mp.z());
	}
while (!is.eof())
{
//	 is>>x; cout<<x; cout<<flush;	
	is.getline(stupid,MM_MAX_LINE_LENGTH,kluge::nl);
	x=name_type(stupid);
//	cout<<x<<"\n";
	if (x.length()<6) continue; 
	if (x.substr(0,6)=="COMPND")
	{ std::cout<<x;}	
//	atom_list.push_back(pdb_atom());
	//}
	if ((x.substr(0,4)=="ATOM")||(x.substr(0,6)=="HETATM")&&hetok)
	{ 
//		pdb_atom  p = pdb_atom(x,radius_map,color_map);
		pdb_atom  p = pdb_atom(stupid,radius_map,color_map);
		if (!center_restrict|| center_restrict&&!p.clears(rcenter)) 
		atom_list.push_back(p); 
	    sigma+=p.loc(); 
	}		
	else if (cl)
	{
		typedef int index_type;
		if ((x.substr(0,6)=="CONECT")){
		//	cout<<x<<"\n";
		const index_type src=atoi(x.substr(7,4).c_str());
		const index_type dst=atoi(x.substr(12,4).c_str());
		if (!(src&&dst)) { std::cout<<"Bad read on this :"<<x<<"\n"; }
		cl->add_bond(src,dst); 
		} //if connect
		} // if cl
		} // while 
	
	
return is;	
	}
	
	
};

#endif
