#ifndef BUILD_NEIGHBOR_MAP_H__
#define BUILD_NEIGHBOR_MAP_H__

/*
void BuildPointListMap( pdb_atom& probe
	,const build_point_list_param &bplp= build_point_list_param())
{
	coord_type neighborhood_radius,filter_radius, grid_space, shell_thickness,safe_radius;
	bplp.load(neighborhood_radius,filter_radius, grid_space, shell_thickness,safe_radius);
	coord_type cx=0, cy=0,cz=0;
	cx=probe.x(); cy=probe.y(); cz=probe.z(); 
	probe.radius(filter_radius);
	probe.highlight(true); probe.show_point(true); 
	coord_type test_grid_radius=neighborhood_radius-probe.radius()-safe_radius;
	mmdd::dout(MMDDLOC)<<"test grid radius is "<<test_grid_radius<<"\n";
	InitialRestrict(-test_grid_radius+cz,safe_radius+probe.radius());
	coord_type x=0; coord_type y=0; coord_type z=0;
	coord_type tgr2=test_grid_radius*test_grid_radius; 
	count_type reject=0; 
	coord_type naive_delta=grid_space;
	for (z=-test_grid_radius; z<test_grid_radius; z+=grid_space)
	{coord_type gr2=(tgr2-z*z); coord_type grid_rho=sqrt(gr2);
	cout<< "z :"<<z<<" of "<< reject
	<<" map="<< m_test_map.size()   <<"z-list "<<m_composite.size()<<" \n"; cout.flush();
	Restrictx(false,cx-grid_rho,safe_radius+probe.radius()); 
	for (x=-grid_rho; x<grid_rho; x+=grid_space)
	{
	coord_type ylim=sqrt(gr2-x*x);
	Restricty(false,cy-ylim,safe_radius+probe.radius()); 
	for (y=-ylim; y<ylim;) // y-=grid_space)
	{	probe.move_to(x+cx,y+cy,z+cz); 
	if (FitCheckShell(m_composite_y,probe,shell_thickness,naive_delta)) 
	{ //probe.highlight(true); probe.show_point(true); 
	//probe.oriented_toward(*m_closest_hit,probe.radius());
	//probe.color(*m_closest_hit);dest.push_back(probe);
	m_test_map[m_closest_hit]++;
	} else {++reject;}
	 y+=grid_space;
	Restricty(true,cy+y,safe_radius+probe.radius()); 
	} 
	Restrictx(true,cx+x+grid_space,safe_radius+probe.radius()); 
	} //x
	IncrementalRestrict(z+cz+grid_space,safe_radius+probe.radius());
	} //z
	
}

inline bool FitCheckShell(const PtrListTy & al, const pdb_atom & probe, const coord_type sh, coord_type & naive_delta)
{	coord_type cl=1000000;
	for ( PtrCItorTy i=al.begin(); i!=al.end(); ++i)
	{ // The minimum amount worth checking again on failure is -c(>0)	
		coord_type c=probe.clearance(**i); if (c<0) { naive_delta=-c; return false;	}
		if (c<cl) {cl=c;  m_closest_hit=(*i); }
		}
	// if we aren't close enough, the minimum amount worth trying again
	// is cl(>0)-sh
	naive_delta=cl-sh;
	return (cl<sh);;
} 
*/
#endif
