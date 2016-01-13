#include "prefabfactory.h"

PrefabFactory::PrefabFactory()
{

}

void PrefabFactory::add(const BaseWorld& world, const Entity& entity)
{
    int prefabId = entity.getPrefabId();
    //if entity isn't linked to a prefab, link the entity to a prefab, generating a giving it a new prefab id.
    if( prefabId == 0 )
    {
        prefabId = generatePrefabId();
        entity.setPrefabId( prefabId );
    }
    else if(m_prefabs.contains(prefabId))
    {
        std::cerr<<"warning : prefab can't be added to the prefabFactory, because there is already a prefab with id : "<<prefab.getId()<<std::endl;
        return;
    }

    m_prefabs[prefabId] = entity.toPrefab(world);
}

void PrefabFactory::add(std::shared_ptr<Prefab> prefab)
{
    if(m_prefabs.contains(prefab.getId()))
    {
        std::cerr<<"warning : prefab can't be added to the prefabFactory, because there is already a prefab with id : "<<prefab.getId()<<std::endl;
        return;
    }

    m_prefabs[prefab.getId()] = prefab;
}

void PrefabFactory::remove(int prefabId)
{
    auto findIt = std::find(m_prefabs.begin(), m_prefabs.end(), prefabId);

    if(findIt != m_prefabs.end())
        m_prefabs.erase(findIt);
    else
        std::cerr<<"warning : can't remove prefab from prefabFactory. There is not prefab with id : "<<prefab.getId()<<std::endl;
}

void PrefabFactory::clear()
{
    m_prefabs.clear();
}

std::shared_ptr<Prefab> PrefabFactory::get(int prefabId)
{
    auto findIt = std::find(m_prefabs.begin(), m_prefabs.end(), prefabId);

    if(findIt != m_prefabs.end())
        return *findIt;
    else
        return std::shared_ptr<Prefab>();
}

std::shared_ptr<Prefab> PrefabFactory::clone(int prefabId)
{
    auto findIt = std::find(m_prefabs.begin(), m_prefabs.end(), prefabId);

    if(findIt != m_prefabs.end())
        return std::make_shared<Prefab>( **findIt );
    else
        return std::shared_ptr<Prefab>();
}

