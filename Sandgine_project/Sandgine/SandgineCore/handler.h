#ifndef HANDLER_H
#define HANDLER_H

#include <iostream>

#include "carray.h"

struct InternalHandler
{
    int m_index;
    int m_generation;
    std::type_index m_typeId;

    inline InternalHandler(int index, int generation, std::type_index typeId) : m_index(index), m_generation(generation), m_typeId(typeId)
    {

    }

    inline InternalHandler(const Handler& handler)
    {
        m_index = handler.m_index;
        m_generation = handler.m_generation;
        m_typeId = handler.m_typeId;
    }

    template<typename T>
    friend std::ostream& operator<<(std::ostream& stream, const InternalHandler<T>& internalHandler);
    template<typename T>
    friend std::istream& operator>>(std::istream& stream, InternalHandler<T>& internalHandler);
};

template<typename T>
std::ostream& operator<<(std::ostream& stream, const InternalHandler<T>& internalHandler)
{
    stream<<internalHandler.m_index<<internalHandler.m_generation<<internalHandler.m_typeId;
}

template<typename T>
std::istream& operator>>(std::istream& stream, InternalHandler<T>& internalHandler)
{
    stream>>internalHandler.m_index>>internalHandler.m_generation>>internalHandler.m_typeId;
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
    std::type_info m_typeId;

    inline Handler(CArray<T>* arrayPtr = nullptr, int index = -1): m_datas(arrayPtr), m_index(index)
    {
        m_generation = m_datas->getGeneration(m_index);
        m_typeId = typeid(T);
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

    template<typename T>
    friend std::ostream& operator<<(std::ostream& stream, const Handler<T>& handler);

};

template<typename T>
std::ostream& operator<<(std::ostream& stream, const Handler<T>& handler)
{
    stream<<handler.m_index<<handler.m_generation<<handler.m_typeId;
}


#endif // HANDLER_H
