#ifndef TEXT_COMMANDS_H
#define TEXT_COMMANDS_H


#include "mjm_globals.h"

#include "../view/text_commands_extension.h"
//#include <vector> // command history
#include <iostream>
#include <stack>


class text_commands : public text_commands_extension // just to pickup typedefs
{
	typedef text_commands_extension Super;
	typedef Super * ext_ptr_type;
	typedef text_commands Myt;
	
	

	
	typedef point3_template<int> CenterTy; 

class CmdMapValue{ public: CmdMapValue(const ConFuncTy con=NULL,
	const CamCtrlTy cam=NULL, const int cam3=-1, const PuntFuncTy punt=NULL)
	:m_con(con),m_cam(cam),m_cam3(cam3),m_punt(punt),m_help() {}
	
 	ConFuncTy m_con; CamCtrlTy m_cam; int m_cam3; PuntFuncTy m_punt; 
 	const CmdMapValue &  help(const string_type & h) { m_help=h;return *this; }
 	const string_type & help() const { return m_help;}
	 string_type m_help; 
};

	typedef std::map<string_type,CmdMapValue> CmdMapTy;
 	typedef pair<ext_ptr_type,string_type> annotated_ext_cmd_ptr;
	typedef std::map<string_type,annotated_ext_cmd_ptr> ECmdMapTy;
 	typedef std::basic_istream<char> SourceTy;
 	typedef std::stack<SourceTy*> SStackTy; 
 	class StackTy : public SStackTy
 	{public:
 		~StackTy() {Clear(); }
 	private:	
void Clear() {size_t i=size(); while (i) {delete top(); pop();--i; }}  
 	};
 	
public:
volatile static int*  global_panic;
// 2008-03-18 wtf with new compiler?
//friend class Super;
friend class text_commands_extension;
text_commands (GlutTy * ggb):Super(),m_in(),m_run_script(true),m_ggb(ggb),
console_mode(0),console_func_ptr(0),camera_func(&CameraTy::clip_up)

{
	r_one=this;
	LoadDefaultCommands();
}
public:
	static void keyboardCallback( unsigned char key, int x, int y );
	void KeyboardCallback( SigCh key, int x, int y ) ;
	static void specialCallback( int key, int x, int y );
 	void SpecialCallback( int key, int x, int y ) ;
	static void runScript();



protected:
public:
// This is not a reference since it is a dummy for compiler, ignored the help stuff
	static CmdMapValue addCmd
		(const string_type & cmd, const ext_ptr_type p,const string_type & help);

	CmdMapValue add_cmd
		(const string_type & cmd, const ext_ptr_type p,const string_type & help ) ;

	void more_scripting() {m_run_script=true; }
	void LoadDefaultCommands();
	count_type cin_problem() ;
	CrTy key_to_number(CmpCh key);
	void np_slice(SigCh key);
	void console_fp(SigCh key);
	void console_np(SigCh key);
	void console_zoom(SigCh  key);
	void unified_rotate(CmpCh key,CrTy & delta_x,CrTy & delta_y,CrTy & x,CrTy & y,CrTy & z);
	void console_light_rotate(SigCh key);
	void console_rotate(SigCh key);
	void quit(string_parse_type & ss);
	count_type run_script(); 
	count_type source_script();
	 void call_cam(float x, float y, CameraTy & cam, const CamCtrlTy & ptr);
//{//(myTest.*funcPtr)()  
//	(cam.*ptr)(x); 	
//}
	void help(string_parse_type & ss) ;
	void rotcen(string_parse_type & ss) ;
	void center(string_parse_type & ss) ;
	void capture(string_parse_type & ss) ;
	void history(string_parse_type & ss) ;
	void idletoggle(string_parse_type & ss) ;
	void rot(string_parse_type & ss) ;
	void update(string_parse_type & ss) ;
	void source(string_parse_type & ss) ;
	void dumpq(string_parse_type & ss) ;
	template <class Ty> 
		void setdun(const console_type & lbl, const Ty & x) const
		{glutPostRedisplay(); cout<<lbl<<x<<"\n";  }

	void DoString(const console_type & whole_cmd);
	void DoDoString(const console_type & whole_cmd);
	
	
protected:
private:
void AddSourceStream(SourceTy * is);
public:
	CmdMapTy cmd_map; 
	ECmdMapTy ext_cmd_map;
	cmd_line_input m_in;
	bool m_run_script;
	GlutTy *m_ggb;

	int console_mode; //=0;
	console_type console_string;
	console_func_type console_func_ptr; //= NULL;
	ctrl_ptr_type camera_func ;//= &CameraTy::clip_up;
	static text_commands * r_one;

	StackTy m_stack; 



};



#endif


