#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenuBar>
#include <QAction>
#include<QMessageBox>
#include <QFile>
#include <QTextStream>
#include<QDir>
#include "dialog.h"
#include<QXmlStreamAttribute>
#include<QXmlStreamReader>
#include<QByteArray>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusBar->showMessage("Ready");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    close();

}

void MainWindow::on_actionOpen_2_triggered()
{
    Dialog *wnd = new Dialog(this);
    wnd->show();
    connect(wnd, SIGNAL(FilePath(QString)), this, SLOT(ReadFile(QString)));

}

void MainWindow::ReadFile(const QString &pathStr)
{
    QFile mFile(pathStr);
    QString data;

    if(!mFile.open(QFile::ReadOnly))
    {
        QMessageBox::information(this, "Error", "Wrong path.");
        ui->statusBar->showMessage("Error");
        return;
    }
    else
    {
        QXmlStreamReader stream;
        stream.setDevice(&mFile);

        while(!stream.atEnd())
            {
                QXmlStreamReader::TokenType token = stream.readNext();
                if(token == QXmlStreamReader::StartElement) {
                    if(stream.name() == "Name") {
                        stream.readNext();
                    }
                }
                data.append(stream.text().toString());
            }

    }

    ui->textBrowser->setText(data);

    ui->statusBar->showMessage("Reading complete:" + pathStr);


   /*QTextStream stream (&mFile);


    QString buff = stream.readAll();

    ui->textBrowser->setText(buff);*/
    mFile.flush();
    mFile.close();

}
