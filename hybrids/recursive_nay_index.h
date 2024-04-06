#ifndef RECURSIVE_NAY_INDEX_H__
#define RECURSIVE_NAY_INDEX_H__

#include "pdb_hybrid_typedefs.h"
#include "word_array.h"
#include <map>
#include <vector>
#include <string>
#include <memory.h>

#include <algorithm>
#include "enclave_base_classes.h"


class cygwin_stream : public std::istream
{
	typedef cygwin_stream Myt;
	typedef std::istream Super;	
public:
std::ostream & eos() { return std::cerr; } 
	cygwin_stream (Super * is) : m_is(is),m_buf(new char[1<<20]), m_sz(1<<20),m_ptr(0) 
	{
		eos()<<MM_MARK<<'\n';
		reload();
	//	m_sz=m_is->readsome(m_buf,1<<20);
		eos()<<MM_MARK<<" size is "<<m_sz<<'\n';
				m_ptr=0;	
		}
	~cygwin_stream() { delete [] m_buf; }
	bool  reload() {              	m_sz=m_is->readsome(m_buf,1<<20); m_ptr=0;	                             
	while ((m_sz==0)&&(*m_is)){ sleep(1);   	m_sz=m_is->readsome(m_buf,1<<20);           } return (m_sz==0);  } 
	bool crlf(const char c ) { return (c=='\n')||(c=='\r'); }
	// note this SKIPS blank lines...
	Super & getline(char * c, int sz)
	{eos()<<MM_MARK<<'\n';
		int i=0;
		int szn=sz-1;
		if (m_ptr==m_sz)
			{ if (reload()) return (*this);  
		}
		while ((i<szn)&&(!crlf(m_buf[m_ptr]))) 
		{
			if (m_ptr==m_sz)
			{
				if (reload()) break;
			}
			c[i]=m_buf[m_ptr]; ++m_ptr; ++i; 
			if (m_ptr==m_sz)
			{	if (reload()) break;
			}
		}
		c[i]=0;
		eos()<<MM_MARK<<'\n';
		return *this;
	}
	operator bool() {return (*m_is)||(m_ptr!=m_sz); }
private:
Super * m_is;
char * m_buf;
	int m_sz,m_ptr;
	
};


class nay_db_driver : public pdb_typedefs
{
	typedef nay_db_driver Myt;
typedef recursive_nay_index RnTy;
typedef RnTy::CoTy CoTy;
typedef hit_pair Hp;
typedef terminal_entry Te;
typedef hit_vector Hv;
typedef Hv::iterator Hi; 

typedef indexed_worm_monolith<Te> Iw;

public:	
	nay_db_driver():m_flags(0),m_format(0),m_x_r(0),m_y_r(0),m_z_r(0),m_r_r(0)
	{ }
	
	int main ( int argc, char ** argv);


private:
	// note that this is an instance, not a pointer.
	RnTy m_m;
	// output options
	IdxTy m_flags,m_format;
	CoTy m_x_r,m_y_r,m_z_r,m_r_r;
	// some output formats need a label, this is it
	St m_name;
	
	Hv m_hits;
	Iw m_worm;
	
	// arrg, this needs to delegate now... 
api_type SetDebug(Chr * const nm) 
{
	m_debug=atoi(nm);
//	m_mols.m_debug=m_debug; 
//	m_plo.m_debug=m_debug; m_sumo.m_debug=m_debug;
	return 0;	

}
api_type SetOut(Chr * const nm) {open_out(nm,m_out); 
return 0;	}
api_type SetErr(Chr * const nm) {open_out(nm,m_err); 
  return 0;	}
api_type SetName(Chr * const nm)
{ m_name=nm; return 0;  }
api_type LoadPdb(Chr * const nm)
{	
	word_array<256> wa(2048);
	int linect=0;
	const bool using_hier= ((m_flags&1)!=0);
	IsTy * myin=& std::cin;
	// This doesn't work with cin  
	///myin =new cygwin_stream(myin);
	// terrible kluge, but I don't have a simple class for these things... 
	bool need_close=false;
	if (strcmp(nm,"-")!=0)
	{
		need_close=true;
		myin=open_in(nm); 
		if (myin==0) return -1; 
	}
	while (true)
	{	//eos()<<MM_MARK<<'\n';
		wa.read_normaler(*myin, & eos(), ++linect);
		if (!*myin) break;
		int sz=wa.mindex();
		if (sz<5) continue;
		CoTy z=atof(wa.pi(sz-2));
		CoTy y=atof(wa.pi(sz-3));
		CoTy x=atof(wa.pi(sz-4));
		RnTy::Ptr p=m_m.get_or_make(wa.pi(0)[0]);
		if ( using_hier)
		{
			for ( int i=1; i<(sz-4); ++i)
			{	p=p.ptr()->get_or_make(wa.pi(i)[0]);		}
			p.ptr()->add(wa.pi(sz-1),x,y,z);
		}
		else 
		{	//St key=wa.pi(0);
			//for ( int i=1; i<(sz-4); ++i) key+=wa.pi(i);
			const int mx=(sz-4)*3;
			char xx[mx]; xx[0]=0;
			for (int i=0; i<(sz-4); ++i) strcpy(xx+strlen(xx),wa.pi(i));
			Te te=Te(wa.pi(sz-1),x,y,z); 
			
			m_worm.add(xx,te);
		}
	}
	if (need_close) delete myin; 
	if (debug()) eos()<<MM_MARK<<" loaded "<<linect<<" entries\n";
	return 0;

}
api_type QueryHits(Chr * const nm)
{
	std::sort(m_hits.begin(),m_hits.end(),hit_pair_sort());
	if (debug()) eos()<<MM_MARK<<" sorted "<< m_hits.size()<<" entries\n";
	// These are now alpha by sample and then by ( variant ) distance
	// between sample and query patch. 
	const IdxTy limm=atoi(nm);
	Hi ii=m_hits.begin(), ie=m_hits.end();
	while (ii!=ie)
	{
		Hv e;
		Hi is=ii;
		while ( strcmp((*ii).sample(),(*is).sample())==0)
		{
			e.push_back(*ii);
			++ii;
			if (ii==ie) break;
		}
		// now we have isolated a query and sample molecule. size!=0...
	//	const IdxTy sz=e.size();
	//	eos()<<MM_MARK<<" hit size to "<<(*is).sample()<<" is "<<sz<<'\n';
		//CoTy d[sz][sz];
		IdxTy i=0, j=0,qh=0,dumped=0;
		Hi iei=e.begin(), iee=e.end();
		while (iei!=iee)
		{	// If two hits are close on both sides, then we have two
			// matched patches reducing false hit rate. 
			Hi jei=iei; 
			//++jei; 
			j=i+1*0; 
			IdxTy hhx=0;
			while (jei!=iee) {  
				//d[i][j]
				CoTy ddd=(*jei).d(*iei); 
				// many patterns self-hit leading to redundancy.
				// proximity gating helps avoid this.
				const CoTy olap=10; // this is A^2, about 3A
				const CoTy near=100;
				if ((ddd<near)&&(ddd>olap)||(limm==0)){  
					 if (0)
					 {
					 	eos()<<MM_MARK<<" distance is "<<ddd<<'\n';
					 	//dump_formated(*iei);
						(*jei).dump(eos()<<"pair 1:")<<'\n'; (*iei).dump(eos()<<"pair 2:")<<'\n';
					 }
					 ++qh;++hhx;
					if (hhx>=limm)  {++dumped; //if (dumped<20) 
						dump_formated(*iei); break;} 
					
					} 
			
			++jei; ++j;}
			++iei; ++i;
		//	if (dumped>20) {  eos()<<MM_MARK<<" Stopping dump based on limit \n"; break;}
		}
		//if (dumped>20) {  eos()<<MM_MARK<<" Stoppeddump at 20 but had"<<dumped<<" \n"; }
		if (qh!=0)
			eos()<<MM_MARK<<" Qualifed hits are "<<qh<<'\n';
	}
	return 0;	
}


void dump_formated(const Hp & h )
{
	const Te & s=h.m_sample;
	const Te & q=h.m_query;
	dos()<<s.m_entry<<" "<<s.m_x<<" "<<s.m_y<<" "<<s.m_z<<" "<<q.m_entry<<" "
					<<q.m_x<<" "<<q.m_y<<" "<<q.m_z<<'\n';
}

api_type QueryPdb(Chr * const nm)
{	
	word_array<256> wa(2048);
	int linect=0;
	m_hits.clear(); 
	IsTy * is= open_in(nm);
	const bool using_hier= ((m_flags&1)!=0);
	if (debug()) eos()<<MM_MARK<<" open query file "<<nm<<'\n';
	if (is==0) { eos()<<MM_MARK<<" failed to open "<<nm<<'\n'; return failcode();  }
	while (true)
	{	// now, we are reading a set of queries.
		// in real life, we would sort the queries first to avoid
		// tedious tree traversal if the limbs are on disk somewhere...
		wa.read_normaler(*is, & eos(), ++linect);
		if (!*is) break;
		int sz=wa.mindex();
		if (sz<5) continue;
		CoTy z=atof(wa.pi(sz-2));
		CoTy y=atof(wa.pi(sz-3));
		CoTy x=atof(wa.pi(sz-4));
		// at this point, we could spatially restrict the queries... 
	//	std::cerr<<MM_MARK<<" wtf :"<<" "<<m_r_r<<'\n';
		if (m_r_r!=0)
		{
			CoTy l2=x-m_x_r; l2*=l2; l2+=(y-m_y_r)*(y-m_y_r);
			l2+=(z-m_z_r)*(z-m_z_r);
		//	std::cerr<<MM_MARK<<" wtf :"<<l2<<" "<<m_r_r<<'\n';
			if ( l2>m_r_r*m_r_r) continue; 	
			std::cerr<<MM_MARK<<" accepting zoned thing at "<<x<<" "<<y<<" "<<z<<'\n';
		}
		typedef RnTy::TerVec::value_type Vt;
		RnTy::TerVec const * e=0;
		RnTy::TerVec ex;
		if (using_hier)
		{
			RnTy::Ptr p=m_m.get(wa.pi(0)[0]);
			for ( int i=1; i<(sz-4); ++i)
			{	if (p.ptr()==0) break; p=p.ptr()->get(wa.pi(i)[0]);		}
			if (p.ptr()==0) continue; 
			//eos()<<MM_MARK<<" found \n";
			const RnTy::TerVec  & ex=p.ptr()->entries();
			e=&ex;
		}
		else
		{
			const int mx=(sz-4)*3;
			char xx[mx]; xx[0]=0;
			for (int i=0; i<(sz-4); ++i) strcpy(xx+strlen(xx),wa.pi(i));
			m_worm.find(xx, ex);
			e=&ex; 
		}
		const IdxTy n=e->size();
		// speed here is not critical... 
		for (IdxTy i=0; i<n; ++i)
		{
			const Vt & v=(*e)[i];
			const CoTy dx=v.m_x-x; const CoTy dy=v.m_y-y;
			const CoTy dz=v.m_z-z;
			const CoTy dr=::sqrt(dx*dx+dy*dy+dz*dz);
			switch (m_format)
			{
			case 0:{ 
				for ( IdxTy j=0; j<(sz-4); ++j) dos()<<wa.pi(j);
				dos()<<" hits " <<v.m_entry<<" "<<dr<<'\n';
				break; }
			
			case 1:{ 
				dos()<<m_name<<" "<<x<<" "<<y<<" "<<z<<" "<<v.m_entry<<" "
					<<v.m_x<<" "<<v.m_y<<" "<<v.m_z<<'\n';
				//for ( IdxTy j=0; j<(sz-4); ++j) dos()<<wa.pi(j);
				//dos()<<" hits " <<v.m_entry<<" "<<dr<<'\n';
				break; }
			
			case 2:{ 
				Hp qqq=Hp(m_name.c_str(),x,y,z,v);
				
				if (!qqq.self())
				{	St hxx;
					for ( IdxTy j=0; j<(sz-4); ++j) hxx+=wa.pi(j);
					qqq.string(hxx); 
					m_hits.push_back(qqq);
				}
			//	dos()<<m_name<<" "<<x<<" "<<y<<" "<<z<<" "<<v.m_entry<<" " <<v.m_x<<" "<<v.m_y<<" "<<v.m_z<<'\n';
				//for ( IdxTy j=0; j<(sz-4); ++j) dos()<<wa.pi(j);
				//dos()<<" hits " <<v.m_entry<<" "<<dr<<'\n';
				break; }
			
			
			}//switch
		}
		//p.ptr()->add(wa.pi(sz-4),x,y,z);
		
	}
	if (debug()) eos()<<MM_MARK<<" done with "<<linect<<" queries\n";
	return 0;

}






template <class Os> static void rhelp( Os& os)
{
	os<<Ldr(0)<<"Usage: nay_db_driver" <<Trl();
	os<<Ldr(1)<<"-load - : load data from stdin(- needed for later)" <<Trl();
	os<<Ldr(1)<<"-query fn: query consists of parsed pdb from pdb_proc" <<Trl();
	os<<Ldr(1)<<"-format n: format for subsequent query output" <<Trl();
	
}
template <class Os> static void credit_and_ref( Os& os)
{// nominally for human consumption but not formatted, should parse ok 
os<<Ldr(0)<<MM_MARK<<" Built on "<<__DATE__<<" at "<<__TIME__<<Trl();
os<<Ldr(0)<<"Contact: Mike Marchywka, marchywka@hotmail.com" <<Trl();
}

};


#endif






