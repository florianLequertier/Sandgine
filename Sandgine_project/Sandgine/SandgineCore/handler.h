#ifndef HANDLER_H
#define HANDLER_H

#include <iostream>

#include "worldobject.h"

template<class T>
class CArray;

template<class T>
class Handler;

class BaseCArray;

class World;

/*
 * Only deserializable throught the BaseWorld.readInternalHandler().
 */
struct InternalHandler
{
    int m_index;
    BaseCArray* m_datas;
    int m_generation;
    std::string m_typeId;

    inline InternalHandler(int index = 0, int generation = 0, std::string typeId = "") : m_datas(nullptr), m_index(index), m_generation(generation), m_typeId(typeId)
    {

    }

    template<typename T>
    inline InternalHandler(const Handler<T>& handler)
    {
        m_index = handler.m_index;
        m_generation = handler.m_generation;
        m_typeId = handler.m_typeId;
    }

    WorldObject& get() const;

    bool isNull() const;

    WorldObject* operator->();

    WorldObject& operator*();

    inline operator bool() const
    {
        return !isNull();
    }

    friend std::ostream& operator<<(std::ostream& stream, const InternalHandler& handler);

};

std::ostream& operator<<(std::ostream& stream, const InternalHandler& handler)
{
    stream<<handler.m_index<<handler.m_generation<<handler.m_typeId;
}



/*
 * Only deserializable throught the BaseWorld.readHandler().
 */
template<class T>
struct Handler
{
    int m_index;
    CArray<T>* m_datas;
    int m_generation;
    std::string m_typeId;

    inline Handler(CArray<T>* arrayPtr = nullptr, int index = -1): m_datas(arrayPtr), m_index(index)
    {
        m_generation = m_datas->getGeneration(m_index);
        m_typeId = typeid(T).name();
    }

    T& get() const
    {
        return (*m_datas)[m_index];
    }

    inline bool isNull() const
    {
        return (m_datas == nullptr
                || m_index < 0
                || m_index >= m_datas->size()
                || !m_datas->isValid(m_index, m_generation));
    }

    T* operator->()
    {
        return &(*m_datas)[m_index];
    }

    T& operator*()
    {
        return (*m_datas)[m_index];
    }

    inline operator bool() const
    {
        return !isNull();
    }

    friend std::ostream& operator<<(std::ostream& stream, const Handler<T>& handler);

};

template<typename T>
std::ostream& operator<<(std::ostream& stream, const Handler<T>& handler)
{
    stream<<handler.m_index<<handler.m_generation<<handler.m_typeId;
}


#endif // HANDLER_H
