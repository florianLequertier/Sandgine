#ifndef PREFABFACTORY_H
#define PREFABFACTORY_H

#include <map>

#include "prefab.h"
#include "entity.h"

class PrefabFactory
{
private:
    std::map<int, std::shared_ptr<Prefab>> m_prefabs;

public:
    PrefabFactory();

    void add(const BaseWorld &world, const Entity& entity);
    void add(std::shared_ptr<Prefab> prefab);
    void remove(int prefabId);
    void clear();
    std::shared_ptr<Prefab> get(int prefabId);
    std::shared_ptr<Prefab> clone(int prefabId);

};

#endif // PREFABFACTORY_H
