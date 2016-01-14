#include "prefab.h"

Prefab::Prefab(std::string name, int prefabId) : m_name(name), m_prefabId(prefabId)
{

}

Prefab::Prefab(const Entity& entity, const World& world)
{
    m_prefabId = entity.getId();
    m_name = entity.getName();

    int componentCount = entity.getComponentCount();
    for(int i = 0; i < componentCount; i++)
    {
        //get, clone then add the new component to the prefab
        addComponent( entity.getComponent(i)->clone() );
    }
}

Handler<Entity> Prefab::toEntity(BaseWorld& world)
{
    auto newEntity = std::make_shared<Entity>(m_name, m_prefabId);
    Handler<Entity> entityHandler = world.addEntity(newEntity);

    for(int i = 0; i < m_components.size(); i++)
    {
        m_components[i]->addToEntity(world, entityHandler);
    }

    return entityHandler;
}

void Prefab::addComponent(std::shared_ptr<Component> component)
{
    m_components.push_back(component);
}

void Prefab::removeComponent(std::shared_ptr<Component> component)
{
    auto findIt = std::find(m_components.begin(), m_components.end(), component);

    if(findIt != m_components.end())
        m_components.erase(findIt);
}

void Prefab::save(World& world, std::ostream& stream)
{
    stream<<m_prefabId
          <<m_name
          <<m_components.size();

    for(int i = 0; i < m_components.size(); i++)
    {
        stream<<m_components[i].getTypeId();
        m_components[i].save(world, stream);
    }
}

void Prefab::load(World& world, std::istream& stream)
{
    int componentCount = 0;
    std::type_index typeId;

    stream>>m_prefabId
          >>m_name
          >>componentCount;

    m_components.resize(componentCount);

    for(int i = 0; i < m_components.size(); i++)
    {
        stream>>typeId;
        m_components[i] = ComponentFactory::instance().copy(typeId);
        m_components[i].load(world, stream);
    }
}

