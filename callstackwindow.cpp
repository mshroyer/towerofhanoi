#include "callstackwindow.h"
#include "ui_callstackwindow.h"

CallStackWindow::CallStackWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CallStackWindow)
{
    ui->setupUi(this);
}

CallStackWindow::~CallStackWindow()
{
    delete ui;
}
