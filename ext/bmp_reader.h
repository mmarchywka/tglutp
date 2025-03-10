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

 



#ifndef BMP_READER_H__
#define BMP_READER_H__
#include <unistd.h>		// for getopt()
#include <string.h>		// for memset()
#include <stdlib.h>		// atoi()
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <assert.h>

#include <string> 

extern "C" {
#undef HAVE_STDLIB_H    // prevents warning...
#include <jpeglib.h>
}

#include <iostream>

#include "bmp_header.h"


/*
 * Prototypes...
 */
typedef unsigned char GLubyte;
//extern GLubyte *LoadDIBitmap(const char *filename, BITMAPINFO **info);
extern GLubyte *    LoadDIBitmap(const char *filename, BITMAPINFO ** info ) ;
                       //     GLubyte *bits);

class bmp_reader
{

public:
	typedef unsigned char Da;
	bmp_reader(const char * fn)
	{
		m_name=fn;
		m_data=LoadDIBitmap(fn,&m_info);
	}
	~bmp_reader() {delete [] m_data; delete m_info; }
	Da * data() { return m_data; }
	int w() {return m_info->bmiHeader.biWidth;}
	int h() {return m_info->bmiHeader.biHeight;}
private:
	BITMAPINFO * m_info;
	Da * m_data;
	std::string m_name;	

};

#endif
