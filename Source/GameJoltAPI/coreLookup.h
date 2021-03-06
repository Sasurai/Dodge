//////////////////////////////////////////////////////////
//*----------------------------------------------------*//
//| Part of the Core Engine (http://www.maus-games.at) |//
//*----------------------------------------------------*//
//| Released under the zlib License                    |//
//| More information available in the README.md        |//
//*----------------------------------------------------*//
//////////////////////////////////////////////////////////
#pragma once
#ifndef _CORE_GUARD_LOOKUP_H_
#define _CORE_GUARD_LOOKUP_H_

typedef unsigned int coreUint;


// ****************************************************************
// lookup container class
// NOTE: much faster than map and u_map with only few items
// TODO: implement quicksort and binary search
// TODO: implement access cache
// TODO: measure performance with high amount of entries (>200)
template <typename T> class coreLookup final
{
public:
    //! internal types
    typedef std::pair<std::string, T>         corePair;
    typedef std::vector<corePair>             coreList;
    typedef typename coreList::iterator       coreIterator;
    typedef typename coreList::const_iterator coreConstIterator;


private:
    coreList m_aList;   //!< vector-list with pair-values


public:
    coreLookup()noexcept;
    coreLookup(const coreLookup<T>& c)noexcept;
    coreLookup(coreLookup<T>&& m)noexcept;
    ~coreLookup();

    //! assignment operator
    //! @{
    coreLookup<T>& operator = (coreLookup<T> o)noexcept;
    template <typename S> friend void swap(coreLookup<S>& a, coreLookup<S>& b)noexcept;
    //! @}

    //! access specific entry
    //! @{
    const T& at(const char* pcKey)const noexcept;
    T& operator [] (const char* pcKey)noexcept;
    inline T& operator [] (const coreUint& iIndex)noexcept {return m_aList[iIndex].second;}
    //! @}

    //! check number of existing entries
    //! @{
    inline coreUint count(const char* pcKey)const noexcept {return this->__check(this->__retrieve(pcKey)) ? 1 : 0;}
    inline coreUint size()const noexcept                   {return m_aList.size();}
    inline bool empty()const noexcept                      {return m_aList.empty();}
    //! @}

    //! remove existing entries
    //! @{
    void erase(const char* pcKey)noexcept;
    inline void erase(const coreUint& iIndex)noexcept {/*assert(iIndex < m_aList.size());*/ m_aList.erase(m_aList.begin()+iIndex);}
    inline void clear()noexcept                       {m_aList.clear();}
    //! @}

    //! retrieve internal iterator
    //! @{
    inline coreIterator begin()noexcept            {return m_aList.begin();}
    inline coreConstIterator begin()const noexcept {return m_aList.begin();}
    inline coreIterator end()noexcept              {return m_aList.end();}
    inline coreConstIterator end()const noexcept   {return m_aList.end();}
    //! @}


private:
    //! retrieve iterator by specific key
    //! @{
    coreIterator __retrieve(const char* pcKey)noexcept;
    coreConstIterator __retrieve(const char* pcKey)const noexcept;
    //! @}

    //! check for valid iterator
    //! @{
    inline bool __check(const coreIterator& it)const noexcept      {return (it != m_aList.end()) ? true : false;}
    inline bool __check(const coreConstIterator& it)const noexcept {return (it != m_aList.end()) ? true : false;}
    //! @}
};


// ****************************************************************
// constructor
template <typename T> coreLookup<T>::coreLookup()noexcept
{
    // reserve variable sized memory
    constexpr_var coreUint iSize = 1 + 64/sizeof(T);
    m_aList.reserve(iSize);
}

template <typename T> coreLookup<T>::coreLookup(const coreLookup<T>& c)noexcept
: m_aList (c.m_aList)
{
}

template <typename T> coreLookup<T>::coreLookup(coreLookup<T>&& m)noexcept
: m_aList (std::move(m.m_aList))
{
}


// ****************************************************************
// destructor
template <typename T> coreLookup<T>::~coreLookup()
{
    m_aList.clear();
}


// ****************************************************************
// assignment operator
template <typename T> coreLookup<T>& coreLookup<T>::operator = (coreLookup<T> o)noexcept
{
    swap(*this, o);
    return *this;
}

template <typename S> void swap(coreLookup<S>& a, coreLookup<S>& b)noexcept
{
    using std::swap;
    swap(a.m_aList, b.m_aList);
}


// ****************************************************************
// access specific entry
template <typename T> const T& coreLookup<T>::at(const char* pcKey)const noexcept
{
    // retrieve and check iterator by specific key
    auto it = this->__retrieve(pcKey);
    /*assert(this->__check(it));*/

    return it->second;
}


// ****************************************************************
// access specific entry and create it if necessary
template <typename T> T& coreLookup<T>::operator [] (const char* pcKey)noexcept
{
    // retrieve and check iterator by specific key
    auto it = this->__retrieve(pcKey);
    if(!this->__check(it))
    {
        // create new entry
        m_aList.push_back(corePair(pcKey, T()));
        it = m_aList.end()-1;
    }

    return it->second;
}


// ****************************************************************
// remove existing entry
template <typename T> void coreLookup<T>::erase(const char* pcKey)noexcept
{
    // retrieve and check iterator by specific key
    auto it = this->__retrieve(pcKey);
    if(this->__check(it))
    {
        // remove existing entry
        m_aList.erase(it);
    }
}


// ****************************************************************
// retrieve iterator to specific key
template <typename T> typename coreLookup<T>::coreIterator coreLookup<T>::__retrieve(const char* pcKey)noexcept
{
    // traverse all entries
    FOR_EACH(it, m_aList)
    {
        // compare string-keys
        if(!std::strcmp(it->first.c_str(), pcKey))
            return it;
    }
    return m_aList.end();
}

template <typename T> typename coreLookup<T>::coreConstIterator coreLookup<T>::__retrieve(const char* pcKey)const noexcept
{
    // traverse all entries
    FOR_EACH(it, m_aList)
    {
        // compare string-keys
        if(!std::strcmp(it->first.c_str(), pcKey))
            return it;
    }
    return m_aList.end();
}


#endif // _CORE_GUARD_LOOKUP_H_