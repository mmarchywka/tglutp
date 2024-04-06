#ifndef PDB_CHAIN_MAKER_H__
#define PDB_CHAIN_MAKER_H__

#include "../atom/pdb_atom.h"
#include "../atom/molecule_view_list.h"
#include "../atom/atom_list.h"
#include "../debug_and_support.h"
#include <string>
#include <map>

class pdb_chain_maker
{
	typedef pdb_chain_maker Myt;
	typedef int count_type;
	typedef int index_type;
	typedef float coord_type;
	typedef index_type IdxTy;
	typedef char ChTy;
	typedef basic_string<ChTy> string_type;
	typedef pdb_atom atom_type;
	typedef const atom_type * AtomPtrTy;
	typedef int flag_type;
	typedef AtomLiTy AtomListTy;
	typedef string_type ContigNameTy;
	typedef annotated_list<atom_type>  ChainListTy;
	typedef map<ContigNameTy,ChainListTy> ChainMapTy;
	typedef ChainMapTy::const_iterator ChMapItorTy;
	typedef ChainMapTy::iterator MapItorTy;
	
	typedef map<string_type,AtomPtrTy> ResidueMapTy;
	typedef ResidueMapTy::iterator ResItorTy;
	
public:

pdb_chain_maker(const AtomListTy & a) : m_atom_list(a),m_attach(NULL),m_this_chain(" ") {}

template <typename Dest> void make_chains(Dest& all)
{MakeGoodChains(all); }

private:

enum {NCHAINS=3,CHAIN=0, SIDE=1,HETERO=2};

void Attach(ChainListTy& li,ResItorTy & it,bool mv)
{
	 if (it==m_res.end()) return; 

	AtomPtrTy as=(*it).second;
	li.push_back(m_attach);
	li.push_back(as); if (mv)m_attach=as;
	m_res.erase(it);
	}
template <typename Dest > void Attach(Dest & all,ResItorTy & it,const string_type& chain_id ,
AtomPtrTy & ca_at, AtomPtrTy & at_save)
{	ChainListTy& li=all[CHAIN][chain_id]; 
	// These are nowparis...
if (!li.size()) {GenerateColor(all,CHAIN,chain_id); } 	
	Attach(li,it,true);
	it=m_res.find("CA"); Attach(li,it,true);ca_at=m_attach;
	it=m_res.find("C");  Attach(li,it,true);
	
	it=m_res.find("O");  Attach(li,it,false);
	at_save=m_attach;
//	std::cout<<" Chain was :"<<chain_id<<". and acid is :"<<acid_id<<".\n";
}
template <typename Dest > void AttachSide(Dest & all,const string_type& chain_id)
{
	ChainListTy& li=all[SIDE][chain_id];
	if (!li.size()) {GenerateColor(all,SIDE,chain_id); } 
//	it=m_res.begin(); while (it!=m_res.end()) { cout<<(*(it++)).first<<" ";  } cout<<"\n";
	ResItorTy it=m_res.begin(), ie=m_res.end(); 
	if (it!=ie) ie--; 
	while (it!=ie)
	{	ResItorTy jt=it; // m_res.begin();
		jt++; // avoid duplicates. 
		AtomPtrTy as=(*it).second;
		AtomPtrTy as2=(*it).second;
		bool wierd=(as->field(13)!="C")&&
					(as->field(13)!="N")&&
					(as->field(13)!="O");
	//	bool cysmet=(as->field(4)=="CYS");
		float bond_olap=-1.62f;
		if (wierd) bond_olap=-0.0f;
		while (jt!=m_res.end()) 
		{  as2=(*jt).second; 
		bool wierd2=(as2->field(13)!="C")&&
					(as2->field(13)!="N")&&
					(as2->field(13)!="O");
		if (wierd2) bond_olap=0.0f;
			if ((as2->clearance(*as)<bond_olap)) 
				{li.push_back(as2); li.push_back(as);  }
		++jt; }
		if ((m_attach->clearance(*as)<bond_olap)) 
			{li.push_back(m_attach); li.push_back(as);  }
		++it;
	}
}
template <typename Dest > void MakeFromMap(Dest & all)
{
	ResItorTy it=m_res.find("N") ; //,inull=m_res.end();
	if (it==m_res.end()) return; 
	//std::cout<<"adding residues "<<m_res.size()<<"\n"; 
	AtomPtrTy as=(*it).second;
	string_type chain_id=(*as).field(5);
	string_type ChNames="ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	if ( ChNames.find(chain_id)==ChNames.npos )
	{ std::cout<<"nofind chain"<<chain_id<<"\n"; return; }
	if ((m_attach==0)||(chain_id!=m_this_chain)) 
	{ m_this_chain=chain_id; m_attach=as;}
	if( as->dist(*m_attach)>5) 
	{  mmdd::dout(MMDDLOC)<<" Bond too long for chain :\n"<<as->desc()<<"\n"<<
		m_attach->desc()<<"\n"; 
		m_attach=as;
		}
	string_type acid_id=(*as).field(4);
	AtomPtrTy ca_at=0,at_save=0;
	Attach(all,it,chain_id,ca_at,at_save);
//http://peds.oxfordjournals.org/cgi/reprint/10/7/777.pdf
//http://www.umass.edu/microbio/rasmol/rasbonds.htm
//http://bioinformatics.oxfordjournals.org/cgi/reprint/17/10/949.pdf
//http://deposit.pdb.org/cc_dict_tut.html
	m_attach=ca_at;
	AttachSide(all,chain_id);
	m_attach=at_save;
	}
template <typename Dest > void MakeGoodChains(Dest & all)
{
	IdxTy i=0,_end=m_atom_list.size();
	string_type pending_residue="";
	while (i!=_end)
	{
	const atom_type & a=m_atom_list[i];
	if (a.desc().length()<25) continue;
	string_type res=a.field(3)+a.field(4)+a.field(5)+a.field(6);
	//std::cout<<res<<"\n";
	if (res!=pending_residue){ 
		MakeFromMap(all);
		m_res.clear();
		pending_residue=res;
	}
	m_res[a.field(2)]=&a;
	
	
	++i;
	}//while
}

template <typename Dest> void GenerateColor(Dest& all,const int ch,const string_type &chain_id)
{	ChainListTy& li=all[ch][chain_id]; // const access????
	ChainAttrTy thecol=ChainAttrTy();
	int x=*((chain_id.c_str()));
	if(ch==CHAIN) { thecol=ChainAttrTy(2,1.0f*(x&7)/7,1.0f*((~(x/8))&7)/8,.7f*(x&1));   }
	if(ch==SIDE) { thecol=ChainAttrTy(2,1.0f*(x&7)/7,.3f,.7f*(x&1));   }
	li.atr()=thecol;
}



const AtomListTy & m_atom_list;
//ChainListTy m_main,m_side;

ResidueMapTy m_res;

AtomPtrTy m_attach;
string_type m_this_chain;
};
#endif


