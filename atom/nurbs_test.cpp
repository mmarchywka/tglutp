#include "nurbs_test.h"

typedef nurbs_test MyClass;



void MyClass::LoadLists(const TgtListTy & li)
{
for (TgtCItorTy p=li.begin(); p!=li.end(); ++p)
{  TgtPtr pp=&(*p); m_x.push_back(pp); }
m_y=m_x; m_z=m_x;
PtrItorTy ii=m_x.begin(); PtrItorTy jj=m_x.end();
mmdd::dout(MMDDLOC)<<"Sorting x list "<<m_x.size()<<"\n";
std::sort(ii,jj,compare_x());
mmdd::dout(MMDDLOC)<<"Sorting y list\n";
ii=m_y.begin();  jj=m_y.end(); std::sort(ii,jj,compare_y());
mmdd::dout(MMDDLOC)<<"Sorting z list\n";
ii=m_z.begin();  jj=m_z.end(); std::sort(ii,jj,compare_z());
}


void MyClass::add_triangles( connectivity_list * cage, const coord_type rad, const bool debuggin)
{
coord_type radius=rad; //3.0f;
	Restrictor rz,rx,ry;
	PtrItorTy ii=m_z.begin(), ie=m_z.end(); // for each along z
	cout<<"AddNay1\n"; cout.flush();
	if (ii==ie) return;
	rz.init_itors(ii,ie);
	rz.restrict<&TgtTy::z,true,true>((**ii).z(),radius,m_z,m_composite,compare_x());
	//cout<<"AddNay1  "<<m_composite.size()  <<"\n"; cout.flush();
	while (ii!=ie)
	{	// This is not real efficient in this mode but seems to work ok and
		// test the idea... 
		rx.init_itors(rz);
		rx.restrict<&TgtTy::x,true,true>( (**ii).x(),radius,m_composite,m_composite_x,compare_y());
		ry.init_itors(rx);
		// template parameter, need function param...
	//	pdb_atom::compare_distance::center(*ii);
		pdb_atom::compare_distance::center(*ii);
		pdb_atom::compare_distance fack;
		ry.restrict<&TgtTy::y,true,true>
		 ((**ii).y(),radius,m_composite_x,m_composite_y,fack);
	// 2024	
		//PtrItorTy yi=ry.m_yi, ye=ry.m_yf; 
		PtrItorTy yi=ry.m_yi; // , ye=ry.m_yf; 
//		std::sort(yi,ye,fack);
	//	std::sort(yi,ye,pdb_atom::compare_distance());
//		yi=m_composite_y.begin(), ye=m_composite_y.end();
		count_type n=(m_composite_y.size()-1);
		   ++yi; //self
		  while (n>1) {  
			coord_type rreal=(**ii).dist((**yi));
			count_type lim=4;
			if (((lim--)>0)&&(rreal!=0)&&(rreal<=radius)) 
			{	
				count_type v2=(**(yi++)).serial();
				cage->add_bond((**ii).serial(),v2,
			                         (**(yi++)).serial()     );  }
			else {if (debuggin) {cout<<rreal<<" "; (**ii).dump_loc(cout); 
			cout<<" vs "; (**yi).dump_loc(cout); cout<<"\n"; }}	 
		n-=2;		
		} // >2,n
	//cout<<"AddNay4  "<<m_composite_x.size()  <<"\n"; cout.flush();
	++ii;
	if (ii!=ie) rz.restrict<&TgtTy::z,true,false>((**ii).z(),radius,m_z,m_composite,compare_x());
	} 
}
void MyClass::add_neighbors( connectivity_list * cage, const coord_type rad, const bool debuggin)
{
	coord_type radius=rad; //3.0f;
	Restrictor rz,rx,ry;
	PtrItorTy ii=m_z.begin(), ie=m_z.end(); // for each along z
	cout<<"AddNay1\n"; cout.flush();
	if (ii==ie) return;
	rz.init_itors(ii,ie);
	rz.restrict<&TgtTy::z,true,true>((**ii).z(),radius,m_z,m_composite,compare_x());
	//cout<<"AddNay1  "<<m_composite.size()  <<"\n"; cout.flush();
	while (ii!=ie)
	{	// This is not real efficient in this mode but seems to work ok and
		// test the idea... 
		rx.init_itors(rz);
		rx.restrict<&TgtTy::x,true,true>( (**ii).x(),radius,m_composite,m_composite_x,compare_y());
		ry.init_itors(rx);
		pdb_atom::compare_distance::center(*ii);
		pdb_atom::compare_distance fack;
		ry.restrict<&TgtTy::y,true,true>
		((**ii).y(),radius,m_composite_x,m_composite_y,fack);
		PtrItorTy yi=ry.m_yi,ye=ry.m_yf; //
		if (yi!=ye) ++yi; // get rid of center...
		count_type most_neighbors=4;
		count_type neh=0;
		while ((neh<most_neighbors)&&(yi!=ye)) {
			coord_type rreal=(**ii).dist((**yi));
			if ((rreal!=0)&&(rreal<=radius)) 
			{++neh;	cage->add_bond((**ii).serial(),(**yi).serial());  }
			else {
			if (debuggin) {cout<<rreal<<" "; (**ii).dump_loc(cout); 
			cout<<" vs "; (**yi).dump_loc(cout); cout<<"\n"; }
				
				}	
				++yi; }
	//cout<<"AddNay4  "<<m_composite_x.size()  <<"\n"; cout.flush();
	++ii;
	if (ii!=ie) rz.restrict<&TgtTy::z,true,false>
		( (**ii).z(),radius,m_z,m_composite,compare_x());
	} 
}



//void add_x_neighbors( connectivity_list * cage, const coord_type rad, const bool debuggin)
//{ add_something<&Myt::add_something_delegate>(cage,rad,debuggin); 	 }
//typedef void (Myt:: *DelTy) (connectivity_list *,PtrItorTy&,PtrItorTy,PtrItorTy,const coord_type, const bool);

/*
void add_(connectivity_list * cage,const coord_type radius,const bool debuggin)
{
	typedef rti<coord_type,PtrItorTy,TgtTy,PtrListTy> 
		RTItor;
		// find a list of atoms restricted to radius around each of the 
		// ordered elements in m_z
		RTItor ri(m_z,radius);
	while(ri.valid())
	{
	PtrItorTy &yi=ri.begin();	
	PtrItorTy & ye=ri.end();
	const TgtTy & atom=ri.atom();
	if (yi!=ye) ++yi; // get rid of center...
	count_type most_neighbors=4;
	count_type neh=0;
	while ((neh<most_neighbors)&&(yi!=ye)) {
		coord_type rreal=atom.dist((**yi));
		if ((rreal!=0)&&(rreal<=radius)) 
		{++neh;	cage->add_bond(atom.serial(),(**yi).serial());  }
		else { if (debuggin) {cout<<rreal<<" "; atom.dump_loc(cout); 
		cout<<" vs "; (**yi).dump_loc(cout); cout<<"\n"; }	
				} //else	
				++yi; 
		}
		}

	ri.inc(); 
	}

*/
/*
template < DelTy Delegate>
void add_something( connectivity_list * cage, const coord_type rad, const bool debuggin)
{
	coord_type radius=rad; //3.0f;
	Restrictor rz,rx,ry;
	PtrItorTy ii=m_z.begin(), ie=m_z.end(); // for each along z
	cout<<"AddNay1\n"; cout.flush();
	if (ii==ie) return;
	rz.init_itors(ii,ie);
	rz.restrict<&TgtTy::z,true,true>((**ii).z(),radius,m_z,m_composite,compare_x());
	//cout<<"AddNay1  "<<m_composite.size()  <<"\n"; cout.flush();
	while (ii!=ie)
	{	// This is not real efficient in this mode but seems to work ok and
		// test the idea... 
		rx.init_itors(rz);
		rx.restrict<&TgtTy::x,true,true>( (**ii).x(),radius,m_composite,m_composite_x,compare_y());
		ry.init_itors(rx);
		pdb_atom::compare_distance::center(*ii);
		pdb_atom::compare_distance fack;
		ry.restrict<&TgtTy::y,true,true>
		((**ii).y(),radius,m_composite_x,m_composite_y,fack);
		
		(this->*Delegate)(cage,ii,ry.m_yi,ry.m_yf,radius,debuggin);
	//	PtrItorTy yi=ry.m_yi,ye=ry.m_yf; //
	//(this->*Delegate)(cage,ii,yi,ye,radius,debuggin);
	//	if (yi!=ye) ++yi; // get rid of center...
	//	count_type most_neighbors=4;
	//	count_type neh=0;
	//	while ((neh<most_neighbors)&&(yi!=ye)) {
	//		coord_type rreal=(**ii).dist((**yi));
	//		if ((rreal!=0)&&(rreal<=radius)) 
	//		{++neh;	cage->add_bond((**ii).serial(),(**yi).serial());  }
	//		else {
	//		if (debuggin) {cout<<rreal<<" "; (**ii).dump_loc(cout); 
	//		cout<<" vs "; (**yi).dump_loc(cout); cout<<"\n"; }
	//			
	//			}	
	//			++yi; } 
	//cout<<"AddNay4  "<<m_composite_x.size()  <<"\n"; cout.flush();
	++ii;
	if (ii!=ie) rz.restrict<&TgtTy::z,true,false>
		( (**ii).z(),radius,m_z,m_composite,compare_x());
	} 
}
*/



// clean this up and put it somewhere...

#include "build_neighbor_map.h"
