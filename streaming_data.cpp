// Mike marchywka, March 2008
// This uses the glutp infrastructure to visualize a real-time data stream
// right now, 7 floats/line give size, position and color, for each data point
// All the standard manipulation features should be available.
// Probably be faster once I clean up the glut code. LOL.

// this is a bit odd as it uses affx headers and pthreads.
#include "background_loader.h"

#include "../math/student_t_stat.h"
#include "../things/trial_group.h"

#include "data_display_header.h"

#include "dd_display_options.h"
 
typedef std::ostream OsTy;
OsTy & eos() { return std::cerr; } 
OsTy & dos() { return std::cout; } 

// oh crap the runtime inits these....
class defer
{
public:
background_loader bloader;
background_junk_bin bfifo;
fred_and_lamont jfifo;
dd_display_options dddo;
};
defer * defered=NULL;


int lop=0;
//float  linep[7];
typedef int IdxTy;

bool line_handler(background_loader * src)
{
background_loader &bloader=defered->bloader;
background_junk_bin& bfifo=defered->bfifo;
fred_and_lamont& jfifo=defered->jfifo;
dd_display_options& dddo=defered->dddo;


		const int sz= src->size(); //7;
 		char * p[sz];
 	
 //		for (IdxTy i=0; i<sz; ++i) linep[i]=atof(src->data(i));	
 		for (IdxTy i=0; i<sz; ++i) p[i]=src->data(i);	
//std::cout<<MM_MARK<<" linehandler "<<sz<<"  \n";
 		bool x= jfifo.tryst(p,sz); 
 		//bool x=bfifo.write_one(linep,sz);
 	
 		//dos()<<" read ";for ( int ii=0; ii<sz ; ii++) dos()<<linep[ii]<<" ";dos()<<'\n';
//std::cout<<MM_MARK<<" linehandler "<<x<<"  \n";
		if (x)model_runnable();
		return x;
}

void run_model()
{
background_loader &bloader=defered->bloader;
background_junk_bin& bfifo=defered->bfifo;
fred_and_lamont& jfifo=defered->jfifo;
dd_display_options& dddo=defered->dddo;
	// really need an init method
std::cout<<MM_MARK<<" model runs \n";
	if (lop==0) {} lop++;
	
	//if (!bloader.enter_out_force()) ;//return;
	// we have the lock at this point, should be ok...
	//if ( bfifo.avail()==0) model_blocked(); 
	if ( jfifo.avail()==0) model_blocked(); 
	//dddo.clr_model();
//	while (bfifo.avail()!=0) { bfifo.read_one(linep,7);  dddo.push_from_model(linep);}
	while (jfifo.avail()!=0)
	{	typedef char Ty;
		Ty * pn=jfifo.pread();
		const IdxTy sz=(int)*pn;
	//	if ( sz!=8) dos()<<" size is not 8 = "<<sz<<" at avail "<<jfifo.avail()<<'\n';
std::cout<<MM_MARK<<" model jfifo loop runs  "<<pn<<"\n";
		jfifo.dele(pn,1);
		// This is dumb. We hold the lock so we can "delete" the pointers and still pass
		// them from fred_and_lamont ( well named...). 
	
		char * cdum[sz];	
		float linep[sz];
		for (IdxTy i=0; i<sz; ++i) 
		{	Ty * c=jfifo.pread();
			cdum[i]=c;
		linep[i]=atof(c); jfifo.dele(c,strlen(c)+1);  }
		// this needs to be fast since we still retain the lock here... 
		// dddo.push_from_model(sz,linep);	
	
		dddo.push_from_model(sz,cdum);
	}
	//dddo.recalculate(); 
	//dddo.trim_model(50);
	// dos()<<" write "; for ( int ii=0; ii<7 ; ii++) dos()<<linep[ii]<<" "; dos()<<'\n';
	//model_blocked();
std::cout<<MM_MARK<<" model jfifo loop exit  "<<" "<<"\n";
	bloader.exit_out();

}
////////////////////////////////////


void _run_model() {
std::cout<<MM_MARK<<" model running \n";
background_loader &bloader=defered->bloader;
background_junk_bin& bfifo=defered->bfifo;
fred_and_lamont& jfifo=defered->jfifo;
dd_display_options& dddo=defered->dddo;
run_model();  
dddo.update();

}
void help_msg()
{
	std::ostream & os= std::cout;
	os<<" Usage: streaming_data \n";
	os<<"    -something to run\n";
	os<<"      Program to generate and display model data generated for indefinite duration.\n";
	os<<"      Uses my earlier glut demo program/molecule viewer\n";
	os<<"      For full command list, type 'idle' in viewer window and then 'help'\n";
	os<<"      Contact: marchywka@hotmail.com, Mike Marchywka 2008\n"; 
}
void about_msg()
{
	std::ostream & os= std::cout;
	os<<" aboutmsg\n";
	os<<"   Mike Marchywka, 2006. Uses NIST matrix code and various other source.\n";
	os<<"  References:\n";
	os<<"     Originally developed using cygwin.\n";	
	os<<"      matrix code downloaded from the Mathworks and NIST; see\n";  
	os<<"     http://math.nist.gov/javanumerics/jama\n";
	os<<"     http://mathworld.wolfram.com/Studentst-Distribution.html\n";
	os<<"     http://en.wikipedia.org/wiki/Student's_t-distribution\n";
	os<<"     http://citeseer.ist.psu.edu/hoppe94piecewise.html\n";
	os<<"     http://citeseer.ist.psu.edu/597835.html\n";
	os<<"     http://www.mat.univie.ac.at/~nuhag/papers/irreg.html\n";
	os<<"     http://www.math.umd.edu/~jjb/recentmss.html\n";
	
	
	
}
int main(int argc, char **argv) 
{	

// movefromother file, hope to miss some errors
// SOME OTHER  CALLS THSI FUDFK

  glut_generic::almost_start_glut(argc,argv);


defer defered_make;
defered=&defered_make;

background_loader &bloader=defered->bloader;
background_junk_bin& bfifo=defered->bfifo;
fred_and_lamont& jfifo=defered->jfifo;
dd_display_options& dddo=defered->dddo;
// This does not validate command line or config file parameters


	int x=global_config.load(argc,argv,&help_msg, &about_msg);
	if (x!=0) return x; 
	dd.configure();
	//char * leader=0;
	typedef std::string St;
	St leader;
	global_config.get(St("leader"),leader);
	std::cerr<<"QQQQQQQQQQQQQQQQQQ"<<leader<<".\n";
	dddo.configure(global_config); 
	// The command handler is not really thread safe but we only yse idle right now...	
	if (leader.length()!=0)bloader.set_leader(leader.c_str());
	bloader.set_panic(&panic);
	bloader.init(&line_handler);
	// needed to init before glut is running...
	// run_model();
// maybe not???
	dddo.init();
//	data_display::display_stl(p_list);
//	model_init(_run_model,&argc,argv);
	int fake=1; char* fs[]={"dummy","dummy"};
	ggb->set_df(data_display::draw_a_thing);
	int w=512, h=512;
	global_config.get(St("width"),w);
	global_config.get(St("height"),h);
	glut_generic::set_ini_dim(w,h);

// someother callsthis 
//  glut_generic::almost_start_glut(argc,argv);

//	dddo.init();

	model_init(_run_model,&fake,fs);
	//ggb->set_idle(_run_model); ggb->idle_enable(true); glut_generic::start_glut(&argc,argv);
	return 0;
	}	
	
	


