#include "cryptseonodeconfigdialog.h"
#include "ui_cryptseonodeconfigdialog.h"

#include <QModelIndex>

CryptseoNodeConfigDialog::CryptseoNodeConfigDialog(QWidget *parent, QString nodeAddress, QString privkey) :
    QDialog(parent),
    ui(new Ui::CryptseoNodeConfigDialog)
{
    ui->setupUi(this);
    QString desc = "rpcallowip=127.0.0.1<br>rpcuser=REPLACEME<br>rpcpassword=REPLACEME<br>staking=0<br>server=1<br>listen=1<br>port=REPLACEMEWITHYOURPORT<br>masternode=1<br>masternodeaddr=" + nodeAddress + "<br>masternodeprivkey=" + privkey + "<br>";
    ui->detailText->setHtml(desc);
}

CryptseoNodeConfigDialog::~CryptseoNodeConfigDialog()
{
    delete ui;
}
