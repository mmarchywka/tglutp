#include "../atom/pot_energy_contour.h"
#include "../base/cheap_greps.h"
#include "../atom/potential_param.h"
#include "../atom/new_potential_follow.h"
#include "../atom/raw_points_list.h"


typedef pot_energy_contour MyClass;

MyClass::pot_energy_contour 
	(config_type & cf,StParse & p, const string_type nm, SrcTy * ap,DestTy*& cage)
{
	Configure(cf,p,nm);
//	potential_param pp=potential_param(cf,nm);
	new_potential_follow *  pf=new new_potential_follow(cf,nm);
	pf->contour(ap,cage);
	delete pf;
}
void MyClass::Configure(config_type& cf,StParse & p, const string_type nm)
{
	// hard coded defaults.
	string_type hard_coded_defaults= 
		"target=1 start=0,0,0 ds=.2 dy=1 points_i=100 points_j=100 imax=200";
	// default key to look in config for config defaults
	string_type default_key="isodefkey";
	// over ride in order: hard coded, config file key, config values, user input
	typedef greps::eqn_subs util_class;
	string_type resultant=util_class::netMap(cf,p,hard_coded_defaults,default_key);	
	cf.unload(nm); cf.load(nm+" "+resultant);
	return ;
}
