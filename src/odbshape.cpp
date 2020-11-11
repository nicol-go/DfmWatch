#include "odbshape.h"
#include <iostream>
#include <fstream>
#include <string>
#include <climits>
#include <algorithm>

#define MIL2P         1
#define PI 3.141592653589793238

using namespace std;
static double Tolerance = 0.0000001;

/* two point to whole_info_type exclude the two point is same*/
double calc_2point_angle(point p1, point p2, double *angle)
{
    if (1.0 * (p1.y - p2.y) * (p2.y - p1.y) > -Tolerance)
    {
        if (p1.x < p2.x)
        {
            *angle = 0;
        }
        else
        {
            *angle = 180;
        }
    }
    else
    {
        if (1.0 * (p1.x - p2.x) * (p2.x - p1.x) > -Tolerance)
        {
            if (p1.y < p2.y)
            {
                *angle = 90;
            }
            else
            {
                *angle = 270;
            }
        }
        else
        {
            double a = (p1.y - p2.y)*1.0 / (p1.x - p2.x);
            *angle = atan(a) * 180 / PI;
            if (p1.x > p2.x && p1.y < p2.y)
            {
                *angle = 180 + *angle;
            }
            else
            {
                if (p1.x > p2.x && p1.y > p2.y)
                {
                    *angle = 180 + *angle;
                }
                else
                {
                    if (p1.x < p2.x && p1.y > p2.y)
                    {
                        *angle = 360 + *angle;
                    }
                }
            }
        }
    }

    return 0;
}

int break_arc_to_point(shape *item, point_node **points, int *point_num)
{
    double step_angle = 5;
    if (item->shape_type != 'A')
    {
        return (0);
    }

    double s_angle;
    double e_angle;

    calc_2point_angle(item->cur.center, item->cur.ps, &s_angle);
    calc_2point_angle(item->cur.center, item->cur.pe, &e_angle);
    if (item->cur.arc_direction == 1)
    {
        if (s_angle > e_angle)
        {
            e_angle += 360;
        }
    }
    else
    {
        if (s_angle <= e_angle)
        {
            s_angle += 360;
        }
    }

    double total_angle;
    int step_num;
    total_angle = e_angle - s_angle;
    step_num = (int)(abs(total_angle) / step_angle) + 1;
    step_angle = total_angle / step_num;

    int j;
    double now_angle = s_angle + step_angle;
    point_node *tmp_node = *points;
    for (j = 1; j < step_num; j++)
    {
        point_node *pt_node = new point_node;
        pt_node->p.x = long(item->cur.center.x + item->cur.arc_radius * cos(now_angle * PI / 180));

        int delta_x = pt_node->p.x - item->cur.center.x;
        int delta_y = (int)(sqrt(1.0 * item->cur.arc_radius * item->cur.arc_radius - 1.0 * delta_x * delta_x));
        if (sin(now_angle * PI / 180) < 0)
        {
            pt_node->p.y = long(item->cur.center.y - delta_y);
        }
        else
        {
            pt_node->p.y = long(item->cur.center.y + delta_y);
        }

        pt_node->next = NULL;

        tmp_node->next = pt_node;
        tmp_node = pt_node;
        (*point_num)++;

        now_angle += step_angle;
    }

    point_node *pt_node = new point_node;
    pt_node->p.x = item->cur.pe.x;
    pt_node->p.y = item->cur.pe.y;
    pt_node->next = NULL;

    tmp_node->next = pt_node;
    tmp_node = pt_node;
    *points = tmp_node;
    (*point_num)++;

    return 0;
}


double getvalue(ifstream& fp)
{
    int i;
    char c;
    char *end_str;
    double result;
    char strtmp[100];

    fp.get(c);
    i = 0;

    while (c == ' ')
    {
        fp.get(c);
    }

    while (c != ' ' && c != '\n')
    {
        strtmp[i++] = c;
        fp.get(c);
    }

    strtmp[i] = '\0';
    result = strtod(strtmp, &end_str);

    return result;
}

int get_int_value(ifstream& fp)
{
    double value;

    value = getvalue(fp);

    return ((int)(value * MIL2P));
}

//input data from file
int f_in(string filename, struct layer **layer)
{
    char c;
    char d;
    string filename2 = "C:/debug/feature";
    struct shape *pre_shape;
    struct shape *new_shape;
    struct node *new_node;
    struct node *pre_node;
    struct layer *pre_layer;
    struct layer *new_layer;
    struct node *head;

    ifstream inFile;

    if(filename.length() > 0){
        inFile.open(filename);
    }else{
        inFile.open(filename2);

    }
    if(!inFile){
        cout << "Unable to open file";
        exit(1); // terminate with error
    }

    *layer = NULL;
    /* read data from layer */
    /* A is Arc, L is Line, N is new layer, S is start surface, E is end of surface */
    /* filter out no need letter at the beginning */
    inFile.get(c);
    while (c != EOF && c != 'N')
    {
        inFile.get(c);
    }

    bool is_surf_first_edge = false;
    bool is_surface = false;
    while (c != EOF && c == 'N')
    {
        head = NULL;

        /* get the start of line or arc */
        while (c != EOF)
        {
            inFile.get(c);
            if (c == 'S')
            {
                is_surf_first_edge = true;
                is_surface = true;
            }
            if (c == 'L' || c == 'A')
            {
                break;
            }
        }

        /* collect line and arc for one layer */
        while (c != EOF && (c == 'L' || c == 'A'))
        {
            new_shape = (struct shape *) malloc(sizeof(struct shape));
            new_shape->next = NULL;

            if (!is_surface || is_surf_first_edge)
            {
                new_node = (struct node *) malloc(sizeof(struct node));
                new_node->points = NULL;
                new_node->point_num = 0;
                new_node->item = new_shape;

                if (is_surf_first_edge)
                {
                    new_node->complicate = true;
                }
                else
                {
                    new_node->complicate = false;
                }
            }

            inFile >> d;
            if (c == 'L')
            {
                new_shape->shape_type = 'L';
                new_shape->seg.ps.x = get_int_value(inFile);
                new_shape->seg.ps.y = get_int_value(inFile);
                new_shape->seg.pe.x = get_int_value(inFile);
                new_shape->seg.pe.y = get_int_value(inFile);
            }
            else if (c == 'A')  // c == 'A'
            {
                new_shape->shape_type = 'A';
                new_shape->cur.lt.x = get_int_value(inFile);
                new_shape->cur.lt.y = get_int_value(inFile);
                new_shape->cur.rb.x = get_int_value(inFile);
                new_shape->cur.rb.y = get_int_value(inFile);
                new_shape->cur.ps.x = get_int_value(inFile);
                new_shape->cur.ps.y = get_int_value(inFile);
                new_shape->cur.pe.x = get_int_value(inFile);
                new_shape->cur.pe.y = get_int_value(inFile);
                new_shape->cur.center.x = get_int_value(inFile);
                new_shape->cur.center.y = get_int_value(inFile);
                new_shape->cur.arc_radius = get_int_value(inFile);
                new_shape->cur.arc_direction = (int)(getvalue(inFile));
            }

            if (is_surf_first_edge)
            {
                pre_shape = new_shape;
            }
            else
            {
                if (is_surface)
                {
                    pre_shape->next = new_shape;
                    pre_shape = new_shape;
                }
            }

            if (!is_surface || is_surf_first_edge)
            {
                new_node->next = NULL;
                if (head == NULL)
                {
                    head = new_node;
                    pre_node = new_node;
                }
                else
                {
                    pre_node->next = new_node;
                    pre_node = new_node;
                }
            }

            /* still same layer */
            if (is_surf_first_edge)
            {
                is_surf_first_edge = false;
            }
            while (c != EOF && c != 'N')
            {
                inFile.get(c);
                if (c == 'E')
                {
                    is_surface = false;
                }

                if (c == 'S')
                {
                    is_surface = true;
                    is_surf_first_edge = true;
                }

                if (c == 'L' || c == 'A')
                {
                    break;
                }
            }
        }

        if (head == NULL)
        {
            break;
        }

        new_layer = (struct layer *) malloc(sizeof(struct layer));
        new_layer->item = head;
        new_layer->b_visible = true;
        new_layer->next = NULL;
        new_layer->contour_item = NULL;

        if (*layer == NULL)
        {
            *layer = new_layer;
            pre_layer = new_layer;
        }
        else
        {
            pre_layer->next = new_layer;
            pre_layer = new_layer;
        }
    } // end of if (fp==NULL)

    inFile.close();

    return 0;
}

void update_rect(point *pt, struct rect *area)
{
    if (pt->x < area->lt.x)
    {
        area->lt.x = pt->x;
    }
    if (pt->x > area->rb.x)
    {
        area->rb.x = pt->x;
    }

    if (pt->y > area->lt.y)
    {
        area->lt.y = pt->y;
    }
    if (pt->y < area->rb.y)
    {
        area->rb.y = pt->y;
    }
}

void get_rect(struct layer *layer, struct rect *area)
{
    struct node *tmp_node;
    struct layer *tmp_layer;
    struct shape *tmp_shape;

    /* init rect */
    area->lt.x = INT_MAX;
    area->lt.y = INT_MAX;
    area->rb.x = INT_MAX;
    area->rb.y = INT_MAX;

    tmp_layer = layer;
    while (tmp_layer != NULL)
    {
        if (!tmp_layer->b_visible)
        {
            tmp_layer = tmp_layer->next;
            continue;
        }

        /* loop item in head to get bigest rect */
        tmp_node = tmp_layer->item;
        while (tmp_node != NULL)
        {
            tmp_shape = tmp_node->item;
            while (tmp_shape != NULL)
            {
                if (tmp_shape->shape_type == 'L')
                {
                    update_rect(&tmp_shape->seg.ps, area);
                    update_rect(&tmp_shape->seg.pe, area);
                }
                else
                {
                    update_rect(&tmp_shape->cur.lt, area);
                    update_rect(&tmp_shape->cur.rb, area);
                }
                tmp_shape = tmp_shape->next;
            }

            tmp_node = tmp_node->next;
        }
        tmp_layer = tmp_layer->next;
    }
}

void DPtoLP(point *p, const show_info *info)
{
    int logical_x;
    int logical_y;

    logical_x = *(info->left_x) + (int)(p->x * *(info->scale));
    logical_y = *(info->top_y) - (int)(p->y * *(info->scale));

    p->x = logical_x;
    p->y = logical_y;
}

void LPtoDP(point *p, const show_info *info)
{
    int dp_x;
    int dp_y;

    dp_x = (int)((p->x - *(info->left_x)) / *(info->scale));
    dp_y = (int)((*(info->top_y) - p->y) / *(info->scale));

    p->x = dp_x;
    p->y = dp_y;
}

void calc_scale(struct rect *logical_rect, point *window_size, double *scale)
{
    int dx;
    int dy;
    dx = abs(logical_rect->rb.x - logical_rect->lt.x);
    dy = abs(logical_rect->lt.y - logical_rect->rb.y);
    *scale = max((dx*1.0) / window_size->x, (dy*1.0) / window_size->y);

}

