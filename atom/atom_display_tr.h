#ifndef ATOM_DISPLAY_TR_H__
#define ATOM_DISPLAY_TR_H__

#include "../data_and_config.h"
#include "../atom/connectivity_list.h"
#include "atoms.h" // pickup the proximity classes

class atom_display_tr {
	public: 
	typedef data_and_config DaCoTy;
	typedef DaCoTy::string_type string_type;
	typedef DaCoTy::name_type name_type;
	typedef DaCoTy::Ch Ch;
	typedef int index_type;
	typedef atom_list_proximity ALPTy;
	typedef ALPTy::figure_param FigTy;
	typedef ALPTy::BPLPTy BPLPTy; 
	
	typedef DaCoTy::ConvStreamTy ConvStreamTy;
	typedef DaCoTy::OsStreamTy OsStreamTy;
	
	typedef connectivity_list ConnListTy;
	
	};


#endif
