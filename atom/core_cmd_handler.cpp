#include "../atom/core_cmd_handler.h"
#include "../base/cheap_greps.h"

#include "../atom/screen_contents_prop.h"

typedef core_cmd_handler MyClass;
MyClass::core_cmd_handler(atom_io * aio,config_type & cf) : Super(),m_config(cf),m_io(),m_ad()
,m_script_started(false),m_script_more(false),m_si()
	{
		text_commands::addCmd("list",this," "); 
		text_commands::addCmd("cf",this,"change configuration"); 
		text_commands::addCmd("SCRIPT",this,"run a script"); 
		text_commands::addCmd("load",this,"load pdb file and make cavity"); 
		text_commands::addCmd("loado",this,"load pdb file only, sic"); 
		text_commands::addCmd("vtree",this,"view display tree");
		text_commands::addCmd("stree",this,"set display tree"); 
		text_commands::addCmd("screen",this,"set display tree");
		text_commands::addCmd("mark",this,"add a mark or annotation");
	
	}
	void MyClass::cmd( text_commands * src, const console_type & s) 
	{
//		cout<<"Cmad called with val "<<s<<"\n";
		string_parse_type p(s);
		string_type rcmd="";
		p>>rcmd; 
		if (rcmd=="load") {
			string_type plp="point_list_param",str="figure";
			// wtf???
			Cavity(Load(p),plp,str); return ; 
		}
		if (rcmd=="loado") {Load(p); return ; }
		if (rcmd=="list") {List(m_io,p); return; }
		// move to config handler...
		if (rcmd=="cf") {Cf(p); return; }
		if (rcmd=="SCRIPT") {Script(rcmd,src,p); return; }
		// move to general purpose display tree stuff
		if (rcmd=="vtree") {Vtree(p); return; }
		if (rcmd=="stree") {Stree(p); return; }
		if (rcmd=="screen") { Screen(p,src); return; }
		if (rcmd=="mark") { Mark(p,src); return; }
		cout<<"no recognize ."<<rcmd<<".\n"; return;
		
	}
	void MyClass::Mark(string_parse_type & p, text_commands * src)
	{	
		glut_generic_base * ggb=src->m_ggb;
		//ggb->annotate(p.str().c_str());
			cout<<MM_MARK<<"\n"; 
			ggb->annotate(p);
		glutPostRedisplay();
		
		}
	
	void MyClass::Screen(string_parse_type & p, text_commands * src)
	{	//index_type i=0; p>>i;
		//AtomLiTy * ap=m_io.get_list(i);
		string_type nm="screeninfo";
		typedef screen_contents_prop St;
		return_values_type &r=src->symbols();
		St x=St(m_config,p,nm);
		int w=r_cam.width(), h=r_cam.height();
	//	char * buf= new char[w*h*4];
		//glReadPixels(0,0,w,h, GL_RGBA, GL_UNSIGNED_BYTE,buf);
		x.calculate(w,h);
		r+=x.results();
		x.results().dump(std::cout);
		// no redisplay...
		}
	
	
	void MyClass::Vtree(string_parse_type & p)
	{
		if (p.eof()){m_io.tree().traverse(std::cout,0); return; }
		OsStreamTy r;
		m_io.tree().traverse(r,0);
		string_parse_type pr(r.str());
		typedef greps::cmd_line_form Gr;
		Gr gr = Gr(pr,p);
		Gr::ItorTy ii=gr.begin(), ie=gr.end();
		while (ii!=ie) { cout<<(*(ii++))<<"\n"; }
		}
	void MyClass::Stree(string_parse_type & p)
	{
		index_type i=0; // p>>i;
		// fix this...
		Ch c=' ', e=':',f='+',g='$',ff='>', gg='<';
		// the tree should remain consistent for now...
		display_tree* r= &m_io.tree();
		p.get(c); while((c==' ')&&!p.eof()&&p.good()) p.get(c); 
		while (!p.eof()&&(c!=e)&&(c!=f)&&(c!=g)&&(c!=gg)&&(c!=ff)&&r) 
		{p.putback(c);  p>>i; r=(*r)[i]; 
		 p.get(c); while((c==' ')&&!p.eof()&&p.good()) p.get(c); 
		}
		string_type nm=""; p>>nm;
		std::cout<<" trying to set :"<<nm<<". and c is "<<c<<"\n";
		// 1== mom, 2==kids 
		if ((p.good()||(nm=="help"))&&r) 
		{     	index_type val=((c==f)?2:0); val+=((c==g)?1:0);  
			val+=((c==ff)?2:0); val+=((c==gg)?1:0); 
			r->sset(nm,p,val);
		 	glutPostRedisplay(); }
		else {std::cout<<"Couldnt call set as something wrong.\n";
		std::cout<<"Syntax: [branch sequence]{:,>,<}field value\n";
		std::cout<<"  sequence is from top, >< to traverse up or down\n";
		std::cout<<" Example: vtree 0 1 0>hide 1 to set hide starting at 0,1,0\n";
		}
		bool verify=false;
		if(verify) m_io.tree().traverse(std::cout,0);				
	}
	
	void MyClass::Cf(string_parse_type & p)
	{
		index_type i=0; p>>i;
		m_config.load(p.str().substr(3)); 
	}
	void MyClass::Script(const string_type & rcmd,text_commands * src,string_parse_type & p)
	{
		if (!m_script_started) 
			m_si=m_config.get_all(rcmd);
		m_script_started=true;
		m_script_more=true;
		bool post_glut=true; 
		config_type::value_iterator sf=m_config.get_end(rcmd);
		while (m_si!=sf)
		{
			string_type s=(*m_si);
			if (s!=rcmd)
			{
			std::cout<<"Scripting: "<< s<<"...\n";
			src->DoString(s);
			if ( post_glut) { glutPostRedisplay();
				src->more_scripting(); ++m_si; 	return; }
			}
		++m_si;
		
		}
		m_script_more=false;
		m_script_started=false;
	}
	
	void MyClass::List(atom_io & io,string_parse_type & p)
	{
	cout<<"Spheres =0\n";
	io.get_list().names(cout); //atom_lists.names(cout); 
	 cout<<"Connections=1\n";//NameList(conn_lists.begin(),conn_lists.end());	
	io.get_conn().names(cout); //conn_lists.names(cout);
}
AtomLiTy * MyClass::Load(string_parse_type &p) {return Load(m_io,p,m_config);}
 AtomLiTy * MyClass::Load(atom_io & io,string_type s,config_type & cf)
{ string_parse_type p(s); return Load(io,p,cf); }
	AtomLiTy * MyClass::Load(atom_io & io, string_parse_type & p, config_type & cf)
	{
		string_type name="",file=""; 
		p>>name;
		file=name;//+".pdb"; 
		cout<<"Attempting to load "<<file<<"\n";
		AtomLiTy * al=new AtomLiTy(name);
		ConListTy * new_conn=new ConListTy(*al);
		new_conn->set_bond_color(2,.1f,.9f,.4f);
		IsTy* is =DaCoTy::open(file);
		if (*is) {delete &io.loadSphereList(*al,*is,cf ,new_conn); }
		else {std::cout<<"Not trying to load...\n"; }
		if (!al->size())
		{
			cout<<"Didnt load any atoms, removing...\n";
			delete al; 
			return NULL; 
		}
		cout<<"Making connection lists\n";	
		new_conn->make_chains();
		// need to add them LAST
		io.push_atom(al); 
		io.push_conn(new_conn);
		
		return al; }
void MyClass::Cavity(AtomLiTy* al,const string_type & plp, const string_type & str) 
{Cavity (m_io,al,m_config,plp,str); }
void MyClass::Cavity(atom_io & io,AtomLiTy* al,config_type & cf,const string_type & plp, const string_type & str)
	{	if (!al) return; 
//		string_type str="figure";
		string_type name=(*al).name();
		FigTy px= FigTy(cf, str);
		ALPTy * alp=new ALPTy(*al);
		std::cout<<" list no whas "<<(*al).size()<<" elements \n";
		BPLPTy bplp;
		string_type _plp=plp, _str=str;
		if (cf.get(_plp,_str)) bplp=BPLPTy(cf,_str);
		AtomLiTy * al_cavity=new AtomLiTy(name+"_cavity"+atom_io::serial_name());
		
		alp->figure(px,bplp,*al_cavity);
		
		AtomLiTy *al_bound=new  AtomLiTy(name+"_boundary"+atom_io::serial_name(), alp->answer());		
		io.push_atom(al_cavity); 
		io.push_atom(al_bound); //atom_lists.push_back(al_bound); 
		delete alp;
	}
void MyClass::Cavity(AtomLiTy* al, const string_type & str) 
{Cavity (m_io,al,m_config,str); }
	
void MyClass::Cavity(atom_io & io,AtomLiTy* al,config_type & cf, const string_type & str)
	{	if (!al) return; 
		string_type name=(*al).name();
		FigTy px= FigTy(cf, str);
		ALPTy * alp=new ALPTy(*al);
		std::cout<<" list adfasfasfs "<<(*al).size()<<" elements \n";
		BPLPTy bplp;
		//string_type  _str=str;
		bplp=BPLPTy(cf,str);
		AtomLiTy * al_cavity=new AtomLiTy(name+"_cavity"+atom_io::serial_name());
		
		alp->figure(px,bplp,*al_cavity);
		
		AtomLiTy *al_bound=new  AtomLiTy(name+"_boundary"+atom_io::serial_name(), alp->answer());		
		io.push_atom(al_cavity); 
		io.push_atom(al_bound); //atom_lists.push_back(al_bound); 
		std::cout<<" cavity sizes are "<<al_cavity->size()
			<<" and "<<al_bound->size()<<"\n";
		delete alp;
	}


