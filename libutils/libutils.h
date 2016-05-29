#pragma once

#define for_each(_type, _object, _iterator) for (_type::iterator _iterator=_object.begin(); _iterator!=_object.end();_iterator++)
#define for_each_const(_type, _object, _iterator) for (_type::const_iterator _iterator=_object.begin(); _iterator!=_object.end();_iterator++)

#define LIBUTILS_API 

