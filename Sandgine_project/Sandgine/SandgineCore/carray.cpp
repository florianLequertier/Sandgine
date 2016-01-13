#include "carray.h"


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
template<typename T>
int CArray<T>::indexOf(int i)
{
    if(i >= 0 && i < m_valuePtrs.size())
        return m_valuePtrs[i];
    else
        return -1;
}



template<class T>
void CArray<T>::sort()
{
    //nothing
}





