/* CDraw designed for use with Qt's Pixmaps */
/* This port is a C++ port, however this can be used with C just fine. */

#ifndef CDRAW_H
#define CDRAW_H

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

/* We typedef surface to 'void' here because this is a c library and qt is a 
 * C++ library, while there is nothing truly stoping us from using and 
 * returning a C++ class, which we are doing, C will not recognize it as such. 
 * Therefore we just say our surface, which is a QPixmap class, is actually 
 * just a void pointer. */
typedef void surface;

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
surface *DL_above(surface *top, surface *bot);

/* Creates a new surface with a rectangle drawn based on the given width,
 * height, and color. */
surface *DL_rectangle (int w, int h, color_t color);

/* Creates a new surface with a square drawn based on the given side length and
 * color */
surface *DL_square (int s, color_t color);

/* Create a new empty surface based on the given width and height */
surface *DL_empty (int w, int h);

/* Creates a new surface with the given text drawn on it with the given font 
 * and font size and color used. */
surface *DL_text(const char* text, int size, color_t color, const char* font, unsigned char bold, unsigned char italics);

/* Overlays the front surface over the back surface, aligned at the middle */
surface *DL_overlay (surface *back, surface *front);

/* Get the width of the surface */
int DL_get_width(surface *surf);

/* Get the height of the surface */
int DL_get_height(surface *surf);

/* Free the surface */
void DL_free_surface(surface *surf);

#endif /* CDRAW_H */
