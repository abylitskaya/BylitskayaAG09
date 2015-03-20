#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtWidgets>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
private:
    int rows;
    int columns;
    char **FirstGeneration;
    char **NextGeneration;
    QTableWidget *table;
    QLabel *label;
    QPushButton *next;
    int count1, count2;
    bool flag;
    int countOfCells();
    int countOfAlifeCells(int x, int y);
    int difference();
    void drowWorld();
    static void sleep(unsigned long secs){QThread::sleep(secs);}
private slots:
    void greetings();
    void newGame();
    void aboutMenu();
    void help();
    void getStarted();
    void setRows(int r);
    void setColumns(int c);
    void setItem(int i, int j);
    void setElement(int i, int j);
    void processing();
    void getMessage1();
    void getMessage2();
signals:
    void message1();
    void message2();
};

#endif // WIDGET_H
