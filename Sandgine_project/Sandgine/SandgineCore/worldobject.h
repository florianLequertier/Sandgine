#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include <string>
#include <memory>

class BaseWorld;

class WorldObject
{
protected:
    int m_id;
    std::string m_typeId;
public:
    WorldObject(int id = 0, std::string typeId = "");
    virtual ~WorldObject();
    virtual std::shared_ptr<WorldObject> clone() = 0;
    virtual void save(std::ostream& stream, BaseWorld* world = nullptr );
    virtual void load(std::istream& stream, BaseWorld* world = nullptr );

    std::string getTypeId() const;
    void setTypeId(std::string& type);

    int getId() const;
    void setId(int id);

};

#endif // WORLDOBJECT_H
