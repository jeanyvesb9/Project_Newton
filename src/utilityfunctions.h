#ifndef UTILITYFUNCTIONS_H
#define UTILITYFUNCTIONS_H

#include <QSqlQuery>
#include <QString>

#include <string>
#include <sstream>

int querySize(QSqlQuery *qry);
inline long double QStringToLongDouble(const QString *str) { return std::stold(str->toStdString()); }
inline long double QStringToLongDouble(const QString &str) { return std::stold(str.toStdString()); }
QString longDoubleToQString(long double num);

#endif // UTILITYFUNCTIONS_H
