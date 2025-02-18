#include "addgenreform.h"
#include "ui_addgenreform.h"

AddGenreForm::AddGenreForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddGenreForm)
{
    ui->setupUi(this);
    ui->btnSaveGenre->setEnabled(false); // Anfangs auf Disabled ( Button zum Speichern )
}

AddGenreForm::~AddGenreForm() { delete ui; }

/*!
 * \brief AddGenreForm::on_genreLineEdit_textChanged
 * Schaltet den Button frei, sobal die Textlänge größer als ein Charakter ist.
 * \param arg1
 */
void AddGenreForm::on_genreLineEdit_textChanged(const QString &arg1) { ui->btnSaveGenre->setEnabled(arg1.length() > 1); }

/*!
 * Beim Drücken des Buttons, wird das Signal losgeschickt, mit dem Namen der Genre. (Empfänger ist
 * Widget)
 * \brief AddGenreForm::on_btnSaveGenre_clicked
 */
void AddGenreForm::on_btnSaveGenre_clicked() { emit genreAdded(ui->genreLineEdit->text()); ui->genreLineEdit->clear(); }

