#ifndef PDB_TYPEDEFS_H___
#define PDB_TYPEDEFS_H___

#include "global.h"

#include <fstream>
#include <strings.h>
#include <string.h>
#include <string>

class pdb_typedefs
{	
public:
	typedef std::ostream OsTy;
	typedef std::istream IsTy;
	typedef char Chr;
	typedef std::string St;
	typedef St string_type; 
	typedef int IdxTy;
	typedef int api_type;
	enum {ALL=~0,FAILCODE=1 };
	
	pdb_typedefs():m_fail_code(FAILCODE), m_debug(0), m_out(0), m_err(0),m_own(true) {}
	~pdb_typedefs() { if (!m_own) return; delete m_out; delete m_err;}
	OsTy & eos() { if ( m_err==0) return std::cerr; return *m_err; } 
	OsTy & dos() { if (m_out==0) return std::cout;  return *m_out; } 
	bool debug() const { return m_debug!=0; }
	bool verbose() const { return m_debug==ALL; }
	// continue on fail should return a zero here, kind of a kluge perhaps...
	api_type failcode() const { return m_fail_code; }
	void continue_on_error(const bool c) {  m_fail_code=c?0:FAILCODE;    }
	IsTy * open_in ( const Chr * nm) 
	{ IsTy * is= new std::ifstream(nm); 
		if (!*is)
		{	eos()<<MM_MARK<<" can't open input file "<<nm<<Trl();  delete is; return 0; } 
		return is; 
	}
	OsTy * open_out ( const Chr * nm, OsTy *& out) 
	{ 	if (strcmp(nm,"-")==0) {  delete out; out=0;    return 0; }
		OsTy * os= new std::ofstream(nm); 
		if (!*os)
		{
			eos()<<MM_MARK<<" can't open output file "<<nm<<Trl(); 
			delete os; return 0; 
		}
		if (out!=0) { delete out; }
		out=os; 
		return os; 
	}
	static St Ldr(const IdxTy i)  {St x="    "; IdxTy j=i; while( 0!=(j--)) x+=" "; return x;} 
	static St Trl()  { return "\n"; }
	api_type m_fail_code;
	IdxTy m_debug;
	OsTy  *m_out,* m_err;
	bool m_own;
};
#endif
