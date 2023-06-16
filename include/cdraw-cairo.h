/* CDraw designed for use with Cairo Graphics */
/* This port is for the most part platform agnostic */

#ifndef CDRAW_H
#define CDRAW_H

#include <cairo/cairo.h>

typedef struct color_t {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} color_t;

typedef enum align_t {
    TOP,
    BOTTOM,
    MIDDLE,
    CENTER,
    LEFT,
    RIGHT
} align_t;

typedef cairo_surface_t surface;

/* Creates a new surface with the given left and right surfaces drawn beside
 * each other, aligned as per align.
 *
 * Alignments are either TOP, BOTTOM, or CENTER */
surface *DL_beside_align (surface *left, surface *right, align_t align);

/* Creates a new surface with the given left and right surfaces drawn beside
 * each other, aligned at the center of each surface. */
surface *DL_beside (surface *left, surface *right);

/* Creates a new surface with the given top and bottom surfaces on positioned
 * vertically relative to each other, algined by align.
 *
 * Alignments are either LEFT, RIGHT, or CENTER */
surface *DL_above_align(surface *top, surface *bottom, align_t align);

/* Creates a new surface with the given top and bottom surfaces on positioned
 * vertically relative to each other, algined at the center. */
surface *DL_above(surface *top, surface *bottom);

/* Creates a new surface with a rectangle drawn based on the given width,
 * height, and color. */
surface *DL_rectangle (int width, int height, color_t color);

/* Creates a new surface with a square drawn based on the given side length and
 * color */
surface *DL_square (int side, color_t color);

/* Create a new empty surface based on the given width and height */
surface *DL_empty (int width, int height);

/* Creates a new surface with the given text drawn on it with the given font 
 * and font size and color used. */
surface *DL_text(const char* text, int size, color_t color, const char* font, unsigned char bold, unsigned char italics);

/* Overlays the front surface over the back surface, aligned at the middle */
surface *DL_overlay (surface *back, surface *front);

/* Get the width of the surface */
int DL_get_width(surface *surface);

/* Get the height of the surface */
int DL_get_height(surface *surface);

/* Free the surface */
void DL_free_surface(surface *surface);

#endif /* CDRAW_H */