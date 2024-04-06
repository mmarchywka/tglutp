#ifndef WAVELET_SEPARATION_H__
#define WAVELET_SEPARATION_H__
#include "stat_calc_classes.h"
#include "./math/nonuniform_wavelet_model.h"

#include <list>

/*
$ ./stats_test -statistic wavelet_sep -disp_interval 1000 -ok_limit 0 -p_limit
.95 -m1 2 -n1 250 -n2 250 -graphic exp | grep -v [a-z] | awk '{if($4!=0) print
"3 "$2*200" "$3*1000" " l3*00" 1 "last-1" " l2-1; l3=l2; l2=last; last=$1; }' |
 ./streaming_data -history 500

  677  ./stats_test -statistic wavelet_sep -disp_interval 1000 -ok_limit 0 -p_li
mit .95 -m1 1.5 -n1 250 -n2 250 -graphic exp | grep -v [a-z] | awk '{ print "3 "
$2*200" "$7*100" " l3*00" 1 "$8" " 1; l3=l2; l2=last; last=$1; }' | ./streaming_
data -history 500
  678  ./stats_test -statistic wavelet_sep -disp_interval 1000 -ok_limit 0 -p_li
mit .95 -m1 1.5 -n1 250 -n2 250 -graphic exp  2>junk

$ ./stats_test -statistic wavelet_sep -disp_interval 1000 -ok_limit 0 -p_limit .95 -m1 1.2 -n1 250 -n2 250 -graphic line | grep -v [a-z] | awk 'BEGIN{print "LINE 0 0 0 0 1000 0 1"; print "PUSHCOLOR 1 0 0"; print "LINE 0 0 0 1000 0 0 1";} { print "3 "$2*500" "$7*200" " l3*00" 1 "$8" " 1; l3=l2; l2=last; last=$1; }' | ./streaming_data -history 500





*/
class wavelet_sep: public stat_base_class
{	
	typedef double model_type;
	non_uniform_wavelet_model<3,model_type> mm;
	class delay {public : double x[8]; };	
	typedef std::list<delay> Dv;
public:
	const Chr * name() const { return "wavelet_sep";}
	wavelet_sep(gen_param & gp1, gen_param & gp2, const randTy lim,const randTy p )
		:m_gp1(gp1),m_gp2(gp2),m_ok_limit(lim),m_p(p) {}
	virtual ~wavelet_sep() {}
	bool run(GrTy &g ,const stat_params & sp, it_stats & it) 
	{
		const IdxTy n1=m_gp1.n(),n2=m_gp2.n();
		const IdxTy m_events=sp.events;
		IdxTy nlim=n1+n2;
		if ((nlim>m_events)&&(m_events!=0)) nlim=m_events;
		IdxTy nx[3]; nx[2]=nx[1]=0;
		mm.clear_point_stack();
		// d is failures at time t, n are alive prior to t
		IdxTy state=0,nright=0;//,s1=0,s2=0; 
		Dv q;
		const bool he= g.have_effect();
		//eos()<<MM_MARK<<" effect is "<<(he?"T":"F")<<'\n';
		for (IdxTy i=0; i<nlim; ++i)
		{
			GrTy::value_type & pp=g[i];
			const IdxTy gn=pp.gr();
			model_type s=(gn==1)?n1:n2;
			// at this point, you could argue that the wavelets "know" which group
			// since the increment varies unless each group has same number of patients. 
			s=((model_type)nx[gn])/s;
			if (he ) s=pp.outcome().effect();
			if (!he)mm.add_point(pp.time(),s);
			else mm.add_point(pp.time(),pp.outcome().effect() );
			if (!mm.m_was_ok) {eos()<<"At i="<<i<<"\n";mm.DumpRelevantPoints(); }
			model_type * xx=mm.m_X[0];
			//float n1=(xx[4]+xx[3]+xx[2]*0+xx[1]+xx[0])/(xx[2]-xx[3])/(xx[2]-xx[1])/4;
			// these are the derived coefficients
			//model_type wn=(xx[4]+xx[3]+xx[2]*0+xx[1]+xx[0])/(xx[2])/(xx[2])/4;
			// This is the most recent filter result
			// This needs to be delayed 1 or two...
			model_type m1=mm.m_result;
			if (state==0) if (m1>.02) state=1;
			if (state==1) if (m1<-.02) state=0;
			delay d;
			d.x[0]=gn; d.x[1]=pp.time(); d.x[2]=m1; d.x[3]=xx[2]; 
			d.x[4]=xx[1]; d.x[5]=xx[0]; d.x[6]=s; //d.x[7]=s1; 
			q.push_back(d);
			if (q.size()==2)
			{
			delay & b=q.front();
			dos()<<b.x[0]<<" "<<b.x[1]<<" "<<b.x[2]<<" "<<b.x[3]<<" "<<b.x[4]<<" "<<b.x[5]<<" "
			<<b.x[6]<<" "<<state<<'\n';
			if (int(b.x[0]+.5)==(state+1)) ++nright;
			q.pop_front(); 	
			}
			nx[gn]++;
		
		//	dos()<<gn<<" "<<pp.time()<<" "<<m1<<" "<<xx[2]<<" "<<xx[1]<<" "<<xx[0] <<" "<<s<<" "<<s1<<'\n';
		//s2=s1; s1=state; 
	}
//	return true;
		TiTy fr=(TiTy(nright)/nlim);
		if (fr<.5) fr=1.0-fr;
		fr=(fr-.5)*2;
//		eos()<<fr<<'\n';
	//	TiTy zraw=vj/var*vj;
		TiTy z=0;//  sqrt(zraw*zraw);
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

#endif
