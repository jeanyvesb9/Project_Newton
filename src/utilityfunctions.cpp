#include "utilityfunctions.h"

int querySize(QSqlQuery *qry)
{
    int numberOfRows = 0;
    if(qry->last())
    {
        numberOfRows =  qry->at() + 1;
        qry->first();
        qry->previous();
    }
    return numberOfRows;
}

QString longDoubleToQString(long double num)
{
    std::stringstream ss;
    ss << num;
    return QString::fromStdString(ss.str());
}
