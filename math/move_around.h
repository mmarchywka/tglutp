#ifndef MOVE_AROUND_H__
#define MOVE_AROUND_H__
// This is repeated code from the pdb_proc utility etc.
// but the signauter have been changed etc.
#include "../base/points_base_classes.h"


// yes, this is dupilicated code...
#include <math.h>
//namespace {


class move_around
{//http://en.wikipedia.org/wiki/Euler_angles
	typedef move_around Myt; 
	typedef double Ty;
	typedef point3_template<Ty> PointTy;
public:
// float double, kept calling default ctor since it could convert, WTF
template < class Tx> move_around(const point3_template<Tx> & x, const point3_template<Tx>  & e)  
:xo(x.x()),yo(x.y()),zo(x.z())
{
	const Ty c1=::cos(e.x());	const Ty c2=::cos(e.y());	const Ty c3=::cos(e.z());
	const Ty s1=::sin(e.x());	const Ty s2=::sin(e.y());	const Ty s3=::sin(e.z());
	//std::cout<<"making the readl one, "<<c1<<" "<<s1<<'\n';
	// zyx
	t00=c1*c2; t01=-c2*s1; t02=s2;
	t10=c3*s1+c1*s2*s3; t11=c1*c3-s1*s2*s3; t12=-c2*s3;
	t20=s1*s3-c1*c3*s2; t21=c3*s1*s2+c1*s3; t22=c2*c3;
	shake=0;
}

// this may be confusing compiler with above, not sure 
template < class Tyy > void operator() (Tyy & v, 	const Tyy & s) const
{ 
	const Ty ex= s.x()-xo; const Ty ey=s.y()-yo; const Ty ez=s.z()-zo;
	v=s;
	const Ty arr=shake;//.2;
	const Ty fx= t00*ex+t01*ey+t02*ez+xo+arr*(rand());///RAND_MAX; 
	const Ty fy= t10*ex+t11*ey+t12*ez+yo+arr*(rand());///RAND_MAX; 
	const Ty fz= t20*ex+t21*ey+t22*ez+zo+arr*(rand());///RAND_MAX;
	v.move_to(fx,fy,fz);

}




move_around(const Ty & x=0,const Ty & y=0, const Ty & z=0,
const Ty & a=0, const Ty & b=0, const Ty & c=0)  :xo(x),yo(y),zo(z)
{ //std::cout<<" in default ctro\n";

	const Ty c1=::cos(a);	const Ty c2=::cos(b);	const Ty c3=::cos(c);
	const Ty s1=::sin(a);	const Ty s2=::sin(b);	const Ty s3=::sin(c);
	// zyx
	t00=c1*c2; t01=-c2*s1; t02=s2;
	t10=c3*s1+c1*s2*s3; t11=c1*c3-s1*s2*s3; t12=-c2*s3;
	t20=s1*s3-c1*c3*s2; t21=c3*s1*s2+c1*s3; t22=c2*c3;
	shake=0;
}

// rotate things so that vector Rxn= is zero. That is, if you want an orientation
// with R.x=0, this will find R(cross)x and rotate by -sin(theta) around the
// axes which should make R and X conincide.
template < class Tx> move_around(const point3_template<Tx> & x, const point3_template<Tx>  & r,
	const point3_template<Tx> & n )  
:xo(x.x()),yo(x.y()),zo(x.z())
{	typedef point3_template<Tx> Pt;
	Pt junk;
	Pt cp=junk.cross(r,n); // this signature is dum as it uses p1xp2 and could be static. 
	const Ty cplen=cp.length();
	const Ty rl=r.length();
	if ((rl==0)||(cp==0)) { (*this)=Myt();      return; };
	const Ty rsign=r.dot(n);
	// this is positive -> 0<theta<pi/2
	Ty theta=asin(cplen/rl);
	if (rsign<0) theta=(Myt::pi())-theta;
	std::cout<<__FILE__<<__LINE__<<" rsign="<<rsign<<" theta is now "<<theta<<'\n';
	// This is a bit wasteful as it has to recalculate a bunch of stuff
	// but this whole thing of putting resuable code in a ctor is dumb already... 
	Pt cpn=cp.normalize();
	(*this)=Myt(x,cpn,theta);
}

//http://en.wikipedia.org/wiki/Rotation_matrix
// This is a rotation around n by amount theta
//NOT TESTED for TYPOS etc. Seems to work now, used in above ctor ...

template < class Tx> move_around(const point3_template<Tx> & x, const point3_template<Tx>  & n,
	const Ty & theta )  
:xo(x.x()),yo(x.y()),zo(x.z())
{	// presumably the caller is quite capable of normalizing n, LOL
	const Ty c1=::cos(theta); const Ty s1=::sin(theta); const Ty c11=1.0f-c1;
	const Ty xx=n.x()*n.x()*c11; const Ty xy=n.x()*n.y()*c11; const Ty xz=n.x()*n.z()*c11;
	const Ty yy=n.y()*n.y()*c11; const Ty yz=n.y()*n.z()*c11; const Ty zz=n.z()*n.z()*c11;
	t00=xx+c1; t01=xy-n.z()*s1; t02=xz+n.y()*s1;
	t10=xy+n.z()*s1; t11=yy+c1; t12=yz-n.x()*s1;
	t20=xz-n.y()*s1; t21=yz+n.x()*s1; t22=zz+c1;
	shake=0;
	

}
// this is a single roation around one axis, should reduce from above along axes.
template < class Tx> move_around(const point3_template<Tx> & x, const Ty & theta, const int i)  
:xo(x.x()),yo(x.y()),zo(x.z())
{
	const Ty c1=::cos(theta);	//const Ty c2=::cos(e.y());	const Ty c3=::cos(e.z());
	const Ty s1=::sin(theta);	//const Ty s2=::sin(e.y());	const Ty s3=::sin(e.z());
	//std::cout<<"making the readl one, "<<c1<<" "<<s1<<'\n';
	if ( i==0) // x
	{
		t00=1; t01=0; t02=0;
		t10=0; t11=c1; t12=s1;
		t20=0; t21=-s1; t22=c1;
		
	}
	if ( i==1) // y
	{
		t00=c1; t01=0; t02=-s1;
		t10=0; t11=1; t12=0;
		t20=s1; t21=0; t22=c1;
		
	}
	if ( i==2) // z
	{
		t00=c1; t01=s1; t02=0;
		t10=-s1; t11=c1; t12=0;
		t20=0; t21=0; t22=1;
		
	}
	shake=0;
}


void set_shake(const Ty & s) { shake=s/RAND_MAX; } 

static const Ty & pi() { static const Ty p=acos(0)*2; return p; } 
static const Ty & ip() { static const Ty p=1.0/pi(); return p; }
static const Ty & degrad() { static const Ty p=180.0*ip(); return p; }  
// This converts an vector normal into phi and a rotation axis. 
template <class Tx > static void twirl(Tx & nx, Tx & ny, Tx & nz )
{
	//float m_nx=x[0], m_ny=x[1],m_nz=x[2];
   	if ( nx>0) {nx=-nx; ny=-ny; nz=-nz;};
   	const Tx mag=::sqrt(nx*nx+ny*ny+nz*nz);
   	if ( mag==0) { return;  }
  	const Tx phi=degrad()*acos(nz/mag); // this is target.Z;
 	// std::cout<<phi<<" "<<m_ny<<" "<<m_nx<<" "<<mag<<"\n";
  	//    if (phi!=0) {  glRotatef(phi,-m_ny/mag,m_nx/mag,0); }
  	//PointTy nor=PointTy(phi,-m_ny/mag,m_nx/mag);
	 ny=-ny/mag; nz=nx/mag; nx=phi;
}
// take phi and above axes and invesrt
template <class Tx > static void untwirl(Tx & nx, Tx & ny, Tx & nz )
{
	//float m_nx=x[0], m_ny=x[1],m_nz=x[2];
   	const Tx xnz=::cos(nx/degrad());
   	nx=nz; ny=-ny; nz=xnz;
}
template < class Tyy > void doit(Tyy & s) const
{ 
	const Ty ex= s.x()-xo; const Ty ey=s.y()-yo; const Ty ez=s.z()-zo;
	//v=s;
	const Ty arr=shake;//.2;
	const Ty fx= t00*ex+t01*ey+t02*ez+xo+arr*(rand());///RAND_MAX; 
	const Ty fy= t10*ex+t11*ey+t12*ez+yo+arr*(rand());///RAND_MAX; 
	const Ty fz= t20*ex+t21*ey+t22*ez+zo+arr*(rand());///RAND_MAX;
	//std::cout<<" doin moveeee "<<fx<<" vs "<<s.x()<<'\n';
	s=Tyy(fx,fy,fz);

}

private:
Ty shake;
Ty xo,yo,zo;
Ty t00,t01,t02,t10,t11,t12,t20,t21,t22;



};
//}; //namespace




#endif
