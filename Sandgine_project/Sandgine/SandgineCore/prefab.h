#ifndef PREFAB_H
#define PREFAB_H

#include <string>
#include <vector>

#include "handler.h"
#include "carray.h"
#include "entity.h"
#include "component.h"

//forward
class World;

class Prefab
{
private:
    std::string m_prefabId;
    std::string m_name;
    std::vector<std::shared_ptr<Component>> m_components;

public:
    Prefab(std::string name = "", std::string prefabId = 0);
    Prefab(const Entity& entity, const World& world);

    Handler<Entity> toEntity(BaseWorld &world);

    void addComponent(std::shared_ptr<Component> component);
    void removeComponent(std::shared_ptr<Component> component);

    void save(World& world, std::ostream& stream);
    void load(World& world, std::istream& stream);

    std::vector<std::pair<int, std::string>> getComponentIds();
};

#endif // PREFAB_H
