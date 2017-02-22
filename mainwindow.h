#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString fileName;
    QVector<QString> pages;
    void updateTitle();
    void addElementDataToMap(QXmlStreamReader& xml, QMap<QString, QString>& map);


private slots:
    void slotOpenFile();
};

#endif // MAINWINDOW_H
