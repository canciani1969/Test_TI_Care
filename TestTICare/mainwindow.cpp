#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timerId = startTimer(10000);

    ReadJSon();
}

MainWindow::~MainWindow()
{
    killTimer(timerId);
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    qDebug() << "Timer ID:" << event->timerId();
    ui->label->setText("Read");
    ReadJSon();
}

void MainWindow::ReadJSon()
{
    // create custom temporary event loop on stack
     QEventLoop eventLoop;

     // "quit()" the event-loop, when the network request "finished()"
     QNetworkAccessManager mgr;
     QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

     // the HTTP request
     QNetworkRequest req( QUrl( QString("https://www.bitstamp.net/api/v2/ticker/btceur/") ) );
     QNetworkReply *reply = mgr.get(req);
     eventLoop.exec(); // blocks stack until "finished()" has been called

     if (reply->error() == QNetworkReply::NoError) {

         QString strReply = (QString)reply->readAll();

         // parse json
         QJsonDocument document = QJsonDocument::fromJson(strReply.toUtf8());
         QJsonObject data = document.object();

//       QString call = data["last"].toString();

         ui->label_4->setText(data["high"].toString());
         ui->label_5->setText(data["last"].toString());

         delete reply;
     }
     else
     {
         //failure
         ui->label_4->setText("");
         ui->label_5->setText("");

         qDebug() << "Failure" <<reply->errorString();
         delete reply;
     }

     ui->label->setText("");

}

