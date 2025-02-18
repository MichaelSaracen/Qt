#include "widget.h"
#include "ui_widget.h"
#include "data/appdata.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QKeyEvent>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    QString dbPath = AppData::pathToDb();

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(dbPath);

    if( !database.open() ) {
        qDebug() << "Datenbank konnte nicht geöffnet werden";
    } else {
        qDebug() << "Datenbank wurde erfolgreich verbunden!";
    }

    // Models initialsieren
    initModels();

    connect( ui->addCinemaForm, &AddCinemaForm::cinemaAdded, this, &Widget::onCinemaAdded );
    connect( ui->addGenreForm, &AddGenreForm::genreAdded, this, &Widget::onGenreAdded );
    connect( ui->addMovieForm, &AddMovieForm::movieAdded, this, &Widget::onMovieAdded );
    connect( ui->addPlayTimeForm, &AddPlayTimeForm::playTimeAdded, this, &Widget::onPlayTimeAdded);

}

Widget::~Widget()
{
    delete ui;
}

/*!
 * \brief Widget::onCinemaAdded
 * Hinzufügen eines neuen Saals mit Namen und Anzahl der Plätze.
 * \param name
 * \param places
 */
void Widget::onCinemaAdded( const QString &name, int places )
{
    if ( !QSqlDatabase::database().isOpen() ) {
        QMessageBox::critical(this, "Fehler!",
                              "Die Datenbank kann gerade nicht benutzt werden!",
                              QMessageBox::Close);
        return;
    }

    int row = m_saalModel->rowCount();
    m_saalModel->insertRow(row);
    m_saalModel->setData(m_saalModel->index(row, 1), name);
    m_saalModel->setData(m_saalModel->index(row, 2), places);

    if ( !m_saalModel->submitAll() ) {
        QMessageBox::critical(this, "Fehler!",
                              "Saal konnte nicht hinzugefügt werden: \n" + m_saalModel->lastError().text(),
                              QMessageBox::Close);
        return;
    }

    m_saalModel->select();
}

void Widget::onGenreAdded( const QString &name )
{
    if ( !QSqlDatabase::database().isOpen() ) {
        QMessageBox::critical(this, "Fehler!",
                              "Die Datenbank kann gerade nicht benutzt werden!",
                              QMessageBox::Close);
        return;
    }

    int row = m_genreModel->rowCount();
    m_genreModel->insertRow(row);

    m_genreModel->setData(m_genreModel->index(row, 1), name);

    if (!m_genreModel->submitAll()) {
        QMessageBox::critical(this, "Fehler!",
                              "Genre konnte nicht hinzugefügt werden: \n" + m_genreModel->lastError().text(),
                              QMessageBox::Close);
        return;
    }

    m_genreModel->select();
}

/**
 * Slot der die Informationen für den neu hinzugefügten Film entgegennimmt und in die Datenbank speichert!
 * @brief Widget::onMovieAdded
 * @param title
 * @param genre
 * @param fsk
 * @param duration
 * @param description
 */
void Widget::onMovieAdded( const QString &title, const QString &genre, const QString &fsk,
                          const QString &duration, const QString &description )
{
    int genreID =  getID("genreID", "genre", "name", genre); //getGenreID(genre);

    if ( genreID == -1 ) {
         QMessageBox::critical(this, "Fehler!", "Genre existiert nicht!", QMessageBox::Close);
        return;
    }

    int row = m_movieModel->rowCount();
    m_movieModel->insertRow(row);

    m_movieModel->setData(m_movieModel->index(row, 1), title);
    m_movieModel->setData(m_movieModel->index(row, 2), genreID);
    m_movieModel->setData(m_movieModel->index(row, 3), fsk);
    m_movieModel->setData(m_movieModel->index(row, 4), duration);
    m_movieModel->setData(m_movieModel->index(row, 5), description);

    if (!m_movieModel->submitAll()) {
        QMessageBox::critical(this, "Fehler!", "Film konnte nicht gespeichert werden: \n" +
                                                   m_movieModel->lastError().text(), QMessageBox::Close);
        return;
    }

    m_movieModel->select();

    qDebug() << "Film hinzugefügt:" << title << genre <<fsk << duration << description;
}

/**
 * @brief Widget::onPlayTimeAdded
 * @param movie
 * @param room
 * @param date
 */
void Widget::onPlayTimeAdded(const QString &movie, const QString &room, const QString &date)
{
    int movieID = getID("movieID", "movie", "title", movie);
    int saalID = getID("saalID", "saal", "name", room);

    if ( movieID == -1 || saalID == -1 ) {
        QMessageBox::critical(this, "Fehler!", "ID existiert nicht!", QMessageBox::Close);
        return;
    }

    int row = m_playTimeModel->rowCount();
    m_playTimeModel->insertRow(row);

    m_playTimeModel->setData(m_playTimeModel->index(row, 1), date);
    m_playTimeModel->setData(m_playTimeModel->index(row, 2), saalID);
    m_playTimeModel->setData(m_playTimeModel->index(row, 3), movieID);

    if (!m_playTimeModel->submitAll()) {
        QMessageBox::critical(this, "Fehler!", "Film konnte nicht gespeichert werden: \n" +
                                                   m_playTimeModel->lastError().text(), QMessageBox::Close);
        return;
    }

    m_playTimeModel->select();
    selectMoviesWithDate();
}

void Widget::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    Q_UNUSED(topLeft)
    Q_UNUSED(bottomRight)
    Q_UNUSED(roles)
    selectMoviesWithDate();
}

void Widget::deleteSelectedRow(QTableView *tableView, QSqlTableModel *model)
{
    QItemSelectionModel *selectionModel = tableView->selectionModel();
    if ( !selectionModel->hasSelection() ) {
        QMessageBox::warning(this, "Keine Auswahl", "Bitte wählen eine Zeile zum Löschen aus.");
        return;
    }

    QModelIndexList selectedRows = selectionModel->selectedRows();
    if( selectedRows.isEmpty() ) {
        return;
    }

    int row = selectedRows.first().row();

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Löschen bestätigen",
                                                              "Diesen Eintrag wirklich löschen?",
                                                              QMessageBox::Yes | QMessageBox::No);
    if ( reply == QMessageBox::No ) {
        return;
    }

    model->removeRow(row);
    if (!model->submitAll()) {
        QMessageBox::critical(this, "Fehler", "Eintrag konnte nicht gelöscht werden: \n" + model->lastError().text());
        model->revertAll();
    } else {
        model->select();
    }
}



void Widget::selectMoviesWithDate()
{
    QString queryText = R"(
    SELECT movie.title, genre.name, spielzeiten.datum, saal.name
    FROM spielzeiten
    JOIN movie ON movie.movieID = spielzeiten.movieID
    JOIN genre ON movie.genreID = genre.genreID
    JOIN saal ON saal.saalID = spielzeiten.saalID

)";
    m_allModel->setQuery(queryText);
}



/**
 * @brief Widget::getGenreID
 * Gibt über den Namen des Genres die id wieder
 * @param name
 * @return
 */
int Widget::getID(const QString &idName, const QString &table, const QString &helper, const QString &name)
{
    QSqlQuery query;
    QString prepareText = QString("SELECT %1 FROM %2 WHERE %3=:name").arg(idName, table, helper);
    query.prepare(prepareText);
    query.bindValue(":name", name);

    if ( !query.exec() || !query.next() ) {
        qDebug() << "Fehler! ID nicht gefunden!";
        return -1;
    }

    return query.value(0).toInt();
}

/*!
 * \brief Widget::keyPressEvent
 * Zum entferen von Einträgen aus der DB und den Tabellen.
 * \param event
 */
void Widget::keyPressEvent(QKeyEvent *event)
{
    if ( event->key() == Qt::Key_Delete ) {

        QTableView *currentTable = nullptr;
        QSqlTableModel *currentModel = nullptr;

        if ( ui->movieTableView->hasFocus() ) {
            currentTable = ui->movieTableView;
            currentModel = m_movieModel;
        } else if ( ui->genreTableView->hasFocus() ) {
            currentTable = ui->genreTableView;
            currentModel = m_genreModel;
        } else if ( ui->saalTableView->hasFocus() ) {
            currentTable = ui->saalTableView;
            currentModel = m_saalModel;
        } else if ( ui->playTableView->hasFocus() ) {
            currentTable = ui->playTableView;
            currentModel = m_playTimeModel;
        }

        if ( currentTable && currentModel ) {
            deleteSelectedRow(currentTable, currentModel);
            currentTable = nullptr;
            currentModel = nullptr;
        }
    }

    QWidget::keyPressEvent(event);
}


/**
 * Initialisiert die Model und wählt deren Tabellen.
 * @brief Widget::initModels
 */

void Widget::initModels()
{
    m_genreModel = new QSqlTableModel(this);
    m_genreModel->setTable("genre");
    m_genreModel->select();


    ui->genreTableView->setModel(m_genreModel);


    m_saalModel = new QSqlTableModel(this);
    m_saalModel->setTable("saal");
    m_saalModel->select();
    ui->addPlayTimeForm->setSaalModel(m_saalModel);

    ui->saalTableView->setModel(m_saalModel);


    m_movieModel = new QSqlTableModel(this);
    m_movieModel->setTable("movie");
    m_movieModel->select();
    ui->addPlayTimeForm->setMovieModel(m_movieModel);
    ui->addMovieForm->setGenreModel(m_genreModel);

    ui->movieTableView->setModel(m_movieModel);

    m_playTimeModel = new QSqlTableModel(this);
    m_playTimeModel->setTable("spielzeiten");
    m_playTimeModel->select();

    ui->playTableView->setModel(m_playTimeModel);



    m_allModel = new QSqlTableModel(this);

    selectMoviesWithDate();
    ui->tableViewAll->setModel(m_allModel);

    ui->genreTableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    ui->saalTableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    ui->movieTableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    ui->playTableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    ui->movieTableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    ui->tableViewAll->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(m_genreModel, &QSqlTableModel::dataChanged, this, &Widget::onDataChanged);
    connect(m_saalModel, &QSqlTableModel::dataChanged, this, &Widget::onDataChanged);
    connect(m_movieModel, &QSqlTableModel::dataChanged, this, &Widget::onDataChanged);
    connect(m_playTimeModel, &QSqlTableModel::dataChanged, this, &Widget::onDataChanged);

}























