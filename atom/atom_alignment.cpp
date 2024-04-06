// This is sensitive to order, needs to be fixed.
#include "../hybrids/global.h"
#include "../hybrids/pdb_hybrid_typedefs.h"
#include "../hybrids/point_list_stuff.h"
#include "../hybrids/find_surface_molecules.h"


#include "../atom/atom_alignment.h"
#include "../atom/pdb_atom.h"
//#include "../math/various_searches.h"
#include "../chem/residue_properties.h"
#include "../base/cheap_greps.h"



#include <vector>
#include <algorithm>
#include <map>



// yes, this is dupilicated code...
#include <math.h>




////////////////////////////////////////////////////////////////
class basic_atom_alignment : public energy_tr, public caving_tr
{
	typedef basic_atom_alignment Myt;
	typedef atom_container  DestTy;
	typedef atom_container SrcTy;
	
	typedef string_parse_type StParse;
	typedef point3_template<coord_type> PointTy;	
	// leave this in modify, too odd here...
//	typedef residue_properties ColorTy;
//	typedef searches::Restrictor<coord_type,PtrItorTy,TgtTy,PtrListTy> Restrictor;
	public:
	
template <class Cf> basic_atom_alignment(Cf & cf, const string_type& str )
: m_mode(0) //, m_move(0,0,0),m_rotcen(0,0,0),m_eulers(0,0,0),m_shake(0)
{
	// this is really all we have right now... 
	cf.parse(str,"mode",m_mode);
	//cf.parse(str,"map_name",m_map_name);
	// someone has set a default for this that overrides the coord
	// sets- don't know where. ...

	dump(std::cout);
}
template <class Os> void dump(Os& os)
{
		os<<"  mode="<<m_mode
	//	<<" move="<<m_move
	//	<<" rotcen="<<m_rotcen
	//	<<" eulers="<<m_eulers
	//	<<" shake="<<m_shake
		<<"\n";
}

class centere
{
	public:
	centere(const PointTy & cc) : c(cc) {}
int operator() (const TgtTy * a, const TgtTy * b) const
{ 	PointTy ap=PointTy( *a);  PointTy bp=PointTy(*b);  
	return ( (ap-c).length()<(bp-c).length() ); 
}

PointTy c;	
};

template<class Ma> void do_something( SrcTy * src, atom_selection & sel, Ma& ma)
{	
	index_type sell=0;
	for (TgtItorTy p=src->begin(); p!=src->end(); ++p)
	{  TgtTy  &pp=*p; 
		if (sel.fits_criteria(pp))
		{
			++sell;
			PointTy dest=PointTy(pp);
			ma.doit(dest);
			pp.move_to(dest.x(),dest.y(),dest.z()); 
		}
	
	}
	mmdd::dout(MMDDLOC)<<sell<<" atoms selected\n";
	}
void re_center( SrcTy * src, atom_selection & sel, const PointTy& c)
{	
	for (TgtItorTy p=src->begin(); p!=src->end(); ++p)
	{  TgtTy  &pp=*p; 
		if (sel.fits_criteria(pp))
		{
			PointTy dest=PointTy(pp)-c;
			pp.move_to(dest.x(),dest.y(),dest.z()); 
		}
	}
}


template <class Tc,class Tl > void dump_check(const Tl & loc,const int line, const Tc & t, const char * m)
{
	PointTy c=PointTy (*t[0]); PointTy nn=PointTy (*t[1]);
	PointTy n2=PointTy(*t[2]);
	mmdd::dout(loc,line)<<" "<<m<<(c)<<" nn ="<<(nn-c)<<" next at "<<(n2-c) <<" \n";	
}
void align(SrcTy * src, atom_selection & sel)
{	typedef TgtTy * Tt;
	typedef std::vector<Tt> Vt;
	Vt temp;
	typedef PointTy Pp; 
	typedef move_around Ma;
	const coord_type pix=Ma::pi();
	// The user should have centered this on zero already, but let's clean it up.
	for (TgtItorTy p=src->begin(); p!=src->end(); ++p)
	{  TgtTy  &pp=*p; 
		if (sel.fits_criteria(pp)) {temp.push_back(&(pp)); }
	}
	// need at least a plane to orient 
	if (temp.size()<3) return ;
	const Pp orig=Pp(0,0,0);
	const Pp ax1=Pp(1,0,0); // first axis
	std::sort(temp.begin(),temp.end(), centere(orig));
	// get the center at zero
	Pp c=Pp (*temp[0]); 
	dump_check(MMDDLOC, temp,"original loc");
	//center
	re_center(src,sel,c);
	c=Pp (*temp[0]); Pp nn=Pp (*temp[1]);
	dump_check(MMDDLOC,temp,"recentered");
	Ma ma;
	// rotate around (nn-c)x(ax1) to move (nn-c) to ax1 direction
	ma=Ma(orig, (nn-c), ax1);
	do_something(src,sel,ma);
	// so now the NN should be along the + x-axis.
	//c=Pp (*temp[0]); //nn=Pp (*temp[1]);
	dump_check(MMDDLOC,temp,"nn along X?" );
	// now rotate around x^ until the next neighbor nn is such that nn.z=0 and
	// center of gravity is at z<0 ( note that rotating around x that y>0 or y<0).
	nn=Pp (*temp[2]);
	// this is rho projected into x plane 
	coord_type rho=::sqrt(nn.z()*nn.z()+nn.y()*nn.y());
	coord_type rdot=0;
	if (rho!=0)
	{	// This is the angle bewteen z and rho
		rdot=acos(nn.z()/rho);
		// the nulls got deeper when I used a better pi :)
		if ( nn.y()<0)  rdot=-rdot; //3.14159/2;
		rdot+=(pix/2); 
		ma=Ma(orig,ax1,rdot);
		do_something(src,sel,ma);
	}
	//nn=PointTy (*temp[2]);
	dump_check(MMDDLOC,temp," second no Z?" );
	coord_type zed=0,total=0;
	for (TgtItorTy p=src->begin(); p!=src->end(); ++p)
	{  TgtTy  &pp=*p; 
		if (sel.fits_criteria(pp))
		{
			PointTy dest=PointTy(pp);//-c;
			zed+=dest.z();++total;
		}
	}
	// finally, put the center of mass behind the front atoms on the z==0 plane. 
	if (zed>0) 
	{
		ma=Ma(orig,ax1,pix);
		do_something(src,sel,ma);
		dump_check(MMDDLOC,temp,"second sign zed ok?"); 
	}
		
	return ;
	
}
void execute( SrcTy * src, atom_selection & sel)
{	align(src,sel); 
	
	/*
	bool doing_rot=(m_eulers!=PointTy(0,0,0));
	move_around ma(m_rotcen,m_eulers);

	index_type sell=0;
	for (TgtItorTy p=src->begin(); p!=src->end(); ++p)
	{  TgtTy  &pp=*p; 
		if (sel.fits_criteria(pp))
		{
			++sell;
			PointTy dest=PointTy(pp)+m_move;
			if (doing_rot) {ma.doit(dest); }
			pp.move_to(dest.x(),dest.y(),dest.z()); 
		}
	
}
*/
//	mmdd::dout(MMDDLOC)<<sell<<" atoms selected\n";
	}
private:
index_type m_mode;
//string_type m_map_name;
//PointTy m_move;
// now more versatile- may 2008
//PointTy m_rotcen, m_eulers;
//coord_type m_shake;


};
//////////////////////////////////////////////////////////////////



atom_exposure::atom_exposure
	(config_type & cf,StParse & p, const string_type nm, 
		AtomContainerTy * src,AtomContainerTy * dest)
{
	Configure(cf,p,nm);
//	typedef basic_atom_alignment Ty;
//	Ty* x= new Ty(cf,nm);
//	x->execute(src,m_sel);
//	delete x;
	// This is the radius that is added to atom radius for testing.
	coord_type radius=1.0;
	cf.parse(nm,"probe_radius",radius);
	// This is the number of exposed patches needed to take an atom
	// into the dest list. Unforunatly, this depends on coverage parameters,
	// needs to be normalized to something user relevant. 
	// take those atoms that have more (lim=>0, take >lim) or less (lim<0, take <=-lim)
	// exposed areas. you can't have <0 exposed, so 0 means >0 areas
	// -1 means less than zero
	index_type nlim=2;
	cf.parse(nm,"patches",nlim);
	typedef point_list PlTy;
//	typedef std::vector<PlTy> MolColTy;
	typedef find_surface_molecules<PlTy,pdb_typedefs>  Fs ; //SuMoTy;
	//typedef find_surface_molecules Fs;
	Fs fs;
	fs.find(*src,radius);
	// now we need to take the surfac atoms expanded and put into the dest. 
	fs.roll_grid(std::cerr, 0,8);
	fs.take(*dest,nlim);
	
}
atom_exposure::atom_exposure
	(config_type & cf,StParse & p, const string_type nm, 
		AtomContainerTy * src,RawListTy * dest)
{
	// maybe done by caller
	Configure(cf,p,nm);
	// This is the radius that is added to atom radius for testing.
	coord_type radius=1.0;
	cf.parse(nm,"test_radius",radius);
	// This is the maximum distance FROM ZERO that a surface tile can lie and
	// be accepted. 
	coord_type d_max=0;
	// This doesn't work, the first param is having a "3" appended to it.
	// So, "DEF" is ok since it soaks up the bug but what else is flakey?
	cf.parse(nm,"dmax",d_max);
	coord_type r_frac=0;
	// subtract this fraction of probe radius from total radius. 
	cf.parse(nm,"rfrac",r_frac);
	mmdd::dout(MMDDLOC)<<" test_radius="<<radius<<" dmax="<<d_max<<" probe fraction(rrfrac)="<<r_frac<<" \n";
	typedef point_list PlTy;
	typedef find_surface_molecules<PlTy,pdb_typedefs>  Fs ; 
	Fs fs;
	fs.find(*src,radius);
	// this count and catalogs exposed patches. 
	fs.roll_grid(std::cerr, 0,8);	
	typedef RawListTy  DestTy;
	typedef DestTy::point_type PairTy; 
	// In theory, this takes care of everything but kluge is kluged.
	// after lunch, add a distance restriction and then we are all set...
	// r_frac is amount of radius to remove from position ( zero for none, 1 for atom surface)
	fs.take_patches<DestTy,PairTy>(*dest,d_max,r_frac);
	
}



void atom_exposure::Configure(config_type& cf,StParse & p, const string_type nm)
{
	// hard coded defaults.
	// these are confusing in atom_list_modifier... 
	// the hardcoded value overrides field selections later making order
	// confusing. 
	string_type hard_coded_defaults= 
	// default key to look in config for config defaults
	" move=0,0,0";	
	string_type default_key="alignkey";
	// over ride in order: hard coded, config file key, config values, user input
	typedef greps::eqn_subs util_class;
	string_type resultant=util_class::netMap(cf,p,hard_coded_defaults,default_key);	
	cf.unload(nm); cf.load(nm+" "+resultant);	
	m_sel.select_fields(cf,nm);
//	AddCmd(cf,nm,"axes");
//	AddCmd(cf,nm,"centroid");
	return ;
}




//////////////////////////////////////////////////////////////////
typedef atom_alignment MyClass;

MyClass::atom_alignment
	(config_type & cf,StParse & p, const string_type nm, 
		AtomContainerTy * src)
{
	Configure(cf,p,nm);
	typedef basic_atom_alignment Ty;
	Ty* x= new Ty(cf,nm);
	x->execute(src,m_sel);
	delete x;
}
void MyClass::Configure(config_type& cf,StParse & p, const string_type nm)
{
	// hard coded defaults.
	// these are confusing in atom_list_modifier... 
	// the hardcoded value overrides field selections later making order
	// confusing. 
	string_type hard_coded_defaults= 
	// default key to look in config for config defaults
	" move=0,0,0";	
	string_type default_key="alignkey";
	// over ride in order: hard coded, config file key, config values, user input
	typedef greps::eqn_subs util_class;
	string_type resultant=util_class::netMap(cf,p,hard_coded_defaults,default_key);	
	cf.unload(nm); cf.load(nm+" "+resultant);	
	m_sel.select_fields(cf,nm);
//	AddCmd(cf,nm,"axes");
//	AddCmd(cf,nm,"centroid");
	return ;
}
