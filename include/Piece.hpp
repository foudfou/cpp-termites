#ifndef _PIECE_H_
#define _PIECE_H_

#include <memory>
#include <string>

class Piece
{
public:
  Piece();
  virtual ~Piece();

  virtual bool isWoodChip() const;
  virtual bool isTermite() const;

  virtual std::string dump() const = 0;
};

typedef std::shared_ptr<Piece> PiecePtr;

#endif /* _PIECE_H_ */
