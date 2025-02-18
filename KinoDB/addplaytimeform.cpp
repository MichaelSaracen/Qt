#include "addplaytimeform.h"
#include "ui_addplaytimeform.h"

AddPlayTimeForm::AddPlayTimeForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddPlayTimeForm)
{
    ui->setupUi(this);
}

AddPlayTimeForm::~AddPlayTimeForm()
{
    delete ui;
}

void AddPlayTimeForm::setMovieModel(QSqlTableModel *model)
{
    ui->filmComboBox->setModel(model);
    ui->filmComboBox->setModelColumn(model->fieldIndex("title"));
}

void AddPlayTimeForm::setSaalModel(QSqlTableModel *model)
{
    ui->saalComboBox->setModel(model);
    ui->saalComboBox->setModelColumn(model->fieldIndex("name"));
}

void AddPlayTimeForm::on_pushButton_clicked()
{
    QString date = ui->datumDateEdit->dateTime().toString("dd-MM-yyyy");
    emit playTimeAdded(ui->filmComboBox->currentText(), ui->saalComboBox->currentText(), date);
}

