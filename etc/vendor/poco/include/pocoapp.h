#ifndef _poco_app_context_h_
# define _poco_app_context_h_

//
// This header file is for application to build embedded or stand alone containers.
//

#include <stdlib.h>
#include "pocotypes.h"

class _POCO_CAPSULE_EXPORT POCO_AppEnv;

class _POCO_CAPSULE_EXPORT POCO_AppContext {
  protected:
	virtual ~POCO_AppContext() {}
  public:
	//
	// create a context from a given source
	//
	static POCO_AppContext* create(
			const char* desc_src,	// xml or dom file name, or string buf
			const char* type,	// desc src type: "file", "string", etc. 
			POCO_AppEnv* env = NULL);

	//
	// get and change the default application env of this context
	//
	static POCO_AppEnv*	getDefaultAppEnv();
	static POCO_AppEnv*	setDefaultAppEnv(POCO_AppEnv* env);
	static POCO_AppEnv*	initDefaultAppEnv(int argc, char** argv);

	//
	// get a bean of the id
	//
	virtual void* getBean(
			const char* id, 	// bean's id
			POCO_AppEnv* env = NULL) = 0;

	//
	// get a bean of the id and cast it to the specified class type
	//
	virtual void* getBean(
			const char* id, 	// bean's id
			const char* class_name, // class name
			POCO_AppEnv* env = NULL) = 0;

	//
	// get the class pointer name of the bean
	//
	virtual const char* getBeanPtrTypeId(
			const char* id, 	// bean's id
			POCO_AppEnv* env = NULL) = 0;

	//
	// initial all non-lazy singleton beans (and others they depend on)
	//
	virtual int initSingletons(POCO_AppEnv* env = NULL) = 0;

	//
	// release all singleton beans
	//
	virtual int terminate(POCO_AppEnv* env = NULL) = 0;

	//
	// release memory resources of tihs context
	//
	virtual void destroy() = 0;
};

class POCO_AppEnvImpl;

class _POCO_CAPSULE_EXPORT POCO_AppEnv {
	POCO_AppEnvImpl*	_impl;

  protected:
	virtual ~POCO_AppEnv();

  public:
	POCO_AppEnv();
	POCO_AppEnv(int argc, const char** argv);

	virtual void reset();
	virtual const char* get_message();
	virtual void warning(const char*);
	virtual int has_error();
	virtual void fatal_error(const char*);

	virtual void setValue(const char* name, const char* value);
	virtual const char* getValue(const char* name);

	virtual void setArray(const char* name, int argc, const char** argv);
	virtual const char** getArray(const char* name);
};

#endif
