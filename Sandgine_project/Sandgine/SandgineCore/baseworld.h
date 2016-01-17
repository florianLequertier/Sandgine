#ifndef WORLD_H
#define WORLD_H

#include <typeinfo>
#include <typeindex>
#include <map>
#include <assert.h>

#include "handler.h"
#include "carray.h"
#include <iostream>

#include "entity.h"
#include "prefabfactory.h"
#include "prefab.h"

#include "sprite.h"

class BaseWorld
{
private:
    //datas :
    CArray<Entity> m_entities;

    CArray<Sprite> m_sprites;
    //CArray<SoundEmitter> m_soundEmitters;         TODO
    //CArray<ParticleSystem> m_particleSystems;     TODO
    //CArray<Light> m_lights;                       TODO
    //CArray<Camera> m_cameras;                     TODO

    //data mapping :
    std::map<std::string, BaseCArray*> m_componentMapping;

    //systems :
    //RenderSystem m_renderSystem;                  TODO
    //SoundSystem m_soundSystem;                    TODO
    //ScriptSystem m_scriptSystem;                  TODO
    //PhysicSystem m_physicSystem;                  TODO

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
    Handler<T> internalToHandler(const InternalHandler& internalHandler) const;

    template<typename T>
    Handler<T> instantiate(std::string prefabId);

    Handler<Entity> addEntity(std::shared_ptr<Entity> entity);

    template<typename T>
    Handler<T> addComponent(const Component& component);

    template<typename T>
    Handler<T> addComponent();

    template<typename T>
    CArray<T>* getContainer(const std::string& typeId);

    BaseCArray* getBaseContainer(const std::string& typeId);

    template<typename T>
    Handler<T> readHandler(std::istream& stream);

    InternalHandler readInternalHandler(std::istream& stream);

    template<typename T>
    bool removeComponent(InternalHandler internalHandler);

};


template<typename T>
Handler<T> BaseWorld::getComponent(int index)
{
    std::string id = typeid(T).name;

    assert( dynamic_cast<CArray<T>*>(m_componentMapping[id]) == m_componentMapping[id]);
    return static_cast<CArray<T>*>(m_componentMapping[id])[index];
}

template<typename T>
Handler<T> BaseWorld::internalToHandler(const InternalHandler& internalHandler) const
{
    assert( dynamic_cast<CArray<T>*>( m_componentMapping.at(internalHandler.m_typeId) ) == m_componentMapping.at(internalHandler.m_typeId) );
    CArray<T>* array =  static_cast<CArray<T>*>(m_componentMapping.at(internalHandler.m_typeId));

    return Handler<T>(array, internalHandler.m_index);
}

template<typename T>
Handler<T> BaseWorld::instantiate(std::string prefabId)
{
    std::shared_ptr<Prefab> prefab = PrefabFactory::instance().get(prefabId);

    return prefab->toEntity(*this);
}

template<typename T>
Handler<T> BaseWorld::addComponent(const Component& component)
{
    auto container = getContainer<T>(typeid(T).name()); //!!!
    return container->add(component);
}

template<typename T>
Handler<T> BaseWorld::addComponent()
{
    auto container = getContainer<T>(typeid(T).name()); //!!!
    return container->add();
}

template<typename T>
CArray<T>* BaseWorld::getContainer(const std::string &typeId)
{
    assert( dynamic_cast<CArray<T>*>(m_componentMapping[typeId]) == m_componentMapping[typeId]);
    return static_cast<CArray<T>*>(m_componentMapping[typeId]);
}

template<typename T>
Handler<T> BaseWorld::readHandler(std::istream& stream)
{
    InternalHandler itHandler = readInternalHandler(stream);

    return internalToHandler<T>(itHandler);
}

template<typename T>
bool BaseWorld::removeComponent(InternalHandler internalHandler)
{
    CArray<T>* container = getContainer<T>(internalHandler.m_typeId);
    return container->remove(internalHandler.m_index);
}

#endif // WORLD_H
