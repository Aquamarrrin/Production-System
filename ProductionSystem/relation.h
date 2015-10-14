#ifndef RELATION_H
#define RELATION_H

#include <QString>
class Relation
{
public:
    Relation();
    Relation(int from, int to, QString type);

    int from;
    int to;
    QString type;
};

#endif // RELATION_H
