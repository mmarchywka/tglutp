#ifndef ABSTRACT_MODELLIING_MATRIX_H__
#define ABSTRACT_MODELLIING_MATRIX_H__
#include "../nist/jama_svd.h"
#include "../nist/tnt_array2d_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <map>
#include <vector>
#include <list>
//#include "data_display.h"

// be sure to undef when done :)
#define MY_ASSERT(x,y,w,z) if(!(y)) {std::cout<<(z)<<" but have "<<(x)<<" vs "<<(w)<<"\n";}
#define MY_ITOR(b,c,d)  const count_type M__ = (b).dim1();const count_type K__ = (b).dim2();\
for (count_type (c)=0; (c)<M__; (c)++)for (count_type (d)=0; (d)<K__; (d)++)
namespace Tr
{
	typedef int count_type; 
	typedef int flag_type;
	};

template <Tr::count_type _xN, class _xDataTy >class abstract_modelling_matrix
{
	protected:
	
	typedef abstract_modelling_matrix<_xN,_xDataTy> Myt;
	typedef  _xDataTy DataTy;
	typedef TNT::Array2D<DataTy> MatTy; // Could have an "N" specialization...
	typedef MatTy VectorTy; // 
	typedef JAMA::SVD<DataTy> InverterTy;
	
	typedef pair<DataTy,DataTy> DataPointTy;
	typedef list<DataPointTy> PointStackTy;
	typedef typename PointStackTy::const_iterator PointItorTy;

template <class Tc> const Tc TC(const Tc& x) { return data_type(x); }	

public:

enum {N=_xN,M=_xN}; // used to be a problem with template parameters???
	typedef  DataTy data_type;
	typedef  Tr::count_type count_type;
	typedef Tr::flag_type flag_type;

// This is ok- y is a COLUMN vector and x gets reset...
abstract_modelling_matrix(): m_A(N,M),m_A_inv(N,M),m_X(N,1),m_Y(N,1),m_u(N,M),m_v(N,M),m_s(N,M)
//abstract_modelling_matrix(): m_A(N,M),m_A_inv(N,M),m_X(1,M),m_Y(1,M),m_u(N,M),m_v(N,M),m_s(N,M)


,m_tol(1e-4),m_debug_flag(0),m_was_ok(true),m_result(0)
{ // only care about forward part right now
Initialize(); Clear(m_X); Clear(m_Y); SetYValue(0,1);	}

data_type condition() const {return m_s[0][0]/m_s[N-1][N-1]; }

virtual ~abstract_modelling_matrix() {}
void SetYValue(const count_type i,const data_type y)
{
	MY_ASSERT(i,i<N,N," y index value too high")
	MY_ASSERT(i,i>=0,0," y index too low")
	m_Y[i][0]=y;
}	
virtual void DeriveFilter()
{//PointItorTy li=m_points.end();
	for (count_type state=0; state<N; ++state){
	//data_type x_=(*(--li)).second;
	m_X[N-state-1][0]*=((state&1)?1:-1);}
	if (m_debug_flag&4){std::cout<<m_X ;}
	}
	void Filter()
{
//	count_type top=m_points.size()-1; 
//	std::cout<<"process with "<<top<<" pts\n";
	data_type y=0;
	PointItorTy li=m_points.end();
	for (count_type state=0; state<N; ++state){
	data_type y_=(*(--li)).second;
	//y+=((state&1)?1:-1)*x_*m_X[N-state-1][0];}
	y+=y_*m_X[state][0];}
	if ( m_debug_flag&1) {std::cout<<" "<<y<<"\n"; }
	m_result=y;
}
void DumpRelevantPoints()
{PointItorTy li=m_points.end();
	for ( count_type i=0; i<N; i++) 
	{DataPointTy xy=(*(--li));  std::cout<<"("<<xy.first<<","<<xy.second<<")";   }
	}
void SetupA() 
{	// Move all of the calculation of A to here- it can depend on all the points...
	
	count_type i=0; 
	while (i<N){ m_A[0][i]=data_type(1);  m_A[1][i++]=data_type((i&1)?(1):(-1));}  
	const count_type points=N;
	data_type e_x=0;
	PointItorTy li=m_points.end();
	for ( count_type i=0; i<points; i++){ e_x+=(*(--li)).first; }
	e_x/=points;
	li=m_points.end();
//	count_type top=m_points.size()-1; 
//	std::cout<<"process with "<<top<<" pts\n";
	for (count_type state=0; state<points; ++state){
	data_type x_=(*(--li)).first-e_x; data_type xx=x_;  
	count_type i=2; while (i<N){ m_A[i++][state]+=xx; xx*=x_;} 
}}
void Process() {
//std::cout<<"Invert\n"; cout.flush();
	Invert();
	//std::cout<<"Solve\n"; cout.flush();
	Solve();
	if (m_debug_flag&2) if (m_s[0][0]<.01*m_s[N-1][N-1]) {std::cout<<Transpose(m_X)<<"\n";}
	CheckQuality();
	DeriveFilter();
	Filter();
	Initialize();
	};
void Initialize()
{ Clear(m_A); 
m_state=0; m_sum_x=0;
}
void Solve() { m_X=matmult(m_A_inv,m_Y);	 }

void Invert()
{
	InverterTy inv(m_A);
	inv.getU(m_u); inv.getV(m_v); inv.getS(m_s);
//	std::cout<<"1Invert\n"; cout.flush();
	MatTy ut(N,M); ut=Transpose(m_u);
//	std::cout<<"2Invert\n"; cout.flush();
	MatTy x(N,M); MatTy si(N,M); si=m_s; Recip(si);
//	std::cout<<"3Invert\n"; cout.flush();
	x=TNT::matmult(m_v,si); x=TNT::matmult(x,ut);
	m_A_inv=x;
	}
void CheckQuality() //const
{
	MatTy x(N,M);
	x=matmult(m_A,m_A_inv); // should be I
	m_was_ok=NearlyIdentity(x,m_tol);
	if (!m_was_ok) if (m_debug_flag&2){std::cout<<x ;
	std::cout<<"\nIs not identity\n"; }
	//for (count_type i=0; i<N; ++i) std::cout<<m_s[i][i]<<" "; std::cout<<"\n";
	if (m_s[0][0]<.01*m_s[N-1][N-1]) {std::cout<<__FILE__<<__LINE__<<m_s[0][0]<<" "<<m_s[N-1][N-1]<<"\n"; }
}


void Clear (MatTy & m)  const
{ MY_ITOR(m,i,j) {m[i][j]=0; } }
MatTy Transpose( const MatTy & m) const
{	MatTy mt(m.dim2(),m.dim1()); 
MY_ITOR(m,i,j) {    // cout<<i<<" "<<j<<"\n";
    ;  mt[j][i]=m[i][j];}  return mt;}
void Recip(  MatTy & m) const
{ MY_ITOR(m,i,j) {data_type& x= m[i][j]; if(x) x =data_type(1)/x;}}
bool NearlyIdentity(MatTy & x,const data_type tol) const
{const data_type tol2=tol*tol;  MY_ITOR(x,i,j) { data_type d=x[i][j]-((i==j)?1:0);  
	if(d*d>tol2 ) return false;   }  return true;    }

data_type Trace(const MatTy & x) const
{  data_type t=0; count_type i=(x.dim1()<x.dim2())?x.dim1():x.dim2(); 
while (i){ t+=x[i][i]; i--;} return t; } 

// Model is AX=Y;
	MatTy m_A,m_A_inv;
	VectorTy m_X,m_Y;
	// model is assessed with SVD stuff
	MatTy m_u,m_v,m_s;
	count_type m_state;
	data_type m_tol,m_sum_x;
	
	PointStackTy m_points;
	flag_type m_debug_flag;
	public: bool m_was_ok; data_type m_result;
	
	
	
	}; // class


#undef MY_ITOR


#endif
