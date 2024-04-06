#ifndef POTENTIAL_PARAM_H_H_
#define POTENTIAL_PARAM_H_H_

class potential_param 
{
	
	
public:
typedef float distance_type;
typedef pdb_atom::coord_type coord_type;
typedef int count_type;

	typedef data_and_config::string_type string_type;
	public:
	potential_param(const coord_type pr,const coord_type nr, const count_type ci)
	: m_probe_radius(pr),m_ds(nr),m_dx1(0),m_dx2(0),m_p1(ci),m_p2(0),m_max_iterations(20),m_mp() {}
	
	template <class Ty,class Tx> potential_param(const Ty & cf,const Tx & str)
	:m_probe_radius(1),m_ds(1),m_dx1(0),m_dx2(0),m_p1(-1),m_p2(0),m_max_iterations(20),m_mp()
	{
		cf.parse(str,"target",m_probe_radius);
		cf.parse(str,"start",m_mp);
		cf.parse(str,"ds",m_ds);
		cf.parse(str,"dx",m_dx1);
		cf.parse(str,"dy",m_dx2);
		cf.parse(str,"points_i",m_p1);
		cf.parse(str,"points_j",m_p2);
		cf.parse(str,"imax", m_max_iterations);
	//	cf.parse(str,"make_shell",m_shell);
	}
	coord_type probe() const { return m_probe_radius; }
	coord_type ds() const { return m_ds; }
	coord_type dx1() const { return m_dx1; }
	coord_type dx2() const { return m_dx2; }
	count_type p1() const { return m_p1; }
	count_type p2() const { return m_p2; }
	count_type imax() const { return m_max_iterations; }
	myPoint3 & mp() { return m_mp; }
	myPoint3 mp() const { return m_mp; }
	
	template <class Os> void dump(Os& os) const
	{
		os << "{target"<<m_probe_radius<<" start "<<
		string_type(m_mp)<<    // it needs to call the right op but now I just need to 
								// make sure it is right.
		" ds="<<m_ds<<" dx="<<m_dx1<<" dy="<<m_dx2<<" points_i="<<m_p1<<
		" points_j="<<m_p2<<" imax="<< m_max_iterations<<"}\n";
		}
	
//	void load( coord_type & pr, coord_type & nr, count_type & ci)
//	{ pr=m_probe_radius; nr=m_neighborhood_radius; ci=m_center_idx;}
private:
	coord_type m_probe_radius,m_ds,m_dx1,m_dx2;
	count_type m_p1,m_p2,m_max_iterations;
	
	myPoint3 m_mp;
	};



#endif
