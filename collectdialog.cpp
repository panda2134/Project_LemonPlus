#include "collectdialog.h"
#include "ui_collectdialog.h"

CollectDialog::CollectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CollectDialog)
{
    ui->setupUi(this);
}

CollectDialog::~CollectDialog()
{
    delete ui;
}

void CollectDialog::setContest(Contest *value)
{
    disconnect(this, 0, 0, 0);

    contest = value;
    ui->progressBar->setMaximum(contest->server->onlineCount());

    connect(contest->server, SIGNAL(showStartCollecting(QString)), this, SLOT(showStartCollecting(QString)));
    connect(contest->server, SIGNAL(showFinishCollecting(QString)), this, SLOT(showFinishCollecting(QString)));
}

void CollectDialog::showStartCollecting(const QString & name)
{
    ui->stateLabel->setText(tr("Now collecting:"));
    ui->contestantNameLabel->setText(name);
}

void CollectDialog::showFinishCollecting(const QString & name)
{
    ui->stateLabel->setText(tr("Finished collecting:"));
    ui->contestantNameLabel->setText(name);

    ui->progressBar->setValue(ui->progressBar->value() + 1);
//    if(ui->progressBar->maximum() == ui->progressBar->value())
//        accept();
}
