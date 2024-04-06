#ifndef PROXIMITY_CACHE_H__
#define PROXIMITY_CACHE_H__

#include "../math/various_searches.h"

#include <vector>
#include <algorithm>

template <class Targ, class Tr> class proximity_cache
{
	typedef proximity_cache Myt; 
	typedef Tr TR;

	typedef Targ TgtTy;
	typedef typename TR::count_type count_type;
	typedef typename TR::index_type index_type;
	typedef typename TR::coord_type coord_type;
	typedef typename TR::string_type string_type;
	typedef typename TR::ostream_type ostream_type;
	
	typedef vector<TgtTy> TgtVector;
	typedef typename TgtVector::iterator TgtItor;
	
	typedef point3_template<coord_type> ItemTy;
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
proximity_cache(): m_v(),m_cache(),m_last(),m_size_used(0),m_use_cache(false){}
 typedef TgtItor iterator; 
 void clear() { m_cache.clear(); } 
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
count_type size() const { return m_size_used; }
void using_point(const TgtTy & ii)
{
	if (!m_use_cache) 
					m_cache.push_back(ii); 
} 
void new_location(const ItemTy & loc)
{	coord_type d=10;
	TgtItor yi=m_v.begin(), yf=m_v.end();
	////////////////////////////////////////////////
	m_use_cache=((loc-m_last).length2()<4)&&(m_cache.size()>10); 
	if (m_use_cache)
		{yi=m_cache.begin(); yf=m_cache.end(); m_size_used=(yf-yi);}
	else{	
	Limit(yi,yf,loc.x()-d,loc.x()+d);
			m_cache.clear(); 
			m_last=loc;
		}
	m_ii=yi; m_if=yf;
}
TgtItor & begin() { return m_ii; }
TgtItor & end() { return m_if; }

	TgtVector m_v;
	//TinyTy 
	TgtVector m_cache;	
	ItemTy m_last; 
	count_type m_size_used;
	bool m_use_cache;
	TgtItor m_ii,m_if; 
};

#endif
