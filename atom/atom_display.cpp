#include "../atom/basic_caving.h"
#include "../atom/atom_list_modifier.h"
#include "../atom/atom_alignment.h"
#include "../atom/basic_pointelism.h"
#include "atom_display.h"
#include "../base/cheap_greps.h"
#include "../atom/atom_selection.h"
#include "../atom/pot_energy_contour.h"
#include "../atom/spatial_properties.h"
#include "../atom/raw_points_list.h"
 
// added for "expo"


// This needs to be moved:
atom_display * atom_display::r_one=NULL; 
 
// as does the whole class: 
typedef atom_cmd_handler MyClass;


MyClass::atom_cmd_handler(atom_io * aio,config_type & cf) : Super(),m_config(cf),
	m_io(*aio),m_ad()

	{
	
		text_commands::addCmd("nurbs",this," "); 
		text_commands::addCmd("cage",this,"draw neighbor cage"); 
		text_commands::addCmd("cave",this,"make a new cavity from n at x,y,z"); 
		text_commands::addCmd("caving",this,"make a new cavity from n at x,y,z"); 
		text_commands::addCmd("cavity",this,"cave with better param names");
		text_commands::addCmd("tri",this,"triangles around cavity"); 
	//	text_commands::addCmd("energy",this,"make potential");
		text_commands::addCmd("pot",this,"make potential");
		text_commands::addCmd("pointel",this,"make potential");
		text_commands::addCmd("modify",this,"modify some pdb atom list properties in place");
		text_commands::addCmd("modpoints",this,"as with modify, but operates on [2] points lists");
		text_commands::addCmd("expo",this,"find exposed atoms similar to cavity but better");
		text_commands::addCmd("prop",this,"calculate a list of properites");
		text_commands::addCmd("select",this,"[input list] name distance parameters");
		text_commands::addCmd("selpoints",this,"[input list] name distance params, for lists in group 2"); 
		text_commands::addCmd("align",this," manipulate atom into a standard display orientation. "); 
	}
	void MyClass::cmd( text_commands * src, const console_type & s) 
	{
		cout<<"Cmad called with val "<<s<<"\n";
		string_parse_type p(s);
		string_type rcmd="";
		p>>rcmd; 
	
		if (rcmd=="nurbs") {Nurbs(p); return; }
		if (rcmd=="cage") {Cage(p); return; }
		if (rcmd=="tri") {Tri(p); return; }
		if (rcmd=="select") {Select(p); return; }
		if (rcmd=="selpoints") {SelPoints(p); return; }
		if (rcmd=="cave") {Cave(p); return; }
		if (rcmd=="cavity") {Cavity(p); return; }
	//	if (rcmd=="energy") {Iso(p); return; }
		if (rcmd=="pot") {Pot(p); return; }
		if (rcmd=="pointel") {Pointel(p,src->symbols()); return; }
		if (rcmd=="caving") {Caving(p,src->symbols()); return; }
		if (rcmd=="prop") { Properties(p,src->symbols()); return; }
		if (rcmd=="modify") { Modify(p,src->symbols()); return; }
		if (rcmd=="modpoints") { ModPoints(p,src->symbols()); return; }
		if (rcmd=="align") { Align(p,src->symbols()); return; }
		if (rcmd=="expo") { Expo(p,src->symbols()); return; }
		cout<<"no recognize ."<<rcmd<<".\n"; return;
		
	}
	void MyClass::Caving(string_parse_type & p, return_values_type & r)
	{
		index_type i=0; p>>i;
		AtomLiTy * src=m_io.get_list(i);
		if (src==NULL) return; 
		string_type nm=src->name();
		nm+=atom_io::serial_name(); 
		AtomLiTy * dest= new AtomLiTy(nm);
		nm="caving";
		typedef basic_caving St;
		//St x=
		 St(m_config,p,nm,src,dest);
		m_io.push_atom(dest);
		return;
		
	}
	
	void MyClass::Expo(string_parse_type & p, return_values_type & r)
	{
		index_type i=0; 
		string_type ty="atom";
		p>>ty;
		bool doing_pieces=(ty=="pieces");
		bool doing_whole_atom=!doing_pieces;
		p>>i;
		AtomLiTy * ap=m_io.get_list(i);
		string_type nm="default" ;
		// Note that this takes the first parameter as the name, 
		// even if that is not intended LOL
		p>>nm; nm+=atom_io::serial_name(); 
		cout<<" doing "<<(doing_pieces?"pieces":"whole atoms") << " for expo\n";		
	//	typedef greps::eqn_subs util_class;
	//	string_type select_key="expo_key",default_key="defkey", 
	//	hard_coded_defaults="";
	//	string_type resultant=
	//		util_class::netMap(m_config,p,hard_coded_defaults,default_key);
	//	m_config.unload(select_key); m_config.load(select_key+" "+resultant);
		// right now, this code is in atom_alignment. 
		
		typedef atom_exposure ExpoTy;
		if (doing_pieces)
		{
			RawListTy* cage=new RawListTy(ap->name()+atom_io::serial_name());
			ExpoTy(m_config,p,nm,ap,cage);
			if (cage!=NULL) m_io.push_point(cage); 
		}
		if ( doing_whole_atom)
		{
			AtomLiTy * sel_atoms= new AtomLiTy(nm);
			ExpoTy(m_config,p,nm,ap,sel_atoms);
			cout<<" found "<<sel_atoms->size()<<"\n"; cout.flush();
			m_io.push_atom(sel_atoms);
		}
		return ; 	
		/*
		index_type i=0; p>>i;
		AtomLiTy * src=m_io.get_list(i);
		if (src==NULL) return; 
		string_type nm=src->name();
	//	nm+=atom_io::serial_name(); 
	//	AtomLiTy * dest= new AtomLiTy(nm);
		nm="modify";
		typedef atom_list_modifier St;
		St(m_config,p,nm,src);
		// note NO REDISPLAY HERE...
	//	m_io.push_atom(dest);
		return;
		*/
		}
	
	void MyClass::Modify(string_parse_type & p, return_values_type & r)
	{
		index_type i=0; p>>i;
		AtomLiTy * src=m_io.get_list(i);
		if (src==NULL) return; 
		string_type nm=src->name();
	//	nm+=atom_io::serial_name(); 
	//	AtomLiTy * dest= new AtomLiTy(nm);
		nm="modify";
		typedef atom_list_modifier St;
		St(m_config,p,nm,src);
		// note NO REDISPLAY HERE...
	//	m_io.push_atom(dest);
		return;
		
	}
	void MyClass::ModPoints(string_parse_type & p, return_values_type & r)
	{
		index_type i=0; p>>i;
		RawListTy* src=m_io.get_raw_rw(i);
		if (src==NULL) return; 
		string_type nm=src->name();
	//	nm+=atom_io::serial_name(); 
	//	AtomLiTy * dest= new AtomLiTy(nm);
		nm="modify";
		typedef atom_list_modifier St;
	
		St(m_config,p,nm,src);
		// note NO REDISPLAY HERE...
		return;
	}
	typedef atom_selection SelectTy;
	void MyClass::SelPoints(string_parse_type & p)
	{	 
		string_type wtf=p.str();
		if (wtf.find("?")!=string_type::npos) {select_help(p); return ; }
		index_type i=0; p>>i;
	//	const AtomLiTy * ap=m_io.get_list(i);
		RawListTy* ap=m_io.get_raw_rw(i);
		string_type nm="default" ;
		p>>nm; nm+=atom_io::serial_name(); 
	//	AtomLiTy * sel_atoms= new AtomLiTy(nm);
	//	AtomLiTy * sel_atoms= new AtomLiTy(nm);
		typedef greps::eqn_subs util_class;
		string_type select_key="select_key",default_key="defkey", 
		hard_coded_defaults="";
		string_type resultant=
			util_class::netMap(m_config,p,hard_coded_defaults,default_key);
		m_config.unload(select_key); m_config.load(select_key+" "+resultant);
		SelectTy as=SelectTy(m_config, select_key);
		RawListTy* sel_atoms=new RawListTy(ap->name()+atom_io::serial_name());
		//ExpoTy(m_config,p,nm,ap,cage);
		as.select_near_2(ap,sel_atoms);
		cout<<" found "<<sel_atoms->size()<<"\n"; cout.flush();
		if (sel_atoms!=NULL) m_io.push_point(sel_atoms); 
	}
	
	
	
	void MyClass::Align(string_parse_type & p, return_values_type & r)
	{
		index_type i=0; p>>i;
		AtomLiTy * src=m_io.get_list(i);
		if (src==NULL) return; 
		string_type nm=src->name();
		nm="align";
		typedef atom_alignment St;
		St(m_config,p,nm,src);
		glutPostRedisplay();
		return;
		
		}
	
	
	void MyClass::Properties(string_parse_type & p, return_values_type & r)
	{	index_type i=0; p>>i;
		AtomLiTy * ap=m_io.get_list(i);
		string_type nm="potential";
		typedef spatial_properties St;
		St x=St(m_config,p,nm);
		x.calculate(ap);
		r+=x.results();
		x.results().dump(std::cout);
		// no redisplay...
		}
	
	void MyClass::Nurbs(string_parse_type & p)
	{
		index_type i=0; p>>i;
		const AtomLiTy * ap=m_io.get_list(i); //atom_lists[i];
		nurbs_test nt(ap); 	
	}
	void MyClass::Cavity(string_parse_type & p)
	{   index_type i=0; p>>i;
		AtomLiTy * ap=m_io.get_list(i);
		typedef greps::eqn_subs util_class;
		string_type select_key="cavedraw",default_key="defcavitykey", 
		hard_coded_defaults=
		"probe_radius=1 big_radius=15 center_index=-1 center=0,0,0 make_shell=1";
		hard_coded_defaults+=
		" small_radius=12 grid_space=.25 shell_thickness=.2 safety_radius=3";
		string_type resultant=
			util_class::netMap(m_config,p,hard_coded_defaults,default_key);
		m_config.unload(select_key); m_config.load(select_key+" "+resultant);
		std::cout<<(select_key+" "+resultant)<<"\n";
//		string_type plp="point_list_param";- merged
		core_cmd_handler::Cavity(m_io,ap,m_config,select_key);

	}
	
	void MyClass::Cave(string_parse_type & p)
	{   index_type i=0; p>>i;
		//const - ????
		AtomLiTy * ap=m_io.get_list(i);
		// figure_param user_figure_name
//user_figure_name probe_radius=1 neighbor_radius=15 center_index=20 center=0,0,0 make_shell=1
///////////////////////////////////////////////////////////////
/*		 string_type plp="point_list_param",str="figuretwo",str_tar="figtarget";
		
		string_type figtarget=
		"probe_radius=1 neighbor_radius=15 center_index=20 center=0,0,0 make_shell=1";
		greps::eqn_subs msub=greps::eqn_subs(figtarget,p);
		figtarget=string_type("figuretwo ")+figtarget;
		std::cout<<" using string \n"<<figtarget<<"\n";
		m_config.unload(str); m_config.unload(str_tar);
		m_config.load("figuretwo figtarget");
		m_config.load(figtarget);
		*/
		///////////////////////////////////////////////////////
		typedef greps::eqn_subs util_class;
		string_type select_key="figuretwo",default_key="deffigkey", 
		hard_coded_defaults=
		"probe_radius=1 neighbor_radius=15 center_index=20 center=0,0,0 make_shell=1";
		string_type resultant=
			util_class::netMap(m_config,p,hard_coded_defaults,default_key);
		m_config.unload(select_key); m_config.load(select_key+" "+resultant);
		string_type plp="point_list_param";
		core_cmd_handler::Cavity(m_io,ap,m_config,plp,select_key);
		////////////////////////////////////////////////////////
	//	core_cmd_handler::Cavity(m_io,ap,m_config,plp,str);
	}
	
	void MyClass::Cage(string_parse_type & p)
	{
		index_type i=0; p>>i;
		const AtomLiTy * ap=m_io.get_list(i); //atom_lists[i];
		nurbs_test nt(ap);  // The nurbs_test has all the adjacency stuff
		ConListTy* cage=new ConListTy(*ap);
		cout<<"Adding1\n"; cout.flush();
		nt.add_neighbors(cage,5,false); 
		cout<<"Adding2\n"; cout.flush();
		cage->set_bond_color(2,1,.05f,.05f); 
		cout<<"Addin3  "<<cage->size()<<"\n"; cout.flush();
		//cage->make_chains();
		cage->make_bonds_only();
		cout<<"Adding4\n"; cout.flush();
		m_io.push_conn(cage); //conn_lists.push_back(cage);
		
	}
	void MyClass::Tri(string_parse_type & p)
	{
		index_type i=0; p>>i;
		const AtomLiTy * ap=m_io.get_list(i); //atom_lists[i];
		nurbs_test nt(ap);  // The nurbs_test has all the adjacency stuff
		ConListTy* cage=new ConListTy(*ap);
		cout<<"Adding1\n"; cout.flush();
		cage->set_bond_name("triangles");
		cage->set_bond_color(3,0.2f,.95f,.15f); 
		nt.add_triangles(cage,3,false); 
		cout<<"Adding2\n"; cout.flush();
		
		cout<<"Addin3  "<<cage->size()<<"\n"; cout.flush();
		//cage->make_chains();
		cage->make_triangles();
		cout<<"Adding4\n"; cout.flush();
		m_io.push_conn(cage);// conn_lists.push_back(cage);
		
		}
	
	void MyClass::select_help(string_parse_type & p)
	{
		std::cout<<" select [src index] dest_name criteria \n";
		std::cout<<" criteria for distance are cutoff, segment_count, and pointN\n";
		std::cout<<" field criteria are:";
		SelectTy::help(); 
	}
	
	// General theory: hard-code some defaults. Allow user to enter some
	// predefined "templates" with various names with additional command line
	// modification. Only parameters, not operands, are specified.
	// Resulting parameters are passed either in a parameter object
	// OR via manipulation of the configuration object. 
	// Can use configuration manipulation to load the param object
	// soe nothing changes except target of config stuff.
	// Things: 1) key for any default lists(default key or user spec key).
	// 	2) key used by target object to extract resultant parameters
	void MyClass::Select(string_parse_type & p)
	{	 
		string_type wtf=p.str();
		if (wtf.find("?")!=string_type::npos) {select_help(p); return ; }
		index_type i=0; p>>i;
		const AtomLiTy * ap=m_io.get_list(i);
		string_type nm="default" ;
		p>>nm; nm+=atom_io::serial_name(); 
		AtomLiTy * sel_atoms= new AtomLiTy(nm);
		typedef greps::eqn_subs util_class;
		string_type select_key="select_key",default_key="defkey", 
		hard_coded_defaults="";
		string_type resultant=
			util_class::netMap(m_config,p,hard_coded_defaults,default_key);
		m_config.unload(select_key); m_config.load(select_key+" "+resultant);
		SelectTy as=SelectTy(m_config, select_key);
		as.select_near(ap,sel_atoms);
		cout<<" found "<<sel_atoms->size()<<"\n"; cout.flush();
		m_io.push_atom(sel_atoms);
		
	}

void MyClass::Pot(string_parse_type & p)
	{	index_type i=0; p>>i;
		AtomLiTy * ap=m_io.get_list(i);
		if (ap==NULL) { return;}
		string_type nm="potential";//,select_name="energy_name";
		RawListTy* cage=new RawListTy(ap->name()+atom_io::serial_name());
		pot_energy_contour(m_config,p,nm,ap,cage);
		// could have been errors and cage gets nulled out...
		if (cage!=NULL) m_io.push_point(cage); 
		glutPostRedisplay();
	}
void MyClass::Pointel(string_parse_type & p, return_values_type & r)
	{	index_type i=0; p>>i;
		const RawListTy * red=m_io.get_raw(i);
		if (red==NULL) { return;}
		p>>i;
		const RawListTy * blue=m_io.get_raw(i);
		if (blue==NULL) { return;}
		string_type nm="pointel";
		RawListTy* dest=new RawListTy(red->name()+blue->name()+atom_io::serial_name());
		basic_pointelism(m_config,p,nm,red,blue,dest);
		// could have been errors and cage gets nulled out...
		if (dest!=NULL) m_io.push_point(dest); 
		glutPostRedisplay();
	}
