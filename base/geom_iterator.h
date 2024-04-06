#ifndef GEOM_ITERATOR_H__
#define GEOM_ITERATOR_H__

#include "../base/points_base_classes.h"
#include <iostream>

// This is a lot more regular than it looks - it would be really nice if
// the compile

template <class UTy > class geom_iterator
{
	typedef geom_iterator Myt;
	typedef UTy user_type;
	typedef int count_type;
	typedef float coord_type;
	typedef point3_template<coord_type> point_type;
	typedef point_type PointTy;

public:
	typedef  PointTy& ( user_type:: *CallTy)(PointTy &);
	typedef void (user_type:: *NotifyTy)(Myt & );
public:
	geom_iterator( user_type * u, CallTy f,
	const PointTy & dx1, const PointTy & dx2,
	const count_type i, const count_type j):
	m_user(u),m_converge(f),m_row(0),m_quad(0),
	// dx1 and dx2 are the scan directions. dx1 is crossed with grad
	// at each point and the norm is set to the length of m_dx1.
	// Simiarly for dx2 at the end of each scan line along dx1
	m_dx1(dx1), m_dx2(dx2),m_d1(m_dx1.length()),m_d2(m_dx2.length()),
	// I and J are the number of points in first and second direction
	m_I(i),m_J(j),m_i(0),m_j(0),m_q(0),
	// loc is current location with ini is start of line info.
	m_loc(),m_loc_ini(),m_loc_zero(), m_valid(false) {}
//	
	template <typename Tf> //template <Tf func> 
		void start( Tf row,Tf quad,PointTy& loc) {m_i=0; m_j=0;  m_q=0;
		m_valid=true;
		m_row=row; m_quad=quad;
	//	 move to a valid point...
		m_grad_zero=m_grad_ini=(m_user->*m_converge)(loc);
		m_loc_zero=m_loc_ini=m_loc=loc;
	}
	const count_type column() const { return m_i;}
	const count_type row()const { return m_j;}
	const count_type quad() const { return m_q; } 
	const PointTy & loc() const { return m_loc;}
	const PointTy & loc_ini() const { return m_loc_ini; }
	const PointTy & operator *() { return m_loc; }
	inline Myt & operator+=( PointTy & grad) 
	{ 	
		++m_i; if (m_i==m_I){ Inc1(grad); return *this; }
		
		PointTy dl= m_dx1.cross(grad);
		if (!dl.normalize_if())
			{std::cout<<"cant normalize "<<grad<<"X"<<m_dx1<<"\n";}
		m_loc+=(dl*m_d1);	
		grad=(m_user->*m_converge)(m_loc);
		return *this;
	}
	
	bool valid() const { return m_valid; }
	operator bool() const { return m_valid; }

private:
	void Inc1( PointTy & grad) 
	{ // need to move back to a converged location near set point. 
		if (m_row!=0) { (m_user->*m_row)(*this); }
		m_i=0; ++m_j;
		if (m_j==m_J){  Inc2(grad);  return; }
		PointTy dir2=m_dx2.cross(m_grad_ini); 
		if (!dir2.normalize_if()) 
			{std::cout<<"cant normalize "<<grad<<"Y"<<m_dx2<<"\n";}
		dir2=dir2*m_d2;
		m_loc_ini+=dir2;
		grad=m_grad_ini=(m_user->*m_converge) (m_loc_ini);
		m_loc=m_loc_ini;
		
	}
void  Inc2(PointTy &grad )
{
	if (m_quad!=0) { (m_user->*m_quad)(*this); }
	m_j=0; m_loc=m_loc_zero; grad=m_grad_zero;
	if ((m_q&1)==0) m_d1*=(-1.0f);else m_d2*=(-1.0f);
	if (m_q==3) { m_valid=false; return ; }	
	grad=m_grad_ini=(m_user->*m_converge)(m_loc);
	m_loc_ini=m_loc;
	++m_q;	
}

	user_type * const  m_user;
	CallTy  const m_converge;
	NotifyTy m_row,m_quad;
	const PointTy m_dx1,m_dx2;
	coord_type m_d1,m_d2;
	const count_type m_I,m_J;
	count_type m_i,m_j,m_q;
	PointTy m_loc,m_loc_ini,m_loc_zero,m_grad_ini,m_grad_zero;
	bool m_valid;
};


#endif
