#include "baseworld.h"
#include "carray.h"

BaseCArray::BaseCArray() : m_size(0), m_capacity(0)
{

}

BaseCArray::~BaseCArray()
{

}

/**
 * @brief return the value pointed in pointers container as WorldObject
 * @param i
 * @return
 */
WorldObject& BaseCArray::operator[] (unsigned int i)
{
    return *m_basePtrs[m_valuePtrs[i]];
}

/**
 * @brief The size of the array.
 * @return
 */
int BaseCArray::size()
{
    return m_size;
}

/**
 * @brief The capacity of the array.
 * @return
 */
int BaseCArray::capacity()
{
    return m_capacity;
}

/**
 * @brief Return the next inactive element in the array. Warning : It doesn't remove it from m_empties. You have to do it manualy if it is necessary.
 * @return
 */
int BaseCArray::getNextEmptyIndex()
{
    if(!m_empties.empty())
    {
        return m_empties.back();
    }
    else
    {
        return m_size;
    }
}

/**
 * @brief return true if the element at given index is active in the array, and the generations matches.
 * @param index
 * @param generation
 * @return
 */
bool BaseCArray::isValid(unsigned int index, unsigned int generation) const
{
    return ( index >=0
             && index < m_size
             && m_states[index] == true
             && m_generations[index] == generation);
}

/**
 * @brief return the generation of the element at index "index"
 * @param index
 * @return
 */
int BaseCArray::getGeneration(int index)
{
    return m_generations[index];
}

/**
 * @brief Activate an element of the array.
 * @param index
 */
void BaseCArray::activate(unsigned int index)
{
    m_generations[index]++;
    m_states[index] = true;
}

/**
 * @brief Desactivate an element of the array.
 * @param index
 */
void BaseCArray::desactivate(unsigned int index)
{
    m_states[index] = false;
    m_empties.push_back(index);
}

/**
 * @brief Remove an element of the array at given index. (match the index in m_valuePtrs, not m_values)
 * @param index
 */
bool BaseCArray::remove(int index)
{
    auto it = std::find(m_valuePtrs.begin(), m_valuePtrs.end(), index);

    if(it != m_valuePtrs.end())
    {
        m_empties.push_back(index);
        m_states[index] = false;
        return true;
    }
    return false;
}

//return the index in m_container of the i th member of m_paluePtrs
// return the real index of the active element i
int BaseCArray::indexOf(int i)
{
    if(i >= 0 && i < m_valuePtrs.size())
        return m_valuePtrs[i];
    else
        return -1;
}


///////////////////////////////
////////////CARRAY////////////
//////////////////////////////


template<class T>
void CArray<T>::sort()
{
    //nothing
}



template<typename T>
void CArray<T>::save(std::ostream& stream, BaseWorld* world)
{
    stream<<m_capacity
          <<m_size;

    for(int i = 0; i < m_size; i++)
    {
        m_values[i].save(stream, world);

        stream<<(int)m_states[i]
              <<m_valuePtrs[i]
              <<m_empties[i]
              <<m_generations[i];
    }
}

template<typename T>
void CArray<T>::load(std::istream& stream, BaseWorld* world)
{
    stream>>m_capacity
          >>m_size;

    resize(m_capacity);

    for(int i = 0; i < m_size; i++)
    {
        m_values[i].load(stream, world);
        m_basePtrs[i] = &m_values[i];

        int boolInt = 0;
        stream>>boolInt;
              m_states[i] = (boolInt == 0) ? false : true;
        stream>>m_valuePtrs[i]
              >>m_empties[i]
              >>m_generations[i];
    }
}

template<typename T>
CArray<T>::CArray()
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
    m_basePtrs.resize(m_capacity);

    for(int i = m_size; i < m_capacity; i++)
    {
        m_states[i] = false;
        m_valuePtrs[i] = i;
        m_empties[i] = i;
        m_generations[i] = 0;
        m_basePtrs[i] = nullptr;
    }
}





