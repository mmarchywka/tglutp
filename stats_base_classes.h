#ifndef STATS_BASE_CLASSES_H__
#define STATS_BASE_CLASSES_H__





#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <fstream>

/*
// right now, this is just a group and a time of event
	// can expand into prognostic factors, etc
	// expectation is to make lists of these objects, not pointers,
	// that may be searched, sorted etc. Keep short with pointers
	// to suporting information. 
	template <class Tx=void > class event_desc
	{	
		typedef Tx Tinfo;
		typedef int IdxTy;
		typedef double TiTy;
		
		public:
		event_desc():m_group(0),m_time(0),m_info(0) {}
		event_desc(const IdxTy gr, const TiTy & t): m_group(gr),m_time(t),m_info(0) {}
		event_desc(const IdxTy gr, const TiTy & t, Tinfo * i)
				: m_group(gr),m_time(t),m_info(i) {}
		IdxTy gr() const { return m_group;}
		// one of the compilers had a problem with const overloads , IIRC involving templates. 
		const TiTy & time() const { return m_time;}
		TiTy & time() { return m_time;}
		Tinfo * info() { return m_info; } 
		private:
			IdxTy m_group;
			TiTy m_time;
			Tinfo* m_info;
	};
	*/
	// general patient descrbied by prognostic factors, treatment, and outcome.
	
	template < class Tprog, class Ttreat, class Toutcome> class patient_desc
	{
		typedef patient_desc Myt;
		typedef Tprog Tp;
		typedef Ttreat Tt;
		typedef Toutcome To;
		typedef typename Tt::GroupTy GrTy;
		typedef typename To::TiTy TiTy;
	public:
		patient_desc(): m_prog(),m_treatment(),m_outcome() {}
		Tp & prognosis() { return m_prog; }
		Tt & treatment() { return m_treatment; }
		To & outcome() { return m_outcome; }
		patient_desc(const GrTy gr, const TiTy & t): m_treatment(gr),m_outcome(t) {}
		GrTy gr() const { return m_treatment.group();}
		// one of the compilers had a problem with const overloads , IIRC involving templates. 
		const TiTy & time() const { return m_outcome.time();}
		TiTy & time() { return m_outcome.time();}
	private:
		Tp m_prog;
		Tt m_treatment;
		To m_outcome;
		
	};
	
	template <class Tpa > class patient_group : public std::vector< Tpa >
	{
		typedef patient_group Myt;
		typedef std::vector< Tpa > Super;
		typedef typename Super::value_type Patient;
		
		class TimeSort {public: template < class Tyy> bool operator()(const Tyy & a, const Tyy &b) const
	{ return (a.time()<b.time());}};
	class GroupSort {public: template < class Tyy> bool operator()(const Tyy & a, const Tyy &b) const
	{ return (a.gr()<b.gr());}};
		public:
		patient_group():m_effect(false) {}
		void time_sort() {std::sort(this->begin(),this->end(), TimeSort() ); }
		void group_sort() {std::sort(this->begin(),this->end(), GroupSort());  }
		// These were originally cofined to the group parameter thing but 
		// are really group attributes once the group is generated. 
		bool&  have_effect() { return m_effect;}
	private:
		bool m_effect;
		
	};
	/*
	template <class Ty=void> class event_group : public std::vector< event_desc<Ty> >
	{
		
		class TimeSort {public: template < class Tyy> bool operator()(const Tyy & a, const Tyy &b) const
	{ return (a.time()<b.time());}};
	class GroupSort {public: template < class Tyy> bool operator()(const Tyy & a, const Tyy &b) const
	{ return (a.gr()<b.gr());}};
		public:
		void time_sort() {std::sort(this->begin(),this->end(), TimeSort() ); }
		void group_sort() {std::sort(this->begin(),this->end(), GroupSort());  }
	private:
	
	};
*/
class Outcome  {public: typedef double TiTy; public: Outcome() : m_time(0), m_y(0) {}
			Outcome(const TiTy & t) : m_time(t), m_y(0) {} ;
			TiTy & time(){return m_time; } 
			const TiTy & time()const {return m_time; } 
			TiTy & effect(){return m_y; } 
			const TiTy & effect()const {return m_y; } 
			TiTy m_time,m_y;
			};
class Treatment  {public: typedef int GroupTy ; public: Treatment() : m_gr(0) {}
			Treatment(const GroupTy & t) : m_gr(t) {} ;
			const GroupTy & group()const {return m_gr; } 
			GroupTy m_gr;
		};
class Prognosis {public:};
		
class stats_typedefs
{
	public:
	typedef int IdxTy;
	typedef char Chr; 
	typedef std::string string_type;
	typedef string_type St;
	typedef double randTy;
	typedef double TiTy;
	//typedef event_group<> GrTy;
	typedef patient_desc<Prognosis, Treatment, Outcome> PaTy;
	typedef patient_group< PaTy > GrTy;
	typedef std::ostream OsTy;
	typedef std::istream IsTy;
	
	OsTy & eos() { return std::cerr; } 
	OsTy & dos() { return std::cout; } 
	static St Ldr(const IdxTy i) {St x="    "; IdxTy j=i; while( 0!=(j--)) x+=" "; return x;} 
	static St Trl() { return "\n"; }
};

class gen_param
{
	//	typedef TiTy (* Xform)(const TiTy & );
	typedef int IdxTy;
	typedef double randTy;
public:
	gen_param(const IdxTy gr, const IdxTy n, const randTy m)
	:m_num(gr),m_n(n),m_m(m),m_distrib_type(0),m_make_effect(0)
	{}
	IdxTy distr() const { return m_distrib_type; }
	void distr(const IdxTy d) {m_distrib_type=d; }
	IdxTy&  effect()  { return m_make_effect; }
	void effect(const IdxTy e) { m_make_effect=e;}
	randTy & m() { return m_m;}
	IdxTy & n() { return m_n; } 
	IdxTy m_num; //group number
	IdxTy m_n; // number in group
	randTy m_m; // slope
	
	IdxTy m_distrib_type; // distribution to iuse
	IdxTy m_make_effect;
};

class group_generator : public stats_typedefs
{
	typedef group_generator Myt;
	
	typedef TiTy (* Xform)(const TiTy & );
	
public:
	
	group_generator() {}
	template <class Tyy > void generate(	 Tyy & gr, gen_param & gp, const Chr * nm)
	{
		if (nm==0) generate(gr,gp);
		else load(gr,nm,gp);
	}
	template <class Tyx > void generate(	 Tyx & gr, gen_param & gp)
	{
		TiTy dell=1e-6;
		if (gp.distr()==0) this->iterate<GrTy,&Myt::uniform>(gr,gp.m_num,gp.m_n,gp.m_m,dell);
		else if (gp.distr()==1) this->iterate<GrTy,&Myt::rexp>(gr,gp.m_num,gp.m_n,gp.m_m,dell);	
		if (gp.effect()!=0) add_effect<GrTy>(gr,gp.m_num,gp.effect());
	//	eos()<<MM_MARK<<" effect is "<<gp.effect()<<'\n';
	
	}
	template <class Tyz > void load(	 Tyz & gr, const Chr* nm,gen_param & gp)
	{	 Tyz * pgr=&gr,* pend=pgr+1;
		IsTy * is = new std::ifstream(nm);
		if (*is) Load(*is,pgr,pend,gp.m_num);
		else eos()<<MM_MARK<<" cant opern "<<nm<<'\n';
		delete is;
		
	}
	
	
private:
	static TiTy uniform(const TiTy & x) { return x; } 
	static TiTy rexp(const TiTy & x) { return -::log(x+1e-10); } 
	randTy myrand(int seed)
	{ return randTy(rand())/RAND_MAX; }
	
	template <class Ty, Xform p > void iterate( 
		Ty & g, const IdxTy group,const IdxTy n, const TiTy& m, const TiTy& dell)
	{ 	typedef typename Ty::value_type Vt;
		for(IdxTy i=0; i<n; ++i) g.push_back(Vt(group,m*(*p)(myrand(0))-dell));  }

	template <class Ty > void add_effect( 
		Ty & g, const IdxTy gn, const IdxTy e)
	{ 	typedef typename Ty::value_type Vt;
		const IdxTy n=g.size();
		for(IdxTy i=0; i<n; ++i) if ( gn==g[i].gr()) g[i].outcome().effect()=g[i].time()+myrand(0)+e;  
		g.have_effect()=true;
	}

	template <class Is,class It > IdxTy Load(Is& is, It * mss, It * pe,const IdxTy grn)
	{	typedef char Chr;
		typedef typename It::value_type Vt;
		enum {LEN=(1<<15),LENSZ=LEN+2, LENTS=LEN-2,EOS=0};
		Chr buf[LENSZ];
		while (is)
		{
			buf[LENTS]=EOS;
			//IdxTy len=
			is.getline(buf,LEN);	
			if (buf[LENTS]!=EOS)
			{	buf[40]=EOS;
				eos()<<MM_MARK<<" Line too long starts "<<buf<<'\n';
				return -1;
			}
			if (!is) return 0;
			IdxTy i=0,g=grn;
			It* j=mss;
		//	const TiTy t=atof(buf);
			while (j!=pe) {
				const TiTy v=atof(buf+i);
				j->push_back(Vt(g,v));
				while ((buf[i]!=' ')&&(buf[i]!=EOS)) ++i; 
				while ((buf[i]==' ')&&(buf[i]!=EOS)) ++i; 
				if ( buf[i]==EOS) break; 
				++j; ++g;
			} 
		}	
		
		
	}


};

// for iterating over many trial runs, this should be a little cleaner.
class it_stats 
{
	public : 
	it_stats() :modi(0),ok(0),total(0),interval(1),p(0) {} 
	int modi,ok,total,interval,p; 
};




#endif
