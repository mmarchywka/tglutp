#ifndef ANNOTATION_HANDLER_H__
#define ANNOTATION_HANDLER_H__


#include "global.h"
#include "../base/points_base_classes.h"

#include <vector>
#include <string>
// This is a general purpose junk bin for stuff that you can put into the
// model coordinates or screen coordinates and provide general cataloging
// of stuff. 
#include "debug_and_support.h"
//#ifndef MM_MARK
//#define MM_MARK __FILE__<<" "<<__LINE__
//#endif

class annotation_junk;

class annotation_handler
{
	typedef annotation_handler Myt;
	typedef annotation_junk Myp;
	typedef char Chr;
	typedef std::basic_string<Chr> StrTy;	
	
public:
	enum {OK=0 };
	typedef float coord_type;
	typedef int api_type; 
	// this was a toss up between int and string, but speed may matter here
	// even if just for UI right now. 
	typedef int handle_type;
	typedef int op_code_type;
	typedef StrTy name_type;
	//typedef Point3 P3; 
	typedef point3_template<coord_type> P3;
	typedef myPoint4<coord_type> P4;
	typedef annotation_junk junk_type;
	annotation_handler(); 
	virtual ~annotation_handler();// {delete m_ptr; }
	// called during glutp redraw callback. 
	virtual api_type redraw();// { return api_type(OK); }
	template <class Tx> api_type annotate( Tx & cmd)
	{
		StrTy opp;
		m_s.clear();
		cmd>>opp; // first, get the opcode.
		std::cerr<<MM_MARK<<opp<<'\n';
		if (( opp=="help")||(opp=="?"))
		{help(std::cout); return 0; }
		//	m_s.push_back(opp);
		op(opp); while (opp!="") 
		{
			m_s.push_back(opp); 
			std::cerr<<MM_MARK<<opp<<'\n';
		 	opp=""; cmd>>opp; operand(opp);    
		}
		
		return exec();
	}
	template <class Os> void help(Os & os)
	{	StrTy e="\n";
		os<<MM_MARK<<" Available commands for annotation:"+e;
		os<<"	dump	: list all current annotations "+e;
		os<<"   ax	x y z l r g b a : make xyz axis at {xyz} or length l and color {rgba}"+e;
		os<<" 	label x y z w1 w2 ... : put text label at {xyz} using default color and scale"+e;
		os<<"	text x y w1 w2 ... : put text on screen at {xy} as above "+e;
		os<<" 	image x y fn : put bitmap file fn at screen location {xy}"+e;
		os<<"   color r g b a: set default color"+e;
		os<<"   scale s : set default scale"+e;
		os<<"  Screen is maintained as +/-512 in both directions, world is up to user"+e;
		
		
	} 
	handle_type define_area(const name_type & name, const name_type & type );
	api_type op(const StrTy & opcode); 
	api_type operand(const StrTy & opnd); 
	api_type op(const op_code_type & opcode );
	api_type exec(); 
private:
	Myp* m_ptr;
	std::vector<StrTy> m_s;
	
	double atof(const std::string & c)
{ return ::atof(c.c_str());  }
	
};

#endif


