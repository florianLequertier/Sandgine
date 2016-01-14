#include "entity.h"
#include "component.h"
#include "baseworld.h"


Component::Component(Handler<Entity> owner) : m_typeId(typeid(Component)), m_owner(owner)
{

}

Handler<Entity> Component::getOwner()
{
    return m_owner;
}

void Component::setOwner(Handler<Entity> owner)
{
    m_owner = owner;
}

Handler<Entity> Component::getParent()
{
    if(m_owner)
        return m_owner->getParent();
    else
        return Handler<Entity>();
}

std::type_index Component::getTypeId() const
{
    return m_id.type;
}

void Component::setType(ComponentType type)
{
    m_id.type = type;
}

int Component::getId() const
{
    return m_id;
}

void Component::setId(int id)
{
    m_id = id;
}

void Component::save(BaseWorld& world, std::ostream& stream)
{
    stream<<m_id
          <<m_typeId
          <<m_owner;
}

void Component::load(BaseWorld& world, std::istream& stream)
{
    stream>>m_id
          >>m_typeId;
    m_owner = world.readHandler(stream);
}

void Component::save(std::ostream& stream)
{
    stream<<m_id
          <<m_typeId
}

void Component::load(std::istream& stream)
{
    stream>>m_id
          >>m_typeId;
}
