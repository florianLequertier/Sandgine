#include <algorithm>

#include "component.h"
#include "entity.h"
#include "baseworld.h"
#include "prefab.h"
#include "componentfactory.h"


Entity::Entity(std::string name, int id) : base(id, "entity"), m_name(name)
{

}

Entity::~Entity()
{

}

std::shared_ptr<WorldObject> Entity::clone()
{
    return std::static_pointer_cast<WorldObject>( std::make_shared<Entity>(*this) );
}


void Entity::save(std::ostream& stream, BaseWorld* world)
{
    base::save(stream, world);

    stream<<m_prefabId
          <<m_name
          <<m_components.size();

    for(int i = 0; i < m_components.size(); i++)
    {
        stream<<m_components[i];
    }

    stream<<m_parent;
}

void Entity::load(std::istream& stream, BaseWorld* world)
{
    base::load(stream, world);

    int componentsCount = 0;

    stream>>m_prefabId
          >>m_name
          >>componentsCount;

    m_components.resize(componentsCount);

    for(int i = 0; i < m_components.size(); i++)
    {
        m_components[i] = world->readInternalHandler(stream);
    }

    m_parent = world->readInternalHandler(stream);

    std::vector<std::pair<int, std::string>> componentIds = PrefabFactory::instance().getComponentIds(m_prefabId);
    for(int i = 0; i < m_components.size(); i++)
    {
        int currentId = m_components[i]->getId();                                                                               //???
        std::string currentTypeId = m_components[i]->getTypeId();                                                               //???

        auto findIt = std::find(componentIds.begin(), componentIds.end(), std::pair<int, std::string>(currentId, currentTypeId));

        if(findIt != componentIds.end())
        {
            componentIds.erase(findIt);
        }
    }
    for(auto it = componentIds.begin(); it != componentIds.end(); it++)
    {
        std::shared_ptr<Component> newComponent = ComponentFactory::instance().clone(it->second);
        newComponent->addToEntity(*world, *this);
    }
}

std::string Entity::getPrefabId() const
{
    return m_prefabId;
}

Handler<Entity> Entity::getParent(const BaseWorld &world)
{
    return world.internalToHandler<Entity>(m_parent);
}

void Entity::setParent(InternalHandler parent)
{
    m_parent = parent;
}

std::string Entity::getName() const
{
    return m_name;
}

void Entity::setName(std::string name)
{
    m_name = name;
}


std::shared_ptr<Prefab> Entity::toPrefab(BaseWorld& world)
{
    auto newPrefab = std::make_shared<Prefab>(m_name, m_prefabId);

    for(int i = 0; i < m_components.size(); i++)
    {
        newPrefab->addComponent( std::static_pointer_cast<Component>(m_components[i]->clone()) );
    }

    return newPrefab;
}

template<typename T>
Handler<T> Entity::addComponent(BaseWorld& world)
{
    Handler<T> handler = world.addComponent<T>();
    m_components.push_back(handler);

    return handler;
}

template<typename T>
Handler<T> Entity::addComponent(BaseWorld& world, const Component& component)
{
    Handler<T> handler = world.addComponent<T>(component);
    m_components.push_back(handler);

    return handler;
}

template<typename T>
void Entity::removeComponent(BaseWorld& world)
{
    auto it = std::find_if(m_components.begin(), m_components.end(), [](const Component& c){
                                                                            return typeid(c) == typeid(T);
                                                                        });

    if(it != m_components.end())
    {
        if(world.removeComponent<T>(*it))
            m_components.erase(it);
        else
            std::cerr<<"problem while removing a component of type : "<<typeid(T).name()<<" from an entity"<<std::endl
                     <<"the component is present in Entity but not in the World."<<std::endl;
    }
}

template<typename T>
Handler<T> Entity::getComponent(BaseWorld& world)
{
    auto it = std::find_if(m_components.begin(), m_components.end(), [](const Component& c){
                                                                            return typeid(c) == typeid(T); // ???
                                                                        });
    if(it != m_components.end())
    {
        return world.internalToHandler<T>(*it);
    }
    else
        return Handler<T>();
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
