/* CDraw designed for use with Cairo Graphics */
/* This port is for the most part platform agnostic */

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
surface *DL_beside_align (surface *left, surface *right, align_t align) {
    surface *new;
    cairo_t *cr;
    int newWidth, newHeight;
    int x, y;
    int leftW, leftH, rightW, rightH;

    /* Get the width of our left surface */
    leftW = cairo_image_surface_get_width(left);
    leftH = cairo_image_surface_get_height(left);

    /* Same for the right */
    rightW = cairo_image_surface_get_width(right);
    rightH = cairo_image_surface_get_height(right);

    /* Since we are placing the images beside one another, the new width will
     * be the sum of the left and right width. And the height will be the
     * height of the larger surface */

    newWidth = leftW + rightW;

    if(leftH > rightH) {
        newHeight = leftH;
    }
    else{
        newHeight = rightH;
    }

    /* Create our new surface, and get a cairo context for it */
    new = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, newWidth, newHeight);
    cr = cairo_create(new);

    /* The x to draw our left side image will always be 0, our y will change
     * based on our align, so perform those calculations here then draw it to
     * our new surface */
    x = 0;

    if (align == TOP)           y = 0;
    else if(align == BOTTOM)    y = newHeight - leftH;
    else                        y = (newHeight / 2.0) - (leftH / 2.0);

    cairo_set_source_surface (cr, left, x, y);
    cairo_paint (cr);

    /* Now do the same for the right side. The y is the same process as the
     * left, but this time the x is the width of left image. */
    x = leftW;

    if (align == TOP)           y = 0;
    else if(align == BOTTOM)    y = newHeight - rightH;
    else                        y = (newHeight / 2.0) - (rightH / 2.0);

    cairo_set_source_surface (cr, right, x, y);
    cairo_paint (cr);

    /* We are done drawing, kill our cairo context */
    cairo_destroy(cr);

    return new;
}

/* Creates a new surface with the given left and right surfaces drawn beside
 * each other, aligned at the center of each surface. */
surface *DL_beside (surface *left, surface *right) {
    return DL_beside_align(left, right, CENTER);
}

/* Creates a new surface with the given top and bottom surfaces on positioned
 * vertically relative to each other, algined by align.
 *
 * Alignments are either LEFT, RIGHT, or CENTER */
surface *DL_above_align(surface *top, surface *bottom, align_t align) {
    surface *ret;
    cairo_t *cr;
    int newWidth, newHeight;
    int topW, topH, botW, botH;
    int x, y;

    topW = cairo_image_surface_get_width(top);
    topH = cairo_image_surface_get_height(top);

    botW = cairo_image_surface_get_width(bottom);
    botH = cairo_image_surface_get_height(bottom);

    if (botW > topW) {
        newWidth = botW;
    }
    else{
        newWidth = topW;
    }

    newHeight = topH + botH;

    ret = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, newWidth, newHeight);
    cr = cairo_create(ret);

    y = 0;

    if (align == LEFT)          x = 0;
    else if(align == RIGHT)     x = newWidth - topW;
    else                        x = (newWidth / 2.0) - (topW / 2.0);

    cairo_set_source_surface (cr, top, x, y);
    cairo_paint (cr);

    y = topH;
    if (align == LEFT)          x = 0;
    else if(align == RIGHT)     x = newWidth - botW;
    else                        x = (newWidth / 2.0) - (botW / 2.0);

    cairo_set_source_surface (cr, bottom, x, y);
    cairo_paint (cr);

    cairo_destroy(cr);

    return ret;
}

/* Creates a new surface with the given top and bottom surfaces on positioned
 * vertically relative to each other, algined at the center. */
surface *DL_above(surface *top, surface *bot) {
    return DL_above_align(top, bot, CENTER);
}

/* Creates a new surface with a rectangle drawn based on the given width,
 * height, and color. */
surface *DL_rectangle (int width, int height, color_t color) {
    surface *ret;
    cairo_t *cr;

    /* Create our new surface and get a cairo context for it */
    ret = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cr = cairo_create(ret);

    /* Set our brush color for the rectangle, then draw it */
    cairo_set_source_rgb(cr, (double)(color.r / 255.0), (double)(color.g / 255.0), (double)(color.b / 255.0));

    cairo_rectangle(cr, 0, 0, width, height);
    cairo_paint(cr);

    /* We are done drawing here */
    cairo_destroy(cr);

    return ret;
}

/* Creates a new surface with a square drawn based on the given side length and
 * color */
surface *DL_square (int side, color_t color) {
    return DL_rectangle(side, side, color);
}

/* Create a new empty surface based on the given width and height */
surface *DL_empty (int width, int height) {
    surface *ret;

    ret = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);

    return ret;
}

/* Creates a new surface with the given text drawn on it with the given font 
 * and font size and color used. */
surface *DL_text(const char* text, int size, color_t color, const char* font, unsigned char bold, unsigned char italics) {
    /* We need to create a dummy surface so we can size up our text before creating 
     * our actual surface */
    surface *ret;
    cairo_t *cr;

    cairo_text_extents_t te;
    cairo_font_extents_t fe;

    int w, h;
    int fontSlant, fontWeight;

    /* Select our font properties and then size up our font */ 
    ret = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 0, 0);
    cr = cairo_create(ret);

    if (bold) {
        fontWeight = CAIRO_FONT_WEIGHT_BOLD;
    }
    else {
        fontWeight = CAIRO_FONT_WEIGHT_NORMAL;
    }

    if(italics) {
        fontSlant = CAIRO_FONT_SLANT_ITALIC;
    }
    else {
        fontSlant = CAIRO_FONT_SLANT_NORMAL;
    }

    cairo_select_font_face (cr, font, fontSlant, fontWeight);
    cairo_set_font_size (cr, size);

    cairo_text_extents(cr, text, &te);
    cairo_font_extents(cr, &fe);

    /* Since we were just drawing to the dummy surface, we need to free it and 
     * destroy the context */
    cairo_destroy(cr);
    cairo_surface_destroy(ret);

    w = te.x_advance;
    h = fe.ascent + fe.descent;

    /* Now create our real context and surface we will actually use */
    ret = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, w, h);
    cr = cairo_create (ret);

    cairo_select_font_face (cr, font, fontSlant, fontWeight);
    cairo_set_font_size (cr, size);

    cairo_set_source_rgb(cr, (double)(color.r / 255.0), (double)(color.g / 255.0), (double)(color.b / 255.0));
    
    cairo_move_to (cr, 0, fe.ascent);

    /* Since cairo_show_text is described as a "toy api", we should probably move
     * away from it or port to pango (or have a compile flag to use it) */
    cairo_show_text (cr, text);

    cairo_destroy (cr);

    return ret;
}

/* TODO: Add DL_overlay_align */

/* Overlays the front surface over the back surface, aligned at the middle */
surface *DL_overlay (surface *back, surface *front) {
    surface *ret;
    cairo_t *cr;
    int backW, backH, frontW, frontH;
    int newWidth, newHeight;
    int x, y;

    /* Get the width and height of the back, then make sure the new height and 
     * width of the new image is the larger height and the larger width */
    backW = cairo_image_surface_get_width(back);
    backH = cairo_image_surface_get_height(back);
    frontW = cairo_image_surface_get_width(front);
    frontH = cairo_image_surface_get_height(front);

    if(frontW > backW) {
        newWidth = frontW;
    }
    else{
        newWidth = backW;
    }

    if(frontH > backH) {
        newHeight = frontH;
    }
    else{
        newHeight = backH;
    }

    ret = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, newWidth, newHeight);
    cr = cairo_create(ret);

    x = (newWidth / 2.0) - (backW / 2.0);
    y = (newHeight / 2.0) - (backH / 2.0);

    cairo_set_source_surface (cr, back, x, y);
    cairo_paint(cr);

    x = (newWidth / 2.0) - (frontW / 2.0);
    y = (newHeight / 2.0) - (frontH / 2.0);

    cairo_set_source_surface (cr, front, x, y);
    cairo_paint(cr);

    cairo_destroy(cr);
    return ret;
}

/* Get the width of the surface */
int DL_get_width(surface *surf) {
    return cairo_image_surface_get_width(surf);
}

/* Get the height of the surface */
int DL_get_height(surface *surf) {
    return cairo_image_surface_get_height(surf);
}

/* Free the surface */
void DL_free_surface(surface *surf) {
    cairo_surface_destroy(surf);
}
