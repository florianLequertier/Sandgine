#include "baseworld.h"

BaseWorld::BaseWorld()
{
    m_componentMapping["entity"] = &m_entities;
    m_componentMapping["sprite"] = &m_sprites;
    //m_componentMapping["soundEmitter"] = &m_soundEmitters;        TODO
    //m_componentMapping["particleSystem"] = &m_particleSystems;    TODO
    //m_componentMapping["light"] = &m_lights;                      TODO
    //m_componentMapping["camera"] = &m_cameras;                    TODO
}

void BaseWorld::load(std::string path)
{
    std::ifstream stream;
    stream.open(path);

    m_entities.load(stream, this);
    m_sprites.load(stream, this);
    //m_soundEmitters.load(stream, this);       TODO
    //m_particleSystems.load(stream, this);     TODO
    //m_lights.load(stream, this);              TODO
    //m_cameras.load(stream, this);             TODO

    //...
    stream.close();
}

void BaseWorld::save(std::string path)
{
    std::ofstream stream;
    stream.open(path);

    m_entities.save(stream, this);
    m_sprites.save(stream, this);
    //m_soundEmitters.save(stream, this);         TODO
    //m_particleSystems.save(stream, this);       TODO
    //m_lights.save(stream, this);                TODO
    //m_cameras.save(stream, this);               TODO

    //...
    stream.close();
}

void BaseWorld::clear()
{
    m_entities.clear();

    m_sprites.clear();
    //m_soundEmitters.clear();                    TODO
    //m_particleSystems.clear();                  TODO
    //m_lights.clear();                           TODO
    //m_cameras.clear();                          TODO
}

void BaseWorld::init()
{
    //m_renderSystem.init(this);                  TODO
    //m_soundSystem.init();                       ???
    //m_scriptSystem.init(this);                  TODO
    //m_physicSystem.init(this);                  TODO
}

void BaseWorld::update()
{
    //m_renderSystem.update(this);                TODO
    //m_soundSystem.update();                     ???
    //m_scriptSystem.update(this);                TODO
    //m_physicSystem.update(this);                TODO
}

Handler<Entity> BaseWorld::addEntity(std::shared_ptr<Entity> entity)
{
    m_entities.add(*entity);
}

BaseCArray* BaseWorld::getBaseContainer(const std::string& typeId)
{
    return m_componentMapping[typeId];
}

InternalHandler BaseWorld::readInternalHandler(std::istream &stream)
{
    int index, generation;
    std::string typeId;

    stream>>index
          >>generation
          >>typeId;


    InternalHandler newInternal(index, generation, typeId);

    newInternal.m_datas = getBaseContainer(typeId);
}
