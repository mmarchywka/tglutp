#ifndef NIST_UTIL_H__
#define NIST_UTIL_H__
// be sure to undef when done :)
#define MY_ASSERT(x,y,w,z) if(!(y)) {std::cout<<(z)<<" but have "<<(x)<<" vs "<<(w)<<"\n";}
#define MY_ITOR(b,c,d)  const count_type M__ = (b).dim1();const count_type K__ = (b).dim2();\
for (count_type (c)=0; (c)<M__; (c)++)for (count_type (d)=0; (d)<K__; (d)++)



template <class data_type> class NistUtil
{public:
	typedef TNT::Array2D<data_type> MatTy; // Could have an "N" specialization...
	typedef MatTy VectorTy; // 
	typedef JAMA::SVD<data_type> InverterTy;
	typedef int count_type;
static void Clear (MatTy & m) 
{ MY_ITOR(m,i,j) {m[i][j]=0; } }
static MatTy Transpose( const MatTy & m)
{	MatTy mt(m.dim2(),m.dim1()); 
MY_ITOR(m,i,j) {    // cout<<i<<" "<<j<<"\n";
    ;  mt[j][i]=m[i][j];}  return mt;}
static void Recip(  MatTy & m) 
{ MY_ITOR(m,i,j) {data_type& x= m[i][j]; if(x) x =data_type(1)/x;}}
static bool NearlyIdentity(MatTy & x,const data_type tol)
{const data_type tol2=tol*tol;  MY_ITOR(x,i,j) { data_type d=x[i][j]-((i==j)?1:0);  
	if(d*d>tol2 ) return false;   }  return true;    }

static data_type Trace(const MatTy & x)
{  data_type t=0; count_type i=(x.dim1()<x.dim2())?x.dim1():x.dim2(); 
while (i){ t+=x[i][i]; i--;} return t; } 


};



#endif
