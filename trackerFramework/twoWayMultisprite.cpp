#include "twoWayMultisprite.h"
#include "gamedata.h"
#include "frameFactory.h"

void TwoWayMultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
        if(velocityX() >= 0){
            currentFrame = (currentFrame+1) % (numberOfFrames/2);
        }
        else {
            if(currentFrame > numberOfFrames/2){
                currentFrame =  (currentFrame+1) % numberOfFrames;
            }

            if(currentFrame < numberOfFrames/2){
                currentFrame = (currentFrame + (numberOfFrames/2) + 1) % numberOfFrames;
            }
        }
        timeSinceLastFrame = 0;
	}
}

TwoWayMultiSprite::TwoWayMultiSprite( const std::string& name) :
  MultiSprite(name, FrameFactory::getInstance().getTwoWayFrames(name)),
{ }

TwoWayMultiSprite::TwoWayMultiSprite(const TwoWayMultiSprite& s) :
  MultiSprite(s), 
{ }