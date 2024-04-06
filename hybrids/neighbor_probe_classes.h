#ifndef NEIGHBOR_PROBE_CLASSES_H__
#define NEIGHBOR_PROBE_CLASSES_H__





#include <math.h>
#include <vector>
#include <map>
#include "../base/points_base_classes.h"

class mr_rogers 
{
		// this keeps track of the situation in the neighborhood.
		// neighboors, user defined, for a given index are stored in NaMap
		// while an overall stat, such as obscured area, is kept in HoMap
	typedef int IdxTy;
	typedef double FlTy;
public:
	// This is a list of nieghbors
	typedef std::vector<IdxTy> NayTy;
	// This is what we should inherit, a list of neighbors for each member
	typedef std::map<IdxTy, NayTy> NaMap;
	// This is a list of metrics for each member's 'hood. 
	// Right now, this is gross area obscured by neighbors ( out of 4*pi ) 
	typedef std::map<IdxTy, FlTy> HoMap;
	// nx,ny,nz, cos(alpha)
	typedef myPoint4<float > DetTy;
	typedef std::vector<DetTy> Dv;
	typedef std::map<IdxTy, Dv> DetMap;

	NaMap & n() { return m_neighbors; }
	NaMap & ne() { return m_expo_neighbors; }
	HoMap & h() { return m_hood; } 
	DetMap & d() { return m_det; } 
	DetMap & s() { return m_samp; } 
	void clear() { 
		m_neighbors.clear(); 
		m_expo_neighbors.clear(); 
		m_hood.clear(); 
		m_det.clear(); 
		m_samp.clear();
	}
private:
	// key is atom, value is a vector of indicies of all neighbors
	NaMap m_neighbors;
	// These are indicies of exposed neighbors ( surface atoms near this one).
	NaMap m_expo_neighbors;
	// a summary number for the crowdedness ( obscuration ) around key atom
	HoMap m_hood;
	// detailed obscuration map- stores a vector containing normal direction
	// and subtended anble of each offending neighbor.
	DetMap m_det;
	// This is a set of exposed samples for atom key. The samples are
	// normal and area rather than angle. 
	DetMap m_samp;
};



template <class Ty, class Tr> class direction_generator : public Tr
{
	typedef typename Tr::IdxTy IdxTy;
	typedef Ty Fl;
	typedef  myPoint4<Ty > LoTy;
	typedef std::vector<LoTy> LoV;
public:
	typedef LoTy dir_type;
	typedef LoV generated_list;
	//dg.generate(lx,ly,kluge);
	generated_list&  generated() { return m_spots; }
	// This now needs to have no directional biases so that equal amounts
	// of area are sampled in all directions for later averaging. 
	void generate(const IdxTy lx, const IdxTy ly, const Fl kluge)
	{
		const Fl kr=1.0/kluge;
		for ( IdxTy ix=0; ix<=lx; ++ix)
		{ 
			const Fl theta=3.14159*ix/lx;
			const Fl ct=::cos(theta); const Fl st=::sin(theta);
			const Fl rr=fabs(st); // this is the relative amount of length to span. 
			const IdxTy lyx=IdxTy(kluge*rr+1);
			for (IdxTy iy=0; iy<lyx; ++iy)
			{
				Fl phi=3.14159*2*iy/(lyx);
				// sure, I guess you could make a lut if you really care about 
				// speed but anything of O(1) isn't worth it. 
				// now, if you have a lut class somewhere, perhaps... 
				Fl xs=st*::cos(phi);
				Fl ys=st*::sin(phi);
				Fl zs=ct;
				m_spots.push_back(LoTy(xs,ys,zs,rr+kr));
			}
		}
	}
	
private:
	LoV m_spots;

};




class expand_radius
{
public:
expand_radius(const float d) : dd(d) {}
template < class Ty > void operator() (Ty & v, 	const Ty & s) const
{ v=s; v.radius(v.radius()+dd);   }
float dd;
};

#include "../math/move_around.h" 
/*
class move_around
{//http://en.wikipedia.org/wiki/Euler_angles
	typedef double Ty;
public:
move_around(const Ty & x,const Ty & y, const Ty & z,
const Ty & a, const Ty & b, const Ty & c)  :xo(x),yo(y),zo(z)
{
	const Ty c1=::cos(a);	const Ty c2=::cos(b);	const Ty c3=::cos(c);
	const Ty s1=::sin(a);	const Ty s2=::sin(b);	const Ty s3=::sin(c);
	// zyx
	t00=c1*c2; t01=-c2*s1; t02=s2;
	t10=c3*s1+c1*s2*s3; t11=c1*c3-s1*s2*s3; t12=-c2*s3;
	t20=s1*s3-c1*c3*s2; t21=c3*s1*s2+c1*s3; t22=c2*c3;
	shake=0;
}
void set_shake(const Ty & s) { shake=s/RAND_MAX; } 
template < class Tyx > void operator() (Tyx & v, 	const Tyx & s) const
{ 
	const Ty ex= s.x()-xo; const Ty ey=s.y()-yo; const Ty ez=s.z()-zo;
	v=s;
	const Ty arr=shake;//.2;
	const Ty fx= t00*ex+t01*ey+t02*ez+xo+arr*(rand());///RAND_MAX; 
	const Ty fy= t10*ex+t11*ey+t12*ez+yo+arr*(rand());///RAND_MAX; 
	const Ty fz= t20*ex+t21*ey+t22*ez+zo+arr*(rand());///RAND_MAX;
	v.move_to(fx,fy,fz);

}
private:
Ty shake;
Ty xo,yo,zo;
Ty t00,t01,t02,t10,t11,t12,t20,t21,t22;



};
*/

#endif
