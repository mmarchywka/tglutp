#ifndef MY_HYPERGEOMETRIC_H__
#define MY_HYPERGEOMETRIC_H__



class gamma_functions
{
	public:
typedef double NTy;
static NTy gamma(const NTy x) 
{ return 0;}
static NTy loggamma(const NTy x) 
{ return 0;}	
//http://en.wikipedia.org/wiki/Gamma_function
// slow to converge...
static NTy gamma_ratio(const NTy n,const NTy d) 
{ 
	NTy zz=n-d;
	const NTy g=0.577215664901532860606512090082402431042159335;
	NTy p =d/n*exp(-g*zz);
	NTy ni=1;
	NTy nilim=20000; //20*fabs(zz)+100;
	NTy term=p;
	while ((ni<nilim)&&(term>0.0001)) {
	term=(ni+d)/(ni+n)*exp(zz/ni);
	p*=term;
	ni+=1;
	}
	return p;


}	
//	http://mathworld.wolfram.com/GammaFunction.html
static NTy gamma_ratiox_half_int(const NTy n,const NTy d) 
{ // should be close enough that this holds true...
	int ni=int(n*2);
	int di=int(d*2);
	if ( n*2!=ni) {   return 0; }
	if ( d*2!=di) {   return 0; }
	NTy p=1;//sqrt(3.14159);
	NTy dn=(n-d)*2;
	NTy nx=2*n-2;
	NTy d2=::pow(2,n-d);
	p=p/d2;
	if (dn>0) 
	{
		while (nx>=d*2) { p*=nx; nx-=2; }
		if ((ni&1)==(di&1)) return p;
		// ni&1->
		while (nx>1) { p*=(nx)/(nx-1); nx-=2;}
	
	}
	return p;


}		
	//	http://mathworld.wolfram.com/GammaFunction.html
static NTy gamma_ratio_half_int(const int n,const int d) 
{ // should be close enough that this holds true...
	int ni=n&1;
	int di=(d&1)<<1;
	int mag=(n>d)?8:0;
	NTy pis=sqrt(3.14159);
	NTy p=1;
	switch (ni+di+mag)
	{
		// gamma and factorial differe by one- doh!
	case 0: {ni=n>>1; di=d>>1;   while (di!=ni) p*=(++ni); p=1.0/p;  break;}	
	case 8: {ni=n>>1; di=d>>1;   while (di!=ni) p*=(++di);   break;}
	// n odd and > even d	
	case 9: {int ni=n>>1; int di=d>>1; p=pis;  
		while (ni>0) {p*=(ni-.5); if (ni<di) p=p/ni; ni--; }
	 break;}	
		
		
		
	}
	return p;


}		
};

class hypergeometric_functions
{
public:
typedef double NTy;
//	http://en.wikipedia.org/wiki/Confluent_hypergeometric_function
static NTy series_1f1( const NTy a, const NTy b, const NTy z)
{

	NTy aa=a, bb=b,nf=1;	
	NTy sum=1;
	NTy n=1;
	NTy zz=1;
	NTy coef=aa/(bb*nf);
	while (1)
	{ 
	const NTy  term=coef*zz;
	sum+=term;
	if (fabs(term)<fabs(sum*.001)) break;
	if (n>10000) break;
	 zz*=z;
	//	aa*=(a+n); bb*=(b+n); //n+=1; 
//		nf*=(n+1);
		coef*=(a+n)/((b+n)*(n+1));
		
		n+=1;
	}
	return sum;
		
}
	static NTy series_2f1( const NTy a, const NTy b, const NTy c,const NTy z)
{

	NTy aa=a, bb=b,cc=c,nf=1;	
	NTy sum=1;
	NTy n=1;
	NTy zz=z;
	NTy coef=aa*bb/(cc*nf);
	while (1)
	{ 
	const NTy  term=coef*zz;
	sum+=term;
	if (fabs(term)<fabs(sum*.000001)) break;
	if (n>10000) break;
	 zz*=z;
		coef*=(a+n)*(b+n)/((c+n)*(n+1));
		
		n+=1;
	}
	return sum;
		
}
	
};

class numeric_combos
{
	public:
typedef double NTy;
//http://en.wikipedia.org/wiki/Student%27s_t-distribution
static NTy student(const NTy x,const NTy nu)
{
	
	
return 	.5+(gamma_functions::gamma_ratio_half_int(int(nu+1),int(nu))
*x/sqrt(nu*3.14159)*hypergeometric_functions::series_2f1(.5,(nu+1)/2,1.5,-x*x/nu)
);
	
	}
	
	
};

#endif
