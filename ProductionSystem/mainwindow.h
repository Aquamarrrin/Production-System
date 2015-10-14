#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QTextEdit>
#include <QLineEdit>
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

private:
    QPushButton* btnAnalyze;
    QPushButton* btnLoad;

    QLineEdit* lineFilePath;

    QVBoxLayout* layoutV;

    QTextEdit* te;

};

#endif // MAINWINDOW_H
