#include "componentfactory.h"

ComponentFactory::ComponentFactory()
{
    auto sprite = std::make_shared<Sprite>();
    auto soundEmitter = std::make_shared<SoundSystem>();
    auto particleSystem = std::make_shared<ParticleSystem>();
    auto light = std::make_shared<Light>();
    auto camera = std::make_shared<Camera>();


    add(sprite);
    add(soundEmitter);
    add(particleSystem);
    add(light);
    add(camera);
}

void ComponentFactory::add(std::shared_ptr<Component> component)
{
    m_components[component->getTypeId()] = component;
}

void ComponentFactory::remove(std::string componentId)
{
    m_components.erase(componentId);
}

void ComponentFactory::clear()
{
    m_components.clear();
}

std::shared_ptr<Component> ComponentFactory::get(std::string componentId)
{
    auto findIt = std::find(m_components.begin(), m_components.end(), componentId);
    if(findIt != m_components.end())
        return *findIt;
    else
        return std::shared_ptr<Component>();
}

std::shared_ptr<Component> ComponentFactory::clone(int componentId)
{
    auto findIt = std::find(m_components.begin(), m_components.end(), componentId);
    if(findIt != m_components.end())
        return std::make_shared<Component>(**findIt);
    else
        return std::shared_ptr<Component>();
}

