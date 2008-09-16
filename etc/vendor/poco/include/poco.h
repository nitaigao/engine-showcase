#ifndef _pocomatic_h_
# define _pocomatic_h_

//
// this header file is used by generated proxy file
//

#include "pocotypes.h"

# ifndef POCO_NAME_VALUE
#  define POCO_NAME_VALUE

struct POCO_NameValue {
	const char*	name;
	const char*	value;
};

# endif

class POCO_VAR {
	char*	_ptr;
  public:
	POCO_VAR(char* p) : _ptr(p) {}
	~POCO_VAR() { delete _ptr; }
};

typedef void* (*POCO_proxy_t)(void* this_ptr, void* params[]);

// need to import this method
class _POCO_CAPSULE_EXPORT pocomatic_register_proxy {
  public:
	pocomatic_register_proxy(
	int, const char*, const char*, const char*,
	POCO_proxy_t, const char*, const char*);
};

class POCO_AppEnv;
typedef void* (*POCO_factory_wrapper_t)(const char* id, POCO_proxy_t, void* this_ptr, void* params, POCO_AppEnv*);
_POCO_CAPSULE_EXPORT int pocomatic_register_factory_wrapper(const char* id, POCO_factory_wrapper_t);

#if defined(VXWORKS)
# define POCO_ENTRY(x) void poco_entry_##x() {}
#else
# define POCO_ENTRY(x)
#endif

#endif
