#ifndef SPRITE_H
#define SPRITE_H

#include "component.h"

class BaseWorld;

class Sprite : public Component
{
private:
public:
    Sprite();
    virtual ~Sprite();

    virtual std::shared_ptr<WorldObject> clone() override;
    virtual void addToEntity(BaseWorld& world, Entity& entity) override;
};

#endif // SPRITE_H
