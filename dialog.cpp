#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    // it's already a central widget, but who cares
    //this->setCentralWidget(ui->tableView);
    // connect to db
    db = QSqlDatabase::addDatabase("QODBC");
    db.setConnectOptions();
    QString dsn = QString("Driver={SQL Server Native Client 11.0};Server=(localdb)\\Projects;Database=test");
    db.setUserName("odbc");
    db.setPassword("odbc");
    db.setDatabaseName(dsn);

    if (!db.open()) qDebug() << db.lastError().text();
    // create a table model
    model = new QSqlRelationalTableModel(this);
    model->setTable("dbo.people");
    // the table PLACE has a foreign key to the table PEOPLE
    // the primary key in PEOPLE (column# 0) corresponds to the foreign key PERSON_ID in PLACE
    // from PLACE were selecting PLACENAME
    model->setRelation(0, QSqlRelation("dbo.place", "Person_ID", "PlaceName"));
    // select data from the table, and show
    if (!model->select()) qDebug() << model->lastError().text();
    ui->tableView->setModel(model);
}

Dialog::~Dialog()
{
    db.close();
    delete ui;
}
