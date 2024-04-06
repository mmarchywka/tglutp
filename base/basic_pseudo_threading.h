#ifndef PSEUDO_THREADING_H__
#define PSEUDO_THREADING_H__
#include "../debug_and_support.h"
#include <map>
#include <string>
#include <iostream>
// Time shares a real thread among various tasks
// Tasks are setup, a real thread calls run and a task of tasks
// is sliced in. Tasks report finished status.

class basic_pseudo_threading
{
	typedef basic_pseudo_threading Myt;
	typedef int count_type;
	typedef char Ch;
	typedef basic_string<Ch> string_type;
	
	// called to see how runnable task is- like avail on a buffer
//	typedef count_type ( *ReadyPtr)();
	// execute- returns zero when done, or int with how much more coule be done
//	typedef count_type ( * RunPtr) ();
	
	class my_runnable_base{
	public:
	typedef unsigned int count_type;
//	typedef unsigned int alt_count_type;
	my_runnable_base(): m_ok(true) {}
	virtual ~my_runnable_base() {}
	virtual count_type run()=0;
	virtual count_type ready()=0;
	void runnable(bool ok) {m_ok=ok;}
	bool runnable() const { return m_ok;}
	private:
	bool m_ok;
	};
template <class Ty,class Tr> class my_runnable : public my_runnable_base{
	typedef my_runnable_base Super;
	typedef  Ty TgtTy;
//	typedef  Super::count_type (TgtTy:: * FncPtr)();
//	typedef  Tr (TgtTy:: * FncPtr)();
	typedef Tr FncPtr;
//	typedef  Super::alt_count_type (TgtTy:: * AFncPtr)();
	typedef TgtTy * TgtPtr;
	const TgtPtr m_tgt;
	const FncPtr m_ptr,m_rdy;
//	const AFncPtr m_aptr,m_ardy;
public:
my_runnable() : m_tgt(0),m_ptr(0),m_rdy(0) //,m_aptr(0),m_ardy(0) 
{}
my_runnable(const TgtPtr t, const FncPtr p,const FncPtr r=0) : m_tgt(t),m_ptr(p),m_rdy(r)
//,m_aptr(0),m_ardy(0)
{}
count_type run() {  return (m_tgt->*m_ptr)();  }
count_type ready() { return (m_tgt->*m_rdy)(); }

};
template <class Tr> class my_global_runnable : public my_runnable_base{
	typedef my_runnable_base Super;
	typedef Tr FncPtr;
	const FncPtr m_ptr,m_rdy;
public:
my_global_runnable() :m_ptr(0),m_rdy(0) {}
my_global_runnable(const FncPtr p,const FncPtr r=0) : m_ptr(p),m_rdy(r)
{}
count_type run() {  return (*m_ptr)();  }
count_type ready() { return (*m_rdy)(); }

};

	typedef my_runnable_base ProcTy;
	typedef map<string_type,my_runnable_base*> TaskMapTy;
	typedef TaskMapTy::iterator TaskItorTy;
public:

basic_pseudo_threading ():m_tasks(),m_ii(),m_runnable(0) {}
~basic_pseudo_threading()
{
	rs(0)<<" being destroyed with "<<m_tasks.size()<<" tasks left\n";
while (m_tasks.size())
{

	TaskItorTy ii=m_tasks.begin();
remove((*ii).first); 
} 
//	while (ii!=m_tasks.end()) 
// 2024 wtf 
	//{ 	delete (*ii).second; ii=m_tasks.begin(); }
//	{ 	delete (*ii).second;  }
	
}
//template <class Ty,typename Tf > void add_task( const char[7]& nm,
//	const Ty *& thing, const Tf & f) {add_task(string_type(nm),thing,f);}
// This really didn't like the "&" in the signature???
// no problem here but what about general case???
// also complains about const of ptr and f-ptr- I really wouldn't count on this
// working to inline const f* in an innner loop somewhere...
template <class Ts, class Ty,typename Tf > void add_task( const Ts & nm,
  Ty * thing, const Tf f )
{
 if (thing==0) return; 
 if (m_tasks.find(string_type(nm))!=m_tasks.end()) remove(string_type(nm));
 	m_tasks[string_type(nm)]= new my_runnable<Ty,Tf>(thing,f); 
// 	else {rs(2) <<"Attempt to add task "<<nm<<" again\n"; }
	m_ii=m_tasks.begin();
}
template <class Ts, typename Tf > void add_task( const Ts & nm, const Tf f )
{
if (f==0) return; // just ignore a null
 if (m_tasks.find(string_type(nm))!=m_tasks.end()) remove(string_type(nm));
 m_tasks[string_type(nm)]= new my_global_runnable<Tf>(f); 
 //else {rs(2) <<"Attempt to add global task "<<nm<<" again\n"; }
	m_ii=m_tasks.begin();
}
// called when the real thread decides there is time to devote to these tasks


void remove(const string_type & nm)
{
	TaskItorTy ii=m_tasks.find(nm);
	if ( ii==m_tasks.end()) return ;
	delete (*ii).second;
	m_tasks.erase(ii);
}
template <class Ty> void dump(Ty& os) const
{TaskMapTy::const_iterator ii=m_tasks.begin(),ie=m_tasks.end();
	while (ii!=ie) {os<<";"<<(*ii).first; ++ii; }
	
	}

void enable(bool ok, const char * nm)
{ enable(ok, string_type(nm)); }
bool enabled(const char * nm) const
{ return enabled(string_type(nm)); }
void enable(bool ok, const string_type & nm)
{if ( m_tasks.find(nm)!=m_tasks.end()) m_tasks[nm]->runnable(ok); }
bool enabled( const string_type & nm) const 
{if ( m_tasks.find(nm)!=m_tasks.end()) return //m_tasks[nm]
(*(m_tasks.find(nm))).second->runnable(); else return false; }
count_type runnables()
{	count_type oks=0;
	TaskItorTy ii=m_tasks.begin(),ie=m_tasks.end();
	while (ii!=ie) 
		if ((*(ii++)).second->runnable()) ++oks;
	return oks;
}
count_type tasks() const {  return m_tasks.size(); }
//count_type Final()
//{m_ii=m_tasks.begin(); return runnables();	 }
count_type run()
{	if (m_tasks.size()==0) return 0;
	count_type ru=0;
	m_ii=m_tasks.begin();
	while (m_ii!=m_tasks.end())
	{	ProcTy * proc=(*(m_ii)).second;
		if (proc->runnable()) 
		{	++ru;
			count_type n=proc->run(); 	
			// some will run twice- priority is alphabetical :)
			if (n==0) // ru won'tbe accurate but ok... 
			{ delete proc; m_tasks.erase(m_ii);--ru; m_ii=m_tasks.begin();}
			else m_ii++;
		} else ++m_ii;
	
	}
return ru;
}

private:
typedef mmdd::wtf ostream_type;
static ostream_type & rs(const count_type msg=0 )
{  return mmdd::dout(MMDDLOC,msg); }
TaskMapTy m_tasks;
TaskItorTy m_ii;
count_type m_runnable;

};
#endif

