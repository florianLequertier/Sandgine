#include "worldobject.h"
#include "baseworld.h"

WorldObject::WorldObject(int id = 0, std::string typeId = "") : m_id(id), m_typeId(typeId)
{

}

WorldObject::~WorldObject()
{

}

std::string& WorldObject::getTypeId() const
{
    return m_typeId;
}

void WorldObject::setTypeId(std::string type)
{
    m_typeId = type;
}

int WorldObject::getId() const
{
    return m_id;
}

void WorldObject::setId(int id)
{
    m_id = id;
}

void WorldObject::save(std::ostream& stream, BaseWorld* world)
{
    stream<<m_id
          <<m_typeId;
}

void WorldObject::load(std::istream& stream, BaseWorld* world)
{
    stream>>m_id
          >>m_typeId;
}



