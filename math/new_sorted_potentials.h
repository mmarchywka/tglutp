#ifndef SORTED_POTENTIALS_H__
#define SORTED_POTENTIALS_H__


#include "../math/radial_potentials.h"
#include "../base/points_base_classes.h"
#include "../math/proximity_cache.h"

#include "../data_and_config.h"
#include <vector> 
#include <algorithm>

#include <iostream>
#include <string>

template <int N,class Tr > class new_sorted_potentials
{	typedef new_sorted_potentials Myt; 
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
	typedef   proximity_cache<TgtTy, TR> CacheTy;
	typedef typename CacheTy::iterator TgtItor;
static ostream_type & rs()
{ 	static ostream_type & os=std::cout; return os; }

public:
	typedef CoherentTy coherent_type;
	typedef PotTy potential_type;
	new_sorted_potentials() {}
	count_type used_size() const
	{ 	return  m_pcache.size(); }
	template <class Ti> void load(Ti & ii, Ti& ie)
	{ 	m_pcache.load(ii,ie);	 }

	
	// locs are relative and contain (2*dx,dx*dx,0) for x,y, and z
	potential_type Vfast( const coherent_type & locs)
	{
		typedef coord_type (*potential_func)( const coord_type );
		potential_func potential_ptr=radial_potentials::one_over_4;
		//potential_ptr=radial_potentials::one_well;
		potential_type v=potential_type();
		v=0;
		m_pcache.new_location(locs[0]);
			TgtItor ii=m_pcache.begin(),ie=m_pcache.end();
		while (ii!=ie)
		{
			ItemTy x=(*ii)[0]-locs[0];
			//ItemTy dr=x-locs[0],dr1=x-locs[1],dr2=x-locs[2],dr3=x-locx[3]; 
			coord_type d=x.length2();
			if ( d<100) {m_pcache.using_point(*ii); 
			if ((d<49)){
				v[0]+=(potential_ptr)(d);
//			v[1]+=1.0f/(d-locs[1].x()*x[0]+locs[1].y());
//			v[2]+=1.0f/(d-locs[2].x()*x[1]+locs[2].y());
//			v[3]+=1.0f/(d-locs[3].x()*x[2]+locs[3].y());
			
			v[1]+=(potential_ptr)(d-locs[1].x()*x[0]+locs[1].y());
			v[2]+=(potential_ptr)(d-locs[2].x()*x[1]+locs[2].y());
			v[3]+=(potential_ptr)(d-locs[3].x()*x[2]+locs[3].y());
			
			}
			}
			++ii;
		}
		// we are too far away, at least get a direction...
		if (v[0]==0)  { m_pcache.clear(); return Vslow(locs); }
		
		v[1]-=v[0]; v[2]-=v[0]; v[3]-=v[0];
		return v;
		
		
		}
	potential_type Vslow( const coherent_type & locs)
	{
		potential_type v=potential_type();
		v=0;
		TgtItor ii=m_pcache.begin(), ie=m_pcache.end();
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
#if 0 
	potential_type Vdefer( const coherent_type & locs)
	{
		potential_type v=potential_type();
		v=0;
		my_scalar_array<coord_type,8> nd; // order is n0,d0,n1,d1 etc
		const coord_type scale_const=1e-6;
		nd=scale_const;
		count_type current=0,add_count=0 ;
		coord_type d=7;
		// 2008-03-18 this didn't compile either, it is possible that this code was never called before
		// it looks like that is the case, so I will comment out whole thing for now...
		TgtItor ii=m_v.begin(), ie=m_v.end();
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
#endif
	
	
	ItemTy nearestColor(const ItemTy& loc)// const
	{
		ItemTy colo=ItemTy(0,0,0); 
		coord_type tot=0;
		// it better be valid... 
		TgtItor ii=m_pcache.begin(),ie=m_pcache.end();
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


	CacheTy m_pcache;
	
};

#endif


