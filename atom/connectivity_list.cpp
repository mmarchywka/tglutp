#include "connectivity_list.h"
typedef connectivity_list MyClass;

void MyClass::draw(DispTy & root)
{	index_type j=0,k=0;
	if (root.hidden()) return;
 	DispTy* rp=&root;
 	for (index_type i=0; i<NCHAINS; ++i){
	ChainMapTy cm=m_all[i];
	if (cm.size()){  rp=(*rp)[k++];
	if (!rp->hidden())
	{ j=0; MapItorTy ii=cm.begin(), ie=cm.end();
	 while(ii!=ie) { 
		// This is a mess- the map can change without display_tree 
		// vector...
	//	const string_type& name=(*ii).first;
		ChainListTy& li=(*ii).second;
		li.draw(*((*rp)[j++]));
	//	li.create_display_tree(root->get_child(name)); 
		++ii;
	} }  
	rp=rp->mom();
	}
	}}

void MyClass::create_display_tree(DispTy * root)
{
	root=root->get_child(name()+mprintf(mangled_list));
	for (index_type i=0; i<NCHAINS; ++i){
	ChainMapTy cm=m_all[i];
	if (cm.size()){ root=root->get_child(string_type(chain_names()[i])); 
	MapItorTy ii=cm.begin(), ie=cm.end();
	while(ii!=ie) { 
		const string_type& name=(*ii).first;
		ChainListTy& li=(*ii).second;
		li.create_display_tree(root->get_child(name)); 
		++ii;
		} 
	root=root->mom();
	} 
	}
}


void MyClass::MakeChains()
{
	pdb_chain_maker pcm(m_atom_list);
	pcm.make_chains(m_all);
	
}


MyClass::index_type MyClass::FindSequential(const index_type target)
{
	index_type hi=m_atom_list.size(); index_type lo=0;	
	index_type guess=lo;
	while ( target!=(m_atom_list[guess].serial())) 
	{
	guess++;
		if (guess==hi) return -1;
		}	
	return guess;	
}


void MyClass::MakeThings(const index_type v, const string_type &name)
{
	index_type i=m_bond_stack.size();
	// v-1 entries per object,
	if (v<2){ std::cout<<"Doesnt work for points yet\n"; }
	if (i%(v-1)) 
		{ std::cout<<"MakeThings stack size "<<i<<" not even with "<<v<<"\n";}
	index_type p[v];AtomPtrTy ind[v];
	BondStackTy::iterator ii=m_bond_stack.begin();
	ChainListTy & li=m_all[HETERO][ name ];
	i=i/(v-1);
	while (i)
	{
		BondTy b=(*(ii++));
		index_type s=b.first; index_type d=b.second;
		p[0]=s; p[1]=d;
		for (index_type j=2; j<v; ++j)
		{
			b=*(ii++); s=b.first; 
			if (d!=s)
				{ std::cout<<" not ver="<<v<<" at "<<s<<" "<<" "<<d<<" "<<"\n"; }
			d=b.second;p[j]=d;
		}
		bool poly_ok=true;
		for (index_type j=0; j<v; ++j)
		{
			const count_type idx=FindFunc(m_atom_list,p[j],&atom_type::serial);
			if (idx>=0) ind[j]=&m_atom_list[idx];
			else
			{ poly_ok=false; std::cout<<"Questionable Bond "<<j<<" "<<p[j]<<" "<<idx<<"\n";  }
		}
	if (poly_ok) li.xpush_back(ind,v); 
	--i; 	
	}	
	m_bond_stack.clear();
	std::cout<<"Done Making bonds "<<i<<"\n";
}
