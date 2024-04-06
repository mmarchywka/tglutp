#ifndef BASIC_COMMAND_SHELL_H__
#define BASIC_COMMAND_SHELL_H__

#include "../base/basic_context_stack.h"
#include "mjm_globals.h"

#include <string>
#include <queue>
#include <stack>
#include <iostream>
// "normal" commands containing the variable_symbol($) are expanded
// and expressions evaluated [ need type information and an expr parser ]
// commands begin with cmd_symbol($) and are processed by interpretter
// possibly spawning a scheduled execution ( so screen can be updated during exec)
// or changing the mode so normal commands are not passed back for immediate exec.
// example $sub foo($x, $y, $z)
// the command sub loads a subroutein until $end
// $call foo($x,$y,$z)
// $let $a=$b
//  $if 
//
class basic_command_shell: public context_typedefs
{
	typedef basic_command_shell Myt;
	typedef unsigned char ExtCh;
	typedef std::basic_ostream<Ch> ostream_type;
	typedef std::basic_istream<Ch> istream_type;
	class stream_def { public: 
	stream_def(istream_type* s, string_type n,index_type l):ss(s),nm(n),level(l) {}
	istream_type* ss; string_type nm; index_type level;}; 
	typedef std::stack<stream_def> param_stream_stack_type;
	typedef ContextInfo::SymTy SymTy ;
	typedef MethodClass MethTy;
	typedef std::deque<StTy> QueTy;
	
public:
	basic_command_shell():  
		m_que(),m_methods(),m_main(),m_stack(),m_callback(false), m_param_stream_stack() 
	// creatre the global context
	{ // need a skeleton method...
		// the map pts aren't stable at global scope...
		m_main.code.push_back("");
		m_stack.push_back(ContextInfo(&m_main,SymTy(),SymTy())); 
	
	}
	
	
	typedef SymTy symbols_type; 
	// called on all input source new line- keyboard, source etc.
	// expand the line with variable subs and pass back or
	// execute and don't pass back. 
	void translate(const string_type & raw_input) 
	{m_que.push_back(raw_input);}
	// after calling translate, the user checks to see if we passed anything
	// back from the command que. 
	string_type pending() 
	{
		if (m_que.size()==0) return ""; 
		StTy x=Expand(m_que.front()); m_que.pop_front(); 
	//	mmdd::dout(MMDDLOC)<<"Try to e "<<x<<".\n";
		return x; 
	}
	SymTy & symbols() { return m_stack.back().syms ;  }
	bool callback() 
	{ if (!m_callback) return false; m_callback=false;return true; }

// we are now scheduled to execute at with no further input source
// we are the source until we run out of stuff by returning zero.
// execution continues until the screen needs to be updated. 
template <class Ty> index_type foo(Ty * src)
{
	// This was called because we are executing a subroutine. 
	// with or without stremaing parameters. 
	StTy line=Step();
//	mmdd::dout(MMDDLOC)<<"foo geets "<<line<<"\n";
	if ( line.length()>0 ) src->DoString(line); else return 0; 
	return 1;
	}
private:
	void BuildSub(StTy & in);
	StTy Step(); 
	StTy Subscript(StTy & xx, const StTy & sub);
	StTy Evaluate(StTy & in);
	StTy JustSubstitute(StTy & in);
	StTy tok(StTy& in);
	// assignment, conditional, expression,parameter
	StTy Expand(StTy & in);
	StTy Dump(StTy & in); // dump context info somewhere
	StTy Let(StTy & in);  // assignment 
	// Added May 2008 a simple file "read" to assign a "things" from a file
	// to a list of variables. No current notion of an open file however.
	StTy ReadSome(StTy & in); 
	StTy Cond(StTy & in);  // assignment 
	StTy Clear(StTy & in);  // assignment 
	StTy Pollute(StTy & in);  // export variables 
	StTy Sub(StTy & in);  // begin subroutine
	StTy End(StTy & in);  // end subroutine definition
	StTy Call(StTy & in); // call subtourtine
	StTy Stream(StTy & in); // stream subroutine parameters
	StTy StreamAgain();
	bool Stream(StTy & name, istream_type * ss);
	StTy Echo(StTy & in);
	StTy remove_formatting(const StTy & v);
	void ReturnLocalValues();
	void Call(const StTy & nm,  SymTy & params);
	index_type Return(SymTy & params);
	StTy  FindVariable(const StTy & nm);
	void StoreVariable(const StTy & nm,const StTy & val);

	// execution que
	QueTy m_que;
	// method defintions
	MethodMapTy m_methods;
	MethTy m_main; 
	// local context stack
	ContextStackTy m_stack;
	 
	bool m_callback;
	param_stream_stack_type m_param_stream_stack;
	
	class StateInfo
	{public: StateInfo(): sub(),state(0) {}
		string_type sub;
		index_type state; // 1== pshing sub,
		} m_si;
	
};

#endif
