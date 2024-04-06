#ifndef VARIOUS_SEARCHES_H__
#define VARIOUS_SEARCHES_H__

#include "../debug_and_support.h"
#include <algorithm>
namespace searches
{

// Find something in a sorted list. The thing has a serial number function 
// and an array operator and size() function- but otherwise is arbitrary.
//typedef coord_type (TgtTy:: *p_type)() const;
/* from connectivity list: 
index_type FindNear(const index_type target)
{
index_type hi=m_atom_list.size()-1; index_type lo=0;
	index_type guess=target-1; index_type result=0;
	if ( target==0) return 0; // there is a problem with find serial "0" since it should be
				// starting at 1...
	if (guess>hi) guess=hi;
	index_type newguess=-1;
	while ( target!=(result=m_atom_list[guess].serial())) 
	{  
		//cout<<"guess:"<<target<<" "<<guess<<" "<<result<<" "<<hi<<" "<<lo<<"\n";
		
		if ( lo==hi) return -1;
		if ( result<target )lo=guess+1; 
		
		else hi=guess-1;  
	  newguess=result-target-guess;
	  if (newguess<lo) newguess=lo; if ( newguess>hi) newguess=hi;
	 if (guess==newguess)  return -1;
	 guess=newguess;
	}
	return guess;
}


*/
// return the range of pointers for lo<*itor<hi

template < class It, class Ty, typename Tc> void 
	bound(It & begin, It & end, const Ty& targ, const Tc f)
{	// The end is valid as long as there is at least one element in the list
	// on return, hih is no included in results and could be invalid. 
	It hih=end-1,loh=begin;
	//--hih;
	//mmdd::dout(MMDDLOC)<<"got here d="<<(hih-loh) <<"\n";
	//while (hih>loh)
	// ^^^^^^^^^^^^^^ this is the tentative offender
	while ((hih-loh)>0)
	{	// this / is the bias....
	//mmdd::dout(MMDDLOC)<<"got here d="<<(hih-loh) <<"\n";
		It midh=loh+((hih-loh)>>1);
		const Ty guess= (*midh)[0].x() ;//f(*midh);
//		const Ty guess= f(*midh);
		if (guess>targ) hih=midh-1;
		else if (guess<targ) loh=midh+1;
		// this is bad since it gives us no direction...
		// if we return now, the hi and lo could be quite broad.
		// a little cursing is in order, or at least recursing.
		else if (guess==targ){
			 It glo=midh;
			 bound(midh,hih,targ,f);
			 bound(loh, glo,targ,f);
			begin=loh; end=hih; // recurse should have added one for us
			return;
			}	
	}	
	// if *begin>target, the last iteration should have been
	// 
//	mmdd::dout(MMDDLOC)<<"got here d="<<(hih-loh) <<"\n";
	begin=loh; end=hih+1;
//	mmdd::dout(MMDDLOC)<<"got here d="<<(end-begin) <<"\n";
	return;
}

template < class It, class Ty,typename Tc> void 
	bound(It & begn, It & en, const Ty& targ_lo, const Ty & targ_hi, const Tc f)
{	
//	mmdd::dout(MMDDLOC)<<"got "<<(en-begn)<<"\n";
// 2024
	//It hih=en,loh=begn;
	It hih=en;
//mmdd::dout(MMDDLOC)<<"got here\n";
	// the end for this is the lo for the hi
	bound(begn,hih,targ_lo,f);
//	mmdd::dout(MMDDLOC)<<"got "<<(hih-begn)<<"\n";
//	mmdd::dout(MMDDLOC)<<"got here\n";
	// This isn't a lot of help since it is a log improvement...
	bound(hih,en,targ_hi,f);
	
	return;
/*	while (hih>loh)
	{
		It midh=loh+((hih-loh)>>1);
		const Ty guess=(*midh);
		if (guess>targ_hi) hih=midh-1;
		if (guess<targ_lo) loh=midh+1;
		if (guess==targ_lo){
	//		 It glo=guess;
//			 bound(guess,hih,targ);
	//		 bound(loh, glo,targ);
			begin=loh; end=hih;
			return;
			}	
	}	
	begin=loh; end=hih;
	return;  */
}

template < class index_type, class ListTy ,class Ty>
index_type  FindNear(const ListTy & li, const index_type target, index_type(Ty:: *Serial )() const)
{
//	typedef index_type(Ty:: *ftype )() ;
index_type hi=li.size()-1; index_type lo=0;
	index_type guess=target-1; index_type result=0;
	if ( target==0) return 0; // there is a problem with find serial "0" since it should be
				// starting at 1...
	if (guess>hi) guess=hi;
	index_type newguess=-1;
	while ( target!=(result=(li[guess].*Serial)())) 
	{  
		//cout<<"guess:"<<target<<" "<<guess<<" "<<result<<" "<<hi<<" "<<lo<<"\n";
		
		if ( lo==hi) return -1;
		if ( result<target )lo=guess+1; 
		
		else hi=guess-1;  
	  newguess=result-target-guess;
	  if (newguess<lo) newguess=lo; if ( newguess>hi) newguess=hi;
	 if (guess==newguess)  return -1;
	 guess=newguess;
	}
	return guess;
}
// from nurbs_test : order something using iterators - needs std::sort but already
// probably included...
/*
class Restrictor
{
	typedef Restrictor Myt;
	typedef nurbs_test OuterTy;
public:
typedef OuterTy::PtrItorTy PtrItorTy;
PtrItorTy m_xi,m_xf,m_yi,m_yf;
//bool m_sort;

typedef coord_type (TgtTy:: *p_type)() const;
 //Restrictor (bool sort):m_sort(sort) {}
 Restrictor () {}
 void init_itors(const PtrItorTy ini, const PtrItorTy fin)
 {m_xi=ini; m_xf=fin; }
 void init_itors(const Myt & x) {m_xi=x.m_yi; m_xf=x.m_yf; }
template <p_type pp,bool tp_sort,bool inc,class So > 
void restrict( const coord_type x, const coord_type radius,
PtrListTy & c_in, PtrListTy & c_out,  const So & cmp ) //const p_type pp)

{	c_out.clear();
	PtrItorTy x_end=c_in.end();
	const coord_type xi=x-radius; const coord_type xf=x+radius;
	while (m_xi!=x_end) { if(((**m_xi).*pp)()<xi) ++m_xi; else break; }
	PtrItorTy xx=inc?m_xi:m_xf;
	while (xx!=x_end) { if(((**xx).*pp)()<=xf) ++xx; else break; } m_xf=xx;
	for(xx=m_xi; xx!=m_xf; ++xx) c_out.push_back(*xx);
	if (tp_sort)
	{m_yi=c_out.begin(), m_yf=c_out.end(); 
	std::sort(m_yi,m_yf,cmp); m_yi=c_out.begin(); m_yf=c_out.end();}
	//cout<<"x list is "<<m_composite_x.size()<<" vs z of "<<m_composite.size()<<"\n";
}

}; //restrictor

//m_composite_y.begin(), ye=m_composite_y.end(); 
	//	pdb_atom::compare_distance::center(*ii);
		
	//	std::sort(yi,ye,pdb_atom::compare_distance());
	//	yi=m_composite_y.begin(), ye=m_composite_y.end();
		//ry.restrict<&TgtTy::y,pdb_atom::compare_distance,true>
		 //(true,(**ii).y(),radius,m_composite_x,m_composite_y,&TgtTy::y);
		//PtrItorTy & yi = ry.m_yi, &ye=ry.m_yf;

*/


template <class coord_type, class PtrItorTy,class TgtTy,class PtrListTy> class Restrictor
{
	typedef Restrictor Myt;
//	typedef nurbs_test OuterTy;
public:
//typedef OuterTy::PtrItorTy PtrItorTy;
PtrItorTy m_xi,m_xf,m_yi,m_yf;
//bool m_sort;

typedef coord_type (TgtTy:: *p_type)() const;
 //Restrictor (bool sort):m_sort(sort) {}
 Restrictor () {}
 void init_itors(const PtrItorTy ini, const PtrItorTy fin)
 {m_xi=ini; m_xf=fin; }
 void init_itors(const Myt & x) {m_xi=x.m_yi; m_xf=x.m_yf; }
template <p_type pp,bool tp_sort,bool inc,class So > 
void restrict( const coord_type x, const coord_type radius,
PtrListTy & c_in, PtrListTy & c_out,  const So & cmp ) //const p_type pp)

{	c_out.clear();
	PtrItorTy x_end=c_in.end();
	const coord_type xi=x-radius; const coord_type xf=x+radius;
	while (m_xi!=x_end) { if(((**m_xi).*pp)()<xi) ++m_xi; else break; }
	PtrItorTy xx=inc?m_xi:m_xf;
	while (xx!=x_end) { if(((**xx).*pp)()<=xf) ++xx; else break; } m_xf=xx;
	for(xx=m_xi; xx!=m_xf; ++xx) c_out.push_back(*xx);
	m_yi=c_out.begin(), m_yf=c_out.end(); 
	if (tp_sort)
	{
	std::sort(m_yi,m_yf,cmp); m_yi=c_out.begin(); m_yf=c_out.end();}
	//cout<<"x list is "<<m_composite_x.size()<<" vs z of "<<m_composite.size()<<"\n";
}

}; //restrictor











};



#endif
