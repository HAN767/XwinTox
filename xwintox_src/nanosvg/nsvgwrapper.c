#define NANOSVG_IMPLEMENTATION  // Expands implementation
#define NANOSVGRAST_IMPLEMENTATION  // Expands implementation
#define NANOSVG_ALL_COLOR_KEYWORDS  // Include full list of color keywords.

#include <stdio.h>
#include <stdlib.h>

#include "nanosvg/nanosvgrast.h"

unsigned char *svgrast(const char *data, int W, int H, int S, double factor)
{
	char* newdata =strdup(data);
	struct NSVGimage* nimage = nsvgParse(newdata, "px", 97);
	free (newdata);
	// Create rasterizer (can be used to render multiple images).
	struct NSVGrasterizer* rast = nsvgCreateRasterizer();
	// Allocate memory for image
	unsigned char *img = (unsigned char *)malloc(W*H*4);

	// Rasterize
	nsvgRasterize(rast, nimage, 0,0,S * factor, img, W, H, W*4);
	return img;
}
