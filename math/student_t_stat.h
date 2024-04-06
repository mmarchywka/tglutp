#ifndef STUDENT_T_STAT_H__
#define STUDENT_T_STAT_H__

#include <math.h>

// for non-parametric stuff, see for example
//http://www.meduniwien.ac.at/imc/biometrie/programme/xlinrank/TechRep08_2002.pdf

class student_t_stat
{
//	enum (DOF=_DOF);
	typedef float data_type;
	typedef int count_type;
	typedef int index_type;
public:	
student_t_stat(): m_min(-10), m_dx(.02)  {}
student_t_stat(const index_type DOF): m_min(-10), m_dx(.02) 
{
	fill(DOF);
	}
	// non-normalized
static data_type y(const data_type x, const index_type DOF) 
{	
	data_type a=(1+x*x/DOF); 
	return ::pow(a,-(1.0f+DOF)/2.0f);     
}
data_type integrate(const data_type xi, const data_type xf,const index_type DOF)
{
data_type sum=0;
data_type xxf=xf, xxi=xi;
while (xxi!=xxf){
while (1) { 
data_type dy= (y(xxf,DOF)-y(xxi,DOF))/y(xxf,DOF);
// monotonic 
if (dy<.001) break;
xxf=(xxf-xxi)/2;
}
sum+=y(xxf,DOF)*(xxf-xxi);
xxi=xxf;
xxf=xf;
} 

return sum; 	
}
data_type t(const data_type x) const 
{  
	index_type i=(int)((x-m_min)/m_dx);
	if (i>999) i=999;
	if (i<0) i=0;
	return m_s[i]/m_s[999]; 

}
void fill(const index_type DOF)
{	
	data_type v=m_min;
	data_type integ=0; 
	m_s[0]=integrate(m_min-100*m_dx,m_min,DOF);
	for (int i=1; i<1000; ++i)
	{
	integ=integrate(v-m_dx,v,DOF);
		m_s[i]=integ+m_s[i-1];
		v+=m_dx;
		}
}	
data_type m_min, m_dx;
data_type m_s[1000];	
	
};

#endif
