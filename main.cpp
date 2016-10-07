#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "window.h"
#include <QApplication>

class Page
{

};
using namespace std;

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "rus");
    ifstream book("book.fb2");
    if (!book.is_open())
    {
        cout<<"ошибка при открытии книги"<<endl;
    }
    vector<string> lines;
    while(1) {
        string tp;
        getline(book, tp);
        if(!book) break;
        lines.push_back(move(tp));
    }

    QApplication a(argc, argv);
    Window w;
    w.show();

    return a.exec();
}


