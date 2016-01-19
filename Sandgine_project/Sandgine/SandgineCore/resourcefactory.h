#ifndef RESOURCEFACTORY_H
#define RESOURCEFACTORY_H

#include <map>
#include <iostream>
#include <memory>
#include <vector>
#include <assert.h>

#include "resource.h"
#include "singleton.h"

enum ResourceTypeId{NONE, TEXTURE, TILE_SET, SOUND, RESOURCE_TYPE_COUNT};

std::shared_ptr<Resource> loadResource(ResourceTypeId typeId, const std::string& resourcePath);

class ResourceFactory : public Singleton<ResourceFactory>
{
private:
    std::map<std::string, std::shared_ptr<Resource>> m_textures;
    std::map<std::string, std::shared_ptr<Resource>> m_tileSets;
    std::map<std::string, std::shared_ptr<Resource>> m_sounds;

    std::map<ResourceTypeId, map<std::string, std::shared_ptr<Resource>>*> m_resourceTypesMapping;

public:
    ResourceFactory();

    void add(ResourceTypeId typeId, std::shared_ptr<Resource> resourceToAdd);

    void add(ResourceTypeId typeId, const std::string& resourceName, std::vector<std::string>& resourcePath);

    void add(ResourceTypeId typeId, const std::string& resourceName, const std::string& resourcePath);

    void add(ResourceTypeId typeId, std::vector<std::pair<const std::string&, const std::string&>> resourcesNamesPathes);

    void remove(ResourceTypeId typeId, const std::string& resourceName);

    void clear();

    template<typename T>
    std::shared_ptr<T> get(ResourceTypeId typeId, const std::string& resourceName);

    template<typename T>
    std::shared_ptr<T> clone(ResourceTypeId typeId, const std::string& resourceName);

    void load(std::istream& stream);
    void save(std::ostream& stream);
};

template<typename T>
std::shared_ptr<T> ResourceFactory::get(ResourceTypeId typeId, const std::string& resourceName)
{
    auto resourceContainerPtr = m_resourceTypesMapping[typeId];
    if(resourceContainerPtr != nullptr)
    {
        return (*resourceContainerPtr)[resourceName];
    }
    else
    {
        std::cerr<<"error : There is no valid resource container which match with the given key."<<std::endl;
    }
}

template<typename T>
std::shared_ptr<T> ResourceFactory::clone(ResourceTypeId typeId, const std::string& resourceName)
{
    auto resourceContainerPtr = m_resourceTypesMapping[typeId];
    if(resourceContainerPtr != nullptr)
    {
        return std::make_shared<T>( *(*resourceContainerPtr)[resourceName] );
    }
    else
    {
        std::cerr<<"error : There is no valid resource container which match with the given key."<<std::endl;
    }
}

#endif // RESOURCEFACTORY_H
