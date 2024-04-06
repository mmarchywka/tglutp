#ifndef TEXT_COMMANDS_EXTENSION_H__
#define TEXT_COMMANDS_EXTENSION_H__


#include "../base/cmd_line_input.h"
#include "glut_mol_gui.h"
#include "camera_and_lights.h"
#include "../base/points_base_classes.h"

#include "../base/basic_command_shell.h"


class text_commands;
class text_commands_extension
{
protected:
	typedef text_commands Myt;
	typedef unsigned char SigCh;
	typedef char CmpCh;
	typedef std::basic_string< CmpCh> console_type;
	typedef istringstream string_parse_type; 
	typedef console_type::size_type size_type;
	typedef unsigned int count_type;
	typedef float ParamTy;

	typedef void ( *xconsole_func_type)(SigCh key);
//	double (Foo::*funcPtr)( long ) = &Foo::One; 	Foo aFoo; 	
//	double result =(aFoo.*funcPtr)( 2 );
//typedef long (Test::*Multiplier)() const;
	typedef camera_and_lights CameraTy;
	typedef void (CameraTy::*ctrl_ptr_type)(float);
	typedef void (CameraTy::*ctrl_ptr3_type)(int,int,int);
	typedef ctrl_ptr_type CamCtrlTy;
	typedef ctrl_ptr_type CamCtrl3Ty;
	typedef float CrTy;
	typedef  glut_generic_base GlutTy;
public:
	typedef std::basic_string<char> string_type;
protected:
	typedef void (Myt::*console_func_type)(SigCh key);
	typedef void (Myt::*punt_func_type)(string_parse_type & ss);
	typedef console_func_type ConFuncTy;
	typedef punt_func_type PuntFuncTy;

	typedef basic_command_shell ShellTy;
	 
text_commands_extension():m_shell() {}
public:
	virtual void cmd(text_commands * src, const console_type & s) 
	{ 	std::cout<<"Base class handler called for "<<s<<"\n";  } 
	virtual ~text_commands_extension() {}	

protected:
public: // faking atom_cmd_handler is denyed fcking access?????
typedef ShellTy::symbols_type return_values_type;
return_values_type & symbols() { return m_shell.symbols(); } 

private:
protected:
	ShellTy  m_shell; 
	
};

#endif
