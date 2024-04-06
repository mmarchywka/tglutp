#ifndef DISTANCE_TO_SEGMENTS_H__
#define DISTANCE_TO_SEGMENTS_H__

#include "../base/points_base_classes.h"
#include <list>
#include <string>
class distance_to_segments
{
	typedef float coord_type;
	typedef point3_template<coord_type> PointTy;
	typedef point3_array<coord_type, 2> SixTy;
	typedef point3_array<coord_type, 3> StackTy;
	typedef list<StackTy> RegionListTy;
	typedef RegionListTy::const_iterator ItorTy;
	
	typedef basic_string<char> string_type;
public:
	typedef PointTy point_type;
	typedef SixTy pair_type;
	distance_to_segments():m_list() {}
	
	// convert to more useful offset and normal form. 
	// Do NOT normalize since we need to know extent.
	void push_segment(	const point_type & p1, const point_type &p2, const coord_type d)
	{m_list.push_back(StackTy()(p1,0)((p2-p1),1)(point_type(d,0,0),2)); }
	int size() { return m_list.size(); }
	coord_type nearest(const point_type & p ) const
	{	ItorTy ii=m_list.begin(),ie=m_list.end();
		coord_type best=10000000;
		while (ii!=ie)
		{
			const StackTy & x=(*ii);	
			coord_type d=0;
			point_type y=p-x[0];
			coord_type l2=x[1].length2();
			coord_type dot=y.dot(x[1]);
			if (dot>l2) {  d=(p-x[1]-x[0]).length2(); }
			else if (dot<0) { d=(y).length2(); }
			else {     d= y.dot(y)-(y.dot(x[1]))*(y.dot(x[1]))/l2;     }
			if (d<best) best=d;
			++ii;
		}
		return sqrt(best);
	}
	
	bool close_enuff(const point_type & p ) const
	{	ItorTy ii=m_list.begin(),ie=m_list.end();
	//	coord_type best=10000000;
		while (ii!=ie)
		{
			const StackTy & x=(*ii);	
			coord_type d=0;
			point_type y=p-x[0];
			coord_type l2=x[1].length2();
			coord_type dot=y.dot(x[1]);
			if (dot>l2) {  d=(p-x[1]-x[0]).length2(); }
			else if (dot<0) { d=(y).length2(); }
			else {     d= y.dot(y)-(y.dot(x[1]))*(y.dot(x[1]))/l2;     }
			//if (d<best) best=d;
			coord_type target=x[2].x(); target=target*target;
			if (d<=target) return true;
			++ii;
		}
		return false;
	}
	
	/////////////////////////////////////
	
	template <class Ty,class Tx,class Tz> void select_segments(const Ty & cf,const Tx & str,const Tz n_)
{
	Tz n=n_,i=1; 	
	typedef pair_type PairTy;
	typedef std::ostringstream OsStreamTy;
	PairTy pt=PairTy();
//	cf.parse(str,"segment_count",n);
	while (n)
	{	OsStreamTy os; //=OsStreamTy();
		os<<i;
		string_type nm="point"+os.str();
		cf.parse(str,nm,pt);
		coord_type cutoff=0; //m_cutoff_distance;
		string_type nm2="cutoff"+os.str();
		cf.parse(str,nm2,cutoff);
//		fhgfhgfh
		push_segment(pt[0],pt[1],cutoff);
		n--; i++;
	}
}
	
	
	//////////////////////////////////
private:
	RegionListTy m_list;
	
};


#endif
