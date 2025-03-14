#include "pdb_atom.h"

typedef pdb_atom MyClass;

/*
COLUMNS      DATA TYPE        FIELD      DEFINITION
------------------------------------------------------
------------0---------------------
 1 -  6      Record name      "ATOM    "
 7 - 11      Integer          serial     Atom serial number.
13 - 16      Atom             name       Atom name.
17           Character        altLoc     Alternate location indicator.
18 - 20      Residue name     resName    Residue name.
-------------------5------------------
22           Character        chainID    Chain identifier.
23 - 26      Integer          resSeq     Residue sequence number.
27           AChar            iCode      Code for insertion of residues.
31 - 38      Real(8.3)        x          Orthogonal coordinates for X in 
                                         Angstroms
39 - 46      Real(8.3)        y          Orthogonal coordinates for Y in 
                                         Angstroms
----------------------10-------------------------------------
47 - 54      Real(8.3)        z          Orthogonal coordinates for Z in 
                                         Angstroms
55 - 60      Real(6.2)        occupancy  Occupancy.
61 - 66      Real(6.2)        tempFactor Temperature factor.
77 - 78      LString(2)       element    Element symbol, right-justified.
79 - 80      LString(2)       charge     Charge on the atom.

*/


MyClass::pdb_atom(): m_serial(0),m_x(0),m_y(0),m_z(0),m_rad(100),
	m_r(1),m_g(1),m_b(1),m_alpha(.7)
	,m_highlight(false),m_show_point(false),
	m_atom(false),m_hetero(false){}

MyClass::pdb_atom(const coord_type x,const coord_type y,const coord_type z,const coord_type r )
	: m_serial(0),m_x(x),m_y(y),m_z(z),m_rad(r),
	m_r(1),m_g(1),m_b(1),m_alpha(.7)
	,m_highlight(false),m_show_point(false),
	m_atom(false),m_hetero(false){}
//	ATOM    890  O   GLU A 114      12.300  -1.832  33.589  1.00 16.30           O
// use raw version, not sure this work....
MyClass::pdb_atom(const pdb_entry & s, RadMapTy & radius_map,  ColMapTy & color_map )
: 	m_serial(0),m_x(0),m_y(0),m_z(0),m_rad(100),
	m_r(1),m_g(1),m_b(1),m_alpha(.145)
	,m_highlight(false),m_show_point(false),m_atom(false),m_hetero(false)
{ 
	std::cout<<"not parsing this record, wrgon ctor\n\n";
	return ; 
	m_desc=s;
	pdb_entry x=s;

//	http://www.wwpdb.org/documentation/format3.0-dif.pdf
//	column based:N, CA, C, O,
//	http://bmerc-www.bu.edu/needle-doc/latest/atom-format.html
//	http://www.wwpdb.org/documentation/format23/sect9.html#ATOM
      	m_field[0]=x.substr(0,6);
      	string_type cr=m_field[0];
      	while (cr.substr(cr.length()-1)==" ") {cr=cr.substr(0,cr.length()-1); } 
      	m_field[0]=cr; m_atom=(m_field[0]=="ATOM"); m_hetero=(m_field[0]=="HETATM") ;
	m_field[1]=x.substr(6,5); m_serial=atoi(m_field[1].c_str());
	if (m_serial==0) { std::cout<<"Probable stupid error in pdb file "<<x<<"\n"; }
	m_field[2]=x.substr(12,4);m_name=m_field[2];  
 	pdb_entry junk=m_name; while (junk.substr(junk.length()-1)==" ") {
 	junk=junk.substr(0,junk.length()-1); }
	m_rad=radius_map[junk.substr(1)]; 
	if ((m_rad<.1)||(m_rad>10) )
	{if(msg_once(1)) {cout <<"pdb_atom "<<m_serial<<" radius "<<m_name<<" "<<m_rad<<"\n";} 
	if ( m_rad==0) {m_rad=1; 
	if(msg_once(2)) cout<<"pdb_atom setting radius to default, "<<m_rad<<"\n";
	}
	}{Point3 x=color_map[junk.substr(1,1)];
	m_r=x.x(); m_g=x.y(); m_b=x.z(); if (m_r+m_g+m_b==0) { m_r=1; m_g=1; m_b=1; }}
	m_field[3]=x.substr(16,1);m_field[4]=x.substr(17,3);
	m_field[5]=x.substr(21,1);m_field[6]=x.substr(22,4);
	m_field[7]=x.substr(26,1);m_field[8]=x.substr(30,8);m_x=atof(m_field[8].c_str()); 
	m_field[9]=x.substr(38,8);m_y=atof(m_field[9].c_str());
	m_field[10]=x.substr(46,8);m_z=atof(m_field[10].c_str());
	m_field[11]=x.substr(54,6);
	
	m_field[12]=x.substr(60,6);
	m_symbol=m_field[13]=x.substr(76,2);
	m_field[14]=x.substr(78,2);
	
}

MyClass::pdb_atom(const RawPtr s, RadMapTy & radius_map,  ColMapTy & color_map )
	: // These will all be set- comment out once debuggin done
	m_serial(0),m_x(0),m_y(0),m_z(0),m_rad(100),
	m_r(1),m_g(1),m_b(1),m_alpha(.145),
	m_highlight(false),m_show_point(false),m_atom(false),m_hetero(false)
{	enum {MAXLEN=4096, DUMMY=100};
	static RawTy scratch[MAXLEN]; // This makes it thread unsafe and ties up block forever...
	RawPtr sp=scratch,sp_end=sp+MAXLEN;
	RawPtr src=s; // should have a save end here...
	m_desc=s;
//	count_type brk[]={0,6,12,16,17,21,22,26,30,38,46,54,DUMMY};
	count_type brk[]={0,6,12,16,17,21,22,26,30,38,46,54,60,76,78,DUMMY};
	count_type field_ptr=0; //src_idx=0;
	RawPtr fend=s+brk[1];
	while (*src&&(sp<sp_end))
	{
	 	// src=s+brk[field_ptr+0]; // documenetation only	
		while (*src&&(sp<sp_end)&&(src<fend) )
		{RawTy c=*(src++);if (c!=' ') {*(sp++)=c; } }
		*sp=RawTy(0); sp=scratch;
		m_field[field_ptr]=scratch; 
		//std::cout<<"field :"<<m_field[field_ptr]<<" field="<<field_ptr<<"\n";
		switch (field_ptr)
		{
		case 0: { m_atom=(strcmp(sp,"ATOM")==0); 
			m_hetero=(strcmp(sp,"HETATM")==0);break;}	
		case 1: {if (!(m_serial=atoi(sp)))
		{ std::cout<<"Probable stupid error in pdb file "<<m_desc<<"\n"; } ;break;}
		case 2: {m_name=sp; break;}
		case 8:{m_x=atof(sp); break; }
		case 9:{m_y=atof(sp); break; }
		case 10:{m_z=atof(sp); break; }
		case 13:{m_symbol=sp; break; }
	};
	fend=s+brk[1+(++field_ptr)];
}
characterize(radius_map, color_map );	
}
void MyClass::characterize(RadMapTy & radius_map,  ColMapTy & color_map )
{
//	m_rad=radius_map[m_name]; 
	string_type radius_key=m_symbol;
	if (radius_map.find(radius_key)==radius_map.end())
	{
		radius_key=m_field[2].substr(0,1); 
		std::cout<<"radius problem with \n"<<m_desc<<" try name as ."<<radius_key<<".\n";		
	}
	m_rad=radius_map[radius_key]; 
	if ((m_rad<.1)||(m_rad>10) )
	 	if(msg_once(1)){cout <<"pdb_atom "<<m_serial<< " radius "<<m_desc<<" "<<m_rad<<"\n";; }
	 if ( m_rad==0) 
	 {
	 	m_rad=1; 
	 	if(msg_once(1)){cout<<"pdb_atom setting radius to default, "<<m_rad<<"\n";}
	 }
	 
	 string_type color_key=m_symbol;
	 if (color_map.find(m_name)!=color_map.end()) color_key=m_name;
	 if (color_map.find(color_key)==color_map.end())
	 	std::cout<<"No color :"<<m_desc<<"\n";
	 Point3 x=color_map[color_key];  
	m_r=x.x(); m_g=x.y(); m_b=x.z(); 
	if (m_r+m_g+m_b==0) { m_r=1; m_g=1; m_b=1; }
	
}
