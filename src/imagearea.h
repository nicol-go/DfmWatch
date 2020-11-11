#ifndef IMAGEAREA_H
#define IMAGEAREA_H

#include <QWidget>

class ImageArea : public QWidget
{
    Q_OBJECT

public:
    ImageArea(QWidget *parent = 0);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // IMAGEAREA_H
