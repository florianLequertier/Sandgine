#ifndef PREFABFACTORY_H
#define PREFABFACTORY_H

#include <map>

#include "prefab.h"
#include "entity.h"

class PrefabFactory
{
private:
    std::map<std::string, std::shared_ptr<Prefab>> m_prefabs;

public:
    PrefabFactory();

    void add(const BaseWorld &world, const Entity& entity);
    void add(std::shared_ptr<Prefab> prefab);
    void remove(std::string prefabId);
    void clear();
    std::shared_ptr<Prefab> get(std::string prefabId);
    std::shared_ptr<Prefab> clone(std::string prefabId);

    void load(std::istream& stream);
    void save(std::ostream& stream);
};

#endif // PREFABFACTORY_H
