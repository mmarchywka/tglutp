#include "../math/student_t_stat.h"
#include "../things/trial_group.h"
#include "../debug_and_support.h"
#include "../data_and_config.h"

#include "../view/glut_generic.h"
#include "../atom/atom_display.h"
#include "../data_display.h"
#include "../tess_display.h"
 
//#include "../math/linear_regression_matrix.h"
#include "test/noisy_signal_source.h"
#include "test/testing_histogram.h"

#include "./math/hypergeometric.h"

// uses gsl 
#include <gsl/gsl_sf_hyperg.h>
#include <math.h>
config_type global_config;
data_display dd(global_config);

glut_generic global_generic;

glut_generic_base* ggb=NULL;
typedef list<float*> IdleListTy;
IdleListTy p_list;
/////////////////////////////////////////////////
//extern void run_model();
void push_from_model(float *x);
typedef float coord_type;

typedef noisy_signal_source<coord_type> SigTy;
int lop=0;
int trial=0, four_months=0,means_differ=0;
typedef testing_histogram<1000,float> HistoTy;
	HistoTy histo(10,250),histoxx(10,250),histon(10,250);

trial_group placebo, drug;
student_t_stat tstat;
int DOF=198;
void run_model()
{
	if (lop==0)
	{
	histo.color(1.0f,0,0); histoxx.color(0,1,0); 	
	histon.color(0,.5f,1); //histon.m()=50; 	
		tstat.fill(DOF);
	}
	lop++;
	
for (int j=0; j<100; ++j)
{

placebo.clear(); drug.clear();
for (int i=0; i<100; ++i)
{
	placebo.add_patient(40.0*SigTy::myrand(0));
	drug.add_patient(40.0*SigTy::myrand(0));
}
placebo.compute(); drug.compute();


//std::cout<<" medians :"<<placebo.median()<<" "<<drug.median()<<"\n";
//std::cout<<"slope :"<<placebo.slope()<<" "<<drug.slope()<<"\n";
++trial;
if (drug.median()-placebo.median()<4){ ++four_months;
std::cout<<(1.0f*four_months/trial)<<" trials ="<<trial<<" hit by "<<(drug.median()-placebo.median())<<"\n";
std::cout<<" medians :"<<placebo.median()<<" "<<drug.median()<<"\n";
std::cout<<"slope :"<<placebo.slope()<<" "<<drug.slope()<<"\n";
}

//for(int ii=1; ii<200; ii+=10)
//std::cout<<"g test "<<gamma_functions::gamma_ratio_half_int(199,198)<<"\n";
std::cout<<"g test "<<hypergeometric_functions::series_2f1(.25,.5,.75,80.0/81)<<"\n";
 std::cout<<"g test "<<hypergeometric_functions::series_2f1(1.0/3.0,2.0/3.0,5.0/6.0,27.0/32)<<"\n"; 
  
histo.add_point(placebo.median());
histoxx.add_point(drug.median());
histon.add_point(placebo.median()-drug.median()); 
typedef coord_type time_type;  
time_type n1=placebo.N();
time_type n2=drug.N();  
time_type dof=n1+n2-2;    
//http://mathworld.wolfram.com/Studentst-Distribution.html
//http://en.wikipedia.org/wiki/Student's_t-distribution
time_type s1=placebo.sd(), s2=drug.sd();  
time_type gsd= sqrt(((n1-1)*s1+(n2-1)*s2)*(n1+n2)/n1/n2/(dof)); 
//time_type diff=
time_type test_stat= (drug.avg()-placebo.avg())/gsd;
time_type combo=numeric_combos::student(test_stat,198); 
time_type p=1-tstat.t(test_stat); 
std::cout<<" p diffs :"<<combo<<" "<<(1-p)<<"\n"; 
if(p<(.025f)) {++means_differ;
std::cout<<"stats:" <<drug.avg()<<" "<<placebo.avg()<<" "<<test_stat<<" "
	<<p <<" "<<(1.0f*means_differ/trial) <<"\n";

}
}
 
//typedef std::ostringstream OsStreamTy;
//{ OsStreamTy  os; histo.find_peaks(os) ; dd.add_text("kluge",os.str());  }
//{ OsStreamTy  os; histoxx.find_peaks(os) ; dd.add_text("kluge2",os.str());  }

//dd.add_text("hello","there");
 //std::cout<<"model xxx\n";
//histo.find_peaks(std::cout); histoxx.find_peaks(std::cout);
histo>> push_from_model; histoxx>>&push_from_model;
histon>>push_from_model;


}
////////////////////////////////////

void push_from_model(float *x)
{	
	 float * own = new float[7];	
	 for ( int ii=0; ii<7 ; ii++) own[ii]=x[ii];
	p_list.push_back(own);
	
	}
void _run_model() {
//	std::cout<<"model call made\n\n"; std::cout.flush();
IdleListTy::iterator ii=p_list.begin(),ie=p_list.end();
while (ii!=ie) delete *(ii++); p_list.clear();
run_model();  data_display::display_stl(p_list);glutPostRedisplay();}
void help_msg()
{
	std::ostream & os= std::cout;
	os<<" Usage: simumain \n";
	os<<"    -something to run\n";
	os<<"      Program to generate and display model data generated for indefinite duration.\n";
	os<<"      Uses my earlier glut demo program/molecule viewer\n";
	os<<"      For full command list, type 'idle' in viewer window and then 'help'\n";
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
//	global_config.load(argc,argv);
	int x=global_config.load(argc,argv,&help_msg, &about_msg);
	if (x!=0) return x; 
	dd.configure();
run_model();
data_display::display_stl(p_list);
ggb->set_idle(_run_model);
ggb->idle_enable(true); 
glut_generic::start_glut(&argc,argv);
	return 0;}	
	
	


