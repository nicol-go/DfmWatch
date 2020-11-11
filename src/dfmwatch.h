#ifndef DFMWATCH_H
#define DFMWATCH_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class dfmwatch; }
QT_END_NAMESPACE

class dfmwatch : public QWidget
{
    Q_OBJECT

public:
    dfmwatch(QWidget *parent = nullptr);
    ~dfmwatch();

private:
    Ui::dfmwatch *ui;
};
#endif // DFMWATCH_H
