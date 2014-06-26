/*
 * Copyright 2002-2010 Based on a libpng example writtent by Guillaume
 * Cottenceau. Modified by Maxime Biais to transform it to a
 * non-premultiplied alpha TO premultiplied alpha converter.
 * http://twitter.com/maximeb
 *
 * This software may be freely redistributed under the terms
 * of the X11 license.
 *
 */

/*
 Modified for use in the Chilli Source engine.
 03/02/2012, Ian Copland: Added premultiplication of palettised pngs
 25/06/2014, Ian Copland: Minor tidy up changes.
 */
 
/*
 Defines
*/
#define PNG_DEBUG 3
#define DEFAULT_PALETTE_SIZE (256)
#define DEFAULT_COLOUR_PALETTE_SIZE (DEFAULT_PALETTE_SIZE*3)
#define DEFAULT_ALPHA_PALETTE_SIZE DEFAULT_PALETTE_SIZE
#define SAFE_ALLOCATE(pointer, size, type) if(pointer!=NULL){abort_("trying to reallocate an already allocated pointer!");}else{pointer=(type)malloc(size);}
#define SAFE_DELETE(pointer) if (pointer!=NULL){free(pointer);pointer=NULL;}
#define IN_ILOOP_RED (i*3+0)
#define IN_ILOOP_GREEN (i*3+1)
#define IN_ILOOP_BLUE (i*3+2)
#define IN_ILOOP_ALPHA (i)

/*
 Includes
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "LibPng/png.h"
#include "PNGAlphaPremultiplier.h"
#include "CLogging.h"
/*
 Global vars
*/
const char* in_filename;
int x, y;
int width, height;
png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep *row_pointers;

/*palettised thingies*/
int i = 0; 
unsigned char* colour_palette = NULL;
unsigned char* alpha_palette = NULL;
int palette_size = 0;
png_color_16p trans_colour;

int dwNumAlpha = 0;

/*
 Abort:
 Bails from the application with and error message.
*/
void abort_(const char * s, ...) 
{
  va_list args;
  va_start(args, s);
  char abyMessage[1024];
  vsprintf(abyMessage, s, args);
  LogFatal(abyMessage);
  va_end(args);
}

/*
Print Message:
Prints out a message to std out.
*/
void print_message(const char * s, ...) 
{
  va_list args;
  va_start(args, s);
  char abyMessage[1024];
  vsprintf(abyMessage, s, args);
  LogVerbose(abyMessage);
  va_end(args);
}

/*
Print Warning:
Prints out a warning
*/
void print_warning(const char * s, ...) 
{
  va_list args;
  va_start(args, s);
  char abyMessage[1024];
  vsprintf(abyMessage, s, args);
  LogWarning(abyMessage);
  va_end(args);
}

/*
 Read PNG File:
 Reads any png file.
*/
void read_png_file(const char* file_name) 
{
  char header[8];    /*8 is the maximum size that can be checked*/

  /* open file and test for it being a png */
  FILE *fp = fopen(file_name, "rb");
  if (!fp)
	abort_("[read_png_file] File %s could not be opened for reading", file_name);
  fread(header, 1, 8, fp);
  if (png_sig_cmp((png_byte*)header, 0, 8))
	abort_("[read_png_file] File %s is not recognized as a PNG file", file_name);


  /* initialize stuff */
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr)
	abort_("[read_png_file] png_create_read_struct failed");

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
	abort_("[read_png_file] png_create_info_struct failed");

  if (setjmp(png_jmpbuf(png_ptr)))
	abort_("[read_png_file] Error during init_io");

  png_init_io(png_ptr, fp);
  png_set_sig_bytes(png_ptr, 8);

  png_read_info(png_ptr, info_ptr);

  width = png_get_image_width(png_ptr, info_ptr);
  height = png_get_image_height(png_ptr, info_ptr);
  color_type = png_get_color_type(png_ptr, info_ptr);
  bit_depth = png_get_bit_depth(png_ptr, info_ptr);

  number_of_passes = png_set_interlace_handling(png_ptr);
  png_read_update_info(png_ptr, info_ptr);


  /* read file */
  if (setjmp(png_jmpbuf(png_ptr)))
	abort_("[read_png_file] Error during read_image");

  row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
  for (y=0; y<height; y++)
	row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));

  png_read_image(png_ptr, row_pointers);

  fclose(fp);
}
/*
 Write PNG File:
 Outputs a normal pre-multipled png.
*/
void write_png_file(const char* file_name) 
{
  /* create file */
  FILE *fp = fopen(file_name, "wb");
  if (!fp)
	abort_("[write_png_file] File %s could not be opened for writing", file_name);


  /* initialize stuff */
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr)
	abort_("[write_png_file] png_create_write_struct failed");

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
	abort_("[write_png_file] png_create_info_struct failed");

  if (setjmp(png_jmpbuf(png_ptr)))
	abort_("[write_png_file] Error during init_io");

  png_init_io(png_ptr, fp);


  /* write header */
  if (setjmp(png_jmpbuf(png_ptr)))
	abort_("[write_png_file] Error during writing header");

  png_set_IHDR(png_ptr, info_ptr, width, height,
			   bit_depth, color_type, PNG_INTERLACE_NONE,
			   PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);


  /* write bytes */
  if (setjmp(png_jmpbuf(png_ptr)))
	abort_("[write_png_file] Error during writing bytes");

  png_write_image(png_ptr, row_pointers);


  /* end write */
  if (setjmp(png_jmpbuf(png_ptr)))
	abort_("[write_png_file] Error during end of write");

  png_write_end(png_ptr, NULL);

  /* cleanup heap allocation */
  for (y=0; y<height; y++)
	free(row_pointers[y]);
  free(row_pointers);

  fclose(fp);
}
/*
 Check Is Premultiplied:
 Checks whether or not a normal png is already premultiplied
*/
int check_is_premultiplied() 
{	   
  for (y = 0; y < height; y++) 
  {
	png_byte* row = row_pointers[y];
	for (x = 0; x < width; x++) 
	{
	  png_byte* ptr = &(row[x*4]);
	  if ((ptr[3] != 255) && (ptr[0] > ptr[3] || ptr[1] > ptr[3] \
							  || ptr[2] > ptr[3]))
		  return 0;
	}
  }
  
  return 1;
}
/*
 Convert File:
 Conversion to pre-multiplied alpha for normal pngs
*/
void convert_file() 
{
  for (y = 0; y < height; y++) 
  {
	png_byte* row = row_pointers[y];
	for (x = 0; x < width; x++) 
	{
	  png_byte* ptr = &(row[x*4]);
	  ptr[0] = ptr[0] * (ptr[3] / 255.);
	  ptr[1] = ptr[1] * (ptr[3] / 255.);
	  ptr[2] = ptr[2] * (ptr[3] / 255.);
	}
  }
}
/*
 Write PNG File Palettised:
 Outputs a palettised pre-multipled png.
*/
void write_png_file_palettised(const char* file_name) 
{
  	/* create file */
  	FILE *fp = fopen(file_name, "wb");
  	if (!fp)
		abort_("[write_png_file] File %s could not be opened for writing", file_name);


 	 /* initialize stuff */
  	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  
  	if (!png_ptr)
		abort_("[write_png_file] png_create_write_struct failed");

  	info_ptr = png_create_info_struct(png_ptr);
  	if (!info_ptr)
		abort_("[write_png_file] png_create_info_struct failed");

 	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during init_io");

 	 png_init_io(png_ptr, fp);


  	/* write header */
  	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during writing header");

  	png_set_IHDR(png_ptr, info_ptr, width, height,
			   	bit_depth, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	/*set colour palette*/
	png_colorp palette = NULL;
	SAFE_ALLOCATE(palette, DEFAULT_PALETTE_SIZE*sizeof(png_color), png_colorp);
	for(i = 0; i < DEFAULT_PALETTE_SIZE; i++)
	{
		palette[i].red   = colour_palette[IN_ILOOP_RED];
		palette[i].green = colour_palette[IN_ILOOP_GREEN];
		palette[i].blue  = colour_palette[IN_ILOOP_BLUE];
	}
	png_set_PLTE(png_ptr, info_ptr, palette, DEFAULT_PALETTE_SIZE);
	
	/*set alpha palette*/
	png_bytep alpha_ptr = NULL;
	SAFE_ALLOCATE(alpha_ptr, dwNumAlpha, png_bytep);
	for(i = 0; i < dwNumAlpha; i++)
	{
		 alpha_ptr[i] = alpha_palette[IN_ILOOP_ALPHA];
	}
	png_set_tRNS(png_ptr, info_ptr, alpha_ptr, dwNumAlpha, NULL);
	
	/*write info*/
  	png_write_info(png_ptr, info_ptr);

  	/* write bytes */
  	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during writing bytes");

  	png_write_image(png_ptr, row_pointers);

	/* end write */
  	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during end of write");

  	png_write_end(png_ptr, NULL);

  	/* cleanup heap allocation */
  	for (y=0; y<height; y++)
		free(row_pointers[y]);
  	free(row_pointers);

  	/*cleanup palette data*/
  	SAFE_DELETE(colour_palette);
  	SAFE_DELETE(alpha_palette);
	SAFE_DELETE(alpha_ptr);
	SAFE_DELETE(palette);

  	fclose(fp);
}
/*
 Check Is Premultiplied Palettised:
 Checks whether or not a palettised png is already premultiplied
*/
int check_is_premultiplied_palettised() 
{	  
	for (i = 0; i < palette_size; i++) 
	{
		unsigned char r = colour_palette[IN_ILOOP_RED];
		unsigned char g = colour_palette[IN_ILOOP_GREEN];
		unsigned char b = colour_palette[IN_ILOOP_BLUE];
		unsigned char a = alpha_palette[IN_ILOOP_ALPHA];
		if ((a != 255) && (r > a || g > a || b > a))
			return 0;
	}
  
  	return 1;
}
/*
 Convert File Palettised:
 Conversion to pre-multiplied alpha for palettised pngs
*/
void convert_file_palettised() 
{
    for (i = 0; i < palette_size; i++) 
  	{
	  	colour_palette[IN_ILOOP_RED] = colour_palette[IN_ILOOP_RED] * (alpha_palette[IN_ILOOP_ALPHA] / 255.);
	  	colour_palette[IN_ILOOP_GREEN] = colour_palette[IN_ILOOP_GREEN] * (alpha_palette[IN_ILOOP_ALPHA] / 255.);
	  	colour_palette[IN_ILOOP_BLUE] = colour_palette[IN_ILOOP_BLUE] * (alpha_palette[IN_ILOOP_ALPHA] / 255.);
  	}
}
/*
 Read Palette:
 Reads the additional palette data.
*/
int read_palette()
{
	png_bytep alpha_ptr;
	png_colorp palette;
	
	/*Create the palette date*/
	SAFE_ALLOCATE(colour_palette, DEFAULT_COLOUR_PALETTE_SIZE, unsigned char*);
	png_get_PLTE(png_ptr, info_ptr, &palette, &palette_size);
	for(i = 0; i < palette_size; i++)
	{
		colour_palette[IN_ILOOP_RED] = palette[i].red;
		colour_palette[IN_ILOOP_GREEN] = palette[i].green;
		colour_palette[IN_ILOOP_BLUE] = palette[i].blue;
	}
				
	/*If there is transparency data, then get it too. if theres not, we have a problem...*/
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
	{
		png_get_tRNS(png_ptr, info_ptr, &alpha_ptr, &dwNumAlpha, &trans_colour);

		SAFE_ALLOCATE(alpha_palette, dwNumAlpha, unsigned char*);
		
		int dwSizeMismatch = 0;
		if (palette_size != dwNumAlpha)
		{
			print_warning("%s does not have a supported alpha palette size. Output will be identical input.", in_filename);
			dwSizeMismatch = 1;
		}
			
		for(i = 0; i < dwNumAlpha; i++)
		{
			alpha_palette[IN_ILOOP_ALPHA] = alpha_ptr[i];	
		}	

		if(dwSizeMismatch == 1)
			return 0;
	}
	else
	{
		SAFE_DELETE(colour_palette);
		print_warning("%s does not have an alpha channel. Output will be identical input.", in_filename);
		return 0;
	}
	
	return 1;
}
/*
 convert
 entry point for the conversion
*/
void convert(const char* input, const char* output) 
{
	in_filename = input;

	/*load the png*/
  	read_png_file(in_filename);
  	
  	/*check that the png of a type we can use*/
  	int type = png_get_color_type(png_ptr, info_ptr);
  	
  	/*perform different conversions based on the type*/
  	switch(type)
  	{
  		case PNG_COLOR_TYPE_RGBA:
			if (check_is_premultiplied() != 0)
				print_warning("%s is already pre-multiplied. Output will be identical to input.", in_filename);
			else
				convert_file();
  			write_png_file(output);
  			break;
		case PNG_COLOR_TYPE_RGB:
			write_png_file(output);
			break;
		case PNG_COLOR_TYPE_PALETTE:
  			if (read_palette() != 0)
			{
				if (check_is_premultiplied_palettised() != 0)
					print_warning("%s is already pre-multiplied. Output will be identical to input.", in_filename);
				else
					convert_file_palettised();
			}
  			write_png_file_palettised(output);
  			break;
  		default:
  			print_warning("%s is in an unsupported format. Output will be identical to input.", in_filename);
			write_png_file(output);
  			break;
  	}
}
