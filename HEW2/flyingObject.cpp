
#include <list>
#include "flyingObject.h"

std::list<FlyingObject> flyingObjects;

std::list<FlyingObject>* GetFlyingObjects(){
	return &flyingObjects;
}