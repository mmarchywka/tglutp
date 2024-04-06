#ifndef PDB_ATOMS_H
#define PDB_ATOMS_H
#include <math.h>
#include <cmath>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h> // raw strings faster...
// needed to pickup "mypoint3"...
// this still isn't right- camera is needed to ket "Your" point3
// but this canbe moved to the base calss thing and , in any case, needs to be fixed. 
//#include "../view/camera_and_lights.h"
#include "../base/points_base_classes.h"
#include "../view/mathutil.h"

typedef std::basic_string<char> name_type;
typedef name_type AtomNameTy;
typedef float RadTy;
typedef map<AtomNameTy,RadTy> RadMapTy;

typedef map<AtomNameTy,Point3> ColMapTy;


//class ViewInfo;
//class ViewParam;
class pdb_atom
{
//      friend ViewInfo  operator*(const ViewParam & vp, const pdb_atom & pp);	
	typedef pdb_atom Myt;
	
	typedef float color_type;
	
	typedef char Ch;
	
typedef basic_ostream<Ch> OstreamTy;
	
	
public:	



typedef int count_type;
//typedef int flag_type;
	typedef int flag_type; 
typedef float coord_type;
typedef std::basic_string<char> pdb_entry;  
typedef pdb_entry::size_type pdb_sz;
typedef pdb_entry string_type;


class compare_serial { public:  int operator () (const Myt & i, const Myt & j) 
const {return (i).serial()<(j).serial();}
int operator () (const Myt *& i, const Myt *& j) 
const {return (*i).serial()<(*j).serial();}
};
class compare_distance { public: 
// the cast is for const...
static const Myt & center(const Myt* x) { static  Myt const *  cn=0; if (x) cn=(Myt*)(x);  return *cn; } 
//const Myt & m_center;
//compare_distance(const Myt & center) : m_center(center) {}
 int operator () (const Myt & i, const Myt & j) 
const {return (i).dist(center(0))<(j).dist(center(0));}
int operator () (const Myt * const & i, const Myt * const & j) 
const {return (*i).dist(center(0))<(*j).dist(center(0));}
};

static bool msg_once(flag_type bit) {  static flag_type onced=0; 
bool setted=((onced&bit)!=0); onced|=bit;return !setted; }

static string_type field_name(const count_type i)
// 2024 const
{ static const Ch* nm[]=	{"type","serial","atom","na",
					"residue","chain","seq", 
					"na","x","y","z","occupancy","temp",
					"element","charge","" };
return string_type(nm[i]);
}




coord_type x() const { return m_x; } coord_type y() const { return m_y; } coord_type z() const { return m_z; } 
coord_type xn() const { return m_x+m_t.m_nx; } 
coord_type yn() const { return m_y+m_t.m_ny; } 
coord_type zn() const { return m_z+m_t.m_nz; } 

Point3  loc() const { return Point3(m_x,m_y,m_z); } 
myPoint4<float>  get_color_v() const { return myPoint4<float>(m_r,m_g,m_b,m_alpha); }
myPoint3  get_color_3() const { return myPoint3(m_r,m_g,m_b); }
Point3  get_color() const { return Point3(m_r,m_g,m_b); } 
 //const myPoint3& 
operator myPoint3() const { return myPoint3(m_x,m_y,m_z); }
typedef char RawTy;
typedef RawTy * RawPtr;
// pseduo atom stuff 
pdb_atom();

pdb_atom(const coord_type x,const coord_type y,const coord_type z,const coord_type r );

//	ATOM    890  O   GLU A 114      12.300  -1.832  33.589  1.00 16.30           O
pdb_atom(const pdb_entry & s, RadMapTy & radius_map,  ColMapTy & color_map );

pdb_atom(const RawPtr s, RadMapTy & radius_map,  ColMapTy & color_map );


void characterize(RadMapTy & radius_map,  ColMapTy & color_map );

pdb_entry field(const count_type i ) const { return m_field[i]; }	
count_type serial() const { return m_serial;}
count_type incr() { return ++m_serial;}
flag_type naive_class() const 
{ 
	flag_type i=0;
	i=(i<<1)|(m_highlight?1:0);
	i=(i<<1)|(m_show_point?1:0);
	i=(i<<1)|(m_hetero?1:0);
	i=(i<<1)|(m_atom?1:0);
	return ( i ); }




coord_type coords(const Myt & that) const
{	m_t.xx_x=m_x-that.m_x;m_t.xx_y=m_y-that.m_y; m_t.xx_z=m_z-that.m_z;
	return m_t.coord();
}


Myt & orbit_approx(const Myt& centre, const coord_type dr, const coord_type dtheta, const coord_type dphi)
{
	coord_type ct=cos(m_t.xx_theta), st=sin(m_t.xx_theta), cp=m_t.xx_z/m_t.xx_r,sp=sin(m_t.xx_phi);
	coord_type dx=ct*sp*dr-m_t.xx_r*(st*sp*dtheta+ct*cp*dphi); m_x+=dx;
	dx=st*sp*dr+m_t.xx_r*(ct*sp*dtheta+st*cp*dphi); m_y+=dx;
	dx=dr*cp-m_t.xx_r*sp*dphi; m_z+=dx;
	return *this;
}
inline Myt & oriented_toward(const Myt & neigh, const coord_type d)
{
	coord_type sf=d/coords(neigh); // expensive but localizes code for now...
	m_t.m_nx=-m_t.xx_x*sf; m_t.m_ny=-m_t.xx_y*sf; m_t.m_nz=-m_t.xx_z*sf;
	m_t.derive();	
		
	return *this;
}
//Myt & oriented_toward(const Myt & neigh, const coord_type d)
//{m_t.oriented_toward(neigh,d); return *this; }
//coord_type coords(const Myt & that) const {return m_t.coords(that); }
coord_type theta() const { return m_t.theta(); }
coord_type phi() const { return m_t.phi();}


template<class Ty> void dump_loc(Ty& ss)const  {ss<<m_x<<","<<m_y<<","<<m_x; }

const coord_type radius() const { return m_rad; }
const coord_type radius(const coord_type r) { m_rad=r; return m_rad; } 
const name_type desc() const { return m_desc; }
coord_type dist(const Myt& that) const
{ 
	coord_type dx=m_x-that.m_x;
	coord_type dy=m_y-that.m_y;
	coord_type dz=m_z-that.m_z;
	return sqrt( dx*dx+dy*dy+dz*dz);
}
/*
coord_type dist(const point3_template<coord_type> & that) const
{ 
	coord_type dx=m_x-that.m_x;
	coord_type dy=m_y-that.m_y;
	coord_type dz=m_z-that.m_z;
	return sqrt( dx*dx+dy*dy+dz*dz);
}*/
coord_type theta(const Myt& that) const
{  return atan2(m_x-that.m_x,m_y-that.m_y); }
coord_type phi(const Myt& that) const
{  return acos((m_z-that.m_z)/dist(that)); }



void show(flag_type flags ) const
{show(cout,flags,m_name); }
void show ( OstreamTy & out, flag_type flags,const name_type & nm) const
{
	if (flags&1) { out<<nm<< " x="<<m_x<<" y= "<<m_y<<" z="<<m_z;}
	 if (flags&2) { out<<" r= "<<m_rad; } 
	if (flags&!(flags&256)) {out<<"\n";}
	}


Myt & set_distance( const Myt & center, const coord_type r)
{
	coord_type rx=dist(center);
	// make it predictable... 
	if (r<0) { cout<< " r<0 at set_distance in pdb_atom with "<<center.m_name<<"\n";}
	if (rx<.00001*r) { m_z=center.m_z+r; m_x=center.m_x; m_y=center.m_y; return *this; }
//	cout<<"set distance to "<<r<< " and use rx= "<<rx<<"\n";
	coord_type sf=(r-rx)/rx;
	m_x+=(m_x-center.m_x)*sf; 
	m_y+=(m_y-center.m_y)*sf; 
	m_z+=(m_z-center.m_z)*sf; 
	
	return *this; 
	}
inline coord_type clearance(const Myt& that) const
{ 
	coord_type dx=m_x-that.m_x;
	coord_type dy=m_y-that.m_y;
	coord_type dz=m_z-that.m_z;
	return sqrt( dx*dx+dy*dy+dz*dz)-radius()-that.radius();
}

inline bool clears(const Myt& that) const
{ 
	coord_type limit=radius()+that.radius();
	limit=limit*limit;
	coord_type dx=m_x-that.m_x; // sq as abs slow bu helps later..
	dx*=dx; if ( dx>limit) return true; 
	coord_type dy=m_y-that.m_y;
	dy*=dy; dx+=dy; if ( dx>limit) return true;
	coord_type dz=m_z-that.m_z;
	// awful typo ::: dz*dz+dZZZZZ
	return dz*dz+dx>limit; // well, at least no sqrt();... 
}

inline void move(const coord_type x,const coord_type y,const coord_type z ) { m_x+=x; m_y+=y; m_z+=z; }
inline void move_to(const coord_type x,const coord_type y,const coord_type z ) { m_x=x; m_y=y; m_z=z; }
inline void move_to(const myPoint3 & mp ) 
{ m_x=mp.x(); m_y=mp.y(); m_z=mp.z(); }
inline void color(const color_type x,const color_type y,const color_type z ) { m_r=x; m_g=y; m_b=z; }
inline void highlight(bool hl) { m_highlight=hl;}void show_point(bool hl) { m_show_point=hl;}
inline void color(const Myt & that ) { m_r=that.m_r; m_g=that.m_g; m_b=that.m_b; }
Myt & place(const Myt & that) { m_x=that.m_x; m_y=that.m_y; m_z=that.m_z; return *this;}
// Not really members...



count_type m_serial;
coord_type	m_x,m_y,m_z,m_rad;
color_type    m_r,m_g,m_b,m_alpha;
pdb_entry m_desc;
pdb_entry m_field[12+3];
name_type m_name;
name_type m_symbol;
count_type m_loc;
bool m_highlight,m_show_point,m_atom,m_hetero; 
// Stored temps that are expensive to recompute- no warantee,
// use plenty of Asserts...
class ScratchPad {
public:
// Initialization is a waster here except for normals 
// but also now the display code seems inconsistnet if m_dot_z !=0
ScratchPad():m_nx(0),m_ny(0),m_nz(1),m_dot_z(0) {}
mutable coord_type xx_x,xx_y,xx_z,xx_r, xx_theta,xx_phi;
mutable coord_type m_nx,m_ny,m_nz,m_mag,m_curl_x,m_curl_y,m_dot_z;
coord_type coord() {xx_r=sqrt( xx_x*xx_x+xx_y*xx_y+xx_z*xx_z);
	xx_theta=atan2(xx_x,xx_y); xx_phi=acos(xx_z/xx_r);
	return xx_r;}
void derive() const 
{
	m_mag=sqrt(m_nx*m_nx+m_ny*m_ny+m_nz*m_nz);
	m_curl_y=m_nx/m_mag; m_curl_x=-m_ny/m_mag;
  	m_dot_z=180/3.14159*acos(m_nz/m_mag);
  	        }
coord_type theta() const { return xx_theta; }
coord_type phi() const { return xx_phi;}

 };

mutable ScratchPad m_t;






}
;

  



#endif
