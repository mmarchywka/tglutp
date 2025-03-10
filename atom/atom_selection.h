#ifndef ATOM_SELECTION_H__
#define ATOM_SELECTION_H__


#include "../math/distance_to_segments.h"
#include "../base/points_base_classes.h"
#include "../atom/pdb_atom.h"
#include "../base/cheap_greps.h"
#include <string>
#include <sstream>
#include <vector>
//#include <map>
//#include <pair>

class atom_selection
{
	typedef atom_selection Myt;
	typedef float coord_type;
	typedef point3_template<coord_type> PointTy;
	typedef distance_to_segments DiTy;
	typedef DiTy::pair_type PairTy;
	typedef std::ostringstream OsStreamTy;
	typedef basic_string<char> string_type;
	typedef unsigned int count_type;
	
	typedef pdb_atom target_type;
	typedef target_type TgtTy;
	typedef const TgtTy * TgtPtr;
	typedef std::vector<TgtTy> TgtListTy;
	typedef TgtListTy::iterator TgtItorTy;
	typedef TgtListTy::const_iterator TgtCItorTy;
	
	typedef vector<pair<count_type,string_type> > FieldTy;
	typedef FieldTy::const_iterator FieldItorTy;
public:
	typedef TgtListTy atom_list;
	atom_selection():m_dist(),m_field_vector(),m_reject_vector(),m_check_dist(false)
	,m_cutoff_distance(0),m_point_dist(false),m_origin(0,0,0),m_myradius(0){}
	static void help()
	{
		count_type i=0;
		while (1){ std::cout<< TgtTy::field_name(i)<<" ";
			i++;
			if (TgtTy::field_name(i)=="" ) break; 
		}
		
		}
template <class Ty,class Tx> atom_selection(const Ty & cf,const Tx & str)
:m_dist(),m_field_vector(),m_reject_vector(),m_check_dist(false),m_cutoff_distance(0)
	,m_point_dist(false),m_origin(0,0,0),m_myradius(0)
{
	if (cf.has(str,"?")) { help(); return ; }
	count_type n=0; 	
	//PairTy pt=PairTy();
	cf.parse(str,"segment_count",n);
	if (cf.has(str,"origin"))
	{ 
		m_point_dist=true; cf.parse(str,"origin",m_origin);    
		cf.parse(str,"selradius",m_myradius);
	}
	if (cf.has(str,"ox"))
	{ 
		m_point_dist=true;
		coord_type x=0; 
		cf.parse(str,"ox",x);  m_origin.x()=x;
		cf.parse(str,"oy",x);  m_origin.y()=x;
		cf.parse(str,"oz",x);  m_origin.z()=x;
		cf.parse(str,"selradius",m_myradius);
	}
	if (n!=0) { cf.parse(str,"cutoff",m_cutoff_distance);
	 SelectSegments(cf,str,n);   m_check_dist=true; }
	std::cout<<" Select params: m_dist" << m_dist.size()<<" origin "<<m_origin<<" radius "
	<< m_myradius<<" cutoff distance "<<m_cutoff_distance<<'\n';
	SelectFields(cf,str);

}	
template <class Ty,class Tx> void select_fields(const Ty & cf,const Tx & str)
{ SelectFields(cf,str); }
private:
template <class Ty,class Tx> void SelectFields(const Ty & cf,const Tx & str)
{
	greps::eqn_subs::cf_field_pairs(cf,str,m_field_vector,m_reject_vector,TgtTy());

std::cout<<" have criteria :"<<m_field_vector.size()<<"\n";

}
template <class Ty,class Tx> void SelectSegments(const Ty & cf,const Tx & str,const count_type n_)
{
count_type n=n_,i=1; 	
	PairTy pt=PairTy();
//	cf.parse(str,"segment_count",n);
	while (n)
	{	OsStreamTy os; //=OsStreamTy();
		os<<i;
		string_type nm="point"+os.str();
		cf.parse(str,nm,pt);
		coord_type cutoff=m_cutoff_distance;
		string_type nm2="cutoff"+os.str();
		cf.parse(str,nm2,cutoff);
//		fhgfhgfh
		m_dist.push_segment(pt[0],pt[1],cutoff);
		n--; i++;
	}
}
public:
bool fits_criteria( const TgtTy & p) const
{	
	 FieldItorTy ii=m_field_vector.begin(), ie=m_field_vector.end();
	while (ii!=ie)
	{
		if (p.field((*ii).first)!=(*ii).second) { return false;}
		++ii;
	}
	ii=m_reject_vector.begin(), ie=m_reject_vector.end();
	while (ii!=ie)
	{
		if (p.field((*ii).first)==(*ii).second) { return false;}
		++ii;
	}
	return true;
	/*
	count_type i=0;
// really slow but who cares...
	 FieldItorTy ii=m_field_vector.begin(), ie=m_field_vector.end();
	while (ii!=ie)
		{ if (((*ii).length()!=0)&&(p.field(i)!=(*ii) ))
		{ std::cout<<"reject ."<<p.field(i)<<".";
		 return false;} ++i;	++ii; }
	ii=m_reject_vector.begin(), ie=m_reject_vector.end();
	while (ii!=ie)
		{ if (((*ii).length()!=0)&&(p.field(i)==(*ii)) ) return false; ++i;	++ii; }
	return true;
	*/
	}
//void select_near(/*const coord_type cutoff_distance_,*/ const atom_list * s, atom_list *d) const

template <class Ty> void select_near( const Ty * s, Ty *d) const
{
//	const coord_type cutoff_distance=m_cutoff_distance;
//	TgtCItorTy	ii=s->begin(), ie=s->end();
	typename Ty::const_iterator ii=s->begin(), ie=s->end();
	while (ii!=ie)
	{
		bool b=m_check_dist;
		const bool cp=m_point_dist;
		if (b) b&=!m_dist.close_enuff( PointTy(*ii)); 
		//(m_dist.nearest(PointTy(*ii))>cutoff_distance); 		
		if (!b&&cp) b=((PointTy(*ii)-m_origin).length()<=m_myradius) ;
		//???
		if (b) {if (fits_criteria(*ii))  d->push_back((*ii));}
		/*
		if (!b) 
		{	if (cp) 
			{
				if ((PointTy(*ii)-m_origin).length()<=m_myradius) 
					d->push_back((*ii));   
			}
			else
				if (fits_criteria(*ii))  d->push_back((*ii));
		} */
	
		++ii;
	}
	return; 
}
template <class Ty> void select_near_2( const Ty * s, Ty *d) const
{
//	const coord_type cutoff_distance=m_cutoff_distance;
//	TgtCItorTy	ii=s->begin(), ie=s->end();
	typename Ty::const_iterator ii=s->begin(), ie=s->end();
	while (ii!=ie)
	{
//bool b=m_check_dist;
	//	const bool cp=m_point_dist;
	//	if (b) b&=!m_dist.close_enuff( PointTy(*ii)); 
		//(m_dist.nearest(PointTy(*ii))>cutoff_distance); 		
		bool  b=(((*ii)[0]-m_origin).length()<=m_myradius) ;
		//???
		if (b) // {if (fits_criteria(*ii))  
			d->xpush_back((*ii));//}
		/*
		if (!b) 
		{	if (cp) 
			{
				if ((PointTy(*ii)-m_origin).length()<=m_myradius) 
					d->push_back((*ii));   
			}
			else
				if (fits_criteria(*ii))  d->push_back((*ii));
		} */
	
		++ii;
	}
	return; 
}

private:
	
	DiTy m_dist;
	FieldTy	m_field_vector,m_reject_vector;
	bool m_check_dist;
	coord_type m_cutoff_distance;
	bool m_point_dist;
	PointTy m_origin;
	coord_type m_myradius; 
	
	
};



#endif
