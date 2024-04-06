#include "../atom/basic_caving.h"
#include "../atom/pdb_atom.h"
#include "../math/various_searches.h"
#include "../base/cheap_greps.h"

#include <vector>
#include <algorithm>
#include <map>
////////////////////////////////////////////////////////////////
class basic_cave_digging : public energy_tr, public caving_tr
{
	typedef basic_cave_digging Myt;
	typedef atom_container  DestTy;
	typedef atom_container SrcTy;
	typedef string_parse_type StParse;
	
	typedef point3_template<coord_type> PointTy;
	typedef searches::Restrictor<coord_type,PtrItorTy,TgtTy,PtrListTy> Restrictor;
	public:
	
template <class Cf> basic_cave_digging(Cf & cf, const string_type& str )
: 	m_test_grid_radius(5), m_grid_space(1), m_shell_thickness(.1),
	m_probe_radius(1), m_gross_radius(10)
{
	cf.parse(str,"center",m_center);
	cf.parse(str,"grid_radius",m_test_grid_radius);
	cf.parse(str,"grid_space",m_grid_space);
	cf.parse(str,"shell_thickness",m_shell_thickness);
	cf.parse(str,"probe_radius",m_probe_radius);
	cf.parse(str,"gross_radius",m_gross_radius);

	dump(std::cout);
}
template <class Os> void dump(Os& os)
{
		os<<" center="<<m_center
		<<" grid_radius="<<m_test_grid_radius
		<<" grid_space="<<m_grid_space
		<<" shell_thickness="<<m_shell_thickness
		<<" probe_radius="<<m_probe_radius
		<<" gross_radius="<<m_gross_radius
		<<"\n";
}

void execute( const SrcTy * src,DestTy*& dest)
{
	
	coord_type test_grid_radius=m_test_grid_radius;
	coord_type grid_space=m_grid_space;
	coord_type radius=5;
	coord_type shell_thickness=m_shell_thickness;
	coord_type probe_radius=m_probe_radius;
	coord_type gross_radius=m_gross_radius;
	coord_type cx=m_center.x(), cy=m_center.y(),cz=m_center.z();
	
	
	pdb_atom probe;
	probe.move_to(cx,cy,cz); 
	probe.radius(gross_radius);
	LoadLists(*src,probe);
	probe.radius(probe_radius);
	
	coord_type x=0,y=0,z=0;
	coord_type tgr2=test_grid_radius*test_grid_radius;
	count_type reject=0;
	Restrictor rz,rx,ry;
	PtrItorTy ii=m_z.begin(), ie=m_z.end(); // for each along z
	mmdd::dout(MMDDLOC)<<"\n";
	if (ii==ie) return;
	rz.init_itors(ii,ie);
	rz.restrict<&TgtTy::z,true,true>(cz-test_grid_radius,radius,m_z,m_composite,compare_x());
	for (z=-test_grid_radius+cz; z<test_grid_radius+cz; z+=grid_space)
	{coord_type gr2=(tgr2-(z-cz)*(z-cz)+.01); coord_type grid_rho=sqrt(gr2);
		std::cout<<"z="<<(z-cz)<<" have "<<dest->size()
		<<" rejected "<<reject<<" testing "<<m_composite.size()<<"\n";
		x=cx-grid_rho; //x<cx+grid_rho; x+=grid_space)
		rx.init_itors(rz);
		rx.restrict<&TgtTy::x,true,true>
		( x,radius,m_composite,m_composite_x,compare_y());
		while (true)
		{coord_type ylim=sqrt(gr2-(x-cx)*(x-cx)+.01);
		coord_type ymin=cy-ylim, ymax=cy+ylim;
		y=ymin;  // y<ylim;) // y-=grid_space)
	//	mmdd::dout(MMDDLOC)<<"x size ="<<m_composite_x.size()<<"\n";
		ry.init_itors(rx);
		ry.restrict<&TgtTy::y,false,true>
		 (y,radius,m_composite_x,m_composite_y,compare_y());
	//	mmdd::dout(MMDDLOC)<<"y size ="<<m_composite_y.size()<<"\n";
	//	mmdd::dout(MMDDLOC)<<"y is ="<<y<<" versus"<< m_composite_y[0]->y()<<"\n";
	//	mmdd::dout(MMDDLOC)<<"y is ="<<y<<" versus"<< 
	//		m_composite_y[m_composite_y.size()-1]->y()<<"\n";
		
		while (true)
		{
		///	mmdd::dout(MMDDLOC)<<"size="<<m_composite_y.size()<<"\n";
			probe.move_to(x,y,z); 
		PtrItorTy yi=ry.m_yi, ye=ry.m_yf; 
		// This probably is not a good idea on a null list
		if (yi!=ye) {// break; 
		const pdb_atom*  closest=(*yi);
	//	mmdd::dout(MMDDLOC)<<"\n";
		coord_type cl=100000;
		while (yi!=ye)
		{
		//	mmdd::dout(MMDDLOC)<<"\n";
			coord_type c=probe.clearance(**yi); 
			if (c<0) break ;
			if (c<cl) {cl=c;  closest=(*yi);  }
			++yi;
		}
		if ((yi==ye)&&(cl<shell_thickness))
		{	probe.oriented_toward(*closest,probe.radius());
			probe.color(*closest);probe.incr(); 
			dest->push_back(probe);
		} else reject++;
		}
		y+=grid_space;	
		//mmdd::dout(MMDDLOC)<<y<<"\n";
		if (y>=ymax) break;
		ry.restrict<&TgtTy::y,false,false>
		 (y,radius,m_composite_x,m_composite_y,compare_y());
		}
//		mmdd::dout(MMDDLOC)<<"\n";
		 x+=grid_space;
		if (x>=(cx+grid_rho)) break;
		rx.restrict<&TgtTy::x,true,false>
		( x,radius,m_composite,m_composite_x,compare_y());
	}//x
	rz.restrict<&TgtTy::z,true,false>
		(z+grid_space,radius,m_z,m_composite,compare_x());
	}// z
	}
private:

PointTy m_center;


PtrListTy m_z,m_composite,m_composite_x,m_composite_y;
coord_type m_test_grid_radius,	m_grid_space, m_shell_thickness,
	m_probe_radius,	m_gross_radius;


class compare_x { public:  int operator () (const TgtPtr & i, const TgtPtr & j) 
const {return (*i).x()<(*j).x();}};
class compare_y { public:  int operator () (const TgtPtr & i, const TgtPtr & j) 
const {return (*i).y()<(*j).y();}};
class compare_z { public:  int operator () (const TgtPtr & i, const TgtPtr & j) 
const {return (*i).z()<(*j).z();}};

void LoadLists(const TgtListTy & li,const pdb_atom& pa)
{
for (TgtCItorTy p=li.begin(); p!=li.end(); ++p)
{  TgtPtr pp=&(*p); if (!(pp->clears(pa)))m_z.push_back(pp); }
PtrItorTy ii=m_z.begin(); PtrItorTy jj=m_z.end();
mmdd::dout(MMDDLOC)<<"Sorting z list\n";
ii=m_z.begin();  jj=m_z.end(); std::sort(ii,jj,compare_z());
}




};
//////////////////////////////////////////////////////////////////
typedef basic_caving MyClass;

MyClass::basic_caving
	(config_type & cf,StParse & p, const string_type nm, 
		const AtomContainerTy * src,AtomContainerTy*& dest)
{
	Configure(cf,p,nm);
	typedef basic_cave_digging Ty;
	Ty* x= new Ty(cf,nm);
	x->execute(src,dest);
	delete x;
}
void MyClass::Configure(config_type& cf,StParse & p, const string_type nm)
{
	// hard coded defaults.
	string_type hard_coded_defaults= 
	// default key to look in config for config defaults
	" center=0,0,0 grid_radius=10 grid_space=.5 shell_thickness=.2 probe_radius=1 gross_radius=20";	
	string_type default_key="cavingkey";
	// over ride in order: hard coded, config file key, config values, user input
	typedef greps::eqn_subs util_class;
	string_type resultant=util_class::netMap(cf,p,hard_coded_defaults,default_key);	
	cf.unload(nm); cf.load(nm+" "+resultant);
	return ;
}
