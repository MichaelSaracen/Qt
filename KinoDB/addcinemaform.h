#ifndef ADDCINEMAFORM_H
#define ADDCINEMAFORM_H

#include <QWidget>

namespace Ui {
class AddCinemaForm;
}

class AddCinemaForm : public QWidget
{
    Q_OBJECT

public:
    explicit AddCinemaForm(QWidget *parent = nullptr);
    ~AddCinemaForm();

private slots:
    void on_saalLineEdit_textChanged(const QString &arg1);
    void on_btnSave_clicked();

signals:
    void cinemaAdded(const QString& name, int places);
private:
    Ui::AddCinemaForm *ui;
};

#endif // ADDCINEMAFORM_H
