#ifndef ENCLAVE_BASE_CLASSES_H__
#define ENCLAVE_BASE_CLASSES_H__

#include <vector>
#include <map>


class recursive_nay_index;

class rni_ptr
{
	typedef rni_ptr Myt;
	typedef recursive_nay_index TgtTy;
public:
	rni_ptr() : m_ptr(0) {}
	rni_ptr(TgtTy *  const x) : m_ptr(x) {}
	TgtTy * ptr() { return m_ptr; }
	TgtTy * m_ptr;
	
};
class terminal_entry
{	typedef terminal_entry Myt;
public:
	//typedef double CoTy;
	typedef float CoTy;
	typedef char Chr;
	enum { SZ=8};
	operator const char*() const { return m_entry;}
	CoTy r() const { return m_x*m_x+m_y*m_y+m_z*m_z; } 
	CoTy d( const Myt & t) const 
	{
		CoTy dx=t.m_x-m_x; CoTy r=dx*dx;
		dx=t.m_y-m_y; r+=dx*dx;
		dx=t.m_z-m_z; 
		return r+dx*dx;
	} 
	template <class Os> Os & dump (Os & os) const
	{ os<<m_entry<<" "<<m_x<<" "<<m_y<<" "<<m_z; return os;   }
	terminal_entry(): m_x(0),m_y(0),m_z(0) {m_entry[0]=0; }
	terminal_entry( const Chr * nm, const CoTy & x, const CoTy & y, const CoTy & z)
	: m_x(x),m_y(y),m_z(z) {strncpy(m_entry,nm,7);   }
	Chr m_entry[SZ];
	CoTy m_x,m_y,m_z;
	
};

// Really the hits should be stord as pointers with query as well as library
// parsed. For now, however, I can just use pairs 
class hit_pair
{
	typedef hit_pair Myt;
	typedef terminal_entry Te;
	typedef Te::CoTy CoTy;
	typedef Te::Chr Chr; 
	typedef std::string St;
public:
	hit_pair( const Chr * nm, const CoTy & x, const CoTy & y, const CoTy & z,  const Te & s) 
	: m_query(nm,x,y,z),m_sample(s) {}
	void string(const St & s) { m_string=s; }
	bool self() const 
	{ return strcmp(m_sample,m_query)==0;  }
	// return the great distance, sample-versu-sample or query-versus-query. 
	CoTy d(const Myt & that) const
	{
		CoTy d1=m_query.d(that.m_query);
	 	CoTy d2=m_sample.d(that.m_sample);
	 	return (d1<d2)?d2:d1;
	 }
	CoTy d() const { return m_query.d(m_sample); } 
	const Chr * sample() const { return m_sample; }
	template <class Os> Os& dump (Os & os ) const
	{ return m_sample.dump(m_query.dump(os<<m_string<<" ")<<" ");  }
//private:
	Te m_query, m_sample;
	St m_string;
};
class hit_pair_sort
{
	
public:
template <class Ty> int operator()(const Ty & a, const Ty & b)
{
	int x=strcmp(a.sample(),b.sample());	
	if (x!=0) return (x<0);
	// at this point, distance is used as an easy approxiation.
	// we want to compare things such that sample distance bewteen matches
	// is similar to query distance, not compare sample to query distances
	// which are not invariant against anything. 
	// But, the sorted list should be easy to scan for the coherence of interest
	return a.d()<b.d(); 
	
	
}	
	
};
class hit_vector : public std::vector<hit_pair>
{
	
	
};

class terminal_vector : public std::vector<terminal_entry>
{
	
	
};
// This is a general way to index a tree type key with entries at every leve. 
class recursive_nay_index 
{
	typedef recursive_nay_index Myt;
public:
	typedef rni_ptr Ptr;
	typedef char Chr;
	typedef Chr KeyTy;
	typedef std::map<Chr,Ptr> IdxMap;
	typedef terminal_vector TerVec; 
	typedef std::map<Chr,TerVec> TerMap;
	typedef terminal_vector::value_type TeTy;
	

	typedef TeTy::CoTy CoTy;
	typedef int api_type; 
	Ptr get(const KeyTy c)
	{    // This should return Ptr() which is null if nonexistent.
		return m_idx[c];
	}
	Ptr get_or_make(const KeyTy c)
	{
		if (m_idx.find(c)==m_idx.end())
		{  m_idx[c]= Ptr(new Myt()); }
		return get(c);
	}
	void add( const Chr * nm, const CoTy & x, const CoTy & y, const CoTy & z)
	{m_term.push_back(TeTy(nm,x,y,z));  } 
	const TerVec & entries() const { return m_term;}
private:
	// This is a set of terminal entries that end here
	TerVec m_term;
	// These continue at least one more letter
	IdxMap m_idx;
	
	
	
};

// Entries MUST be sorted arpriori as this loads them sequentially
// into a contiguous block while building up a vector of indicies which can 
// be used for fast access.
// Block format is null terminated key string, pointer to target
// Index contains starts of each string for searching. 

// This is similar to above EXCEPT that the data is contained in
// the block. This may create some cache thrashing during a lookup, but
// shouldn't be a problem with sorted queries. Samples MUST be sorted during loading.
// TgtTy is the ENTRY type, NOT a vector pointer.

template <class TgtTy> class indexed_worm_monolith
{
	typedef indexed_worm_monolith Myt;
	typedef TgtTy Ty;
	typedef char Chr;
	typedef Chr DaTy;
//	typedef Ty* Tpr;
	typedef unsigned int Tu;
	typedef int IdxTy;
	typedef std::pair<DaTy*,Tu> Bp;
	// These are the ordered addresses, Tu is a block index
	typedef std::vector<Bp> IdTy;
	typedef std::map<DaTy*, IdxTy> Et; // pointers to end of block 
	// Note this is the size of the OBJECT, not a pointer.
	enum {TPSZ=sizeof(Ty),BLOCK=(1<<20), MASK=(BLOCK-1) };
public:
	// This is done to prevent "if size==0" tests on all later add calls.
	typedef int api_type;
	indexed_worm_monolith():m_data(new DaTy[BLOCK]),m_ptr(0),m_sz(BLOCK) //,m_last_key(0) 
	{ m_idx.push_back(Bp(m_data,0)); m_ptr=1; }
	~indexed_worm_monolith() { delete m_data; }
	
	// Additions must be ORDERED to allow later searching. A key is either NEW
	// and greater than last key or is equal to the prior key.
	// The end of the block is always an entry associated with the last key. 
	api_type add( const DaTy * key, const Ty & data )
	{	// <0 ->key< last 
		const DaTy * pl=m_idx.back().first+m_idx.back().second;
	 	const IdxTy key_order=strcmp(key,pl);
	 	if ( key_order<0)
	 	{
	 		std::cerr<<__FILE__<<__LINE__<<" key "<<key<<" out of order WRT "<<pl
	 			<<".\n";
	 			return -1;	
	 	}
	 	// just add the thingy.
	 	if (key_order==0)
	 	{
	 		expand_by(TPSZ);
	 		::memcpy(m_data+m_ptr,&data,TPSZ);
	 		m_ptr+=TPSZ;
	 		return 0;
	 	}
	 	
	 	//m_last_key=m_ptr;
	 	const IdxTy sl=1+strlen(key);
	 	const IdxTy need=sl+TPSZ;
	 	expand_by(need);
	 	m_idx.push_back(Bp(m_data,m_ptr));
	 	memcpy(m_data+m_ptr,key,sl);
	 	m_ptr+=sl;
	 	::memcpy(m_data+m_ptr,&data,TPSZ);
	 	m_ptr+=TPSZ;
	 	return 0;
	}
	void expand_by(const IdxTy sz)
	{
		if ( m_ptr+sz<m_sz+2) return;
		DaTy* data= new DaTy[BLOCK];
		// we need to order the blocks, user m_b for that... 
		m_end[m_data]=m_b.size();
		m_b.push_back( Bp(m_data,m_ptr)) ;		
		m_sz=BLOCK; m_data=data; m_ptr=0;
	}
	void close_input()
	{	// if this is used, then the conditional logic in find can be
		// removed but the hi index has to be limited 
		m_b.push_back( Bp(m_data,m_ptr)) ;		
		m_idx.push_back( Bp(m_data,m_ptr)) ;		
	}
	
	template <class Tz > void find(const DaTy * key, Tz & d)
	{
			const IdxTy sz=m_idx.size();
			IdxTy hi=sz; // -1 for close_input
			IdxTy lo=0;
			// ok, so data+index interspersed can make bad guesses 
			// slow for disk.cache problems. We will fix this
			// when queries are sorted and searches confined. 
			IdxTy ptr=(hi+lo)>>1;
			while (hi>lo)
			{
				DaTy * px=pt(ptr);
				const IdxTy key_order=strcmp(key,px);
	 			if ( key_order<0) // key is less than guess
	 			{  hi=ptr;}
	 			else if (key_order>0) { lo=ptr+1;}
				else 
				{   	
					// so, we add the length to the ptr and then need to find the end
					// which is either the next key location, end of block, or last entry.          
					// do this first in any case...
					px+=1+strlen(key);
					DaTy* bend=m_data;
					// if this is not the last,
					if (ptr!=sz-1) bend=m_idx[ptr+1].first; 
					// if it could span many blocks, do this
					DaTy * st=m_idx[ptr].first;
					if ( st!=bend)
					{	// note that an entry MAY traverse many blocks... 
						// this points to the base of the block where we are. 
						// This is the index of m_b where the block info is found. 
						IdxTy ij=m_end[st];
						// find all indicies which may contain pieces.
						//IdxTy ij=ii;
						// ij must be valid and it can;t be zero since st must exist as we
						// are not at the end, but maybe pushing the last block with a call
						// to a "final" method would help before querying...
						const IdxTy bmsz=m_b.size();
						while ( m_b[ij].first!=bend)
						{   
							const DaTy * px2=m_b[ij].first+m_b[ij].second;
							DoPush(d,px,px2);//,__FILE__,__LINE__);
							++ij; if ( ij==bmsz){  px=m_data; break; }
							px=m_b[ij].first; 
						}
					}
					// now for the last piece...
					DaTy * px2=0;
					if ( ptr==sz-1) px2=m_data+m_ptr; else px2=pt(ptr+1);
					DoPush(d,px,px2);//,__FILE__,__LINE__);
					break;
				}
				
				ptr=(hi+lo)>>1;
			}
		
	}
private:
	DaTy * pt(const IdxTy m) { const Bp& b=m_idx[m]; return b.first+b.second; }

template <class Tz> // ,class Tc,class Tl  > 
	bool DoPush(Tz & d , DaTy *& px, const DaTy * px2) //, const Tc & f, const Tl & l)
{
/*
	const IdxTy ns = px2 -px;
	if ( (ns/TPSZ)*TPSZ!=ns) 
	{
		std::cerr<<__FILE__<<__LINE__<< "bad ptr size "<<ns<<'\n';
		return false; 
	}
	*/
	while (px<px2)
	{  d.push_back( *(Ty*)(px)  );  px+=TPSZ; }
	return true;
}


DaTy * m_data;
	// ptr to next write location, maximum size. 
	IdxTy m_ptr, m_sz;
	// m_idx is location of stuff, m_b is block ends. 
	IdTy m_idx,m_b;
	//Bp m_last_key;
	Et m_end;

//	std::vector<Bp> m_b;
	
	
	
};
#endif
