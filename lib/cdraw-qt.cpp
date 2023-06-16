#include <QPixmap>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <stdio.h>

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
extern "C" surface *DL_beside_align (surface *l, surface *r, align_t align) {
    QPixmap *left, *right;

    /* Set our pixmaps to their proper pixmap type */
    left = (QPixmap*)l;
    right = (QPixmap*)r;

    int newWidth, newHeight;
    int x, y;
    int leftW, leftH, rightW, rightH;

    /* Get the width of our left and right surfaces */
    leftW = left->width();
    leftH = left->height();

    rightW = right->width();
    rightH = right->height();

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
    
    /* Create our new surface, and get a QPainter for it */
    QPixmap *ret = new QPixmap(newWidth, newHeight);

    /* Fill the pixmap so we arent writing to uninitialized data */
    ret->fill(QColor("transparent"));

    QPainter p(ret);

    /* The x to draw our left side image will always be 0, our y will change
     * based on our align, so perform those calculations here then draw it to
     * our new surface */
    x = 0;

    if (align == TOP)           y = 0;
    else if(align == BOTTOM)    y = newHeight - leftH;
    else                        y = (newHeight / 2.0) - (leftH / 2.0);

    p.drawPixmap(x, y, *left);

    /* Now do the same for the right side. The y is the same process as the
     * left, but this time the x is the width of left image. */
    x = leftW;

    if (align == TOP)           y = 0;
    else if(align == BOTTOM)    y = newHeight - rightH;
    else                        y = (newHeight / 2.0) - (rightH / 2.0);

    p.drawPixmap(x, y, *right);

    /* Cast our return to void so we can return it to a C context */
    return (void*)ret;
}

/* Creates a new surface with the given left and right surfaces drawn beside
 * each other, aligned at the center of each surface. */
extern "C" surface *DL_beside (surface *left, surface *right) {
    return DL_beside_align(left, right, CENTER);
}

/* Creates a new surface with the given top and bottom surfaces on positioned
 * vertically relative to each other, algined by align.
 *
 * Alignments are either LEFT, RIGHT, or CENTER */
extern "C" surface *DL_above_align(surface *t, surface *b, align_t align) {
    QPixmap *top, *bottom;
    QPixmap *ret;

    top = (QPixmap*)t;
    bottom = (QPixmap*)b;

    int newWidth, newHeight;
    int topW, topH, botW, botH;
    int x, y;

    topW = top->width();
    topH = top->height();

    botW = bottom->width();
    botH = bottom->height();

    if (botW > topW) {
        newWidth = botW;
    }
    else{
        newWidth = topW;
    }

    newHeight = topH + botH;

    ret = new QPixmap(newWidth, newHeight);
    ret->fill(QColor("transparent"));

    QPainter p(ret);

    y = 0;

    if (align == LEFT)          x = 0;
    else if(align == RIGHT)     x = newWidth - topW;
    else                        x = (newWidth / 2.0) - (topW / 2.0);

        p.drawPixmap(x, y, *top);

    y = topH;
    if (align == LEFT)          x = 0;
    else if(align == RIGHT)     x = newWidth - botW;
    else                        x = (newWidth / 2.0) - (botW / 2.0);

        p.drawPixmap(x, y, *bottom);

    return (void*)ret;
}

/* Creates a new surface with the given top and bottom surfaces on positioned
 * vertically relative to each other, algined at the center. */
extern "C" surface *DL_above(surface *top, surface *bot) {
    return DL_above_align(top, bot, CENTER);
}

/* Creates a new surface with a rectangle drawn based on the given width,
 * height, and color. */
extern "C" surface *DL_rectangle (int w, int h, color_t color) {
    QPixmap *ret;

    ret = new QPixmap(w, h);
    ret->fill(QColor("transparent"));
    QPainter p(ret);

    p.fillRect(0, 0, w, h, QColor(color.r, color.g, color.b));

    return (void*)ret;
}

/* Creates a new surface with a square drawn based on the given side length and
 * color */
extern "C" surface *DL_square (int s, color_t color) {
    return DL_rectangle(s, s, color);
}

/* Create a new empty surface based on the given width and height */
extern "C" surface *DL_empty (int w, int h) {
    QPixmap *ret;

    ret = new QPixmap(w, h);
    ret->fill(QColor("transparent"));

    return (void*)ret;
}

/* Creates a new surface with the given text drawn on it with the given font 
 * and font size and color used. */
extern "C" surface *DL_text(const char* text, int size, color_t color, const char* font, unsigned char bold, unsigned char italics) {
    QFont fn(font, size);

    int w, h;
    int fontSlant, fontWeight;

    if (bold) {
        fn.setWeight(QFont::Bold);
    }

    if(italics) {
        fn.setItalic(true);
    }

    QFontMetrics fm(fn);

    /* Get our font size so we know how big to make our surface */
    w = fm.horizontalAdvance(QString(text));
    h = fm.height();

    QPixmap *ret;

    ret = new QPixmap(w, h);
    ret->fill(QColor("transparent"));

    QPainter p(ret);
    p.setFont(fn);
    p.setPen(QColor(color.r, color.g, color.b));
    p.drawText(0, 0, w, h, Qt::TextSingleLine, QString(text));

    return (void*)ret;
}

/* Overlays the front surface over the back surface, aligned at the middle */
extern "C" surface *DL_overlay (surface *b, surface *f) {
    QPixmap *ret;
    int backW, backH, frontW, frontH;
    int newWidth, newHeight;
    int x, y;
    QPixmap *back = (QPixmap*)b;
    QPixmap *front = (QPixmap*)f;

    /* Get the width and height of the back, then make sure the new height and 
     * width of the new image is the larger height and the larger width */
    backW = back->width();
    backH = back->height();
    frontW = front->width();
    frontH = front->height();

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

    ret = new QPixmap(newWidth, newHeight);
    ret->fill(QColor("transparent"));

    QPainter p(ret);

    x = (newWidth / 2.0) - (backW / 2.0);
    y = (newHeight / 2.0) - (backH / 2.0);

    p.drawPixmap(x, y, *back);

    x = (newWidth / 2.0) - (frontW / 2.0);
    y = (newHeight / 2.0) - (frontH / 2.0);

    p.drawPixmap(x, y, *front);

    return (void*)ret;
}

/* Get the width of the surface */
extern "C" int DL_get_width(surface *surf) {
    return ((QPixmap*)surf)->width();
}

/* Get the height of the surface */
extern "C" int DL_get_height(surface *surf) {
    return ((QPixmap*)surf)->height();
}

/* Free the surface */
extern "C" void DL_free_surface(surface *surf) {
    delete (QPixmap*)surf;
}
