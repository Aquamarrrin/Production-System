#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    this->setMinimumSize(500,500);
    layoutV = new QVBoxLayout();
    layoutH = new QHBoxLayout();

    btnLoad=new QPushButton("Загрузить");
    QObject::connect(btnLoad,SIGNAL(clicked()),this,SLOT(loadParser()));
    btnAnalyze = new QPushButton("Найти решение");

    lineFilePath = new QLineEdit();

    teObj=new QTextEdit();
    teRel=new QTextEdit();
    teRes=new QTextEdit();

    //Добавление виджетов в лэйауты
    layoutV->addWidget(lineFilePath);
    layoutV->addWidget(btnLoad);
    //layoutV->addWidget(btnAnalyze);
    layoutH->addWidget(teObj);
    layoutH->addWidget(teRel);
    layoutH->addWidget(teRes);
    layoutV->addLayout(layoutH);

    this->setLayout(layoutV);
}

void MainWindow::loadParser()
{
    if(lineFilePath->isModified() && lineFilePath->text()!="")
    {
        teRes->clear();
        teObj->clear();
        teRel->clear();
        Parser* pars =new Parser(lineFilePath->text());
        for(int i=1;i<=pars->getOldObjects().size();i++)
        {
            teObj->textCursor().insertText(pars->getOldObjects()[i]+"\n");
        }
        for(int i=0;i<pars->getNewRelations().size();i++)
        {
            Relation rel=pars->getNewRelations()[i];
            teRel->textCursor().insertText(pars->getNewObjects()[rel.from]+" "+rel.type+" "+pars->getNewObjects()[rel.to] +"\n");
        }
        for(int i=1+pars->deltaSize;i<=pars->getNewObjects().size();i++)
        {
            teRes->textCursor().insertText(pars->getNewObjects()[i]+"\n");
        }
    }
}

MainWindow::~MainWindow()
{

}
