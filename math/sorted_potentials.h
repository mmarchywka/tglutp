#ifndef SORTED_POTENTIALS_H__
#define SORTED_POTENTIALS_H__

#include "../base/points_base_classes.h"
#include "../math/various_searches.h"
#include "../data_and_config.h"
#include <vector> 
#include <algorithm>

#include <iostream>
#include <string>
class r_traits
{
public:
typedef int count_type;
typedef int index_type;
typedef float coord_type;
typedef char Ch;
typedef basic_string<Ch> string_type;	
	typedef std::basic_ostream<Ch>  ostream_type;
	
};

template <int N,class Tr > class sorted_potentials
{	typedef sorted_potentials Myt; 
	typedef Tr TR;
	enum {NN=N};
	typedef typename TR::count_type count_type;
	typedef typename TR::index_type index_type;
	typedef typename TR::coord_type coord_type;
	typedef typename TR::string_type string_type;
	typedef typename TR::ostream_type ostream_type;

	// sort the objects, not pointers since we use it a lot.
	typedef point3_array<coord_type,NN> TgtTy;
	
	typedef point3_array<coord_type,4> CoherentTy;
	typedef my_scalar_array<coord_type,4> PotTy;
	typedef point3_template<coord_type> ItemTy;
	typedef vector<ItemTy> TinyTy;
	typedef vector<TgtTy> TgtVector;
	typedef typename TgtVector::iterator TgtItor;
	
	// the first element is the "key"
class compare_func { public:  int operator () (const TgtTy & i, const TgtTy & j) 
const {return i[0].x()<j[0].x();}};
class compare_fb { public:  int operator () (const coord_type & j,const TgtTy & i ) 
const {return i[0].x()>=j;}
int operator () (const TgtTy & i,const coord_type & j ) 
const {return i[0].x()<j;}
};
static ostream_type & rs()
{ 	static ostream_type & os=std::cout; return os; }

public:
typedef CoherentTy coherent_type;
typedef PotTy potential_type;
sorted_potentials(): m_v(),m_cache(),m_last(),m_size_used(0) {}
count_type used_size() const
{ return  m_size_used;}
template <class Ti> void load(Ti & ii, Ti& ie)
{
	//mmdd::dout(MMDDLOC)<<"got here\n";
	while (ii!=ie)
	{
		const ItemTy p1=ItemTy((*ii));
		// we can pass a ptr later...
		const ItemTy p2=ItemTy((*ii).get_color_3());
	m_v.push_back(TgtTy()(p1,0)(p2,1));
	++ii;	
}
//mmdd::dout(MMDDLOC)<<"got here\n";
	rs()<<"loaded list with "<<m_v.size()<<" now sorting\n";
	TgtItor yi=m_v.begin(), yf=m_v.end();
	compare_func cmp=compare_func();
	std::sort(yi,yf,cmp);

}
	static coord_type conv(const TgtTy & a) { return a[0].x(); }
	void Limit(TgtItor & ii, TgtItor & ie, const coord_type lo, const coord_type hi)//const
	{ //mmdd::dout(MMDDLOC)<<"got here\n";
		searches::bound(ii,ie,lo,hi,&Myt::conv);
		m_size_used=(ie-ii);
	} 
	// locs are relative and contain (2*dx,dx*dx,0) for x,y, and z
	potential_type Vfast( const coherent_type & locs)
	{
		potential_type v=potential_type();
		v=0;
		coord_type d=10;
		TgtItor yi=m_v.begin(), yf=m_v.end();
		////////////////////////////////////////////////
		bool use_cache=((locs[0]-m_last).length2()<4)&&(m_cache.size()>10); 
		if (use_cache)
		{yi=m_cache.begin(); yf=m_cache.end(); m_size_used=(yf-yi);}
		else{	Limit(yi,yf,locs[0].x()-d,locs[0].x()+d);
			m_cache.clear(); 
			m_last=locs[0];
			}
		
		///////////////////////////
//		Limit(yi,yf,locs[0].x()-d,locs[0].x()+d);
		TgtItor ii=yi,ie=yf;
		
		d=d*d;
		while (ii!=ie)
		{
			ItemTy x=(*ii)[0]-locs[0];
			//ItemTy dr=x-locs[0],dr1=x-locs[1],dr2=x-locs[2],dr3=x-locx[3]; 
			coord_type d=x.length2();
			if ( d<100) {if (!use_cache) m_cache.push_back(*ii); 
			if ((d<49)){
				
				v[0]+=1.0f/d;
			v[1]+=1.0f/(d-locs[1].x()*x[0]+locs[1].y());
			v[2]+=1.0f/(d-locs[2].x()*x[1]+locs[2].y());
			v[3]+=1.0f/(d-locs[3].x()*x[2]+locs[3].y());
			
			}
			}
			//v+=1.0/d/d;
			++ii;
		}
		// we are too far away, at least get a direction...
		if (v[0]==0)  { m_cache.clear(); return Vslow(locs); }
		
		v[1]-=v[0]; v[2]-=v[0]; v[3]-=v[0];
		return v;
		
		
		}
	potential_type Vslow( const coherent_type & locs)
	{
		potential_type v=potential_type();
		v=0;
		TgtItor ii=m_v.begin(), ie=m_v.end();
		while (ii!=ie)
		{
			ItemTy x=(*ii)[0]-locs[0];
			coord_type d=x.length2();
				v[0]+=1.0f/d;
			v[1]+=1.0f/(d-locs[1].x()*x[0]+locs[1].y());
			v[2]+=1.0f/(d-locs[2].x()*x[1]+locs[2].y());
			v[3]+=1.0f/(d-locs[3].x()*x[2]+locs[3].y());
			++ii;
		}
		
		v[1]-=v[0]; v[2]-=v[0]; v[3]-=v[0];
		return v;
		
		}
	// defer divisions- could be done indefinitely with ints but
	// I dont want to cast each time....
	potential_type Vdefer( const coherent_type & locs)
	{
		potential_type v=potential_type();
		v=0;
		my_scalar_array<coord_type,8> nd; // order is n0,d0,n1,d1 etc
		const coord_type scale_const=1e-6;
		nd=scale_const;
		//count_type accum_limit=10,
		count_type current=0,add_count=0 ;
		coord_type d=7;
		TgtItor yi=m_v.begin(), yf=m_v.end();
		Limit(yi,yf,locs[0].x()-d,locs[0].x()+d);
		TgtItor ii=yi,ie=yf;
		d=d*d;
		while (ii!=ie)
		{
			ItemTy x=(*ii)[0]-locs[0];
			//ItemTy dr=x-locs[0],dr1=x-locs[1],dr2=x-locs[2],dr3=x-locx[3]; 
			coord_type d=x.length2();
			if (d<100){
//				v[0]+=1.0f/d;
	//		v[1]+=1.0f/(d-locs[1].x()*x[0]+locs[1].y());
	//		v[2]+=1.0f/(d-locs[2].x()*x[1]+locs[2].y());
	//		v[3]+=1.0f/(d-locs[3].x()*x[2]+locs[3].y());
			nd[0]=nd[0]*d+nd[1]; // this is 1/d	
			nd[1]*=d; // note this doesn't commute with above :)
			coord_type dd=d-locs[1].x()*x[0]+locs[1].y();
			nd[2]=nd[2]*dd+nd[3]; // this is 1/d	
			nd[3]*=dd; // note this doesn't commute with above :)
			dd=d-locs[2].x()*x[1]+locs[2].y();
			nd[4]=nd[4]*dd+nd[5]; // this is 1/d	
			nd[5]*=dd; // note this doesn't commute with above :)
			dd=d-locs[3].x()*x[2]+locs[3].y();
			nd[6]=nd[6]*dd+nd[7]; // this is 1/d	
			nd[7]*=dd; // note this doesn't commute with above :)
			++current;
		if (nd[1]>1e20)
//		if (current==accum_limit)
		{
			v[0]+=nd[0]/nd[1];v[1]+=nd[2]/nd[3];
			v[2]+=nd[4]/nd[5];v[3]+=nd[6]/nd[7];
			current=0;
			++add_count;
			nd=scale_const;
			}
		}
		
			//v+=1.0/d/d;
			++ii;
		}
		if (current!=0)
		{
			v[0]+=nd[0]/nd[1];v[1]+=nd[2]/nd[3];
			v[2]+=nd[4]/nd[5];v[3]+=nd[6]/nd[7]; ++add_count;
		}
		
		v[1]-=v[0]; v[2]-=v[0]; v[3]-=v[0];
		v[0]-=add_count;
		return v;
		
		
		}
	
	/*
	
	
	potential_type V( const coherent_type & locs)
	{
		potential_type v=potential_type();
		v=0;
		coord_type d=7;
		TgtItor yi=m_v.begin(), yf=m_v.end();
		bool use_cache=((locs[0]-m_last).length2()<4); 
		if (use_cache)
		{
			yi=m_cache.begin(); yf=m_cache.end();
			}else{	Limit(yi,yf,locs[0].x()-d,locs[1].x()+d);
			m_cache.clear(); 
			}
		TgtItor ii=yi,ie=yf;
		m_last=locs[0];
		d=d*d;
		while (ii!=ie)
		{
			ItemTy& x=(*ii)[0];
			//ItemTy dr=x-locs[0],dr1=x-locs[1],dr2=x-locs[2],dr3=x-locx[3]; 
			//coord_type d=dr.length();
			coord_type dr=(x-locs[0]).length2();
			if (dr<=d) {
				if (!use_cache) m_cache.push_back(TgtTy()(x,0)); 
				v[0]+=1.0f/dr;
			v[1]+=1.0f/(x-locs[1]).length2();
			v[2]+=1.0f/(x-locs[2]).length2();
			v[3]+=1.0f/(x-locs[3]).length2();
			}
			//v+=1.0/d/d;
			++ii;
		}
		return v;
		
		
	}
	
	*/
	// cache thrash
	coord_type V(const ItemTy& loc) //const
	{ 
		coord_type v=0;
		coord_type d=7;
	//	mmdd::dout(MMDDLOC)<<"got here\n";
		// optimized died here.
		TgtItor yi=m_v.begin(), yf=m_v.end();
	//	mmdd::dout(MMDDLOC)<<"got here\n";
		Limit(yi,yf,loc.x()-d,loc.x()+d);
	//	mmdd::dout(MMDDLOC)<<"got here\n";
		TgtItor ii=yi,ie=yf;
		//TgtItor ii=std::lower_bound(yi,yf,loc.x()-d,compare_fb());
		//TgtItor ie=std::upper_bound(yi,yf,loc.x()+d,compare_fb());
		while (ii!=ie)
		{
			ItemTy dr=(*ii)[0]-loc;
			coord_type d=dr.length();
			v+=1.0/d/d;
			++ii;
		}
		return v;
	}
	ItemTy nearestColor(const ItemTy& loc)// const
	{
		ItemTy colo=ItemTy(0,0,0); //loc;
		coord_type tot=0;
		coord_type d=5;
		TgtItor yi=m_v.begin(), yf=m_v.end();
		Limit(yi,yf,loc.x()-d,loc.x()+d);
		TgtItor ii=yi,ie=yf;
//		TgtItor ii=std::lower_bound(yi,yf,loc.x()-d,compare_fb());
	//	TgtItor ie=std::upper_bound(yi,yf,loc.x()+d,compare_fb());
		while (ii!=ie)
		{
			ItemTy dr=(*ii)[0]-loc;
			coord_type d=dr.length2();
			if (d<25) {tot+=1.0/d; colo+=(((*ii)[1])/(d)); }
			++ii;
		}
		if (tot==0) colo=ItemTy(.5f,.6f,.6f); else colo/=tot;
		return colo;
	}
	
private:

TgtVector m_v;
//TinyTy 
TgtVector m_cache;	
ItemTy m_last; 
	count_type m_size_used;
};

#endif


