#include "boardwidget.h"

BoardPieceWidget::BoardPieceWidget(int id, QWidget *parent) : QWidget(parent),
    id{id}, state{State::Hidden}
{

}

void BoardPieceWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    switch(state)
    {
    case State::Hidden:
        return;
    case State::Player:
    case State::King:
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0xFF, 0xFF, 0xFF, 0xFF));
        break;
    case State::OpPlayer:
    case State::OpKing:
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0x00, 0x00, 0x00, 0xFF));
        break;
    case State::Path:
        painter.setPen(QPen(QColor(0x00, 0x00, 0x00, 0xFF), 1, Qt::PenStyle::DashLine));
        painter.setBrush(QColor(0xBB, 0xBB, 0xBB, 0xFF));
        break;
    }
    painter.drawEllipse(QRectF(0.0, 0.0, width(), height()));
    if(state == State::King || state == State::OpKing)
    {
        painter.drawPixmap(width() * 0.2, height() * 0.2, width() * 0.6, height() * 0.6, QIcon(":/assets/images/crown.svg").pixmap(QSize(width() * 0.6, height() * 0.6)));
    }
}

void BoardPieceWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit clicked(id);
    }
}

void BoardPieceWidget::setState(BoardPieceWidget::State state)
{
    this->state = state;
}

//------------------------------------------------------------------------------------------------------------------

BoardWidget::BoardWidget(QWidget *parent) : QWidget(parent),
    boardValid{false}
{
    for(int i = 0; i < 32; i++)
    {
        pieces.append(new BoardPieceWidget(i, this));
        pieces.last()->setState(BoardPieceWidget::Player);
        connect(pieces.last(), &BoardPieceWidget::clicked, this, &BoardWidget::cellClicked);
    }
}

void BoardWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if(boardValid)
    {
        painter.setBrush(QColor(0xFF, 0xFF, 0xFF, 0xFF));
        painter.setPen(QPen(QColor(0x93, 0x95, 0x98, 0xFF), 1));
        painter.drawRect(QRectF(0.0, 0.0, width(), height()));
        painter.setBrush(QColor(0x93, 0x95, 0x98, 0xFF));
        painter.setPen(Qt::NoPen);
        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                if((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1))
                {
                    painter.drawRect(QRectF((width() / 8.0) * j, (height() / 8.0) * i, width() / 8.0, height() / 8.0));
                }
            }
        }

        float cellSize = width() / 8.0;
        float pieceSize = cellSize * 7.0/9.0;
        float margin = (cellSize - pieceSize) / 2.0;
        for(int i = 0; i < 32; i++)
        {
            pieces.at(i)->resize(pieceSize, pieceSize);
            pieces.at(i)->move(1 + cellSize * ((i % 4) * 2) + cellSize * ((i / 4) % 2) + margin, 1 + cellSize * (i / 4) + margin);

            Game::Piece p = board.at(i);
            BoardPieceWidget::State s;
            switch(p)
            {
            case Game::Piece::Empty:
                s = BoardPieceWidget::Hidden;
                break;
            case Game::Piece::Player:
                s = BoardPieceWidget::Player;
                break;
            case Game::Piece::King:
                s = BoardPieceWidget::King;
                break;
            case Game::Piece::OpPlayer:
                s = BoardPieceWidget::OpPlayer;
                break;
            case Game::Piece::OpKing:
                s = BoardPieceWidget::OpKing;
                break;
            }
            pieces.at(i)->setState(s);
        }
        painter.setPen(QPen(Qt::blue, 2, Qt::SolidLine));
        for(auto &mcv : selectedCellMoves.values())
        {
            for(int i = 0; i < mcv.size(); i++)
            {
                auto mc = mcv.value(i);
                pieces.at(mc.toCellNum())->setState(BoardPieceWidget::Path);
                Game::Cell pmc;
                if(i == 0)
                {
                    pmc = selectedCellMoves.key(mcv)->cell;
                }
                else
                {
                    pmc = mcv.value(i - 1);
                }
                painter.drawLine(pmc.column * 2 * cellSize + cellSize * (pmc.row % 2) + cellSize / 2, pmc.row * cellSize + cellSize / 2,
                                 mc.column * 2 * cellSize + cellSize * (mc.row % 2) + cellSize / 2, mc.row * cellSize + cellSize / 2);
            }
        }
    }
    else
    {
        for(auto &p : pieces)
        {
            p->setState(BoardPieceWidget::Hidden);
        }
        painter.setBrush(QColor(0x93, 0x95, 0x98, 0xFF));
        painter.drawRect(0, 0, width(), height());
        painter.setBrush(QColor(0, 0, 0, 255));
        painter.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, tr("Game not loaded"));
    }
}

void BoardWidget::setBoard(Game::BoardData board)
{
    this->board = board;
    boardValid = true;
    moves.clear();
    selectedCell.clear();
    selectedCellMoves.clear();
    repaint();
}

void BoardWidget::disableBoard()
{
    boardValid = false;
}

void BoardWidget::displayAvailableMoves(Game::BoardPointer board, QVector<Game::MovePointer> moves)
{
    this->boardPtr = board;
    this->moves.clear();
    selectedCell.reset();
    selectedCellMoves.clear();

    for(auto &m : moves)
    {
        this->moves[m->cell].append(m);
    }
}

void BoardWidget::cellClicked(int id)
{
    if(boardPtr.isNull())
    {
        return;
    }
    for(auto &c : moves.keys())
    {
        if(id == c.toCellNum())
        {
            selectedCellMoves.clear();
            selectedCell = Game::CellPointer(new Game::Cell(Game::Cell::fromNum(id)));
            for(auto &mv : moves[c])
            {
                selectedCellMoves[mv] = boardPtr->getMovePathCells(mv);
            }
            repaint();
            return;
        }
    }

    for(auto &cv : selectedCellMoves.values())
    {
        for(auto &c : cv)
        {
            if(id == c.toCellNum())
            {
                emit(selectedMove(selectedCellMoves.key(cv)));
                return;
            }
        }
    }

    selectedCell.reset();
    selectedCellMoves.clear();
    repaint();
}
