#ifndef UTILITYFUNCTIONS_H
#define UTILITYFUNCTIONS_H

#include <QSqlQuery>
#include <QString>
#include <QMap>
#include <QFileDialog>
#include <QMessageBox>

#include <string>
#include <sstream>

#include "src/game/gamestructs.h"

int querySize(QSqlQuery *qry);
inline long double QStringToLongDouble(const QString *str) { return std::stold(str->toStdString()); }
inline long double QStringToLongDouble(const QString &str) { return std::stold(str.toStdString()); }
QString longDoubleToQString(long double num);

Game::BoardData fileToBoard(QString fileName);
QString printMoveTree(QVector<Game::MovePointer> moves);
QString directionToText(Game::DirectionToken direction);

#endif // UTILITYFUNCTIONS_H
