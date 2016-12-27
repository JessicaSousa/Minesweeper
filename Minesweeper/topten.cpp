/**
 * TopTen
 * Displays information about the top ten players
 * @author Stephen Liang
 * @author Aisha Halim
 * Created for CS 340 Fall 2010 - Professor Troy
 */
#include "topten.h"
#include "ui_topten.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

/**
  * Constructor
  * Sets up database connection and connects the reset button to clear all the information
  */
TopTen::TopTen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TopTen)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);

    connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetScores()));

    //Database info
    QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE", "connection");

    db.setDatabaseName("scores.sqlite");

    if ( !db.open() )
    {
        qFatal("Failed to connect to database");
    }

    getTopTenScores();
}

/**
  * resetScores()
  * Erases the score table
  */
void TopTen::resetScores()
{
    QSqlDatabase db = QSqlDatabase::database("connection");

    QSqlQuery sql(db);

    //Clear the table
    sql.prepare("DELETE FROM scores");

    if ( !sql.exec() )
    {
        qFatal("Unable to truncate table from database");
    }

    getTopTenScores(); //Refresh view
}

/**
  * getTopTenScores()
  * Gets the top ten scores from the database and ensures that the scores table already exists
  */
void TopTen::getTopTenScores()
{
    QString topScores = "<ol>";

    QSqlDatabase db = QSqlDatabase::database("connection");

    QSqlQuery sql(db);

    //Ensure that the table exists!
    sql.prepare("CREATE  TABLE  IF NOT EXISTS \"scores\" (\"id\" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , \"score\" INTEGER, \"name\" TEXT)");

    if ( !sql.exec())
    {
        QString error = sql.lastError().text();
    }

    //Grab the top ten players
    sql.prepare("SELECT name, score FROM scores ORDER BY score ASC LIMIT 10");

    if ( !sql.exec() )
    {
        qFatal("Unable to select from database");
    }

    QSqlRecord record = sql.record();

    //Create a list of those top ten players
    for( int r=0; sql.next(); r++ )
              topScores += "<li>" + sql.value(0).toString() + " - " + sql.value(1).toString() +"</li>";

    topScores += "</ol>";

    //Set the list for the user to see
    ui->topTenList->setText(topScores);
}

/**
  * Destructor
  */
TopTen::~TopTen()
{
    QSqlDatabase::database("connection").close();
    QSqlDatabase::removeDatabase("connection");
    delete ui;
}

void TopTen::changeEvent(QEvent *e)
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
