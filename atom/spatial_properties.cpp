#include "spatial_properties.h"
#include "../nist/jama_svd.h"
#include "../nist/tnt_array2d_utils.h"
#include "../base/cheap_greps.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../nist/NistUtil.h"

	typedef spatial_properties MyClass;
	typedef TNT::Array2D<MyClass::coord_type> MatTy; // Could have an "N" specialization...
	typedef MatTy VectorTy; // 
	typedef JAMA::SVD<MyClass::coord_type> InverterTy;
	typedef point3_array<MyClass::coord_type,3> ExMatTy;
	typedef point3_template<MyClass::coord_type> PointTy;
		

ExMatTy  crap(const MatTy & m) 
{   
	typedef MyClass::count_type count_type;
	ExMatTy x;
	MY_ITOR(m,i,j) {x[i][j]=m[i][j];  }	
	return x;
}
namespace {
basic_ostream<char> & operator <<(basic_ostream<char>& is, 
const point3_array<float,3> & x)
{ is<<x[0]; is<<x[1]; is<<x[2];return is; } 
}
void MyClass::Configure(config_type & cf, StParse& p, const string_type nm)
	{
		if (p.str().find("?")!=string_type::npos)
		{
			std::cout<<" axes,centroid\n";
			}
	// hard coded defaults.
	string_type hard_coded_defaults= 
		"target=1 start=0,0,0 ds=.2 dy=1 points_i=100 points_j=100 imax=200";
	// default key to look in config for config defaults
	string_type default_key="propertykey";
	// over ride in order: hard coded, config file key, config values, user input
	typedef greps::eqn_subs util_class;
	string_type resultant=util_class::netMap(cf,p,hard_coded_defaults,default_key);	
	cf.unload(nm); cf.load(nm+" "+resultant);
	// The selection criteria names can't conflict with function names...
	m_sel.select_fields(cf,nm);
	
	AddCmd(cf,nm,"axes");
	AddCmd(cf,nm,"centroid");
	AddCmd(cf,nm,"selected");
//	string_type dest="";
//	cf.parse(nm,"axes",dest);
//	if (dest.length()!=0) m_lut["axes"]=dest;
//	dest="";
//	cf.parse(nm,"centroid",dest);
//	if (dest.length()!=0) m_lut["centroid"]=dest;
	return ;
	}
void MyClass::calculate(SrcTy* src)
{ 
	LutItor ii=m_lut.begin(), ie=m_lut.end();
	while (ii!=ie)
	{
		string_type nm=(*ii).first;	
		if (nm=="centroid") centroid(src,nm);
		else if (nm=="axes") axes(src,nm);
		else if (nm=="selected") selected(src,nm);
		++ii;
	}	
}
void MyClass::selected(SrcTy* src, const string_type & nm)
{
// 2024
//	PointTy x=PointTy(0,0,0);
	count_type n=0;
	string_buffer_type sb;
	SrcTy::iterator ii=src->begin(),ie=src->end();
	while (ii!=ie) { 
		if (m_sel.fits_criteria(*ii)){sb<<PointTy(*ii); ++n;}++ii; 
	}
	m_store.store(m_lut[nm]+"N",n);
	m_store.store(m_lut[nm],sb.str());
}
void MyClass::centroid(SrcTy* src, const string_type & nm)
{
	PointTy x=PointTy(0,0,0);
	count_type n=0;
	SrcTy::iterator ii=src->begin(),ie=src->end();
	while (ii!=ie) { 
		if (m_sel.fits_criteria(*ii)){x+=PointTy(*ii); ++n;}++ii; 
	}
	if (n!=0) x/=coord_type(n);
	m_store.store(m_lut[nm]+"N",n);
	m_store.store(m_lut[nm],x);
}
void MyClass::minmax(SrcTy* src, const string_type & nm)
{
	enum {MM=10000};
	PointTy xmin=PointTy(MM,MM,MM),xmax=PointTy(-MM,-MM,-MM);
	
	count_type n=0;
	SrcTy::iterator ii=src->begin(),ie=src->end();
	while (ii!=ie) { 
		if (m_sel.fits_criteria(*ii))
		{const PointTy &x=PointTy(*ii); 
		xmin.min(x); xmax.max(x);
		++n;}++ii; 
	}
	//if (n!=0) x/=coord_type(n);
	m_store.store(m_lut[nm]+"min",xmin);
	m_store.store(m_lut[nm]+"max",xmax);
}

void MyClass::axes(SrcTy* src, const string_type & nm)
{
	//http://en.wikipedia.org/wiki/Moment_of_inertia
	PointTy xx=PointTy(0,0,0);
	count_type n=0;
	SrcTy::iterator ii=src->begin(),ie=src->end();
	while (ii!=ie) { if (m_sel.fits_criteria(*ii)) {xx+=PointTy(*ii); ++n;}
	++ii; }
	if (n!=0) xx/=coord_type(n);
	
	MatTy m(3,3);
	MY_ITOR(m,i,j){m[i][j]=0; }
	//count_type 
	n=0;
	//SrcTy::iterator 
	ii=src->begin(),ie=src->end();
	while (ii!=ie)
	{
		if (m_sel.fits_criteria(*ii)){
	const PointTy  loc=PointTy(*ii)-xx;
	//MY_ITOR(m,i,j){m[i][j]+=; }
	m[0][0]+=loc[1]*loc[1]+loc[2]*loc[2];
	m[1][1]+=loc[0]*loc[0]+loc[2]*loc[2];
	m[2][2]+=loc[0]*loc[0]+loc[1]*loc[1];
	m[0][1]-=loc[0]*loc[1]; m[1][0]=m[0][1];
	m[0][2]-=loc[0]*loc[2]; m[2][0]=m[0][2];
	m[1][2]-=loc[1]*loc[2]; m[2][1]=m[1][2];
	++n; }
	++ii;
	}
	InverterTy inv(m);
	MatTy u(3,3),v(3,3),sss(3,3);
	inv.getU(u); inv.getV(v); inv.getS(sss);
//	std::cout<<"1Invert\n"; cout.flush();
	typedef NistUtil<coord_type> Nu;
	MatTy ut(3,3); ut=Nu::Transpose(u);
//	std::cout<<"2Invert\n"; cout.flush();
	MatTy x(3,3); MatTy si(3,3); si=sss; Nu::Recip(si);
//	std::cout<<"3Invert\n"; cout.flush();
	x=TNT::matmult(v,si); x=TNT::matmult(x,ut);
	//m_A_inv=x;
	ExMatTy xxx=crap(x);
	xxx/=coord_type(n);
	m_store.store(m_lut[nm],xxx);
	xxx=crap(sss); m_store.store(m_lut[nm]+"S",xxx);
	xxx=crap(ut); m_store.store(m_lut[nm]+"ut",xxx);
	xxx[0].normalize();xxx[1].normalize();xxx[2].normalize();
	 m_store.store(m_lut[nm]+"utn",xxx);
}	
	
	
#undef MY_ASSERT
#undef MY_ITOR
