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

Game::BoardData fileToBoard(QString fileName)
{
    Game::BoardData rtn;
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();
        while(data.size() != 0)
        {
            auto ch = data.at(0);
            data.remove(0, 1);
            switch(ch)
            {
            case ' ':
                rtn.append(Game::Piece::Empty);
                break;
            case 'P':
                rtn.append(Game::Piece::Player);
                break;
            case 'K':
                rtn.append(Game::Piece::King);
                break;
            case 'O':
                if(data.at(0) == 'P')
                {
                    rtn.append(Game::Piece::OpPlayer);
                }
                else
                {
                    rtn.append(Game::Piece::OpKing);
                }
                data.remove(0, 1);
                break;
            }
        }
        file.close();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText(QStringLiteral("Error: Could not open file."));
    }
    return rtn;
}

QString _printMoveTreeInternal(Game::MovePointer move, int iteration = 0)
{
    QString rtn;
    rtn.fill(' ', (iteration + 1) * 4);
    rtn.append(directionToText(move->direction));
    if(!move->concatenatedMove.isNull())
    {
        rtn.append(QStringLiteral("\n")).append(_printMoveTreeInternal(move->concatenatedMove, iteration + 1));
    }
    return rtn;
}

QString printMoveTree(QVector<Game::MovePointer> moves)
{
    QMap<QString, QVector<Game::MovePointer>> mvOrd;
    for(auto &move : moves)
    {
        mvOrd[move->cell.toQString()].append(move);
    }

    QString rtn;
    QMap<QString, QVector<Game::MovePointer>>::const_iterator i = mvOrd.constBegin();
    while(i != mvOrd.constEnd())
    {
        rtn.append(QStringLiteral("Moves for ")).append(i.key()).append(QStringLiteral(":"));
        for(auto &move : i.value())
        {
            rtn.append(QStringLiteral("\n")).append(_printMoveTreeInternal(move));
        }
        rtn.append(QStringLiteral("\n"));
        ++i;
    }
    return rtn;
}

QString directionToText(Game::DirectionToken direction)
{
    switch(direction)
    {
    case Game::DirectionToken::FLeft:
        return QStringLiteral("FLeft");
        break;
    case Game::DirectionToken::FRight:
        return QStringLiteral("FRight");
        break;
    case Game::DirectionToken::BLeft:
        return QStringLiteral("BLeft");
        break;
    case Game::DirectionToken::BRight:
        return QStringLiteral("BRight");
        break;
    case Game::DirectionToken::JFLeft:
        return QStringLiteral("JFLeft");
        break;
    case Game::DirectionToken::JFRight:
        return QStringLiteral("JFRight");
        break;
    case Game::DirectionToken::JBLeft:
        return QStringLiteral("JBLeft");
        break;
    case Game::DirectionToken::JBRight:
        return QStringLiteral("JBRight");
        break;
    }
}
