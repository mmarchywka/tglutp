#ifndef POINTS_BASE_CLASSES_H__
#define POINTS_BASE_CLASSES_H__
#include "../data_and_config.h"

// 2008-03-18 not sure why ::glrotatef was missing before? 
#include <GL/glut.h>

// in have conversions to point3 from the old code...
// but this include seems to need mypoints to....
//#include "../view/camera_and_lights.h"
#include "../view/mathutil.h"

#include <iostream>
template <class CorTy > class point3_template
{

typedef point3_template Myt;
typedef CorTy coord_type;
typedef data_and_config DaCoTy;
typedef DaCoTy::string_type string_type;

public:
	point3_template() :m_x(0),m_y(0),m_z(0) {}
	point3_template(const coord_type x, const coord_type y,const coord_type z) 
	:m_x(x),m_y(y),m_z(z) {}
	point3_template(const Point3 & c) : m_x(c.x()),m_y(c.y()),m_z(c.z()) {}
coord_type x() const { return m_x; }
coord_type y() const { return m_y; }
coord_type z() const { return m_z; }
coord_type &x()  { return m_x; }
coord_type &y()  { return m_y; }
coord_type &z()  { return m_z; }
operator Point3() const {return Point3(m_x,m_y,m_z); }
operator string_type() const { 
	pr_stream_type pr("",ios_base::ate); pr<<"{"<<m_x<<" "<<m_y<<" "<<m_z<<"}";
	return pr.str();} 
coord_type&  operator[] (int idx) { return (idx==0)?m_x:(idx==1)?m_y:m_z;}
const coord_type&  operator[] (int idx) const { return (idx==0)?m_x:(idx==1)?m_y:m_z;}
//operator const coord_type* () const { return &m_x[0]; }
//operator  coord_type* ()  { return &m_x[0]; }

void glRotatef(const coord_type angle) const {::glRotatef(angle, x(),y(),z()); } 	
void do_twirl () const{  ::glRotatef(m_x,m_y,m_z,0); }
Myt z_twirl() const {
//glTranslatef(p[0].x(),p[0].y(),p[0].z());				
//		 coord_type m_nx=p[1].x(), m_ny=p[1].y(),m_nz=p[1].z();
//	coord_type m_mag=sqrt(m_nx*m_nx+m_ny*m_ny+m_nz*m_nz);
//	coord_type	m_curl_y=m_nx/m_mag, m_curl_x=-m_ny/m_mag;
//	coord_type 	m_dot_z=180/3.14159*acos(m_nz/m_mag);
//		 glRotatef(m_dot_z,m_curl_x,m_curl_y,0);
	coord_type m_mag=sqrt(m_x*m_x+m_y*m_y+m_z*m_z);
	coord_type	m_curl_y=m_x/m_mag, m_curl_x=-m_y/m_mag;
	coord_type 	m_dot_z=180/3.14159*acos(m_z/m_mag);
 return Myt(m_dot_z,m_curl_x,m_curl_y);
}

Myt & operator +=(const Myt & that) {  m_x+=that.m_x; m_y+=that.m_y; m_z+=that.m_z; return *this; }
Myt & operator -=(const Myt & that) {  m_x-=that.m_x; m_y-=that.m_y; m_z-=that.m_z; return *this; }

Myt  operator -(const Myt & that) const {Myt x;
  x.m_x=m_x-that.m_x; x.m_y=m_y-that.m_y; x.m_z=m_z-that.m_z; return x; }
  Myt  operator *(const coord_type & that) const {Myt x;
  x.m_x=m_x*that; x.m_y=m_y*that; x.m_z=m_z*that; return x; }
Myt  operator +(const Myt & that) const {Myt x;
  x.m_x=m_x+that.m_x; x.m_y=m_y+that.m_y; x.m_z=m_z+that.m_z; return x; }
Myt & operator /=(const coord_type  that) {  m_x/=that; m_y/=that; m_z/=that; return *this; }

Myt  operator /(const coord_type  that) const
{  Myt w; w.m_x=m_x/that; w.m_y=m_y/that; w.m_z=m_z/that; return w; }


Myt & operator /=(const Myt&  that) {  m_x/=that.m_x; m_y/=that.m_y; m_z/=that.m_z; return *this; }
coord_type length2() const { return (m_x*m_x+m_y*m_y+m_z*m_z); }
coord_type dot(const Myt& that) const { return (m_x*that.m_x+m_y*that.m_y+m_z*that.m_z); }
coord_type length() const { return sqrt(m_x*m_x+m_y*m_y+m_z*m_z); } 
Myt & normalize() {  coord_type l=length(); *(this)/=l;return *this; }
bool normalize_if() {  coord_type l=length2(); bool ok=(l!=0); if (ok) *(this)/=sqrt(l);
return ok;  }
operator bool() const { return m_x||m_y||m_z; }
template <class Ty, class Tx> Myt& cross( const Tx &x, const Ty &y)
{
Myt x1=(*this)-x, x2=(*this)-y;	
m_x=x1.m_y*x2.m_z-x2.m_y*x1.m_z;
m_y=x1.m_z*x2.m_x-x2.m_z*x1.m_x;
m_z=x1.m_x*x2.m_y-x1.m_y*x2.m_x;
	
return *this;	
}
Myt& max(const Myt& that)
{ if (that.m_x>m_x) m_x=that.m_x;     
if (that.m_y>m_y) m_y=that.m_y;   
if (that.m_z>m_z) m_z=that.m_z;   
return *this;
}
Myt& min(const Myt& that)
{ if (that.m_x<m_x) m_x=that.m_x;     
if (that.m_y<m_y) m_y=that.m_y;   
if (that.m_z<m_z) m_z=that.m_z;   
return *this;
}
template <class Tx> Myt cross( const Tx &x) const
{Myt y;
const Myt &x2=x,& x1=(*this);	
y.m_x=x1.m_y*x2.m_z-x2.m_y*x1.m_z;
y.m_y=x1.m_z*x2.m_x-x2.m_z*x1.m_x;
y.m_z=x1.m_x*x2.m_y-x1.m_y*x2.m_x;
	
return y;	
}


private:	
coord_type m_x,m_y,m_z;
	
};

#define MY_ITOR for(count_type i=0; i<N; ++i) 

typedef point3_template<float>  myPoint3;

template <class Ty, int N>
class point3_array 
{ 
	typedef point3_array Myt;
typedef point3_template<Ty> PtTy;
typedef int count_type;
public : PtTy& operator[](const int y) {return m_x[y];}
const PtTy& operator[](const int y) const {return m_x[y];}
	Myt& operator()(const PtTy & that, const int i) { m_x[i]=that; return *this;}
Myt& operator /=(const Ty x) {   
	MY_ITOR {m_x[i]/=x; } return *this;
	}
private:
	PtTy m_x[N];

};
template <class Ty, int N>
class my_scalar_array 
{ 
	typedef my_scalar_array Myt;
	typedef Ty PtTy;
	typedef int count_type;

public : 
	PtTy& operator[](const int y) {return m_x[y];}
	const PtTy& operator[](const int y) const {return m_x[y];}
	Myt& operator()(const PtTy & that, const int i) { m_x[i]=that; return *this;}
	Myt& operator=(const PtTy that) { MY_ITOR m_x[i]=that;  return *this; }
	
// I'm also using these for fake fracions, will need to move these sometimes;
	// 2008-03-18 this doesn't look like it ever was used, got wrong snap shot
	// probably nd is m_x? 
	void add_nd( const count_type i, const PtTy n, const PtTy d)
	{  //nd[i]=nd[i]*d+nd[i+1]; // this is 1/d	
		//	nd[i+1]*=d; // note this doesn't commute with above :)
			  
	}
private:
	PtTy m_x[N];


};
#undef MY_ITOR
#define MY_ITOR for(int i=0; i<N; ++i) 
template <class Ty,class Tx,int N> basic_ostream<Ty> & operator <<(basic_ostream<Ty>& is, 
my_scalar_array<Tx,N> & x)
{ is<<"{";  MY_ITOR is<<" "<<x[i]; is<<"}"; return is; } 
template <class Ty,int N> basic_ostream<Ty> & operator <<(basic_ostream<Ty>& is, 
my_scalar_array<int,N> & x)
{ is<<"{";  MY_ITOR is<<" "<<x[i]; is<<"}"; return is; } 
template <class Ty,class Tx,int N> basic_istream<Ty> & operator >>(basic_istream<Ty>& is, 
my_scalar_array<Tx,N> & x)
{ MY_ITOR is>>x[i]; return is; } 

#undef MY_ITOR

// compiler may not pick these as templates- need to check ordering...
/*
template <class Ty,class Td> basic_istream<Ty> & operator >>(basic_istream<Ty>& is, 
point3_template<Td> & x)
{ is>>x.x(); is>>x.y();is>>x.z(); return is; } 
template <class Ty,class Td> basic_ostream<Ty> & operator <<(basic_ostream<Ty>& is, 
point3_template<Td> & x)
{ is<<"{"<<x.x()<<" "<<x.y()<<" "<<x.z()<<"}"; return is; } 
*/
template <class Ty> void rm_punc( basic_istream<Ty> &is)
{  Ty c=' '; is.get(c); 
	while (((c=='{')||(c==',')||(c=='}'))&&(is.good()&&!is.eof())) is.get(c);
	is.putback(c);
}
template <class Ty> basic_istream<Ty> & operator >>(basic_istream<Ty>& is, 
point3_template<int> & x)
{
	float kluge=0;
	rm_punc(is); is>>kluge ; x.x()=int(kluge); 
	rm_punc(is);is>>kluge; x.y()=int(kluge);
	rm_punc(is);is>>kluge; x.z()=int(kluge); 
	return is; 
} 
template <class Ty> basic_ostream<Ty> & operator <<(basic_ostream<Ty>& is, 
point3_template<int> & x)
{ is<<"{"<<x.x()<<" "<<x.y()<<" "<<x.z()<<"}"; return is; } 
template <class Ty> basic_istream<Ty> & operator >>(basic_istream<Ty>& is, 
point3_template<float> & x)
{ rm_punc(is); is>>x.x();rm_punc(is); is>>x.y();rm_punc(is);is>>x.z(); return is; } 
//basic_istream<char> & operator >>(basic_istream<char>& is, 
//point3_template<float> & x)
//{ is>>x.x(); is>>x.y();is>>x.z(); return is; } 
template <class Ty> basic_ostream<Ty> & operator <<(basic_ostream<Ty>& is, 
const point3_template<float> & x)
{ is<<"{"<<x.x()<<" "<<x.y()<<" "<<x.z()<<"}"; return is; } 

template <class Ty> basic_istream<Ty> & operator >>(basic_istream<Ty>& is, 
point3_array<float,2> & x)
{ is>>x[0]; is>>x[1]; return is; } 
template <class Ty> basic_ostream<Ty> & operator <<(basic_ostream<Ty>& is, 
point3_array<float,2> & x)
{ is<<x[0]; is<<x[1]; return is; } 
template <class Ty> basic_istream<Ty> & operator >>(basic_istream<Ty>& is, 
point3_array<float,3> & x)
{ is>>x[0]; is>>x[1];is>>x[2]; return is; } 
template <class Ty> basic_ostream<Ty> & operator <<(basic_ostream<Ty>& is, 
const point3_array<float,3> & x)
{ is<<x[0]; is<<x[1]; is<<x[2];return is; } 

template <class DataTy> class myPoint4
{
typedef myPoint4 Myt;
typedef DataTy coord_type;
typedef data_and_config DaCoTy;
typedef DaCoTy::string_type string_type;
typedef int IndTy;
enum {SSIZE=4};
#undef MY_ITOR
#define MY_ITOR for (IndTy i=0; i<SSIZE; ++i)
public:
	myPoint4()  {MY_ITOR  m_x[i]=0;} 
//	myPoint4(const coord_type * y)  {MY_ITOR { m_x[i]=*(y+i);}} 
//	myPoint4(const coord_type y[4])  {MY_ITOR { m_x[i]=y[i];}} 
	myPoint4(const coord_type x, const coord_type y,const coord_type z,const coord_type w) 
	 {m_x[0]=x;m_x[1]=y;m_x[2]=z;m_x[3]=w;}
	
//myPoint4(const coord_type y[4])  {for (int i=0; i<4; ++i) m_x[i]=y[i];} 
myPoint4(const float y[SSIZE])  {MY_ITOR m_x[i]=y[i];} 
myPoint4(const double y[SSIZE])  {MY_ITOR m_x[i]=y[i];} 
myPoint4(const int y[SSIZE])  {MY_ITOR m_x[i]=y[i];} 


Myt& max(const Myt& that)
{ MY_ITOR if (that.m_x[i]>m_x[i]) m_x[i]=that.m_x[i];     
return *this;
}
Myt& min(const Myt& that)
{ MY_ITOR if (that.m_x[i]<m_x[i]) m_x[i]=that.m_x[i];     
return *this;
}




myPoint4& operator=(const coord_type y[SSIZE])  {MY_ITOR m_x[i]=y[i]; return *this;} 
myPoint4& operator+=(const coord_type y[SSIZE])  {MY_ITOR m_x[i]+=y[i]; return *this;} 
coord_type&  operator[] (int idx) { 
	//assert((idx>=0)&&(idx<SSIZE));
	return m_x[idx];}
	
	coord_type  operator[] (int idx) const { 
	//assert((idx>=0)&&(idx<4));
	return m_x[idx];}
	
//void glRotatef(const coord_type angle) const {::glRotatef(angle, x(),y(),z()); } 	
operator string_type() const { 
	pr_stream_type pr("",ios_base::ate); pr<<"{"<<m_x[0]<<" "<<m_x[1]
	<<" "<<m_x[2]<<" "<<m_x[3]<<"}";
	return pr.str();}
template <class Ty> void load(Ty &  is) {  MY_ITOR is>>m_x[i];  } 
template <class Ty> void store(Ty &  os ){  MY_ITOR os<<" "<<m_x[i];  } 
bool operator==(const int x) const
	{ MY_ITOR if (m_x[i]!=x) return false; return true; }
bool operator!=(const int x) const
	{ MY_ITOR if (m_x[i]!=x) return true; return false; }
operator const bool() const { 
	// thisw worked in display with member 
	//std::cout<<" the rigt operator was called\n";
	return m_x[0]||m_x[1]||m_x[2]||m_x[3]; 
}

operator const coord_type* () const {
	
	//std::cout<<"wtf 1\n";
	 return &m_x[0]; }
operator  coord_type* ()  { 
	// this is what gets called for a bool test????
//	std::cout<<"wtf2 ";
	return &m_x[0]; }
//template <class Tx> operator myPoint4<Tx>() const { return myPoint4<Tx>(m_x); }
private:	
coord_type m_x[SSIZE];
	
};

#undef MY_ITOR




#endif

