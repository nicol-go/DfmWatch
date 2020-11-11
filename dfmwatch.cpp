#include "dfmwatch.h"
#include "ui_dfmwatch.h"

dfmwatch::dfmwatch(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::dfmwatch)
{
    ui->setupUi(this);
}

dfmwatch::~dfmwatch()
{
    delete ui;
}

