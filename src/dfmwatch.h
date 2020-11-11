#ifndef DFMWATCH_H
#define DFMWATCH_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class DfmWatch; }
QT_END_NAMESPACE
class ImageArea;

class DfmWatch : public QWidget
{
    Q_OBJECT

public:
    DfmWatch(QWidget *parent = nullptr);
    ~DfmWatch();

private:
    //Ui::dfmwatch *ui;
    void creatActions();

    ImageArea* imageArea;
};
#endif // DFMWATCH_H
