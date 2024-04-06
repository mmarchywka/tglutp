#include <stdlib.h>
#include <stdio.h>
#include <math.h>
typedef double mytype;

template <int _NPROGS ,int _MPROGS > class patient
{
	enum {NPROGS=_NPROGS, MPROGS=_MPROGS};
	typedef patient Myt;
public:
	enum {UNKNOWN,PLACEBO,PROVENGE,XOVER};	
	enum {TAX,NOTAX};
	typedef float float_type;
	typedef int count_type;
	count_type m_group,m_confound;
	float_type progs[NPROGS];
	float_type results[MPROGS];
 patient() :m_group(UNKNOWN),m_confound(UNKNOWN) {for ( count_type i=0; i<NPROGS; ++i) progs[i]=0;  
 	for ( count_type i=0; i<MPROGS; ++i) results[i]=0;  
  	}
	
	
};

const int NPAT=50;
typedef patient<5,2> paTy;

paTy samples[NPAT];
//mytype x,y,vx,vy; mytype E,L; mytype t,dt;

mytype expP(mytype t, mytype M) { static mytype kxxx=log(2); return (exp(-kxxx*t/M)); }
mytype lnP(mytype t, mytype M) {return 1.0/(1+t/M*t/M); }
mytype linP(mytype t, mytype M) { mytype off=4.5; if (t<off) return 1; t=t-off; M=M-off;
	mytype x=1-t/M/2; if (x<0) x=0; return x; }

mytype (*f1)(mytype , mytype) =linP;
typedef double randTy;
randTy myrand(int seed)
{ return randTy(rand())/RAND_MAX; }


mytype line1(int p)
{ return (p*1.1-p*p/1000)*(1+myrand(0)*.1); }
mytype line2(int p)
{ return (p*1+400)*(1+myrand(0)*.1); }
mytype (*funcptr1)(int) =line1;
mytype (*funcptr2)(int) =line2;


void populate()
{
for (int i=0; i<NPAT; ++i)
{
	float p1=(float)myrand(0);
	float p2=(float)myrand(0);
	samples[i].progs[0]=p1; samples[i].progs[1]=p2;
	float model=p1*20+log(p2+1)*30;
	int drug=myrand(0)>.5?1:0;
	samples[i].m_group=drug;
	samples[i].results[0]=model+myrand(0)*3+drug*5*(1+.5*myrand(0)); 
	
	}	
	
	
}
void dump_patients()
{
	for (int i=0; i<NPAT; ++i)
{
	paTy& tp=samples[i];
	printf("%f %f %f\n",tp.progs[0],tp.progs[1],tp.results[0]);
	
	}
	
}
void makestream()
{
 int leng=1000;
	mytype wavelets[leng],waveletsf[leng],data[leng],sort1[leng],sort2[leng];
	int loc1[leng],loc2[leng];
	mytype cutoff=1-.13;
for ( int i=0; i<leng; i++)
{	
	
mytype x=	( myrand(0) < cutoff) ? funcptr1(i): funcptr2(i);
//printf ("%d %g\n", i,x);
data[i]=x; 
}
//for ( int i=300; i<400; i++) {	 mytype x=	( i&1) ? funcptr1(i): funcptr2(i); data[i]=x;  }
//for ( int i=400; i<500; i++) {	 mytype x=	( i&2) ? funcptr1(i): funcptr2(i); data[i]=x;  }
//for ( int i=500; i<600; i++) {	 mytype x=	( i&4) ? funcptr1(i): funcptr2(i); data[i]=x;  }
	
//return; 
mytype filter[9]; filter[0]=.0331456; filter[8]=filter[0]; filter[1]=.06629; filter[7]=filter[1];
filter [2]=-0.17677; filter[6]=filter[2];  filter[3]=-0.419844; filter[5]=filter[3]; filter[4]=0.994368;
int c1=0; int c2=0;
int state=-1;
mytype result=0; 
for (int i=0; i<leng; i++) { wavelets[i]=0; waveletsf[i]=0;}
for (int i=4; i<leng-4; i++)
{
	result=0; 
for ( int j=-4; j<5; j++) {result+=data[i+j]*filter[j+4]; }
wavelets[i]=waveletsf[i]=result;
//printf ("%d %g\n",i,result);


}
mytype del=.8; mytype delf=10*10;
//for (int i=1; i<leng-1; i++) { result=wavelets[i]*wavelets[i];   
//	if ((result<wavelets[i-1]*wavelets[i-1]*del)||(result<wavelets[i+1]*wavelets[i+1]*del)) {waveletsf[i]=0;}   }
//	if ((wavelets[i-1]*wavelets[i-1]<delf)||(wavelets[i+1]*wavelets[i+1]<delf)) {waveletsf[i]=0;}   }
	
for (int i=0; i<leng; i++) {
result=waveletsf[i];	
printf ("%d %g\n",i,result);
bool rs=(state==0);
bool ut=(rs?(result>250):(result>300));
bool lt=(rs?(result<-460):(result<-30));
if ( ut) state=1; if ( lt) state=0;
//if ( result>300) {state=1;}
//if ( result<-160) { state=0;} 
if (state==0) { loc1[c1]=i; sort1[c1]=data[i]; c1++;}
if (state==1) { loc2[c2]=i; sort2[c2]=data[i]; c2++;}	
	
}


printf("\n");
//return ; 
//printf("color=0\n");
for (int k=0; k<c1; k++ ) { printf ("%d %g\n",loc1[k],sort1[k]); }
//printf("NEXT\ncolor=2\n");
printf("\n");
for (int k=0; k<c2; k++ ) { printf ("%d %g\n",loc2[k],sort2[k]); }

}
void makexxxstream()
{
long bins[1001],mbins[1001];
long xbins[1001],xmbins[1001];
long i,j,k;
i=0;
for (j=0; j<1000; j++) {bins[j]=0; xbins[j]=0; mbins[j]=0; xmbins[j]=0; }
while (i<1000000)
{
j=0;
while (j<1000)
{
//printf("%g\n",myrand(0));
int samples[3];
int summ=0;
for (k=0; k<3; k++){ samples[k]=int(myrand(0)*1000); summ+=samples[k]; }
summ/=3;
mbins[summ]++;
for (k=0; k<1; k++) 
{ 
int mmin=0; int mmax=0;
while (samples[mmin]==-1) mmin++;
mmax=mmin; 
for ( int kk=0; kk<3;kk++) {if (samples[kk]!=-1) if (samples[kk]>=samples[mmax] ) {mmax=kk;}
if (samples[kk]!=-1) if (samples[kk]<=samples[mmin]) {mmin=kk;}
}
samples[mmin]=-1; samples[mmax]=-1;
  	
}
int found=0; 
for (k=0; k<3; ++k) {if (samples[k]!=-1){found++; bins[samples[k]]++;} }

if (found!=1) { fprintf(stderr,"fond=%d\n",found); return; }
//bins[int(myrand(0)*1000)]++;

j++;
}
printf("color=0\n");
for (k=0; k<1000; k++ ) { printf ("%d %ld\n",int(k),xbins[k]); }
printf("color=0\n");
for (k=0; k<1000; k++ ) { printf ("%d %ld\n",int(k),xmbins[k]); }
printf("NEXT\ncolor=2\n");
for (k=0; k<1000; k++ ) { printf ("%d %ld\n",int(k),bins[k]); xbins[k]=bins[k]; }
printf("NEXT\ncolor=3\n");
for (k=0; k<1000; k++ ) { printf ("%d %ld\n",int(k),mbins[k]); xmbins[k]=mbins[k]; }
printf("NEXT\n");

i++;

}


}

int main(int argc, char **argv) 
{
	int x=1;
switch (x)
{
case 0: {makestream(); break;}
case 1: { populate(); dump_patients(); break; }
};
return 0;
//printf("%g %g \n",log(2),log(exp(1)));
printf("%g %g\n",atof(argv[1]),atof(argv[2]));
//regu();
//update();
return 0;
}
