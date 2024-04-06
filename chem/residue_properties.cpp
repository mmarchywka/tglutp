#include "residue_properties.h"
#include "../debug_and_support.h"

typedef residue_properties MyClass;
MyClass::external_map_type & MyClass::getmap( const string_type & nm)
{
	if(m_bigmap.find(nm.c_str())==m_bigmap.end())
	{
		dump_map(std::cout); 
		return m_map;
	}
	xlate_type * mm=&m_bigmap[nm.c_str()];
	
	m_map.clear();
//	SortTy sort_vec; //=m_map;
	m_vec.clear();
	XlateItor ii=mm->begin(), ie=mm->end();
	while (ii!=ie)
	{
		ordering_type v=atof((*ii).second);
		string_type nms=string_type((*ii).first);
	//	mmdd::dout(MMDDLOC)<<"sortintg\n";
		Ch  buffer[nms.length()+1];
		// This is supposed to pickup the "0"
		for (index_type i=0; i<=nms.length(); ++i) 
		{ 	buffer[i]=::toupper(nms.c_str()[i]);}
		nms=string_type(buffer);
	//mmdd::dout(MMDDLOC)<<nms <<"  \n";
		m_map[nms ]=v;
		if (nms.length()==3) m_vec.push_back(*(m_map.find(nms)));
		++ii;	
	}
	//mmdd::dout(MMDDLOC)<<"sortintg\n";
	::sort(m_vec.begin(),m_vec.end(),vsort());
	//mmdd::dout(MMDDLOC)<<"sortintg\n";
	SortTy::const_iterator iia=m_vec.begin(),iea=m_vec.end();
	index_type i=0;
	while (iia!=iea)
	{//mmdd::dout(MMDDLOC)<< (*iia).first <<" \n";
		m_colors[(*iia).first]=m_r_spectrum[i++];
	++iia;
	}
//	mmdd::dout(MMDDLOC)<<"sortintg\n";
	return m_map;		
}
void MyClass::load_map()
{

xlate_type * mm=&m_bigmap["Molecularweight"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Molecularweight.html";
(*mm)["Amino"]="acid scale: Molecular weight of each amino acid.";
(*mm)["Ala"]="89.000";(*mm)["Arg"]="174.000";
(*mm)["Asn"]="132.000";(*mm)["Asp"]="133.000";(*mm)["Cys"]="121.000";(*mm)["Gln"]="146.000";
(*mm)["Glu"]="147.000";(*mm)["Gly"]="75.000";(*mm)["His"]="155.000";(*mm)["Ile"]="131.000";
(*mm)["Leu"]="131.000";(*mm)["Lys"]="146.000";(*mm)["Met"]="149.000";(*mm)["Phe"]="165.000";
(*mm)["Pro"]="115.000";(*mm)["Ser"]="105.000";(*mm)["Thr"]="119.000";(*mm)["Trp"]="204.000";
(*mm)["Tyr"]="181.000";(*mm)["Val"]="117.000";
mm=&m_bigmap["Numbercodons"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Numbercodons.html";
(*mm)["Amino"]="acid scale: Number of codon(s) coding for each";

(*mm)["Ala"]="4.000";(*mm)["Arg"]="6.000";(*mm)["Asn"]="2.000";(*mm)["Asp"]="2.000";
(*mm)["Cys"]="1.000";(*mm)["Gln"]="2.000";(*mm)["Glu"]="2.000";(*mm)["Gly"]="4.000";
(*mm)["His"]="2.000";(*mm)["Ile"]="3.000";(*mm)["Leu"]="6.000";(*mm)["Lys"]="2.000";
(*mm)["Met"]="1.000";(*mm)["Phe"]="2.000";(*mm)["Pro"]="4.000";(*mm)["Ser"]="6.000";
(*mm)["Thr"]="4.000";(*mm)["Trp"]="1.000";(*mm)["Tyr"]="2.000";(*mm)["Val"]="4.000";

mm=&m_bigmap["Bulkiness"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Bulkiness.html";
(*mm)["Amino"]="acid scale: Bulkiness.";
(*mm)["Ala"]="11.500";(*mm)["Arg"]="14.280";
(*mm)["Asn"]="12.820";(*mm)["Asp"]="11.680";(*mm)["Cys"]="13.460";(*mm)["Gln"]="14.450";
(*mm)["Glu"]="13.570";(*mm)["Gly"]="3.400";(*mm)["His"]="13.690";(*mm)["Ile"]="21.400";
(*mm)["Leu"]="21.400";(*mm)["Lys"]="15.710";(*mm)["Met"]="16.250";(*mm)["Phe"]="19.800";
(*mm)["Pro"]="17.430";(*mm)["Ser"]="9.470";(*mm)["Thr"]="15.770";(*mm)["Trp"]="21.670";
(*mm)["Tyr"]="18.030";(*mm)["Val"]="21.570";
mm=&m_bigmap["PolarityZimmerman"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/PolarityZimmerman.html";
(*mm)["Amino"]="acid scale: Polarity.";

(*mm)["Ala"]="0.000";(*mm)["Arg"]="52.000";(*mm)["Asn"]="3.380";(*mm)["Asp"]="49.700";
(*mm)["Cys"]="1.480";(*mm)["Gln"]="3.530";(*mm)["Glu"]="49.900";(*mm)["Gly"]="0.000";
(*mm)["His"]="51.600";(*mm)["Ile"]="0.130";(*mm)["Leu"]="0.130";(*mm)["Lys"]="49.500";
(*mm)["Met"]="1.430";(*mm)["Phe"]="0.350";(*mm)["Pro"]="1.580";(*mm)["Ser"]="1.670";
(*mm)["Thr"]="1.660";(*mm)["Trp"]="2.100";(*mm)["Tyr"]="1.610";(*mm)["Val"]="0.130";

mm=&m_bigmap["PolarityGrantham"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/PolarityGrantham.html";
(*mm)["Amino"]="acid scale: Polarity (p).";
(*mm)["Ala"]="8.100";(*mm)["Arg"]="10.500";
(*mm)["Asn"]="11.600";(*mm)["Asp"]="13.000";(*mm)["Cys"]="5.500";(*mm)["Gln"]="10.500";
(*mm)["Glu"]="12.300";(*mm)["Gly"]="9.000";(*mm)["His"]="10.400";(*mm)["Ile"]="5.200";
(*mm)["Leu"]="4.900";(*mm)["Lys"]="11.300";(*mm)["Met"]="5.700";(*mm)["Phe"]="5.200";
(*mm)["Pro"]="8.000";(*mm)["Ser"]="9.200";(*mm)["Thr"]="8.600";(*mm)["Trp"]="5.400";
(*mm)["Tyr"]="6.200";(*mm)["Val"]="5.900";
mm=&m_bigmap["Refractivity"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Refractivity.html";
(*mm)["Amino"]="acid scale: Refractivity.";

(*mm)["Ala"]="4.340";(*mm)["Arg"]="26.660";(*mm)["Asn"]="13.280";(*mm)["Asp"]="12.000";
(*mm)["Cys"]="35.770";(*mm)["Gln"]="17.560";(*mm)["Glu"]="17.260";(*mm)["Gly"]="0.000";
(*mm)["His"]="21.810";(*mm)["Ile"]="19.060";(*mm)["Leu"]="18.780";(*mm)["Lys"]="21.290";
(*mm)["Met"]="21.640";(*mm)["Phe"]="29.400";(*mm)["Pro"]="10.930";(*mm)["Ser"]="6.350";
(*mm)["Thr"]="11.010";(*mm)["Trp"]="42.530";(*mm)["Tyr"]="31.530";(*mm)["Val"]="13.920";

mm=&m_bigmap["Recognitionfactors"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Recognitionfactors.html";
(*mm)["Amino"]="acid scale: Recognition factors.";
(*mm)["Ala"]="78.000";(*mm)["Arg"]="95.000";
(*mm)["Asn"]="94.000";(*mm)["Asp"]="81.000";(*mm)["Cys"]="89.000";(*mm)["Gln"]="87.000";
(*mm)["Glu"]="78.000";(*mm)["Gly"]="84.000";(*mm)["His"]="84.000";(*mm)["Ile"]="88.000";
(*mm)["Leu"]="85.000";(*mm)["Lys"]="87.000";(*mm)["Met"]="80.000";(*mm)["Phe"]="81.000";
(*mm)["Pro"]="91.000";(*mm)["Ser"]="107.000";(*mm)["Thr"]="93.000";(*mm)["Trp"]="104.000";
(*mm)["Tyr"]="84.000";(*mm)["Val"]="89.000";
mm=&m_bigmap["Hphob.Eisenberg"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Hphob.Eisenberg.html";
(*mm)["Amino"]="acid scale: Normalized consensus hydrophobicity scale.";

(*mm)["Ala"]="0.620";(*mm)["Arg"]="-2.530";(*mm)["Asn"]="-0.780";(*mm)["Asp"]="-0.900";
(*mm)["Cys"]="0.290";(*mm)["Gln"]="-0.850";(*mm)["Glu"]="-0.740";(*mm)["Gly"]="0.480";
(*mm)["His"]="-0.400";(*mm)["Ile"]="1.380";(*mm)["Leu"]="1.060";(*mm)["Lys"]="-1.500";
(*mm)["Met"]="0.640";(*mm)["Phe"]="1.190";(*mm)["Pro"]="0.120";(*mm)["Ser"]="-0.180";
(*mm)["Thr"]="-0.050";(*mm)["Trp"]="0.810";(*mm)["Tyr"]="0.260";(*mm)["Val"]="1.080";

mm=&m_bigmap["Hphob.Sweet"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Hphob.Sweet.html";
(*mm)["Amino"]="acid scale: Optimized matching hydrophobicity (OMH).";
(*mm)["Ala"]="-0.400";(*mm)["Arg"]="-0.590";
(*mm)["Asn"]="-0.920";(*mm)["Asp"]="-1.310";(*mm)["Cys"]="0.170";(*mm)["Gln"]="-0.910";
(*mm)["Glu"]="-1.220";(*mm)["Gly"]="-0.670";(*mm)["His"]="-0.640";(*mm)["Ile"]="1.250";
(*mm)["Leu"]="1.220";(*mm)["Lys"]="-0.670";(*mm)["Met"]="1.020";(*mm)["Phe"]="1.920";
(*mm)["Pro"]="-0.490";(*mm)["Ser"]="-0.550";(*mm)["Thr"]="-0.280";(*mm)["Trp"]="0.500";
(*mm)["Tyr"]="1.670";(*mm)["Val"]="0.910";
mm=&m_bigmap["Hphob.Woods"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Hphob.Woods.html";
(*mm)["Amino"]="acid scale: Hydrophilicity.";

(*mm)["Ala"]="-0.500";(*mm)["Arg"]="3.000";(*mm)["Asn"]="0.200";(*mm)["Asp"]="3.000";
(*mm)["Cys"]="-1.000";(*mm)["Gln"]="0.200";(*mm)["Glu"]="3.000";(*mm)["Gly"]="0.000";
(*mm)["His"]="-0.500";(*mm)["Ile"]="-1.800";(*mm)["Leu"]="-1.800";(*mm)["Lys"]="3.000";
(*mm)["Met"]="-1.300";(*mm)["Phe"]="-2.500";(*mm)["Pro"]="0.000";(*mm)["Ser"]="0.300";
(*mm)["Thr"]="-0.400";(*mm)["Trp"]="-3.400";(*mm)["Tyr"]="-2.300";(*mm)["Val"]="-1.500";

mm=&m_bigmap["Hphob.Doolittle"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Hphob.Doolittle.html";
(*mm)["Amino"]="acid scale: Hydropathicity.";
(*mm)["Ala"]="1.800";(*mm)["Arg"]="-4.500";
(*mm)["Asn"]="-3.500";(*mm)["Asp"]="-3.500";(*mm)["Cys"]="2.500";(*mm)["Gln"]="-3.500";
(*mm)["Glu"]="-3.500";(*mm)["Gly"]="-0.400";(*mm)["His"]="-3.200";(*mm)["Ile"]="4.500";
(*mm)["Leu"]="3.800";(*mm)["Lys"]="-3.900";(*mm)["Met"]="1.900";(*mm)["Phe"]="2.800";
(*mm)["Pro"]="-1.600";(*mm)["Ser"]="-0.800";(*mm)["Thr"]="-0.700";(*mm)["Trp"]="-0.900";
(*mm)["Tyr"]="-1.300";(*mm)["Val"]="4.200";
mm=&m_bigmap["Hphob.Manavalan"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Hphob.Manavalan.html";
(*mm)["Amino"]="acid scale: Average surrounding hydrophobicity.";

(*mm)["Ala"]="12.970";(*mm)["Arg"]="11.720";(*mm)["Asn"]="11.420";(*mm)["Asp"]="10.850";
(*mm)["Cys"]="14.630";(*mm)["Gln"]="11.760";(*mm)["Glu"]="11.890";(*mm)["Gly"]="12.430";
(*mm)["His"]="12.160";(*mm)["Ile"]="15.670";(*mm)["Leu"]="14.900";(*mm)["Lys"]="11.360";
(*mm)["Met"]="14.390";(*mm)["Phe"]="14.000";(*mm)["Pro"]="11.370";(*mm)["Ser"]="11.230";
(*mm)["Thr"]="11.690";(*mm)["Trp"]="13.930";(*mm)["Tyr"]="13.420";(*mm)["Val"]="15.710";

mm=&m_bigmap["Hphob.Leo"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Hphob.Leo.html";
(*mm)["Amino"]="acid scale: Hydrophobicity (delta G1/2 cal)";(*mm)["Ala"]="0.440";(*mm)["Arg"]="-2.420";
(*mm)["Asn"]="-1.320";(*mm)["Asp"]="-0.310";(*mm)["Cys"]="0.580";(*mm)["Gln"]="-0.710";
(*mm)["Glu"]="-0.340";(*mm)["Gly"]="0.000";(*mm)["His"]="-0.010";(*mm)["Ile"]="2.460";
(*mm)["Leu"]="2.460";(*mm)["Lys"]="-2.450";(*mm)["Met"]="1.100";(*mm)["Phe"]="2.540";
(*mm)["Pro"]="1.290";(*mm)["Ser"]="-0.840";(*mm)["Thr"]="-0.410";(*mm)["Trp"]="2.560";
(*mm)["Tyr"]="1.630";(*mm)["Val"]="1.730";
mm=&m_bigmap["Hphob.Black"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Hphob.Black.html";
(*mm)["Amino"]="acid scale: Hydrophobicity of physiological L-alpha amino acids";

(*mm)["Ala"]="0.616";(*mm)["Arg"]="0.000";(*mm)["Asn"]="0.236";(*mm)["Asp"]="0.028";
(*mm)["Cys"]="0.680";(*mm)["Gln"]="0.251";(*mm)["Glu"]="0.043";(*mm)["Gly"]="0.501";
(*mm)["His"]="0.165";(*mm)["Ile"]="0.943";(*mm)["Leu"]="0.943";(*mm)["Lys"]="0.283";
(*mm)["Met"]="0.738";(*mm)["Phe"]="1.000";(*mm)["Pro"]="0.711";(*mm)["Ser"]="0.359";
(*mm)["Thr"]="0.450";(*mm)["Trp"]="0.878";(*mm)["Tyr"]="0.880";(*mm)["Val"]="0.825";

mm=&m_bigmap["Hphob.Breese"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Hphob.Breese.html";
(*mm)["Amino"]="acid scale: Hydrophobicity (free energy of transfer to";
(*mm)["Ala"]="0.610";(*mm)["Arg"]="0.690";
(*mm)["Asn"]="0.890";(*mm)["Asp"]="0.610";(*mm)["Cys"]="0.360";(*mm)["Gln"]="0.970";
(*mm)["Glu"]="0.510";(*mm)["Gly"]="0.810";(*mm)["His"]="0.690";(*mm)["Ile"]="-1.450";
(*mm)["Leu"]="-1.650";(*mm)["Lys"]="0.460";(*mm)["Met"]="-0.660";(*mm)["Phe"]="-1.520";
(*mm)["Pro"]="-0.170";(*mm)["Ser"]="0.420";(*mm)["Thr"]="0.290";(*mm)["Trp"]="-1.200";
(*mm)["Tyr"]="-1.430";(*mm)["Val"]="-0.750";
mm=&m_bigmap["Hphob.Fauchere"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Hphob.Fauchere.html";
(*mm)["Amino"]="acid scale: Hydrophobicity scale (pi-r).";

(*mm)["Ala"]="0.310";(*mm)["Arg"]="-1.010";(*mm)["Asn"]="-0.600";(*mm)["Asp"]="-0.770";
(*mm)["Cys"]="1.540";(*mm)["Gln"]="-0.220";(*mm)["Glu"]="-0.640";(*mm)["Gly"]="0.000";
(*mm)["His"]="0.130";(*mm)["Ile"]="1.800";(*mm)["Leu"]="1.700";(*mm)["Lys"]="-0.990";
(*mm)["Met"]="1.230";(*mm)["Phe"]="1.790";(*mm)["Pro"]="0.720";(*mm)["Ser"]="-0.040";
(*mm)["Thr"]="0.260";(*mm)["Trp"]="2.250";(*mm)["Tyr"]="0.960";(*mm)["Val"]="1.220";

mm=&m_bigmap["Hphob.Guy"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Hphob.Guy.html";
(*mm)["Amino"]="acid scale: Hydrophobicity scale based on free energy";
(*mm)["Ala"]="0.100";(*mm)["Arg"]="1.910";
(*mm)["Asn"]="0.480";(*mm)["Asp"]="0.780";(*mm)["Cys"]="-1.420";(*mm)["Gln"]="0.950";
(*mm)["Glu"]="0.830";(*mm)["Gly"]="0.330";(*mm)["His"]="-0.500";(*mm)["Ile"]="-1.130";
(*mm)["Leu"]="-1.180";(*mm)["Lys"]="1.400";(*mm)["Met"]="-1.590";(*mm)["Phe"]="-2.120";
(*mm)["Pro"]="0.730";(*mm)["Ser"]="0.520";(*mm)["Thr"]="0.070";(*mm)["Trp"]="-0.510";
(*mm)["Tyr"]="-0.210";(*mm)["Val"]="-1.270";
mm=&m_bigmap["Hphob.Janin"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Hphob.Janin.html";
(*mm)["Amino"]="acid scale: Free energy of transfer from inside";

(*mm)["Ala"]="0.300";(*mm)["Arg"]="-1.400";(*mm)["Asn"]="-0.500";(*mm)["Asp"]="-0.600";
(*mm)["Cys"]="0.900";(*mm)["Gln"]="-0.700";(*mm)["Glu"]="-0.700";(*mm)["Gly"]="0.300";
(*mm)["His"]="-0.100";(*mm)["Ile"]="0.700";(*mm)["Leu"]="0.500";(*mm)["Lys"]="-1.800";
(*mm)["Met"]="0.400";(*mm)["Phe"]="0.500";(*mm)["Pro"]="-0.300";(*mm)["Ser"]="-0.100";
(*mm)["Thr"]="-0.200";(*mm)["Trp"]="0.300";(*mm)["Tyr"]="-0.400";(*mm)["Val"]="0.600";

mm=&m_bigmap["Hphob.Miyazawa"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Hphob.Miyazawa.html";
(*mm)["Amino"]="acid scale: Hydrophobicity scale (contact energy derived from";
(*mm)["Ala"]="5.330";(*mm)["Arg"]="4.180";
(*mm)["Asn"]="3.710";(*mm)["Asp"]="3.590";(*mm)["Cys"]="7.930";(*mm)["Gln"]="3.870";
(*mm)["Glu"]="3.650";(*mm)["Gly"]="4.480";(*mm)["His"]="5.100";(*mm)["Ile"]="8.830";
(*mm)["Leu"]="8.470";(*mm)["Lys"]="2.950";(*mm)["Met"]="8.950";(*mm)["Phe"]="9.030";
(*mm)["Pro"]="3.870";(*mm)["Ser"]="4.090";(*mm)["Thr"]="4.490";(*mm)["Trp"]="7.660";
(*mm)["Tyr"]="5.890";(*mm)["Val"]="7.630";
mm=&m_bigmap["Hphob.Argos"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Hphob.Argos.html";
(*mm)["Amino"]="acid scale: Membrane buried helix parameter.";

(*mm)["Ala"]="1.360";(*mm)["Arg"]="0.150";(*mm)["Asn"]="0.330";(*mm)["Asp"]="0.110";
(*mm)["Cys"]="1.270";(*mm)["Gln"]="0.330";(*mm)["Glu"]="0.250";(*mm)["Gly"]="1.090";
(*mm)["His"]="0.680";(*mm)["Ile"]="1.440";(*mm)["Leu"]="1.470";(*mm)["Lys"]="0.090";
(*mm)["Met"]="1.420";(*mm)["Phe"]="1.570";(*mm)["Pro"]="0.540";(*mm)["Ser"]="0.970";
(*mm)["Thr"]="1.080";(*mm)["Trp"]="1.000";(*mm)["Tyr"]="0.830";(*mm)["Val"]="1.370";

mm=&m_bigmap["Hphob.Roseman"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Hphob.Roseman.html";
(*mm)["Amino"]="acid scale: Hydrophobicity scale (pi-r).";
(*mm)["Ala"]="0.390";(*mm)["Arg"]="-3.950";
(*mm)["Asn"]="-1.910";(*mm)["Asp"]="-3.810";(*mm)["Cys"]="0.250";(*mm)["Gln"]="-1.300";
(*mm)["Glu"]="-2.910";(*mm)["Gly"]="0.000";(*mm)["His"]="-0.640";(*mm)["Ile"]="1.820";
(*mm)["Leu"]="1.820";(*mm)["Lys"]="-2.770";(*mm)["Met"]="0.960";(*mm)["Phe"]="2.270";
(*mm)["Pro"]="0.990";(*mm)["Ser"]="-1.240";(*mm)["Thr"]="-1.000";(*mm)["Trp"]="2.130";
(*mm)["Tyr"]="1.470";(*mm)["Val"]="1.300";
mm=&m_bigmap["Hphob.Wolfenden"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Hphob.Wolfenden.html";
(*mm)["Amino"]="acid scale: Hydration potential (kcal/mole) at 25øC.";

(*mm)["Ala"]="1.940";(*mm)["Arg"]="-19.920";(*mm)["Asn"]="-9.680";(*mm)["Asp"]="-10.950";
(*mm)["Cys"]="-1.240";(*mm)["Gln"]="-9.380";(*mm)["Glu"]="-10.200";(*mm)["Gly"]="2.390";
(*mm)["His"]="-10.270";(*mm)["Ile"]="2.150";(*mm)["Leu"]="2.280";(*mm)["Lys"]="-9.520";
(*mm)["Met"]="-1.480";(*mm)["Phe"]="-0.760";(*mm)["Pro"]="0.000";(*mm)["Ser"]="-5.060";
(*mm)["Thr"]="-4.880";(*mm)["Trp"]="-5.880";(*mm)["Tyr"]="-6.110";(*mm)["Val"]="1.990";

mm=&m_bigmap["Hphob.Welling"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Hphob.Welling.html";
(*mm)["Amino"]="acid scale: Antigenicity value X 10.";
(*mm)["Ala"]="1.150";(*mm)["Arg"]="0.580";
(*mm)["Asn"]="-0.770";(*mm)["Asp"]="0.650";(*mm)["Cys"]="-1.200";(*mm)["Gln"]="-0.110";
(*mm)["Glu"]="-0.710";(*mm)["Gly"]="-1.840";(*mm)["His"]="3.120";(*mm)["Ile"]="-2.920";
(*mm)["Leu"]="0.750";(*mm)["Lys"]="2.060";(*mm)["Met"]="-3.850";(*mm)["Phe"]="-1.410";
(*mm)["Pro"]="-0.530";(*mm)["Ser"]="-0.260";(*mm)["Thr"]="-0.450";(*mm)["Trp"]="-1.140";
(*mm)["Tyr"]="0.130";(*mm)["Val"]="-0.130";
mm=&m_bigmap["Hphob.Wilson"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Hphob.Wilson.html";
(*mm)["Amino"]="acid scale: Hydrophobic constants derived from HPLC peptide";

(*mm)["Ala"]="-0.300";(*mm)["Arg"]="-1.100";(*mm)["Asn"]="-0.200";(*mm)["Asp"]="-1.400";
(*mm)["Cys"]="6.300";(*mm)["Gln"]="-0.200";(*mm)["Glu"]="0.000";(*mm)["Gly"]="1.200";
(*mm)["His"]="-1.300";(*mm)["Ile"]="4.300";(*mm)["Leu"]="6.600";(*mm)["Lys"]="-3.600";
(*mm)["Met"]="2.500";(*mm)["Phe"]="7.500";(*mm)["Pro"]="2.200";(*mm)["Ser"]="-0.600";
(*mm)["Thr"]="-2.200";(*mm)["Trp"]="7.900";(*mm)["Tyr"]="7.100";(*mm)["Val"]="5.900";

mm=&m_bigmap["Hphob.Parker"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Hphob.Parker.html";
(*mm)["Amino"]="acid scale: Hydrophilicity scale derived from HPLC peptide";
(*mm)["Ala"]="2.100";(*mm)["Arg"]="4.200";
(*mm)["Asn"]="7.000";(*mm)["Asp"]="10.000";(*mm)["Cys"]="1.400";(*mm)["Gln"]="6.000";
(*mm)["Glu"]="7.800";(*mm)["Gly"]="5.700";(*mm)["His"]="2.100";(*mm)["Ile"]="-8.000";
(*mm)["Leu"]="-9.200";(*mm)["Lys"]="5.700";(*mm)["Met"]="-4.200";(*mm)["Phe"]="-9.200";
(*mm)["Pro"]="2.100";(*mm)["Ser"]="6.500";(*mm)["Thr"]="5.200";(*mm)["Trp"]="-10.000";
(*mm)["Tyr"]="-1.900";(*mm)["Val"]="-3.700";
mm=&m_bigmap["Hphob.pH3.4"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Hphob.pH3.4.html";
(*mm)["Amino"]="acid scale: Hydrophobicity indices at ph 3.4 determined";

(*mm)["Ala"]="0.420";(*mm)["Arg"]="-1.560";(*mm)["Asn"]="-1.030";(*mm)["Asp"]="-0.510";
(*mm)["Cys"]="0.840";(*mm)["Gln"]="-0.960";(*mm)["Glu"]="-0.370";(*mm)["Gly"]="0.000";
(*mm)["His"]="-2.280";(*mm)["Ile"]="1.810";(*mm)["Leu"]="1.800";(*mm)["Lys"]="-2.030";
(*mm)["Met"]="1.180";(*mm)["Phe"]="1.740";(*mm)["Pro"]="0.860";(*mm)["Ser"]="-0.640";
(*mm)["Thr"]="-0.260";(*mm)["Trp"]="1.460";(*mm)["Tyr"]="0.510";(*mm)["Val"]="1.340";

mm=&m_bigmap["Hphob.pH7.5"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Hphob.pH7.5.html";
(*mm)["Amino"]="acid scale: Hydrophobicity indices at ph 7.5 determined";
(*mm)["Ala"]="0.350";(*mm)["Arg"]="-1.500";
(*mm)["Asn"]="-0.990";(*mm)["Asp"]="-2.150";(*mm)["Cys"]="0.760";(*mm)["Gln"]="-0.930";
(*mm)["Glu"]="-1.950";(*mm)["Gly"]="0.000";(*mm)["His"]="-0.650";(*mm)["Ile"]="1.830";
(*mm)["Leu"]="1.800";(*mm)["Lys"]="-1.540";(*mm)["Met"]="1.100";(*mm)["Phe"]="1.690";
(*mm)["Pro"]="0.840";(*mm)["Ser"]="-0.630";(*mm)["Thr"]="-0.270";(*mm)["Trp"]="1.350";
(*mm)["Tyr"]="0.390";(*mm)["Val"]="1.320";
mm=&m_bigmap["Hphob.mobility"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Hphob.mobility.html";
(*mm)["Amino"]="acid scale: Mobilities of amino acids on chromatography";

(*mm)["Ala"]="5.100";(*mm)["Arg"]="2.000";(*mm)["Asn"]="0.600";(*mm)["Asp"]="0.700";
(*mm)["Cys"]="0.000";(*mm)["Gln"]="1.400";(*mm)["Glu"]="1.800";(*mm)["Gly"]="4.100";
(*mm)["His"]="1.600";(*mm)["Ile"]="9.300";(*mm)["Leu"]="10.000";(*mm)["Lys"]="1.300";
(*mm)["Met"]="8.700";(*mm)["Phe"]="9.600";(*mm)["Pro"]="4.900";(*mm)["Ser"]="3.100";
(*mm)["Thr"]="3.500";(*mm)["Trp"]="9.200";(*mm)["Tyr"]="8.000";(*mm)["Val"]="8.500";

mm=&m_bigmap["HPLCHFBA"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/HPLCHFBA.html";
(*mm)["Amino"]="acid scale: Retention coefficient in HFBA.";
(*mm)["Ala"]="3.900";(*mm)["Arg"]="3.200";
(*mm)["Asn"]="-2.800";(*mm)["Asp"]="-2.800";(*mm)["Cys"]="-14.300";(*mm)["Gln"]="1.800";
(*mm)["Glu"]="-7.500";(*mm)["Gly"]="-2.300";(*mm)["His"]="2.000";(*mm)["Ile"]="11.000";
(*mm)["Leu"]="15.000";(*mm)["Lys"]="-2.500";(*mm)["Met"]="4.100";(*mm)["Phe"]="14.700";
(*mm)["Pro"]="5.600";(*mm)["Ser"]="-3.500";(*mm)["Thr"]="1.100";(*mm)["Trp"]="17.800";
(*mm)["Tyr"]="3.800";(*mm)["Val"]="2.100";
mm=&m_bigmap["HPLCTFA"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/HPLCTFA.html";
(*mm)["Amino"]="acid scale: Retention coefficient in TFA.";

(*mm)["Ala"]="7.300";(*mm)["Arg"]="-3.600";(*mm)["Asn"]="-5.700";(*mm)["Asp"]="-2.900";
(*mm)["Cys"]="-9.200";(*mm)["Gln"]="-0.300";(*mm)["Glu"]="-7.100";(*mm)["Gly"]="-1.200";
(*mm)["His"]="-2.100";(*mm)["Ile"]="6.600";(*mm)["Leu"]="20.000";(*mm)["Lys"]="-3.700";
(*mm)["Met"]="5.600";(*mm)["Phe"]="19.200";(*mm)["Pro"]="5.100";(*mm)["Ser"]="-4.100";
(*mm)["Thr"]="0.800";(*mm)["Trp"]="16.300";(*mm)["Tyr"]="5.900";(*mm)["Val"]="3.500";

mm=&m_bigmap["HPLC2.1"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/HPLC2.1.html";
(*mm)["Amino"]="acid scale: Retention coefficient in HPLC, pH 2.1.";
(*mm)["Ala"]="-0.100";(*mm)["Arg"]="-4.500";
(*mm)["Asn"]="-1.600";(*mm)["Asp"]="-2.800";(*mm)["Cys"]="-2.200";(*mm)["Gln"]="-2.500";
(*mm)["Glu"]="-7.500";(*mm)["Gly"]="-0.500";(*mm)["His"]="0.800";(*mm)["Ile"]="11.800";
(*mm)["Leu"]="10.000";(*mm)["Lys"]="-3.200";(*mm)["Met"]="7.100";(*mm)["Phe"]="13.900";
(*mm)["Pro"]="8.000";(*mm)["Ser"]="-3.700";(*mm)["Thr"]="1.500";(*mm)["Trp"]="18.100";
(*mm)["Tyr"]="8.200";(*mm)["Val"]="3.300";
mm=&m_bigmap["HPLC7.4"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/HPLC7.4.html";
(*mm)["Amino"]="acid scale: Retention coefficient in HPLC, pH 7.4.";

(*mm)["Ala"]="0.500";(*mm)["Arg"]="0.800";(*mm)["Asn"]="0.800";(*mm)["Asp"]="-8.200";
(*mm)["Cys"]="-6.800";(*mm)["Gln"]="-4.800";(*mm)["Glu"]="-16.900";(*mm)["Gly"]="0.000";
(*mm)["His"]="-3.500";(*mm)["Ile"]="13.900";(*mm)["Leu"]="8.800";(*mm)["Lys"]="0.100";
(*mm)["Met"]="4.800";(*mm)["Phe"]="13.200";(*mm)["Pro"]="6.100";(*mm)["Ser"]="1.200";
(*mm)["Thr"]="2.700";(*mm)["Trp"]="14.900";(*mm)["Tyr"]="6.100";(*mm)["Val"]="2.700";

mm=&m_bigmap["buriedresidues"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/buriedresidues.html";
(*mm)["Amino"]="acid scale: Molar fraction (%) of 2001 buried";
(*mm)["Ala"]="11.200";(*mm)["Arg"]="0.500";
(*mm)["Asn"]="2.900";(*mm)["Asp"]="2.900";(*mm)["Cys"]="4.100";(*mm)["Gln"]="1.600";
(*mm)["Glu"]="1.800";(*mm)["Gly"]="11.800";(*mm)["His"]="2.000";(*mm)["Ile"]="8.600";
(*mm)["Leu"]="11.700";(*mm)["Lys"]="0.500";(*mm)["Met"]="1.900";(*mm)["Phe"]="5.100";
(*mm)["Pro"]="2.700";(*mm)["Ser"]="8.000";(*mm)["Thr"]="4.900";(*mm)["Trp"]="2.200";
(*mm)["Tyr"]="2.600";(*mm)["Val"]="12.900";
mm=&m_bigmap["accessibleresidues"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/accessibleresidues.html";
(*mm)["Amino"]="acid scale: Molar fraction (%) of 3220 accessible";

(*mm)["Ala"]="6.600";(*mm)["Arg"]="4.500";(*mm)["Asn"]="6.700";(*mm)["Asp"]="7.700";
(*mm)["Cys"]="0.900";(*mm)["Gln"]="5.200";(*mm)["Glu"]="5.700";(*mm)["Gly"]="6.700";
(*mm)["His"]="2.500";(*mm)["Ile"]="2.800";(*mm)["Leu"]="4.800";(*mm)["Lys"]="10.300";
(*mm)["Met"]="1.000";(*mm)["Phe"]="2.400";(*mm)["Pro"]="4.800";(*mm)["Ser"]="9.400";
(*mm)["Thr"]="7.000";(*mm)["Trp"]="1.400";(*mm)["Tyr"]="5.100";(*mm)["Val"]="4.500";

mm=&m_bigmap["Hphob.Chothia"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Hphob.Chothia.html";
(*mm)["Amino"]="acid scale: Proportion of residues 95% buried (in";
(*mm)["Ala"]="0.380";(*mm)["Arg"]="0.010";
(*mm)["Asn"]="0.120";(*mm)["Asp"]="0.150";(*mm)["Cys"]="0.500";(*mm)["Gln"]="0.070";
(*mm)["Glu"]="0.180";(*mm)["Gly"]="0.360";(*mm)["His"]="0.170";(*mm)["Ile"]="0.600";
(*mm)["Leu"]="0.450";(*mm)["Lys"]="0.030";(*mm)["Met"]="0.400";(*mm)["Phe"]="0.500";
(*mm)["Pro"]="0.180";(*mm)["Ser"]="0.220";(*mm)["Thr"]="0.230";(*mm)["Trp"]="0.270";
(*mm)["Tyr"]="0.150";(*mm)["Val"]="0.540";
mm=&m_bigmap["Hphob.Rose"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Hphob.Rose.html";
(*mm)["Amino"]="acid scale: Mean fractional area loss (f) [average";

(*mm)["Ala"]="0.740";(*mm)["Arg"]="0.640";(*mm)["Asn"]="0.630";(*mm)["Asp"]="0.620";
(*mm)["Cys"]="0.910";(*mm)["Gln"]="0.620";(*mm)["Glu"]="0.620";(*mm)["Gly"]="0.720";
(*mm)["His"]="0.780";(*mm)["Ile"]="0.880";(*mm)["Leu"]="0.850";(*mm)["Lys"]="0.520";
(*mm)["Met"]="0.850";(*mm)["Phe"]="0.880";(*mm)["Pro"]="0.640";(*mm)["Ser"]="0.660";
(*mm)["Thr"]="0.700";(*mm)["Trp"]="0.850";(*mm)["Tyr"]="0.760";(*mm)["Val"]="0.860";

mm=&m_bigmap["Ratioside"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Ratioside.html";
(*mm)["Amino"]="acid scale: Atomic weight ratio of hetero elements";
(*mm)["Ala"]="0.000";(*mm)["Arg"]="0.650";
(*mm)["Asn"]="1.330";(*mm)["Asp"]="1.380";(*mm)["Cys"]="2.750";(*mm)["Gln"]="0.890";
(*mm)["Glu"]="0.920";(*mm)["Gly"]="0.740";(*mm)["His"]="0.580";(*mm)["Ile"]="0.000";
(*mm)["Leu"]="0.000";(*mm)["Lys"]="0.330";(*mm)["Met"]="0.000";(*mm)["Phe"]="0.000";
(*mm)["Pro"]="0.390";(*mm)["Ser"]="1.420";(*mm)["Thr"]="0.710";(*mm)["Trp"]="0.130";
(*mm)["Tyr"]="0.200";(*mm)["Val"]="0.000";
mm=&m_bigmap["Averageburied"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Averageburied.html";
(*mm)["Amino"]="acid scale: Average area buried on transfer from";

(*mm)["Ala"]="86.600";(*mm)["Arg"]="162.200";(*mm)["Asn"]="103.300";(*mm)["Asp"]="97.800";
(*mm)["Cys"]="132.300";(*mm)["Gln"]="119.200";(*mm)["Glu"]="113.900";(*mm)["Gly"]="62.900";
(*mm)["His"]="155.800";(*mm)["Ile"]="158.000";(*mm)["Leu"]="164.100";(*mm)["Lys"]="115.500";
(*mm)["Met"]="172.900";(*mm)["Phe"]="194.100";(*mm)["Pro"]="92.900";(*mm)["Ser"]="85.600";
(*mm)["Thr"]="106.500";(*mm)["Trp"]="224.600";(*mm)["Tyr"]="177.700";(*mm)["Val"]="141.000";

mm=&m_bigmap["Averageflexibility"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Averageflexibility.html";
(*mm)["Amino"]="acid scale: Average flexibility index.";
(*mm)["Ala"]="0.360";(*mm)["Arg"]="0.530";
(*mm)["Asn"]="0.460";(*mm)["Asp"]="0.510";(*mm)["Cys"]="0.350";(*mm)["Gln"]="0.490";
(*mm)["Glu"]="0.500";(*mm)["Gly"]="0.540";(*mm)["His"]="0.320";(*mm)["Ile"]="0.460";
(*mm)["Leu"]="0.370";(*mm)["Lys"]="0.470";(*mm)["Met"]="0.300";(*mm)["Phe"]="0.310";
(*mm)["Pro"]="0.510";(*mm)["Ser"]="0.510";(*mm)["Thr"]="0.440";(*mm)["Trp"]="0.310";
(*mm)["Tyr"]="0.420";(*mm)["Val"]="0.390";
mm=&m_bigmap["alpha-helixFasman"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/alpha-helixFasman.html";
(*mm)["Amino"]="acid scale: Conformational parameter for alpha helix (computed";

(*mm)["Ala"]="1.420";(*mm)["Arg"]="0.980";(*mm)["Asn"]="0.670";(*mm)["Asp"]="1.010";
(*mm)["Cys"]="0.700";(*mm)["Gln"]="1.110";(*mm)["Glu"]="1.510";(*mm)["Gly"]="0.570";
(*mm)["His"]="1.000";(*mm)["Ile"]="1.080";(*mm)["Leu"]="1.210";(*mm)["Lys"]="1.160";
(*mm)["Met"]="1.450";(*mm)["Phe"]="1.130";(*mm)["Pro"]="0.570";(*mm)["Ser"]="0.770";
(*mm)["Thr"]="0.830";(*mm)["Trp"]="1.080";(*mm)["Tyr"]="0.690";(*mm)["Val"]="1.060";

mm=&m_bigmap["beta-sheetFasman"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/beta-sheetFasman.html";
(*mm)["Amino"]="acid scale: Conformational parameter for beta-sheet (computed from";
(*mm)["Ala"]="0.830";(*mm)["Arg"]="0.930";
(*mm)["Asn"]="0.890";(*mm)["Asp"]="0.540";(*mm)["Cys"]="1.190";(*mm)["Gln"]="1.100";
(*mm)["Glu"]="0.370";(*mm)["Gly"]="0.750";(*mm)["His"]="0.870";(*mm)["Ile"]="1.600";
(*mm)["Leu"]="1.300";(*mm)["Lys"]="0.740";(*mm)["Met"]="1.050";(*mm)["Phe"]="1.380";
(*mm)["Pro"]="0.550";(*mm)["Ser"]="0.750";(*mm)["Thr"]="1.190";(*mm)["Trp"]="1.370";
(*mm)["Tyr"]="1.470";(*mm)["Val"]="1.700";
mm=&m_bigmap["beta-turnFasman"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/beta-turnFasman.html";
(*mm)["Amino"]="acid scale: Conformational parameter for beta-turn (computed from";

(*mm)["Ala"]="0.660";(*mm)["Arg"]="0.950";(*mm)["Asn"]="1.560";(*mm)["Asp"]="1.460";
(*mm)["Cys"]="1.190";(*mm)["Gln"]="0.980";(*mm)["Glu"]="0.740";(*mm)["Gly"]="1.560";
(*mm)["His"]="0.950";(*mm)["Ile"]="0.470";(*mm)["Leu"]="0.590";(*mm)["Lys"]="1.010";
(*mm)["Met"]="0.600";(*mm)["Phe"]="0.600";(*mm)["Pro"]="1.520";(*mm)["Ser"]="1.430";
(*mm)["Thr"]="0.960";(*mm)["Trp"]="0.960";(*mm)["Tyr"]="1.140";(*mm)["Val"]="0.500";

mm=&m_bigmap["alpha-helixRoux"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/alpha-helixRoux.html";
(*mm)["Amino"]="acid scale: Conformational parameter for alpha helix.";
(*mm)["Ala"]="1.489";(*mm)["Arg"]="1.224";
(*mm)["Asn"]="0.772";(*mm)["Asp"]="0.924";(*mm)["Cys"]="0.966";(*mm)["Gln"]="1.164";
(*mm)["Glu"]="1.504";(*mm)["Gly"]="0.510";(*mm)["His"]="1.003";(*mm)["Ile"]="1.003";
(*mm)["Leu"]="1.236";(*mm)["Lys"]="1.172";(*mm)["Met"]="1.363";(*mm)["Phe"]="1.195";
(*mm)["Pro"]="0.492";(*mm)["Ser"]="0.739";(*mm)["Thr"]="0.785";(*mm)["Trp"]="1.090";
(*mm)["Tyr"]="0.787";(*mm)["Val"]="0.990";
mm=&m_bigmap["beta-sheetRoux"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/beta-sheetRoux.html";
(*mm)["Amino"]="acid scale: Conformational parameter for beta-sheet.";

(*mm)["Ala"]="0.709";(*mm)["Arg"]="0.920";(*mm)["Asn"]="0.604";(*mm)["Asp"]="0.541";
(*mm)["Cys"]="1.191";(*mm)["Gln"]="0.840";(*mm)["Glu"]="0.567";(*mm)["Gly"]="0.657";
(*mm)["His"]="0.863";(*mm)["Ile"]="1.799";(*mm)["Leu"]="1.261";(*mm)["Lys"]="0.721";
(*mm)["Met"]="1.210";(*mm)["Phe"]="1.393";(*mm)["Pro"]="0.354";(*mm)["Ser"]="0.928";
(*mm)["Thr"]="1.221";(*mm)["Trp"]="1.306";(*mm)["Tyr"]="1.266";(*mm)["Val"]="1.965";

mm=&m_bigmap["beta-turnRoux"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/beta-turnRoux.html";
(*mm)["Amino"]="acid scale: Conformational parameter for beta-turn.";
(*mm)["Ala"]="0.788";(*mm)["Arg"]="0.912";
(*mm)["Asn"]="1.572";(*mm)["Asp"]="1.197";(*mm)["Cys"]="0.965";(*mm)["Gln"]="0.997";
(*mm)["Glu"]="1.149";(*mm)["Gly"]="1.860";(*mm)["His"]="0.970";(*mm)["Ile"]="0.240";
(*mm)["Leu"]="0.670";(*mm)["Lys"]="1.302";(*mm)["Met"]="0.436";(*mm)["Phe"]="0.624";
(*mm)["Pro"]="1.415";(*mm)["Ser"]="1.316";(*mm)["Thr"]="0.739";(*mm)["Trp"]="0.546";
(*mm)["Tyr"]="0.795";(*mm)["Val"]="0.387";
mm=&m_bigmap["CoilRoux"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/CoilRoux.html";
(*mm)["Amino"]="acid scale: Conformational parameter for coil.";

(*mm)["Ala"]="0.824";(*mm)["Arg"]="0.893";(*mm)["Asn"]="1.167";(*mm)["Asp"]="1.197";
(*mm)["Cys"]="0.953";(*mm)["Gln"]="0.947";(*mm)["Glu"]="0.761";(*mm)["Gly"]="1.251";
(*mm)["His"]="1.068";(*mm)["Ile"]="0.886";(*mm)["Leu"]="0.810";(*mm)["Lys"]="0.897";
(*mm)["Met"]="0.810";(*mm)["Phe"]="0.797";(*mm)["Pro"]="1.540";(*mm)["Ser"]="1.130";
(*mm)["Thr"]="1.148";(*mm)["Trp"]="0.941";(*mm)["Tyr"]="1.109";(*mm)["Val"]="0.772";

mm=&m_bigmap["alpha-helixLevitt"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/alpha-helixLevitt.html";
(*mm)["Amino"]="acid scale: Normalized frequency for alpha helix.";
(*mm)["Ala"]="1.290";(*mm)["Arg"]="0.960";
(*mm)["Asn"]="0.900";(*mm)["Asp"]="1.040";(*mm)["Cys"]="1.110";(*mm)["Gln"]="1.270";
(*mm)["Glu"]="1.440";(*mm)["Gly"]="0.560";(*mm)["His"]="1.220";(*mm)["Ile"]="0.970";
(*mm)["Leu"]="1.300";(*mm)["Lys"]="1.230";(*mm)["Met"]="1.470";(*mm)["Phe"]="1.070";
(*mm)["Pro"]="0.520";(*mm)["Ser"]="0.820";(*mm)["Thr"]="0.820";(*mm)["Trp"]="0.990";
(*mm)["Tyr"]="0.720";(*mm)["Val"]="0.910";
mm=&m_bigmap["beta-sheetLevitt"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/beta-sheetLevitt.html";
(*mm)["Amino"]="acid scale: Normalized frequency for beta-sheet.";

(*mm)["Ala"]="0.900";(*mm)["Arg"]="0.990";(*mm)["Asn"]="0.760";(*mm)["Asp"]="0.720";
(*mm)["Cys"]="0.740";(*mm)["Gln"]="0.800";(*mm)["Glu"]="0.750";(*mm)["Gly"]="0.920";
(*mm)["His"]="1.080";(*mm)["Ile"]="1.450";(*mm)["Leu"]="1.020";(*mm)["Lys"]="0.770";
(*mm)["Met"]="0.970";(*mm)["Phe"]="1.320";(*mm)["Pro"]="0.640";(*mm)["Ser"]="0.950";
(*mm)["Thr"]="1.210";(*mm)["Trp"]="1.140";(*mm)["Tyr"]="1.250";(*mm)["Val"]="1.490";

mm=&m_bigmap["beta-turnLevitt"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/beta-turnLevitt.html";
(*mm)["Amino"]="acid scale: Normalized frequency for beta-turn.";
(*mm)["Ala"]="0.770";(*mm)["Arg"]="0.880";
(*mm)["Asn"]="1.280";(*mm)["Asp"]="1.410";(*mm)["Cys"]="0.810";(*mm)["Gln"]="0.980";
(*mm)["Glu"]="0.990";(*mm)["Gly"]="1.640";(*mm)["His"]="0.680";(*mm)["Ile"]="0.510";
(*mm)["Leu"]="0.580";(*mm)["Lys"]="0.960";(*mm)["Met"]="0.410";(*mm)["Phe"]="0.590";
(*mm)["Pro"]="1.910";(*mm)["Ser"]="1.320";(*mm)["Thr"]="1.040";(*mm)["Trp"]="0.760";
(*mm)["Tyr"]="1.050";(*mm)["Val"]="0.470";
mm=&m_bigmap["Totalbeta-strand"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Totalbeta-strand.html";
(*mm)["Amino"]="acid scale: Conformational preference for total beta strand";

(*mm)["Ala"]="0.920";(*mm)["Arg"]="0.930";(*mm)["Asn"]="0.600";(*mm)["Asp"]="0.480";
(*mm)["Cys"]="1.160";(*mm)["Gln"]="0.950";(*mm)["Glu"]="0.610";(*mm)["Gly"]="0.610";
(*mm)["His"]="0.930";(*mm)["Ile"]="1.810";(*mm)["Leu"]="1.300";(*mm)["Lys"]="0.700";
(*mm)["Met"]="1.190";(*mm)["Phe"]="1.250";(*mm)["Pro"]="0.400";(*mm)["Ser"]="0.820";
(*mm)["Thr"]="1.120";(*mm)["Trp"]="1.540";(*mm)["Tyr"]="1.530";(*mm)["Val"]="1.810";

mm=&m_bigmap["Antiparallelbeta-strand"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Antiparallelbeta-strand.html";
(*mm)["Amino"]="acid scale: Conformational preference for antiparallel beta strand.";
(*mm)["Ala"]="0.900";(*mm)["Arg"]="1.020";
(*mm)["Asn"]="0.620";(*mm)["Asp"]="0.470";(*mm)["Cys"]="1.240";(*mm)["Gln"]="1.180";
(*mm)["Glu"]="0.620";(*mm)["Gly"]="0.560";(*mm)["His"]="1.120";(*mm)["Ile"]="1.540";
(*mm)["Leu"]="1.260";(*mm)["Lys"]="0.740";(*mm)["Met"]="1.090";(*mm)["Phe"]="1.230";
(*mm)["Pro"]="0.420";(*mm)["Ser"]="0.870";(*mm)["Thr"]="1.300";(*mm)["Trp"]="1.750";
(*mm)["Tyr"]="1.680";(*mm)["Val"]="1.530";
mm=&m_bigmap["Parallelbeta-strand"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Parallelbeta-strand.html";
(*mm)["Amino"]="acid scale: Conformational preference for parallel beta strand.";

(*mm)["Ala"]="1.000";(*mm)["Arg"]="0.680";(*mm)["Asn"]="0.540";(*mm)["Asp"]="0.500";
(*mm)["Cys"]="0.910";(*mm)["Gln"]="0.280";(*mm)["Glu"]="0.590";(*mm)["Gly"]="0.790";
(*mm)["His"]="0.380";(*mm)["Ile"]="2.600";(*mm)["Leu"]="1.420";(*mm)["Lys"]="0.590";
(*mm)["Met"]="1.490";(*mm)["Phe"]="1.300";(*mm)["Pro"]="0.350";(*mm)["Ser"]="0.700";
(*mm)["Thr"]="0.590";(*mm)["Trp"]="0.890";(*mm)["Tyr"]="1.080";(*mm)["Val"]="2.630";

mm=&m_bigmap["A.A.composition"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/A.A.composition.html";
(*mm)["Amino"]="acid scale: Overall amino acid composition (%).";
(*mm)["Ala"]="8.300";(*mm)["Arg"]="5.700";
(*mm)["Asn"]="4.400";(*mm)["Asp"]="5.300";(*mm)["Cys"]="1.700";(*mm)["Gln"]="4.000";
(*mm)["Glu"]="6.200";(*mm)["Gly"]="7.200";(*mm)["His"]="2.200";(*mm)["Ile"]="5.200";
(*mm)["Leu"]="9.000";(*mm)["Lys"]="5.700";(*mm)["Met"]="2.400";(*mm)["Phe"]="3.900";
(*mm)["Pro"]="5.100";(*mm)["Ser"]="6.900";(*mm)["Thr"]="5.800";(*mm)["Trp"]="1.300";
(*mm)["Tyr"]="3.200";(*mm)["Val"]="6.600";
mm=&m_bigmap["A.A.Swiss-Prot"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/A.A.Swiss-Prot.html";
(*mm)["Amino"]="acid scale: Amino acid composition (%) in the";

(*mm)["Ala"]="7.890";(*mm)["Arg"]="5.400";(*mm)["Asn"]="4.130";(*mm)["Asp"]="5.350";
(*mm)["Cys"]="1.500";(*mm)["Gln"]="3.950";(*mm)["Glu"]="6.670";(*mm)["Gly"]="6.960";
(*mm)["His"]="2.290";(*mm)["Ile"]="5.900";(*mm)["Leu"]="9.650";(*mm)["Lys"]="5.920";
(*mm)["Met"]="2.380";(*mm)["Phe"]="3.960";(*mm)["Pro"]="4.830";(*mm)["Ser"]="6.830";
(*mm)["Thr"]="5.410";(*mm)["Trp"]="1.130";(*mm)["Tyr"]="3.030";(*mm)["Val"]="6.730";

mm=&m_bigmap["Relativemutability"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/Relativemutability.html";
(*mm)["Amino"]="acid scale: Relative mutability of amino acids (Ala=100).";
(*mm)["Ala"]="100.000";(*mm)["Arg"]="65.000";
(*mm)["Asn"]="134.000";(*mm)["Asp"]="106.000";(*mm)["Cys"]="20.000";(*mm)["Gln"]="93.000";
(*mm)["Glu"]="102.000";(*mm)["Gly"]="49.000";(*mm)["His"]="66.000";(*mm)["Ile"]="96.000";
(*mm)["Leu"]="40.000";(*mm)["Lys"]="56.000";(*mm)["Met"]="94.000";(*mm)["Phe"]="41.000";
(*mm)["Pro"]="56.000";(*mm)["Ser"]="120.000";(*mm)["Thr"]="97.000";(*mm)["Trp"]="18.000";
(*mm)["Tyr"]="41.000";(*mm)["Val"]="74.000";
mm=&m_bigmap["protscale_help"];
(*mm)["link"]="http://www.expasy.org/tools/pscale/protscale_help.html";



}





