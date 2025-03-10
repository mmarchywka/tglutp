#include "command_map.h"

#include <linux/icmp.h>
#include <linux/skbuff.h>
#include <net/sock.h>
#include <net/protocol.h>


#include <pthread.h>
#include <iostream>
#include <fstream>

// note: this is included from the "affx" program files- the string manipulator and annotater.

#include "word_array.h"

#include <unistd.h> //sleep
#include "global.h"

#include <fstream>
#include <string> 

#include <sstream> 
//#include <strstream> 
class basic_typedefs
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
	
	basic_typedefs():m_fail_code(FAILCODE), m_debug(0), m_out(0), m_err(0),m_own(true) {}
	~basic_typedefs() { if (!m_own) return; delete m_out; delete m_err;}
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

///////////////////////////////////////////////////////

class mt_typedefs : public basic_typedefs
{
	typedef mt_typedefs Myt;
	public:
	mt_typedefs():
	empty_sema(true),//m_leader(0),
	m_panic(0) 
	{}
	void set_panic(int * p) {m_panic=p; }
	
	
// The handler sets up some thing with data and the consumer polls for availability
// when it is enabled. enter_out checks for stuff and gets lock if needed.
// exit_out is called when consumer has freed the buffer.
bool enter_out()
{
	if (empty_sema ) return false;
	return enter_out_force();
}
bool enter_out_force()
{
	mutex_lock(mutex);
	empty_sema=true; 
	return true;
}
void exit_out()
{
//	pthread_cond_signal(&empty);
	mutex_unlock(mutex);
}
	
protected:

	typedef pthread_mutex_t MuTy;
	typedef pthread_cond_t CondTy;
	typedef bool ( ReadFunc)(Myt* );
	// This could get VERY slow if many derivers make many instances and use
	// class lock, even though lock within a derived class may be intended.
	static void mutex_lock(MuTy & mu) {pthread_mutex_lock(&mu); }
	static void mutex_unlock(MuTy & mu) {pthread_mutex_unlock(&mu); }
	static MuTy & class_mutex()  { static MuTy mu; return mu;  }
	static void class_lock() { mutex_lock(class_mutex()); }
	static void class_unlock() { mutex_unlock(class_mutex()); }
	static Myt * &plaunch() { static Myt * xplaunch=0; return xplaunch; } 

	static void* data_source(void * param) {  (*plaunch()).data_source();   return 0; } 
	bool do_panic() const 
	{ if (m_panic==0) return false; if (*m_panic!=0) return true; return false;}
	void spin_write()
	{pthread_cond_wait(&empty,&mutex);  usleep(1);}
	void* data_source()
	{return 0;  }
	MuTy mutex;
	CondTy empty;
	ReadFunc* pfunc;
//	LineTy words;
	volatile bool empty_sema;
	volatile int * m_panic;


};
/////////////////////////////////////////////////////
class threaded_logger : public mt_typedefs
{
	typedef threaded_logger Myt;
	
	//typedef OsTy & ( Myt:: * OsFu )();
	typedef OsTy OsFu;
	typedef std::ostringstream SoTy; 
public:
//OsTy 
threaded_logger(OsFu * const osfu ): m_fu(osfu)
{}

SoTy * new_thread_local_buffer() { return new SoTy(); }
SoTy * out(SoTy * x) 
{
	// This is rather extreme but multiple instances could point to same underlying
	// stream and speed is normally not a big issue. Deadlock is unlikely AFAIK. 
	class_lock();
	(*m_fu)<<x->str()<<'\n';  
	class_unlock();
	delete x;
	return new_thread_local_buffer(); 
}
//int set_leader(const char *nm) { m_leader=new std::ifstream(nm); if (*m_leader) return 0; else return 1;   }
// initialize the loader to call ReadFunc everytime it gets a new line from the input
// stream, right now cin. 
int init(ReadFunc* pf) 
{
	/*
	pfunc=pf;
	pthread_t  pt=0;
	pthread_cond_init(&empty,0); 
	class_lock();
	plaunch()=this;
	int threads=pthread_create(&pt,0,&data_source,0);
	threads=0;
	// need condition or sempaphore here...
	while (plaunch()!=0) sleep(0); 
	class_unlock();
	*/
	return 0;
}




private:

//OsTy * m_logger; 
OsFu * m_fu;



};









////////////////////////////////////////////////////////
class port_util;
typedef port_util MyClass;

class port_util : public basic_typedefs
{
	typedef port_util Myt;
	
	typedef threaded_logger LoTy;
	
	LoTy*  m_lo;
	
	api_type Listen(Chr * const nm)
	{
	
	
		return 0;
	}
public:
	
	port_util(): m_lo(new LoTy(&std::cout)) 
	{}
template <class Os> static void rhelp( Os& os)
{
	os<<Ldr(0)<<"Usage: port_util" <<Trl();
	
		
		
}
template <class Os> static void credit_and_ref( Os& os)
{// nominally for human consumption but not formatted, should parse ok 
os<<Ldr(0)<<MM_MARK<<" Built on "<<__DATE__<<" at "<<__TIME__<<Trl();
os<<Ldr(0)<<"Contact: Mike Marchywka, marchywka@hotmail.com" <<Trl();
}

int main ( int argc, char ** argv)
{
	int i=1;
	if ( r_cmd.size()==0)
	{
		// I cleverly delegated the debug function to all the members, so this
		// is awkward now... 
//		push_cmd("-debug",&Myt::m_debug,1,ALL); 
//		push_cmd("-v",&Myt::m_debug,1,1);
//		push_cmd("-q",&Myt::m_debug,2,(0));
		
//		push_cmd("-pfx",&Myt::m_pfx,0);
		
	//push_cmd("-flags",&Myt::m_flags,0);
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
	static stupid fn[]={	&Myt::Listen,
							
					};
	push_cmd("-listen",&fn[0],3,0);
	
	
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
	/*	else if ((strcmp(argv[i],"-add")==0)&&(argc>(i+2))) 
		{ m_plo.add(argv[i+1],argv[i+2]); i+=2;}
		else if ((strcmp(argv[i],"-op")==0)&&(argc>(i+2))) 
		{ m_plo.operate(argv[i+1],argv[i+2]); i+=2;}
		else if ((strcmp(argv[i],"-center")==0)&&(argc>(i+3))) 
		{ m_x=atof(argv[i+1]);m_y=atof(argv[i+2]);m_z=atof(argv[i+3]);  i+=3;}
		else if ((strcmp(argv[i],"-angles")==0)&&(argc>(i+3))) 
		{ m_a=atof(argv[i+1]);m_b=atof(argv[i+2]);m_c=atof(argv[i+3]);  i+=3;}
		*/
		else { eos()<<"Unable to parse "<<argv[i]<<'\n'; return -1; }
		++i;
	}
	return 0;	
}	
	
	
	
};





#ifdef STANDALONE

int main ( int argc, char ** argv) { return MyClass().main(argc,argv);	}

#endif



