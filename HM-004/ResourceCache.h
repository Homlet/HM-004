#pragma once


#include "Base.h"
#include "ResourceLoader.h"


template <typename T>
class ResourceCache {
private:
	ResourceLoader<T>* loader;
	std::map<std::string, T*> cache;

public:
	ResourceCache( void );

	T* getResource( std::string url );
};


/*!
 * Creates a new resource loader for type T.
 */
template <typename T>
inline ResourceCache<T>::ResourceCache( void ) :
	loader( new ResourceLoader<T>() )
{
}


/*! 
 * Returns a pointer to the cached resource. If the resource
 * has not been cached, it is loaded and stored first.
 *
 * @param url URL of the resource to load.
 */
template <typename T>
inline T* ResourceCache<T>::getResource( std::string url )
{
	// Iterator for the resource object in cache.
	auto itr = cache.find( url );

	if ( itr == cache.end() )
	{
		// If resource is not in the cache already, load and insert it.
		std::cout << "Loading resource: \"" + url "\"" << std::endl;

		T* t_ptr = loader->load( url );
		cache[url] = t_ptr;

		return t_ptr;
	} else
	{
		return itr->second();
	}
}
