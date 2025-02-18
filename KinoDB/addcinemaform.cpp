#include "addcinemaform.h"
#include "ui_addcinemaform.h"

AddCinemaForm::AddCinemaForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddCinemaForm)
{
    ui->setupUi(this);
    ui->btnSave->setEnabled(false);


}

AddCinemaForm::~AddCinemaForm()
{
    delete ui;
}

/*!
 * \brief AddGenreForm::on_saalLineEdit_textChanged
 * Schaltet den Button frei, sobal die Textlänge größer als ein Charakter ist.
 * \param arg1
 */
void AddCinemaForm::on_saalLineEdit_textChanged(const QString &arg1) { ui->btnSave->setEnabled(arg1.length() > 1); }


/*!
 * Beim Drücken des Buttons, wird das Signal losgeschickt. (Empfänger ist Widget)
 * \brief AddGenreForm::on_btnSave_clicked
 */
void AddCinemaForm::on_btnSave_clicked()
{
    emit cinemaAdded(ui->saalLineEdit->text(), ui->sitzplTzeSpinBox->value());
    ui->saalLineEdit->clear();
}

