#ifndef PARSER_H
#define PARSER_H

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QVector>
#include <QMap>
#include <QtDebug>
#include "relation.h"

using namespace std;

class Parser
{
public:
    Parser(QString filePath);
    QMap<int,QString> getNewObjects();
    QMap<int,QString> getOldObjects();
    QVector<Relation> getNewRelations();

    int deltaSize;

private:
    QVector<QString> progTxt;
    QMap<int,QString> objects;
    QVector<Relation> relations;
    QMap<int,QString> newObjects;
    QVector<Relation> newRelations;

    void parseFile(); //Парсит файл

    void findObjects(QString str); //Ищет объекты в файле
    void findRules(QString str); //Ищет правила в файле
    Relation findRelations(QString str); //Ищет отношения в файле

    bool findNewObjects(QMap<int,QString> objects, QVector<Relation> relations);  //Ищет и формирует новые объекты из имеющихся
    bool findNewRelations(QVector<Relation> relations); //Ищет и формирует новые отношения из имеющихся

    QString deleteScobs(QString str); //Удаляет скобки вида ()

    bool isSameNewObjects(QString str);  //Проверяет, есть ли уже такой же найденный объект
    bool isSameRelations(Relation rel);  //Проверяет, есть ли уже такое же найденное отношение

    int countNewObjects(); //Cчитает колличество новых объектов
    int countLeftScobs(QString str); //Cчитает колличество скобок (
    int countRightScobs(QString str); //Cчитает колличество скобок )

    QString cutFunc(QString obj, QString func);  //Вырезает функцию из объекта

    QVector<int> whatObj(QString str);  //Проверяет и возвращает индексы объектов , содержащихся в данном объекте
};

#endif // PARSER_H
