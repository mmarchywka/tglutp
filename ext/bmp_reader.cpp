/*
 * Windows BMP file functions for OpenGL.
 *
 * Written by Michael Sweet:  OpenGL SuperBible
 */

#include "bmp_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#define MM_LOC __FILE__<<" "<<__LINE__

#ifdef WIN32
/*
 * 'LoadDIBitmap()' - Load a DIB/BMP file from disk.
 *
 * Returns a pointer to the bitmap if successful, NULL otherwise...
 */

GLubyte *                          /* O - Bitmap data */
LoadDIBitmap(const char *filename, /* I - File to load */
             BITMAPINFO **info)    /* O - Bitmap information */
    {
    FILE             *fp;          /* Open file pointer */
    GLubyte          *bits;        /* Bitmap pixel bits */
    int              bitsize;      /* Size of bitmap */
    int              infosize;     /* Size of header information */
    BITMAPFILEHEADER header;       /* File header */

	int bmfh_sz=14;

 	std::cerr<<MM_LOC<<" try to read "<<filename<<'\n';
    /* Try opening the file; use "rb" mode to read this *binary* file. */
    if ((fp = fopen(filename, "rb")) == NULL)
    {
        std::cerr<<MM_LOC<<" no open for "<<filename<<'\n';
        return (NULL);
	}
    /* Read the file header and any following bitmap information... */
   MM_ERR(" read has a possible heade comment issue ") 
/*
    // This gets packed.... 
    //unsigned short bfType;           // Magic number for file 
    fread(&header.bfType, 2, 1, fp);
    //unsigned int   bfSize;           // Size of file 
    fread(&header.bfSize, 4, 1, fp);
    unsigned short bfReserved1;      // Reserved 
    unsigned short bfReserved2;      // ... 
    fread(&header.bfReserved1, 4, 1, fp)
    unsigned int   bfOffBits;        // Offset to bitmap data 
    
    */
    
    if (fread(&header.bfOffBits, 4, 1, fp) < 1)
        {
        std::cerr<<MM_LOC<<" no good header  "<<filename<<'\n';
        /* Couldn't read the file header - return NULL... */
		fclose(fp);
        return (NULL);
        }

    if (header.bfType != 'MB')	/* Check for BM reversed... */
        {
        std::cerr<<MM_LOC<<" no good type "<<filename<<  " " << char(header.bfType&0x0ff)  << " "  <<'\n';
        /* Not a bitmap file - return NULL... */
        fclose(fp);
        return (NULL);
        }

    infosize = header.bfOffBits - bmfh_sz;
    if ((*info = (BITMAPINFO *)malloc(infosize)) == NULL)
        {
        /* Couldn't allocate memory for bitmap info - return NULL... */
        std::cerr<<MM_LOC<<" no allocation for   "<<infosize<<'\n';
        fclose(fp);
        return (NULL);
        }

    if (fread(*info, 1, infosize, fp) < infosize)
        {
        	 std::cerr<<MM_LOC<<" no fread for   "<<infosize<<'\n';
        /* Couldn't read the bitmap header - return NULL... */
        free(*info);
        fclose(fp);
        return (NULL);
        }

    /* Now that we have all the header info read in, allocate memory for *
     * the bitmap and read *it* in...                                    */
   std::cerr<<MM_LOC<< " have hader info "<<
   ( (*info)->bmiHeader.biSizeImage)<<" "<< (*info)->bmiHeader.biWidth <<" "<<
   (*info)->bmiHeader.biBitCount<<" "<<((*info)->bmiHeader.biHeight)<<"\n";
  	           
    if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
        bitsize = ((*info)->bmiHeader.biWidth *
                   (*info)->bmiHeader.biBitCount + 7) / 8 *
  	           abs((*info)->bmiHeader.biHeight);

    if ((bits = malloc(bitsize)) == NULL)
        {
        	 std::cerr<<MM_LOC<<" no allocation for   "<<bitsize<<'\n';
        /* Couldn't allocate memory - return NULL! */
        free(*info);
        fclose(fp);
        return (NULL);
        }

    if (fread(bits, 1, bitsize, fp) < bitsize)
        {
        /* Couldn't read bitmap - free memory and return NULL! */
        std::cerr<<MM_LOC<<" no fread for   "<<bitsize<<'\n';
        free(*info);
        free(bits);
        fclose(fp);
        return (NULL);
        }
 	std::cerr<<MM_LOC<<" read seems ok   "<<'\n';
    /* OK, everything went fine - return the allocated bitmap... */
    fclose(fp);
    return (bits);
    }


#else /* !WIN32 */
/*
 * Functions for reading and writing 16- and 32-bit little-endian integers.
 */

static unsigned short read_word(FILE *fp);
static unsigned int   read_dword(FILE *fp);
static int            read_long(FILE *fp);
#if 0
static int            write_word(FILE *fp, unsigned short w);
static int            write_dword(FILE *fp, unsigned int dw);
static int            write_long(FILE *fp, int l);
#endif

/*
 * 'LoadDIBitmap()' - Load a DIB/BMP file from disk.
 *
 * Returns a pointer to the bitmap if successful, NULL otherwise...
 */

GLubyte *                          /* O - Bitmap data */
LoadDIBitmap(const char *filename, /* I - File to load */
             BITMAPINFO **info)    /* O - Bitmap information */
    {
    FILE             *fp;          /* Open file pointer */
    GLubyte          *bits;        /* Bitmap pixel bits */
    GLubyte          *ptr;         /* Pointer into bitmap */
    GLubyte          temp;         /* Temporary variable to swap red and blue */
    int              x, y;         /* X and Y position in image */
    int              length;       /* Line length */
    int              bitsize;      /* Size of bitmap */
    int              infosize;     /* Size of header information */
    BITMAPFILEHEADER header;       /* File header */

  std::cerr<<MM_LOC<<'\n';
    /* Try opening the file; use "rb" mode to read this *binary* file. */
    if ((fp = fopen(filename, "rb")) == NULL)
    {
        std::cerr<<MM_LOC<<" not able to open "<<filename<<'\n';
        return (NULL);
	}
    /* Read the file header and any following bitmap information... */
    header.bfType      = read_word(fp);
    header.bfSize      = read_dword(fp);
    header.bfReserved1 = read_word(fp);
    header.bfReserved2 = read_word(fp);
    header.bfOffBits   = read_dword(fp);

    if (header.bfType != BF_TYPE) /* Check for BM reversed... */
        {std::cerr<<MM_LOC<<'\n';
        /* Not a bitmap file - return NULL... */
        fclose(fp);
        return (NULL);
        }
std::cerr<<MM_LOC<<'\n';
    infosize = header.bfOffBits - 14; //18;
    if ((*info = (BITMAPINFO *)malloc(sizeof(BITMAPINFO))) == NULL)
        {std::cerr<<MM_LOC<<'\n';
        /* Couldn't allocate memory for bitmap info - return NULL... */
        fclose(fp);
        return (NULL);
        }
std::cerr<<MM_LOC<<'\n';
    (*info)->bmiHeader.biSize          = read_dword(fp);
    (*info)->bmiHeader.biWidth         = read_long(fp);
    (*info)->bmiHeader.biHeight        = read_long(fp);
    (*info)->bmiHeader.biPlanes        = read_word(fp);
    (*info)->bmiHeader.biBitCount      = read_word(fp);
    (*info)->bmiHeader.biCompression   = read_dword(fp);
    (*info)->bmiHeader.biSizeImage     = read_dword(fp);
    (*info)->bmiHeader.biXPelsPerMeter = read_long(fp);
    (*info)->bmiHeader.biYPelsPerMeter = read_long(fp);
    (*info)->bmiHeader.biClrUsed       = read_dword(fp);
    (*info)->bmiHeader.biClrImportant  = read_dword(fp);
std::cerr<<MM_LOC<<'\n';
    if (infosize > 40)
	if (fread((*info)->bmiColors, infosize - 40, 1, fp) < 1)
            {
            /* Couldn't read the bitmap header - return NULL... */
std::cerr<<MM_LOC<<'\n';
            free(*info);
            fclose(fp);
            return (NULL);
            }

    /* Now that we have all the header info read in, allocate memory for *
     * the bitmap and read *it* in...                                    */
    if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
        bitsize = ((*info)->bmiHeader.biWidth *
                   (*info)->bmiHeader.biBitCount + 7) / 8 *
  	           abs((*info)->bmiHeader.biHeight);
	std::cerr<<MM_LOC<<"bits is "<<bitsize<<'\n';
	bits =new unsigned char[bitsize]; 
    //if ((bits = malloc(bitsize)) == NULL)
     if ( bits==NULL)
        {std::cerr<<MM_LOC<<'\n';
        /* Couldn't allocate memory - return NULL! */
        free(*info);
        fclose(fp);
        return (NULL);
        }
	typedef int Ti;
	std::cerr<<MM_LOC<<'\n';
    if (Ti(fread(bits, 1, bitsize, fp)) < Ti(bitsize))
        {
        	std::cerr<<MM_LOC<<'\n';
        /* Couldn't read bitmap - free memory and return NULL! */
        free(*info);
        free(bits);
        fclose(fp);
        return (NULL);
        }
std::cerr<<MM_LOC<<'\n';
    /* Swap red and blue */
    length = ((*info)->bmiHeader.biWidth * 3 + 3) & ~3;
    for (y = 0; y < (*info)->bmiHeader.biHeight; y ++)
        for (ptr = bits + y * length, x = (*info)->bmiHeader.biWidth;
             x > 0;
	     x --, ptr += 3)
	    { // was 0,2 -> green went to red. 
	    	// 0,1 -> green became blue. 
	    temp   = ptr[0];
	    ptr[0] = ptr[2];
	    ptr[2] = temp;
	    }
std::cerr<<MM_LOC<<'\n';
    /* OK, everything went fine - return the allocated bitmap... */
    fclose(fp);
    return (bits);
    }



/*
 * 'read_word()' - Read a 16-bit unsigned integer.
 */

static unsigned short     /* O - 16-bit unsigned integer */
read_word(FILE *fp)       /* I - File to read from */
    {
    unsigned char b0, b1; /* Bytes from file */

    b0 = getc(fp);
    b1 = getc(fp);

    return ((b1 << 8) | b0);
    }


/*
 * 'read_dword()' - Read a 32-bit unsigned integer.
 */

static unsigned int               /* O - 32-bit unsigned integer */
read_dword(FILE *fp)              /* I - File to read from */
    {
    unsigned char b0, b1, b2, b3; /* Bytes from file */

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
    }


/*
 * 'read_long()' - Read a 32-bit signed integer.
 */

static int                        /* O - 32-bit signed integer */
read_long(FILE *fp)               /* I - File to read from */
    {
    unsigned char b0, b1, b2, b3; /* Bytes from file */

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return ((int)(((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
    }


/*
 * 'write_word()' - Write a 16-bit unsigned integer.
 */

#if 0

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

#endif // zero

#endif /* WIN32 */

