#include "frameFactory.h"
#include "extractSurface.h"
#include "ioManager.h"
#include "vector2f.h"

FrameFactory::~FrameFactory() {
  std::cout << "Deleting FrameFactory" << std::endl;
  std::map<std::string, SDL_Surface*>::iterator itSurf = surfaces.begin();
  while ( itSurf != surfaces.end() ) {
    SDL_FreeSurface( itSurf->second );
    ++itSurf;
  }
  std::map<std::string, std::vector<SDL_Surface*> >::iterator 
    surfaces = multiSurfaces.begin();
  while ( surfaces != multiSurfaces.end() ) {
    for (unsigned int i = 0; i < surfaces->second.size(); ++i) {
      SDL_FreeSurface( surfaces->second[i] );
    }
    ++surfaces;
  }
  std::map<std::string, Frame*>::iterator frame = frames.begin();
  while ( frame != frames.end() ) {
    delete frame->second;
    ++frame;
  }
  std::map<std::string, std::vector<Frame*> >::iterator 
    frames = multiFrames.begin();
  while ( frames != multiFrames.end() ) {
    for (unsigned int i = 0; i < frames->second.size(); ++i) {
      delete frames->second[i];
    }
    ++frames;
  }
}

FrameFactory& FrameFactory::getInstance() {
  static FrameFactory factory;
  return factory;
}

Frame* FrameFactory::getFrame(const std::string& name) {
    std::map<std::string, Frame*>::const_iterator pos = frames.find(name); 
  if ( pos == frames.end() ) {
    SDL_Surface * const surface =
      IOManager::getInstance().loadAndSet(
          gdata.getXmlStr(name+"/file"),
          gdata.getXmlBool(name+"/transparency"));
    surfaces[name] = surface;
    Frame * const frame =new Frame(surface);
    frames[name] = frame;
    return frame;
  }
  else {
    return pos->second;
  }
}

std::vector<Frame*> FrameFactory::getFrames(const std::string& name) {
  // First search map to see if we've already made it:
  std::map<std::string, std::vector<Frame*> >::const_iterator 
    pos = multiFrames.find(name); 
  if ( pos != multiFrames.end() ) {
    return pos->second;
  }

  // It wasn't in the map, so we have to make the vector of Frames:
  SDL_Surface* surface = IOManager::
     getInstance().loadAndSet(gdata.getXmlStr(name+"/file"), true);
  unsigned numberOfFrames = gdata.getXmlInt(name+"/frames");
  std::vector<Frame*> frames;
  std::vector<SDL_Surface*> surfaces;
  frames.reserve(numberOfFrames);

  Uint16 width = surface->w;
  Uint16 height = surface->h/numberOfFrames;

  SDL_Surface* surf;
  for (unsigned i = 0; i < numberOfFrames; ++i) {
    unsigned frameY = i * height;
   surf = ExtractSurface::getInstance().
               get(surface, width, height, 0, frameY); 
    surfaces.push_back( surf );
    frames.push_back( new Frame(surf) );
  }
  SDL_FreeSurface(surface);
  multiSurfaces[name] = surfaces;
  multiFrames[name] = frames;
  return frames;
}

std::vector<Frame*> FrameFactory::getTwoWayFrames(const std::string& name) {
  // First search map to see if we've already made it:
  std::map<std::string, std::vector<Frame*> >::const_iterator 
    pos = multiFrames.find(name); 
  if ( pos != multiFrames.end() ) {
    return pos->second;
  }

  // It wasn't in the map, so we have to make the vector of Frames:
  SDL_Surface* rightSurface = IOManager::
     getInstance().loadAndSet(gdata.getXmlStr(name+"/filePositive"), true);
  unsigned numberOfXFrames = gdata.getXmlInt(name+"/xframes");
  unsigned numberOfYFrames = gdata.getXmlInt(name+"/yframes");
  std::vector<Frame*> frames;
  std::vector<SDL_Surface*> surfaces;
  frames.reserve(numberOfXFrames*numberOfYFrames*2);

  Uint16 width = rightSurface->w/numberOfXFrames;
  Uint16 height = rightSurface->h/numberOfYFrames;

  SDL_Surface* surf;
  for (unsigned i = 0; i < numberOfYFrames; ++i) {
      for (unsigned j = 0; j < numberOfXFrames; ++j){
                 
        unsigned frameY = i * height;
        unsigned frameX = j * width;
        surf = ExtractSurface::getInstance().
               get(rightSurface, width, height, frameX, frameY); 
        surfaces.push_back( surf );
        frames.push_back( new Frame(surf) );
    } 
  }
  
  // It wasn't in the map, so we have to make the vector of Frames:
  SDL_Surface* leftSurface = IOManager::
     getInstance().loadAndSet(gdata.getXmlStr(name+"/fileNegative"), true);
  
  Uint16 width1 = leftSurface->w/numberOfXFrames;
  Uint16 height1 = leftSurface->h/numberOfYFrames;

  for (unsigned i = 0; i < numberOfYFrames; ++i) {
      for (unsigned j = 0; j < numberOfXFrames; ++j){
                 
        unsigned frameY = i * height1;
        unsigned frameX = j * width1;
        surf = ExtractSurface::getInstance().
               get(leftSurface, width1, height1, frameX, frameY); 
        surfaces.push_back( surf );
        frames.push_back( new Frame(surf) );
    } 
  }
  
  
  SDL_FreeSurface(rightSurface);
  SDL_FreeSurface(leftSurface);
  multiSurfaces[name] = surfaces;
  multiFrames[name] = frames;
  return frames;
}