/*#include "lose.h"
#include "ui_lose.h"

Lose::Lose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Lose)
{
    ui->setupUi(this);
    QMovie *movie = new QMovie(":/images/you_lose.gif");

    QLabel *processLabel = new QLabel(this);
    ui->gridLayout->addWidget(processLabel);
    processLabel->setMovie(movie);
    movie->start();
}

Lose::~Lose()
{
    delete ui;
}

*/
