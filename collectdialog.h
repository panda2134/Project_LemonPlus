#ifndef COLLECTDIALOG_H
#define COLLECTDIALOG_H

#include "contest.h"
#include "contestserver.h"

#include <QDialog>

namespace Ui {
class CollectDialog;
}

class CollectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CollectDialog(QWidget *parent = 0);
    ~CollectDialog();

    void setContest(Contest *value);

private:
    Ui::CollectDialog *ui;
    Contest *contest;
private slots:
    void showStartCollecting(const QString &);
    void showFinishCollecting(const QString &);
};

#endif // COLLECTDIALOG_H
