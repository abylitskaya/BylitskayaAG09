#include "widget.h"
#include <dos.h>
#include <QtWidgets>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    greetings();
    QMenuBar *menuBar = new QMenuBar();
    menuBar->setStyleSheet("QMenuBar {background-color:Black; color:White;}"
                           "QMenuBar::item {padding: 5px; background-color:Black;}"
                           "QMenuBar::item:selected {background-color: White; color:Black}");
    QMenu *menu = new QMenu("Menu");
    menu->setStyleSheet("QMenu {background-color:Black; color:White; border-width:0px;}"
                        "QMenu::item {padding: 5px;}"
                        "QMenu::item:selected {background-color: White; color:Black}");
    menuBar->addMenu(menu);
    menu->addAction("New Game",this,SLOT(newGame()),Qt::CTRL + Qt::Key_N);
    menu->addSeparator();
    menu->addAction("About", this, SLOT(aboutMenu()));
    menu->addSeparator();
    menu->addAction("Exit",this,SLOT(close()),Qt::CTRL + Qt::Key_Q);
    menuBar->addMenu(menu);
    menuBar->addAction("Help",this,SLOT(help()));
    QVBoxLayout *layout1 = new QVBoxLayout;
    this->setLayout(layout1);
    layout1->setMenuBar(menuBar);
    rows=0;
    columns=0;
    FirstGeneration=0;
    count1=0;
    count2=-1;
    flag=false;
}
/////////////////////////////////////////////////
//                                             //
//   Прошу не судить слишком строго,           //
//  всё делалось этой ночью, так так           //
//  в остальное время я лежала в больнице :(   //
//                                             //
/////////////////////////////////////////////////
Widget::~Widget()
{

}
void Widget::greetings()
{
     QMessageBox::information(this,"Greetings!","\tHello!\n"
                                                "Welcome to The Game of Life!");
}
int Widget::countOfCells()
{
//    Количество живых клеток
    int count = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (FirstGeneration[i][j] == '*')
            {
                count++;
            }
        }
    }
    return count;
}
int Widget::countOfAlifeCells(int x, int y)
{
//     Количество живых соседей у клетки с координатами x, y
    int count = 0;
        for (int i = x - 1; i <= x + 1; i++)
        {
            for (int j = y - 1; j <= y + 1; j++)
            {
                if (i < 0 || j < 0 || i >= columns || j >= rows || (i == x && j == y))
                    continue;
                if (NextGeneration[i][j] == '*')
                    count++;
            }
        }
        return count;
}
int Widget::difference()
{
//    Сравнение миров текуцего и предыдущего поколения
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (NextGeneration[i][j] != FirstGeneration[i][j])
            {
                return -1;
            }
        }
    }
    return 0;
}
void Widget::drowWorld()
{
//    Рисование мира
    table->clear();
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            QTableWidgetItem *item = new QTableWidgetItem();
            table->setItem(i, j, item);
            item->setText(QString(NextGeneration[i][j]));
            item->setTextAlignment(Qt::AlignCenter);
        }
    }
}
void Widget::processing()
{
//    QThread
    do {
    //    Рисование мира
        drowWorld();
    //    Копирование мира
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < columns; j++)
                NextGeneration[i][j] = FirstGeneration[i][j];
    //     Сгенерировать следующее поколение мира
        char p;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                p = FirstGeneration[i][j];
                count1 = countOfAlifeCells(i,j);

                if (p == '*') {
                    if (count1 == 2 || count1 == 3) NextGeneration[i][j] = '*';
                    else NextGeneration[i][j] = '0';
                }
                else {
                    if (count1 == 3) NextGeneration[i][j] = '*';
                    else NextGeneration[i][j] = '0';
                }
            }
        }
        flag = (difference() == 0?true:false);
        count2 = countOfCells();
        if (flag) {
            sleep(1000);
            emit message1();
            sleep(10000);
        }
        if (count2 == 0) {
            sleep(1000);
            emit message2();
            sleep(10000);
        }
        sleep(1000);
    } while (count2 != 0 && !flag);
    connect(this,SIGNAL(message1()),this,SLOT(getMessage1()));
    connect(this,SIGNAL(message2()),this,SLOT(getMessage2()));
}

void Widget::newGame()
{
    if (table) layout()->removeWidget(table);//но это как-то не так работает, как я хочу
    if (next) layout()->removeWidget(next);
    getStarted();
    //processing();
    NextGeneration = new char*[rows];
    for (int i = 0; i < rows; i++)
        NextGeneration[i] = new char[columns];
    connect(next,SIGNAL(clicked()),this,SLOT(processing()));

}

void Widget::aboutMenu()
{
    QMessageBox::about(this, "About The Game of Life",
                             "\tThe Game of Life, also known simply as Life, is a cellular automaton devised by the British mathematician John Horton Conway in 1970.\n"
                             "The 'game' is a zero-player game, meaning that its evolution is determined by its initial state, requiring no further input.\n"
                             "\n\t\t\t\tRules\n"
                             "\tThe universe of the Game of Life is an infinite two-dimensional orthogonal grid of square cells, each of which is in one of two possible states, alive or dead.\n"
                             "Every cell interacts with its eight neighbours, which are the cells that are horizontally, vertically, or diagonally adjacent.\n"
                             "\nAt each step in time, the following transitions occur:\n"
                             "1. Any live cell with fewer than two live neighbours dies, as if caused by under-population.\n"
                             "2. Any live cell with two or three live neighbours lives on to the next generation.\n"
                             "3. Any live cell with more than three live neighbours dies, as if by overcrowding.\n"
                             "4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.");
}
void Widget::help()
{
    QMessageBox::information(this, "Help","Press 'Menu' -> 'New Game' and let's play!");
}

void Widget::getStarted()
{
    QInputDialog *inputR = new QInputDialog;
    int r = inputR->getInt(this, tr("Rows"),tr("Enter number of rows:"),1,1,10,1);
    connect(inputR,SIGNAL(intValueChanged(int)),this,SLOT(setRows(int)));
    QInputDialog *inputC = new QInputDialog;
    int c = inputC->getInt(this, tr("Columns"),tr("Enter number of columns:"),1,1,10,1);
    connect(inputC,SIGNAL(intValueChanged(int)),this,SLOT(setColumns(int)));

    label = new QLabel("Set alife and dead cells. (0 - dead, * - alife)");
    this->layout()->addWidget(label);
    next = new QPushButton("Next Steps of Evolution");
    this->layout()->addWidget(next);

    table = new QTableWidget();
    table->setRowCount(r);
    table->setColumnCount(c);
    table->horizontalHeader()->setDefaultSectionSize(30);
    table->verticalHeader()->setDefaultSectionSize(30);
    table->verticalHeader()->hide();
    table->horizontalHeader()->hide();
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->layout()->addWidget(table);
    FirstGeneration = new char*[rows];
    for (int i = 0; i < r; i++)
    {
        FirstGeneration[i] = new char[columns];
        for (int j = 0; j < c; j++)
        {
            QTableWidgetItem *item = new QTableWidgetItem();
            table->setItem(i, j, item);
            item->setText("0");
            item->setTextAlignment(Qt::AlignCenter);
            FirstGeneration[i][j] = '0';
        }
    }
    connect(table,SIGNAL(cellClicked(int,int)),this,SLOT(setItem(int,int)));
    connect(table,SIGNAL(cellClicked(int,int)),this,SLOT(setElement(int,int)));


}
void Widget::setRows(int r)
{
    rows=r;
}
void Widget::setColumns(int c)
{
    columns=c;
}
void Widget::setElement(int i, int j)
{
    if (FirstGeneration[i][j]=='0')
    {
        FirstGeneration[i][j]='*';
    }
    else
    {
        FirstGeneration[i][j]='0';
    }
}
void Widget::setItem(int i, int j)
{
    if (FirstGeneration[i][j]=='0')
    {
        table->item(i,j)->setText("*");
    }
    else
    {
        table->item(i,j)->setText("0");
    }
}
void Widget::getMessage1()
{
    QMessageBox::information(this,"","Optimal change of generations.");
}
void Widget::getMessage2()
{
    QMessageBox::information(this,"","All cells are died. :(.");
}
