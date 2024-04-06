#include "mjm_globals.h"

#include "text_commands.h"


#include "../ext/jpeg_writer.h"
#include "../ext/bmp_writer.h"
#include <fstream>
#include <ios>

typedef text_commands MyClass;

MyClass::count_type MyClass::cin_problem()
{
	 typedef char CinCh;
const count_type avail= cin.rdbuf()->in_avail();	

 std::cout<<"check ing :"<<avail<<" \n";
 count_type av=avail; CinCh c=' ';
 while (av--)
 {	
 	cin.get(c);
 	int ikey=(int)c;
	if ((ikey<(CmpCh)' ')||(ikey>126)) std::cout<<" unknown cin "<<ikey<<"..."; 
	 KeyboardCallback(SigCh(c),0,0);
}
return avail;	
}

MyClass::CmdMapValue MyClass::addCmd(const string_type & cmd, const ext_ptr_type p
,const string_type & help)
{if (r_one) return r_one->add_cmd(cmd,p,help);  
else {std::cout<<"Attempt to add command failed as there is no instance\n"; }
return CmdMapValue();
}
MyClass::CmdMapValue MyClass::add_cmd(const string_type & cmd, const ext_ptr_type p
,const string_type & help ) 
{ext_cmd_map[cmd]=annotated_ext_cmd_ptr(p,help);
 return CmdMapValue();
 }
//~text_commands() {  Super::~(); }
void MyClass::LoadDefaultCommands()
{
	cmd_map["slice"]=CmdMapValue(&Myt::np_slice)
	.help("move near and far plane together");
	cmd_map["near"]=CmdMapValue(&Myt::console_np,&CameraTy::clip_up);
	cmd_map["far"]=CmdMapValue(&Myt::console_fp,&CameraTy::far_plane);
	cmd_map["rotate"]=CmdMapValue(&Myt::console_rotate)
	.help(" rotate scene in 3d with a,s;d,f;z,x and shift for axes/increment selection");
	cmd_map["quit"]=CmdMapValue(NULL,NULL,-1,&Myt::quit)
	.help(" couldnt use ctrl-C with automated scripts, ROFLOL ");
	cmd_map["light"]=CmdMapValue(&Myt::console_light_rotate);
	cmd_map["zoom"]=CmdMapValue(&Myt::console_zoom,NULL,4);
	cmd_map["move"]=CmdMapValue(NULL,NULL,0);
	//cmd_map["blend"]=CmdMapValue(NULL,NULL,-1,&Myt::setblend);
	cmd_map["print"]=CmdMapValue(NULL,NULL,-1,&Myt::capture);
	cmd_map["history"]=CmdMapValue(NULL,NULL,-1,&Myt::history);
	cmd_map["rotcen"]=CmdMapValue(NULL,NULL,-1,&Myt::rotcen)
	.help("Set rotation center point, should work with right click ");
	cmd_map["rot"]=CmdMapValue(NULL,NULL,-1,&Myt::rot)
	.help("Rotate by phi,around normal x,y,z ( 4 params) ");
	cmd_map["center"]=CmdMapValue(NULL,NULL,-1,&Myt::center)
	.help("Set scene center point, should work with right click ");
	cmd_map["help"]=CmdMapValue(NULL,NULL,-1,&Myt::help);
	//cmd_map["tree"]=CmdMapValue(NULL,NULL,-1,&Myt::traverse);
	cmd_map["idle"]=CmdMapValue(NULL,NULL,-1,&Myt::idletoggle);
	cmd_map["dumpq"]=CmdMapValue(NULL,NULL,-1,&Myt::dumpq). help
	(" dump contents of various ques, mostly debug"); 
	cmd_map["source"]=CmdMapValue(NULL,NULL,-1,&Myt::source)
	.help("execute commands from file ");
	cmd_map["update"]=CmdMapValue(NULL,NULL,-1,&Myt::update)
	.help("update shell variables with model and view parameters ");
	}


MyClass::CrTy MyClass::key_to_number(CmpCh key)
{
	string_type keys=".,asdf123456789";
	size_type n=keys.find((CmpCh)key);
	if (sizeof(size_type)!=sizeof(string_type::npos))
		MM_DIE(" bad npos "<<MMPR2( sizeof(size_type),sizeof(string_type::npos) ))
	float vals[]={.2f,-.2f,-100.0f,-10.0f,10.0f,100.0f,
	-1000.0f,-100.0f,-10.0f,-10.0f,-1.0f,1.0f,10.0f,100.0f,1000.0f};
	if (n!=string_type::npos){ return vals[n];};
	cout<<"Key not know in context "<<key<<"\n";
	return 0;
}
void MyClass::np_slice(SigCh key)
{ 	
	//console_fp(key); 
	//console_np(key);
	float x=key_to_number((CmpCh)key); 
	if (x>0){ r_cam.move_far_plane(x); r_cam.move_near_plane(x);}
	if (x<0){ r_cam.move_near_plane(x); r_cam.move_far_plane(x);}
	cout<<"slice "<<x<<" "; cout.flush(); 	glutPostRedisplay();
 }
void MyClass::console_fp(SigCh key)
{
	float x=key_to_number((CmpCh)key); if (x){ r_cam.move_far_plane(x); 
	cout<<"far "<<x<<" "; cout.flush(); }	glutPostRedisplay();
	}
void MyClass::console_np(SigCh key)
{
	cout<<"near\n";
	CrTy x=key_to_number((CmpCh)key);
	if (x){ r_cam.move_near_plane(x);
	cout<<"near "<<x<<" "; cout.flush(); }
	glutPostRedisplay();
	}
	void MyClass::console_zoom(SigCh  key)
{
	int ikey=(CmpCh)key;
	cout<<"xooom\n";
	switch (ikey)
	{
	case '.': { r_cam.move_something(4,1,0);glutPostRedisplay();	return; }	
	case ',': { r_cam.move_something(4,-1,0);glutPostRedisplay();	return; }
		}
	}
	void MyClass::unified_rotate(CmpCh key,CrTy & delta_x,CrTy & delta_y,CrTy & x,CrTy & y,CrTy & z)
	{

	switch (key)
	{
	case 'a': {  delta_x=.2; x=1; break; }	case 's': {delta_x=-.2; x=1; break; }
	case 'd': {  delta_x=.2; y=1; break; }	case 'f': {delta_x=-.2; y=1; break; }
	case 'A': {  delta_x=2; x=1; break; }	case 'S': {delta_x=-2; x=1; break; }
	case 'D': {  delta_x=2; y=1; break; }	case 'F': {delta_x=-2; y=1; break; }
	case 'z': {  delta_x=.2; z=1; break; }	case 'x': {delta_x=-.2; z=1; break; }
	case 'Z': {  delta_x=2; z=1; break; }	case 'X': {delta_x=-2; z=1; break; }
	case 'Q': {  delta_x=90; x=1; break; }	case 'W': {delta_x=-90; x=1; break; }
	case 'E': {  delta_x=90; y=1; break; }	case 'R': {delta_x=-90; y=1; break; }
	default:  std::cout<<"roate bad key "<<(char)key<<"\n";
	}	
	std::cout<<"urotate dx="<<delta_x<<" dy="<<delta_y<<" vec="<<x<<" "<<y<<" "
	 <<z<<"             \r";    	
		
		}	
	void MyClass::console_light_rotate(SigCh key)
	{
	float delta_x=0; float delta_y=0; float x=0,y=0,z=0;
	unified_rotate((char)key,delta_x,delta_y,x,y,z); 
	//camera_and_lights::
	r_cam.rotate_light(delta_x,myPoint3(x,y,z),delta_y,myPoint3(0,0,1));
	glutPostRedisplay();	
	}
	void MyClass::console_rotate(SigCh key)
	{
	float delta_x=0; float delta_y=0; float x=0,y=0,z=0;
	unified_rotate((CmpCh)key,delta_x,delta_y,x,y,z); 
	r_cam.rotate_scene(delta_x,myPoint3(x,y,z),delta_y,myPoint3(0,0,1));
	glutPostRedisplay();	
}

 void MyClass::call_cam(float x, float y, CameraTy & cam, const CamCtrlTy & ptr)
{//(myTest.*funcPtr)()  
	(cam.*ptr)(x); 	
}
//http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/readpixels.html
void MyClass::capture(string_parse_type & ss) 
{
	string_type name=""; ss>>name; 
	string_type fmt="jpeg"; ss>>fmt; 
	const char * nm=name.c_str();
	int w=r_cam.width(), h=r_cam.height();
	
	if (fmt=="jpeg")
	{	char * buf= new char[w*h*4];
		glReadPixels(0,0,w,h, GL_RGBA, GL_UNSIGNED_BYTE,buf);
		write_jpg(nm,buf,w,h);
 	delete[] buf;
 	}
 	else
 	{GLubyte * buf= new GLubyte[w*h*4];
 		glReadPixels(0,0,w,h, GL_RGB, GL_UNSIGNED_BYTE,buf);
 		int s=w*h;
 		for (int i=0; i<s; ++i) {GLubyte & c1=buf[3*i]; GLubyte & c2=buf[3*i+2];
 		 c1^=c2; c2^=c1; c1^=c2;
 		  }
		write_bmp(nm,w,h,buf);
		delete[] buf;
 	}
 //m_ggb->show_list(x,y,z);
// glReadPixelsGLint x,   GLint y, GLsizei width, GLsizei height,GLenum format, GLenum type, GLvoid *pixels )
//GL_COLOR_INDEX,GL_STENCIL_INDEX, GL_DEPTH_COMPONENT, GL_RED, GL_GREEN, GL_BLUE,	GL_ALPHA, 
//GL_RGB, GL_RGBA, GL_LUMINANCE, andGL_LUMINANCE_ALPHA.
//type GL_UNSIGNED_BYTE, GL_BYTE, GL_BITMAP,GL_UNSIGNED_SHORT, GL_SHORT, 
//GL_UNSIGNED_INT, GL_INT,or GL_FLOAT.


setdun("tried to outpu...  ",0); }//m_ggb->show_list(x,y)); }
//void MyClass::traverse(string_parse_type & ss) 
//{
	//m_ggb->m_tree.traverse(cout,0);		
//}
void MyClass::rot(string_parse_type & ss)
	{ myPoint3 x=myPoint3(); float phi=0;
	ss>>phi; ss>>x;
	r_cam.rotate_scene(phi,x,0,myPoint3(0,0,0));
	glutPostRedisplay();	
}

void MyClass::help(string_parse_type & ss) 
{string_type c=""; ss>>c; 
CmdMapTy & cm=cmd_map;
if ( cm.find(c)!=cm.end()) cout<<c<<"-"<<cm[c].help()<<"\n";
else {CmdMapTy::const_iterator ii=cm.begin(),ie=cm.end();
cout<<"Add on commands:\n";
while (ii!=ie ) {cout<<(*ii).first<<"\n  -"<<(*ii).second.help()<<"\n"; ++ii; }
	ECmdMapTy::const_iterator ei=ext_cmd_map.begin(),ee=ext_cmd_map.end();
	while (ei!=ee ) {cout<<(*ei).first<<" :"<<(*ei).second.second<<"\n";; ++ei; }
		}
//setdun("not implemented ",m_ggb->show_list(x,y));
 }
void MyClass::idletoggle(string_parse_type & ss) 
{bool x= m_ggb->idle_toggle(); setdun("Setting idle flag to ",x?1:0); }

void MyClass::source(string_parse_type & ss) 
{string_type x=""; ss>>x;
//typedef std::istream IsTy;
SourceTy * is = new std::ifstream(x.c_str());
if (!*is) { setdun("Failed to source from ",x); delete is; return; }
AddSourceStream(is); 
setdun("Reading from file ",x); }

void MyClass::AddSourceStream(SourceTy * is)
{
	m_stack.push(is); 
	m_ggb->add_idle_task("source",this, &Myt::source_script);	
	m_ggb->idle_enable(true); 
}
void MyClass::rotcen(string_parse_type & ss) 
{	CenterTy x=CenterTy(); ss>>x; r_cam.set_rot_center(x);
	setdun("Setting rotation center ",string_type(x));
}
void MyClass::center(string_parse_type & ss) 
{	CenterTy x=CenterTy(); ss>>x; r_cam.set_scene_center(x);
	setdun("Setting scene center ",string_type(x));
}
void MyClass::update(string_parse_type & ss) 
{	//CenterTy x=CenterTy(); ss>>x; r_cam.set_scene_center(x);
	//setdun("Setting scene center ",string_type(x));
	symbols().store("nearplane",r_cam.get_near());
	symbols().store("farplane",r_cam.get_far());
   symbols().store("staretarget",myPoint3(r_cam.getTarget()));
  symbols().store("cameraposition",myPoint3(r_cam.getPosition()));

}
void MyClass::history(string_parse_type & ss) 
{	string_type nm=""; ss>>nm;
std::ofstream* x= new std::ofstream(nm.c_str(),ios_base::out);
	if (*x) m_in.dump_history(*x);
	setdun((*x)?"Dumping history to ":"Unable to dump to ",string_type(nm));
	delete x;
}
void MyClass::dumpq(string_parse_type & ss) 
{	string_type nm=""; ss>>nm;
m_ggb->dump_q(std::cout);
std::cout<<"\n automated process stack size is "<<m_stack.size()<<"\n";
}
MyClass::count_type MyClass::source_script()
{
//	std::cout <<"Srouce script called\n";
	enum {MM_MAX_LINE=2048};
	typedef char Ch;
	Ch stupid[MM_MAX_LINE];
	if (m_stack.size()==0) return 0;
//	mmdd::dout(MMDDLOC)<<"have a stack\n";
	SourceTy * is=m_stack.top();
	// This is a kluge for the shell interpretter...
	if (is==0) {
//		mmdd::dout(MMDDLOC)<<"FOO\n";
		if (m_shell.foo(this)==0) {m_stack.pop();  return 1;} else return 1; }
//	mmdd::dout(MMDDLOC)<<"READ\n";
	if ( (*is)&&(is->good())&&!is->eof())
	{ //mmdd::dout(MMDDLOC)<<"get\n";
		is->getline(stupid,MM_MAX_LINE,'\n');
		string_type x=string_type(stupid);
		if (x.length()>0) if (x.substr(x.length()-1)=="\n") 
			x=x.substr(0,x.length()-1);
		if (x.length()>0) if (x.substr(x.length()-1)=="\r") 
			x=x.substr(0,x.length()-1);
	//	mmdd::dout(MMDDLOC)<<"do:"<<x<<"\n";
		DoString(x);
	}
	else {m_stack.pop(); delete is; }
	return m_stack.size(); 	
	
}
MyClass::count_type MyClass::run_script()
{	//std::cerr<<__FILE__<<__LINE__<<"run script called\n";
	if (!m_run_script) return 0;
	const console_type & s="SCRIPT";
	m_run_script=false;
	if(ext_cmd_map.find(s)!=ext_cmd_map.end())
	{
		(ext_cmd_map[s].first)->cmd(this,s); 
	}
	
	// give the callee a chance to reset this...
	//m_run_script=false;
	return 1; 	
	
}

void MyClass::DoString(const console_type & whole_cmd) 
{	
MM_ERR(__FUNCTION__)
	m_shell.translate(whole_cmd);
	console_type cmd=m_shell.pending();
	if (m_shell.callback()) AddSourceStream(NULL);
	while (cmd!="") { DoDoString(cmd); cmd=m_shell.pending();  }
MM_ERR(" exit "<< __FUNCTION__)

}
void MyClass::DoDoString(const console_type & whole_cmd) 
{
	string_parse_type ss(whole_cmd);
	MM_ERR(MMPR2(__FUNCTION__,ss.str()))
	console_type cms; ss>>cms;
	if(ext_cmd_map.find(cms)!=ext_cmd_map.end())
	{	(ext_cmd_map[cms].first)->cmd(this,whole_cmd); }
	else if (cmd_map.find(cms)!=cmd_map.end())
	{
	//	cout<<"Found "<<cms<<" in cmd map\n";
		CmdMapValue v=cmd_map[cms];
		if (v.m_punt) ((*this).*(v.m_punt))(ss); 
		float x=-9999; ss>>x;
		if (x==-9999) 
		{console_func_ptr=v.m_con; if(console_func_ptr) console_mode=1; return;}
		if (v.m_cam) (r_cam.*(v.m_cam))(x); 
		if (v.m_cam3>=0) { 
			float y=0; ss>>y; // should be ok even with eof
			r_cam.move_something(v.m_cam3,(int)x,(int) y );   }
	glutPostRedisplay();	
	} else cout<<"Didnt find "<<cms<< " in command map \n";  
	}

void MyClass::keyboardCallback( unsigned char key, int x, int y ) {r_one->KeyboardCallback(key,x,y);}
volatile int*  MyClass::global_panic=0; 
void MyClass::quit( string_parse_type & ss)
{
	if (global_panic!=0) *global_panic=1; exit(0);
}
void MyClass::KeyboardCallback( SigCh key, int x, int y ) {
//volatile 

if (console_mode==1)
{
	int ikey=(CmpCh)key;
	switch (ikey)
	{	case 0x03: {if (global_panic!=0) *global_panic=1; exit(-1); }
		case 0x0D:  {console_mode=0;  break; 	}
		default: { ((*this).*console_func_ptr)(key);  return;}
	}
	console_mode=0;
	return;	
}
if ( m_in.key(key,0)) DoString(m_in.process());
	

}
void MyClass::specialCallback( int key, int x, int y ) 
{	r_one->SpecialCallback(key,x,y);}

void MyClass::SpecialCallback( int key, int x, int y ) 
{ m_in.key(key,1);return;}

void MyClass::runScript()
{		}


text_commands * text_commands::r_one=NULL;




