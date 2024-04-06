#ifndef MODELLIING_MATRIX_H__
#define MODELLIING_MATRIX_H__
#include "test/fake_test_main.h"
#include "test/testing_histogram.h"
#include "test/noisy_signal_source.h"

#include "./math/nonuniform_wavelet_model.h"

#include <sstream> // for stupid text conversion ONLY

	typedef float mytype;
	typedef float model_type;
	noisy_signal_source<mytype> ss;
	// 2008-03-20 apparently there was a typo "nonuniforn"
	// this was not a compiler problem...  
	non_uniform_wavelet_model<5,model_type> mm,mmxx;
	typedef testing_histogram<1000,float> HistoTy;
	HistoTy histo(100,500),histoxx(100,500),histon(100,500);
	int mmm=0;
	mytype scales=50;
 
 typedef data_and_config::string_type string_type;
void run_model()
{
	
	//histon.b()=-100;
	if ( mmm==0) { 
		histo.color(1.0f,0,0); histoxx.color(0,1,0); 	
	histon.color(0,.5f,1); histon.m()=50; 
	ss.x_noise(1).offset(40); 
	 global_config.get(string_type("cutoff"),ss.m_cutoff);
	 global_config.get(string_type("offset"),ss.m_offset);
	 global_config.get(string_type("xnoise"),ss.m_noise_x);
	 global_config.get(string_type("ynoise"),ss.m_noise_y);
		//cout<<"set cutoff to "<<ss.m_cutoff<<"\n"; 
	 global_config.get(string_type("hadapt"),histo.m());
	 global_config.get(string_type("hstatic"),histoxx.m());
	 global_config.get(string_type("hx2"),histon.m());
	global_config.get(string_type("scales"),scales);
		ss.m_hyst=true;
	
	}
	std::cout<<"start"<<"\n";
	cout.flush();
	mm.clear_point_stack();
	mmxx.clear_point_stack();
	ss.time_reset(); 
	for ( int i=0; i<1000; i++)
	{	
	//printf ("%d %g          \r", i,x);
	ss.Inc();
	mm.add_point(ss.x(),ss.y());
	mmxx.add_point(ss.x_(),ss.y());
	if (!mm.m_was_ok) {std::cout<<"At i="<<i<<"\n";mm.DumpRelevantPoints(); }
	
	model_type * xx=mm.m_X[0];
	//float n1=(xx[4]+xx[3]+xx[2]*0+xx[1]+xx[0])/(xx[2]-xx[3])/(xx[2]-xx[1])/4;
	float n1=(xx[4]+xx[3]+xx[2]*0+xx[1]+xx[0])/(xx[2])/(xx[2])/4;
	//float n1=(1.0f)/(xx[2]-xx[1]);
	//float n1=(2*xx[4]+xx[3]+xx[2]*0+xx[1]-2*xx[0])/(xx[2])/(xx[2])/4;
	float m1=mm.m_result, m2=mmxx.m_result;
	float maggxx=fabs(m2),magg=(m1);
	// 200-803-20 get rid of warnings unused... 
	//float rr= (maggxx>magg)?1:0; float gg=(maggxx>1)?1:0; float bb=(magg>1)?1:0;
//float crap[] = {3.0f,(mm.m_result)*n1, mmxx.m_result,mm.m_X[2][0]*300 ,
//magg/100,(magg>10 )?.1f:.6f, (magg>50 )?0.0f:((1.0f-magg/50 ))};	
//	float crap[] = {3.0f,(m1)*scales, m2*scales,m1*n1*scales ,rr,gg,bb};
	{float crap[] = {4.0f,ss.x_(), m1*scales/2/3+150,0 ,1,0,0};
	push_from_model(crap); }
{	float crap[] = {4.0f,ss.x_(), m1*n1*scales/3+200,0 ,1,1,1};
	push_from_model(crap); }
{	float crap[] = {4.0f,ss.x_(), m2*scales/3/3+225,0 ,1,0,1};
	push_from_model(crap); }
	float crap2[] = {4.0f,ss.x(),ss.y()-i+100,0 , (magg*n1>1)?1:0,(maggxx>1)?1:0, 0.5f};
	push_from_model(crap2); 
histo.add_point(mm.m_result);
histoxx.add_point(mmxx.m_result);
histon.add_point(xx[2]);

mmm+=1;
}
//std::cout<<"model xxxaaaaaaaaaaaaaaaaaa\n";
typedef std::ostringstream OsStreamTy;
{ OsStreamTy  os; histo.find_peaks(os) ; dd.add_text("kluge",os.str());  }
{ OsStreamTy  os; histoxx.find_peaks(os) ; dd.add_text("kluge2",os.str());  }

dd.add_text("hello","there");
 //std::cout<<"model xxx\n";
//histo.find_peaks(std::cout); histoxx.find_peaks(std::cout);
histo>> push_from_model; histoxx>>&push_from_model;
histon>>push_from_model;
//std::cout<<"model xxxzzzzzzzzzzzz\n";

}




#endif


