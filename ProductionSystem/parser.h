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

private:
    QVector<QString> progTxt;
    QMap<int,QString> objects;
    QVector<Relation> relations;
    QMap<int,QString> newObjects;
    QVector<Relation> newRelations;

    void parseFile();

    void findObjects(QString str);
    Relation findRelations(QString str);
    void findRules(QString str);

    bool findNewObjects(QMap<int,QString> objects, QVector<Relation> relations);
    bool findNewRelations(QVector<Relation> relations);

    QString deleteScobs(QString str);

    bool isSameNewObjects(QString str);
    bool isSameRelations(Relation rel);


};

#endif // PARSER_H
