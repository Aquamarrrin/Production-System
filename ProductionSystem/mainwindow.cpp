#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    this->setMinimumSize(300,300);
    layoutV = new QVBoxLayout();

    btnLoad=new QPushButton("Загрузить");
    QObject::connect(btnLoad,SIGNAL(clicked()),this,SLOT(loadParser()));
    btnAnalyze = new QPushButton("Найти решение");

    lineFilePath = new QLineEdit();

    te=new QTextEdit();

    //Добавление виджетов в лэйауты
    layoutV->addWidget(lineFilePath);
    layoutV->addWidget(btnLoad);
    //layoutV->addWidget(btnAnalyze);
    layoutV->addWidget(te);

    this->setLayout(layoutV);
}

void MainWindow::loadParser()
{
    if(lineFilePath->isModified() && lineFilePath->text()!="")
    {
        te->clear();
        Parser* pars =new Parser(lineFilePath->text());
        for(int i=1;i<=pars->getNewObjects().size();i++)
        {
            te->textCursor().insertText(pars->getNewObjects()[i]+"\n");
        }
    }
}

MainWindow::~MainWindow()
{

}
