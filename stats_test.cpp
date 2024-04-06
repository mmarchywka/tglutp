// Mike marchywka, March 2008
// stats_test - see help and about commands for more complete documentation.
// 

#include "../debug_and_support.h"
#include "../data_and_config.h"


#include "stats_base_classes.h"
#include "stat_calc_classes.h"
#include "wavelet_separation.h"
 
 #include <map>
 
config_type global_config;
 





class stats_thing : public stats_typedefs
{
	typedef stats_thing Myt;

	typedef void (Myt:: * pStat)(GrTy &, it_stats & it);
	
public:
	stats_thing():m_gp1(1,300,1),m_gp2(2,200,1),
	m_events(0),m_display_interval(0),m_ok_limit(0),m_alpha(0),m_p_limit(0),
	m_statistic("area"), m_stat(0),m_once(false)
	{}
	~stats_thing() { delete m_stat; }
	template <class Ty > int configure(Ty& co) { return Config(co); }
	
	bool make_stat_thing(gen_param & gp1, gen_param & gp2)
	{
		if (m_stat!=0) delete m_stat;
		m_stat=0; 
		if (m_statistic=="area") m_stat = new area_stat(gp1,gp2,m_ok_limit);
		else if (m_statistic=="log_rank") m_stat = new log_rank_stat(gp1,gp2,m_ok_limit,m_p_limit);
		else if (m_statistic=="log_rank_v") m_stat = new log_rank_v_stat(gp1,gp2,m_ok_limit,m_p_limit);
		else if (m_statistic=="wilcoxon_v") m_stat = new wilcoxon_v_stat(gp1,gp2,m_ok_limit,m_p_limit);
		else if (m_statistic=="kaplan_meier") m_stat = new kaplan_meier_stat(gp1,gp2,m_ok_limit,m_p_limit);
		else if (m_statistic=="wavelet_sep") m_stat = new wavelet_sep(gp1,gp2,m_ok_limit,m_p_limit);
		//else if (m_statistic=="wilcoxon") m_stat = new wilcoxon_stat(gp1,gp2,m_ok_limit);
		else if (m_statistic=="rank_sum") m_stat = new rank_sum_stat(gp1,gp2,m_ok_limit,m_p_limit);
		else eos()<<__FILE__<<" "<<__LINE__<<" no available statistic class for "<<m_statistic<<'\n';
		if (m_stat!=0) eos()<<MM_MARK<<" Making test stat "<<m_stat->name()<<'\n';
		
		return (m_stat!=0);
	}
	IdxTy run()
	{
		if ( m_once) return run_once();
		if (m_alpha!=0) return find_alpha();
		it_stats it;
		it.interval=m_display_interval;
		stat_params sp(m_events);
		group_generator gg;
		make_stat_thing(m_gp1,m_gp2);
		while (true)
		{	
			doo(m_gp1,m_gp2,gg,it,sp);	
		}
		return 0;
	}
	
	IdxTy run_once()
	{
		it_stats it;
		it.interval=0; //m_display_interval;
		stat_params sp(m_events);
		group_generator gg;
		make_stat_thing(m_gp1,m_gp2);
		GrTy g;
		gg.generate(g,m_gp1,m_gr_file[0]);
		gg.generate(g,m_gp2,m_gr_file[1]);
		g.time_sort();		
		m_stat->run(g,sp, it);
		dump_groups(g,"stats_test_gr1.txt",1);
		dump_groups(g,"stats_test_gr2.txt",2);
		dos()<<MM_MARK<<" The calculated stat is "<<m_stat->result()<<Trl(); 
		return 0;	
	}
	IdxTy dump_groups( GrTy & g, const Chr * nm1, const IdxTy gr)
	{
		OsTy * os = new std::ofstream(nm1);
		if (!*os) {eos()<<MM_MARK<<" cant output group "<<gr<<" to file "<< nm1<<Trl(); delete os; return -1;}
		GrTy::iterator ii=g.begin(),ie=g.end();
		while ((ii!=ie)&&(*os))
		{
			if ((*ii).gr()==gr) (*os)<<(*ii).time()<<'\n';
			++ii;
		}
		if (!*os) 
		{  eos()<<MM_MARK<<" cant write to output file"<<nm1<<Trl();   delete os ; return 1;}
		delete os;
		return 0;
	}
	void doo(gen_param & gp1, gen_param & gp2, group_generator & gg, it_stats & it,
			stat_params & sp)
	{
		GrTy g;
		gg.generate(g,gp1,m_gr_file[0]);
		gg.generate(g,gp2,m_gr_file[1]);
		g.time_sort();
		++it.total; 		
		m_stat->run(g,sp, it);
		
	}
	IdxTy find_alpha()
	{
		gen_param sg=m_gp1; // take 2 as the control for now.
		sg.m_m=m_gp2.m_m;
		TiTy ok_lo=0, ok_hi=m_ok_limit;
		stat_params sp(m_events);
		group_generator gg;
		// probably want a histogram to get close, but this should be ok for now...
		// reset m_ok_limit here
		TiTy p=0;
		do {
		m_ok_limit=(ok_lo+ok_hi)/2;
		make_stat_thing(sg,m_gp2);
		it_stats it;
		it.interval=-1;
		for (IdxTy i=0; i<50000; ++i)
		{ doo(sg,m_gp2,gg,it,sp);}
		p=(1.0f*it.ok/it.total);
		if ( p>m_alpha) ok_lo=m_ok_limit*.99; else ok_hi=m_ok_limit*1.01;
		MM_PRINT " p="<<p<<" at limit="<<m_ok_limit<<'\n';
		} while ( (p<.0249)||(p>.0251)); 
		make_stat_thing(m_gp1,m_gp2);
		it_stats it;
		it.interval=-1;
		for (IdxTy i=0; i<50000; ++i)
		{ doo(m_gp1,m_gp2,gg,it,sp);}
		p=(1.0f*it.ok/it.total);
		MM_PRINT " power at this ratio is ="<<p<<" at limit="<<m_ok_limit<<'\n';
		return 0;	
	};
	
	TiTy find_x2_p(TiTy x2, IdxTy dfi)
	{
		double p=0; 
		IdxTy status= cdf::chi_square(p,x2,dfi);
		if ( status==0) return p;
		return -1.0;
	
	}
	
	
	void init() {}
	void update()  {}
	void clr_model()
	{}
	void trim_model(int sz)
	{}
	void push_from_model(float *x)
	{	
	}
	
	
	template <class Os> static void help(Os& os) {Help(os); }
private:

	template <class Tg, class Os,class Co> void check_get
	( const Co & co, const char * st, Os & os, Tg & t, const bool dump   )
	{
		St x=St(st);
		if (co.has(x)) { co.get(x,t); if (dump) os<<" setting "<<st<<" to "<<t<<Trl(); }
			
	} 
	template <class Os,class Co> bool check_get
	( const Co & co, const char * st, Os & os, const bool dump   )
	{
		St x=St(st);
		if (co.has(x)) {  if (dump) os<<" setting "<<st<<" to TRUE" <<Trl();  return true;}
		return false; 
	} 
	template <class Ty> int Config(const Ty & co)
	{
		OsTy & os=dos();
		const bool dump=co.has(St("dump"));
		check_get(co,"m1",os,m_gp1.m(),dump); //co.get(St("m1"),m_gp1.m());
		check_get(co,"m2",os,m_gp2.m(),dump); //co.get(St("m2"),m_gp2.m());
		check_get(co,"n1",os,m_gp1.n(),dump); //co.get(St("n1"),m_gp1.n());
		check_get(co,"n2",os,m_gp2.n(),dump); //co.get(St("n2"),m_gp2.n());
		check_get(co,"effect1",os,m_gp1.effect(),dump); //co.get(St("n1"),m_gp1.n());
		check_get(co,"effect2",os,m_gp2.effect(),dump); //co.get(St("n2"),m_gp2.n());
		check_get(co,"events",os,m_events,dump); //co.get(St("events"),m_events);
		check_get(co,"alpha",os,m_alpha,dump); //co.get(St("alpha"),m_alpha);
		check_get(co,"p_limit",os,m_p_limit,dump); //co.get(St("alpha"),m_alpha);
		m_once=check_get(co,"once",os,dump); 
		check_get(co,"file1",os,m_gr_file[0],dump);
		check_get(co,"file2",os,m_gr_file[1],dump); 
		check_get(co,"disp_interval",os,m_display_interval,dump); //co.get(St("disp_interval"),m_display_interval);
		check_get(co,"ok_limit",os,m_ok_limit,dump); //co.get(St("ok_limit"),m_ok_limit);
		St shape="line";
		check_get(co,"graphic",os,shape,dump); //co.get(St("graphic"),shape);
		if (shape=="exp") {	m_gp1.distr(1); m_gp2.distr(1); }
		//St statistic="area";
		check_get(co,"statistic",os,m_statistic,dump); //co.get(St("statistic"),m_statistic);
	//	if (statistic=="log_rank") {	m_statistic=&Myt::log_rank_stat;}
		double xx; int df=0;
		check_get(co,"df",os,df,dump);
		check_get(co,"chisq",os,xx,dump);
		if (df!=0)
		{ dos()<<MM_MARK<<" p for these chi-squaer parame is "<<find_x2_p(xx,df)<<'\n'; return 1; }
		
		
		
		return 0; 
	}
	
	
	
	
	template <class Os> static void Help(Os&os)
	{
		os<<Ldr(0)<<"-n1 N : number in group 1  " <<Trl();
		os<<Ldr(0)<<"-n2 N : number in group 2  " <<Trl();
		os<<Ldr(0)<<"-m1 N : slope of group 1  " <<Trl();
		os<<Ldr(0)<<"-m2 N : slope for group 2  " <<Trl();
		os<<Ldr(0)<<"-events N: limit statistic to first N events  " <<Trl();
		os<<Ldr(0)<<"-disp_interval N : trials per display ( which can be slow)  " <<Trl();
		os<<Ldr(0)<<"-ok_limit N : threshold on test statistic  " <<Trl();
		
	}

	gen_param m_gp1,m_gp2;
	IdxTy m_events,m_display_interval;
	randTy m_ok_limit,m_alpha,m_p_limit;
//	Xform  m_p1,m_p2;
//	pStat m_statistic;
	St m_statistic;
	stat_base_class * m_stat;
	bool m_once;
	std::map<IdxTy,Chr *>  m_gr_file; 
};




typedef std::ostream OsTy;
OsTy & eos() { return std::cerr; } 
OsTy & dos() { return std::cout; } 
typedef std::string St;
typedef int IdxTy;
St Ldr(const IdxTy i)  {St x="    "; IdxTy j=i; while( 0!=(j--)) x+=" "; return x;} 
St Trl()  { return "\n"; }
	
void help_msg()
{
	std::ostream & os= std::cout;
	os<<Ldr(0)<<"Usage: stats_test" <<Trl();
	os<<Ldr(1)<<"Generate various survival curves." <<Trl();
	os<<Ldr(1)<<"This is monte carlo code designed to do brute force simulation of various" <<Trl();
	os<<Ldr(1)<<"clinical trial scenarios. It was written specifically to examine a trial" <<Trl();
	os<<Ldr(1)<<"from Dendreon, but I have generalized it somewhat and used it to test the" <<Trl();
	os<<Ldr(1)<<"related program 'streaming_data' that can visualize data generated in real time." <<Trl();
	os<<Ldr(1)<<"It is designed to be simplistic and adpatable by source code additions and not" <<Trl();
	os<<Ldr(1)<<"to be completely general for user in current form. " <<Trl();
	os<<Ldr(1)<<"Note that, unfortunately, this cmd parser doesn't report errors, use this to check:"<<Trl();
	os<<Ldr(1)<<"-DEBUG : turn on cmd parse diagnostics for subsequent parameter reads  " <<Trl();
		
	stats_thing::help(os);
	os<<"      Contact: marchywka@hotmail.com, Mike Marchywka 2008" <<Trl(); 
}
void about_msg()
{
	std::ostream & os= std::cout;

	os<<Ldr(0)<<"Mike Marchywka, 2008. Uses code from various source, mostly described below." <<Trl();
	os<<Ldr(1)<<"Originally developed using cygwin." <<Trl();
	
	os<<Ldr(1)<<"References:" <<Trl();
	os<<Ldr(2)<<"Ref:The Versatility of Function-indexed Weighted Log-rank Statistics MR. Kosorok, Chin-Yu Lin Jrnl ASA." <<Trl();
	os<<Ldr(2)<<"Ref:http://citeseer.ist.psu.edu/124967.html" <<Trl();
	os<<Ldr(2)<<"Ref:Log-rank explained:" <<Trl();
	os<<Ldr(2)<<"Ref:http://www.ms.uky.edu/~mai/research/LogRank2006.pdf" <<Trl();
	os<<Ldr(2)<<"Ref: Kleinbaum and Klein, Survival Analysis a Self-learning text, page 65:" <<Trl();
	os<<Ldr(2)<<"Ref:http://books.google.com/books?id=GNhzxRkFnJ0C&pg=PA65&lpg=PA65&dq=wilcoxon+log+rank&source=web&ots=Z27qYizj31&sig=SSZvyQmJjRDW8aZkiIgtaPPhXSI&hl=en#PPA66,M1"<<Trl();
	os<<Ldr(2)<<"Ref:Stat disctirutions from here: "<<Trl();
	os<<Ldr(2)<<"Ref:http://people.scs.fsu.edu/~burkardt/cpp_src/dcdflib/dcdflib.html"<<Trl();
	
	os<<Ldr(2)<<"Ref:http://ocw.mit.edu/OcwWeb/Mathematics/18-327Wavelets--Filter-Banks-and-ApplicationsSpring2003/LectureNotes/"<<Trl();
	stat_base_class::about(os);

}
int main(int argc, char **argv) 
{	// This does not validate command line or config file parameters
	int x=global_config.load(argc,argv,&help_msg, &about_msg);
	if (x!=0) return x; 
	stats_thing st;
	if (st.configure(global_config)!=0) return 1;
	st.run();
	return 0;
	}	
	
	


