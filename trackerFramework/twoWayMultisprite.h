#ifndef TWOWAYMULTISPRITE__H
#define TWOWAYMULTISPRITE__H
#include <string>
#include <vector>
#include "multisprite.h"

class TwoWayMultiSprite : public MultiSprite {
public:
  TwoWayMultiSprite(const std::string&);
  TwoWayMultiSprite(const TwoWayMultiSprite&);
  virtual ~TwoWayMultiSprite() { } 

protected:
  virtual void advanceFrame(Uint32 ticks);
};
#endif
