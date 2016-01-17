#ifndef ENTITY_H
#define ENTITY_H


#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#include "handler.h"
#include "worldobject.h"

class Component;
class BaseWorld;
class Prefab;

class Entity : public WorldObject
{    
    friend class Prefab;

private:
    typedef WorldObject base;

    std::string m_prefabId;
    std::string m_name;
    std::vector<InternalHandler> m_components;

    InternalHandler m_parent; //???

public:
    Entity(std::string name = "", int id = 0);
    virtual ~Entity();

    //clone design patern
    virtual std::shared_ptr<WorldObject> clone() override;

    //serialization management
    void save(std::ostream& stream, BaseWorld* world = nullptr );
    void load(std::istream& stream, BaseWorld* world = nullptr );

    //getters/setters
    int getId() const;
    std::string getPrefabId() const;
    std::string getName() const;
    void setName(std::string name);

    //component management
    template<typename T>
    Handler<T> addComponent(BaseWorld& world);
    template<typename T>
    Handler<T> addComponent(BaseWorld& world, const Component& component);
    template<typename T>
    void removeComponent(BaseWorld& world);
    template<typename T>
    Handler<T> getComponent(BaseWorld& world);

    //parenting management
    Handler<Entity> getParent(const BaseWorld& world); //???
    void setParent(InternalHandler parent); //???

    //prefab management
    std::shared_ptr<Prefab> toPrefab(BaseWorld &world);

};



#endif // ENTITY_H
