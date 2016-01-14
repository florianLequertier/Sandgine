#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include <map>

#include "component.h"

class ComponentFactory
{
private:
    std::map<int, std::shared_ptr<Component>> m_components;

public:
    ComponentFactory();

    void add(std::shared_ptr<Component> component);
    void remove(std::string componentId);
    void clear();
    std::shared_ptr<Component> get(std::string componentId);
    std::shared_ptr<Component> clone(std::string componentId);
};

#endif // COMPONENTFACTORY_H
