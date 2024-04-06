#ifndef STAT_CALC_CLASSES_H__
#define STAT_CALC_CLASSES_H__

#include "../debug_and_support.h"
///////////////////////////////////////////////////////////////////////////
// There are two sections here: 1, the calls to the distribution and function library
// and 2, the various statistic calculations. 
// two basic issue swith stats:
// 1) ties are not handled, 2)vector versions not supported
 
#include "dcdlib.h"

// cumulative density functions
class cdf
{
	typedef double F;
	typedef int I;
	typedef int api_type;
	enum {OK=0,ALL=(~0)};
	static std::ostream & eos() { return std::cerr; } 
public:
	// return confidence from statistic and single parameter, df
	static api_type chi_square(F& p, const F& x2, const I& dfi)
	{
		int which=1; // find p and q 
		double q=0;
		double x=x2;
		double df=dfi;
  		int status=0;
  		double bound=0;
  		dcdflib::cdfchi ( &which, &p, &q, &x,&df,&status, &bound );
  		if (status==0) return OK;
  		//    Output, int *STATUS, reports the status of the computation.
	//     0, if the calculation completed correctly;
		if (status<0) eos()<<MM_MARK<<" chi square failed, -I, if the input parameter number I is out of range\n";
		if (status==1) eos()<<MM_MARK<<"+1, if the answer appears to be lower than lowest search bound\n";
		if (status==2) eos()<<MM_MARK<<"+2, if the answer appears to be higher than greatest search bound\n";
		if (status==3) eos()<<MM_MARK<<"+3, if P + Q /= 1\n";
		if (status==10) eos()<<MM_MARK<<" +10, an error was returned from CUMGAM.\n";
		return ALL;
		
	}
	static api_type normal(F& p, const F& x2, const F& mean, const F& sd)
	{
		int which=1; double q=0;
		double x=x2; double meanx=mean,sdx=sd;
  		int status=0; double bound=0;
		dcdflib::cdfnor ( &which, &p, &q, &x, &meanx, &sdx, &status, &bound );
		if (status==0) return OK;
		eos()<<MM_MARK<<" Problem with normal params mean="<<mean<<" sd="<<sd<<" x="<<x<<'\n';
	}
	
};


//////////////////////////////////////////////////////////////////////////
// Stat calculations

class stat_params 
{public : stat_params(int e):events(e) {} int events;};

class stat_base_class: public stats_typedefs
{
	public:
	stat_base_class() : m_result(0) {}
	virtual bool run(GrTy & gr,const stat_params & sp, it_stats& it) =0; 
	virtual const Chr * name() const { return "stat_base_class:something wrong!";}
	static void about(OsTy & os) {
		os<<Ldr(2)<<"Ref:http://www.itl.nist.gov/div898/software/dataplot/refman1/auxillar/ranksum.htm" <<Trl();
		os<<Ldr(2)<<"Ref:http://www.okstate.edu/sas/v7/sashtml/books/stat/chap34/sect13.htm" <<Trl();
		os<<Ldr(2)<<"Ref:http://www.stat.auckland.ac.nz/~wild/ChanceEnc/Ch10.wilcoxon.pdf" <<Trl();
		os<<Ldr(2)<<"Ref:http://www-stat.stanford.edu/~susan/courses/s141/hononpara.pdf" <<Trl();
		os<<Ldr(2)<<"Ref:http://www.pubmedcentral.nih.gov/articlerender.fcgi?artid=1065034"<<Trl();
		os<<Ldr(2)<<"Ref:http://www.nature.com/bmt/journal/v28/n10/full/1703260a.html"<<Trl();
		os<<Ldr(2)<<"Ref:http://www.sta.cuhk.edu.hk/pschan/Courses/STA4008/lecture/set3.pdf"<<Trl();
		}
	const TiTy & result() const { return m_result; } 
	protected:
	void set(const TiTy & r) { m_result=r; }
	TiTy m_result;	
};
// afaik, this metric works.
class area_stat: public stat_base_class
{	// test statistic is the area between curves, normalized to the average
// area. I think this is just equivalent to (a1-a2)/(a1+a2)*2 but implemented
// as a separate integration ( was looking at powers of this)...
public:
	area_stat(gen_param & gp1, gen_param & gp2, const randTy lim)
		:m_gp1(gp1),m_gp2(gp2),m_ok_limit(lim) {}
	const Chr * name() const { return "area_stat";}
	bool run(GrTy &g , const stat_params & sp,it_stats & it) 
	{
		const IdxTy n1=m_gp1.n(),n2=m_gp2.n();
		TiTy last=0; IdxTy j1=n1,j2=n2;
		const TiTy d1=1.0/n1, d2=1.0/n2;
		TiTy a1=0,a2=0,a3=0,dx1=1,dx2=1;
		IdxTy nlim=n1+n2;
		IdxTy m_events=sp.events;
		if ((nlim>m_events)&&(m_events!=0)) nlim=m_events;
		for (IdxTy i=0; i<nlim; ++i)
		{
			TiTy d=g[i].time();
			TiTy dt=d-last;  a1+=dt*dx1; a2+=dt*dx2; a3+=dt*(dx1-dx2); last=d;
			if ( g[i].gr()==1)  { --j1; dx1-=d1;}
			else {  --j2; dx2-=d2;}
		//	dos()<<" x"<<j1<<" "<<j2<<'\n';	
		}
		
		TiTy a4=a3*2/(a1+a2)*1000;
		set(a4);
		if (a4>m_ok_limit) ++it.ok;
		if (it.interval==it.modi) 
		{it.modi=0; dos()<<a1<<" "<<a2<<" "<<a3<<" "<<a4<<" "<<(1.0f*it.ok/it.total) <<'\n'; }
		else ++it.modi;
		return true; 
	}

private:
gen_param m_gp1,m_gp2;
//	IdxTy m_events;
	randTy m_ok_limit;
};

////////////////////////////////////////////
// NOTE: NONE OF THESE NON_PARAMETRIC TEST IS KNOWN TO BE RIGHT
//    THEY ARE A COLLECTION OF THINGS IN PROCESS.
// http://www.itl.nist.gov/div898/software/dataplot/refman1/auxillar/ranksum.htm
/*

class log_rank_stat: public stat_base_class
{//http://www.ms.uky.edu/~mai/research/LogRank2006.pdf
public:
	log_rank_stat(gen_param & gp1, gen_param & gp2, const randTy lim)
		:m_gp1(gp1),m_gp2(gp2),m_ok_limit(lim) {}
	const Chr * name() const { return "log_rank_stat";}
	bool run(GrTy &g ,const stat_params & sp, it_stats & it) 
	{
		const IdxTy n1=m_gp1.n(),n2=m_gp2.n();
		//TiTy last=0; IdxTy j1=n1,j2=n2;
		//const TiTy d1=1.0/n1, d2=1.0/n2;
		//TiTy a1=0,a2=0,a3=0,dx1=1,dx2=1;
		const IdxTy m_events=sp.events;
		IdxTy nlim=n1+n2;
		if ((nlim>m_events)&&(m_events!=0)) nlim=m_events;
		TiTy cum_pay[3];
		cum_pay[2]=cum_pay[1]=0;
		for (IdxTy i=0; i<nlim; ++i)
		{
			GrTy::value_type & pp=g[i];
			//TiTy d=pp.second;
			cum_pay[pp.gr()]+=1.0/(nlim-i);
		//	dos()<<" x"<<j1<<" "<<j2<<'\n';	
	}
		// in this case, higher rank means earlier death and this is bad,
		// assume group 1 is drug, 2 placebo 
		TiTy a4=cum_pay[2];
		set(a4); 
		if (a4>m_ok_limit) ++it.ok;
		if (it.interval==it.modi) 
		{it.modi=0; dos()<<a4<<" "<<(1.0f*it.ok/it.total) <<'\n'; }
		else ++it.modi;
		return true; 
	}

private:
gen_param m_gp1,m_gp2;
	//IdxTy m_events;
	randTy m_ok_limit;
};
class wilcoxon_rank_sum_stat: public stat_base_class
{//http://www.stat.auckland.ac.nz/~wild/ChanceEnc/Ch10.wilcoxon.pdf
//http://www-stat.stanford.edu/~susan/courses/s141/hononpara.pdf
//http://www.itl.nist.gov/div898/software/dataplot/refman1/auxillar/ranksum.htm
public:
	wilcoxon_rank_sum_stat(gen_param & gp1, gen_param & gp2, const randTy lim)
		:m_gp1(gp1),m_gp2(gp2),m_ok_limit(lim) {}
	
	bool run(GrTy &g ,const stat_params & sp, it_stats & it) 
	{
		const IdxTy n1=m_gp1.n(),n2=m_gp2.n();
		const IdxTy m_events=sp.events;
		IdxTy nlim=n1+n2;
		if ((nlim>m_events)&&(m_events!=0)) nlim=m_events;
		IdxTy cum_pay[3];
		cum_pay[2]=cum_pay[1]=0;
		int tot_at_risk[3]; tot_at_risk[1]=n1; tot_at_risk[2]=n2;
		for (IdxTy i=0; i<nlim; ++i)
		{
			GrTy::value_type & pp=g[i];
			const IdxTy gn=pp.gr();
			cum_pay[gn]+=i+1;
		//	dos()<<" x"<<j1<<" "<<j2<<'\n';	
	}
		// assume group 1 is drug, 2 placebo 
		// You need a table for small N and unclear on censoring approach
		// this still isn't right, the <e> is substracted in pieces- wtf? 
		TiTy e= n1*n2/2.0, sd=sqrt(n1*n2*(n1+n2+1)/12);
		TiTy z=(cum_pay[1]-e)/sd;
		TiTy p=0;
		cdf::normal(p,z,0,1);
		//TiTy a4=cum_pay[2]-cum_pay[1];
		set(z); 
		if (z>m_ok_limit) ++it.ok;
		if (it.interval==it.modi) 
			{it.modi=0; dos()<<z<<" at p="<<p <<" frac="<<(1.0f*it.ok/it.total) <<'\n'; }
		else ++it.modi;
		return true; 
	}

private:
	gen_param m_gp1,m_gp2;
	randTy m_ok_limit;
};

*/
class log_rank_v_stat: public stat_base_class
{	// http://www.sta.cuhk.edu.hk/pschan/Courses/STA4008/lecture/set3.pdf
public:
	const Chr * name() const { return "log_rank_v_stat";}
	log_rank_v_stat(gen_param & gp1, gen_param & gp2, const randTy lim,const randTy p )
		:m_gp1(gp1),m_gp2(gp2),m_ok_limit(lim),m_p(p) {}
	
	bool run(GrTy &g ,const stat_params & sp, it_stats & it) 
	{
		const IdxTy n1=m_gp1.n(),n2=m_gp2.n();
		const IdxTy m_events=sp.events;
		IdxTy nlim=n1+n2;
		if ((nlim>m_events)&&(m_events!=0)) nlim=m_events;
		TiTy cum_pay[3];
		cum_pay[2]=cum_pay[1]=0;
		IdxTy nx[3]; nx[2]=nx[1]=0;
		TiTy var=0,vj=0;; 
		// d is failures at time t, n are alive prior to t
		for (IdxTy i=0; i<nlim; ++i)
		{
			GrTy::value_type & pp=g[i];
			const IdxTy gn=pp.gr();
			nx[gn]++;
		}
	//	nx[1]=n1; nx[2]=n2;
	//	IdxTy nn1=nx[1], nn2=nx[2];
	//	nx[1]=n1; nx[2]=n2;
		for (IdxTy i=0; i<nlim; ++i)
		{
			GrTy::value_type & pp=g[i];
			TiTy nj=(nlim-i);
			const IdxTy gn=pp.gr();
			//if (nj!=1)
			// d==1, otherwise we have (nj-d)/(nj-1)
			//if (gn==1)
			var+=1.0*nx[1]*(nj-nx[1])/nj/nj;
			if (gn==1)vj+=1.0-1.0f*nx[1]/nj;
			else vj+=0-1.0f*nx[1]/nj;;
			//cum_pay[1]+=1.0*nx[1]/(nlim-i);
			//cum_pay[2]+=1.0*nx[2]/(nlim-i);
			nx[gn]--;
		//	dos()<<" x"<<j1<<" "<<j2<<'\n';	
	}
		TiTy zraw=vj/var*vj;
		TiTy z= sqrt(zraw*zraw);
		TiTy p=0;
		set(z); 
		cdf::chi_square(p,z,1);
		//TiTy a4=cum_pay[2]-cum_pay[1];
		if (p>m_p) ++it.p;
		if (z>m_ok_limit) ++it.ok;
		if (it.interval==it.modi) 
			{it.modi=0; dos()<<z<<" at p="<<p <<" frac="<<(1.0f*it.ok/it.total)
			<<" "<<(1.0f*it.p/it.total) <<'\n'; }
		else ++it.modi;
		return true; 
	}

private:
	gen_param m_gp1,m_gp2;
	randTy m_ok_limit,m_p;
};
class wilcoxon_v_stat: public stat_base_class
{	// http://www.sta.cuhk.edu.hk/pschan/Courses/STA4008/lecture/set3.pdf
public:
	const Chr * name() const { return "wilcoxon_v_stat";}
	wilcoxon_v_stat(gen_param & gp1, gen_param & gp2, const randTy lim,const randTy p )
		:m_gp1(gp1),m_gp2(gp2),m_ok_limit(lim),m_p(p) {}
	
	bool run(GrTy &g ,const stat_params & sp, it_stats & it) 
	{
		const IdxTy n1=m_gp1.n(),n2=m_gp2.n();
		const IdxTy m_events=sp.events;
		IdxTy nlim=n1+n2;
		if ((nlim>m_events)&&(m_events!=0)) nlim=m_events;
		TiTy cum_pay[3];
		cum_pay[2]=cum_pay[1]=0;
		IdxTy nx[3]; nx[2]=nx[1]=0;
		TiTy var=0,vj=0;; 
		// d is failures at time t, n are alive prior to t
		for (IdxTy i=0; i<nlim; ++i)
		{
			GrTy::value_type & pp=g[i];
			const IdxTy gn=pp.gr();
			nx[gn]++;
		}
	//	nx[1]=n1; nx[2]=n2;
	//	IdxTy nn1=nx[1], nn2=nx[2];
	//	nx[1]=n1; nx[2]=n2;
		for (IdxTy i=0; i<nlim; ++i)
		{
			GrTy::value_type & pp=g[i];
			TiTy nj=(nlim-i);
			const IdxTy gn=pp.gr();
			//if (nj!=1)
			// d==1, otherwise we have (nj-d)/(nj-1)
			//if (gn==1)
			var+=1.0*nx[1]*(nj-nx[1]);///nj/nj;
			if (gn==1)vj+=(1.0-1.0f*nx[1]/nj)*nj;
			else vj+=(0-1.0f*nx[1]/nj)*nj;;
			//cum_pay[1]+=1.0*nx[1]/(nlim-i);
			//cum_pay[2]+=1.0*nx[2]/(nlim-i);
			nx[gn]--;
		//	dos()<<" x"<<j1<<" "<<j2<<'\n';	
	}
		TiTy zraw=vj/var*vj;
		TiTy z= sqrt(zraw*zraw);
		TiTy p=0;
		set(z); 
		cdf::chi_square(p,z,1);
		//TiTy a4=cum_pay[2]-cum_pay[1];
		if (p>m_p) ++it.p;
		if (z>m_ok_limit) ++it.ok;
		if (it.interval==it.modi) 
			{it.modi=0; dos()<<z<<" at p="<<p <<" frac="<<(1.0f*it.ok/it.total)
			<<" "<<(1.0f*it.p/it.total) <<'\n'; }
		else ++it.modi;
		return true; 
	}

private:
	gen_param m_gp1,m_gp2;
	randTy m_ok_limit,m_p;
};
class log_rank_stat: public stat_base_class
{	// http://www.pubmedcentral.nih.gov/articlerender.fcgi?artid=1065034
	// seems to work in uncensored cae, 
public:
	const Chr * name() const { return "log_rank_stat";}
	log_rank_stat(gen_param & gp1, gen_param & gp2, const randTy lim,const randTy p )
		:m_gp1(gp1),m_gp2(gp2),m_ok_limit(lim),m_p(p) {}
	
	bool run(GrTy &g ,const stat_params & sp, it_stats & it) 
	{
		const IdxTy n1=m_gp1.n(),n2=m_gp2.n();
		const IdxTy m_events=sp.events;
		IdxTy nlim=n1+n2;
		if ((nlim>m_events)&&(m_events!=0)) nlim=m_events;
		TiTy cum_pay[3];
		cum_pay[2]=cum_pay[1]=0;
		IdxTy nx[3]; nx[2]=nx[1]=0;
		for (IdxTy i=0; i<nlim; ++i)
		{
			GrTy::value_type & pp=g[i];
			const IdxTy gn=pp.gr();
			nx[gn]++;
		}
	//	nx[1]=n1; nx[2]=n2;
		IdxTy nn1=nx[1], nn2=nx[2];
	//	nx[1]=n1; nx[2]=n2;
		for (IdxTy i=0; i<nlim; ++i)
		{
			GrTy::value_type & pp=g[i];
			const IdxTy gn=pp.gr();
			cum_pay[1]+=1.0*nx[1]/(nlim-i);
			cum_pay[2]+=1.0*nx[2]/(nlim-i);
			nx[gn]--;
		//	dos()<<" x"<<j1<<" "<<j2<<'\n';	
	}
		TiTy zraw=(nn1-cum_pay[1])*(nn1-cum_pay[1])/cum_pay[1];
		zraw+=(nn2-cum_pay[2])*(nn2-cum_pay[2])/cum_pay[2];
		TiTy z=zraw;
		TiTy p=0;
		set(z); 
		cdf::chi_square(p,z,1);
		//TiTy a4=cum_pay[2]-cum_pay[1];
		if (p>m_p) ++it.p;
		if (z>m_ok_limit) ++it.ok;
		if (it.interval==it.modi) 
			{it.modi=0; dos()<<z<<" at p="<<p <<" frac="<<(1.0f*it.ok/it.total)
			<<" "<<(1.0f*it.p/it.total) <<'\n'; }
		else ++it.modi;
		return true; 
	}

private:
	gen_param m_gp1,m_gp2;
	randTy m_ok_limit,m_p;
};
class rank_sum_stat: public stat_base_class
{	// note: this doesn't censor well but seems to work right now. 
	// tested at zero,.05, and .025.
	//http://en.wikipedia.org/wiki/Mann-Whitney_U
//http://www.itl.nist.gov/div898/software/dataplot/refman1/auxillar/ranksum.htm
// this nist presentation is clearer as the <e> isn't taken in pieces BUT
// the conditional logic english is not clear.
public:
	const Chr * name() const { return "rabk_sum_stat";}
	rank_sum_stat(gen_param & gp1, gen_param & gp2, const randTy lim,const randTy p )
		:m_gp1(gp1),m_gp2(gp2),m_ok_limit(lim),m_p(p) {}
	
	bool run(GrTy &g ,const stat_params & sp, it_stats & it) 
	{
		const IdxTy n1=m_gp1.n(),n2=m_gp2.n();
		const IdxTy m_events=sp.events;
		IdxTy nlim=n1+n2;
		if ((nlim>m_events)&&(m_events!=0)) nlim=m_events;
		IdxTy cum_pay[3];
		cum_pay[2]=cum_pay[1]=0;
		IdxTy nx[3]; nx[2]=nx[1]=0;
		int tot_at_risk[3]; tot_at_risk[1]=n1; tot_at_risk[2]=n2;
		for (IdxTy i=0; i<nlim; ++i)
		{
			GrTy::value_type & pp=g[i];
			const IdxTy gn=pp.gr();
			cum_pay[gn]+=i+1;
			nx[gn]++;
		//	dos()<<" x"<<j1<<" "<<j2<<'\n';	
	}
		// assume group 1 is drug, 2 placebo 
		// You need a table for small N and unclear on censoring approach
		/*
		IdxTy low=nx[1]; 
		TiTy T=cum_pay[1];
		if (cum_pay[2]<cum_pay[1]) T=cum_pay[2];
		if (nx[2]<nx[1]) T=cum_pay[2];
		if (nx[1]<nx[2]) T=cum_pay[1];
		*/
		//nx[1]=n1; nx[2]=n2;
		TiTy nn=.5f*nx[1]*nx[2];
		
		TiTy u1=cum_pay[1]*1.0f-.5f*nx[1]*(nx[1]+1);
	//	TiTy u2=cum_pay[2]*1.0f-.5f*nx[2]*(nx[2]+1);
	//	dos()<<nx[1]<<" "<<nx[2]<<" "<<u1<<" "<<u2<<" "<<'\n';
		//if (u2<u1) u1=u2;
		TiTy sd=sqrt(1.0f*nx[1]*nx[2]*(nx[1]+nx[2]+1)/12.0f);
		
		//TiTy e= n1*n2/2.0, sd=sqrt(n1*n2*(n1+n2+1)/12);
		TiTy z=(u1-nn)/sd;
		TiTy p=0;
		set(z); 
		cdf::normal(p,z,0,1);
		//TiTy a4=cum_pay[2]-cum_pay[1];
		if (p>m_p) ++it.p;
		if (z>m_ok_limit) ++it.ok;
		if (it.interval==it.modi) 
			{it.modi=0; dos()<<z<<" at p="<<p <<" frac="<<(1.0f*it.ok/it.total)
			<<" "<<(1.0f*it.p/it.total) <<'\n'; }
		else ++it.modi;
		return true; 
	}

private:
	gen_param m_gp1,m_gp2;
	randTy m_ok_limit,m_p;
};

class kaplan_meier_stat: public stat_base_class
{	// http://en.wikipedia.org/wiki/Kaplan-Meier_estimator
public:
	const Chr * name() const { return "kaplan_meier_stat";}
	kaplan_meier_stat(gen_param & gp1, gen_param & gp2, const randTy lim,const randTy p )
		:m_gp1(gp1),m_gp2(gp2),m_ok_limit(lim),m_p(p) {}
	
	bool run(GrTy &g ,const stat_params & sp, it_stats & it) 
	{
		const IdxTy n1=m_gp1.n(),n2=m_gp2.n();
		const IdxTy m_events=sp.events;
		IdxTy nlim=n1+n2;
		if ((nlim>m_events)&&(m_events!=0)) nlim=m_events;
		//TiTy cum_pay[3]; cum_pay[2]=cum_pay[1]=0;
		IdxTy nx[3]; nx[2]=nx[1]=0;
		TiTy var1=0,v1=1,var2=0,v2=1; 
		//const TiTy vc=v1;
		//nx[1]=n1; nx[2]=n2;
		for (IdxTy i=0; i<nlim; ++i)
		{
			GrTy::value_type & pp=g[i];
			const IdxTy gn=pp.gr();
			nx[gn]++;
	}
		for (IdxTy i=0; i<nlim; ++i)
		{
			GrTy::value_type & pp=g[i];
		//	TiTy nj=(nlim-i);
			const IdxTy gn=pp.gr();
			if (gn==1) if(nx[gn]>1){//if (v1!=vc)
			 var1+=1.0/(nx[gn]+1);  v1=v1*(nx[gn]-1)/nx[gn];   }
			if (gn==2) if(nx[gn]>1) {//if (v2!=vc) 
			var2+=1.0/(nx[gn]+1);  v2=v2*(nx[gn]-1)/nx[gn];   }
			nx[gn]--;
	}
	//	dos()<<v1<<" "<<v2<<" "<<var1<<" "<<var2<<'\n';
		var1=var1*v1*v1; var2=var2*v2*v2;
		TiTy zraw= (v1-v2)/(var1+var2);//vj/var*vj;
		TiTy z= zraw;//sqrt(zraw*zraw);
		TiTy p=0;
		set(z); 
		cdf::normal(p,z,0,1);
		//TiTy a4=cum_pay[2]-cum_pay[1];
		if (p>m_p) ++it.p;
		if (z>m_ok_limit) ++it.ok;
		if (it.interval==it.modi) 
			{it.modi=0; dos()<<z<<" at p="<<p <<" frac="<<(1.0f*it.ok/it.total)
			<<" "<<(1.0f*it.p/it.total) <<'\n'; }
		else ++it.modi;
		return true; 
	}

private:
	gen_param m_gp1,m_gp2;
	randTy m_ok_limit,m_p;
};

/*
class wilcoxon_stat: public stat_base_class
{//http://www.okstate.edu/sas/v7/sashtml/books/stat/chap34/sect13.htm
public:
	wilcoxon_stat(gen_param & gp1, gen_param & gp2, const randTy lim)
		:m_gp1(gp1),m_gp2(gp2),m_ok_limit(lim) {}
	
	bool run(GrTy &g ,const stat_params & sp, it_stats & it) 
	{
		const IdxTy n1=m_gp1.n(),n2=m_gp2.n();
		const IdxTy m_events=sp.events;
		IdxTy nlim=n1+n2;
		if ((nlim>m_events)&&(m_events!=0)) nlim=m_events;
		TiTy cum_pay[3];
		cum_pay[2]=cum_pay[1]=0;
		int tot_at_risk[3]; tot_at_risk[1]=n1; tot_at_risk[2]=n2;
		for (IdxTy i=0; i<nlim; ++i)
		{
			GrTy::value_type & pp=g[i];
			const IdxTy gn=pp.gr();
			for (IdxTy j=1; j<3; ++j)
			{
				const IdxTy c=(j==gn)?1:0;
				cum_pay[j]+=(c-1.0*tot_at_risk[j]/(n1+n2-i));
			}
			--tot_at_risk[gn];
		//	dos()<<" x"<<j1<<" "<<j2<<'\n';	
	}
		// assume group 1 is drug, 2 placebo 
		TiTy a4=cum_pay[2]-cum_pay[1];
		set(a4); 
		if (a4>m_ok_limit) ++it.ok;
		if (it.interval==it.modi) 
			{it.modi=0; dos()<<a4<<" "<<(1.0f*it.ok/it.total) <<'\n'; }
		else ++it.modi;
		return true; 
	}

private:
	gen_param m_gp1,m_gp2;
	randTy m_ok_limit;
};

*/

#endif

