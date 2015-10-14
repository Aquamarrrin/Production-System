#include "relation.h"


Relation::Relation()
{
    this->from=0;
    this->to=0;
    this->type="";
}



Relation::Relation(int from, int to, QString type)
{
    this->from=from;
    this->to=to;
    this->type=type;
}
