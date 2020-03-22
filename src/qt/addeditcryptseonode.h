#ifndef ADDEDITCSEONODE_H
#define ADDEDITCSEONODE_H

#include <QDialog>

namespace Ui {
class AddEditCryptseoNode;
}


class AddEditCryptseoNode : public QDialog
{
    Q_OBJECT

public:
    explicit AddEditCryptseoNode(QWidget *parent = 0);
    ~AddEditCryptseoNode();

protected:

private Q_SLOTS:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

signals:

private:
    Ui::AddEditCryptseoNode *ui;
};

#endif // ADDEDITCSEONODE_H
