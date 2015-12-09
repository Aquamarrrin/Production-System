#include "parser.h"
// D:\\Qt\\Qt5.1.1\\Tools\\QtCreator\\bin\\ProductionSystem\\viet.txt
Parser::Parser(QString filePath)
{
    QFile myFile(filePath);
    if(!myFile.open(QFile::ReadOnly | QFile::Text))
    {

    }
    QTextStream myText(&myFile);
    while (myText.atEnd() == false)
    {
        QString strTxt=myText.readLine();
        progTxt.push_back(strTxt);
    }
    myFile.close();

    if(progTxt.size()!=0)
        parseFile();

    deltaSize=countNewObjects();
}

void Parser::parseFile()
{
    int i=0;
    while(progTxt[i]!="Bon:" && i!=progTxt.size())
    {
        if(progTxt[i]!="Obj:" && progTxt[i]!="")
            findObjects(progTxt[i]);
        i++;
    }
    i++;
    while(progTxt[i]!="Rul:" && i!=progTxt.size())
    {
        if(progTxt[i]!="")
            relations.push_back(findRelations(progTxt[i]));
        i++;
    }
    i++;

    newObjects=objects;
    newRelations=relations;

    bool x1=findNewRelations(newRelations);
    while(x1)
        x1=findNewRelations(newRelations);

    while(i!=progTxt.size())
    {
        if(progTxt[i]!="")
            findRules(progTxt[i]);
        i++;
    }


    qDebug()<<"1";
    bool x2 = findNewObjects(objects,newRelations);
    while(x2)
        x2=findNewObjects(newObjects,newRelations);
    qDebug()<<"2";
    x1=findNewRelations(newRelations);
    while(x1)
        x1=findNewRelations(newRelations);
    qDebug()<<"3";
}

void Parser::findObjects(QString str)
{
    QString tmpStr;
    int num;
    int i=0;

//СТОИТ ВВЕСТИ ЗДЕСЬ ДОПОЛНИТЕЛЬНУЮ ПРОВЕРКУ НА СООТВЕТСТВИЕ СИМВОЛОВ ЧИСЛАМ
    //Вырезаем номер объекта
    while(str[i]!=' ' && i!=str.size())
    {
        tmpStr+=str[i];
        i++;
    }
    num=tmpStr.toInt();
    tmpStr="";

    i++;
    //Вырезаем имя объекта
    while(str[i]!=' ' && i!=str.size())
    {
        tmpStr+=str[i];
        i++;
    }
    //qDebug()<<tmpStr;
    if(tmpStr!="")
        objects[num].push_back(tmpStr);
}

Relation Parser::findRelations(QString str)
{
    QString tmpStr;
    QString type;
    int from;
    int to;
    int i=0;

//СТОИТ ВВЕСТИ ЗДЕСЬ ДОПОЛНИТЕЛЬНУЮ ПРОВЕРКУ НА СООТВЕТСТВИЕ СИМВОЛОВ ЧИСЛАМ
    //Вырезаем узел "от кого"
    while(str[i]!=' ' && i!=str.size())
    {
        tmpStr+=str[i];
        i++;
    }
    from=tmpStr.toInt();
    tmpStr="";
    i++;

    //Вырезаем узел "связь"
    while(str[i]!=' ' && i!=str.size())
    {
        tmpStr+=str[i];
        i++;
    }
    type=tmpStr;
    tmpStr="";
    i++;

    //Вырезаем узел "к кому"
    while(i!=str.size() && str[i]!=' ')
    {
        tmpStr+=str[i];
        i++;
    }
    to=tmpStr.toInt();

    Relation rel(from, to, type);
    return rel;
}

void Parser:: findRules(QString str)
{
    QString tmpStr;
    int i=0;

    while(i!=str.size() && str[i]!=' ')
    {
        tmpStr+=str[i];
        i++;
    }
    if(tmpStr=="IF")
    {
        i++;
        tmpStr="";
        while(i!=str.size() && str[i]!='T')
        {
            tmpStr+=str[i];
            i++;
        }
        //qDebug()<<tmpStr;

        if(tmpStr.contains("obj"))
        {
            rules.push_back(str);
        }
        else
            if(isSameRelations(findRelations(tmpStr)))
            {
                tmpStr="";
                while(i!=str.size() && str[i]!=' ')
                {
                    tmpStr+=str[i];
                    i++;
                }
                if(tmpStr=="THEN")
                {
                    i++;
                    tmpStr="";
                    while(i!=str.size() )
                    {
                        tmpStr+=str[i];
                        i++;
                    }
                    if(!isSameRelations(findRelations(tmpStr)))
                    {
                        qDebug()<<"NEW REL IN RULES::"<<objects[findRelations(tmpStr).from]<<"--->"<<findRelations(tmpStr).type<<"--->"<<objects[findRelations(tmpStr).to];
                        newRelations.push_back(findRelations(tmpStr));
                    }
                }
            }
    }

}

bool Parser::findNewObjWithRules(QMap<int,QString> objects, QString object1, QString object2, QString str)
{
    QString tmpStr;
    bool found = false;
    int i=0;
    while(i!=str.size() && str[i]!=' ')
    {
        tmpStr+=str[i];
        i++;
    }
    if(tmpStr=="IF")
    {
        i++;
        tmpStr="";
        while(i!=str.size() && str[i]!='T')
        {
            tmpStr+=str[i];
            i++;
        }
        if(tmpStr.contains("obj"))
        {
            QString tmpWithObj=tmpStr;
            bool exist=false;
            tmpStr="";
            while(i!=str.size() && str[i]!=' ')
            {
                tmpStr+=str[i];
                i++;
            }
            if(tmpStr=="THEN")
            {
                i++;
                tmpStr="";
                while(i!=str.size() )
                {
                    tmpStr+=str[i];
                    i++;
                }
            }

            QString obj = tmpWithObj;
            obj=  obj.replace("obj1",object1);
            obj=  obj.replace("obj2",object2);
            obj.remove(' ');
            for(int l=1;l<=objects.size();l++)
            {
                //qDebug()<<objects[l]<<" ----> "<<obj;
                if(objects[l].contains(obj))
                {/*
                    qDebug()<<"RULE::"<<str;
                    qDebug()<<" OBJ:: "<<obj;
                    qDebug()<<" OBJ[l]:: "<<objects[l] ;
                    qDebug()<<" TMPSTR:: "<<tmpStr ;*/
                    exist=true;
                    break;
                }
            }

            if(exist && tmpStr.contains("obj"))
            {

                QString obj1 = tmpStr;
                obj1=  obj1.replace("obj1",object1);
                obj1=  obj1.replace("obj2",object2);
                //qDebug()<<obj<<" :: "<<obj1;
                if(obj1.contains("equals"))
                {
                    QString firstNod = obj1.left(obj1.indexOf("equals"));
                    QString secNod = obj1.right(obj1.size()-obj1.indexOf("equals")-6);
                    firstNod.remove(' ');
                    secNod.remove(' ');/*
                   qDebug()<<"   FIRSTNODE:: "<<firstNod;
                   qDebug()<<"   SECONDNODE:: "<<secNod;*/
                    if (firstNod!="" && secNod!="")
                    {
                        for(int l=1;l<=objects.size();l++)
                        {
                            if(firstNod==objects[l])
                            {
                                if(!isSameNewObjects(secNod))
                                {
                                    newObjects[newObjects.size()+1]=secNod;
                                    Relation rel(l,newObjects.size(),"equals");
                                    newRelations.push_back(rel);
                                    qDebug()<<"*"<<rel.from<<rel.type<<rel.to;
                                    found= true;
                                }
                                else
                                {
                                    Relation rel(l,newObjects.key(secNod,-1),"equals");
                                    if(newObjects.key(secNod,-1)==-1)
                                    {
                                        secNod=secNod.remove(secNod.size()-1,1).remove(0,1);
                                        if(newObjects.key(secNod,-1)==-1)
                                            secNod=secNod.remove(secNod.size()-1,1).remove(0,1);
                                        rel.to=newObjects.key(secNod,-1);
                                    }
                                    if(newObjects.key(secNod,-1)!=-1 && !isSameRelations(rel))
                                    {
                                        newRelations.push_back(rel);
                                        qDebug()<<"**"<<rel.from<<rel.type<<rel.to;
                                        //found= true;
                                    }
                                }
                            }
                            else
                                if(objects[l].contains(firstNod))
                                {
                                    if(!isSameNewObjects(firstNod))
                                    {
                                        newObjects[newObjects.size()+1]=firstNod;
                                        if(!isSameNewObjects(secNod))
                                        {
                                            newObjects[newObjects.size()+1]=secNod;
                                            Relation rel(newObjects.size()-1,newObjects.size(),"equals");
                                            newRelations.push_back(rel);
                                            qDebug()<<"***"<<rel.from<<rel.type<<rel.to;
                                            found= true;
                                        }
                                        else
                                        {
                                            Relation rel(newObjects.size(),newObjects.key(secNod),"equals");
                                            if(!isSameRelations(rel))
                                            {
                                                newRelations.push_back(rel);
                                                qDebug()<<"****"<<rel.from<<rel.type<<rel.to;
                                                //found= true;
                                            }
                                        }
                                    }
                                }
                        }
                    }
                }
            }

        }
    }
    return found;
}

//Нахождение новых объектов
bool Parser::findNewObjects(QMap<int, QString> objects, QVector<Relation> relations)
{
    bool found = false;

    for(int i=1;i<=objects.size();i++)
    {
        //relations=newRelations;
        for(int j=1;j<=objects.size();j++)
        {
            //qDebug();
            if(objects[j].contains(objects[i]))  //подставляем равнозначные значения
            {
                for(int k=0;k<relations.size();k++ )
                {
                    QString tmpStr3=objects[j];
                    QString tmpStr4=objects[j];
                    QString tmpStr1;
                    QString tmpStr2;
                    int indexOFObj=tmpStr3.indexOf(objects[i]);
                    if(indexOFObj>0 && indexOFObj+objects[i].size()<tmpStr3.size() && tmpStr3[indexOFObj-1]=='('
                            && tmpStr3[indexOFObj+objects[i].size()]==')')
                    {
                        tmpStr1 = tmpStr3.replace(objects[i],objects[relations[k].to]);
                        tmpStr2 = tmpStr4.replace(objects[i],objects[relations[k].from]);
                        tmpStr1=deletePlusesAndScobes(tmpStr1,objects[relations[k].to]);
                        tmpStr2=deletePlusesAndScobes(tmpStr2,objects[relations[k].from]);

                        if(relations[k].from==i && relations[k].type=="equals"      //Если содержит объект, вместо него подставляется равный ему другой объект
                                && tmpStr1!="" && !isSameNewObjects(tmpStr1)            //Но только в том случае, если в "главном" объекте уже не содержится
                                && (((objects[i].size()<objects[relations[k].to].size() //"подставляемый" объект, а в нем не содержится тот, вместо которого подставляют
                                      && !objects[relations[k].to].contains(objects[i]))
                                     || objects[i].size()>=objects[relations[k].to].size() ) ) )
                        {
                            qDebug()<<"FROM:::"<<objects[j]<<"USE:::"<<objects[relations[k].to]<<"TO::"<<tmpStr1;
                            newObjects[newObjects.size()+1]=tmpStr1;
                            //qDebug()<<objects[i]<<" : "<<objects[j]<<" : "<<tmpStr1;
                            Relation rel(j,newObjects.size(),"equals");
                            newRelations.push_back(rel);
                            found=true;
                        }
                        else
                            if(relations[k].to==i && relations[k].type=="equals"
                                    && tmpStr2!="" && !isSameNewObjects(tmpStr2)
                                    && (((objects[i].size()<objects[relations[k].from].size() &&!objects[relations[k].from].contains(objects[i]))
                                         || objects[i].size()>=objects[relations[k].from].size()  ) ))
                            {
                                qDebug()<<"FROM:::"<<objects[j]<<"USE:::"<<objects[relations[k].from]<<"TO::"<<tmpStr2;
                                newObjects[newObjects.size()+1]=tmpStr2;
                                // qDebug()<<objects[i]<<" : "<<objects[j]<<" : "<<tmpStr2;
                                Relation rel(j,newObjects.size(),"equals");
                                newRelations.push_back(rel);
                                found=true;
                            }
                    }
                }

            }

            if((objects[j].contains("+("+objects[i]+")") &&  (objects[j].contains("-("+objects[i]+")")))) //вычитаем с разными знаками
            {
                QString tmpStr1 = findOpenScob(objects[j],"+("+objects[i]+")")+findCloseScob(objects[j],"+("+objects[i]+")");
                //qDebug()<<"1ST::"<<tmpStr1;
                if(tmpStr1.contains("-("+objects[i]+")"))
                {
                    QVector<QPair<int,QString> > cutF1 = cutFunc(tmpStr1,"sqrt_");
                    QString tmpStr4=deleteOppositeObjs(tmpStr1,objects[i],cutF1);
                    //qDebug()<<"2ND::"<<tmpStr4;
                    if(tmpStr4!="" )
                    {
                        QString tmpStr2 = objects[j];
                        tmpStr4=tmpStr2.replace(tmpStr1,tmpStr4);
                        //qDebug()<<"3RD::"<<tmpStr4;
                        //tmpStr4=deleteScobs(deleteDoublePM(tmpStr4));
                        if(!isSameNewObjects(tmpStr4))
                        {
                            qDebug()<<"+-::"<<tmpStr4;
                            newObjects[newObjects.size()+1]=tmpStr4;
                            //qDebug()<<objects[i]<<" : "<<objects[j]<<" : "<<newObjects[newObjects.size()];
                            Relation rel(j,newObjects.size(),"equals");
                            newRelations.push_back(rel);
                            found=true;
                        }
                    }
                }
            }

            //qDebug()<<objects[j]<<"  :  "<<objects[i]<<"  :  "<<cutF;
            if((objects[j].contains("+("+objects[i]+")") )) //складываем одинаковые
            {
                QString tmpStr1 = findOpenScob(objects[j],"+("+objects[i]+")")+findCloseScob(objects[j],"+("+objects[i]+")");
               // qDebug()<<"1ST::"<<tmpStr1;
                if(tmpStr1.count("+("+objects[i]+")")>1)
                {
                    QVector<QPair<int,QString> > cutF1 = cutFunc(tmpStr1,"sqrt_");
                    QString tmpStr4=plusMinusSameObjs(1,tmpStr1,objects[i],cutF1);
                 //   qDebug()<<"2ND::"<<tmpStr4;
                    if(tmpStr4!="" )
                    {
                        QString tmpStr2 = objects[j];
                        tmpStr4=tmpStr2.replace(tmpStr1,tmpStr4);
                    //    qDebug()<<"3RD::"<<tmpStr4;
                        //tmpStr4=deleteScobs(deleteDoublePM(tmpStr4));
                        if(!isSameNewObjects(tmpStr4))
                        {
                            qDebug()<<"+::"<<tmpStr4;
                            newObjects[newObjects.size()+1]=tmpStr4;
                            //qDebug()<<objects[i]<<" : "<<objects[j]<<" : "<<newObjects[newObjects.size()];
                            Relation rel(j,newObjects.size(),"equals");
                            newRelations.push_back(rel);
                            found=true;
                        }
                    }
                }
            }

            if((objects[j].contains("-("+objects[i]+")") )) //вычитаем одинаковые
            {
                QString tmpStr1 = findOpenScob(objects[j],"-("+objects[i]+")")+findCloseScob(objects[j],"-("+objects[i]+")");
               // qDebug()<<"1ST::"<<tmpStr1;
                if(tmpStr1.count("-("+objects[i]+")")>1)
                {
                    QVector<QPair<int,QString> > cutF1 = cutFunc(tmpStr1,"sqrt_");
                    QString tmpStr4=plusMinusSameObjs(0,tmpStr1,objects[i],cutF1);
                 //   qDebug()<<"2ND::"<<tmpStr4;
                    if(tmpStr4!="" )
                    {
                        QString tmpStr2 = objects[j];
                        tmpStr4=tmpStr2.replace(tmpStr1,tmpStr4);
                     //   qDebug()<<"3RD::"<<tmpStr4;
                        //tmpStr4=deleteScobs(deleteDoublePM(tmpStr4));
                        if(!isSameNewObjects(tmpStr4))
                        {
                            qDebug()<<"-::"<<tmpStr4;
                            newObjects[newObjects.size()+1]=tmpStr4;
                            //qDebug()<<objects[i]<<" : "<<objects[j]<<" : "<<newObjects[newObjects.size()];
                            Relation rel(j,newObjects.size(),"equals");
                            newRelations.push_back(rel);
                            found=true;
                        }
                    }
                }
            }

            if(objects[j].contains("("+objects[i]+")*")) //выносим за скобки
            {
                QString tmpStr1 = findOpenScob(objects[j],"("+objects[i]+")*")+findCloseScob(objects[j],"("+objects[i]+")*");
             //   qDebug()<<"1ST::"<<tmpStr1<<"OBJ[i]::"<<objects[i];
                if(tmpStr1.count("("+objects[i]+")*")>1)
                {
                    QVector<QPair<int,QString> > cutF1 = cutFunc(tmpStr1,"sqrt_");
                    QString tmpStr4=outOfScobsSameObjs(0,tmpStr1,objects[i],cutF1);
                //    qDebug()<<"2ND::"<<tmpStr4;
                    if(tmpStr4!="" )
                    {
                        QString tmpStr2 = objects[j];
                        tmpStr4=tmpStr2.replace(tmpStr1,tmpStr4);
                  //      qDebug()<<"3RD::"<<tmpStr4;
                        //tmpStr4=deleteScobs(deleteDoublePM(tmpStr4));
                        if(!isSameNewObjects(tmpStr4))
                        {
                            qDebug()<<")*::"<<tmpStr4;
                            newObjects[newObjects.size()+1]=tmpStr4;
                            //qDebug()<<objects[i]<<" : "<<objects[j]<<" : "<<newObjects[newObjects.size()];
                            Relation rel(j,newObjects.size(),"equals");
                            newRelations.push_back(rel);
                            found=true;
                        }
                    }
                }
               // qDebug()<<" *************************************************** ";
            }

            if(objects[j].contains("*("+objects[i]+")")) //выносим за скобки
            {
                QString tmpStr1 = findOpenScob(objects[j],"*("+objects[i]+")")+findCloseScob(objects[j],"*("+objects[i]+")");
                //qDebug()<<"1ST::"<<tmpStr1<<"OBJ[i]::"<<objects[i];
                if(tmpStr1.count("*("+objects[i]+")")>1)
                {
                    QVector<QPair<int,QString> > cutF1 = cutFunc(tmpStr1,"sqrt_");
                    QString tmpStr4=outOfScobsSameObjs(1,tmpStr1,objects[i],cutF1);
                    //qDebug()<<"2ND::"<<tmpStr4;
                    if(tmpStr4!="" )
                    {
                        QString tmpStr2 = objects[j];
                        tmpStr4=tmpStr2.replace(tmpStr1,tmpStr4);
                      //  qDebug()<<"3RD::"<<tmpStr4;
                       // tmpStr4=deleteScobs(deleteDoublePM(tmpStr4));
                        if(!isSameNewObjects(tmpStr4))
                        {
                            qDebug()<<"*(::"<<tmpStr4;
                            newObjects[newObjects.size()+1]=tmpStr4;
                            //qDebug()<<objects[i]<<" : "<<objects[j]<<" : "<<newObjects[newObjects.size()];
                            Relation rel(j,newObjects.size(),"equals");
                            newRelations.push_back(rel);
                            found=true;
                        }
                    }
                }
              //  qDebug()<<" *************************************************** ";
            }

            if(objects[j].contains(")*(")) //Перемножаем 2 объекта
            {
                QString tmpStr1=findExpBeforeSim(objects[j],")*(");
                QString tmpStr2=findExpAfterSim(objects[j],")*(");
                if((tmpStr1.contains('-') || tmpStr1.contains('+'))&&(tmpStr2.contains('-') || tmpStr2.contains('+')))
                {
                    QString tmpStr4=multTwoObjs(objects[j],tmpStr1,tmpStr2);
                   tmpStr4=deleteScobs(deleteDoublePM(tmpStr4));
                    if(tmpStr4!="" && !isSameNewObjects(tmpStr4))
                    {
                        qDebug()<<")*(::"<<tmpStr4;
                        newObjects[newObjects.size()+1]=tmpStr4;
                        //qDebug()<<objects[i]<<" : "<<objects[j]<<" : "<<newObjects[newObjects.size()];
                        Relation rel(j,newObjects.size(),"equals");
                        newRelations.push_back(rel);
                        found=true;
                    }
                }
            }

            if(objects[j].contains(")+(")) //Складываем 2 сложных объекта
            {
                QString tmpStr1=findExpBeforeSim(objects[j],")+(");
                QString tmpStr2=findExpAfterSim(objects[j],")+(");
                if((tmpStr1.contains('-') || tmpStr1.contains('+'))&&(tmpStr2.contains('-') || tmpStr2.contains('+')))
                {
                    QString tmpStr4=plusTwoObjs(objects[j],tmpStr1,tmpStr2);
                   tmpStr4=deleteScobs(deleteDoublePM(tmpStr4));
                    if(tmpStr4!="" && !isSameNewObjects(tmpStr4))
                    {
                        qDebug()<<")+(::"<<tmpStr4;
                        newObjects[newObjects.size()+1]=tmpStr4;
                        //qDebug()<<objects[i]<<" : "<<objects[j]<<" : "<<newObjects[newObjects.size()];
                        Relation rel(j,newObjects.size(),"equals");
                        newRelations.push_back(rel);
                        found=true;
                    }
                }
            }

            if(objects[j].contains(")-(")) //Вычитаем 2 сложных объекта
            {
                QString tmpStr1=findExpBeforeSim(objects[j],")-(");
                QString tmpStr2=findExpAfterSim(objects[j],")-(");
                if((tmpStr1.contains('-') || tmpStr1.contains('+'))&&(tmpStr2.contains('-') || tmpStr2.contains('+')))
                {
                    QString tmpStr4=plusTwoObjs(objects[j],tmpStr1,tmpStr2);
                   tmpStr4=deleteScobs(deleteDoublePM(tmpStr4));
                    if(tmpStr4!="" && !isSameNewObjects(tmpStr4))
                    {
                        qDebug()<<")-(::"<<tmpStr4;
                        newObjects[newObjects.size()+1]=tmpStr4;
                        //qDebug()<<objects[i]<<" : "<<objects[j]<<" : "<<newObjects[newObjects.size()];
                        Relation rel(j,newObjects.size(),"equals");
                        newRelations.push_back(rel);
                        found=true;
                    }
                }
            }

            if(objects[j].contains("(-("+objects[i]+"))")) //Убираем знак - ,там где он не нужен
            {
                QString tmpStr1=objects[j];
                int ind1 = tmpStr1.lastIndexOf('+', tmpStr1.indexOf("-("+objects[i]+")")-1);
                int ind2 = tmpStr1.lastIndexOf('-', tmpStr1.indexOf("-("+objects[i]+")")-1);
                if(ind2>ind1)
                    tmpStr1[ind2]='+';
                if(ind2<ind1)
                    tmpStr1[ind1]='-';
                tmpStr1=tmpStr1.replace("(-("+objects[i]+"))","("+objects[i]+")");
               // tmpStr1=deleteScobs(deleteDoublePM(tmpStr1));
                if(tmpStr1!="" && !isSameNewObjects(tmpStr1))
                {
                    qDebug()<<"--::"<<tmpStr1;
                    newObjects[newObjects.size()+1]=tmpStr1;
                    //qDebug()<<objects[i]<<" : "<<objects[j]<<" : "<<newObjects[newObjects.size()];
                    Relation rel(j,newObjects.size(),"equals");
                    newRelations.push_back(rel);
                    found=true;
                }
            }

            for(int rule=0;rule<rules.size();rule++)
            {
                if(findNewObjWithRules(objects,objects[i],objects[j],rules[rule]))
                    found=true;
            }
        }

    }

    qDebug()<<found;
    return found;
}

bool Parser::findNewRelations(QVector<Relation> relations)
{
    bool found=false;

    for(int i=0;i<relations.size();i++)
    {
        for(int j=0;j<relations.size();j++)
        {
            if(relations[i].from==relations[j].from && relations[i].type==relations[j].type && relations[j].type=="equals")
            {
                Relation rel(relations[i].to,relations[j].to,"equals");
                if(!isSameRelations(rel))
                {
                    newRelations.push_back(rel);
                    //qDebug()<<newObjects[rel.from]<<rel.type<<newObjects[rel.to];
                    found=true;
                }
            }
            if(relations[i].to==relations[j].to && relations[i].type==relations[j].type && relations[j].type=="equals")
            {
                Relation rel(relations[i].from,relations[j].from,"equals");
                if(!isSameRelations(rel))
                {
                    newRelations.push_back(rel);
                    //qDebug()<<newObjects[rel.from]<<rel.type<<newObjects[rel.to];
                    found=true;
                }
            }
        }
    }

    return found;
}

QVector<QPair<int, QString> > Parser::cutFunc(QString obj, QString func)
{
    QVector<QPair<int, QString> > vec;
    QPair<int, QString> pair;
    int index;
    QString str;
    int l=0;
    while(obj!="" && obj.contains(func))
    {
        int i=1;
        index=obj.indexOf(func);
        str=obj.mid(obj.indexOf(func),func.size()+i);
        while(i!=obj.size() && countLeftScobs(str)!=countRightScobs(str))
        {
            i++;
            str=obj.mid(obj.indexOf(func),func.size()+i+1);
        }
        obj=obj.remove(obj.indexOf(func),str.size());
        //qDebug()<<obj<<" : "<<str;
        pair.first=index+l;
        pair.second=str;
        vec.push_back(pair);
        l+=str.size();
       // qDebug()<<pair;
    }
    return vec;
}

QString Parser::deleteAllFuncs(QString obj, QVector<QPair<int, QString> > funcs)
{
    QString str="";
    int l=0;
    for(int i=0;i<funcs.size();i++)
    {
        //qDebug()<<funcs[i].second<<" : "<<funcs[i].first;
        str=obj.remove(funcs[i].first-l,funcs[i].second.size());
        l+=funcs[i].second.size();
    }
    //qDebug()<<obj;
    return str;
}

QString Parser::insertAllFuncs(QString obj, QVector<QPair<int, QString> > funcs)
{
    QString str="";
    for(int i=0;i<funcs.size();i++)
    {
        //qDebug()<<funcs[i].second<<" : "<<funcs[i].first;
        str=obj.insert(funcs[i].first,funcs[i].second);
    }
    return str;
}

QString Parser::findOpenScob(QString obj, QString sim)
{
    QString str="";
    if(obj.contains(sim))
    {
        int ind=obj.indexOf(sim)-1;
        int i=1;
        str=obj.mid(ind,1);
        while(i<ind && (countLeftScobs(str)<=countRightScobs(str)))
        {
            //qDebug()<<"STR::"<<str;
            i++;
            str=obj.mid(ind-i,i+1);
        }
    }
    return str;
}

QString Parser::findCloseScob(QString obj, QString sim)
{
    QString str="";
    if(obj.contains(sim))
    {
        int ind=obj.indexOf(sim);
        int i=1;
        str=obj.mid(ind,i);
        while(i<obj.size() && countLeftScobs(str)>=countRightScobs(str))
        {
            //qDebug()<<"STR::"<<str;
            i++;
            str=obj.mid(ind,i);
        }
    }
    return str;
}


QString Parser::findExpBeforeSim(QString obj,QString sim)
{
    QString str="";
    if(obj.contains(sim))
    {
        int ind=obj.indexOf(sim)-1;
        if(sim.contains(")"))
            ind=obj.indexOf(sim);
        int count = obj.count(sim);
        //if(count==1)
        //{
            int i=1;
            str=obj.mid(ind,1);
            while(i<ind && (countLeftScobs(str)!=countRightScobs(str)))
            {
                i++;
                str=obj.mid(ind-i,i+1);
            }
            //qDebug()<<obj<<" BEF_STR: "<<str;
        //}
    }
    return str;
}


QString Parser::findExpAfterSim(QString obj,QString sim)
{
    QString str="";
    if(obj.contains(sim))
    {
        int ind=obj.indexOf(sim);
        if(sim.contains("("))
            ind=obj.indexOf(sim)-1;
        int count = obj.count(sim);
       // if(count==1)
       // {
            int i=1;
            str=obj.mid(ind+sim.size(),i);
            while(i<obj.size() && countLeftScobs(str)!=countRightScobs(str))
            {
                i++;
                str=obj.mid(ind+sim.size(),i);
            }
        //}
    }
    return str;
}

QString Parser::deleteOppositeObjs(QString obj, QString objIn, QVector<QPair<int, QString> > cutF)
{
    QString str="";
    QString oldObj=obj;
    if((cutF.size()>1) || (cutF.size()==1 && cutF[0].second!=oldObj))
        obj=deleteAllFuncs(obj,cutF);

    int count1 = obj.count("+("+objIn+")")-obj.count("+("+objIn+")*")-obj.count("+("+objIn+")/")-obj.count("+("+objIn+")^");
    int count2 = obj.count("-("+objIn+")")-obj.count("-("+objIn+")*")-obj.count("-("+objIn+")/")-obj.count("-("+objIn+")^");

    int ind1=obj.indexOf("+("+objIn+")");
    int ind2=obj.indexOf("-("+objIn+")");
    if(ind1>=0 && ind1!=obj.indexOf("+("+objIn+")*") && ind1!=obj.indexOf("+("+objIn+")/") && ind1!=obj.indexOf("+("+objIn+")^")
            && ind2>=0 && ind2!=obj.indexOf("-("+objIn+")*") && ind2!=obj.indexOf("-("+objIn+")/") && ind2!=obj.indexOf("-("+objIn+")^"))
    {
        str=obj.remove(ind1,objIn.size()+3);
        if(ind2>=ind1) ind2 = ind2 -objIn.size()-3;
        str=obj.remove(ind2,objIn.size()+3);
        //qDebug()<<"STR:: "<<str;
        if(cutF.size()>1  || (cutF.size()==1 && cutF[0].second!=oldObj))
            for(int k=0;k<cutF.size();k++)
            {
                //qDebug()<<objects[j]<<" :: "<<tmpStr3<<" : "<<objects[i]<<"  ::  " <<ind<<" : "<<cutF[k].first;
                if(cutF[k].first>=ind1)
                {
                    cutF[k].first=cutF[k].first-objIn.size()-3;
                }
                if(cutF[k].first>=ind2)
                {
                    cutF[k].first=cutF[k].first-objIn.size()-3;
                }
            }
        //qDebug()<<str;
        count1 = str.count("+("+objIn+")")-str.count("+("+objIn+")*")-str.count("+("+objIn+")/")-str.count("+("+objIn+")^");
        count2 = str.count("-("+objIn+")")-str.count("-("+objIn+")*")-str.count("-("+objIn+")/")-str.count("-("+objIn+")^");
        //qDebug()<<"COUNT1:: "<<count1<<"COUNT2:: "<<count2;
        while(count1>0 && count2>0)
        {
            ind1=str.indexOf("+("+objIn+")");
            ind2=str.indexOf("-("+objIn+")");
            //qDebug()<<"IND1:: "<<ind1<<"IND2:: "<<ind2;
            if(ind1>=0 && ind1!=str.indexOf("+("+objIn+")*") && ind1!=str.indexOf("+("+objIn+")/") && ind1!=str.indexOf("+("+objIn+")^")
                    && ind2>=0 && ind2!=str.indexOf("-("+objIn+")*") && ind2!=str.indexOf("-("+objIn+")/") && ind2!=str.indexOf("-("+objIn+")^"))
            {
                str=str.remove(ind1,objIn.size()+3);
                if(ind2>=ind1) ind2 = ind2 -objIn.size()-3;
                str=str.remove(ind2,objIn.size()+3);
                for(int k=0;k<cutF.size();k++)
                {
                    if(cutF[k].first>=ind1)
                    {
                        cutF[k].first=cutF[k].first-objIn.size()-3;
                    }
                    if(cutF[k].first>=ind2)
                    {
                        cutF[k].first=cutF[k].first-objIn.size()-3;
                    }
                }
                count1 = str.count("+("+objIn+")")-str.count("+("+objIn+")*")-str.count("+("+objIn+")/")-str.count("+("+objIn+")^");
                count2 = str.count("-("+objIn+")")-str.count("-("+objIn+")*")-str.count("-("+objIn+")/")-str.count("-("+objIn+")^");
            }
        }

       if(cutF.size()>1 || (cutF.size()==1 && cutF[0].second!=oldObj))
            str=insertAllFuncs(str,cutF);
        //qDebug()<<" FINALY:: "<<str;
        //qDebug()<<" *************************************************** ";
    }
    return str;
}

QString Parser::plusMinusSameObjs(bool plus, QString obj, QString objIn, QVector<QPair<int, QString> > cutF)
{
    QString str="";
    QString oldObj=obj;
    if(plus)
    {
       // qDebug()<<obj<<" : "<<cutF.size();
        if((cutF.size()>1) || (cutF.size()==1 && cutF[0].second!=oldObj))
            obj=deleteAllFuncs(obj,cutF);

        int count = obj.count("+("+objIn+")")-obj.count("+("+objIn+")*")-obj.count("+("+objIn+")/")-obj.count("+("+objIn+")^");
        int constCount=count;
        int ind=obj.indexOf("+("+objIn+")");
        if(ind>=0 && ind!=obj.indexOf("+("+objIn+")*") && ind!=obj.indexOf("+("+objIn+")/") && ind!=obj.indexOf("+("+objIn+")^"))
        {
           // qDebug()<<obj<<"  !!  "<<objIn;
            str=obj.remove(ind,objIn.size()+3);
            if(cutF.size()>1  || (cutF.size()==1 && cutF[0].second!=oldObj))
                for(int k=0;k<cutF.size();k++)
                {
                    //qDebug()<<objects[j]<<" :: "<<tmpStr3<<" : "<<objects[i]<<"  ::  " <<ind<<" : "<<cutF[k].first;
                    if(cutF[k].first>=ind)
                    {
                        cutF[k].first=cutF[k].first-objIn.size()-3;
                    }
                }
            //qDebug()<<str;
            if(count>1)
            {
                count = str.count("+("+objIn+")")-str.count("+("+objIn+")*")-str.count("+("+objIn+")/")-str.count("+("+objIn+")^");

                ind=str.indexOf("+("+objIn+")");
                while(count>1)
                {
                   // qDebug()<<ind<<" :: "<<str<<" :: "<<objIn;
                    if(ind>=0 && ind!=str.indexOf("+("+objIn+")*") && ind!=obj.indexOf("+("+objIn+")/") && ind!=obj.indexOf("+("+objIn+")^"))
                    {
                        str=str.remove(ind,objIn.size()+3);
                        for(int k=0;k<cutF.size();k++)
                        {
                            if(cutF[k].first>=ind)
                            {
                                cutF[k].first=cutF[k].first-objIn.size()-3;
                            }
                        }
                        count = str.count("+("+objIn+")")-str.count("+("+objIn+")*")-str.count("+("+objIn+")/")-str.count("+("+objIn+")^");
                    }
                    else
                    {
                        if(ind>=0)
                            ind=str.indexOf("+("+objIn+")",str.indexOf("+("+objIn+")")+1);
                    }
                   // qDebug()<<ind<<" :: "<<str<<" :: "<<objIn;
                }
                ind=str.indexOf("+("+objIn+")");

                QString num;
                num.setNum(constCount);

                if(ind>=0 && ind!=str.indexOf("+("+objIn+")*") && ind!=obj.indexOf("+("+objIn+")/") && ind!=obj.indexOf("+("+objIn+")^"))
                    str=str.replace(ind,objIn.size()+3,"+"+num+"*("+objIn+")");
                for(int k=0;k<cutF.size();k++)
                {
                    if(cutF[k].first>=ind)
                    {
                        cutF[k].first=cutF[k].first+num.size()+1;
                    }
                }
               // qDebug()<<ind<<" :: "<<str<<" :: "<<objIn;
                //qDebug()<<objects[j]<<" :: "<<" : "<<tmpStr4<<" : "<<objects[i];
                if(cutF.size()>1 || (cutF.size()==1 && cutF[0].second!=oldObj))
                    str=insertAllFuncs(str,cutF);/*
                if(str.indexOf('+')!=0 && str[str.indexOf('+')-1]=='(')
                    str.remove(str.indexOf('+'),1);*/
              //  qDebug()<<" FINALY:: "<<str;
              //  qDebug()<<" *************************************************** ";
            }
            else
                str="";
        }
    }
    else
    {
        //qDebug()<<obj;
        if((cutF.size()>1) || (cutF.size()==1 && cutF[0].second!=oldObj))
            obj=deleteAllFuncs(obj,cutF);

        int count = obj.count("-("+objIn+")")-obj.count("-("+objIn+")*")-obj.count("-("+objIn+")/")-obj.count("-("+objIn+")^");
        int constCount=count;
        int ind=obj.indexOf("-("+objIn+")");
        if(ind>=0 && ind!=obj.indexOf("-("+objIn+")*") && ind!=obj.indexOf("-("+objIn+")/") && ind!=obj.indexOf("-("+objIn+")^"))
        {
          //  qDebug()<<obj;
            str=obj.remove(ind,objIn.size()+3);
            if((cutF.size()>1 ) || (cutF.size()==1 && cutF[0].second!=oldObj))
                for(int k=0;k<cutF.size();k++)
                {
                    //qDebug()<<objects[j]<<" :: "<<tmpStr3<<" : "<<objects[i]<<"  ::  " <<ind<<" : "<<cutF[k].first;
                    if(cutF[k].first>=ind)
                    {
                        cutF[k].first=cutF[k].first-objIn.size()-3;
                    }
                }
            //qDebug()<<str;
            if(count>1)
            {
                count = str.count("-("+objIn+")")-str.count("-("+objIn+")*")-str.count("-("+objIn+")/")-str.count("-("+objIn+")^");

                ind=str.indexOf("-("+objIn+")");
                while(count>1)
                {
                   // qDebug()<<ind<<" :: "<<str<<" :: "<<objIn;
                    if(ind>=0 && ind!=str.indexOf("-("+objIn+")*") && ind!=obj.indexOf("-("+objIn+")/") && ind!=obj.indexOf("-("+objIn+")^"))
                    {
                        str=str.remove(ind,objIn.size()+3);
                        for(int k=0;k<cutF.size();k++)
                        {
                            if(cutF[k].first>=ind)
                            {
                                cutF[k].first=cutF[k].first-objIn.size()-3;
                            }
                        }
                        count = str.count("-("+objIn+")")-str.count("-("+objIn+")*")-str.count("-("+objIn+")/")-str.count("-("+objIn+")^");
                    }
                    else
                    {
                        if(ind>=0)
                            ind=str.indexOf("-("+objIn+")",str.indexOf("-("+objIn+")")+1);
                    }

                   // qDebug()<<ind<<" :: "<<str<<" :: "<<objIn;
                }
                ind=str.indexOf("-("+objIn+")");

                QString num;
                num.setNum(constCount);

                if(ind>=0 && ind!=str.indexOf("-("+objIn+")*") && ind!=obj.indexOf("-("+objIn+")/") && ind!=obj.indexOf("-("+objIn+")^"))
                    str=str.replace(ind,objIn.size()+3,"-"+num+"*("+objIn+")");
                for(int k=0;k<cutF.size();k++)
                {
                    if(cutF[k].first>=ind)
                    {
                        cutF[k].first=cutF[k].first+num.size()+1;
                    }
                }
              //  qDebug()<<ind<<" :: "<<str<<" :: "<<objIn;
                //qDebug()<<objects[j]<<" :: "<<" : "<<tmpStr4<<" : "<<objects[i];
                if((cutF.size()>1) || (cutF.size()==1 && cutF[0].second!=oldObj))
                    str=insertAllFuncs(str,cutF);
               // qDebug()<<" FINALY:: "<<str;
               // qDebug()<<" *************************************************** ";
            }
            else
                str="";
        }
    }
    return str;
}

QString Parser::outOfScobsSameObjs(bool bef, QString obj, QString objIn, QVector<QPair<int, QString> > cutF)
{
  //  qDebug()<<"OBJ::"<<obj;
    QString str="";
    QString oldObj=obj;
    if(bef)
    {
        QString fullStr;
        if((cutF.size()>1) || (cutF.size()==1 && cutF[0].second!=oldObj))
            obj=deleteAllFuncs(obj,cutF);

        int count = obj.count("*("+objIn+")");
        int ind=obj.indexOf("*("+objIn+")");
        if(ind>=0)
        {
            str=obj.remove(ind,objIn.size()+3);
            QString strAroundInd = (cutAroundIndex(ind,str));
          //  qDebug()<<" STR:: "<<str<<" strAroundInd:: "<<strAroundInd<<" OBJIN:: "<<objIn<<" INDEX:: "<<ind;
            if(strAroundInd!="")
            {
                fullStr=fullStr+strAroundInd;
                //qDebug()<<" FULL:: "<<fullStr;
                str=str.remove(str.indexOf(strAroundInd,ind-strAroundInd.size()),strAroundInd.size());
            }
            if(cutF.size()>1  || (cutF.size()==1 && cutF[0].second!=oldObj))
                for(int k=0;k<cutF.size();k++)
                {
                    if(cutF[k].first>=ind)
                    {
                        cutF[k].first=cutF[k].first-objIn.size()-3-strAroundInd.size();
                    }
                }
            if(count>1)
            {
                count = str.count("*("+objIn+")");
                while(count>1)
                {
                    ind=str.indexOf("*("+objIn+")");
                    if(ind>=0)
                    {
                        str=str.remove(ind,objIn.size()+3);
                        strAroundInd = (cutAroundIndex(ind,str));
                      //  qDebug()<<" STR:: "<<str<<" strAroundInd:: "<<strAroundInd<<" OBJIN:: "<<objIn<<" INDEX:: "<<ind;
                        if(strAroundInd!="")
                        {
                            fullStr=fullStr+strAroundInd;
                            //qDebug()<<" FULL:: "<<fullStr;
                            str=str.remove(str.indexOf(strAroundInd,ind-strAroundInd.size()),strAroundInd.size());
                        }
                        for(int k=0;k<cutF.size();k++)
                        {
                            if(cutF[k].first>=ind)
                            {
                                cutF[k].first=cutF[k].first-objIn.size()-3-strAroundInd.size();
                            }
                        }
                        count = str.count("*("+objIn+")");
                    }
                }
                ind=str.indexOf("*("+objIn+")");
                if(ind>=0)
                {
                    str=str.remove(ind,objIn.size()+3);
                    strAroundInd = (cutAroundIndex(ind,str));
                  //  qDebug()<<" STR:: "<<str<<" strAroundInd:: "<<strAroundInd<<" OBJIN:: "<<objIn<<" INDEX:: "<<ind;
                    if(strAroundInd!="")
                    {
                        ind=str.indexOf(strAroundInd,ind-strAroundInd.size());
                        fullStr=fullStr+strAroundInd;
                       // qDebug()<<" FULL:: "<<fullStr;
                        str=str.remove(ind,strAroundInd.size());
                    }
                    fullStr="+("+objIn+")*("+fullStr+")";
                    str.insert(ind,fullStr);
                    for(int k=0;k<cutF.size();k++)
                    {
                        if(cutF[k].first>=ind)
                        {
                            cutF[k].first=cutF[k].first-objIn.size()-2-strAroundInd.size()+fullStr.size();
                        }
                    }
                }

                if(cutF.size()>1 || (cutF.size()==1 && cutF[0].second!=oldObj))
                    str=insertAllFuncs(str,cutF);
              //  qDebug()<<" FINALY:: "<<str;
            }
            else
                str="";
        }
    }
    else
    {
        QString fullStr;
        if((cutF.size()>1) || (cutF.size()==1 && cutF[0].second!=oldObj))
            obj=deleteAllFuncs(obj,cutF);

        int count = obj.count("("+objIn+")*");
        int ind=obj.indexOf("("+objIn+")*");
        if(ind>=0)
        {
            str=obj.remove(ind,objIn.size()+3);
            QString strAroundInd = (cutAroundIndex(ind,str));
            if(strAroundInd!="")
            {
                fullStr=fullStr+strAroundInd;
                //qDebug()<<" FULL:: "<<fullStr;
               // qDebug()<<" STR:: "<<str<<" strAroundInd:: "<<strAroundInd<<" INDEX:: "<<str.indexOf(strAroundInd,ind-1);
                if(ind>=strAroundInd.size())
                    str=str.remove(str.indexOf(strAroundInd,ind-strAroundInd.size()-1),strAroundInd.size());
                else
                    str=str.remove(str.indexOf(strAroundInd,ind-1),strAroundInd.size());
              //  qDebug()<<" STR:: "<<str;
            }
            if(cutF.size()>1  || (cutF.size()==1 && cutF[0].second!=oldObj))
                for(int k=0;k<cutF.size();k++)
                {
                    //qDebug()<<objects[j]<<" :: "<<tmpStr3<<" : "<<objects[i]<<"  ::  " <<ind<<" : "<<cutF[k].first;
                    if(cutF[k].first>=ind)
                    {
                        cutF[k].first=cutF[k].first-objIn.size()-3-strAroundInd.size();
                    }
                }
           // qDebug()<<" STR:: "<<str;
            if(count>1)
            {
                count = str.count("("+objIn+")*");
                while(count>1)
                {
                    ind=str.indexOf("("+objIn+")*");
                    if(ind>=0)
                    {
                        //qDebug()<<" OLD_STR:: "<<str<<" INDEX:: "<<ind<<" SIZE:: "<<objIn.size()+3;
                        str=str.remove(ind,objIn.size()+3);
                        //qDebug()<<" NEW_STR:: "<<str;
                        strAroundInd = (cutAroundIndex(ind,str));
                        if(strAroundInd!="")
                        {
                            fullStr=fullStr+strAroundInd;
                           // qDebug()<<" FULL:: "<<fullStr;
                            if(ind>=strAroundInd.size())
                                str=str.remove(str.indexOf(strAroundInd,ind-1-strAroundInd.size()),strAroundInd.size());
                            else
                                str=str.remove(str.indexOf(strAroundInd,ind-1),strAroundInd.size());
                           // qDebug()<<" STR:: "<<str;
                        }
                        for(int k=0;k<cutF.size();k++)
                        {
                            if(cutF[k].first>=ind)
                            {
                                cutF[k].first=cutF[k].first-objIn.size()-3-strAroundInd.size();
                            }
                        }
                        count = str.count("("+objIn+")*");
                    }
                }
                ind=str.indexOf("("+objIn+")*");
                //qDebug()<<" INDEX:: "<<ind;
                if(ind>=0)
                {
                    str=str.remove(ind,objIn.size()+3);
                    strAroundInd = (cutAroundIndex(ind,str));
                    if(strAroundInd!="")
                    {
                        if(ind>=strAroundInd.size())
                            ind=str.indexOf(strAroundInd,ind-1-strAroundInd.size());
                        else
                            ind=str.indexOf(strAroundInd,ind-1);
                        fullStr=fullStr+strAroundInd;
                      //  qDebug()<<" FULL:: "<<fullStr;
                        str=str.remove(ind,strAroundInd.size());
                     //   qDebug()<<" STR:: "<<str;
                    }
                    fullStr="+("+objIn+")*("+fullStr+")";
                    str.insert(ind,fullStr);
                    for(int k=0;k<cutF.size();k++)
                    {
                        if(cutF[k].first>=ind)
                        {
                            cutF[k].first=cutF[k].first-objIn.size()-2-strAroundInd.size()+fullStr.size();
                        }
                    }
                }

                //qDebug()<<ind<<" :: "<<str<<" :: "<<objIn;
                if(cutF.size()>1 || (cutF.size()==1 && cutF[0].second!=oldObj))
                    str=insertAllFuncs(str,cutF);
               // qDebug()<<" FINALY:: "<<str;
               // qDebug()<<" *************************************************** ";
            }
            else
                str="";
        }
    }

   // qDebug()<<"STR::"<<str;
    return str;
}

QString Parser::multTwoObjs(QString origObj, QString obj1, QString obj2)
{
    QVector<QString> objs1 = whatObj(obj1);
    QVector<QString> objs2 = whatObj(obj2);
    QString str=origObj;
    QString tmpStr="";
    if(objs1.size()>1 && objs2.size()>1)
    {
        //qDebug()<<"OBJ1::"<<obj1<<"OBJS1::"<<objs1;
       // qDebug()<<"OBJ2::"<<obj2<<"OBJS2::"<<objs2;
        for(int i=0;i<objs1.size();i++)
        {
            for(int j=0;j<objs2.size();j++)
            {
                if(objs1[i].size()>0 && objs2[j].size()>0)
                {
                    QString objs1Obj = objs1[i];
                    QString objs2Obj = objs2[j];
                    if(objs1[i][0]=='+' && objs2[j][0]=='-')
                        objs1Obj[0]='-';
                    if(objs1[i][0]=='-' && objs2[j][0]=='-')
                        objs1Obj[0]='+';
                    if(objs2[j][0]=='-' || objs2[j][0]=='+')
                        objs2Obj.remove(0,1);
                    tmpStr=tmpStr+objs1Obj+"*"+objs2Obj;
                }
            }
           // qDebug()<<"*******TMPSTR::"<<tmpStr;
        }
    }
    QString multObjStr=obj1+"*"+obj2;
    int ind=str.indexOf(multObjStr);
    //qDebug()<<"IND::"<<ind;
    if(ind>=0 && tmpStr!="")
        str=str.replace(ind,multObjStr.size(),tmpStr);
    else
        str=tmpStr;
    str="("+str+")";
    while(str.contains("()"))
        str=deleteScobs(str);
   // while(str.contains("++"))
        //str=deleteDoublePM(str);
    return str;
}


QString Parser::plusTwoObjs(QString origObj, QString obj1, QString obj2)
{
    QString or_obj1 = obj1;
    QString or_obj2 = obj2;
    QVector<QString> objs1 = whatObj(obj1);
    QVector<QString> objs2 = whatObj(obj2);
    QString str=origObj;
    QString tmpStr="";
    if(objs1.size()>1 && objs2.size()>1)
    {/*
        qDebug()<<"OBJ1::"<<obj1<<"OBJS1::"<<objs1;
        qDebug()<<"OBJ2::"<<obj2<<"OBJS2::"<<objs2;*/
        if(origObj[origObj.indexOf(obj1)-1]=='+')
        {
            tmpStr=obj1.remove(obj1.size()-1,1).remove(0,1);
        }
        else
            if(origObj[origObj.indexOf(obj1)-1]=='-')
            {
                for(int i=0;i<objs1.size();i++)
                {
                    if(objs1[i].size()>0)
                    {
                        QString objs1Obj = objs1[i];
                        if(objs1[i][0]=='+')
                            objs1Obj[0]='-';
                        if(objs1[i][0]=='-')
                            objs1Obj[0]='+';
                        tmpStr=tmpStr+objs1Obj;
                    }
                }
            }

        if(origObj[origObj.indexOf(or_obj2)+or_obj2.size()]!='*' && (origObj[origObj.indexOf(or_obj1)-1]=='+' || origObj[origObj.indexOf(or_obj1)-1]=='-'))
        {
            if(origObj[origObj.indexOf(obj2)-1]=='+')
            {
                tmpStr=tmpStr+obj2.remove(obj2.size()-1,1).remove(0,1);
                //qDebug()<<"ORIG"<<origObj<<"TMPSTR::"<<tmpStr;
            }
            else
                if(origObj[origObj.indexOf(obj2)-1]=='-')
                {
                    for(int i=0;i<objs2.size();i++)
                    {
                        if(objs2[i].size()>0)
                        {
                            QString objs2Obj = objs2[i];
                            if(objs2[i][0]=='+')
                                objs2Obj[0]='-';
                            if(objs2[i][0]=='-')
                                objs2Obj[0]='+';
                            tmpStr=tmpStr+objs2Obj;
                        }
                    }
                }
        }
        //qDebug()<<"IND::"<<ind;
        if(origObj[origObj.indexOf(or_obj2)-1]=='+')
        {
            QString plusObjStr=or_obj1+"+"+or_obj2;
            int ind1=str.indexOf(plusObjStr);
            if(ind1>=0 && tmpStr!="")
                str=str.replace(ind1,plusObjStr.size(),tmpStr);
            else
                str=tmpStr;
        }
        else
            if(origObj[origObj.indexOf(or_obj2)-1]=='-')
            {
                QString minusObjStr=or_obj1+"-"+or_obj2;
                int ind2=str.indexOf(minusObjStr);
                if(ind2>=0 && tmpStr!="")
                    str=str.replace(ind2,minusObjStr.size(),tmpStr);
                else
                    str=tmpStr;
            }
        while(str.contains("()"))
            str=deleteScobs(str);
    }
    return str;
}


QString Parser::cutAroundIndex(int index, QString obj)
{
    QString str="";
    QRegExp re("[A-Z,А-Я,0-9]");
    if(obj!="")
    {
        QString tmpStr="";
        int i=index-1;
        str=obj.mid(0,index);
        while(i>=0  && i<str.size() && (countLeftScobs(tmpStr)!=countRightScobs(tmpStr) || tmpStr.size()==0))
        {
            tmpStr=str[i]+tmpStr;
            i--;
        }
        if(i>=0 && (str[i]=='+' || str[i]=='-'))
            tmpStr=str[i]+tmpStr;

        //qDebug()<<"FIRST PART OF TMPSTR<<"<<tmpStr;
        str="";
        str+=tmpStr;
        i=index;
        tmpStr="";
        while(i<obj.size() && i>=0 && (countLeftScobs(tmpStr)!=countRightScobs(tmpStr) || tmpStr.size()==0))
        {
            tmpStr+=obj[i];
            i++;
        }
        //qDebug()<<"SECOND PART OF TMPSTR<<"<<tmpStr;
        if(tmpStr.contains(re) && tmpStr!="+" && tmpStr!="-")
            str+=tmpStr;
    }
    if(str.contains(re) && str!="+" && str!="-")
        return str;
    else
        return "";
}

QString Parser::deleteScobs(QString str)
{
    if(str.contains("*()"))
        str.remove("*()");
    if(str.contains("()*"))
        str.remove("()*");
    if(str.contains("()/"))
        str.remove("()/");
    if(str.contains("()"))
        str.remove("()");
    return str;
}


QString Parser::deleteDoublePM(QString str)
{
    //qDebug()<<"1::"<<str;
    if(str.contains("++"))
        str=str.replace("++","+");
    //qDebug()<<"2::"<<str;
    if(str.contains("--"))
        str=str.replace("--","-");
    //qDebug()<<"3::"<<str;
    if(str.contains("+-"))
        str=str.replace("+-","-");
   // qDebug()<<"4::"<<str;
    if(str.contains("-+"))
        str=str.replace("-+","-");
   // qDebug()<<"5::"<<str;
  //  qDebug()<<"======================================";
    return str;
}

QString Parser::deletePlusesAndScobes(QString obj1, QString obj2)
{
    while(obj1.contains("(+("+obj2+"))")) //Убираем знак + ,там где он не нужен
    {
        QString tmpStr1=obj1;
        tmpStr1=obj1.replace("(+("+obj2+"))","("+obj2+")");
        if(tmpStr1!="")
        {
            obj1=tmpStr1;
        }
    }
    while(obj1.contains("(("+obj2+"))")) //Убираем знак + ,там где он не нужен
    {
        QString tmpStr1=obj1;
        tmpStr1=obj1.replace("(("+obj2+"))","("+obj2+")");
        if(tmpStr1!="")
        {
            obj1=tmpStr1;
        }
    }
    return obj1;
}

int Parser::countLeftScobs(QString str)
{
    int count =0 ;
    for(int i=0;i<str.size();i++)
    {
        if(str[i]=='(')
            count++;
    }
    return count;
}

int Parser::countRightScobs(QString str)
{
    int count =0 ;
    for(int i=0;i<str.size();i++)
    {
        if(str[i]==')')
            count++;
    }
    return count;
}

bool Parser::isSameNewObjects(QString str)
{
    QString tmpStr = str;
    QString tmpStr2;
    tmpStr = tmpStr.remove(0,1);
    tmpStr = tmpStr.remove(tmpStr.size()-1,1);
    tmpStr2 = tmpStr;
    if(tmpStr2[0]=='+' && tmpStr2[tmpStr2.size()-1]==')')
    {
        tmpStr2 = tmpStr2.remove(0,1);
        tmpStr2 = tmpStr2.remove(tmpStr2.size()-1,1);
    }
    for(int i=1;i<=newObjects.size();i++)
    {
        if(str==newObjects[i])
            return true;
        if(tmpStr==newObjects[i])
            return true;
        if(tmpStr2==newObjects[i])
            return true;
    }
    return false;
}

bool Parser::isSameRelations(Relation rel)
{
    for(int i=0;i<newRelations.size();i++)
    {
        if(rel.from==newRelations[i].from && rel.to==newRelations[i].to && rel.type==newRelations[i].type)
            return true;
    }
    return false;
}

bool Parser::lieOutOfFunc(QString mainObj, QString obj, QVector<QPair<int, QString> >funcs)
{
    for(int i=0;i<funcs.size();i++)
    {
        if(mainObj.indexOf(obj)>=funcs[i].first && mainObj.indexOf(obj)<=funcs[i].first+ funcs[i].second.size())
            return false;
    }
    return true;
}

QVector<QString> Parser::whatObj(QString str)
{
    QVector<QString>  objs;
    QString tmpStr="";
    int i=1;
    //qDebug()<<str;
    while(i<str.size())
    {
        bool flag=false;
        tmpStr="";
        while(i<str.size()  && (countLeftScobs(tmpStr)!=countRightScobs(tmpStr) || (tmpStr.size()<=1 && str[i]!=')')
                                || str[i]=='/' || str[i]=='*' || str[i]=='^'
                                || (i!=0 && (str[i-1]=='^' || str[i-1]=='*' || str[i-1]=='/'))))
        {
            tmpStr=tmpStr+str[i];
            flag=true;
            i++;
        }

//        if(countLeftScobs(tmpStr)>countRightScobs(tmpStr))
//        {
//            while(countLeftScobs(tmpStr)!=countRightScobs(tmpStr))
//                tmpStr.remove(tmpStr.indexOf("("),1);
//        }
//        if(countLeftScobs(tmpStr)<countRightScobs(tmpStr))
//        {
//            while(countLeftScobs(tmpStr)!=countRightScobs(tmpStr))
//                tmpStr.remove(tmpStr.indexOf(")"),1);
//        }
//        D:\\Qt\\Qt5.1.1\\Tools\\QtCreator\\bin\\ProductionSystem\\test.txt
        if(tmpStr!="")
        {
            //if(tmpStr[0]=='+')
                //tmpStr.remove(0,1);
//            if(tmpStr[0]!='(' || tmpStr[tmpStr.size()-1]!=')')
//            {
//                tmpStr="("+tmpStr+")";
//                //qDebug()<<tmpStr;
//            }
            objs.push_back(tmpStr);
        }
        if(!flag)
            i++;
    }
    return objs;
}

int Parser::countNewObjects()
{
    int oldSize=newObjects.size();
    int size=oldSize;
    for(int i(1);i<=objects.size();i++)
    {
        if(objects[i]==newObjects[i])
        {
            size--;
        }
    }

    return oldSize-size;

}

QMap<int, QString> Parser::getNewObjects()
{
    return newObjects;
}

QMap<int, QString> Parser::getOldObjects()
{
    return objects;
}

QVector<Relation> Parser::getNewRelations()
{
    return newRelations;
}

