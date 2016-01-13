#include "baseworld.h"

BaseWorld::BaseWorld()
{

}

void BaseWorld::load(std::string path)
{
    std::ifstream stream;
    stream.open(path);

    m_entities.load(this, stream);
    m_sprites.load(this, stream);
    m_soundEmitters.load(this, stream);
    m_particleSystems.load(this, stream);
    m_lights.load(this, stream);
    m_cameras.load(this, stream);

    //...
    stream.close();
}

void BaseWorld::save(std::string path)
{
    std::ofstream stream;
    stream.open(path);

    m_entities.save(this, stream);
    m_sprites.save(this, stream);
    m_soundEmitters.save(this, stream);
    m_particleSystems.save(this, stream);
    m_lights.save(this, stream);
    m_cameras.save(this, stream);

    //...
    stream.close();
}

void BaseWorld::clear()
{
    m_entities.clear();

    m_sprites.clear();
    m_soundEmitters.clear();
    m_particleSystems.clear();
    m_lights.clear();
    m_cameras.clear();
}

void BaseWorld::init()
{
    m_renderSystem.init(this);
    //m_soundSystem.init(); ?
    m_scriptSystem.init(this);
    m_physicSystem.init(this);
}

void BaseWorld::update()
{
    m_renderSystem.update(this);
    //m_soundSystem.update(); ?
    m_scriptSystem.update(this);
    m_physicSystem.update(this);
}

Handler<Entity> BaseWorld::addEntity(std::shared_ptr<Entity> entity)
{
    m_entities.add(*entity);
}
