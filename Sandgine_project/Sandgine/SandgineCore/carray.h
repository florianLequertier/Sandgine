#ifndef CARRAY_H
#define CARRAY_H

#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <typeinfo>
#include <typeindex>

template<class T>
struct Handler;

class BaseWorld;

class BaseCArray
{
private:
    int m_capacity;
    int m_size;

    std::vector<bool> m_states;
    std::vector<int> m_valuePtrs;
    std::vector<int> m_empties;
    std::vector<int> m_generations;

public:

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
    std::vector<T> m_values;

public:

    CArray();
    ~CArray();

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
        activate(index);
        m_valuePtrs[index] = element;
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


    template<typename T>
    friend std::ostream& operator<<(std::ostream& stream, const CArray& array);
    template<typename T>
    friend std::istream& operator>>(std::istream& stream, const CArray& array);

    void save(BaseWorld& world, std::ostream& stream);
    void load(BaseWorld& world, std::istream& stream);

};

#include "baseworld.h"

template<typename T>
void CArray::save(BaseWorld& world, std::ostream& stream)
{
    stream<<m_capacity
          <<m_size;

    for(int i = 0; i < m_size; i++)
    {
        m_values[i].save(world, stream);

        stream<<m_states[i]
              <<m_valuePtrs[i]
              <<m_empties[i]
              <<m_generations[i];
    }
}

template<typename T>
void CArray::load(BaseWorld& world, std::istream& stream)
{
    stream>>m_capacity
          >>m_size;

    resize(m_capacity);

    for(int i = 0; i < m_size; i++)
    {
        m_values[i].load(world, stream);

        stream>>m_states[i]
              >>m_valuePtrs[i]
              >>m_empties[i]
              >>m_generations[i];
    }
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const CArray<T>& array)
{
    stream<<array.m_capacity
          <<array.m_size;

    for(int i = 0; i < array.m_size; i++)
    {
        stream<<array.m_values[i]
              <<array.m_states[i]
              <<array.m_valuePtrs[i]
              <<array.m_empties[i]
              <<array.m_generations[i];
    }
}

template<typename T>
std::istream& operator>>(std::istream& stream, const CArray<T>& array)
{
    stream>>array.m_capacity
          >>array.m_size;

    array.resize(array.m_capacity);

    for(int i = 0; i < array.m_size; i++)
    {
        stream>>array.m_values[i]
              >>array.m_states[i]
              >>array.m_valuePtrs[i]
              >>array.m_empties[i]
              >>array.m_generations[i];
    }
}

template<typename T>
CArray<T>::CArray() : m_size(0)
{
    resize(1024);
}

template<typename T>
CArray<T>::~CArray()
{
}

/**
 * @brief clear the CArray without destroy it in memory.
 */
template<typename T>
void CArray<T>::clear()
{
    m_size = 0;
    resize(m_capacity);
}


/**
 * @brief Simply resize the array.
 * @param size
 */
template<typename T>
void CArray<T>::resize(unsigned int size)
{
    m_capacity = size;
    if(m_capacity < size)
        m_size = m_capacity;

    m_values.resize(m_capacity);
    m_states.resize(m_capacity);
    m_valuePtrs.resize(m_capacity);
    m_empties.resize(m_capacity);
    m_generations.resize(m_capacity);

    for(int i = m_size; i < m_capacity; i++)
    {
        m_states[i] = false;
        m_valuePtrs[i] = i;
        m_empties[i] = i;
        m_generations[i] = 0;
    }
}





#endif // CARRAY_H
