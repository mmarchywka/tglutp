#ifndef JPEG_WRITER_H__
#define JPEG_WRITER_H__
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

void write_jpg(const char * fname,char *src, int w,int h);





#endif
