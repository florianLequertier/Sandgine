#ifndef ENTITY_H
#define ENTITY_H


#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

class Component;
class BaseWorld;
class Prefab;

class Entity
{    
    friend class Prefab;

private:
    int m_entityId;
    int m_prefabId;
    std::string m_name;
    std::vector<InternalHandler> m_components;

    Handler<Entity> m_parent; //???

public:
    Entity(std::string name = "", int id = 0);
    virtual ~Entity();
    std::shared_ptr<Entity> clone();

    int getId() const;
    int getPrefabId() const;

    template<typename T>
    Handler<T> addComponent(BaseWorld& world);
    template<typename T>
    Handler<T> addComponent(BaseWorld& world, const Component& component);
    template<typename T>
    void removeComponent(BaseWorld& world);
    template<typename T>
    Handler<T> getComponent(BaseWorld& world);

    Handler<Entity> getParent(); //???

    std::string getName() const;
    void setName(std::string name);

    void save(BaseWorld& world, std::ostream& stream);

    void load(BaseWorld& world, std::istream& stream);

    std::shared_ptr<Prefab> toPrefab(BaseWorld &world);

    void restoreComponentLinks();

};

#include "component.h"
#include "baseworld.h"

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
        return world.internalToHandler<T>(m_components[i]);
    }
    else
        return Handler<T>();
}


#endif // ENTITY_H
