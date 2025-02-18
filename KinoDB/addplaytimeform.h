#ifndef ADDPLAYTIMEFORM_H
#define ADDPLAYTIMEFORM_H

#include <QSqlTableModel>
#include <QWidget>

namespace Ui {
class AddPlayTimeForm;
}

class AddPlayTimeForm : public QWidget
{
    Q_OBJECT

public:
    explicit AddPlayTimeForm(QWidget *parent = nullptr);
    ~AddPlayTimeForm();
    void setMovieModel(QSqlTableModel *model);
    void setSaalModel(QSqlTableModel *model);

private slots:
    void on_pushButton_clicked();

signals:
    void playTimeAdded(const QString& movie, const QString& room, const QString& date);


private:
    Ui::AddPlayTimeForm *ui;
};

#endif // ADDPLAYTIMEFORM_H
