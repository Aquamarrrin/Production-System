#ifndef PARSER_H
#define PARSER_H

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QVector>
#include <QMap>
#include <QPair>
#include <QRegExp>
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
    int delta;
    int deltaRel;

    QVector<QString> progTxt;
    QMap<int,QString> objects;
    QVector<Relation> relations;
    QVector<QPair<Relation,Relation> > extraRelations;
    QVector<QString> rules;
    QMap<int,QString> newObjects;
    QVector<Relation> newRelations;
    QVector<QPair<QString,QString> > objWRules;

    void parseFile(); //Парсит файл

    void findObjects(QString str); //Ищет объекты в файле
    void findRules(QString str); //Ищет правила в файле
    Relation findRelations(QString str); //Ищет отношения в файле


    bool findNewObjects(QMap<int,QString> objects, QVector<Relation> relations);  //Ищет и формирует новые объекты из имеющихся
    bool findNewRelations(QVector<Relation> relations); //Ищет и формирует новые отношения из имеющихся
    void findNewObjWithRules(QMap<int,QString> objects, int startSize); //Ищет новые объекты в соответствии с заданными правилами

    QString deleteScobs(QString str); //Удаляет скобки вида ()
    QString deleteDoublePM(QString str); //Удаляет знаки вида ++ --
    QString deletePlusesAndScobes(QString obj1, QString obj2); //Убирает + там , где они не нужны

    bool isSameNewObjects(QString str);  //Проверяет, есть ли уже такой же найденный объект
    bool isSameRelations(Relation rel);  //Проверяет, есть ли уже такое же найденное отношение
    bool lieOutOfFunc(QString mainObj , QString obj , QVector<QPair<int, QString> > funcs);// Проверяет, лежит ли объект внe функции

    int countNewObjects(); //Cчитает колличество новых объектов
    int countLeftScobs(QString str); //Cчитает колличество скобок (
    int countRightScobs(QString str); //Cчитает колличество скобок )

    QString deleteAllFuncs(QString obj,  QVector<QPair<int,QString> > funcs);
    QString insertAllFuncs(QString obj,  QVector<QPair<int,QString> > funcs);

    QVector<QPair<int,QString> > cutFunc(QString obj, QString func);  //Вырезает функцию из объекта
    QString findExpBeforeSim(QString obj,QString sim);  //Вырезает выражение в скобках до определённого символа или строчки
    QString findExpAfterSim(QString obj,QString sim);  //Вырезает выражение в скобках после определённого символа или строчки
    QString deleteOppositeObjs(QString obj, QString objIn, QVector<QPair<int,QString> > cutF); //Cкладывает и вычитает одинаковые элементы
    QString plusMinusSameObjs(bool plus, QString obj, QString objIn, QVector<QPair<int,QString> > cutF); //Cкладывает и вычитает одинаковые элементы
    QString outOfScobsSameObjs(bool bef, QString obj, QString objIn, QVector<QPair<int,QString> > cutF); //Выносит одинаковые элементы за скобки
    QString cutAroundIndex(int index, QString obj); //Вырезает вокруг индекса объекты до +,- или скобок
    QString multTwoObjs(QString origObj, QString obj1, QString obj2); //Перемножаем 2 объекта
    QString plusTwoObjs(QString origObj, QString obj1, QString obj2); //Убираем скобки когда складываем 2 больших объекта
    QString findOpenScob(QString obj, QString sim); //Находит и возвращает выражение c sim до (
    QString findCloseScob(QString obj, QString sim); //Находит и возвращает выражение c sim до )
    QVector<QString> whatObj(QString str);  //Проверяет и возвращает индексы объектов , содержащихся в данном объекте
};

#endif // PARSER_H
