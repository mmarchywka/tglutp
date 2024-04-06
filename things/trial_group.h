#ifndef TRIAL_GROUP_H__
#define TRIAL_GROUP_H__
#include "../math/linear_regression_matrix.h"
#include <vector>
#include <algorithm>

class trial_group
{
public:
typedef float time_type;
typedef int index_type;
typedef linear_regression_matrix<2,time_type> RegTy;
typedef std::vector<time_type> SurTy;
void clear() 
{m_deaths.clear(); m_model.reset_model(); }
void add_patient(time_type tod) { m_deaths.push_back(tod); }
void compute()
{
	SurTy::iterator ii=m_deaths.begin(),ie=m_deaths.end();
	std::sort(ii,ie);
	m_model.reset_model();
	index_type total=m_deaths.size();
	index_type i=0;
//	m_model.add_point(0,1);
	ii=m_deaths.begin();ie=m_deaths.end();
	while (ii!=ie) {++i; 
	//std::cout<<"point "<<(*ii)<<" "<< (1.0f-1.0f*i/total)<<"\n";
	m_model.add_point( (*ii),1.0f-1.0f*i/total ); ++ii;  }
	m_model.invert_model();
	m_model.solve();
}
time_type median() { return m_deaths[m_deaths.size()/2]; }
time_type slope() { return m_model.coef(1); }
index_type N() const { return m_deaths.size(); }
time_type avg() const 
{	time_type a=0;
	SurTy::const_iterator ii=m_deaths.begin(),ie=m_deaths.end();
	while (ii!=ie) {a+=(*ii); ++ii;}
	return (N()>0)?(a/N()):0;
	
}
time_type sd() const 
{	time_type a=avg(),sd=0;
	SurTy::const_iterator ii=m_deaths.begin(),ie=m_deaths.end();
	while (ii!=ie) {sd+=((*ii)-a)*((*ii)-a); ++ii;}
	return (N()>1)?(sd/(N()-1)):0;
}

SurTy m_deaths;
RegTy m_model;
}; // placebo, drug;
#endif
