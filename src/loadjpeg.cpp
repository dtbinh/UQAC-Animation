

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cstdio>
#include <csetjmp>
#include <memory.h>
#include <iostream>

#include <jpeglib.h>

#include "diagnostic.h"



using namespace std;

static jpeg_decompress_struct cinfo;
static jpeg_error_mgr jerr;


bool read_jpeg_file(const char * filename, GLuint& texture_name)
{

	FILE * infile;
	JSAMPARRAY buffer;	
	unsigned char* pixels_jpg;	
	int row_stride;
	jpeg_error_mgr jerr;		


    if ((infile = fopen(filename, "rb")) == NULL) 
	{
		fprintf(stderr, "Can't open %s\n", filename);
		return false;
    }

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

    jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, TRUE);

	jpeg_start_decompress(&cinfo);

	row_stride = cinfo.output_width*cinfo.output_components;
	buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
	
	// Opengl ne supporte pas RGB.
	pixels_jpg = (unsigned char*)malloc(cinfo.output_height*cinfo.output_width*cinfo.output_components);


	cout << cinfo.output_components << endl;

	while (cinfo.output_scanline < cinfo.output_height)
	{
		JDIMENSION read_now = jpeg_read_scanlines(&cinfo, buffer, 1);
		memcpy(&pixels_jpg[(cinfo.output_scanline - read_now)*cinfo.output_width*cinfo.output_components], buffer[0], row_stride);
	}

	cout << (int)pixels_jpg[0] << ' ' <<  (int)pixels_jpg[1] << ' ' << (int)pixels_jpg[2] << ' ' << (int)pixels_jpg[3] << endl;
	cout << (int)pixels_jpg[4] << ' ' <<  (int)pixels_jpg[5] << ' ' << (int)pixels_jpg[6] << ' ' << (int)pixels_jpg[7] << endl;

    glEnable(GL_TEXTURE_2D);    
    glGenTextures(1, &texture_name);	
    glBindTexture(GL_TEXTURE_2D, texture_name);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, cinfo.output_width);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int k;
	unsigned char	*pixels, *p, *q;

	switch ( cinfo.output_components )
	{
		case 1:
			pixels = (unsigned char*)malloc(cinfo.output_height*cinfo.output_width*4);
			for ( k = 0, p = pixels, q = pixels_jpg; k < cinfo.output_height*cinfo.output_width; k++, p+=4, q++ )
			{
				p[0] = q[0];
				p[1] = q[0];
				p[2] = q[0];
				p[3] = 0xFF;	
			}
			break;

		case 3:
			pixels = (unsigned char*)malloc(cinfo.output_height*cinfo.output_width*4);
			for ( k = 0, p = pixels, q = pixels_jpg; k < cinfo.output_height*cinfo.output_width; k++, p+=4, q+=3)
			{
				p[0] = q[0];
				p[1] = q[1];
				p[2] = q[2];
				p[3] = 0xFF;	
			}
			break;

	}

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,  cinfo.output_width, cinfo.output_height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, pixels);
    free(pixels);

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);

	free(pixels_jpg);
	return true;
}

