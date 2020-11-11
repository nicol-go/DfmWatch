#include <QPainter>
#include "imagearea.h"
#include <iostream>

#define MAX_LAYER_NUMBER 5

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
    setPalette(QPalette(Qt::black));
    setAutoFillBackground(true);

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
    return QSize(600, 400);
}


void ImageArea::paintEvent(QPaintEvent * /* event */)
{
    struct layer* layer;
    struct node* tmp_node;
    point_node* head = NULL;
    point_node* points = NULL;
    static int size = 10;

    QPainter painter(this);
    //painter.setRenderHint(QPainter::Antialiasing);// much smoother

    g_window_size.x = this->width();
    g_window_size.y = this->height();
    calc_scale(&g_present_rect, &g_window_size, &g_scale);
    painter.translate(g_present_rect.lt.x, g_present_rect.lt.y);
    painter.scale(1, 1);
    g_left_x = g_present_rect.lt.x;
    g_top_y = g_present_rect.lt.y;

    int dx;
    int dy;
    dx = abs(g_present_rect.rb.x - g_present_rect.lt.x);
    dy = abs(g_present_rect.lt.y - g_present_rect.rb.y);

    int layer_num = 0;
    layer = g_layer;
    while (layer != NULL)
    {
        layer_num++;
        if (!layer->b_visible)
        {
            layer = layer->next;
            continue;
        }

        /* set color */
        QColor ncolor = Qt::white;
        switch (layer_num % MAX_LAYER_NUMBER)
        {
        case 1:
            ncolor.setRgb(255, 0, 0);
            break;
        case 2:
            ncolor.setRgb(0, 0, 255);
            break;
        case 3:
            ncolor.setRgb(0, 255, 0);
            break;
        case 4:
            ncolor.setRgb(255, 255, 0);
            break;
        default:
            ncolor.setRgb(0, 255, 255);
            break;
        }
        /* set pen */
        QPen pen;
        pen.setColor(ncolor);
        /* set brush */
        QBrush brush;
        brush.setColor(ncolor);
        
        painter.setPen(pen);
        painter.setBrush(brush);

        if (layer->contour_item != NULL)
        {
            tmp_node = layer->contour_item;
        }
        else
        {
            tmp_node = layer->item;
        }

        while (tmp_node != NULL)
        {
            struct shape* tmp_shape;
            int point_num = 0;
            bool b_need_break = false;
            bool b_need_fill = false;

            tmp_shape = tmp_node->item;
            b_need_fill = tmp_node->complicate;

            if (b_need_fill && tmp_node->points == NULL)
            {
                b_need_break = true;
            }

            while (tmp_shape != NULL)
            {
                if (tmp_shape->shape_type == 'L')
                {
                    if (b_need_fill)
                    {
                        if (b_need_break)
                        {
                            if (point_num == 0)
                            {
                                point_node* p_node = new point_node;
                                p_node->p.x = tmp_shape->seg.ps.x;
                                p_node->p.y = tmp_shape->seg.ps.y;
                                p_node->next = NULL;
                                points = p_node;
                                head = p_node;
                                point_num++;
                            }

                            point_node* p_node = new point_node;
                            p_node->p.x = tmp_shape->seg.pe.x;
                            p_node->p.y = tmp_shape->seg.pe.y;
                            p_node->next = NULL;
                            points->next = p_node;
                            points = p_node;
                            point_num++;
                        }
                    }
                    else
                    {
                        /*MoveToEx(hdc, tmp_shape->seg.ps.x, tmp_shape->seg.ps.y, NULL);
                        LineTo(hdc, tmp_shape->seg.pe.x, tmp_shape->seg.pe.y);
                        if (show_endpoint)
                        {
                            Rectangle(hdc, tmp_shape->seg.pe.x - size, tmp_shape->seg.pe.y + size,
                                tmp_shape->seg.pe.x + size, tmp_shape->seg.pe.y - size);
                        }*/
                    }
                }
                else
                {
                    if (b_need_fill && b_need_break)
                    {
                        if (point_num == 0)
                        {
                            point_node* pt_node = new point_node;
                            pt_node->p.x = tmp_shape->cur.ps.x;
                            pt_node->p.y = tmp_shape->cur.ps.y;
                            pt_node->next = NULL;
                            point_num++;
                            head = pt_node;
                            points = pt_node;
                        }
                        break_arc_to_point(tmp_shape, &points, &point_num);
                    }
                    else
                    {
                        /*SetArcDirection(hdc, tmp_shape->cur.arc_direction);
                        Arc(hdc,
                            tmp_shape->cur.center.x - (int)(tmp_shape->cur.arc_radius), tmp_shape->cur.center.y + (int)(tmp_shape->cur.arc_radius),
                            tmp_shape->cur.center.x + (int)(tmp_shape->cur.arc_radius), tmp_shape->cur.center.y - (int)(tmp_shape->cur.arc_radius),
                            tmp_shape->cur.ps.x, tmp_shape->cur.ps.y,
                            tmp_shape->cur.pe.x, tmp_shape->cur.pe.y);
                        if (show_endpoint)
                        {
                            Rectangle(hdc, tmp_shape->cur.pe.x - size, tmp_shape->cur.pe.y + size, tmp_shape->cur.pe.x + size, tmp_shape->cur.pe.y - size);
                        }*/
                    }
                }
                tmp_shape = tmp_shape->next;
            }

            if (b_need_fill && b_need_break)
            {
                tmp_node->points = head;
                tmp_node->point_num = point_num;
            }
            QVector<QPoint> qpoints;
            
            if (b_need_fill)
            {
                show_polygon(painter, tmp_node->points, tmp_node->point_num);
            }

            tmp_node = tmp_node->next;
        }

        layer = layer->next;
    }
}

int ImageArea::show_polygon(QPainter& painter, point_node* head, int point_num)
{
    QPoint* points = new QPoint[point_num];
    point_node* pt_node;
    point_node* pre_node;
    pt_node = head;

    for (int i = 0; pt_node != NULL; i++) {
        points[i].setX(pt_node->p.x);
        points[i].setY(pt_node->p.y);
        pre_node = pt_node;
        pt_node = pt_node->next;
    }

    painter.drawPolygon(points, point_num);
    return (1);
}

