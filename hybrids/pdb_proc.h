#ifndef PDB_PROC_H___
#define PDB_PROC_H___

#include "global.h"

#include "find_surface_molecules.h"
#include "point_list_stuff.h" 



#include "hybrid_pdb_reader.h"
//#include "hybrid_expression.h"
//#include <math.h>

#include <vector>
//#include <map>


class pdb_proc : public pdb_typedefs
{
	typedef pdb_proc Myt;
	typedef point_list_operations PloTy;
	typedef point_list PlTy;
	typedef std::vector<PlTy> MolColTy;
	typedef find_surface_molecules<PlTy,pdb_typedefs>  SuMoTy;
	typedef double Fl; 
public:	
	pdb_proc(): m_probe_radius(0),m_visible(0),m_dmax(0), m_flags(0),m_pfx(0),
	m_a(0),m_b(0),m_c(0),m_x(0),m_y(0),m_z(0),m_shake(0),m_enclave_format(1)
	{m_plo.m_own=false; m_sumo.m_own=false; }
	int main ( int argc, char ** argv);


private:

	string_type m_pdb, m_color_file, m_radius_file,m_name;
	// probe radius and fraction visisble to be a surface molecule. 
	float m_probe_radius,m_visible,m_dmax;
	IdxTy m_flags;
	MolColTy m_mols;
	PloTy m_plo;
	SuMoTy m_sumo;
	Chr * m_pfx;
	// parameters for rotate ( Euler angles ) 
	// and center or rotation. 
	Fl m_a,m_b,m_c,m_x,m_y,m_z;
	float m_shake;
	IdxTy m_enclave_format;
	// arrg, this needs to delegate now... 
api_type SetDebug(Chr * const nm) 
{
	m_debug=atoi(nm);
//	m_mols.m_debug=m_debug; 
	m_plo.m_debug=m_debug; m_sumo.m_debug=m_debug;
	return 0;	

}
api_type SetOut(Chr * const nm) {open_out(nm,m_out); 
m_plo.m_out=m_out; m_sumo.m_out=m_out; return 0;	}
api_type SetErr(Chr * const nm) {open_out(nm,m_err); 
m_plo.m_err=m_err; m_sumo.m_err=m_err;   return 0;	}

api_type LoadPdb(Chr * const nm)
{	
	PlTy mol;
	hybrid_pdb_reader hpr;
	hpr.load(m_color_file.c_str(),m_radius_file.c_str());
	IsTy * is=open_in(nm);
	if (is==0) return -1; 
	hpr.load_list(mol,*is);
	delete is;  
	m_mols.push_back(mol);
	return 0;
}
api_type SetColors(Chr * const nm) {m_color_file=nm; return 0; }
api_type SetRadius(Chr * const nm) {m_radius_file=nm;return 0;	}

api_type ShoVis(Chr * const nm) 
{
	float v=atof(nm);
	m_sumo.get(dos(), v);
	return 0;	
}
api_type ShoMesh(Chr * const nm) 
{
	float v=atof(nm);
	m_sumo.mesh(dos(),v,m_dmax);
	return 0;	
}
api_type ShoNorm(Chr * const nm) 
{
	float v=atof(nm);
	m_sumo.norm(dos(),v,m_dmax);
	return 0;	
}
api_type ShoGrid(Chr * const nm) 
{
	float v=atof(nm);
	m_sumo.roll_grid(dos(),v,m_flags);
	return 0;	
}
api_type ShoEdges(Chr * const nm) 
{
	float v=atof(nm);
	m_sumo.sort_edges(dos(),v,m_flags);
	return 0;	
}
api_type SetPfx(Chr * const nm) 
{
	m_pfx=nm;
	return 0;	
}
api_type DumpEnclaves(Chr * const nm) 
{
	float v=atof(nm);
	// added m_enclave foamt, 1 is old atom thing, 2 is new residue code
	m_sumo.make_enclaves(dos(),v,m_flags,(m_pfx==0?" ":m_pfx),m_enclave_format);
	return 0;	
}
api_type DoSurface(Chr * const nm) 
{
	const IdxTy d=atoi(nm);
	const IdxTy sz=m_mols.size();
	const IdxTy i=sz-d-1;
	if ( sz<=d)
	{ eos()<<MM_MARK<<" molecule stack size "<<sz<<" too small for operand "<<d<<'\n'; return -1; }
	//m_plo.catalog(m_mols[sz-d-1]); 
	if (debug()) eos()<<MM_MARK<<" Finding surface for  "<<m_mols[i].name()
		<<" at radius "<<m_probe_radius <<'\n'; 
	m_sumo.find(m_mols[i],m_probe_radius);
	
//	m_sumo.get(dos(), m_visible);
	return 0;	

}
api_type DoCatalog(Chr * const nm) 
{
	const IdxTy d=atoi(nm);
	const IdxTy sz=m_mols.size();
	if ( sz<=d)
	{ eos()<<MM_MARK<<" molecule stack size "<<sz<<" too small for operand "<<d<<'\n'; return -1; }
	m_plo.catalog(m_mols[sz-d-1]); 
	return 0;	

}
api_type DoList(Chr * const nm) 
{
	const IdxTy d=atoi(nm);
	const IdxTy sz=m_mols.size();
	if ( sz<=d)
	{ eos()<<MM_MARK<<" molecule stack size "<<sz<<" too small for operand "<<d<<'\n'; return -1; }
	m_plo.list(dos(), m_mols[sz-d-1]); 
	return 0;	

}
api_type DoMake(Chr * const nm) 
{
	const IdxTy d=atoi(nm);
	const IdxTy sz=m_mols.size();
	PlTy mol;
	if ( sz<=d)
	{ eos()<<MM_MARK<<" molecule stack size "<<sz<<" too small for operand "<<d<<'\n'; return -1; }
	m_plo.assign_some(mol,m_mols[sz-d-1]); 
	m_mols.push_back(mol); 
	if (debug()) eos()<<MM_MARK<<" Added "<<mol.name()<<" of size "<<mol.size()<< " to pdb stack size "<<m_mols.size()<<'\n'; 
	return 0;	

}

//Pl::mutate(m_s,s,expand_radius(probe_radius));
api_type DoMutate(Chr * const nm) 
{
	const IdxTy d=atoi(nm);
	const IdxTy sz=m_mols.size();
	PlTy mol;
	//double dx=0,dy=0,dz=0,a=0,b=0,c=0;
	if ( sz<=d)
	{ eos()<<MM_MARK<<" molecule stack size "<<sz<<" too small for operand "<<d<<'\n'; return -1; }
	move_around ma(m_x,m_y,m_z,m_a,m_b,m_c);
	ma.set_shake(m_shake);
	mol.mutate(mol,m_mols[sz-d-1],ma); 
	m_mols.push_back(mol); 
	if (debug()) eos()<<MM_MARK<<" Added "<<mol.name()<<" of size "<<mol.size()<< " to pdb stack size "<<m_mols.size()<<'\n'; 
	return 0;	

}
api_type DoDrop(Chr * const nm) 
{
	IdxTy d=atoi(nm);
	const IdxTy sz=m_mols.size();
	if (( sz<d)||(d<1)) {d=sz;}
	while (d>0) { m_mols.pop_back();  --d; } 
	if (debug()) eos()<<MM_MARK<<" Dropped "<<d<<" from pdb stack leaving"<<m_mols.size()<<'\n'; 
	return 0;	

}
template <class Os> static void rhelp( Os& os)
{
	os<<Ldr(0)<<"Usage: prb_proc" <<Trl();
	os<<Ldr(1)<<"Utility to load and examine spatial reslationships in pdb files." <<Trl();	
	os<<Ldr(1)<<"-pdb nm : push another pdf file onto stack " <<Trl();
	os<<Ldr(1)<<"-color_file fn : use fn for subsequent color maps" <<Trl();
	os<<Ldr(1)<<"-radius_file fn : use fn for subsequent radii maps" <<Trl();
	os<<Ldr(1)<<"-cat n : show stats for pdb that is n from top of stack" <<Trl();
	os<<Ldr(1)<<"-list n : dump pdb that is n from top of stack, prf with RAW" <<Trl();
	os<<Ldr(1)<<"-drop n : pop the n top stack elements " <<Trl();
	os<<Ldr(1)<<"-make n: apply current filter to n-th from top and push on stack" <<Trl();
	os<<Ldr(1)<<"-mutate n: apply mutation to n-th from top and push on stack" <<Trl();
	os<<Ldr(1)<<"-center x y z: set rotation center for mutate command." <<Trl();
	os<<Ldr(1)<<"-shake a: add a random component of max a to mutated coords" <<Trl();
	os<<Ldr(1)<<"-angles a b c : set Euler rotation angles for mutate command." <<Trl();
	os<<Ldr(1)<<"-add key value: restrict key{type,atom,residue} to NOT value, cumulative" <<Trl();
	os<<Ldr(1)<<"-probe_raidus r: radius of probe to fund surfaces " <<Trl();
	os<<Ldr(1)<<"-visible f: faction visible to be surface atom" <<Trl();
	os<<Ldr(1)<<"-mesh t: output last mesh results at threshold t" <<Trl();
	os<<Ldr(1)<<"-norm t: output last normals results at threshold t" <<Trl();
	os<<Ldr(1)<<"-grid v:  output surface patches using flags as follows " <<Trl();
	os<<Ldr(1)<<"-edge v:  ... " <<Trl();
	os<<Ldr(1)<<"-enclaves n: ... " <<Trl();
	os<<Ldr(2)<<"flags:(1)output each exposed patch (2) output whole atom "<<Trl();
	os<<Ldr(2)<<"if more than v patches (4) use radius of atom, not atom+probe " <<Trl();
	os<<Ldr(1)<<"-dmax p: set dmax value used variously by some following commands" <<Trl();
	os<<Ldr(1)<<"-flags n: set flags variable for use by following commands" <<Trl();
	os<<Ldr(1)<<"-out dest: subsequent data to dest, - for console" <<Trl();
	os<<Ldr(1)<<"-err dest: subsequent errors to dest, - for console" <<Trl();
	os<<Ldr(1)<<"-interactive : type in lines until null " <<Trl();
	os<<Ldr(1)<<"-v mode : set verbosity level, 0 for quit. This differs from other code..." <<Trl();
		
		
}
template <class Os> static void credit_and_ref( Os& os)
{// nominally for human consumption but not formatted, should parse ok 
os<<Ldr(0)<<MM_MARK<<" Built on "<<__DATE__<<" at "<<__TIME__<<Trl();
os<<Ldr(0)<<"Contact: Mike Marchywka, marchywka@hotmail.com" <<Trl();
}
	
};


#endif





