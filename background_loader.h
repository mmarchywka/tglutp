#ifndef MJM_BACKGROUND_LOADER_H__
#define MJM_BACKGROUND_LOADER_H__
// marchywka@hotmail.com - designed to load an input stream into a real time display
// app. very slow right now under cygwin....

#include <pthread.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <string>
#include <stdlib.h>

#include "debug_and_support.h"

// note: this is included from the "affx" program files- the string manipulator and annotater.

#include "word_array.h"

#include <unistd.h> //sleep

// This is a junk allocator for use with fifo. 
// The mutexes are assumed to be invoked elsewhere so there is NO thread
// safety here. 
// allocation occurs circularly so this only works well for fifo alloc/dealloc.
#include <map>
class fred_and_lamont
{
	typedef fred_and_lamont Myt;
	typedef char TgtTy;
	typedef int IdxTy;
	// This is a map of lengths and offsets
	typedef std::map<IdxTy,IdxTy> MaTy;
public:
	fred_and_lamont(const IdxTy sz=(1<<15))
		: m_sz(sz),m_p_alloc(0),m_p_dealloc(0),m_top(0),m_avail(0), m_block(new TgtTy[m_sz])
	{}
	~fred_and_lamont() { delete [] m_block; }
	TgtTy * gnu(const IdxTy sz)
	{  
		if (sz+m_p_alloc>=m_sz) {  if (m_top!=0) return 0; m_top=m_p_alloc; m_p_alloc=0; }  
		IdxTy ed=m_p_alloc+sz;
		bool o1=(ed>m_p_dealloc);
		bool o3=(ed==m_p_dealloc);
		bool o2=(m_top==0);
		if ((!o1&&!o2||o1&&o2)&&(ed<m_sz)&&!o3) { IdxTy x=m_p_alloc; m_p_alloc+=sz; return x+m_block;  }
		return 0; 
	}
	bool dele( const TgtTy * p,const IdxTy sz)
	{
		IdxTy idx=p-m_block;
		if ( (idx!=m_p_dealloc)&&(idx!=0))
			std::cerr<<__FILE__<<__LINE__<<" out of order dealloc for "
					<<idx<<" versus "<<m_p_dealloc<<'\n';
		m_p_dealloc=idx+sz;	
		if (m_top!=0) if (m_p_dealloc==m_top){m_top=0; m_p_dealloc=0; }
		m_avail--;
		return true;
	}
	TgtTy * pread() const { return m_block+m_p_dealloc; } 
	IdxTy avail() const { return m_avail; } 
	bool tryst(TgtTy ** sp, const IdxTy len)
	{
		IdxTy p_alloc=m_p_alloc;
		IdxTy top=m_top;
		if (0==gnu(1)){ m_top=top;m_p_alloc=p_alloc; return false; }
		for ( IdxTy i=0; i<len; ++i) 
			if (0==gnu(1+strlen(sp[i]))){  m_top=top;m_p_alloc=p_alloc; return false; }
		m_p_alloc=p_alloc; m_top=top;
		TgtTy *pz =gnu(1);
		if ( len>126) return false;
		(*pz)=TgtTy(len);
		for (IdxTy i=0; i<len; ++i)
		{
			pz=gnu(1+strlen(sp[i]));
			// this shouldn't be null, just let it throw if so. 
			strcpy(pz,sp[i]);
		}
		m_avail+=len+1;
		return true;
	}
private:

	volatile IdxTy m_sz,m_p_alloc, m_p_dealloc,m_top,m_avail;
	TgtTy * m_block;
	MaTy m_free,m_alloc;
	
};

class background_junk_bin
{
	typedef background_junk_bin Myt;
	typedef int IdxTy;
	typedef float DataTy;
	typedef pthread_mutex_t MuTy;
	public:
//	enum {};
	background_junk_bin() :m_block(0),m_read(0),m_write(0)
	{
		if (sizeof(float)!=sizeof(int)) std::cerr<<__LINE__<<" "<<__FILE__<<" need float and int same\n";	
		m_sz=1<<10;
		m_block= new DataTy[m_sz];
	}
	~background_junk_bin() { delete [] m_block; }
	void lock() { mutex_lock(mutex); }
	void unlock() { mutex_unlock(mutex); }
	bool write(DataTy * src, const IdxTy sz)
	{
		lock();
		bool x=write_one(src,sz);
		unlock();
		return x;
	}
	// need to first put an int size, followed by data 
	bool write_one(DataTy * src, const IdxTy sz)
	{
		if ( (m_sz-avail())<=sz+1) return false;	
		// This should save the number as a real int...
		((int*)m_block)[m_write]=sz;
		for ( IdxTy i=0; i<sz; ++i) 
			m_block[(i+m_write+1)%m_sz]=src[i];
		m_write=(m_write+sz+1)%m_sz;
		return true;
	}
	IdxTy read(DataTy * dest,const IdxTy maxsz)
	{
		IdxTy sz=0;	
		lock();
		m_rd=m_read;
		int x=read_one(dest,sz);
		unlock(); 
		return x; 
	}
	IdxTy read_one(DataTy * src, const IdxTy sz)
	{
		if ( avail()<=sz) return 0;
		IdxTy 	tsz=((int*)m_block)[m_read]; 
		IdxTy szr=tsz;
		if ( szr>sz) szr=sz;
		for ( IdxTy i=0; i<szr; ++i) 
			src[i]=m_block[(i+m_read+1)%m_sz];
		m_read=(m_read+tsz+1)%m_sz;
		return szr;
	}
	IdxTy avail() const { return (m_sz+m_write-m_read)%m_sz; }
	 
private:
	static void mutex_lock(MuTy & mu) {pthread_mutex_lock(&mu); }
	static void mutex_unlock(MuTy & mu) {pthread_mutex_unlock(&mu); }

	volatile DataTy *  m_block;
	volatile IdxTy m_read,m_write,m_sz;
	IdxTy m_rd, m_wr;
	MuTy mutex;
};

class background_loader
{
	typedef background_loader Myt;
	typedef int IdxTy;
	typedef word_array<1024> LineTy;
 	enum {MAXLEN=1<<14,ALL=(~0)};
	typedef std::ostream OsTy;
	typedef std::istream IsTy;
	
	typedef pthread_mutex_t MuTy;
	typedef pthread_cond_t CondTy;
	
	typedef bool ( ReadFunc)(Myt* );
	
public:
background_loader():words(MAXLEN),empty_sema(true),m_leader(0),m_panic(0){}
background_loader(const char * nm):words(MAXLEN),empty_sema(true),m_leader(new std::ifstream(nm) )
,m_panic(0){}
void set_panic(int * p) {m_panic=p; }
int set_leader(const char *nm) {

std::cout<<MM_MARK<<"setting input leader \n";
 m_leader=new std::ifstream(nm); if (*m_leader) return 0; else return 1;   }
// initialize the loader to call ReadFunc everytime it gets a new line from the input
// stream, right now cin. 
int init(ReadFunc* pf) 
{
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
	return 0;
}
// The function pointed to by ReadFunc uses this method to get the words out of the
// line. Normally it is expected that the handler knows the format but we
// need a size method too. 
char * data(const IdxTy i) { return words.pi(i); }
// This is the number of words available on the input line.
IdxTy size() const { return m_sz;}

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



private:

static void mutex_lock(MuTy & mu) {pthread_mutex_lock(&mu); }
static void mutex_unlock(MuTy & mu) {pthread_mutex_unlock(&mu); }
static MuTy & class_mutex()  { static MuTy mu; return mu;  }
static void class_lock() { mutex_lock(class_mutex()); }
static void class_unlock() { mutex_unlock(class_mutex()); }

static Myt * &plaunch() { static Myt * xplaunch=0; return xplaunch; } 

static void* data_source(void * param) {  (*plaunch()).data_source();   return 0; } 
OsTy & eos() { return std::cerr; } 
OsTy & dos() { return std::cout; } 
IsTy & ins() 
{ 
	
	if (m_leader!=0) if (!*m_leader) {delete m_leader; m_leader=0;  }   
//dos()<<MM_MARK<<" setting input to std::cin\n";
	if (m_leader==0) return std::cin; 
 	
 	return *m_leader;
} 
bool do_panic() const 
{ if (m_panic==0) return false; if (*m_panic!=0) return true; return false;}
void spin_write()
{pthread_cond_wait(&empty,&mutex);  usleep(1);}
void* data_source()
{
 	plaunch()=0;
 	int line=0; 
 	
 	while (true)
 	{
//std::cout<<MM_MARK<<" reading\n";
 		if (do_panic()) return 0; 
 		bool fini_eh=(m_leader==0);
 		words.read_normaler(ins(),&eos(),line);
 		m_sz=words.mindex();
 		// need to fix switch over
 //		std::cerr<<"xxxxxxxxxxxxxxx"<<" "<<data(0)<<'\n';
 		if (!ins()){ 
 			if (fini_eh) return 0; 
 			words.read_normaler(ins(),&eos(),line);
 			m_sz=words.mindex();
 			// something really wrgon... 
 			if (!ins()) return 0;
 			//return 0; 
 			
 		}
 		++line;
 	//	if (m_sz==0) { continue; }
 		bool fini=false;
 		while (!fini)
 		{
 			mutex_lock(mutex);
 //		do something with the line.
 			fini=(*pfunc)(this);
// 		dos()<<" read ";for ( int ii=0; ii<7 ; ii++) dos()<<linep[ii]<<" ";dos()<<'\n';
			mutex_unlock(mutex);
			if (!fini) usleep(2); 
		}	
		empty_sema=false;
		// spurious wake-ups are allowed, must check...
		// sleep==1 is still slow but low cpu mode... 
		//while(!empty_sema){ //pthread_cond_wait(&empty,&mutex); 
		//usleep(1); }
		//pthread_cond_init(&empty,0); 
		
		//while(!empty_sema){usleep(1); }
		
		
	}
	return 0; 
}
MuTy mutex;
CondTy empty;
ReadFunc* pfunc;
LineTy words;
volatile bool empty_sema;
volatile IdxTy m_sz;
IsTy * m_leader; 
volatile int * m_panic;


};
#endif

