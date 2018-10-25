#include "win.h"
#include "ui_win.h"

Win::Win(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Win)
{
    ui->setupUi(this);
    QMovie *movie = new QMovie(":/images/you_win.gif");

    QLabel *processLabel = new QLabel(this);
    ui->gridLayout->addWidget(processLabel);
    processLabel->setMovie(movie);
    movie->start();
}

Win::~Win()
{
    delete ui;
}
