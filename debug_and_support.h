#ifndef GENERIC_DEBUG_AND_SUPPORT_HH_H
#define GENERIC_DEBUG_AND_SUPPORT_HH_H
#include <list>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>


#define s(x) string_type ( # x )
#define MMDDLOC  __FILE__,  __LINE__
#define MYASSERT (ME) mmdd::dout(__FILE__,__LINE__,ME);



#define MM_MARK __FILE__<<__LINE__
#define MM_BORN __DATE__<<" "<<__TIME__
#define MM_TRACE std::cerr<<__FILE__<<__LINE__<<'\n';
#define MM_PRINT std::cerr<<__FILE__<<__LINE__<<" "<<


//namespace 
class mmdd
{public:
	typedef std::basic_string<char> string_type;
typedef std::basic_ostream<char> wtf;
static wtf & dout(const string_type & loc, const int & line_is_int)
{  std::cout<<loc<<" at "<<line_is_int<<":";    std::cout.flush(); return std::cout; }
enum {MSG_INFO=0, MSG_WARN=1, MSG_ERR=2};
static wtf & dout(const string_type & loc, const int & line_is_int, const int msg)
{  std::cout<<loc<<" at "<<line_is_int<<":";    std::cout.flush(); return std::cout; }
static wtf & dout(const string_type & loc, const int & line_is_int , const bool & expp )
{ if (!expp)  {std::cout<<loc<<" at "<<line_is_int<<":";  exit( -1); }
std::cout.flush(); return std::cout; }
	
	
	
	};

#endif
