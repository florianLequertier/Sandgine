#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>

#include "resource.h"

int Resource::count = 0;

 std::string Resource::getNameFromPath(const std::string& path)
 {
     auto last = path.find_last_of('.');
     auto first = std::max(path.find_last_of('\\'), path.find_last_of('/'));
     return path.substr(first+1, last-(first+1));
 }


 ////////////////////////////////////////
 /////////////// RESOURCE ///////////////
 ////////////////////////////////////////

Resource::Resource(ResourceType type, std::string path) : m_id(0)
{
    m_id = count;
    count++;
    m_type = type;
    m_path.append(path);
    m_name = getNameFromPath(path);
}

Resource::Resource(Resource::ResourceType type, std::vector<std::string> paths) : m_id(0)
{
    m_type = type;
    if(!paths.isEmpty())
    {
        m_path = paths;
        m_name = getNameFromPath(paths[0]);
    }
}

void Resource::setId(int id)
{
   m_id = id;
}

int Resource::getId() const
{
    return m_id;
}

std::string Resource::getName()
{
    return m_name;
}

Resource::ResourceType Resource::getResourceType()
{
    return m_type;
}

std::string Resource::path(int index)
{
    if(index < m_path.count())
        return m_path[index];
    else
        return "";
}

unsigned int Resource::getPathCount()
{
    return m_path.count();
}

glm::vec3 Resource::getTypeColor()
{
    switch(m_type)
    {
        case IMAGE :
        return glm::vec3(1.f,1.f,0.f);
        break;
        case SOUND :
        return glm::vec3(1.f,0.f,0.f);
        break;
        case TILESET :
        return glm::vec3(0.f,1.f,0.f);
        break;
        case SPRITE_INFO :
        return glm::vec3(0.f,0.5f,0.f);
        break;
        default :
        return glm::vec3(0.F,0.f,0.f);
        break;
    }
}

void Resource::setName(const std::string& newName)
{
    m_name = newName;
}

void Resource::load(std::ifstream& stream, const std::string& path)
{
    stream.seekg(0, stream.beg);

    std::string findResult;

    while(!stream.eof())
    {
        std::getline( stream, findResult);

        std::stringstream ss(findResult);

        std::string pathResult;
        ss>>pathResult;
        if(pathResult == path)
        {
            ss >> m_name;
            int typeInt;
            ss >> typeInt;
            m_type = (ResourceType)typeInt;

            while(!ss.eof())
            {
                ss>>pathResult;
                m_path.push_back(pathResult);
            }
        }
    }
}

void Resource::save(std::ofstream& stream, const std::string& path)
{
    stream<<m_name<<(int)m_type;
    for(int i = 0; i < m_path.size(); i++)
    {
        stream<<m_path[i];
    }
    stream<<std::endl;
}


//////////////////////////
/// Sound
//////////////////////////

Sound::Sound(std::string path) : Resource(Resource::SOUND, path)
{

}

QWidget *Sound::getHandler()
{
    QWidget* handler = new QWidget;

    QVBoxLayout* mainLayout = new QVBoxLayout;

    QHBoxLayout* editFieldTitle = new QHBoxLayout;
    QLabel* icon = new QLabel;
    icon->setPixmap(QPixmap(qApp->applicationDirPath()+"/icons/Music.png"));
    icon->setMinimumSize(64,64);
    QLabel* title = new QLabel("Sound");
    editFieldTitle->addWidget(icon);
    editFieldTitle->addWidget(title);

    QFormLayout* formLayout = new QFormLayout;
    QLineEdit* nameEdit = new QLineEdit(m_name);
    nameEdit->setReadOnly(true);
    formLayout->addRow("name : ", nameEdit);

//    QHBoxLayout* editFieldLayout = new QHBoxLayout;
//    QLabel* name = new QLabel("name : ");
//    QLineEdit* nameEdit = new QLineEdit(m_name);
//    nameEdit->setReadOnly(true);
//    editFieldLayout->addWidget(name);
//    editFieldLayout->addWidget(nameEdit);

    mainLayout->addLayout(editFieldTitle);
    mainLayout->addLayout(formLayout);

    handler->setLayout(mainLayout);

    QObject::connect(nameEdit, SIGNAL(textEdited(QString)), this, SLOT(setName(QString)) );

    return handler;
}


///////////////////////////////////
/// TILESETINFO
//////////////////////////////////

TileSetInfo::TileSetInfo(std::string imgPath, std::string txtPath): Resource(Resource::TILESET, {imgPath, txtPath})
{
    m_id = TileSetManager::Instance().loadTileSet(imgPath, txtPath);
    loadNewSet(txtPath, imgPath);
}

void TileSetInfo::loadNewSet(std::string infoPath, std::string _texturePath)
{
    m_texturePath = _texturePath;
    QFile file(infoPath);

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream ifile(&file);

        QString mot;
        int nombre;

        ifile >> mot;
        std::cout << "nom : " << mot.toStdString() << std::endl;
        m_nomSet = mot;
        ifile >> mot;
        std::cout << "type : " << mot.toStdString() << std::endl;
        m_typeSet = mot;
        int tmpWidth = 0;

        if (m_typeSet == "staticSet")
        {
            ifile >> mot;
            m_nbTile = mot.toInt();
            std::cout << "nbTile : " << mot.toInt() << std::endl;

            m_animX = 1;
            m_animY = 1;

            while (!ifile.atEnd())
            {

                ifile >> mot;
                nombre = mot.toInt();
                m_tabNbTileByRow.push_back(nombre);
                std::cout << "nbTileByRow : " << mot.toInt() << std::endl;

                ifile >> mot;
                nombre = mot.toInt();
                m_tabHeightOfRow.push_back(nombre);
                std::cout << "heightOfRow : " << mot.toInt() << std::endl;
                m_height += nombre;

                tmpWidth = 0;
                for (int i = 0; i<m_tabNbTileByRow[m_tabNbTileByRow.size()-1]; i++)
                {
                    ifile >> mot;
                    nombre = mot.toInt();
                    m_tabWidths.push_back(nombre);
                    std::cout << "width : " << mot.toInt() << std::endl;

                    tmpWidth += nombre;
                }
                if(tmpWidth > m_width)
                    m_width = tmpWidth;
            }
        }
        else if (m_typeSet == "animationSet")
        {
            ifile >> mot;
            m_nbTile = mot.toInt(); //ne compte pas les tiles d'animations
            std::cout << "nbTile : " << mot.toInt() << std::endl;

            ifile >> mot;
            m_animX = mot.toInt();
            std::cout << "animX : " << mot.toInt() << std::endl;
            ifile >> mot;
            m_animY = mot.toInt();
            std::cout << "animY : " << mot.toInt() << std::endl;

            tmpWidth = 0;
            while (!ifile.atEnd())
            {

                ifile >> mot;
                nombre = mot.toInt();
                m_tabNbTileByRow.push_back(nombre);
                std::cout << "mot : " << mot.toInt() << std::endl;

                ifile >> mot;
                nombre = mot.toInt();
                m_tabHeightOfRow.push_back(nombre);
                std::cout << "mot : " << mot.toInt() << std::endl;
                m_height += nombre;

                for (int i = 0; i<m_tabNbTileByRow.size(); i++)
                {
                    ifile >> mot;
                    nombre = mot.toInt();
                    m_tabWidths.push_back(nombre);
                    std::cout << "mot : " << mot.toInt() << std::endl;

                    tmpWidth += nombre;
                }
                if(tmpWidth > m_width)
                    m_width = tmpWidth;
            }
        }
        else
        {
            std::cerr << "tileset de type inconnu" << std::endl;
        }

    }
    else
    {
        std::cerr << "erreur d'ouverture du fichier : " << infoPath.toStdString() << std::endl;
    }
}

TileSetInfo::~TileSetInfo()
{

}

//TileSetInfo::TileSetInfo(const TileSetInfo& other)
//{
//    Resource(other);

//    m_name = other.m_name;
//    m_nomSet = other.m_nomSet;
//    m_typeSet = other.m_typeSet;
//    m_width = other.m_width;
//    m_height = other.m_height;
//    m_nbTile = other.m_nbTile;
//    m_animX = other.m_animX,
//    m_animY = other.m_animY;

//    m_tabHeightOfRow = other.m_tabHeightOfRow;
//    m_tabNbTileByRow = other.m_tabNbTileByRow;
//    m_tabWidths = other.m_tabWidths;

//    m_texturePath = other.m_texturePath;
//    m_textureSet = other.m_textureSet;
//}

int TileSetInfo::getTileWidth(int numTile)
{
    if (numTile<m_tabWidths.size())
        return m_tabWidths[numTile];
}

int TileSetInfo::getHeightOfRow(int numRow)
{
    if (numRow<m_tabHeightOfRow.size())
        return m_tabHeightOfRow[numRow];
}

int TileSetInfo::tileCountByRow(int numRow)
{
    if (numRow<m_tabNbTileByRow.size())
        return m_tabNbTileByRow[numRow];
}

QVector4D TileSetInfo::catchTile(int numeroTile) //Choppe toujour la premiere tile de la serie d'animation lorsque la tileset est animable
{
    //std::cout << "coucou" << std::endl;

    int res = numeroTile;
    int xT = 0, hT = 0, yT = 0, wT = 0;
    int y = 0, x = 0;

    if (m_typeSet == "staticSet")
    {
        while (tileCountByRow(y)-1 < res)
        {
            yT += getHeightOfRow(y);
            res -= tileCountByRow(y);
            y++;
        }//hauteur effectué
        for (int i = res; res > 0; res--)
        {
            xT += getTileWidth((numeroTile - res));
            x++;
        }//largeur effectuée
    }
    else if (m_typeSet == "animationSet")
    {
        yT = numeroTile*getHeightOfRow(y)*(m_animY);
        xT = 0;
    }
    else
    {
        std::cerr << "set de type inconnu" << std::endl;
    }


    hT = getHeightOfRow(y);
    wT = getTileWidth((numeroTile - res));
    //cout << "test tile" << endl;
    //for (int i = 0; i < m_nbTile; i++)
    //cout << "tile width " << getTileWidth(i) << endl;

    QVector4D selection(xT, yT, wT, hT);
    //std::cout << "selection : " << selection.width << " , " << selection.height << " , " << selection.left << " , " << selection.top << std::endl;
    return selection;
}

QVector4D TileSetInfo::catchAnimTile(int numeroTile) //Va chercher la tile animé si le tileset est de type animable
{
    //std::cout << "coucou" << std::endl;

    int res = numeroTile;
    int xT = 0, hT = 0, yT = 0, wT = 0;
    int y = 0, x = 0;

    if (m_typeSet == "staticSet")
    {
        while (tileCountByRow(y) < res)
        {
            yT += getHeightOfRow(y);
            res -= tileCountByRow(y);
            y++;
        }//hauteur effectué
        for (int i = res; res>0; res--)
        {
            xT += getTileWidth((numeroTile - res));
            x++;
        }//largeur effectuée
    }
    else if (m_typeSet == "animationSet")
    {
        while (tileCountByRow(y) < res)
        {
            yT += getHeightOfRow(y);
            res -= tileCountByRow(y);
            y++;
        }//hauteur effectué
        for (int i = res; res>0; res--)
        {
            xT += getTileWidth((numeroTile - res));
            x++;
        }//largeur effectuée

    }
    else
    {
        std::cerr << "set de type inconnu" << std::endl;
    }


    hT = getHeightOfRow(y);
    wT = getTileWidth((numeroTile - res));

    QVector4D selection(xT, yT, wT, hT);
    //std::cout << "selection : " << selection.width << " , " << selection.height << " , " << selection.left << " , " << selection.top << std::endl;
    return selection;
}

QVector2D TileSetInfo::getTextureDimensions()
{
    return QVector2D(m_width, m_height);
}


int TileSetInfo::getAnimX() const
{
    return m_animX;
}

int TileSetInfo::getAnimY() const
{
    return m_animY;
}

QString TileSetInfo::getType() const
{
    return m_typeSet;
}

QString TileSetInfo::getTexturePath() const
{
    return m_texturePath;
}

int TileSetInfo::tileCount() const
{
    return m_nbTile;
}

QWidget *TileSetInfo::getHandler()
{
    QWidget* handler = new QWidget;

    QVBoxLayout* mainLayout = new QVBoxLayout;

    QHBoxLayout* editFieldTitle = new QHBoxLayout;
    QLabel* icon = new QLabel;
    icon->setPixmap(QPixmap(path()));
    icon->setMinimumSize(64,64);
    QLabel* title = new QLabel("TileSet");
    editFieldTitle->addWidget(icon);
    editFieldTitle->addWidget(title);

    QFormLayout* formLayout = new QFormLayout;
    QLineEdit* nameEdit = new QLineEdit(m_name);
    nameEdit->setReadOnly(true);
    formLayout->addRow("name : ", nameEdit);

    mainLayout->addLayout(editFieldTitle);
    mainLayout->addLayout(formLayout);

    handler->setLayout(mainLayout);

    QObject::connect(nameEdit, SIGNAL(textEdited(QString)), this, SLOT(setName(QString)) );

    return handler;
}


///////////////////////////////////
/// SPRITE_INFO
//////////////////////////////////


SpriteInfo::SpriteInfo(TileSetInfo* tileSetPtr, unsigned int indice) : Resource(Resource::SPRITE_INFO, ""), m_tileSetPtr(tileSetPtr), m_indice(indice)
{
    m_id = tileSetPtr->getId();
    m_name = QString::fromStdString(std::to_string(indice));
}

unsigned int SpriteInfo::index()
{
    return m_indice;
}

QWidget *SpriteInfo::getHandler()
{
    QWidget* handler = new QWidget;

    QVBoxLayout* mainLayout = new QVBoxLayout;

    QHBoxLayout* editFieldTitle = new QHBoxLayout;
    QLabel* icon = new QLabel(handler);
    QPixmap* pixmapIcon= QSFMLHelper::PathToPixmap(m_tileSetPtr->getTexturePath(), m_tileSetPtr->catchTile(m_indice));
    icon->setPixmap(*pixmapIcon);
    icon->setMinimumSize(64,64);
    QLabel* title = new QLabel("SpriteInfo", handler);
    editFieldTitle->addWidget(icon);
    editFieldTitle->addWidget(title);

    QFormLayout* formLayout = new QFormLayout;
    QLineEdit* nameEdit = new QLineEdit(m_name, handler);
    nameEdit->setReadOnly(true);
    formLayout->addRow("name : ", nameEdit);

    mainLayout->addLayout(editFieldTitle);
    mainLayout->addLayout(formLayout);

    handler->setLayout(mainLayout);

    QObject::connect(nameEdit, SIGNAL(textEdited(QString)), this, SLOT(setName(QString)) );

    return handler;
}

QString SpriteInfo::getTexturePath() const
{
    return m_tileSetPtr->getTexturePath();
}

QVector4D SpriteInfo::getTextureRect() const
{
    return m_tileSetPtr->catchTile(m_indice);
}



