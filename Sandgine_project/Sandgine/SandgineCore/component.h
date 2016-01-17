#ifndef COMPONENT_H
#define COMPONENT_H



#include <cstdlib>
#include <vector>
#include <memory>
#include <functional>
#include <iostream>

#include "carray.h"
#include "handler.h"
#include "worldobject.h"

class BaseWorld;
class Entity;


class Component : public WorldObject
{

protected:
    typedef WorldObject base;

    int m_componentId;
    InternalHandler m_owner;

public:

    Component(int id = 0, InternalHandler owner = InternalHandler());
    virtual ~Component();

    Handler<Entity> getOwner(BaseWorld& world);
    void setOwner(InternalHandler owner);
    Handler<Entity> getParent(BaseWorld& world); //return owner's parent


    int getComponentId() const;
    void setComponentId(int id);

    void save(std::ostream& stream, BaseWorld* world = nullptr );
    void load(std::istream& stream, BaseWorld* world = nullptr );

    virtual void addToEntity(BaseWorld& world, Entity& entity) = 0;

};

#endif // COMPONENT_H
