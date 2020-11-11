#include "dfmwatch.h"
#include "ui_dfmwatch.h"
#include "imagearea.h"
#include <QtWidgets>

DfmWatch::DfmWatch(QWidget *parent)
    : QWidget(parent)
    //, ui(new Ui::dfmwatch)
{
    //ui->setupUi(this);
    imageArea = new ImageArea;
    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(imageArea, 0, 0);
    setLayout(mainLayout);
}

DfmWatch::~DfmWatch()
{
    //delete ui;
}

void DfmWatch::creatActions()
{

}
