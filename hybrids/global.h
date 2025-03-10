// These files are being released as sample code in the hopes
// they may be useful to someone but there is NO implication they are
// safe or effective for any purpose nor is there any waiver or license
// related to any intellectual property that may protect code or algorithms
// approximated therein.
// Source code contains links to various sources, including me, that should
// be maintained in derivative works where applicable and where no other
// restrictions apply to such works. 
// Source and compiled code may contain many objectionable features, including
// profanity and plain bad design/coding but usually the code is written with
// a specific performance or demonstration objection that may be of use to some readers. 
// This code was developed on my own time using generally freely available tools,
// cygwin for example, and other contributing resources are noted where applicable.
// Mike Marchywka; Marietta GA; marchywka@hotmail.com March 2008

 


// note, this guard has retained the name to avoid confusion
// in case search path gets mixed up and someone tried to include
// the global from the AFFX project. Presumably, this one will get included first.

#ifndef GLOBAL_MARCHYWKA_AFFX_H__
#define GLOBAL_MARCHYWKA_AFFX_H__

#define MM_MARK __FILE__<<__LINE__
#define MM_BORN __DATE__<<" "<<__TIME__
#define MM_TRACE std::cerr<<__FILE__<<__LINE__<<'\n';
#define MM_PRINT std::cerr<<__FILE__<<__LINE__<<" "<<
#include <iostream>
namespace {
// 2024 unused 
//std::ostream & mmeos() { return std::cerr; } 
};
namespace mmstuff
{
template <class Os > void credit_and_license(Os & os)
{
	os<<" Any rebroadcast without expressed written permission of ";
	os<<" Major League Baseball Commissioner is prohibited."; 
	os<<" Seriously, this will probably be open source or free, no particular ";
	os<<" license or representations. Only athletes get paid. "; 
}
template <class Os > void a_few_humble_words(Os & os)
{
	os<<" Most of this stuff was written by Mike Marchywka\n";
	os<<" Contact: marchywka@hotmail.com or marchywka@yahoo.com\n"; 
	os<<" Detailed credits and research links appear in -about text\n"; 
	os<<" in most apps as well as source code, grep for http\n";
	
}

};

#endif

