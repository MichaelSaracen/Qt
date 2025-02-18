#ifndef WIDGET_H
#define WIDGET_H

#include <QSqlTableModel>
#include <QTableView>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void onCinemaAdded( const QString &name, int places );
    void onGenreAdded( const QString &name );
    void onMovieAdded(const QString &title, const QString &genre, const QString &fsk, const QString &duration, const QString &description);
    void onPlayTimeAdded(const QString& movie, const QString& room, const QString& date);
    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
    void deleteSelectedRow(QTableView *tableView, QSqlTableModel *model);



private:
    void initModels();
    void selectMoviesWithDate();
    int getID(const QString &idName,const QString &table, const QString& helper, const QString &name);
private:
    Ui::Widget *ui;
    QSqlTableModel *m_genreModel;
    QSqlTableModel *m_saalModel;
    QSqlTableModel *m_movieModel;
    QSqlTableModel *m_playTimeModel;
    QSqlTableModel *m_allModel;

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event);
};
#endif // WIDGET_H
