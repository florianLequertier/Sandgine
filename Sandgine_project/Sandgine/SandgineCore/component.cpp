#include "component.h"
#include "baseworld.h"


Component::Component(int id, InternalHandler owner) : base( id , typeid(Component).name() ) , m_owner(owner)
{

}

Component::~Component()
{

}

Handler<Entity> Component::getOwner(BaseWorld &world)
{
    return world.internalToHandler<Entity>(m_owner);
}

void Component::setOwner(InternalHandler owner)
{
    m_owner = owner;
}

Handler<Entity> Component::getParent(BaseWorld& world)
{
    if(m_owner)
        return world.internalToHandler<Entity>(m_owner)->getParent(world);
    else
        return Handler<Entity>();
}

int Component::getComponentId() const
{
    return m_componentId;
}

void Component::setComponentId(int id)
{
    m_componentId = id;
}

void Component::save(std::ostream& stream, BaseWorld* world)
{
    base::save(stream, world);

    stream<<m_owner;
}

void Component::load(std::istream& stream, BaseWorld* world)
{
    base::load(stream, world);

    if(world != nullptr)
        m_owner = world->readInternalHandler(stream);
}
