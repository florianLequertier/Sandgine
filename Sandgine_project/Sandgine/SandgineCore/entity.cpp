#include <algorithm>

#include "entity.h"
#include "baseworld.h"
#include "prefab.h"


Entity::Entity(std::string name, int id) : m_name(name), m_entityId(id)
{

}

Entity::~Entity()
{

}

std::shared_ptr<Entity> Entity::clone()
{
    return std::make_shared<Entity>(*this);
}

int Entity::getId() const
{
    return m_entityId;
}

int Entity::etPrefabId() const
{
    return m_prefabId;
}

Handler<Entity> Entity::parent()
{
    return m_parent;
}

std::string Entity::getName() const
{
    return m_name;
}

void Entity::setName(std::string name)
{
    m_name = name;
}


void Entity::save(BaseWorld& world, std::ostream& stream)
{
    stream<<m_entityId
          <<m_prefabId
          <<m_name
          <<m_components.size();

    for(int i = 0; i < m_components.size(); i++)
    {
        stream<<m_components[i];
    }

    stream<<m_parent;
}

void Entity::load(BaseWorld& world, std::istream& stream)
{
    int componentsCount = 0;

    stream>>m_entityId
          >>m_prefabId
          >>m_name
          >>componentsCount;

    m_components.resize(componentsCount);

    for(int i = 0; i < m_components.size(); i++)
    {
        stream>>m_components[i];
    }

    stream>>m_parent;

    std::vector<std::pair<int, std::type_index>> componentIds = PrefabFactory::Instance().getComponentIds(m_prefabId);
    for(int i = 0; i < m_components.size(); i++)
    {
        int currentId = m_components[i].getId();
        std::type_index currentTypeId = m_components[i].getTypeId();

        auto findIt = std::find(componentIds.begin(), componentIds.end(), std::pair<int, std::type_index>(currentId, currentTypeId));

        if(findIt != componentIds.end())
        {
            componentIds.erase(findIt);
        }
    }
    for(auto it = componentIds.begin(); it != componentIds.end(); it++)
    {
        std::shared_ptr<Component> newComponent = ComponentFactory::instance().clone(it->second);
        newComponent->addToEntity(world, *this);
    }
}

std::shared_ptr<Prefab> Entity::toPrefab(BaseWorld& world)
{
    auto newPrefab = std::make_shared<Prefab>(m_name, m_prefabId);

    for(int i = 0; i < m_components.size(); i++)
    {
        newPrefab->addComponent( world.internalToHandler( m_components[i] )->clone() );
    }

    return newPrefab;
}

void Entity::restoreComponentLinks(BaseWorld& world)
{
    for(int i = 0; i < m_components.size(); i++)
    {
        world.internalToHandler( m_components[i] )->setOwner(world.getComponent<Entity>(m_Id));
    }
}

//TODO move it to Qt wrapper
/*
std::vector<QWidget*> Entity::getComponentHandlers(QWidget* parent)
{

    std::vector<QWidget*> childHandlers;

    for(int i = 0; i < m_componentIds.size(); ++i)
    {
        childHandlers.push_back(m_componentManager->getComponent(m_componentIds[i])->getHandler(parent));
    }

    return childHandlers;
}*/
