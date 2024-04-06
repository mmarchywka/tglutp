#include "../math/move_around.h"
#include "../atom/atom_list_modifier.h"
#include "../atom/pdb_atom.h"
//#include "../math/various_searches.h"
#include "../chem/residue_properties.h"
#include "../base/cheap_greps.h"



#include <vector>
#include <algorithm>
#include <map>





////////////////////////////////////////////////////////////////
class basic_atom_modifier : public energy_tr, public caving_tr
{
	typedef basic_atom_modifier Myt;
	typedef atom_container  DestTy;
	typedef atom_container SrcTy;
	
	typedef string_parse_type StParse;
	
	typedef point3_template<coord_type> PointTy;
	
	typedef residue_properties ColorTy;
//	typedef searches::Restrictor<coord_type,PtrItorTy,TgtTy,PtrListTy> Restrictor;
	public:
	
template <class Cf> basic_atom_modifier(Cf & cf, const string_type& str )
: m_map_name(""),m_move(0,0,0),m_rotcen(0,0,0),m_eulers(0,0,0),m_shake(0)
, m_point_center(0,0,0), m_point_radius(0)
{
	cf.parse(str,"map_name",m_map_name);
	// someone has set a default for this that overrides the coord
	// sets- don't know where. ...
	cf.parse(str,"move",m_move);
	cf.parse(str,"mx",m_move.x());
	cf.parse(str,"my",m_move.y());
	cf.parse(str,"mz",m_move.z());
	
	
	//coord_type x=0;
	cf.parse(str,"rotx",m_rotcen.x());
	cf.parse(str,"roty",m_rotcen.y());
	cf.parse(str,"rotz",m_rotcen.z());
	cf.parse(str,"rotcen",m_rotcen);
	cf.parse(str,"ex",m_eulers.x());
	cf.parse(str,"ey",m_eulers.y());
	cf.parse(str,"ez",m_eulers.z());
	cf.parse(str,"eulers",m_eulers);
	cf.parse(str,"shake",m_shake);
//	cf.parse(str,"grid_space",m_grid_space);
//	cf.parse(str,"shell_thickness",m_shell_thickness);
//	cf.parse(str,"probe_radius",m_probe_radius);
//	cf.parse(str,"gross_radius",m_gross_radius);
	cf.parse(str,"point_center",m_point_center);
	cf.parse(str,"point_radius",m_point_radius);
	
	dump(std::cout);
}
template <class Os> void dump(Os& os)
{
		os<<"m_map_name="<<m_map_name
		<<" move="<<m_move
		<<" rotcen="<<m_rotcen
		<<" eulers="<<m_eulers
		<<" shake="<<m_shake
		<<" point_center="<<m_point_center
		<<" point_radius="<<m_point_radius
		<<"\n";
}

void execute( SrcTy * src, atom_selection & sel)
{
	
//	coord_type test_grid_radius=m_test_grid_radius;
//	coord_type grid_space=m_grid_space;
//	coord_type radius=5;
//	coord_type shell_thickness=m_shell_thickness;
//	coord_type probe_radius=m_probe_radius;
//	coord_type gross_radius=m_gross_radius;
//	coord_type cx=m_center.x(), cy=m_center.y(),cz=m_center.z();
	
	string_type map_name=m_map_name;
	
	// new
	bool doing_coloring=(map_name!="");
	bool doing_rot=(m_eulers!=PointTy(0,0,0));
//	move_around ma;
//	ma=move_around(m_rotcen,m_eulers);
	move_around ma(m_rotcen,m_eulers);
	ColorTy m_res;
	ColorTy::color_table_type lut;
	if ( doing_coloring)
	{
		m_res.spectrum_select(0);
		m_res.load_map();
		m_res.getmap(map_name);
		//ColorTy::color_table_type 
		lut=m_res.getcolors();
		if (lut.size()==0) 
		{	mmdd::dout(MMDDLOC)<<"Color map not found, abort\n";	
			return; 
		}
		mmdd::dout(MMDDLOC)<<"Color map size is "<<lut.size()<<"\n";
	}
	index_type sell=0;
	for (TgtItorTy p=src->begin(); p!=src->end(); ++p)
	{  TgtTy  &pp=*p; 
		if (sel.fits_criteria(pp))
		{
			++sell;
			PointTy dest=PointTy(pp)+m_move;
			if (doing_rot) {ma.doit(dest); }
			pp.move_to(dest.x(),dest.y(),dest.z()); 
			if (doing_coloring)
			{
				PointTy color=lut[pp.field(4)];
				if (color==PointTy(0,0,0)) 
				{	mmdd::dout(MMDDLOC)<<"zero color for "<<pp.desc()<<"\n";
					color=PointTy(1,1,1);
				}
				pp.color(color.x(),color.y(),color.z());
			}
		}
	
	}
	mmdd::dout(MMDDLOC)<<sell<<" atoms selected\n";
}

void execute( RawListTy * src, atom_selection & sel)
{
	string_type map_name=m_map_name;
	typedef RawListTy  DestTy;
	typedef DestTy::point_type PairTy; 
	// new
	//bool doing_coloring=(map_name!="");
	bool doing_rot=(m_eulers!=PointTy(0,0,0));
//	move_around ma;
//	ma=move_around(m_rotcen,m_eulers);
	move_around ma(m_rotcen,m_eulers);
	move_around man(PointTy(0,0,0),m_eulers);
	index_type sell=0;
	//index_type n=src->size();
	//for (index_type i=0; i<n; ++i)
	typedef DestTy::iterator Li;
	for (Li p=src->begin(); p!=src->end(); ++p)
	{ // TgtTy  &pp=*p; 
		PairTy & pp=(*p);
		//if (sel.fits_criteria(pp))
		if ((m_point_radius==0)||((pp[0]-m_point_center).length()<m_point_radius))
		{
			++sell;
			pp[0]+=m_move;
			if (doing_rot) {
				ma.doit(pp[0]); 
				man.untwirl(pp[1].x(),pp[1].y(),pp[1].z()); 
				man.doit(pp[1]); 
				man.twirl(pp[1].x(),pp[1].y(),pp[1].z()); 
			}
			// if the coords are rotated, then we also need to change
			// the normals. 
			//PointTy dest=PointTy(pp)+m_move;
			//if (doing_rot) {ma.doit(dest); }
			//pp.move_to(dest.x(),dest.y(),dest.z()); 
		}
	
	}
	mmdd::dout(MMDDLOC)<<sell<<" atoms selected\n";
}



private:

string_type m_map_name;
PointTy m_move;
// now more versatile- may 2008
PointTy m_rotcen, m_eulers;
coord_type m_shake;
// I should make the selection things for the pdb list consistent with
// the atom select parameters but I'll worry about that later.
// these are only for selecting points.
PointTy m_point_center;
coord_type m_point_radius;

};

//////////////////////////////////////////////////////////////////
typedef atom_list_modifier MyClass;

MyClass::atom_list_modifier
	(config_type & cf,StParse & p, const string_type nm, 
		AtomContainerTy * src)
{
	Configure(cf,p,nm);
	typedef basic_atom_modifier Ty;
	Ty* x= new Ty(cf,nm);
	x->execute(src,m_sel);
	delete x;
}
MyClass::atom_list_modifier
	(config_type & cf,StParse & p, const string_type nm, 
		RawListTy * src)
{
	Configure(cf,p,nm);
	typedef basic_atom_modifier Ty;
	Ty* x= new Ty(cf,nm);
	x->execute(src,m_sel);
	delete x;
}
void MyClass::Configure(config_type& cf,StParse & p, const string_type nm)
{
	// hard coded defaults.
	string_type hard_coded_defaults= 
	// default key to look in config for config defaults
	" move=0,0,0";	
	string_type default_key="modifykey";
	// over ride in order: hard coded, config file key, config values, user input
	typedef greps::eqn_subs util_class;
	string_type resultant=util_class::netMap(cf,p,hard_coded_defaults,default_key);	
	cf.unload(nm); cf.load(nm+" "+resultant);
	
	m_sel.select_fields(cf,nm);
//	AddCmd(cf,nm,"axes");
//	AddCmd(cf,nm,"centroid");
	return ;
}
