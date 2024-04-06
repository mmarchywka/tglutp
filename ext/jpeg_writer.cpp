#include "jpeg_writer.h"

//static int do_quantization = 0;
static int quality_factor = 75;
static int smoothing_factor = 0;

void write_jpg(const char * fname,char *src, int w,int h)
{
 FILE* infile = fopen(fname, "w");
  	
  	struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
 
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, infile);

    cinfo.image_width = w;
    cinfo.image_height = h;

	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;
    

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality_factor, TRUE);
    cinfo.smoothing_factor = smoothing_factor;

    jpeg_start_compress(&cinfo, TRUE);

   
   const char * comm="Another fine Marchywka result";
   int len=strlen(comm);
	jpeg_write_marker(&cinfo, JPEG_COM, /*(void *)*/ (JOCTET*)comm,
			  len);

    JSAMPLE ** rowptr = cinfo.mem->alloc_sarray((j_common_ptr) &cinfo, JPOOL_IMAGE,
				      cinfo.input_components * w, 1);
	int zero=0;
//	int flip=(w-1)*h;
	int scan=h-1;
    while (cinfo.next_scanline < cinfo.image_height)
    {
	int line =4*w*(scan);
	JSAMPLE * buf = rowptr[0];
	for (  int i = 0; i < w; i++)
	{
	    if (cinfo.input_components == 3)
	    {
//		*buf++ = im->red[cinfo->next_scanline][i];
//		*buf++ = im->green[cinfo->next_scanline][i];
//		*buf++ = im->blue[cinfo->next_scanline][i];
		
		*buf++ = src[line+4*i];
		*buf++ = src[line+4*i+1];
		*buf++ = src[line+4*i+2];
		
	    }

	}
	zero+=w;
	--scan;
	jpeg_write_scanlines(&cinfo, rowptr, 1);
    }

    jpeg_finish_compress(&cinfo);

    jpeg_destroy_compress(&cinfo);
    
    fclose(infile); 
}

