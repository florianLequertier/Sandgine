#ifndef COMPONENT_H
#define COMPONENT_H



#include <cstdlib>
#include <vector>
#include <memory>
#include <functional>
#include <iostream>
#include "carray.h"
#include "handler.h"

class Entity;
class BaseWorld;


class Component
{

protected:
    int m_id;
    std::type_index m_typeId;
    Handler<Entity> m_owner;

public:
    Component(Handler<Entity> owner = Handler<Entity>());

    virtual std::shared_ptr<Component> clone() = 0;

    Handler<Entity> getOwner();
    void setOwner(Handler<Entity> owner);
    Handler<Entity> getParent(); //return owner's parent

    std::type_index getTypeId() const;
    void setType(std::type_index type);

    int getId() const;
    void setId(int id);

    void save(BaseWorld& world, std::ostream& stream);
    void load(BaseWorld& world, std::istream& stream);

    virtual void addToEntity(BaseWorld& world, Entity& entity) = 0;

};

#endif // COMPONENT_H
