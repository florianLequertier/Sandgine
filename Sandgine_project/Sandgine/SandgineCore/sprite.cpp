#include "sprite.h"
#include "baseworld.h"

Sprite::Sprite()
{

}

Sprite::~Sprite()
{

}

std::shared_ptr<WorldObject> Sprite::clone()
{
    return std::make_shared<Sprite>(*this);
}

void Sprite::addToEntity(BaseWorld& world, Entity& entity)
{
    entity.addComponent<Sprite>(world, *this);
}

