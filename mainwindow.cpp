#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QtCore>
#include <QtGui>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->action_Open, SIGNAL(triggered(bool)), this, SLOT(slotOpenFile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTitle()
{
    QString lTitle = QString( "FB2Reader-%1[*]").arg(fileName);
    setWindowTitle(lTitle);
}

void MainWindow::addElementDataToMap(QXmlStreamReader& xml, QMap<QString, QString>& map)
{
    if (xml.tokenType() != QXmlStreamReader::StartElement)
        return;
    QString elementName = xml.name().toString();
    xml.readNext();
    map.insert(elementName, xml.text().toString());
}


void MainWindow::slotOpenFile()
{


    QString lFileName = QFileDialog::getOpenFileName(this, "Openfile...", QDir::homePath(),"FB2-files(*.fb2)");
    if(lFileName.isEmpty())
        return ;

    QFile lFile(lFileName);
    if(lFile.open(QIODevice::ReadOnly|QIODevice::Text ))
    {
        fileName = lFileName;

        QXmlStreamReader xr(&lFile);

        //ui->textEdit->setText(lFile.readAll());
        //ui->textEdit->set(lFile.readAll());
        updateTitle();

        QString data;
        QMap <QString, QString> tagP;
        bool body = false;

        bool format = false;

        bool right = false;

        bool title = false;
        bool bold = false;
        bool italic = false;
        bool sub = false;
        bool sup = false;
        QString contLine;
         ui->textEdit->insertHtml("<br>");

        while((!xr.atEnd())&&(!xr.hasError()))
        {

            xr.readNext();
            //ui->textEdit->insertPlainText(xr.text().toString()+"\n");
            QString tName = xr.name().toString();
            if(xr.tokenType() == QXmlStreamReader::StartElement)
            {
                //ui->textEdit->insertPlainText(xr.name().toString()+"\n");
                if(body) {
                    if(tName == "title")
                        title = true;
                    if(tName == "empty-line/")
                         ui->textEdit->insertHtml("<br>");
                    if(tName == "emphasis")
                        italic = true;
                    if(tName == "strong")
                        bold = true;
                    if(tName == "sub")
                        sub = true;
                    if(tName == "sup")
                        sup = true;
                    if(tName == "p")
                        format = true;
                    if(tName == "epigraph")
                        right = true;
                    if(tName == "text-author") {
                        format = true;
                        bold = true;
                    }
                }
                else {
                    if(tName == "body")
                        body = true;
                }
            }
            else if(xr.tokenType() == QXmlStreamReader::Characters) {
                if(body && format) {
                    QString line = xr.text().toString();
                    if(title)
                        contLine += line + " ";
                    //std::cout << right << " " << title << std::endl;

                    if(right)
                        ui->textEdit->setAlignment(Qt::AlignRight);
                    if(title) {
                        ui->textEdit->setAlignment(Qt::AlignCenter);
                        line = "<h1>" + line + "</h1>";
                    }

                    if(bold)
                        line = "<b>" + line + "</b>";
                    if(italic)
                        line = "<i>" + line + "</i>";
                    if(sub)
                        line = "<sub>" + line + "</sub>";
                    if(sup)
                        line = "<sup>" + line + "</sup>";

                    ui->textEdit->insertHtml("<p>"+line+"</p><br>");
                    //ui->textEdit->insertPlainText("\n");
                    if(right||title)
                        ui->textEdit->setAlignment(Qt::AlignLeft);
                    //ui->textEdit->insertPlainText(line+"<br>\n");
                }
            }
            else if(xr.tokenType() == QXmlStreamReader::EndElement) {
                if(body) {
                    if(tName == "title") {
                        QListWidgetItem* wi = new QListWidgetItem(contLine);
                        ui->listWidget->addItem(wi);
                        title = false;
                        contLine = "";
                    }
                    if(tName == "emphasis")
                        italic = false;
                    if(tName == "strong")
                        bold = false;
                    if(tName == "sub")
                        sub = false;
                    if(tName == "sup")
                        sup = false;
                    if(tName == "p")
                        format = false;
                    if(tName == "epigraph")
                        right = false;
                    if(tName == "body")
                        body = false;
                    if(tName == "text-author") {
                        format = false;
                        bold = false;
                    }
                }
            }
        }
    /*
        while(QXmlStreamReader::EndElement != xr.tokenType())
        {
            if(QXmlStreamReader::StartElement == xr.tokenType())
                addElementDataToMap(xr, tagP);
            xr.readNext();
        }
    */




        lFile.close();
    }
    else
    {
        QMessageBox::warning(this,"Error",QString("Can't open file %1 for reading").arg(lFile.fileName()),QMessageBox::Ok) ;
    }


}



