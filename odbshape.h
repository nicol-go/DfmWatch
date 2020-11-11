#ifndef ODBSHAPE_H
#define ODBSHAPE_H

#include <stdio.h>
#include <math.h>

#define MIRCO_EPS 10
#define ARC_TOL   100

struct point{
    int x;
    int y;
};

struct point_node
{
    point p;
    point_node *next;
};

struct segment
{
    point ps;
    point pe;
};

struct curve
{
    point lt;
    point rb;
    point ps;
    point pe;
    point center;
    long arc_radius;
    int arc_direction;
};

struct rect
{
    point lt;
    point rb;
};

struct shape
{
    char  shape_type;
    union
    {
        struct segment seg;
        struct curve   cur;
    };
    shape *next;
};

struct node
{
    shape *item;
    point_node *points;
    int point_num;
    bool complicate;
    node *next;
};

struct layer
{
    node *item;
    node *contour_item;  /* contourized shapes */
    layer *next;

    bool b_visible;
};

struct show_info
{
    int *left_x;
    int *top_y;
    double *scale;
};

int points_to_shape(struct node *cont_node);

int break_arc_to_point(shape *item, point_node **points, int *point_num);

double calc_2point_angle(point p1, point p2, double *angle);

int f_in(char *filename, struct layer **layer);

void get_rect(struct layer *layer, struct rect *area);

void calc_scale(struct rect *logical_rect, point *window_size, double *scale);

void DPtoLP(point *p, const show_info *info);
void LPtoDP(point *p, const show_info *info);



#endif // ODBSHAPE_H
