#ifndef TESTING_HISTOGRAM_H__
#define TESTING_HISTOGRAM_H__
#include <queue>

#include "../base/points_base_classes.h"

template <int SZ,class xDataTy> class testing_histogram
{
	
typedef testing_histogram Myt;
typedef xDataTy DataTy;
typedef DataTy data_type;
typedef /* long unsigned  */ int acc_type;
typedef int count_type;
typedef float color_type;
typedef count_type IdxTy;
enum {SIZEE=SZ};
// not thread safe...
static count_type & ctor_serial() { static count_type idx=0; return idx; }
count_type m_inst,m_points;
public:
	testing_histogram(const data_type m=1, const data_type b=0)
		:m_inst(++ctor_serial()),m_points(0),m_x(m),m_b(b),m_rr(1),m_gg(1),m_bb(1) 
		{Clear();}

bool add_point(const data_type x)
{
data_type y=Xform(x);	
IdxTy i=IdxTy(y+data_type(.5f));
if (i<0) i=0; //return false;
if (i>=SIZEE)  i=SIZEE-1; //return false;
m_a[i]++;++m_points;
return true;
}
data_type & m() { return m_x; }
data_type & b() { return m_b; }
void clear() { Clear(); }
void xform_param(const data_type m,const data_type b)
{m_x=m; m_b=b;}
void color( const color_type r, const color_type g, const color_type b)
{ m_rr=r; m_gg=g; m_bb=b;}
class ptr_cmp{ //typedef testing_histogram::acc_type acc_type;
public: bool operator()(const acc_type* x, const acc_type*y) const
	{	return (*x)<(*y); }};
template <class Tx > void find_peaks(Tx & os) const
{
	const acc_type * a=m_a+2;
	const acc_type * af= a+SIZEE-4;
	priority_queue<const acc_type*,vector<const acc_type*>,ptr_cmp> max_que;
	while (a!=af) 
	{
		if(*a>10) if( (*a>*(a-2))&&(*a>*(a-1))&&(*a>*(a+1))&&(*a>*(a+2)))
		{max_que.push(a); }
		++a;
	}
	os<<m_inst<<" peak ";
	count_type lim=max_que.size(); if (lim>3) lim=3;
	while (lim--)
	{ 	a=max_que.top(); max_que.pop();  
		os <<"bin "<<(a-m_a)<<" x="<<InvXform(a-m_a)<<" cnt="<<*a<<"|";  }
	os<<"\n";
}
typedef void (*DestFuncTy)(float*);

Myt& operator>> (const DestFuncTy  p)
{
	const count_type tick=10;
	for (IdxTy i=0; i<SIZEE; ++i) 
{	
	float offset=10*m_inst; 
	bool hi_lo=(((i/tick)&1)==0);
	color_type f=(hi_lo)?1.2f:.8f;
	float crap[] = {3.0f,i,(::log(m_a[i]+1))*5,offset, m_rr*f,m_gg*f,m_bb*f};
	(*p)(crap);}
return *this; 	
	}
private:
void Clear() {m_points=0; for (IdxTy i=0; i<SIZEE; ++i) m_a[i]=0; }
data_type Xform(const data_type x)
{ return m_x*x+m_b; }
data_type InvXform(const acc_type i) const
{ return (i-m_b)/m_x; }
data_type m_x,m_b;
color_type m_rr,m_gg,m_bb;
acc_type m_a[SIZEE];
	
};


#endif

