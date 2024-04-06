#ifndef NONUNIFORM_WAVELET_MODEL_H__
#define NONUNIFORM_WAVELET_MODEL_H__

#include "../math/abstract_modelling_matrix.h"

//http://citeseer.ist.psu.edu/hoppe94piecewise.html

template <Tr::count_type _xN, class _xDataTy >class non_uniform_wavelet_model // modelling_matrix
:  public abstract_modelling_matrix<_xN,_xDataTy>
{
	typedef abstract_modelling_matrix<_xN,_xDataTy> Super;
 	
	typedef typename Super::data_type data_type;
	typedef typename Super::count_type count_type;
	typedef typename Super::PointItorTy PointItorTy;
public:
	
// Note these may NOT be initialized...
//modelling_matrix
// 2008-03-20 typos?
non_uniform_wavelet_model():Super(),m_valid(false) {}

// compiler
typedef typename Super::DataPointTy FuTy;
void set_y_value(const count_type i,const data_type y) {SetYValue(i,y); } 
void clear_point_stack() { Super::m_points.clear(); m_valid=false;  }
//[row][column] multiply takes same inner : [a][x]*[x][b]
void add_point(const data_type x, const data_type y)
{  
	Super::m_points.push_back(FuTy(x,y));
	Super::m_state=(Super::m_state+1)%Super::N;  //std::cout<<"Adding at "<<m_state<<"\n";
	if ( Super::m_points.size()>2*Super::N) { 
		MY_ASSERT(Super::m_sum_x,Super::m_sum_x<x,x,"non-monotinic x coords" )
		SetupA();Super::Process(); m_valid=true;
		//{std::cout<<Transpose(m_X)<<"\n";}
		Super::m_points.pop_front(); }
	Super::m_sum_x=x;
}
void assert_state(const count_type s) 
{ MY_ASSERT(s,s==Super::m_state,Super::m_state," I thought this was the state " )}

void invert_model() {Super::Invert();}
void check_quality() { Super::CheckQuality(); }
void solve() { Super::Solve();	 }
void DeriveFilter(){}
// This assumes y={1,0,0,0,0}
count_type xSetScaleAndZero(const count_type row, const data_type s)
{ 
	count_type i=0; 
	while (i<Super::N){ Super::m_A[row][i]=data_type(s);  Super::m_A[row+1][i++]=data_type((i&1)?(1):(-1));}  
	return row+2;
}
count_type SetScaleAndZero(const count_type row, const data_type s)
{ // This produces 1/16,4/16,3/16,4/16,1/16 
count_type i=0; 
	while (i<Super::N){ Super::m_A[row+1][i]=data_type(s);  Super::m_A[row][i++]=data_type((i&1)?(1):(-1));}  
	return row+2;
} 
// The above works fine but impulse response is all over- need to adjust scale.
// http://citeseer.ist.psu.edu/597835.html
//Irregular Sampling of Wavelet and Short Time Fourier Transforms

count_type SetScaleAndZerox(const count_type row, const data_type s)
{ // This DOESN:t produces 1/16,4/16,3/16,4/16,1/16 
count_type i=0; PointItorTy li=Super::m_points.end(); 
m_xf=(*(--li)).first; //count_type ii=N; //while ((--ii)>0) --li;
	 //m_xi=(*(li)).first;
	 //m_kluge=-(*(++li)).first; // 1
	 //m_kluge+=2*(*(++li)).first; // 1
	// m_kluge-=(*(++li)).first; // 2 ( middle) 
	while (i<Super::N){ Super::m_A[row+1][i]=data_type(1);  
	m_xi=(*(--li)).first;
	  Super::m_A[row][i]=//((i==2)?4:1)*
	  //(m_xf-m_xi)*
	  data_type((i&1)?(1):(-1));
	m_xf=m_xi; i++;
	//m_A[row][0]=1; m_A[row][1]=1;m_A[row][2]=-4;
	//m_A[row][3]=1;m_A[row][4]=1;
	
	}  
	
	 
	//m_A[row][(N/2)+1]=data_type(s);
	//m_A[row][(N/2)+0]=-data_type(s);
	return row+2;
} 
count_type AddZeroMoments(const count_type row, const count_type n)
{
	const count_type points=Super::N; count_type idx=row; const count_type lim=n+row;
	PointItorTy li=Super::m_points.end();data_type e_x=0;
	for ( count_type i=0; i<points; i++){ e_x+=(*(--li)).first; }
	e_x/=points;
	li=Super::m_points.end();
	for (count_type state=0; state<points; ++state){
		data_type x_=(*(--li)).first-e_x; data_type xx=x_;  
		// this relies on A being initialized...
		idx=row; while (idx<lim ){ Super::m_A[idx++][state]+=xx; xx*=x_;} 
	} 
	return idx;
	}
///http://www.mat.univie.ac.at/~nuhag/papers/irreg.html
//http://www.math.umd.edu/~jjb/recentmss.html
void SetupA() 
{	// Move all of the calculation of A to here- it can depend on all the points...
	count_type current_eqn =SetScaleAndZero(0,1);
	current_eqn=AddZeroMoments(current_eqn,Super::N-2);
	MY_ASSERT(current_eqn,current_eqn==Super::N,Super::N," equations dont match size");
	
	
}
bool m_valid;
data_type m_xi,m_xf,m_kluge;

};



#endif
