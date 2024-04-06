#ifndef PDB_PROC_H___
#define PDB_PROC_H___

#include "global.h"


class pdb_typedefs
{	
public:
	typedef std::ostream OsTy;
	typedef std::string St;
	typedef int IdxTy;
	typedef int api_type;
	enum {ALL=~0 };
	
	pdb_typedefs(): m_debug(0) {}
	OsTy & eos() { return std::cerr; } 
	OsTy & dos() { return std::cout; } 
	bool debug() const { return m_debug!=0; }
	bool verbose() const { return m_debug==ALL; }
	
	static St Ldr(const IdxTy i)  {St x="    "; IdxTy j=i; while( 0!=(j--)) x+=" "; return x;} 
	static St Trl()  { return "\n"; }
	
	IdxTy m_debug;
	
};

class pdb_proc : public pdb_typedefs
{
	typedef pdb_proc Myt;
public:	
	int main ( int argc, char ** argv);


private:
template <class Os> static void rhelp( Os& os)
{
		os<<Ldr(0)<<"Usage: prb_proc" <<Trl();
		os<<Ldr(1)<<"Utility to load and examine spatial reslationships in pdb files." <<Trl();
}
template <class Os> static void credit_and_ref( Os& os)
{// nominally for human consumption but not formatted, should parse ok 
os<<Ldr(0)<<MM_MARK<<" Built on "<<__DATE__<<" at "<<__TIME__<<Trl();
os<<Ldr(0)<<"Contact: Mike Marchywka, marchywka@hotmail.com" <<Trl();
}
	
};


#endif





