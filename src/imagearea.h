#ifndef IMAGEAREA_H
#define IMAGEAREA_H

#include <QWidget>
#include "odbshape.h"

class ImageArea : public QWidget
{
    Q_OBJECT

public:
    ImageArea(QWidget *parent = 0);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int show_polygon(QPainter& painter, point_node* head, int point_num);
};

#endif // IMAGEAREA_H
