#pragma once


#include "Base.h"


template <typename T>
class ResourceLoader {
public:
	T* load( std::string url );
};


/*!
 * Dummy function for error handling. This is only called if a resource
 * type is not supported.
 */
template <typename T>
inline T* ResourceLoader<T>::load( std::string url )
{
	throw std::exception( "Invalid resource type." );

	return 0;
}
