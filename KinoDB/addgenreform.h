#ifndef ADDGENREFORM_H
#define ADDGENREFORM_H

#include <QWidget>

namespace Ui {
class AddGenreForm;
}

class AddGenreForm : public QWidget
{
    Q_OBJECT

public:
    explicit AddGenreForm(QWidget *parent = nullptr);
    ~AddGenreForm();

private slots:
    void on_genreLineEdit_textChanged(const QString &arg1);
    void on_btnSaveGenre_clicked();

signals:
    void genreAdded(const QString &name);

private:
    Ui::AddGenreForm *ui;
};

#endif // ADDGENREFORM_H
