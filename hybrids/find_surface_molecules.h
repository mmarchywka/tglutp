#ifndef FIND_SURFACE_MOLE_H__
#define FIND_SURFACE_MOLE_H__

#include "../base/points_base_classes.h"

#include "neighbor_probe_classes.h"

// need the twirl thing
#include "../math/move_around.h"

#include <algorithm>

class enclave_entry
{
	// try to characterize neighbors in terms of 
	// distance and exposure parameters 
	typedef enclave_entry Myt;
	typedef int IdxTy;
	typedef double Fl; 
public:	
	enclave_entry(): m_idx(-1),m_dist(0),m_exposed(0)
	{}
	const Fl & dist() const { return m_dist; }
	template <class Os> Os & d1(Os & os) const
	{	os<<m_dist<<" "<<m_exposed<<" "<<m_cos;
		return os;
		}
//private:
	IdxTy m_idx; // the index of this neighbor in the underlying pdb file.
	Fl m_dist; // distance from reference atom.
	Fl m_exposed; // "amount" of exposed area
	Fl m_cos; // cosine of angle between dr and n
};

template < int Idx > class enclave_sort
{
	typedef enclave_entry Tgt;
	enum {SX=Idx};
public:
	int operator()(const Tgt & a, const Tgt & b ) const
	{
		switch (SX)
		{
			case 0:	return (a.m_dist<b.m_dist); 	
		
		}
		return 0; 
	}; 	
	
	
	
};

template <class Ty, class Tr> class find_surface_molecules : public Tr //pdb_typedefs
{
	typedef find_surface_molecules Myt;
	//typedef point_list Pl;
	typedef Ty Pl; 
	typedef typename Pl::value_type Pt;
	typedef typename Pl::iterator Pi; 
	typedef mr_rogers Na;
	typedef double Fl;
	typedef typename Tr::IdxTy IdxTy;
	// deprecated code...
	typedef typename Tr::OsTy OsTy;
	typedef typename Tr::Chr Chr;
	typedef typename Tr::St StrTy;
	// eh, this is kind of a bastard thing.
	// it is supposed to be average normal for each atom,
	// variously float and double.... 
	//typedef Ty Fl;
	typedef  myPoint4<Fl > AvgTy;
	typedef std::map<IdxTy,AvgTy> AvgMap;

	// at some point, the pictures are turned into area descriptions
	typedef enclave_entry EnTy;
	typedef std::vector<EnTy> EnVec;
	// and we could store the neighbor info for each surfac atom
	typedef std::map<IdxTy, EnVec> EnMap;
	// map the 3 letter names to chars
	typedef std::map<StrTy, Chr> AaMap;
	
	static AaMap & aamap()
	{
		static AaMap amap;
		if ( amap.size()==0)
		{
			amap["GLY"]='G';amap["PRO"]='P';amap["ALA"]='A';amap["VAL"]='V'; 	
			amap["LEU"]='L';amap["ILE"]='I';amap["MET"]='M';amap["CYS"]='C'; 	
			amap["PHE"]='F';amap["TYR"]='Y';amap["TRP"]='W';amap["HIS"]='H'; 	
			amap["LYS"]='K';amap["ARG"]='R';amap["GLN"]='Q';amap["ASN"]='N'; 	
			amap["GLU"]='E';amap["ASP"]='D';amap["SER"]='S';amap["THR"]='T'; 	
		}
	
		return amap;	
	}
	
public:
	find_surface_molecules():m_radius(0) {}
	// This tries to find the amount of obscuration,
	// currently not accounting for overlap.
	// makes law of cosines obscuration calculation and stores
	// direction( normal ) and angle of obscured cone. 
	// Ths source param "s" is now a template to allow list conversion from 
	// other things that contain pdb_atoms. This only need support a const_iterator
	// to the value_type which must be pdb_atom. 
	template <class Tp > void find(const Tp & s,  const Fl & probe_radius)
	{
		// expand all entries by the probe radius
		// if they clear, the probe fits unhindered
		m_radius=probe_radius;
		Pl::mutate(m_s,s,expand_radius(probe_radius));
		Na &  mr=m_mr;
		mr.clear();
		const IdxTy sz=m_s.size();
		for ( IdxTy i=0; i<sz; ++i)
		{
			const Pt & pi=m_s[i];
			Fl r1=pi.radius(); r1*=r1;
			for(IdxTy j=i+1; j<sz; ++j)
			{
				const Pt & pj=m_s[j];	
				// these are close enough to restrict probe access 
				if (!pi.clears(pj))
				{ 	// so they each count as neighbors to each other
					mr.n()[i].push_back(j);mr.n()[j].push_back(i);
					Fl r2=pj.radius(); r2*=r2;
					const Fl dr=pi.dist(pj);
					const Fl del=(r2-r1)/dr;
					const Fl a12=1+((del-dr)/pi.radius())/2;
					const Fl a21=1+((-del-dr)/pj.radius())/2;
					const Fl cos12=-a12+1;
					const Fl cos21=-a21+1;
					// the angle can be >180, but it seems to happen
					// more than I intuitively expect.
					if (( cos12>1)||(cos12<-0.9)||(cos21>1)||(cos21<-0.9))
						Tr::eos()<<MM_MARK<<" cos "<<cos12<<" "<<cos21<<
							" "<<del<<" "<<dr<<" "<<r1<<" "<<r2<<	"\n";
					
					//Fl num= (r1-r2)/dr-dr;
					// 2*pi*(1-cos(alpha) ) is area but we remove it here and there. 
					// but the obscuration, A=2pi*(1-Cos (alpha)), varies
					// depending upon viewpoint as radii differ. 
					// note that overlap ignored and area scaled by 2pi
					mr.h()[i]+=a12;
					mr.h()[j]+=a21; 
					Fl dx=(pj.x()-pi.x())/dr;
					Fl dy=(pj.y()-pi.y())/dr;
					Fl dz=(pj.z()-pi.z())/dr;
					
					mr.d()[i].push_back(Na::DetTy(dx,dy,dz,cos12));
					mr.d()[j].push_back(Na::DetTy(-dx,-dy,-dz,cos21));
				}
			
			}
		}
	}
// draw lines between accessible neighbors subject to visibility ( access)
// and distance constaints. A large probe may make distance atoms neighbors
// but not nearest neighbots. Use a fake cutoff instead of seeing if
// they obscure each other.
template <class Os>	void mesh(Os& os, const Fl & visible,const Fl & dmax)
	{
		const Fl thr= visible*2;
		IdxTy count=0,uniq=0;
		const IdxTy sz=m_s.size();
		for ( IdxTy i=0; i<sz; ++i)
		{	
			if (m_mr.h()[i]<thr)
			{ 
				++uniq;
				Na::NayTy & nv=m_mr.n()[i];
				Pt & p=m_s[i];
				const IdxTy nasz=nv.size();
				for (IdxTy j=0; j<nasz; ++j)
				{
					const IdxTy jj=nv[j];
					if (jj<i) continue; // this should be picking up 2 lines otherwise. 
										// note that boths directions need to be <thr so no loss. 
					if (m_mr.h()[jj]<thr)
					{
						Pt & q=m_s[jj];
						if ( q.dist(p)<dmax) 
						{	os<<"LINE "<<p.x()<<" "<<p.y()<<" "<<p.z()<<" "<<q.x()<<" "<<q.y()<<" "<<q.z()<<" 1"<<"\n";
							++count;
							
							if (Tr::verbose())
								Tr::eos()<<MM_MARK<<" "<<i<< " "<<q.dist(p)<<" "<<p.field(13)<<" "<<q.field(13)<<
							" "<<(p.x()-q.x())<<" "<<(p.y()-q.y())<<" "<<(p.z()-q.z())<<" "<< " \n";
							
						}
					}
				}
			}
		}
		if (Tr::debug()) Tr::eos()<<MM_MARK<<" Mesh output "<<count<<
			" entries out of "<<sz<<" avarging "<<((uniq==0)?0:(1.0f*count/uniq))   <<"\n";
	}
	// For each accessible atom, find a low energy normal and output that.
	// This is based on minimizing "energy" with known neighbors subject
	// to constant length ( LaGrange multiplier is the length).
	template <class Os>	void norm(Os& os, const Fl & visible,const Fl & dmax)
	{
		const Fl thr= visible*2;
		const IdxTy sz=m_s.size();
		IdxTy count=0;
		for ( IdxTy i=0; i<sz; ++i)
		{	
			if (m_mr.h()[i]<thr)
			{
				Na::NayTy & nv=m_mr.n()[i];
				Pt & p=m_s[i];
				const IdxTy nasz=nv.size();
				Fl xs=0, ys=0, zs=0;
				for (IdxTy j=0; j<nasz; ++j)
				{	// the accessibility of the hindering neighbor is not an issue
				// but the hinderance or repulsion should be subtended area weighted and
				// unawkwardly stated.
					const IdxTy jj=nv[j];
					Pt & q=m_s[jj];
					Fl w=q.dist(p); // really should use 1-cos(alpha)
					w=1.0/(w*w); // use one power to normalize, another to decrease distant neighor
							// weight
					xs+=w*(p.x()-q.x());
					ys+=w*(p.y()-q.y());
					zs+=w*(p.z()-q.z());
					//	if ( q.dist(p)<dmax) 
					//		os<<"LINE "<<p.x()<<" "<<p.y()<<" "<<p.z()<<" "<<q.x()<<" "<<q.y()<<" "<<q.z()<<" 1"<<"\n";
				}
				Fl norm=::sqrt(xs*xs+ys*ys+zs*zs);
				xs/=norm; ys/=norm; zs/=norm;
				Fl ee=0;
				for (IdxTy j=0; j<nasz; ++j)
				{	// the accessibility of the hindering neighbor is not an issue
				// but the hinderance or repulsion should be subtended area weighted and
				// unawkwardly stated.
					const IdxTy jj=nv[j];
					Pt & q=m_s[jj];
					Fl w=q.dist(p); // really should use 1-cos(alpha)
					w=1.0/(w); // use one power to normal
					Fl ex= w*(p.x()-q.x())+xs;
					ee+=ex*ex;
					ex= w*(p.y()-q.y())+xs;
					ee+=ex*ex;
					ex= w*(p.z()-q.z())+xs;
					ee+=ex*ex;
					
					//	if ( q.dist(p)<dmax) 
					//		os<<"LINE "<<p.x()<<" "<<p.y()<<" "<<p.z()<<" "<<q.x()<<" "<<q.y()<<" "<<q.z()<<" 1"<<"\n";
				}
				
				if (ee<dmax)
				{
				os<<"NORMAL "<<p.x()<<" "<<p.y()<<" "<<p.z()<<" "<<xs<<" "<<ys<<" "<<zs<<
					" "<<p.m_r <<" "<<p.m_g<<" "<<p.m_b<<" "<<(p.radius()-m_radius)<<'\n';
				if (Tr::verbose())
					Tr::eos()<<MM_MARK<<" "<<i<< " "<<nasz<<" "<<p.field(13)<<" "<< m_mr.h()[i] <<
							" "<<ee<<"\n"; // " "<<(p.y()-q.y())<<" "<<(p.z()-q.z())<<" "<< " \n";
				
				
				++count; 
				}
			}
		}
		if (Tr::debug()) Tr::eos()<<MM_MARK<<" Norm output "<<count<<" entries out of "<<sz<<"\n";
	}

// take those atoms that have more (lim=>0, take >lim) or less (lim<0, take <=-lim)
// exposed areas. you can't have <0 exposed, so 0 means >0 areas
// -1 means less than zero
template <class Td> void take(Td & d, const IdxTy lim)
{
	const IdxTy sz=m_s.size();
	const bool le=(lim<0);
	
	for ( IdxTy i=0; i<sz; ++i)
	{	const IdxTy n=m_mr.s()[i].size();
		if (le&&(n<=(-lim))||(!le)&&(n>lim))
		{
			Pt & p=m_s[i];
			d.push_back(p);
		}
	}
}
// Note that this is centered around ZERO as ti can be moved... 

// this interface to Td is really only for one dum class...
// r_max is max distance from zero to accept, sr is fraction of radius to subtract from
// distance ( zero for boundary, 1 for atom surface. 
template <class Td,class Txx> void take_patches(Td & d, const Fl & r_max=0, const Fl & sr=0)
{
	const IdxTy sz=m_s.size();
	//const bool le=(lim<0);
	//typedef typename Td::value_type PairTy;
//	typedef Pt PointTy;
	typedef move_around Ma;
// 2024 unused 
//	const Fl pi=Ma::pi();
//	const Fl kluge=15;
//	const Fl kpi=pi/kluge;	
	typedef float xcoord_type;
	typedef point3_template<xcoord_type> PointTy;
	for ( IdxTy i=0; i<sz; ++i)
	{	
		Pt & p=m_s[i];
		const Fl sub_rad= sr; //=0;
		const Fl ssx=p.radius()-m_radius*sub_rad;
// 2024 unused
//		const Fl rrad=ssx*kpi; // 1*3.14159/kluge;
		Na::Dv  & ss=m_mr.s()[i];
		const IdxTy n=ss.size();
		for (IdxTy j=0; j<n; ++j)
		{
			Na::DetTy x=ss[j];
		//	PointTy nor=PointTy(x[0],x[1],x[2]);
			float mnx=x[0], mny=x[1],mnz=x[2];
			Ma::twirl(mnx,mny,mnz);
   			PointTy nor=PointTy(mnx,mny,mnz); 
   			/*
   			if ( m_nx>0) {m_nx=-m_nx; m_ny=-m_ny; m_nz=-m_nz;};
   			float mag=sqrt(m_nx*m_nx+m_ny*m_ny+m_nz*m_nz);
  	    	float phi=180/3.14*acos(m_nz/mag); // this is target.Z;
  	    	// std::cout<<phi<<" "<<m_ny<<" "<<m_nx<<" "<<mag<<"\n";
  			//    if (phi!=0) {  glRotatef(phi,-m_ny/mag,m_nx/mag,0); }
			PointTy nor=PointTy(phi,-m_ny/mag,m_nx/mag);
			*/
			PointTy dl=PointTy(p)+PointTy(x[0]*ssx,x[1]*ssx,x[2]*ssx);
			Txx z=Txx()(dl,0)(nor ,1) (PointTy(p.m_r,p.m_g,p.m_b),2);
			 //cage->xpush_back(z);				
			if ((r_max==0)||(r_max>dl.length()))
				d.xpush_back(z);
		}
		
	}
}
// scan the surface looking for open areas and output theses	
// get a set of directions and test each direction for accessibility.
// the accessible directions can be output in a variety of ways,
// this probably needs a simple delegate, not a bunch of conditionals
// in the inner loop....
template <class Os>	void roll_grid(Os& os, const Fl & visible,const IdxTy & flags)
{
//	const Fl thr= visible*2;
	IdxTy count=0,uniq=0,atoms=0;
	const IdxTy sz=m_s.size();
		const bool output_each=((flags&1)!=0);// false;
	const bool output_atom=((flags&2)!=0);//true;
	const bool reduce_radius=((flags&4)!=0);//false; 
	const bool store_accessible=((flags&8)!=0);
	if (Tr::debug()) Tr::eos()<<MM_MARK<<" making grid with flags:"<<
		(output_each?"output each patch, ":" no patches, ")<<
		(output_atom?" output whole atom, ":" no whole atoms, ")<<
		(reduce_radius?" use atom radius, ":" use atom+probe radius, ")<<
		(store_accessible?" save hits to list":" dont save hits ")<<'\n';
	if (store_accessible) m_mr.s().clear();
	const Fl sub_rad=(reduce_radius)?1:0; 
//	typedef Tr Traits;
	typedef direction_generator<float,Tr> DgTy;
	typedef typename DgTy::generated_list FuTy;
	typedef typename DgTy::dir_type Fu2Ty;
	DgTy dg;
	IdxTy lx=15, ly=10;
	Fl kluge=15.0f;
	// kluge==2*lx is required, ly no used. 
	lx=10; ly=20; kluge=lx*2;
	
	dg.generate(lx,ly,kluge);
	
	FuTy & spots=dg.generated();
	
	const IdxTy pts=spots.size();
	for ( IdxTy i=0; i<sz; ++i)
	{	
			// List of oscvurations
		Na::Dv & nv=m_mr.d()[i];
		Pt & p=m_s[i];
		const IdxTy nasz=nv.size();
		const Fl ss=p.radius()-m_radius*sub_rad;
		const Fl rrad=ss*1*3.14159/kluge;
		IdxTy ux=0;
		for ( IdxTy ix=0; ix<pts; ++ix)
		{
			
			Fu2Ty & x=spots[ix];
			const Fl xs=x[0];
			const Fl ys=x[1];
			const Fl zs=x[2];
			bool passed=true;
			for (IdxTy j=0; j<nasz; ++j)
			{
				Na::DetTy v=nv[j];
				const Fl lim=v[3]; // dot product must be less or olap->fail
				// these aint positive defininte -doh!
				Fl dp=xs*v[0]+ys*v[1]+zs*v[2];
			//	Tr::eos()<<MM_MARK<<" "<<v[0]<<" "<<v[1]<<" "<<v[2]<<" "<<lim<<" "<<xs<<" "<<ys<<" "<<zs<<" "<<dp<<'\n';
				if ( dp>=lim) {
				//	Tr::eos()<<MM_MARK<<" "<<v[0]<<" "<<v[1]<<" "<<v[2]<<" "<<lim<<" "<<xs<<" "<<ys<<" "<<zs<<" "<<dp<<'\n';
					passed=false; break;}
				
			}				
			//Tr::eos()<<MM_MARK<<(passed?"PASS ":"FAIL ")<< nasz <<"\n";
			if (passed)
			{	++ux;			
				if ( output_each ) os<<"NORMAL "<<(p.x()+xs*ss)<<" "<<(p.y()+ys*ss)<<" "<<(p.z()+zs*ss)<<
				" "<<xs<<" "<<ys<<" "<<zs<<" "<<p.m_r <<" "<<p.m_g<<" "<<p.m_b<<" "<<(rrad)<<'\n';		
				if ( store_accessible) m_mr.s()[i].push_back(Na::DetTy(xs,ys,zs,rrad));
			}						
		} // itor
		if ( ux!=0) {
			if ( output_atom&&(ux>visible)) 
			{++atoms;
			os<<"RAW "<<(ss)<<" "<<(p.x())<<" "<<(p.y())<<" "<<(p.z())<<
					" "<<p.m_r <<" "<<p.m_g<<" "<<p.m_b<<" "<<'\n';	
			}
			++uniq; count+=ux; 
		}
	}
	if (Tr::debug()) Tr::eos()<<MM_MARK<<" Grid output "<<count<<
			" entries uniq "<<uniq<< " ("<<atoms << ") out of "<<sz<<" avarging "<<((uniq==0)?0:(1.0f*count/uniq))   <<"\n";
}
	
template <class Os>	void get(Os& os, const Fl & visible)
	{	// output a list of atoms that are obscured less than the visible threshold.
		const Fl thr= visible*2; // 4*3.14159;
		const IdxTy sz=m_s.size();
		for ( IdxTy i=0; i<sz; ++i)
		{	
			if (m_mr.h()[i]<thr)
			{
				Pt & p=m_s[i];
				os<<p.radius()<<" "<<p.x()<<" "<<p.y()<<" "<<p.z()<<" "<<p.m_r
					<<" "<<p.m_g<<" "<<p.m_b<<" "<<m_mr.h()[i]<<"\n";
			}
		}
	}
	
template <class Os>Os & locate(Os& os, Pt & p)
{   os<<" "<<p.x()<<" "<<p.y()<<" "<<p.z(); return os; }
template <class Os>Os & color(Os& os, Pt & p)
{   os<<" "<<p.m_r <<" "<<p.m_g<<" "<<p.m_b; return os; }
 template <class Os,class Tyy >Os & locate(Os& os, const Tyy & x, const Tyy & y, const Tyy & z)
{   os<<" "<<x<<" "<<y<<" "<<z; return os; }

// Find the search features called "enclaves" which consist
// of a surface atom and a list of all surface neighbors.
// parameters are distance from reference resident and amount of exposed surface
// sort the output by distance from reference.
void make_enclave(EnVec & ev, const Pt & p, AvgTy & x, Na::NayTy & nv, const IdxTy exposure_limit)
{
	const IdxTy nasz=nv.size();
	// if we have a surface atom, check its neighbors and output a 
	// street level cesnsus result for valid neighbors.
	for ( IdxTy jj=0; jj<nasz; ++jj)
	{
		IdxTy j=nv[jj]; // this points to the offending atom
		AvgTy & y=m_av[j]; // this tells us if it is a surface atom or not.
		Pt & q=m_s[j]; 
//		Tr::eos()<<MM_MARK<<" "<<jj<<" "<<j<<" "<<x[3]<<" "<<y[3]<<" "<<nasz<<" "<<q.dist(p)<<'\n';
		if ( y[3]<exposure_limit) continue;
		
		EnTy e;  // EnVec::value_type ? 
		const Fl xs=x[0]*(p.x()-q.x());
		const Fl ys=x[1]*(p.y()-q.y());
		const Fl zs=x[2]*(p.z()-q.z());
		// note that this could be zero or close there too
		// consider two patches opposed to each other or equatorial
		// band. 
		const Fl nn=x[0]*x[0]+x[1]*x[1]+x[2]*x[2];
		e.m_idx=j;
		e.m_dist=p.dist(q);
		e.m_exposed=y[3];
		e.m_cos=(xs+ys+zs)/e.m_dist/nn;
		ev.push_back(e);
	} // jj
	
}
template <class Os> Os & enc(Os& os, Pt & p)
{ // 1 2 4 5 
	os<<" "<<p.field(1)<<" "<<p.field(2)<<" "<<p.field(4)<<" "<<p.field(5) <<" ";
	return os;
}




template <class Os>	void make_enclaves(Os& os, const Fl & visible,const IdxTy & flags
	, const Chr * pfx, const IdxTy format)
{
	const IdxTy exposure_limit=2;
	MakeAverage();
	m_enclaves.clear(); 
	// for each atom
	const IdxTy sz=m_s.size();
	for ( IdxTy i=0; i<sz; ++i)
	{	
		
		// and net number of hits and approx best direction
		// - see caveats due to improper sampling distribution ....
		AvgTy &  x=m_av[i];
		
		// if (p is not surface atom) continue 
		if (x[3]<exposure_limit) continue; 
		Pt & p=m_s[i];
		// List of oscurations
		Na::NayTy & nv=m_mr.n()[i];
		//Tr::eos()<<MM_MARK<<" "<<x[3]<<" "<<i<<" "<<sz<<" "<<nv.size()<<'\n';
		EnVec   ev;//=m_enclaves[i];
		make_enclave(ev,p,x,nv,exposure_limit); 
		//Tr::eos()<<MM_MARK<<" "<<'\n';
		std::sort(ev.begin(),ev.end(), enclave_sort<0>()); 
	//	Tr::eos()<<MM_MARK<<" "<<'\n';
		const IdxTy esz=ev.size();
	//	const IdxTy format=1; 
		switch ( format)
		{
		case 0: {for ( IdxTy j=0; j<esz; ++j)
					{ev[j].d1(enc(enc(os<<pfx,p),m_s[ev[j].m_idx]));os<<"\n";}
				break; }
				
		case 1: { os<<p.field(13) ;//pfx
			for ( IdxTy j=0; j<esz; ++j)
				{os<<" "<<m_s[ev[j].m_idx].field(13); }; os<<" "; locate(os,p); os<<" "<<pfx<<"\n";
			break; 
			}
		
		case 2: { 
			AaMap & a= aamap();
			os<<a[p.field(4)];
			//os<<p.field(13) ;//pfx
			for ( IdxTy j=0; j<esz; ++j)
				{os<<" "<<a[m_s[ev[j].m_idx].field(4)]; }; os<<" "; locate(os,p); os<<" "<<pfx<<"\n";
			break; 
			}
		
		}//switch 
	//Tr::eos()<<MM_MARK<<" "<<'\n';
		//ev.clear(); /// aren't saving right now... 
	} // i 
	
	
	
}

/*
COLUMNS      DATA TYPE        FIELD      DEFINITION
------------------------------------------------------
------------0---------------------
 1 -  6      Record name      "ATOM    "
 7 - 11      Integer          serial     Atom serial number.
13 - 16      Atom             name       Atom name.
17           Character        altLoc     Alternate location indicator.
18 - 20      Residue name     resName    Residue name.
-------------------5------------------
22           Character        chainID    Chain identifier.
23 - 26      Integer          resSeq     Residue sequence number.
27           AChar            iCode      Code for insertion of residues.
31 - 38      Real(8.3)        x          Orthogonal coordinates for X in 
                                         Angstroms
39 - 46      Real(8.3)        y          Orthogonal coordinates for Y in 
                                         Angstroms
----------------------10-------------------------------------
47 - 54      Real(8.3)        z          Orthogonal coordinates for Z in 
                                         Angstroms
55 - 60      Real(6.2)        occupancy  Occupancy.
61 - 66      Real(6.2)        tempFactor Temperature factor.
77 - 78      LString(2)       element    Element symbol, right-justified.
79 - 80      LString(2)       charge     Charge on the atom.

*/

template <class Os>	void sort_edges(Os& os, const Fl & visible,const IdxTy & flags)
{
//	const Fl thr= visible*2;
//	IdxTy count=0,uniq=0,atoms=0;
	/*
	const bool output_each=((flags&1)!=0);// false;
	const bool output_atom=((flags&2)!=0);//true;
	const bool reduce_radius=((flags&4)!=0);//false; 
	const bool store_accessible=((flags&8)!=0);
	if (Tr::debug()) Tr::eos()<<MM_MARK<<" making grid with flags:"<<
		(output_each?"output each patch, ":" no patches, ")<<
		(output_atom?" output whole atom, ":" no whole atoms, ")<<
		(reduce_radius?" use atom radius, ":" use atom+probe radius, ")<<
		(store_accessible?" save hits to list":" dont save hits ")<<'\n';
	if (store_accessible) m_mr.s().clear();
	const Fl sub_rad=(reduce_radius)?1:0; 
	*/
	
	MakeAverage();
	const IdxTy sz=m_s.size();
	std::map<IdxTy,IdxTy> xxx;
	for ( IdxTy i=0; i<sz; ++i)
	{
		AvgTy x=m_av[i];
		if (x[3]==0) continue;
		Pt & px=m_s[i];
		for ( IdxTy j=i+1; j<sz; ++j)
		{
			AvgTy y=m_av[j];
			if ( y[3]==0) continue;
			// ok, so we have two points that both have exposed areas.
			Pt & py=m_s[j];
			Fl dx=px.x()-py.x();
			Fl dy=px.y()-py.y();
			Fl dz=px.z()-py.z();
			Fl r=::sqrt(dx*dx+dy*dy+dz*dz);
			Fl n1=r*::sqrt(x[0]*x[0]+x[1]*x[1]+x[2]*x[2]);
			Fl n2=r*::sqrt(y[0]*y[0]+y[1]*y[1]+y[2]*y[2]);
			Fl d1=-(x[0]*dx+x[1]*dy+x[2]*dz)/n1;
			Fl d2=(y[0]*dx+y[1]*dy+y[2]*dz)/n2;
			if (( d1>.5)&&(d2>.5))   // point towards each other --> inside or concavity. 
			{xxx[i]++; xxx[j]++;
			
		//	locate(locate(locate ( Tr::eos()<<MM_MARK,px),py),dx,dy,dz)<<" "<<d1<<" "<<d2<<'\n';
		//	locate(locate( Tr::eos()<<MM_MARK,x[0],x[1],x[2]),y[0],y[1],y[2])<<'\n';
			}
			else if (( d1<0)&&(d2<0))   // point away from each other --> outside. 
			{}
		}
	}
	for ( IdxTy i=0; i<sz; ++i)
	{
		if (xxx[i]!=0)
		{
			Pt & p=m_s[i];
			const IdxTy nasz=m_mr.s()[i].size();
			
			AvgTy x=m_av[i];
			Fl l=::sqrt(x[0]*x[0]+x[1]*x[1]+x[2]*x[2]);
			// just output a little plane. 
			color(locate(locate(os<<"NORMAL ",p),x[0]/l,x[1]/l,x[2]/l),p)<<" .5 \n";
			color(os<<"PUSHCOLOR ",p)<<'\n';
			locate(locate(os<<"LINE ",p),p.x()+3*x[0]/l,p.y()+3*x[1]/l,
				p.z()+3*x[2]/l)<<" 1 \n";
			os<<"POPCOLOR\n"; 
			/*
			for (IdxTy j=0; j<nasz; ++j)
			{
				Na::DetTy & n=m_mr.s()[i][j]; //push_back(Na::DetTy(xs,ys,zs,rrad));			
				const Fl xs=n[0]; const Fl ys=n[1]; const Fl zs=n[2];
				const Fl rrad=n[3];
				const Fl ss=p.radius(); //-m_radius*sub_rad;
				os<<"NORMAL "<<(p.x()+xs*ss)<<" "<<(p.y()+ys*ss)<<" "<<(p.z()+zs*ss)<<
				" "<<xs<<" "<<ys<<" "<<zs<<" "<<p.m_r <<" "<<p.m_g<<" "<<p.m_b<<" "<<(rrad)<<'\n';		
			}
			*/
		}
	}


} //sort_edges
	
	
	
	
private:

void MakeAverage()
{
	const IdxTy sz=m_s.size();
	
	AvgMap & av=m_av;
	for ( IdxTy i=0; i<sz; ++i)
	{	
		// List of samples that hit for this atom
		Na::Dv & nv=m_mr.s()[i];
		Pt & p=m_s[i];
		Fl nx=0,ny=0,nz=0;
		const IdxTy nasz=nv.size();
		for (IdxTy j=0; j<nasz; ++j)
		{
				Na::DetTy v=nv[j];
				nx+=v[0]; ny+=v[1]; nz+=v[2];
				//Fl dp=xs*v[0]+ys*v[1]+zs*v[2];
		} // exposed patch iterator 
		if ( nasz!=0)
		{	// probably no big deal for div verses memory allocate but heck..
			const Fl x=1.0/nasz;
			av[i]=(AvgTy(nx*x,ny*x,nz*x,nasz)); 
		}
			else av[i]=(AvgTy(0,0,0,0));
	} // m_s iterator
	
}



	// size of probe used to expand m_s
	Fl m_radius;
	// the modified source pdb file expaned by probe radius
	Pl m_s;
	// accumulated statistics, presumably for m_s;
	Na m_mr;
	// key is atom number, the value is an "average" normal and number or
	// exposed patches ( x[3]).
	AvgMap m_av;
	// enclave map, the final result. 
	EnMap m_enclaves;
	
};







#endif

