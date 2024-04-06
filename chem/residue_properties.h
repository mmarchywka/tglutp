#ifndef RESIDUE_PROPERTIES_H__
#define RESIDUE_PROPERTIES_H__
#include "../base/points_base_classes.h"
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <string.h>
#include <strings.h>

class residue_properties_tr
{public:
	typedef char Ch;
	typedef std::basic_string<Ch> string_type;
	typedef float ordering_type;
	typedef unsigned int index_type;
	typedef std::map<string_type,ordering_type> lut_type;
	typedef std::pair< string_type,ordering_type> PairTy;
	typedef std::vector< PairTy > SortTy;
	typedef lut_type::const_iterator LutItor;
	typedef const Ch* RawStringTy;
	typedef std::map<RawStringTy, RawStringTy > xlate_type; 
	typedef std::map<string_type,string_type> string_xlate_type;
	typedef xlate_type::const_iterator XlateItor;
	typedef std::map<string_type,xlate_type> dict_type;
	typedef dict_type::const_iterator DictItor;
	typedef float color_coord;
	enum {ACIDS=20};
	// 2008-03-18 not sure about this either...
	//typedef myPoint3<color_coord> color_type; 
	typedef point3_template<color_coord> color_type;
	typedef point3_array<color_coord,ACIDS> SpectrumTy;
	typedef std::map<Ch, color_type> sequence_map_type;
	typedef std::map<string_type, ordering_type> external_map_type;
	typedef std::map<string_type, color_type> color_table_type;
};

class residue_properties: public residue_properties_tr
{public:

template <class Td, class Tx, class Ty > static void map_load
	( Td& map, const Tx ** keys, const Ty** vals)
	{
		Tx ** k=keys; Ty** v=vals;
		while (k!=NULL) map[*(k++)]=*(v++);
	}
template <class Td, class Tx > static void map_load
	( Td& map, const Tx ** keys)
	{
	//	const Tx * k=keys[0];
		index_type ii=0; 
		while (keys[ii]!=NULL){ map[string_type(keys[ii])]=
			string_type(keys[ii+1]); ii+=2; }
	}
	template <class Td, class Tx > static void map_load_bk
	( Td& map, const Tx ** keys)
	{
//		const Tx * k=keys[0];
		index_type ii=0; 
		while (keys[ii]!=NULL){ map[string_type(keys[ii+1])]=
			string_type(keys[ii]); ii+=2; }
	}
	
class vsort {public : 
	int operator()(const PairTy & a, const PairTy & b) {return (a.second<b.second);}  
};
external_map_type & getmap( const string_type & nm);
color_table_type & getcolors() { return m_colors;}

void load_map();
template <class Os> void dump_map(Os& os)
{ 
	name_luts();
	DictItor ii=m_bigmap.begin();
	DictItor ie=m_bigmap.end();
	// This will act wierd is the "Amino" entry is missing...
	while (ii!=ie) 
	{ 	
		RawStringTy desc="no description.";
		RawStringTy screw="Amino";
		const xlate_type & xl=(*ii).second;
		if (xl.find(screw)!=xl.end()) desc=xl.find(screw)->second;
		const string_type & nm=(*ii).first;
		os<<nm<<":"<<desc <<"\n"; 
		getmap(nm);
	//	os<<" the new word order:\n";
		for (index_type i=0; i<m_vec.size(); ++i)
		{
			RawStringTy ccc=m_vec[i].first.c_str();
			if (m_fwd.find(ccc)!=m_fwd.end()) os<<m_fwd[ccc];
			else os<<"no find "<<ccc<<"."<<m_fwd.size();
		}
			os<<"\n";
		++ii; }
}

void name_luts()
{
	/*const Ch * aa_names[]=
	{	"Ala","A","Arg","R","Asn","N",
		"Asp","D","Cys","C","Gln","Q",
		"Glu","E","Gly","G","His","H",
		"Ile","I","Leu","L","Lys","K",
		"Met","M","Phe","F","Pro","P",
		"Ser","S","Thr","T","Trp","W",
		"Tyr","Y","Val","V",NULL,NULL};
	*/	const Ch * aa_namesu[]=
	{	"ALA","A","ARG","R","ASN","N",
		"ASP","D","CYS","C","GLN","Q",
		"GLU","E","GLY","G","HIS","H",
		"ILE","I","LEU","L","LYS","K",
		"MET","M","PHE","F","PRO","P",
		"SER","S","THR","T","TRP","W",
		"TYR","Y","VAL","V",NULL,NULL};
map_load(m_fwd,aa_namesu);
map_load_bk(m_back,aa_namesu);

}
void spectrum_select(index_type i)
{
for (index_type j=0; j<ACIDS; ++j)
{
	float phi=2.0f*float(j)/ACIDS;
	color_coord r=0;
	color_coord g=0;
	color_coord b=0;
	if (phi<1) 
		{ r=1-phi; g=phi;  }
	else 
		{b=phi-1; g=2-phi;}
	m_r_spectrum[j]=color_type(r,g,b);	
}	
	
}
private:
dict_type m_bigmap;
string_xlate_type m_fwd,m_back;
external_map_type m_map;
color_table_type m_colors;
SpectrumTy m_r_spectrum;
SortTy m_vec; 
};


#endif
