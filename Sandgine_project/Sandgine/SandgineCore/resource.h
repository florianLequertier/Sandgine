#ifndef RESOURCE_H
#define RESOURCE_H

#include <vector>
#include <string>

class Resource
{
public :
    enum ResourceType{SOUND, IMAGE, TILESET, SPRITE_INFO, NONE};
    static std::string getNameFromPath(const std::string& path);

protected:
    static int count;
    int m_id;
    std::string m_name;
    ResourceType m_type;
    std::vector<std::string> m_path;

public:
    Resource(ResourceType type = Resource::IMAGE, QString path = "");
    Resource(ResourceType type, std::vector<std::string>& paths);

    virtual void load(std::ifstream &stream, const std::string& path);
    virtual void save(std::ofstream &stream, const std::string& path);

    std::string getName();
    ResourceType getResourceType();
    std::string getPath(int index = 0);
    unsigned int getPathCount();

    void setName(std::string& newName);

};


class Sound : public Resource
{
private:

public:
    Sound(std::string path = "");
    virtual QWidget *getHandler() override;



};

class TileSetInfo : public Resource
{
private:
    std::string m_nomSet;
    std::string m_typeSet;
    int m_width;
    int m_height;
    int m_nbTile;
    int m_animX, m_animY;

    std::vector<int> m_tabHeightOfRow;
    std::vector<int> m_tabNbTileByRow;
    std::vector<int> m_tabWidths;

    std::string m_texturePath;
    glm::vec4 m_textureRect;

public:

    TileSetInfo(std::string imgPath = "", std::string txtPath = "");
    void loadNewSet(std::string infoPath, std::string _texturePath);
    //TileSetInfo(const TileSetInfo& other);
    virtual ~TileSetInfo();

    int tileCountByRow(int numRow);
    int getHeightOfRow(int numRow);
    int getTileWidth(int numTile);
    glm::vec4 catchTile(int numeroTile);
    glm::vec4 catchAnimTile(int numeroTile);
    glm::vec4 getTextureDimensions();
    int getAnimX() const;
    int getAnimY() const;
    std::string getType() const;
    std::string getTexturePath() const;
    int tileCount() const;
};

class SpriteInfo : public Resource
{
private :

    TileSetInfo* m_tileSetPtr;
    unsigned int m_indice; // index in set

public:
    SpriteInfo(TileSetInfo* tileSetPtr, unsigned int indice);
    unsigned int index();

    std::string getTexturePath() const;
    glm::vec4 getTextureRect() const;
};

#endif // RESOURCE_H
