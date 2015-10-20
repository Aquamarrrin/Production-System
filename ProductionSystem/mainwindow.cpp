#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    this->setMinimumSize(500,500);
    layoutV = new QGridLayout();

    btnLoad=new QPushButton("Загрузить");
    QObject::connect(btnLoad,SIGNAL(clicked()),this,SLOT(loadParser()));
    btnAnalyze = new QPushButton("Найти");
    QObject::connect(btnAnalyze,SIGNAL(clicked()),this,SLOT(findLine()));

    lineFilePath = new QLineEdit();
    findLineText = new QLineEdit();

    teObj=new QTextEdit();
    teRel=new QTextEdit();
    teRes=new QTextEdit();

    QLabel* lbl1 = new QLabel("Начальные объекты:");
    QLabel* lbl2 = new QLabel("Полученные отношения:");
    QLabel* lbl3 = new QLabel("Полученные объекты:");
    QLabel* lbl4 = new QLabel("Найти выражение:");

    //Добавление виджетов в лэйауты
    layoutV->addWidget(lineFilePath,0,0,1,3);
    layoutV->addWidget(btnLoad,1,0,1,3);
    layoutV->addWidget(lbl1,2,0);
    layoutV->addWidget(lbl2,2,1);
    layoutV->addWidget(lbl3,2,2);
    layoutV->addWidget(teObj,3,0);
    layoutV->addWidget(teRel,3,1);
    layoutV->addWidget(teRes,3,2);
    layoutV->addWidget(lbl4,4,0);
    layoutV->addWidget(findLineText,4,1);
    layoutV->addWidget(btnAnalyze,4,2);

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
            if(rel.type=="equals")
                teRel->textCursor().insertText(pars->getNewObjects()[rel.from]+" = "+pars->getNewObjects()[rel.to] +"\n");
            else
                teRel->textCursor().insertText(pars->getNewObjects()[rel.from]+" "+rel.type+" "+pars->getNewObjects()[rel.to] +"\n");
        }
        for(int i=1+pars->deltaSize;i<=pars->getNewObjects().size();i++)
        {
            teRes->textCursor().insertText(pars->getNewObjects()[i]+"\n");
        }
    }
}

void MainWindow::findLine()
{
    QString str=findLineText->text();
    if(!str.isEmpty())
    {
        bool ok=false;
        teRel->clear();
        Parser* pars =new Parser(lineFilePath->text());
        for(int i=0;i<pars->getNewRelations().size();i++)
        {
            Relation rel=pars->getNewRelations()[i];
            QString tmp;
            if(rel.type=="equals")
                tmp = pars->getNewObjects()[rel.from]+" = "+pars->getNewObjects()[rel.to];
            else
                tmp = pars->getNewObjects()[rel.from]+" "+rel.type+" "+pars->getNewObjects()[rel.to];
            if(tmp==str)
            {
                teRel->textCursor().insertText(tmp+"\n");
                ok=true;
                break;
            }
        }
        if(!ok)
        {
            teRel->textCursor().insertText("Выражение не найдено!\n");
        }
    }
}

MainWindow::~MainWindow()
{

}
