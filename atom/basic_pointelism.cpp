#include "../atom/basic_pointelism.h"
//#include "../atom/pot_energy_contour.h"
#include "../base/cheap_greps.h"
//#include "../atom/potential_param.h"
//#include "../atom/new_potential_follow.h"
#include "../atom/raw_points_list.h"
#include "../debug_and_support.h"
////////////////////////////////////////////////////////////////////////
#include "../math/distance_to_segments.h"
#include <vector>
#include <algorithm>
class raw_point_operations: public energy_tr
{
	typedef raw_point_operations Myt;
//	typedef point3_template<coord_type> PointTy;
	typedef distance_to_segments DiTy;
public:
	typedef RawListTy  DestTy;
	typedef RawListTy  SrcTy;
	typedef SrcTy::position_type PointTy;
	typedef SrcTy::point_type PairTy;
	typedef SrcTy::const_iterator ItorTy;
	typedef std::vector<PairTy> MyListTy;
	typedef MyListTy::iterator MyItorTy;
	
template <class Cf> raw_point_operations(Cf & cf, const string_type& str )
: m_center(0,0,0),m_radius(1),m_segment_count(0),m_max_dist(1)
{
	cf.parse(str,"center",m_center);
	cf.parse(str,"radius",m_radius);
	cf.parse(str,"max_dist",m_max_dist);
	cf.parse(str,"segment_count",m_segment_count);
	
	if (m_segment_count!=0) m_di.select_segments(cf,str,m_segment_count);
	dump(std::cout);
}
template <class Os> void dump(Os& os)
{
		os<<"center="<<m_center<<" radius="<<m_radius<<" segment_count="
		<<m_segment_count<<" max_dist="<<m_max_dist
			<<"\n";
}
class r_sort
{public:
	int operator() (const PairTy & a, const PairTy & b)
	{return a[0].z()<b[0].z();}
	};
void execute( const SrcTy * red, const SrcTy *blue,DestTy*& dest)
{
	LoadList(red,m_red);
	LoadList(blue,m_blue);
	mmdd::dout(MMDDLOC)<<" here\n";
	r_sort x;
	MyItorTy iir=m_red.begin(), ier=m_red.end();
	std::sort(iir,ier,x);
	MyItorTy iib=m_blue.begin(), ieb=m_blue.end();
	std::sort(iib,ieb,x);
	
	mmdd::dout(MMDDLOC)<<" here\n";
	
	coord_type dmax=0, dmin=0;
	iir=m_red.begin(); ier=m_red.end();
	iib=m_blue.begin(); ieb=m_blue.end();
	
	while (iir!=ier)
	{
		const PairTy & p=(*(iir++));
		const PointTy& pp=p[0];
		const coord_type zmin=pp.z()-m_max_dist;
		const coord_type zmax=pp.z()+m_max_dist;
		std::cout<<"zmin="<<zmin<<" max= "<<zmax<<"\n";
	while (iib!=ieb)
	{
		const PairTy & q=(*(iib));
		if (q[0].z()>=zmin) break;
	iib++;
	} //iib
	MyItorTy iibb=iib;
	PairTy r;	
	coord_type closer=m_radius*m_radius;
	coord_type closest=closer;
	while (iibb!=ieb)
	{
		const PairTy & q=(*(iibb++));
		const PointTy qq=q[0];
		if (qq.z()>=zmax) break;
		const coord_type d=(qq-pp).length2();
		if (d<closest) { closest=d; r=q;}
	}
	if (closest<closer) 
	{
		coord_type alpha=(r[0]-p[0]).dot(p[1]);
		if (alpha>dmax) dmax=alpha;
		if (alpha<dmin) dmin=alpha;
		//r[0]=p[0]; // put at test location
		//r[1]=p[1]; // keep probe normal for now
		//r[2]=r[2]
		m_temp.push_back(p);
		m_temp.push_back(r);
		}
	
	
} //iir
	iir=m_temp.begin(); ier=m_temp.end();
	while (iir!=ier)
	{
		PairTy  p=(*(iir++));
		const PairTy & q=(*(iir++));
		coord_type alpha=(q[0]-p[0]).dot(p[1])/(dmax);
		if (alpha>=0) p[2]=PointTy(1,1.0f-alpha,1.0f-alpha);
		else {alpha=1.0f-alpha*dmax/dmin; p[2]=PointTy(alpha,1,alpha); }
		dest->xpush_back(p);
	}

}

private:
void LoadList( const SrcTy* src,MyListTy & li)
{
	ItorTy ii=src->begin(),ie=src->end();
	while (ii!=ie)
	{
		const PairTy & p=(*(ii++));
		if ((m_radius!=0) &&(m_radius<(p[0]-m_center).length2())) continue;
		if ((m_segment_count>0)&&(!m_di.close_enuff(p[0]))) continue;
		li.push_back(p);
	}
}
PointTy m_center;
coord_type m_radius;
index_type m_segment_count;
coord_type m_max_dist;

MyListTy m_red,m_blue,m_temp;
DiTy m_di;

};
///////////////////////////////////////////////////////////////////////////
typedef basic_pointelism MyClass;

MyClass::basic_pointelism
	(config_type & cf,StParse & p, const string_type nm, 
		const SrcTy * red, const SrcTy *blue,DestTy*& dest)
{
	Configure(cf,p,nm);
	typedef raw_point_operations Ty;
	Ty* x= new Ty(cf,nm);
	x->execute(red,blue,dest);
	delete x;
}
void MyClass::Configure(config_type& cf,StParse & p, const string_type nm)
{
	// hard coded defaults.
	string_type hard_coded_defaults= 
		"target=1 start=0,0,0 ds=.2 dy=1 points_i=100 points_j=100 imax=200";
	// default key to look in config for config defaults
	string_type default_key="pointelismkey";
	// over ride in order: hard coded, config file key, config values, user input
	typedef greps::eqn_subs util_class;
	string_type resultant=util_class::netMap(cf,p,hard_coded_defaults,default_key);	
	cf.unload(nm); cf.load(nm+" "+resultant);
	return ;
}
