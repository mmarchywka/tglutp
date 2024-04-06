#ifndef POTENTIAL_FOLLOW_H__
#define POTENTIAL_FOLLOW_H__

#include "../atom/potential_param.h"
#include "../base/points_base_classes.h"
#include "../atom/raw_points_list.h"
#include "../math/sorted_potentials.h"
#include "../debug_and_support.h"

//#include "../atom/pdb_atom.h"


#include <iostream>

class potential_follow
{	typedef potential_follow Myt;
	typedef potential_param ParamTy;
	// get everything from here- param is traits
	typedef ParamTy::coord_type coord_type;
	typedef ParamTy::count_type count_type;
	typedef point3_template<coord_type> PointTy;
	typedef AtomLiTy::iterator AtomItorTy;
	typedef RawListTy  DestTy;
//	typedef point3_array<coord_type,2> PairTy;
	typedef DestTy::point_type PairTy; 
	typedef sorted_potentials<2,r_traits> SortedP;
	typedef SortedP::coherent_type CoTy;
	static const coord_type dd() { static coord_type dd=.1f; return dd; }
public:
	potential_follow(const ParamTy & pp):m_pp(pp),m_ap(0),
	m_dx(dd(),0,0),m_dy(0,dd(),0),m_dz(0,0,dd()) ,m_ds(dd(),dd(),
	dd()),m_v(0),m_grad(),m_s(),m_failed(0)
	{coord_type dx=dd(), dy=dd(), dz=dd();
		m_locs=CoTy()(PointTy(0,0,0),0)(PointTy(2*dx,dx*dx,0) ,1)
			(PointTy(2*dy,dy*dy,0),2)(PointTy(2*dz,dz*dz,0) ,3);
		
		}
	void contour(AtomLiTy * ap,DestTy* cage)
	{ 	m_ap=ap;
		if (ap==0) std::cout<<"ap is null\n";
		if (cage==0) std::cout<<"cage is null\n";
		std::cout<<" have these params:"; m_pp.dump(std::cout);

		AtomLiTy::const_iterator ii=ap->begin(), ie=ap->end();
		m_s.load(ii,ie);
		
		PointTy c=m_pp.mp();
		PointTy loc=c;
		coord_type ds=m_pp.ds();
		coord_type dx2=m_pp.dx2();
		count_type n1=m_pp.p1(),n2=m_pp.p2();
		// These are sampling increments, the derivative steps are fixed for
		// now and various things are pre-calculated in m_locs.
		PointTy dir1=PointTy(ds,0,0);
		const PointTy dir1c=PointTy(ds,0,0);
		PointTy dir1ca=PointTy(0,ds,0),dir1cb=PointTy(0,0,ds);
		coord_type target=m_pp.probe();
		coord_type dnorm=dir1.length();
		std::cout<<" have "<<c<< "and "<<target<<"\n";
		
		for (count_type quadrants=0; quadrants<3; ++quadrants)
		{ loc=c; // ???? left out before...
		// these are "outer loops"- dont' get worried until into FindContour()
		for (count_type i2=0; i2<n2; ++i2){
			m_failed=0;
			FindContour(target,loc); // put it at a converged point... 
			PointTy loc_ini=loc;
		for (count_type i=0; i<n1;++i ){
		FindContour(target,loc);
	//	PairTy x=PairTy()(loc,0)(m_grad.normalize() ,1)(nearestColor(loc),2);
		PairTy x=PairTy()(loc,0)(m_grad.z_twirl() ,1)(nearestColor(loc),2);
		// when there are no atoms in the viewing area, this is EXACTLY
		// zero- if I lurned nothing else, that exceptionsare slllloooooooowwww.
		if (!m_grad.normalize_if()) { m_grad=PointTy(1,1,1);  } 
		if (!m_did_fail) cage->xpush_back(x);	
		PointTy dl= dir1.cross(m_grad).normalize()*dnorm;
		loc+=dl;
	//	if ((loc-c).length()>15) break;
	//	std::cout<<"dl= "<<dl.length()<<" "<<dl<<" ";
	}	 
	
	std::cout<<" locs: "<<loc<<" "<<loc_ini<<" "<<(loc-loc_ini)<<" ";
	loc=loc_ini;
	FindContour(target,loc);
		PointTy dir2=PointTy(0,0,0);
		dir2=dir2.cross(dir1ca,m_grad); dir2.normalize();
		if (dir2.length()<.1f) 
		{  dir2=PointTy(0,0,0);   dir2=dir2.cross(dir1cb,m_grad);dir2.normalize();    }
		dir2=dir2*(dx2);
		std::cout<<" last used :"<<m_s.used_size()<<" failed:"<<m_failed
			<<"                        \n"; std::cout.flush(); 
		if ((i2&15)==0)
		mmdd::dout(MMDDLOC)<<dir2<<" "<<loc<<" "<<m_grad<<"\n";
		std::cout<<"d2= "<<dir2.length()<<" "<<dir2<<" "<<loc<<"\n";;
//?????????????????????????
loc=loc+dir2;		
	//	loc=loc_ini+dir2;
		
//		loc+=dir2;
	//	dir1=dir1*(-1.0f); 
} //n2

if ((quadrants&1)==0)	dir1=dir1*(-1.0f); else dir1ca=dir1ca*(-1.0f); 
	}// quadrants;
	}


private:


	bool FindContour(const coord_type target,PointTy & loc)
	{
	PointTy grad=PointTy();
	coord_type v0=-1;
	coord_type diff=1;
	Grad(loc,v0,grad,target);
	count_type lim=m_pp.imax();
	if (lim<1) {std::cout<<"Iter limit silly :"<<lim<<"\n"; m_pp.dump(std::cout); }
	coord_type ssmax=10;
	while ( (diff>.001f)&&(--lim))
	{
	coord_type ss=.1f/(2.0f+grad.length2());
	//ssmax*=.9999f;
	//if (ss>ssmax) { ss=.5f*ssmax; ssmax=ss;}
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
		if (0){	
			m_locs=CoTy()(loc,0)(loc+m_dx,1)(loc+m_dy,2)(loc+m_dz,3);
			//SortedP::potential_type 
		//	v=m_s.V(m_locs);
			grad=PointTy(v[1]-v[0],v[2]-v[0],v[3]-v[0]);
		} 
		else {   
			m_locs(loc,0);
			//SortedP::potential_type 
			v=m_s.Vfast(m_locs);
		//	v=m_s.Vdefer(m_locs);
			grad=PointTy(v[1],v[2],v[3]);
		}
		grad/=m_ds;
		m_v=v[0]; 
		v0=m_v;
		if (0)
		{
		v0=V(loc); 
		m_v=v0;
		if (v0<0){ grad=m_grad; v0=t+1.0f;  return false;}
		coord_type vx=V(loc+m_dx),vy=V(loc+m_dy), vz=V(loc+m_dz);
		if (vx<0) { vx=v0-V(loc-m_dx);} else vx=vx-v0;
		if (vy<0) { vy=v0-V(loc-m_dy);} else vy=vy-v0;
		if (vz<0) { vz=v0-V(loc-m_dz);} else vz=vz-v0;
		grad=PointTy(vx,vy,vz);
		grad/=m_ds; // This needs to be kept consistent but is easier to write here. 
		
		}
		return true;
	}
	coord_type V(const PointTy& loc)
	{ 
	//	std::cout<<"calling ms....\n";
		return m_s.V(loc);
		coord_type v=0;
		AtomItorTy ii=m_ap->begin(),ie=m_ap->end();
		while (ii!=ie)
		{
			PointTy dr=PointTy(*ii)-loc;
			coord_type d=dr.length();
			//if (d<(*ii).radius()){  m_grad= dr.normalize();return -1; }
			v+=1.0/d/d;
			++ii;
		}
		return v;
	}
	PointTy nearestColor(const PointTy& loc)
	{
		return m_s.nearestColor(loc); 
	//	coord_type dnear=1000000;
		PointTy colo=PointTy(0,0,0); //loc;
		coord_type tot=0;
		AtomItorTy ii=m_ap->begin(),ie=m_ap->end();
		while (ii!=ie)
		{
			PointTy dr=PointTy(*ii)-loc;
			coord_type d=dr.length();
			tot+=1.0/d/d;
			//if (d<dnear){ dnear=d; colo=(*ii).get_color_3();  }
			colo+=(((*ii).get_color_3())/(d*d));
			++ii;
		}
		colo/=tot;
		return colo;
	}
	
	
	CoTy m_locs;
	
ParamTy m_pp; 
AtomLiTy * m_ap;
PointTy m_dx,m_dy,m_dz,m_ds;
coord_type m_v;
PointTy m_grad;

SortedP m_s;
count_type m_failed; 
bool m_did_fail;
};

#endif
