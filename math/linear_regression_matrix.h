#ifndef LINEAR_REGRESSION_MATRIX_H__
#define LINEAR_REGRESSION_MATRIX_H__
//#include "test/fake_test_main.h"
//#include "test/testing_histogram.h"
//#include "test/noisy_signal_source.h"

#include "abstract_modelling_matrix.h"
#include <sstream>

template <Tr::count_type _xN, class _xDataTy >class linear_regression_matrix
:  public abstract_modelling_matrix<_xN,_xDataTy>
{
	typedef abstract_modelling_matrix<_xN,_xDataTy> Super;
 	enum {PARAMETERS=_xN,P=_xN};
	typedef typename Super::data_type data_type;
	typedef typename Super::count_type count_type;
	typedef typename Super::PointItorTy PointItorTy;
public:
	
// Note these may NOT be initialized...
linear_regression_matrix():Super(),m_valid(false) {ResetModel();}
typedef typename Super::DataPointTy FuTy;
//[row][column] multiply takes same inner : [a][x]*[x][b]
void add_point(const data_type x, const data_type y)
{  
	Super::m_points.push_back(FuTy(x,y));
	Super::m_A[0][0]+=1;
	Super::m_A[0][1]+=x;
	Super::m_A[1][1]+=x*x;
	Super::m_Y[0][0]+=y;
	Super::m_Y[1][0]+=y*x; ///????
	// cen defer for later...
	Super::m_A[1][0]=Super::m_A[0][1];
}
void add_point(const data_type* x, const data_type y)
{  
#define DDITOR(zz) for (count_type i=zz; i<P; ++i) for (count_type j=j; i<P; ++j) 
DDITOR(1) Super::m_A[i][j]+=x[i-1]*x[j-1];
for (count_type i=1; i<P; ++i) { Super::m_Y[i][0]+=y*x[i];  Super::m_A[0][i]+=x[i];}
Super::m_Y[0][0]+=y; Super::m_A[0][0]+=1;
DDITOR(0) Super::m_A[j][i]=Super::m_A[i][j];
	Super::m_points.push_back(FuTy(x[0],y));
//	m_A[1][0]=m_A[0][1];

#undef DDITOR
}


void reset_model() { ResetModel();  }
void invert_model() {Super::Invert();}
void check_quality() { Super::CheckQuality(); }
void solve() { Super::Solve();	 //std::cout<<m_X<<"\n";
//CheckQuality(); 
//for (count_type i=0; i<N; ++i) std::cout<<m_s[i][i]<<" "; std::cout<<"\n";
//std::cout<<" dimx "<<m_X.dim1()<<" "<<m_X.dim2()<<" "<<m_Y.dim1()<<" "<<m_Y.dim2()<<" \n";
//std::cout<<" interctp = "<<coef(0)<<" lsope = "<<coef(1)<<"\n";


}                      
data_type & coef(count_type i) { return Super::m_X[i][0];  }
void all() {  Super::Invert(); Super::CheckQuality(); solve(); }
void ResetModel() {Clear(Super::m_A); Clear(Super::m_X); Clear(Super::m_Y); Super::m_points.clear();}

bool m_valid;

};

/////////////////////////////////////////////////////////////////////////


#if 0 

	typedef float mytype;
	typedef float model_type;
	noisy_signal_source<mytype> ss;
	linear_regression_matrix<2,model_type> mm,mmxx;
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
//	mm.clear_point_stack();
//	mmxx.clear_point_stack();
	ss.time_reset(); 
	for ( int i=0; i<1000; i++)
	{	
	//printf ("%d %g          \r", i,x);
	ss.Inc();
	mm.add_point(ss.x(),ss.y());
	mmxx.add_point(ss.x_(),ss.y());
	mm.all();
	mmxx.all();


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
mm.reset_model();
mmxx.reset_model(); 
}

#endif


#endif


