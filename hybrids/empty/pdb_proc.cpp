


#include "global.h"
#include "command_map.h"
#include "pdb_proc.h"


typedef pdb_proc MyClass;
typedef MyClass::api_type api_type;



int MyClass::main ( int argc, char ** argv)
{
	int i=1;
	if ( r_cmd.size()==0)
	{
		push_cmd("-debug",&Myt::m_debug,1,ALL); 
		push_cmd("-v",&Myt::m_debug,1,1);
		push_cmd("-q",&Myt::m_debug,2,(0));
		
		/*
	push_cmd("-flags",&Myt::m_flags,0);
	push_cmd("-doall",&Myt::m_flags,1,DOALLFLAG);
	push_cmd("-doone",&Myt::m_flags,5,~DOALLFLAG);
	push_cmd("-all_samples",&Myt::m_flags,1,DOALLSAMPLESFLAG);
	push_cmd("-one_samples",&Myt::m_flags,5,~DOALLSAMPLESFLAG);
	push_cmd("-scatter",&Myt::m_flags,1,DOSCATTERFLAG);
	push_cmd("-no_scatter",&Myt::m_flags,5,~DOSCATTERFLAG);
	push_cmd("-marked",&Myt::m_flags,1,MARKEDFLAG);
	push_cmd("-no_marks",&Myt::m_flags,5,~MARKEDFLAG);
	typedef api_type (Myt::* stupid)(Chr * const  );
	static stupid fn[]={	&Myt::LoadRules,&Myt::LoadSeqs,&Myt::Align
							,&Myt::UseRule,&Myt::DumpAlignment,&Myt::RuleStats
							, &Myt::LoadMultiRules, &Myt::Alignss,&Myt::StringAlign,
							&Myt::AddRule,&Myt::ClearRules,&Myt::Coverage,
							&Myt::MergeMultiRules,&Myt::DistinctiveRuleAlign
						    ,&Myt::LoadPairs,&Myt::StringPAirAlign,&Myt::PairParams
						    ,&Myt::ClusterBluster,&Myt::Dest,&Myt::RuleSubsets,
						    &Myt::RefineOptions
					};
	
	push_cmd("-ref",&Myt::m_ref,0); push_cmd("-sample",&Myt::m_sample,0);
	push_cmd("-szlimit",&Myt::m_szlimit,0);
	push_cmd("-nocat",&Myt::m_cat,1,1);
	push_cmd("-rules",&fn[0],3,0); push_cmd("-fastas",&fn[1],3,0);
	push_cmd("-align",&fn[2],4,0); push_cmd("-use_rule",&fn[3],3,0);
	push_cmd("-output",&fn[4],3,0); push_cmd("-stats",&fn[5],4,0);
	push_cmd("-mrules",&fn[6],3,0); push_cmd("-alignss",&fn[7],4,0);
	push_cmd("-string_align",&fn[8],3,0);push_cmd("-add_rule",&fn[9],3,0);
	push_cmd("-clear_rules",&fn[10],4,0);push_cmd("-coverage",&fn[11],3,0);
	push_cmd("-merge_mrules",&fn[12],3,0);
	push_cmd("-cover_limit",&Myt::m_cover,0);
	push_cmd("-sort_rules",&Myt::DoRulesSort);
	push_cmd("-rare_align",&fn[13],3,0);
	push_cmd("-pair_rules",&fn[14],3,0);
	push_cmd("-pair_align",&fn[15],3,0);
	push_cmd("-pair_params",&fn[16],3,0);
	push_cmd("-cluster",&fn[17],3,0);
	push_cmd("-dest",&fn[18],3,0);
	push_cmd("-uniques",&fn[19],3,0);
	push_cmd("-refine",&fn[20],3,0);
	*/
}

	if ( argc<2) { rhelp(eos()); return -1; } 
	while (i<argc)
	{
		if (::main(*this,argc,argv,i)==0) {} 
		else if ( strcmp(argv[i],"-source")==0) 
		{	int x=0;
			if ((x=cmd_source(this,argv[++i]))!=0) return x; 
		}
		else if ( strcmp(argv[i],"-about")==0) {credit_and_ref(dos()) ; }
		else { eos()<<"Unable to parse "<<argv[i]<<'\n'; return -1; }
		++i;
	}
	return 0;	
}	






#ifdef STANDALONE

int main ( int argc, char ** argv) { return MyClass().main(argc,argv);	}

#endif


