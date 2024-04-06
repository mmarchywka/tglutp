// this is going to be a search order nightmare...
#include "global.h"

#include "pdb_proc.h"

// need to move the word array thing if it works...
#include "command_map.h"
#include "word_array.h"

typedef pdb_proc MyClass;
typedef MyClass::api_type api_type;



int MyClass::main ( int argc, char ** argv)
{
	int i=1;
	if ( r_cmd.size()==0)
	{
		// I cleverly delegated the debug function to all the members, so this
		// is awkward now... 
//		push_cmd("-debug",&Myt::m_debug,1,ALL); 
//		push_cmd("-v",&Myt::m_debug,1,1);
//		push_cmd("-q",&Myt::m_debug,2,(0));
		push_cmd("-probe_radius",&Myt::m_probe_radius,0);
		push_cmd("-visible",&Myt::m_visible,0);
		push_cmd("-shake",&Myt::m_shake,0);
		push_cmd("-dmax",&Myt::m_dmax,0);
//		push_cmd("-pfx",&Myt::m_pfx,0);
		
	push_cmd("-flags",&Myt::m_flags,0);
	push_cmd("-atomxlate",&Myt::m_enclave_format,0);
	/*
	push_cmd("-doall",&Myt::m_flags,1,DOALLFLAG);
	push_cmd("-doone",&Myt::m_flags,5,~DOALLFLAG);
	push_cmd("-all_samples",&Myt::m_flags,1,DOALLSAMPLESFLAG);
	push_cmd("-one_samples",&Myt::m_flags,5,~DOALLSAMPLESFLAG);
	push_cmd("-scatter",&Myt::m_flags,1,DOSCATTERFLAG);
	push_cmd("-no_scatter",&Myt::m_flags,5,~DOSCATTERFLAG);
	push_cmd("-marked",&Myt::m_flags,1,MARKEDFLAG);
	push_cmd("-no_marks",&Myt::m_flags,5,~MARKEDFLAG);
	*/
	typedef api_type (Myt::* stupid)(Chr * const  );
	static stupid fn[]={	&Myt::LoadPdb,
							&Myt::SetColors,
							&Myt::SetRadius,
							&Myt::DoCatalog,
							&Myt::DoMake,
							&Myt::DoSurface,
							&Myt::ShoVis,
							&Myt::ShoMesh,
							&Myt::SetOut,
							&Myt::SetErr,
							&Myt::ShoNorm,
							&Myt::DoList,
							&Myt::SetDebug,
							&Myt::ShoGrid,
							&Myt::ShoEdges,
							&Myt::DumpEnclaves,
							&Myt::SetPfx,
							&Myt::DoMutate
					};
	push_cmd("-pdb",&fn[0],3,0);
	push_cmd("-color_file",&fn[1],3,0);
	push_cmd("-radius_file",&fn[2],3,0);
	push_cmd("-cat",&fn[3],3,0);
	push_cmd("-make",&fn[4],3,0);
	push_cmd("-surf",&fn[5],3,0);
	push_cmd("-vis",&fn[6],3,0);
	push_cmd("-mesh",&fn[7],3,0);
	push_cmd("-out",&fn[8],3,0);
	push_cmd("-err",&fn[9],3,0);
	push_cmd("-norm",&fn[10],3,0);
	push_cmd("-list",&fn[11],3,0);
	push_cmd("-v",&fn[12],3,0);
	push_cmd("-grid",&fn[13],3,0);
	push_cmd("-edges",&fn[14],3,0);
	push_cmd("-enclaves",&fn[15],3,0);
	push_cmd("-pfx",&fn[16],3,0);
	push_cmd("-mutate",&fn[17],3,0);
	/*
	push_cmd("-ref",&Myt::m_ref,0); push_cmd("-sample",&Myt::m_sample,0);
	push_cmd("-szlimit",&Myt::m_szlimit,0);
	push_cmd("-nocat",&Myt::m_cat,1,1);
	 push_cmd("-fastas",&fn[1],3,0);
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
		else if ( strcmp(argv[i],"-interactive")==0) 
		{	int x=0;
			word_array<256> wa(2048);
				int linect=0;
			while (true)
			{
				wa.read_normaler(std::cin, & eos(), ++linect);
				int sz=wa.mindex();
				if (sz==0) break;
				char * argg[sz+1];
				argg[0]="console";
				for ( int i=0; i<sz; ++i) argg[i+1]=wa.pi(i);
				//int ii=0;
				//while ((x==0)) 
				{x=main(sz+1,argg); }
				if ( x!=0) { eos()<<MM_MARK<<" interactive error,cr to terminate\n"; //return x;  
				}
			}
		//	if ((x=cmd_source(this,argv[++i]))!=0) return x; 
		
		}
		else if ( strcmp(argv[i],"-about")==0) {credit_and_ref(dos()) ; }
		else if ((strcmp(argv[i],"-add")==0)&&(argc>(i+2))) 
		{ m_plo.add(argv[i+1],argv[i+2]); i+=2;}
		else if ((strcmp(argv[i],"-op")==0)&&(argc>(i+2))) 
		{ m_plo.operate(argv[i+1],argv[i+2]); i+=2;}
		else if ((strcmp(argv[i],"-center")==0)&&(argc>(i+3))) 
		{ m_x=atof(argv[i+1]);m_y=atof(argv[i+2]);m_z=atof(argv[i+3]);  i+=3;}
		else if ((strcmp(argv[i],"-angles")==0)&&(argc>(i+3))) 
		{ m_a=atof(argv[i+1]);m_b=atof(argv[i+2]);m_c=atof(argv[i+3]);  i+=3;}
		else { eos()<<"Unable to parse "<<argv[i]<<'\n'; return -1; }
		++i;
	}
	return 0;	
}	






#ifdef STANDALONE

int main ( int argc, char ** argv) { return MyClass().main(argc,argv);	}

#endif


