#include "atoms.h"


typedef atom_list_proximity MyClass;


MyClass::Myt &  MyClass::figure( const FpTy & fp, const BPLPTy &bplp,AtomLiTy & dest)
{
mmdd::dout(MMDDLOC)<<"start figure with list = "<<m_list.size()<<"   \n";
	coord_type probe_radius=fp.probe(); //2;
	coord_type neighborhood_radius=fp.neighbor(); //6; 
	count_type center_idx=fp.center(); // 750;
	pdb_atom probe=pdb_atom(0,0,0,probe_radius);
	if (center_idx>=0)
	{
		cout<<m_list[center_idx].m_desc<<"\n";
		const pdb_atom& center=m_list[center_idx];
	
		probe.place(center);
		probe.show(cout,1," probe cnetered"); 
		MakeTestList(center,neighborhood_radius);
		MinimizeAgainstTestList(probe,center, probe.radius()+center.radius());
	}
	else {probe.move_to(fp.mp()); MakeTestList(probe,neighborhood_radius);}
	
	pdb_atom newcenter=probe;
	probe.color(1,1,1);
	m_cavity_list.push_back(probe);
	typedef search_strategy SSTy;
	SSTy ss= SSTy();
	ss.LoadLists(m_test_list); 
	if(fp.make_shell()!=0)ss.BuildPointList(probe,  dest ,bplp);
	if(fp.make_shell()==0) ss.BuildPointListMap(probe,bplp);
	// use cavity list...
	ss.get_boundary(m_cavity_list); 
return *this;
}

//MyClass::AtomLiTy & MyClass::answer() { return m_cavity_list; }	

MyClass::coord_type MyClass::MinimizeAgainstTestList(pdb_atom &probe, const pdb_atom & center, const coord_type r)
{
	coord_type dtheta=.1; coord_type dphi=0;
	coord_type tt=0;
	count_type i=0;
	center.show(cout,1,"center energy");
	while (i<10000)
	{
	probe.set_distance(center,r+.00001);
	coord_type energy=Energy(probe);
	if (energy>0) return energy;
	probe.coords(center);
	tt+=dtheta; if( tt>2*3.14) { tt=0; dphi=.1;} 
	probe.orbit_approx(center, 0,dtheta,dphi);
//	probe.show(cout,1,"moved to");
	++i;
	}
	return -1; 
	}
//bool ContinueProbing() {  return (m_cavity_list.size()<50); }
MyClass::coord_type MyClass::Energy(const pdb_atom & probe)
{
	coord_type V=0; 
	for ( AtomLiTyItorConst i=m_test_list.begin(); i!=m_test_list.end(); ++i)
	{ coord_type c=(*i).clearance(probe);
		if (c<0) return -1;
		V+=coord_type(1.0)/c;	}
	return V;
	
} 

MyClass::count_type MyClass::MakeTestList(const pdb_atom& center, const coord_type r)
{
	count_type j=0; 
	for ( AtomLiTyItorConst i=m_list.begin(); i!=m_list.end(); ++i)
	{
		coord_type rtest=(*i).dist(center);
		if (((*i).field(0)=="ATOM")&&(rtest<=r)&&(rtest!=0)) {
			 // need a new list type 
			 
			 m_test_list.push_back((*i)); ++j;
	//	(*i).show(cout,1,"added to test list");
		}
	}
	return j;
}
