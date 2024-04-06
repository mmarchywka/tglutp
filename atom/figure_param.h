#ifndef FIGURE_PARAM_H_H_
#define FIGURE_PARAM_H_H_

class figure_param 
{
	
	

typedef float distance_type;
typedef pdb_atom::coord_type coord_type;
typedef int count_type;

	typedef data_and_config::string_type string_type;
	public:
	figure_param(const coord_type pr,const coord_type nr, const count_type ci)
	: m_probe_radius(pr),m_neighborhood_radius(nr),m_center_idx(ci),m_shell(0),m_mp() {}
	
	template <class Ty,class Tx> figure_param(const Ty & cf,const Tx & str)
	:m_probe_radius(1),m_neighborhood_radius(1),m_center_idx(-1),m_shell(0),m_mp()
	{
		cf.parse(str,"probe_radius",m_probe_radius);
		cf.parse(str,"neighbor_radius",m_neighborhood_radius);
		cf.parse(str,"big_radius",m_neighborhood_radius);
		cf.parse(str,"center_index",m_center_idx);
		cf.parse(str,"center",m_mp);
		cf.parse(str,"make_shell",m_shell);
	}
	coord_type probe() const { return m_probe_radius; }
	coord_type neighbor() const { return m_neighborhood_radius; }
	count_type center() const { return m_center_idx; }
	myPoint3 & mp() { return m_mp; }
	count_type make_shell() const { return m_shell; }
	myPoint3 mp() const { return m_mp; }
	void load( coord_type & pr, coord_type & nr, count_type & ci)
	{ pr=m_probe_radius; nr=m_neighborhood_radius; ci=m_center_idx;}
private:
	coord_type m_probe_radius,m_neighborhood_radius;
	count_type m_center_idx,m_shell;
	myPoint3 m_mp;
	};

class   build_point_list_param//BuildPointListParam
{
	
	

typedef float distance_type;
typedef pdb_atom::coord_type coord_type;
typedef int count_type;

	typedef coord_type CT;
	typedef const CT CCT;
	public:
//	build_point_list_param(): neighborhood_radius(10), filter_radius(1),grid_space(.2f),
//	shell_thickness(.2f),safe_radius(0.0f) {}
	build_point_list_param(): neighborhood_radius(45), filter_radius(2),grid_space(.5f),
	shell_thickness(.15f),safe_radius(3.0f) {}
	
	build_point_list_param (CCT & nr, CCT&fr,CCT& gs, CCT&st,CCT&sr)
	: neighborhood_radius(nr), filter_radius(fr),grid_space(gs),
	shell_thickness(st),safe_radius(sr) {}
	template <class Ty,class Tx> build_point_list_param(const Ty & cf,const Tx & str)
	:neighborhood_radius(45), filter_radius(2),grid_space(.5f),
	shell_thickness(.15f),safe_radius(3.0f) 
	{
		cf.parse(str,"neighbor_radius",neighborhood_radius);
		cf.parse(str,"small_radius",neighborhood_radius);
		cf.parse(str,"probe_radius",filter_radius);
		//cf.parse(str,"ce_index",filter_radius);
		cf.parse(str,"grid_space",grid_space);
		cf.parse(str,"shell_thickness",shell_thickness);
		cf.parse(str,"safety_radius",safe_radius);
		
	}
	void load ( CT & nr, CT&fr,CT& gs, CT&st,CT&sr) const 
	{nr=neighborhood_radius; fr=filter_radius; 
	gs=grid_space; st=shell_thickness; sr=safe_radius;} 
	private:
	coord_type neighborhood_radius; //=10;
	coord_type filter_radius; // =1;
	coord_type grid_space; //.1f;
	coord_type shell_thickness; //.1f;
	coord_type safe_radius; // .5f;
}; 



#endif
