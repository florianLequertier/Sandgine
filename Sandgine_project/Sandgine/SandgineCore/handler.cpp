#include "handler.h"
#include "baseworld.h"


WorldObject& InternalHandler::get() const
{
    return (*m_datas)[m_index];
}

bool InternalHandler::isNull() const
{
    return (m_datas == nullptr
            || m_index < 0
            || m_index >= m_datas->size()
            || !m_datas->isValid(m_index, m_generation));
}

WorldObject* InternalHandler::operator->()
{
    return &(*m_datas)[m_index];
}

WorldObject& InternalHandler::operator*()
{
    return (*m_datas)[m_index];
}
/*
void InternalHandler::save( std::ostream& stream, World* world = nullptr )
{
    stream<<m_index
          <<m_generation
          <<m_typeId;
}

void InternalHandler::load( std::istream& stream, World* world = nullptr )
{
    stream>>m_index
          >>m_generation
          >>m_typeId;

    assert(world != nullptr);
    m_datas = world->getBaseContainer(m_typeId);
}
*/
