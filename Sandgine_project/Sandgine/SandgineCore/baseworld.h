#ifndef WORLD_H
#define WORLD_H

#include <typeinfo>
#include <typeindex>
#include <map>

#include "carray.h"
#include <iostream>

#include "entity.h"
#include "prefabfactory.h"
#include "prefab.h"

class BaseWorld
{
private:
    //datas :
    CArray<Entity> m_entities;

    CArray<Sprite> m_sprites;
    CArray<SoundEmitter> m_soundEmitters;
    CArray<ParticleSystem> m_particleSystems;
    CArray<Light> m_lights;
    CArray<Camera> m_cameras;

    //data mapping :
    std::map<std::type_index, BaseCArray*> m_componentMapping;

    //systems :
    RenderSystem m_renderSystem;
    SoundSystem m_soundSystem;
    ScriptSystem m_scriptSystem;
    PhysicSystem m_physicSystem;

public:
    BaseWorld();

    void load(std::string path);
    void save(std::string path);

    /**
     * @brief clear all datas
     */
    void clear();
    /**
     * @brief initialyze world
     */
    void init();
    /**
     * @brief update world
     */
    void update();

    template<typename T>
    Handler<T> getComponent(int index);

    template<typename T>
    Handler<T> internalToHandler(const InternalHandler<T>& internalHandler);

    template<typename T>
    Handler<T> instantiate(std::string prefabId);

    Handler<Entity> addEntity(std::shared_ptr<Entity> entity);

    template<typename T>
    Handler<T> addComponent<T>(const Component& component);

    template<typename T>
    Handler<T> addComponent<T>();

    template<typename T>
    CArray<T>* getContainer(const std::type_index& typeId);

    template<typename T>
    Handler<T> readHandler(std::istream& stream);

    template<typename T>
    bool removeComponent(InternalHandler internalHandler);

};


template<typename T>
Handler<T> BaseWorld::getComponent(int index)
{
    std::type_index id = typeid(T);

    assert( dynamic_cast<CArray<T>*>(m_componentMapping[id]) == m_componentMapping[id]);
    return static_cast<CArray<T>*>(m_componentMapping[id])[index];
}

template<typename T>
Handler<T> BaseWorld::internalToHandler(const InternalHandler<T>& internalHandler)
{
    assert( dynamic_cast<CArray<T>*>(m_componentMapping[internalHandler.m_typeId]) == m_componentMapping[internalHandler.m_typeId] );
    CArray<T>* array =  static_cast<CArray<T>*>(m_componentMapping[internalHandler.m_typeId])[internalHandler.m_index];

    return Handler<T>(array, internalHandler.m_index);
}

template<typename T>
Handler<T> BaseWorld::instantiate(std::string prefabId)
{
    std::shared_ptr<Prefab> prefab = PrefabFactory::instance().get(prefabId);

    return prefab->toEntity(this);
}

template<typename T>
Handler<T> BaseWorld::addComponent(const Component& component)
{
    auto container = getContainer<T>(std::type_index(T)); //!!!
    return container->add(component);
}

template<typename T>
Handler<T> BaseWorld::addComponent<T>()
{
    auto container = getContainer<T>(std::type_index(T)); //!!!
    return container->add();
}

template<typename T>
CArray<T>* BaseWorld::getContainer(const std::type_index& typeId)
{
    assert( dynamic_cast<CArray<T>*>(m_componentMapping[typeId]) == m_componentMapping[typeId]);
    return static_cast<CArray<T>*>(m_componentMapping[typeId]);
}

template<typename T>
Handler<T> BaseWorld::readHandler(std::istream& stream)
{
    InternalHandler itHandler;
    stream>>itHandler;

    return internalToHandler<T>(itHandler);
}

template<typename T>
bool BaseWorld::removeComponent(InternalHandler internalHandler)
{
    CArray<T>* container = getContainer<T>(internalHandler.m_typeId);
    return container->remove(internalHandler.m_index);
}

#endif // WORLD_H
