#ifndef STRINGSTORAGE_H
#define STRINGSTORAGE_H

#include <string>
#include <map>
#include <vector>
#include <limits>

template< typename T> class MappedStorageIterator;

template <typename T> class MappedStorage
{
    friend class MappedStorageIterator< T >;

    typedef size_t size_type;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
public:
    MappedStorage(){}
    reference operator []( size_type index )
    {
        return m_Data[ index ];
    }
    size_type operator []( value_type data)
    {
        auto found = m_MappedData.find( data );
        if ( found != m_MappedData.end() )
        {
            return found->second;
        } else
        {
            return std::numeric_limits< size_t >::max();
        }
    }
    bool operator!=( reference other )
    {
        return true;
    }
    size_type insert( const reference element )
    {
        auto found = m_MappedData.find(element);
        if(found != m_MappedData.end())
        {
            return found->second;
        }
        m_Data.push_back( element );
        size_type n_in_vector = m_Data.size() - 1;
        m_MappedData.insert( std::make_pair(element, n_in_vector) );
        return n_in_vector;
    }
    size_type size() const
    {
        return m_Data.size();
    }
 /*   MappedStorageIterator<T> begin() const
    {
        return m_Data.begin();
    }
    MappedStorageIterator<T> end() const
    {
        return m_Data.end();
    }*/

private:
    std::map< value_type, size_type >  m_MappedData;
    std::vector< value_type >  m_Data;
};

template< typename T> class MappedStorageIterator
{

};


#endif // STRINGSTORAGE_H
