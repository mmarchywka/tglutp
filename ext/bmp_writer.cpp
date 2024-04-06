
// 
// This is a copy of the affx JPEG_writer(SIC) file that writes bmp's...
// adapted from source cited below.

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

 



#include "bmp_writer.h"
/*
 * Windows BMP file definitions for OpenGL.
 *
 * Written by Michael Sweet.
 */
// http://local.wasp.uwa.edu.au/~pbourke/dataformats/bmp/BITMAP.H
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
//////////////////////////////////////////////////////////////////
/*
 * 'write_word()' - Write a 16-bit unsigned integer.
 */

static int                     /* O - 0 on success, -1 on error */
write_word(FILE           *fp, /* I - File to write to */
           unsigned short w)   /* I - Integer to write */
    {
    putc(w, fp);
    return (putc(w >> 8, fp));
    }


/*
 * 'write_dword()' - Write a 32-bit unsigned integer.
 */

static int                    /* O - 0 on success, -1 on error */
write_dword(FILE         *fp, /* I - File to write to */
            unsigned int dw)  /* I - Integer to write */
    {
    putc(dw, fp);
    putc(dw >> 8, fp);
    putc(dw >> 16, fp);
    return (putc(dw >> 24, fp));
    }


/*
 * 'write_long()' - Write a 32-bit signed integer.
 */

static int           /* O - 0 on success, -1 on error */
write_long(FILE *fp, /* I - File to write to */
           int  l)   /* I - Integer to write */
    {
    putc(l, fp);
    putc(l >> 8, fp);
    putc(l >> 16, fp);
    return (putc(l >> 24, fp));
    }





int                                /* O - 0 = success, -1 = failure */
write_bmp(const char *filename, /* I - File to load */
      //       BITMAPINFO *info,     /* I - Bitmap information */
	   const  int w, const int h,
	     GLubyte    *bits)     /* I - Bitmap data */
    {
    FILE *fp;                      /* Open file pointer */
    unsigned int  size,                     /* Size of file */
         infosize,                 /* Size of bitmap info */
         bitsize;                  /* Size of bitmap pixels */


    /* Try opening the file; use "wb" mode to write this *binary* file. */
    if ((fp = fopen(filename, "wb")) == NULL)
        return (-1);

BITMAPINFOHEADER binfo;

	binfo.biSize=sizeof(BITMAPINFOHEADER);           /* Size of info header */
     binfo.biWidth=w;          /* Width of image */
binfo.biHeight=h;         /* Height of image */
    binfo.biPlanes=1;         /* Number of color planes */
    binfo.biBitCount=24;       /* Number of bits per pixel */
    binfo.biCompression=BI_RGB;    /* Type of compression to use */
    binfo. biSizeImage=w*h*3;      /* Size of image data */
    binfo.biXPelsPerMeter=0;  /* X pixels per meter */
    binfo.biYPelsPerMeter=0;  /* Y pixels per meter */
    binfo.biClrUsed=0;        /* Number of colors used */
    binfo.biClrImportant=0;   /* Number of important colors */
 
 BITMAPINFO finfo;
 finfo.bmiHeader=binfo; 
BITMAPINFO * info=&finfo;
 if (( w&3)!=0)
    {  std::cerr<<__FILE__<<__LINE__<<" Cant pad output, width="<<w<<" is not 0 mod 4.\n";  return -1;  }
    /* Figure out the bitmap size */
    if (info->bmiHeader.biSizeImage == 0)
	bitsize = (info->bmiHeader.biWidth *
        	   info->bmiHeader.biBitCount + 7) / 8 *
		  abs(info->bmiHeader.biHeight);
    else
	bitsize = info->bmiHeader.biSizeImage;

    /* Figure out the header size */
    infosize = sizeof(BITMAPINFOHEADER);
    switch (info->bmiHeader.biCompression)
	{
	case BI_BITFIELDS :
            infosize += 12; /* Add 3 RGB doubleword masks */
            if (info->bmiHeader.biClrUsed == 0)
	      break;
	case BI_RGB :
            if (info->bmiHeader.biBitCount > 8 &&
        	info->bmiHeader.biClrUsed == 0)
	      break;
	case BI_RLE8 :
	case BI_RLE4 :
            if (info->bmiHeader.biClrUsed == 0)
              infosize += (1 << info->bmiHeader.biBitCount) * 4;
	    else
              infosize += info->bmiHeader.biClrUsed * 4;
	    break;
	}
// This works in mspaint but is shiftred in kodakimg.


    size = sizeof(BITMAPFILEHEADER) + infosize + bitsize;
	size=14+infosize+bitsize; // under g++ 3.3.3 cygwin 
std::cerr<<"sizes are "<<size <<" "<<sizeof(BITMAPFILEHEADER)<<" "<<infosize<<" "<<bitsize<<" ";
//                                   This iszeof is return 16, not 14? wtf
    /* Write the file header, bitmap information, and bitmap pixel data... */
    write_word(fp, BF_TYPE);        /* bfType */
    // -2 changed nothing
    write_dword(fp, size-2);          /* bfSize */
    write_word(fp, 0);              /* bfReserved1 */
    write_word(fp, 0);              /* bfReserved2 */
	// 	write_dword(fp,40); 

    write_dword(fp, 18*0+14 + infosize); /* bfOffBits */
// This is 40, not the above thing.
    write_dword(fp, info->bmiHeader.biSize);
    write_long(fp, info->bmiHeader.biWidth);
    write_long(fp, info->bmiHeader.biHeight);
    write_word(fp, info->bmiHeader.biPlanes);
    write_word(fp, info->bmiHeader.biBitCount);
    write_dword(fp, info->bmiHeader.biCompression);
    write_dword(fp, info->bmiHeader.biSizeImage);
    write_long(fp, info->bmiHeader.biXPelsPerMeter);
    write_long(fp, info->bmiHeader.biYPelsPerMeter);
    write_dword(fp, info->bmiHeader.biClrUsed);
    write_dword(fp, info->bmiHeader.biClrImportant);
    if (infosize > 40)
	if (fwrite(info->bmiColors, infosize - 40, 1, fp) < 1)
            {
            /* Couldn't write the bitmap header - return... */
            fclose(fp);
            return (-1);
            }

    if (fwrite(bits, 1, bitsize, fp) < bitsize)
        {
        /* Couldn't write the bitmap - return... */
        fclose(fp);
        return (-1);
        }

    /* OK, everything went fine - return... */
    fclose(fp);
    return (0);
    }


#ifdef HAVESTUPIDWRITERTHING


#endif

//////////////////////////////////////////////////


