#ifndef PIECESTYPE_H
#define PIECESTYPE_H

namespace Checkers {

enum class Places { OpKing, OpPlayer, Empty, Player, King };
using BoardVector = QList<Checkers::Places>;

}

#endif // PIECESTYPE_H
