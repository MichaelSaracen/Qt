#ifndef ADDMOVIEFORM_H
#define ADDMOVIEFORM_H

#include <QSqlTableModel>
#include <QWidget>

namespace Ui {
class AddMovieForm;
}

class AddMovieForm : public QWidget
{
    Q_OBJECT

public:
    explicit AddMovieForm(QWidget *parent = nullptr);
    ~AddMovieForm();




    void setGenreModel(QSqlTableModel *newGenreModel);

private slots:
    void on_btnAddMovie_clicked();

signals:
    void movieAdded(const QString &title, const QString &genre, const QString &fsk, const QString &duration, const QString &description);

private:
    Ui::AddMovieForm *ui;
    QSqlTableModel *m_genreModel;
};

#endif // ADDMOVIEFORM_H
