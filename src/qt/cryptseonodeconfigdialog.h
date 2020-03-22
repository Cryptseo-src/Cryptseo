#ifndef CSEONODECONFIGDIALOG_H
#define CSEONODECONFIGDIALOG_H

#include <QDialog>

namespace Ui {
    class CryptseoNodeConfigDialog;
}

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

/** Dialog showing transaction details. */
class CryptseoNodeConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CryptseoNodeConfigDialog(QWidget *parent = 0, QString nodeAddress = "123.456.789.123:28666", QString privkey="MASTERNODEPRIVKEY");
    ~CryptseoNodeConfigDialog();

private:
    Ui::CryptseoNodeConfigDialog *ui;
};

#endif // CSEONODECONFIGDIALOG_H
