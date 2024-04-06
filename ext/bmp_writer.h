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

 



#ifndef BMP_WRITER_H__
#define BMP_WRITER_H__
#include <unistd.h>		// for getopt()
#include <string.h>		// for memset()
#include <stdlib.h>		// atoi()
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <assert.h>

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
extern int     write_bmp(const char *filename, const int w , const int h,//BITMAPINFO *info,
                            GLubyte *bits);



#endif
