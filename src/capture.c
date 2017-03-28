#include "capture.h"

#include <stdlib.h>
#include <stdio.h>
#include <X11/Xutil.h>
#include <jpeglib.h>

#include "display_info.h"
#include "args.h"

void capture_sht(area_t area, char* filename)
{
	XImage* img = XGetImage(
			display_info.display,
			display_info.root,
			area.x,
			area.y,
			area.w,
			area.h,
			AllPlanes,
			ZPixmap
	);

	FILE* out = fopen(filename, "wb");
	char* buffer = malloc(sizeof(char) * 3 * img->width * img->height);

	for (int y = 0; y < img->height; y++)
	{
		for (int x = 0; x < img->width; x++)
		{
			unsigned long px = XGetPixel(img, x, y);
			unsigned int p = (y * img->width + x) * 3;

			buffer[p++] = (char)(px >> 16);
			buffer[p++] = (char)((px & 0xff00) >> 8);
			buffer[p++] = (char)(px & 0xff);
		}
	}

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr err;

	cinfo.err = jpeg_std_error(&err);

	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, out);

	cinfo.image_width = img->width;
	cinfo.image_height = img->height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, args.quality, 1);
	jpeg_start_compress(&cinfo, 1);

	while (cinfo.next_scanline < cinfo.image_height)
	{
		JSAMPROW row_pointer = &buffer[cinfo.next_scanline * img->width * (img->depth >> 3)];
		jpeg_write_scanlines(&cinfo, &row_pointer, 1);
	}

	free(buffer);
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	fclose(out);

	XDestroyImage(img);
}
