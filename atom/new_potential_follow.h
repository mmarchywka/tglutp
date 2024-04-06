#ifndef NEW_POTENTIAL_FOLLOW_H__
#define NEW_POTENTIAL_FOLLOW_H__

#include "../atom/energy_tr.h"
#include "../base/points_base_classes.h"
#include "../atom/raw_points_list.h"
#include "../math/new_sorted_potentials.h"
#include "../debug_and_support.h"

#include "../base/geom_iterator.h"

#include <iostream>

class new_potential_follow : public energy_tr
{	typedef new_potential_follow Myt;

	typedef point3_template<coord_type> PointTy;
	typedef AtomLiTy::iterator AtomItorTy;
	typedef RawListTy  DestTy;
	typedef DestTy::point_type PairTy; 
	typedef new_sorted_potentials<2,energy_tr> SortedP;
	typedef SortedP::coherent_type CoTy;
	typedef geom_iterator<Myt> MitorTy;
	static const coord_type dd() { static coord_type dd=.1f; return dd; }
public:
template <class Cf>	new_potential_follow(Cf & cf, const string_type& str ):m_ap(0),
	m_dx(dd(),0,0),m_dy(0,dd(),0),m_dz(0,0,dd()) ,m_ds(dd(),dd(),
	dd()),m_v(0),m_grad(),m_s(),m_failed(0),m_first_try(true)
	{coord_type dx=dd(), dy=dd(), dz=dd();
		m_locs=CoTy()(PointTy(0,0,0),0)(PointTy(2*dx,dx*dx,0) ,1)
			(PointTy(2*dy,dy*dy,0),2)(PointTy(2*dz,dz*dz,0) ,3);
			
		
			
		cf.parse(str,"target",m_probe_radius);
		cf.parse(str,"start",m_mp);
		cf.parse(str,"ds",m_dsp);
		cf.parse(str,"dx",m_dx1);
		cf.parse(str,"dy",m_dx2);
		cf.parse(str,"points_i",m_p1);
		cf.parse(str,"points_j",m_p2);
		cf.parse(str,"imax", m_max_iterations);
		
		m_first_scan_dir=PointTy(m_dsp,0,0);
		m_second_scan_dir=PointTy(0,m_dx2,0);
		cf.parse(str,"dir_i",m_first_scan_dir);
		cf.parse(str,"dir_j",m_second_scan_dir);
		m_first_scan_dir=m_first_scan_dir.normalize()*m_dsp;
		m_second_scan_dir=m_second_scan_dir.normalize()*m_dx2;
	
	}
	PointTy & callback(PointTy & loc)
	{ FindContour(m_probe_radius,loc);return m_grad; }
	void QCall(MitorTy & itor)
	{}
	void RowCall(MitorTy & itor)
	{
		if (m_failed!=0)
		mmdd::dout(MMDDLOC)<<"failed "<<m_failed<<" recent v= "<<m_v<<"\n";
		if ((itor.row()&15)==0)
		mmdd::dout(MMDDLOC)<<itor.row()<<" "<<itor.loc()<<" "<<m_grad<<"\n";
		m_failed=0;
		m_first_try=true;
		}
void contour(AtomLiTy * ap,DestTy* cage)
	{ 	m_ap=ap;
		AtomLiTy::const_iterator ii=ap->begin(), ie=ap->end();
		m_s.load(ii,ie);
		
		PointTy c=m_mp;
		PointTy loc=c;

		MitorTy x= 
		MitorTy(this,&Myt::callback,m_first_scan_dir,m_second_scan_dir,m_p1,m_p2);
		x.start(&Myt::RowCall,&Myt::QCall,loc);
		while (x.valid()) {  
			if (!m_did_fail) {
				PairTy z=PairTy()(x.loc(),0)(m_grad.z_twirl() ,1)
					(nearestColor(x.loc()),2);
			 	cage->xpush_back(z);	
			}
			x+=m_grad; 
		}
	}
	
	
private:


	bool FindContour(const coord_type target,PointTy & loc)
	{
	PointTy grad=PointTy();
	coord_type v0=-1;
	coord_type diff=1;
	Grad(loc,v0,grad,target);
	count_type lim=m_max_iterations;
	if (m_first_try) {
		std::cout<<" first v0="<<v0<<" \n"; 
		m_first_try=false;
		}
//	if (lim<1) {std::cout<<"Iter limit silly :"<<lim<<"\n"; m_pp.dump(std::cout); }
//	coord_type ssmax=10;
	while ( (diff>.001f)&&(--lim))
	{
	coord_type ss=.1f/(2.0f+grad.length2());
	coord_type step=ss*10*(v0-target);
	loc-=grad*(step);
	Grad(loc,v0,grad,target);
	diff=(v0-target)*(v0-target);
//	std::cout<<"\n"<<loc<<" "<<step<<" "<<grad<<" "<<target<<" "<<v0<<" "<<(v0-target);
	//std::cout.flush(); 
}	
//if (lim==0)           std::cout<<"f: "<<v0<<" ";        ///std::cout<<" failed at "<<loc<<" with "<<v0<<"\n";
	m_did_fail=(lim==0);
	if (lim==0) ++m_failed;
	m_grad=grad;
	return true;
}

	
	bool Grad(const PointTy& loc,coord_type & v0,PointTy& grad,const coord_type t)
	{
		SortedP::potential_type v;
			m_locs(loc,0);
			v=m_s.Vfast(m_locs);
		//	v=m_s.Vdefer(m_locs);
			grad=PointTy(v[1],v[2],v[3]);
		
		grad/=m_ds;
		m_v=v[0]; 
		v0=m_v;
		return true;
	}
	
	PointTy nearestColor(const PointTy& loc)
	{return m_s.nearestColor(loc); }
	
	CoTy m_locs;
	
	coord_type m_probe_radius,m_dsp,m_dx1,m_dx2;
	count_type m_p1,m_p2,m_max_iterations;
	PointTy m_mp;
	 PointTy m_first_scan_dir, m_second_scan_dir; 
	AtomLiTy * m_ap;
	PointTy m_dx,m_dy,m_dz,m_ds;
	coord_type m_v;
	PointTy m_grad;
	SortedP m_s;
	count_type m_failed; 
	bool m_did_fail,m_first_try;
};

#endif
