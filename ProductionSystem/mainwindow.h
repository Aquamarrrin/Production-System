#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QString>
#include "parser.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadParser();
    void findLine();

private:
    QPushButton* btnAnalyze;
    QPushButton* btnLoad;

    QLineEdit* lineFilePath;
    QLineEdit* findLineText;

    QGridLayout* layoutV;

    QTextEdit* teObj;
    QTextEdit* teRel;
    QTextEdit* teRes;

};

#endif // MAINWINDOW_H
