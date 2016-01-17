#ifndef CARRAY_H
#define CARRAY_H

#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <typeinfo>
#include <typeindex>

#include "worldobject.h"

template<class T>
struct Handler;

class BaseWorld;

class BaseCArray
{
protected:
    int m_capacity;
    int m_size;

    std::vector<bool> m_states;
    std::vector<int> m_valuePtrs;
    std::vector<int> m_empties;
    std::vector<int> m_generations;

    std::vector<WorldObject*> m_basePtrs;

public:

    BaseCArray();
    virtual ~BaseCArray();

    /**
     * @brief return the value pointed in pointers container as WorldObject
     * @param i
     * @return
     */
    WorldObject& operator[] (unsigned int i);

    /**
     * @brief The size of the array.
     * @return
     */
    int size();

    /**
     * @brief The capacity of the array.
     * @return
     */
    int capacity();

    /**
     * @brief Return the next inactive element in the array. Warning : It doesn't remove it from m_empties. You have to do it manualy if it is necessary.
     * @return
     */
    int getNextEmptyIndex();

    /**
     * @brief get the generation of the element at index "index"
     * @param index
     * @return
     */
    int getGeneration(int index);

    /**
     * @brief Activate an element of the array.
     * @param index
     */
    void activate(unsigned int index);

    /**
     * @brief Desactivate an element of the array.
     * @param index
     */
    void desactivate(unsigned int index);

    /**
     * @brief Remove an element of the array at given index. (match the index in m_valuePtrs, not m_values)
     * @param index
     */
    bool remove(int index);

    /**
     * @brief return true if the element at given index is active in the array, and the generations matches.
     * @param index
     * @param generation
     * @return
     */
    bool isValid(unsigned int index, unsigned int generation) const;

    /**
     * @brief Return the index in m_container of the i th member of m_paluePtrs. Return the real index of the active element i
     * @param i
     * @return
     */
    int indexOf(int i);

};

template<class T>
class CArray : public BaseCArray
{
private:
    typedef BaseCArray base;
    using BaseCArray::operator [];

    std::vector<T> m_values;

public:

    CArray();
    virtual ~CArray();

    /**
     * @brief clear the CArray without destroy it in memory.
     */
    void clear();

    /**
     * @brief Simply resize the array.
     * @param size
     */
    void resize(unsigned int size);

    /**
     * @brief Activate an element of the array.
     * @param index
     */
    void activate(unsigned int index, const T& element)
    {
        base::activate(index);
        m_values[m_valuePtrs[index]] = element;
    }

    /**
     * @brief Sort the internal array (ie : m_values). The "pointers" in m_valuesPtrs keep valid.
     */
    void sort();

    /**
     * @brief return the value pointed in pointers container
     * @param i
     * @return
     */
    T& operator[] (unsigned int i)
    {
        return m_values[m_valuePtrs[i]];
    }
    /**
     * @brief encapsulate the index into an handler pointing to this CArray
     * @param index
     * @return
     */
    Handler<T> getHandler(unsigned int index)
    {
        return Handler<T>(this, index);
    }

    /**
     * @brief parse the values container without passing by the pointers container
     * @param i
     * @return
     */
    T& parse(unsigned int i)
    {
        return m_values[i];
    }

    /**
     * @brief Swap two elements of the array.
     * @param indexA
     * @param indexB
     */
    void swap(unsigned int indexA, unsigned int indexB)
    {
        T temp = m_values[m_valuePtrs[indexA]];
        m_values[m_valuePtrs[indexA]] = m_values[m_valuePtrs[indexB]];
        m_values[m_valuePtrs[indexB]] = temp;

        WorldObject tempPtr = m_basePtrs[indexA];
        m_basePtrs[indexA] = m_basePtrs[indexB];
        m_basePtrs[indexB] = temp;

        int tmpIndex = m_valuePtrs[indexA];
        m_valuePtrs[indexA] = m_valuePtrs[indexB];
        m_valuePtrs[indexB] = tmpIndex;
    }

    /**
     * @brief add an element to the array. ( Active an element, or resize the container and then active an element if there are no inactive elements in the array).
     * @return
     */
    Handler<T> add()
    {
        int nextIndex = getNextEmptyIndex();

        if(nextIndex >= m_size)
        {
            resize(m_capacity * 512);
            activate(nextIndex);
        }
        else
        {
            activate(nextIndex);
            m_empties.pop_back();
        }

        return Handler<T>(this, nextIndex);//&m_values[m_valuePtrs[nextIndex]];
    }

    /**
     * @brief add an element to the array. ( Active an element, or resize the container and then active an element if there are no inactive elements in the array).
     * @return
     */
    Handler<T> add(const T& element)
    {
        int nextIndex = getNextEmptyIndex();

        if(nextIndex >= m_size)
        {
            resize(m_capacity * 512);
            activate(nextIndex, element);
        }
        else
        {
            activate(nextIndex, element);
            m_empties.pop_back();
        }

        return Handler<T>(this, nextIndex);//&m_values[m_valuePtrs[nextIndex]];
    }

    void save(std::ostream& stream, BaseWorld* world = nullptr );
    void load(std::istream& stream, BaseWorld* world = nullptr );

};


#endif // CARRAY_H
