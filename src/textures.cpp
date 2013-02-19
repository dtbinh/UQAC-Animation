/*
 *  textures.cpp
 *  water
 *
 *  Created by ychirico on 09-02-28.
 *  Copyright 2009 UQAC. All rights reserved.
 *
 */

#include "textures.h"

#ifdef __MAC_OS_X__
#include <Carbon/Carbon.h>
#include <GLUT/GLUT.h>
#else
#include "loadjpeg.h"
#endif

using namespace std;

GLuint id_texture[nb_texture];


bool  load_texture(const string& image_path, GLuint& texture_name)
{ 
#ifndef __MAC_OS_X__
    return read_jpeg_file(image_path.c_str(), texture_name);
#else
    int dummy0, dummy1;    
    return load_texture(image_path, texture_name, dummy0, dummy1);
#endif
}

#ifdef __MAC_OS_X__
bool  load_texture(const string& image_path, GLuint& texture_name, int& w, int& h)
{		
    GLuint texture_width, texture_height;
    
    CFStringRef filepath = CFStringCreateWithCString(0, image_path.c_str(), kCFStringEncodingASCII);

    CFURLRef  image_url = CFURLCreateWithFileSystemPath(NULL, filepath, kCFURLPOSIXPathStyle, false);
    
    CGDataProviderRef jpeg_provider = CGDataProviderCreateWithURL(image_url);
    if ( !jpeg_provider )
    { 
        return false;
    }
       
    
	CGImageRef image_ref = CGImageCreateWithJPEGDataProvider(jpeg_provider, 0, true, kCGRenderingIntentDefault);
    
    CGDataProviderRelease(jpeg_provider);
    
	w = texture_width = CGImageGetWidth(image_ref);
    h = texture_height = CGImageGetHeight(image_ref);
	CGRect rect = {{0, 0}, {texture_width, texture_height}};
	void* pixels = calloc(4*texture_width , texture_height);
	
	CGColorSpaceRef space = CGColorSpaceCreateDeviceRGB();
	CGContextRef bm_context = CGBitmapContextCreate(pixels, 
                                                    texture_width, texture_height, 8,
                                                    4*texture_width, space,
                                                    kCGBitmapByteOrder32Host |
                                                    kCGImageAlphaPremultipliedFirst);
    CGContextDrawImage(bm_context, rect, image_ref); 
    
    glEnable(GL_TEXTURE_2D);    
    glGenTextures(1, &texture_name);	
    glBindTexture(GL_TEXTURE_2D, texture_name);  
    
    glPixelStorei(GL_UNPACK_ROW_LENGTH, texture_width);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture_width, texture_height,
                 0, GL_BGRA_EXT, GL_UNSIGNED_INT_8_8_8_8_REV, pixels);
    
    CGContextRelease(bm_context); 
    free(pixels);
    glDisable(GL_TEXTURE_2D);
    return true;
}
#endif

 

