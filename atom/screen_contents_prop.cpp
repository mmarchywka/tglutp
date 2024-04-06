#include "screen_contents_prop.h"

//#include <list>
//#include <map>
//#include <vector>
//#include <string>
#include "../base/cheap_greps.h"
#include "../base/points_base_classes.h"
#include <GL/glut.h>

//	char * buf= new char[w*h*4];
//glReadPixels(0,0,w,h, GL_RGBA, GL_UNSIGNED_BYTE,buf);
//GL_DEPTH_COMPONENT,GL_FLOAT
//m_store.store(m_lut[nm]+"N",n);
//	m_store.store(m_lut[nm],sb.str());

#undef MY_ITOR
#define MY_ITOR for(int i=0; i<11; ++i) 
template <class Ty,class Tx> basic_ostream<Ty> & operator <<(basic_ostream<Ty>& is, 
my_scalar_array<Tx,11> & x)
{ is<<"{";  MY_ITOR is<<" "<<x[i]; is<<"}"; return is; } 
template <class Ty,class Tx> basic_ostream<Ty> & operator <<(basic_ostream<Ty>& is, 
const myPoint4<Tx> & x)
{ is<<"{";  is<<" "<<x[0]<<" "<<x[1]<<" "<<x[2]<<" "<<x[3]; is<<"}"; return is; } 
basic_ostream<char> & operator <<(basic_ostream<char>& is, 
const my_scalar_array<int,11> & x)
{ is<<"{";  MY_ITOR is<<" "<<x[i]; is<<"}"; return is; } 
#undef MY_ITOR


typedef screen_contents_prop MyClass; 

void MyClass::calculate(index_type w, index_type h)
{
	m_store.store(string_type("screenwidth"),w);
	m_store.store(string_type("screenheight"),h);
	LutItor ii=m_lut.begin(), ie=m_lut.end();
	while (ii!=ie)
	{
		string_type nm=(*ii).first;	
		
		if (nm=="bounds"){ Bounds(nm,w,h);}
	//	else if (nm=="percent") {Percent(nm,w,h);}
		else if (nm=="zstats") {Zstats(nm,w,h);};
		++ii;
	}	

}
void MyClass::Zstats(const string_type &nm,index_type w, index_type h)
{		typedef float Fl;
			Fl* buf= new Fl[w*h];
			glReadPixels(0,0,w,h, GL_DEPTH_COMPONENT,GL_FLOAT,buf);
			my_scalar_array<int,11> ar; ar=0; 
			Fl maxx_z=-100000, minn_z=1000000;
			for(int i=0; i<h;++i) for (int j=0; j<w;++j)
			{
				Fl val= buf[i*w+j];
				ar[int(val*10)]++;
				if (val!=1) if ( val>maxx_z) maxx_z=val;
				if (val<minn_z) minn_z=val;
			}
			//mins[0]=maxes[0]; mins[1]=maxes[1];
		//	m_store.store(m_lut[nm]+"N",n);
			m_store.store(m_lut[nm],ar);
			m_store.store(m_lut[nm]+"min",minn_z);
			m_store.store(m_lut[nm]+"max",maxx_z);
			delete[] buf;	
}
void MyClass::Bounds(const string_type &nm,index_type w, index_type h)
{		typedef myPoint4<int> Tp;		
		Tp mins=Tp(w,h,w,h),
			maxes=Tp(0,0,0,0);
			index_type n=0;
			char * buf= new char[w*h*4];
			unsigned int * ibuf =(unsigned int*)buf;
			const unsigned int blank=0;
			glReadPixels(0,0,w,h, GL_RGBA, GL_UNSIGNED_BYTE,buf);
			for(int i=0; i<h;++i) for (int j=0; j<w;++j)
			{
				if ((ibuf[i*w+j]&0x0ffffff)!=blank)
				{++n; mins.min(Tp(0,0,i,j)); maxes.max(Tp(i,j,0,0));}
			}
			mins[0]=maxes[0]; mins[1]=maxes[1];
			m_store.store(m_lut[nm]+"N",n);
			m_store.store(m_lut[nm]+"p",float(n)/(w*h));
			m_store.store(m_lut[nm],mins);
			delete[] buf;	
}
void MyClass::Configure(config_type & cf, StParse& p, const string_type nm)
	{
		if (p.str().find("?")!=string_type::npos)
		{
			std::cout<<" bounds,percent,zstats\n";
			}
	// hard coded defaults.
	string_type hard_coded_defaults= 
		"target=1 start=0,0,0 ds=.2 dy=1 points_i=100 points_j=100 imax=200";
	// default key to look in config for config defaults
	string_type default_key="screenkey";
	// over ride in order: hard coded, config file key, config values, user input
	typedef greps::eqn_subs util_class;
	string_type resultant=util_class::netMap(cf,p,hard_coded_defaults,default_key);	
	cf.unload(nm); cf.load(nm+" "+resultant);
	// The selection criteria names can't conflict with function names...
//	m_sel.select_fields(cf,nm);
	
	AddCmd(cf,nm,"bounds");
	AddCmd(cf,nm,"percent");
	AddCmd(cf,nm,"zstats");
	
	return ;
}
