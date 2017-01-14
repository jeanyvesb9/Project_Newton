#include "gamefile.h"

Game::GameFile::GameFile()
{

}

Game::GameFile::GameFile(QString file)
    : fileName{file}
{
    db = QSqlDatabase::addDatabase("QSQLITE", "gameDb");
    db.setDatabaseName(file);
    if(!db.open())
    {
        valid = false;
        //TODO: RAISE EXCEPTION
    }
    valid = loadData();
}

Game::GameFile::~GameFile()
{
    if(db.isOpen())
    {
        saveAndClose();
    }
}

QString Game::GameFile::getFileName() const
{
    return fileName;
}

bool Game::GameFile::saveAndClose()
{
    db.close();
    db = QSqlDatabase();
    QSqlDatabase::removeDatabase("gameDb");
    valid = false;
    return true;
}

void Game::GameFile::finishGame(Player winningPlayer)
{
    activePlayer = Player::None;
    gameStatus = winningPlayer == Player::Player1 ? GameStatus::Player1Won : GameStatus::Player2Won;

    QSqlQuery query(db);
    query.prepare(QStringLiteral("UPDATE generalData SET ActivePlayer = :activePlayer, GameStatus = :gameStatus"));
    query.bindValue(QStringLiteral(":activePlayer"), static_cast<int>(activePlayer));
    query.bindValue(QStringLiteral(":gameStatus"), static_cast<int>(gameStatus));
    if(!query.exec())
    {
        //TODO: RAISE EXCEPTION
    }
}

bool Game::GameFile::hasFinished() const
{
    return gameStatus != GameStatus::Playing;
}

Player Game::GameFile::getWinnerPlayer() const
{
    switch(gameStatus)
    {
    case GameStatus::Playing:
        return Player::None;
    case GameStatus::Player1Won:
        return Player::Player1;
    case GameStatus::Player2Won:
        return Player::Player2;
    }
}

QString Game::GameFile::getPlayerName() const
{
    return playerName;
}

quint64 Game::GameFile::getDateCreated() const
{
    return dateCreated;
}

quint8 Game::GameFile::getDifficulty() const
{
    return difficulty;
}

void Game::GameFile::addPlayTime(quint64 time)
{
    setPlayTime(gameTime + time);
}

void Game::GameFile::setPlayTime(quint64 time)
{
    gameTime = time;
    QSqlQuery query(db);
    query.prepare(QStringLiteral("UPDATE generalData SET GameTime = :time;"));
    query.bindValue(QStringLiteral(":time"), gameTime);
    if(!query.exec())
    {
        //TODO: RAISE EXCEPTION
    }
}

quint64 Game::GameFile::getPlayTime() const
{
    return gameTime;
}

Game::BoardData Game::GameFile::getBoardData() const
{
    return board;
}

void Game::GameFile::addMove(PlayerMovePointer move, Game::BoardData newBoard)
{
    QSqlQuery query(db);
    query.prepare(QStringLiteral("INSERT INTO historyOfPlays (Time, Player, Move, Crowned) VALUES (:time, :player, :move, :crowned)"));
    query.bindValue(QStringLiteral(":time"), move->time);
    query.bindValue(QStringLiteral(":player"), static_cast<int>(move->player));
    query.bindValue(QStringLiteral(":move"), __addMove(move->move));
    query.bindValue(QStringLiteral(":crowned"), (move->boardMdf->newCrownedCell.isNull() ? QVariant() : move->boardMdf->newCrownedCell->toCellNum()));
    if(!query.exec())
    {
        //TODO: RAISE EXCEPTION
    }
    query.clear();
    if(!query.exec(QStringLiteral("SELECT last_insert_rowid()")))
    {
        //TODO: RAISE EXCEPTION
    }
    query.next();
    int id = query.value(0).toInt();
    query.clear();
    for(auto &rmc : move->boardMdf->removedCells)
    {
        query.prepare(QStringLiteral("INSERT INTO removedCells (Play, Cell) VALUES (:play, :cell)"));
        query.bindValue(QStringLiteral(":play"), id);
        query.bindValue(QStringLiteral(":cell"), rmc.toCellNum());
        if(!query.exec())
        {
            //TODO: RAISE EXCEPTION
        }
        query.clear();
    }

    this->activePlayer = move->player == Player::Player1 ? Player::Player2 : Player::Player1;
    this->board = newBoard;
    query.prepare(QStringLiteral("UPDATE generalData SET ActivePlayer = :activePlayer, Board = :board"));
    query.bindValue(QStringLiteral(":activePlayer"), static_cast<int>(this->activePlayer));
    query.bindValue(QStringLiteral(":board"), boardDataToString(newBoard));
    if(!query.exec())
    {
        //TODO: RAISE EXCEPTION
    }
    currentMoveNumber++;
}

quint64 Game::GameFile::__addMove(Game::MovePointer move) const
{
    QSqlQuery query(db);
    query.prepare(QStringLiteral("INSERT INTO moves (CellNumber, Direction, ConcatenatedMove) VALUES (:cell, :direction, :concatenatedMove)"));
    query.bindValue(QStringLiteral(":cell"), move->cell.toCellNum());
    query.bindValue(QStringLiteral(":direction"), static_cast<int>(move->direction));
    query.bindValue(QStringLiteral(":concatenatedMove"), (move->concatenatedMove.isNull() ? QVariant() : __addMove(move->concatenatedMove)));
    if(!query.exec())
    {
        //TODO: RAISE EXCEPTION
    }
    query.clear();
    if(!query.exec(QStringLiteral("SELECT last_insert_rowid()")))
    {
        //TODO: RAISE EXCEPTION
    }
    query.next();
    return query.value(0).toInt();
}

QVector<PlayerMovePointer> Game::GameFile::getMoves() const
{
    QVector<PlayerMovePointer> ret;
    QSqlQuery query(db);
    if(!query.exec(QStringLiteral("SELECT * FROM historyOfPlays")))
    {
        //TODO: RAISE EXCEPTION
    }
    while(query.next())
    {
        PlayerMovePointer ptr(new PlayerMove());
        ptr->player = static_cast<Player>(query.value(QStringLiteral("Player")).toInt());
        ptr->time = query.value(QStringLiteral("Time")).toULongLong();
        ptr->boardMdf = BoardModificationPointer(new BoardModification());
        if(!query.value(QStringLiteral("Crowned")).isNull())
        {
            CellPointer cptr(new Cell());
            *cptr = Cell::fromNum(query.value(QStringLiteral("Crowned")).toUInt());
            ptr->boardMdf->newCrownedCell = cptr;
        }
        QSqlQuery query2(db);
        query2.prepare(QStringLiteral("SELECT Cell FROM removedCells WHERE Play == :id"));
        query2.bindValue(QStringLiteral(":id"), query.value(QStringLiteral("Id")).toInt());
        if(!query2.exec())
        {
            //TODO: RAISE EXCEPTION
        }
        while(query2.next())
        {
            ptr->boardMdf->removedCells.append(Cell::fromNum(query2.value(QStringLiteral("Cell")).toUInt()));
        }
        ptr->move = __getMove(query.value(QStringLiteral("Move")).toULongLong());
    }
    return ret;
}

Game::MovePointer Game::GameFile::__getMove(quint64 id) const
{
    Game::MovePointer ptr(new Game::Move());
    QSqlQuery query(db);
    query.prepare(QStringLiteral("SELECT * FROM moves WHERE Id == :id"));
    query.bindValue(QStringLiteral(":id"), id);
    if(!query.exec())
    {
        //TODO: RAISE EXCEPTION
    }
    query.next();
    ptr->cell = Cell::fromNum(query.value(QStringLiteral("CellNumber")).toUInt());
    ptr->direction = static_cast<Game::DirectionToken>(query.value(QStringLiteral("Direction")).toInt());
    if(!query.value(QStringLiteral("ConcatenatedMove")).isNull())
        ptr->concatenatedMove = __getMove(query.value(QStringLiteral("ConcatenatedMove")).toULongLong());
    return ptr;
}

quint64 Game::GameFile::getCurrentMoveNumber() const
{
    return currentMoveNumber;
}

Player Game::GameFile::getActivePlayer() const
{
    return activePlayer;
}

Game::GameFilePointer Game::GameFile::createNewGame(QString file, QString name, quint8 difficulty)
{
    GameFilePointer ptr(new GameFile());
    ptr->fileName = file;
    ptr->valid = false;
    QSqlDatabase &db = ptr->db;
    db = QSqlDatabase::addDatabase("QSQLITE", "gameDb");
    db.setDatabaseName(file);
    if(!db.open())
    {
        //TODO: RAISE EXCEPTION
    }

    QSqlQuery query(db);
    if(!query.exec(QStringLiteral("CREATE TABLE generalData (PlayerName TEXT NOT NULL DEFAULT Player, DateCreated INTEGER NOT NULL, GameTime INTEGER NOT NULL, Difficulty INTEGER NOT NULL DEFAULT (0), ActivePlayer INTEGER NOT NULL DEFAULT (1), Board TEXT NOT NULL, GameStatus INTEGER NOT NULL);")))
    {
        //TODO: RAISE EXCEPTION
    }
    query.clear();
    if(!query.exec(QStringLiteral("CREATE TABLE moves (Id INTEGER PRIMARY KEY AUTOINCREMENT, CellNumber INTEGER NOT NULL, Direction INTEGER NOT NULL, ConcatenatedMove INTEGER REFERENCES moves (Id));")))
    {
        //TODO: RAISE EXCEPTION
    }
    query.clear();
    if(!query.exec(QStringLiteral("CREATE TABLE historyOfPlays (Id INTEGER PRIMARY KEY AUTOINCREMENT, Time INTEGER NOT NULL, Player INTEGER NOT NULL, Move INTEGER NOT NULL REFERENCES moves (Id), Crowned INTEGER);")))
    {
        //TODO: RAISE EXCEPTION
    }
    query.clear();
    if(!query.exec(QStringLiteral("CREATE TABLE removedCells (Play INTEGER REFERENCES historyOfPlays (Id), Cell INTEGER, PRIMARY KEY (Play, Cell));")))
    {
        //TODO: RAISE EXCEPTION
    }
    query.clear();

    ptr->playerName = name;
    ptr->dateCreated = QDateTime::currentMSecsSinceEpoch();
    ptr->gameTime = 0;
    ptr->difficulty = difficulty;
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(1, 2);
    ptr->activePlayer = static_cast<Player>(uni(rng));
    ptr->board = Game::Board::defaultBoard()->getBoardData();
    ptr->currentMoveNumber = 1;

    query.prepare(QStringLiteral("INSERT INTO generalData(PlayerName, DateCreated, GameTime, Difficulty, ActivePlayer, Board, GameStatus) VALUES (:name, :dateCreated, 0, :difficulty, :activePlayer, :board, :gameStatus);"));
    query.bindValue(QStringLiteral(":name"), ptr->playerName);
    query.bindValue(QStringLiteral(":dateCreated"), ptr->dateCreated);
    query.bindValue(QStringLiteral(":difficulty"), ptr->difficulty);
    query.bindValue(QStringLiteral(":activePlayer"), static_cast<int>(ptr->activePlayer));
    query.bindValue(QStringLiteral(":board"), boardDataToString(ptr->board));
    query.bindValue(QStringLiteral(":gameStatus"), static_cast<int>(GameStatus::Playing));
    if(!query.exec())
    {
        //TODO: RAISE EXCEPTION
    }

    ptr->valid = true;
    return ptr;
}

bool Game::GameFile::loadData()
{
    QSqlQuery query(db);
    if(!query.exec(QStringLiteral("SELECT PlayerName, DateCreated, GameTime, Difficulty, ActivePlayer, Board, GameStatus FROM generalData;")) || !query.next())
    {
        //TODO: RAISE EXCEPTION;
        return false;
    }

    playerName = query.value(QStringLiteral("PlayerName")).toString();
    dateCreated = query.value(QStringLiteral("DateCreated")).toULongLong();
    gameTime = query.value(QStringLiteral("GameTime")).toULongLong();
    difficulty = static_cast<quint8>(query.value(QStringLiteral("Difficulty")).toUInt());
    activePlayer = static_cast<Player>(query.value(QStringLiteral("ActivePlayer")).toInt());
    board = stringToBoardData(query.value(QStringLiteral("Board")).toString());
    gameStatus = static_cast<GameStatus>(query.value(QStringLiteral("GameStatus")).toInt());
    query.clear();
    if(!query.exec(QStringLiteral("SELECT COUNT(Id) FROM historyOfPlays")) || !query.next())
    {
        return false;
        //TODO: RAISE EXCEPTION
    }
    currentMoveNumber = query.value(0).toULongLong() + 1;
    return true;
}

Game::BoardData Game::GameFile::stringToBoardData(QString raw)
{
    Game::BoardData rtn;
    for(auto &c : raw)
    {
        rtn.append(static_cast<Game::Piece>(QString(c).toInt()));
    }
    return rtn;
}

QString Game::GameFile::boardDataToString(Game::BoardData board)
{
    QString rtn;
    for(auto &p : board)
    {
        rtn.append(QString::number(static_cast<int>(p)));
    }
    return rtn;
}
