#include "addmovieform.h"
#include "ui_addmovieform.h"
#include "data/appdata.h"

AddMovieForm::AddMovieForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddMovieForm)
{
    ui->setupUi(this);
    ui->btnAddMovie->setEnabled(false);
    //ui->genreComboBox->addItems(AppData::genres);
    ui->fSKComboBox->addItems(AppData::fsk);

    // [!] Freischaltungen des Buttons zum Senden des Signals prüfen ---
    connect(ui->textEditDescription, &QTextEdit::textChanged, this, [=]() {
        ui->btnAddMovie->setEnabled( ui->textEditDescription->toPlainText().length() > 0 && ui->titelLineEdit->text().length() > 0 );});
    connect(ui->titelLineEdit, &QLineEdit::textChanged, this, [=](const QString &text) {
        ui->btnAddMovie->setEnabled(text.length() > 0 && ui->textEditDescription->toPlainText().length() > 0 );
    });
}

AddMovieForm::~AddMovieForm() { delete ui; }


/**
 * Sendet die Informationen über den Film an das Widget.
 * @brief AddMovieForm::on_btnAddMovie_clicked
 */
void AddMovieForm::on_btnAddMovie_clicked()
{
    qDebug() << ui->dauerTimeEdit->time().toString("HH:mm");

    emit movieAdded( ui->titelLineEdit->text(), ui->genreComboBox->currentText(),
                     ui->fSKComboBox->currentText(), ui->dauerTimeEdit->time().toString("HH:mm"),
                     ui->textEditDescription->toPlainText() );
    ui->titelLineEdit->clear();
    ui->textEditDescription->clear();
    //void movieAdded(const QString &title, const QString &genre, const QString &fsk, const QString &duration, const QString &description);
}

void AddMovieForm::setGenreModel(QSqlTableModel *newGenreModel)
{
    m_genreModel = newGenreModel;
    ui->genreComboBox->setModel(m_genreModel);
    ui->genreComboBox->setModelColumn(m_genreModel->fieldIndex("name"));
}



