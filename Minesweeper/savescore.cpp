/**
 * SaveScore
 * Saves the winner's score and name
 * @author Stephen Liang
 * @author Aisha Halim
 * Created for CS 340 Fall 2010 - Professor Troy
 */

#include "savescore.h"
#include "ui_savescore.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

/**
  * Constructor
  * Opens a database connection and connects the ok button to save a score
  */
SaveScore::SaveScore(int score, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveScore)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    this->score = score;
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(save()));

    //Database information
    QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE", "connection");

    db.setDatabaseName("scores.sqlite");

    if ( !db.open() )
    {
        qFatal("Failed to connect to database");
    }
}

/**
  * Destructor
  */
SaveScore::~SaveScore()
{
    delete ui;
    QSqlDatabase::database("connection").close();
    QSqlDatabase::removeDatabase("connection");
}

void SaveScore::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/**
  * save()
  * Saves the score and the name
  */
void SaveScore::save()
{
    //Grab the name and check the input
    QString name = ui->name->text();
    name = name.trimmed ();
    if ( name.compare("") == 0)
    {
        QMessageBox error;
        error.setText ("You must enter a name");
        error.exec ();
        return;
    }

    //Insert the name and score into the sqlite database
    QSqlDatabase db = QSqlDatabase::database("connection");
    QSqlQuery sql(db);

    //Ensure that the table exists!
    sql.prepare("CREATE  TABLE  IF NOT EXISTS \"scores\" (\"id\" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , \"score\" INTEGER, \"name\" TEXT)");

    if ( !sql.exec())
    {
        qFatal("Unable to create table");
    }

    sql.prepare("INSERT INTO scores (name, score) VALUES (:name, :score)");

    //Ensure sanitary input
    sql.bindValue(":name", ui->name->text());
    sql.bindValue(":score", this->score);

    if ( !sql.exec( ) )
    {
        qFatal("Unable to insert to database");
    }

    this->close();
}
