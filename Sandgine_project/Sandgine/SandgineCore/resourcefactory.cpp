#include "resourcefactory.h"

std::shared_ptr<Resource> loadResource(ResourceTypeId typeId, const std::string& resourcePath)
{
    switch(typeId)
    {
        case ResourceTypeId::TEXTURE:
            auto newResource_texture = std::make_shared<Texture>(resourcePath);
            newResource_texture.load(resourcePath);
            return newResource_texture;
        break;
        case ResourceTypeId::TILE_SET:
            auto newResource_tileSet = std::make_shared<TileSet>(resourcePath);
            newResource_tileSet.load(resourcePath);
            return newResource_tileSet;
        break;
        case ResourceTypeId::SOUND:
            auto newResource_Sound = std::make_shared<Sound>(resourcePath);
            newResource_Sound.load(resourcePath);
            return newResource_Sound;
        break;
        default:
            return std::shared_ptr<Resource>();
        break;
    }
}

ResourceFactory::ResourceFactory()
{

}

void ResourceFactory::add(ResourceTypeId typeId, std::shared_ptr<Resource> resourceToAdd)
{
    auto resourceContainerPtr = m_resourceTypesMapping[typeId];
    if(resourceContainerPtr != nullptr)
    {
        if(resourceContainerPtr.contains(resourceToAdd->getName()))
        {
            std::cerr<<"warning : resource can't be added to the resourceFactory, because there is already a resource with name : "<resourceToAdd->getName()<<std::endl;
            return;
        }
        resourceContainerPtr[resourceToAdd->getName()] = resourceToAdd;
    }
    else
    {
        std::cerr<<"error : There is no valid resource container which match with the given key."<<std::endl;
    }
}

void ResourceFactory::add(ResourceTypeId typeId, const std::string& resourceName, const std::string& resourcePath)
{
    auto resourceContainerPtr = m_resourceTypesMapping[typeId];
    if(resourceContainerPtr != nullptr)
    {
        if(resourceContainerPtr.contains(resourceName))
        {
            std::cerr<<"warning : resource can't be added to the resourceFactory, because there is already a resource with name : "<resourceName<<std::endl;
            return;
        }
        resourceContainerPtr[resourceName] = loadResource(typeId, resourcePath);
    }
    else
    {
        std::cerr<<"error : There is no valid resource container which match with the given key."<<std::endl;
    }
}

void ResourceFactory::add(ResourceTypeId typeId, std::vector<std::pair<const std::string&, const std::string&>> resourcesNamesPathes)
{
    auto resourceContainerPtr = m_resourceTypesMapping[typeId];
    if(resourceContainerPtr != nullptr)
    {
        for(auto& namePath : resourcesNamesPathes)
        {
            if(resourceContainerPtr.contains(namePath.first))
            {
                std::cerr<<"warning : resource can't be added to the resourceFactory, because there is already a resource with name : "<resourceName<<std::endl;
                return;
            }
            resourceContainerPtr[namePath.first] = loadResource(typeId, namePath.second);
        }
    }
    else
    {
        std::cerr<<"error : There is no valid resource container which match with the given key."<<std::endl;
    }
}

void ResourceFactory::remove(ResourceTypeId typeId, const std::string& resourceName)
{
    auto resourceContainerPtr = m_resourceTypesMapping[typeId];
    if(resourceContainerPtr != nullptr)
    {
        resourceContainerPtr.erase(resourceName);
    }
    else
    {
        std::cerr<<"error : There is no valid resource container which match with the given key."<<std::endl;
    }
}

void ResourceFactory::clear()
{
    m_textures.clear();
    m_tileSets.clear();
    m_sounds.clear();

    m_resourceTypesMapping.clear();
}

void ResourceFactory::load(std::istream& stream)
{
    int resourceCount = 0;
    std::string resourceName;
    std::string resourcePath;


    for(int j = 0; j < ResourceTypeId::RESOURCE_TYPE_COUNT; j++)
    {
        stream>>resourceCount;
        for(int i = 0; i < resourceCount; i++)
        {
            stream>>resourceName
                  >>resourcePath;

            add(j, resourceName, resourcePath);
        }
    }

    /*
    stream>>resourceCount;
    for(int i = 0; i < resourceCount; i++)
    {
        stream>>resourceType
              >>resourceName
              >>resourcePath;

        add(resourceType, resourceName, resourcePath);
    }

    stream>>resourceCount;
    for(int i = 0; i < resourceCount; i++)
    {
        stream>>resourceType
              >>resourceName
              >>resourcePath;

        add(resourceType, resourceName, resourcePath);
    }

    stream>>resourceCount;
    for(int i = 0; i < resourceCount; i++)
    {
        stream>>resourceType
              >>resourceName
              >>resourcePath;

        add(resourceType, resourceName, resourcePath);
    }*/
}

void ResourceFactory::save(std::ostream& stream)
{

    for(int j = 0; j < ResourceTypeId::RESOURCE_TYPE_COUNT; j++)
    {
        stream<<m_resourceTypesMapping[j]->size();
        for(auto& keyValue : *m_resourceTypesMapping[j])
        {
            stream<<keyValue.first
                  <<keyValue.second->getPath();
        }
    }
    /*
    stream<<m_textures.size();
    for(auto& keyValue : m_textures)
    {
        stream<<keyValue.first
              <<keyValue.second->getName()
              <<keyValue.second->getPath();
    }

    stream<<m_tileSets.size();
    for(auto& keyValue : m_tileSets)
    {
        stream<<keyValue.first
              <<keyValue.second->getName()
              <<keyValue.second->getPath();
    }

    stream<<m_sounds.size();
    for(auto& keyValue : m_sounds)
    {
        stream<<keyValue.first
              <<keyValue.second->getName()
              <<keyValue.second->getPath();
    }*/
}

