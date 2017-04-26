#include "capture.h"
#include <stdlib.h>
#include <stdio.h>
#include <X11/Xutil.h>
#include <jpeglib.h>
#include "display_info.h"
#include "args.h"

XImage* get_selection_ximage(area_t area)
{
	return XGetImage(
			display_info.display,
			display_info.root,
			area.x,
			area.y,
			(unsigned int)area.w,
			(unsigned int)area.h,
			AllPlanes,
			ZPixmap
	);
}

char* ximage_to_buffer(XImage* img)
{
	unsigned long img_size = sizeof(char) * 3 * img->width * img->height;
	char* buffer = malloc(img_size);

	for (int y = 0; y < img->height; y++)
	{
		for (int x = 0; x < img->width; x++)
		{
			unsigned long px = XGetPixel(img, x, y);
			unsigned int p = (unsigned int)(y * img->width + x) * 3;

			buffer[p++] = (char)(px >> 16);
			buffer[p++] = (char)((px & 0xff00) >> 8);
			buffer[p++] = (char)(px & 0xff);
		}
	}

	return buffer;
}

unsigned char* capture_sht(area_t area, unsigned long* size)
{
	unsigned char* result = NULL;
	XImage* img = get_selection_ximage(area);
	char* buffer = ximage_to_buffer(img);

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr err;

	cinfo.err = jpeg_std_error(&err);

	jpeg_create_compress(&cinfo);
	jpeg_mem_dest(&cinfo, &result, size);

	cinfo.image_width = (JDIMENSION)img->width;
	cinfo.image_height = (JDIMENSION)img->height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, arg_values.quality, 1);
	jpeg_start_compress(&cinfo, 1);

	while (cinfo.next_scanline < cinfo.image_height)
	{
		JSAMPROW row_pointer = (JSAMPROW)&buffer[cinfo.next_scanline * img->width * (img->depth >> 3)];
		jpeg_write_scanlines(&cinfo, &row_pointer, 1);
	}

	free(buffer);
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

	XDestroyImage(img);

	return result;
}
