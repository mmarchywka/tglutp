#ifndef HYBRID_PDB_LOADER_H__
#define HYBRID_PDB_LOADER_H__
#include "global.h"
#include "../atom/pdb_atom.h"

#include "pdb_hybrid_typedefs.h"

#include <iostream>
////////////////////////////////////////////////////////////
typedef pdb_typedefs::IsTy IsTy;

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


class hybrid_pdb_reader : public pdb_typedefs
{
	myRadMapTy radius_map;
	myColMapTy color_map; 
	
	public:
	void load(const Chr * col, const Chr * rad)
	{	
		IsTy * cp=open_in(col);
		IsTy * rp=open_in(rad);
		load(cp ,rp);	
		delete cp;
		delete rp;
	}
	void load (IsTy * cmis, IsTy * rmis)
	{color_map.load(*cmis); radius_map.load(*rmis); }
	
	template <class Ty> IsTy & load_list(Ty & al, IsTy & is)
	{
	enum {MAX_LINE_LENGTH=256};
	Point3 sigma; 	char stupid[MAX_LINE_LENGTH];
	name_type x="";
	const bool hetok=true; 
	while (!is.eof())
	{
		is.getline(stupid,MM_MAX_LINE_LENGTH);
		x=string_type(stupid);
		if (x.length()<6) continue; 
		if (x.substr(0,6)=="COMPND") { std::cout<<x<<'\n'; if (x.length()>6) al.name(x.substr(6)); }	
		if ((x.substr(0,4)=="ATOM")||(x.substr(0,6)=="HETATM")&&hetok)
		{ 
			pdb_atom  p = pdb_atom(stupid,radius_map,color_map);
			al.push_back(p); 
	    	//sigma+=p.loc(); 
		}		
	} // while 
	return is;	
}
	
	
};






#endif
