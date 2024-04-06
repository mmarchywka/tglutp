#include "atom_lists.h"
#include "./view/glut_commons.h"
#include "./view/glut_picking.h"

#include <list>
namespace {GLUquadricObj* xxx=gluNewQuadric();};


atom_io * atom_io::r_one=NULL; 


void atom_io::model_draw(display_tree& root)
{	root.m_idx_min=0;root.m_idx_max=0;
	atom_lists.draw(*root[0]);
	conn_lists.draw(*root[1]);
	point_lists.draw(*root[2]);
	}

void atom_io::drawSphereList()
{
if (r_one)r_one->model_draw(r_one->m_root);
else std::cout<<" The atom_io drawer is null- need an instance to draw\n";

}


void atom_io::drawSelectionSphereList(int x, int y)
{
	glut_picking<50000> gp;
 //	glPushMatrix(); //???
 	gp.startPicking(x,y);
	glInitNames();
	const string_type cmd="pick";
	string_type cr="1";
	if (!r_one)  std::cout<<" The atom_io drawer is null- need an instance to draw\n";
	display_tree & dt=r_one->m_root;
	dt.ssets(cmd,cr,2); // ever hear of an enum?
	r_one->model_draw(r_one->m_root);
   	// compiler never said that "true" didn't match :)
   	cr="0"; dt.ssets(cmd,cr,2);
   	gp.stopPicking();
   	if (gp.size())
   	{
   		DaCoTy::OsStreamTy os;
   		int i=gp.nearest();
   		if (i>=0){
   	//		std::cout<<"looking for "<<i<<"\n";
//   		typedef list<pdb_atom const* > HitTy;
   		typedef list<pdb_atom > HitTy;
//   		pdb_atom const* x=NULL; 
  		HitTy x;
   		dt.find_picked(i,x,os);
   		string_type loc_id=os.str();
   		DaCoTy::ConvStreamTy is(os.str());
   		r_one->model_find(r_one->m_root,i,x,is);
		HitTy::const_iterator ii=x.begin(),ie=x.end();
		while (ii!=ie) { 
			const pdb_atom &y=(*ii);
			pdb_atom::pdb_entry nm=y.desc();
			if (nm.length()>1) cout<<nm<<"\n"; 
			else std::cout<<"unknown "<<loc_id <<" at"<<y.x()<<" "<<y.y()<<" "<<
			y.z()<<" r="<<y.radius()<<"\n";
			++ii; 
			r_one->m_pick_history.push_back(y); // this is a memory leak- no pruning.
		}
		if (x.size()==2)
		{	ii=x.begin();
			const pdb_atom &y=(*ii); ++ii;
			const pdb_atom &z=(*ii);
			myPoint3 d=myPoint3(y)-myPoint3(z);
			std::cout<<"dx="<<d.x()<<" dy="<<d.y()<<" dz="<<d.z()
			<<" d="<<y.dist(z)<<" olap="<<y.clearance(z)<<"\n";
			
		}
 //  		std::cout<<"looked for "<<i<<" on branch"<<os.str()<<"\n"; 
   	//	if (x) std::cout<< x->desc()<<"\n";
   	//	else std::cout<<"had "<<gp.size()<<" hits but nothing found\n";
   		} else std::cout<<"nearest is not right :"<<i<<"\n";
   	} else std::cout<<"No matching hits\n";
   	
}
