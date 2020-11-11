#include <QPainter>
#include "imagearea.h"
#include "odbshape.h"
#include <iostream>

using namespace std;

struct layer* g_layer = NULL;
struct rect g_present_rect;
point g_previous_point;
point g_present_point;
point g_window_size;
double g_scale = 1;
int g_left_x = 0;
int g_top_y = 0;
bool g_fill = false;
bool g_endpoint = false;
bool need_show_zoom_area = false;

struct show_info g_info = { &g_left_x, &g_top_y, &g_scale };

ImageArea::ImageArea(QWidget *parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Text);
    string filename;
    f_in(filename, &g_layer);

    get_rect(g_layer, &g_present_rect);
}

QSize ImageArea::minimumSizeHint() const
{
    return QSize(100, 100);
}


QSize ImageArea::sizeHint() const
{
    return QSize(400, 200);
}


void ImageArea::paintEvent(QPaintEvent * /* event */)
{
    static const QPoint points[4] = {
         QPoint(10, 80),
         QPoint(20, 10),
         QPoint(80, 30),
         QPoint(90, 70)
    };

    QPainter painter(this);
    painter.drawPolygon(points,4);
}

