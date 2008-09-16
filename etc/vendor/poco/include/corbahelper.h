#ifndef _poco_corba_helper_h_
# define _poco_corba_helper_h_

#if defined(BUILD_WITH_TAO) || defined(USE_TAO)
#include <tao/corba.h>
#include <tao/PortableServer/PortableServer.h>
#else
#include <corba.h>
#endif

#include "poco.h"

#if defined(WIN32)
#  if defined(BUILD_ORB_DLL)
#    define _POCO_ORB_EXPORT __declspec(dllexport)
#  else
#    define _POCO_ORB_EXPORT __declspec(dllimport)
#  endif
#else
#  define _POCO_ORB_EXPORT
#endif

class _POCO_ORB_EXPORT POCO_CORBAHelper {
  public:
	static CORBA::ORB_ptr	ORB_init(int argc, const char** argv);

	// to support Event/Notification
	static CORBA::Object_ptr subscribe(
				CORBA::ORB_ptr		orb,
				CORBA::Object_ptr	service,
				CORBA::Object_ptr	channel,
				CORBA::Object_ptr	admin,
				const char*		event_type,
				const char*		topic,
				int			prop_count,
				const POCO_NameValue	props[],
				CORBA::Object_ptr	remote_consumer);

	static CORBA::Object_ptr subscribe(
				CORBA::ORB_ptr		orb,
				CORBA::Object_ptr	service,
				CORBA::Object_ptr	channel,
				CORBA::Object_ptr	admin,
				const char*		event_type,
				const char*		topic,
				int			prop_count,
				const POCO_NameValue	props[],
				PortableServer::Servant consumer_servant);

	static CORBA::Object_ptr subscribe(
				CORBA::ORB_ptr		orb,
				CORBA::Object_ptr	service,
				CORBA::Object_ptr	channel,
				CORBA::Object_ptr	admin,
				const char*		event_type,
				const char*		topic,
				int			prop_count,
				const POCO_NameValue	props[],
				void*			native_consumer,
				const char*		class_name);

	static CORBA::Object_ptr subscribe(
				CORBA::ORB_ptr		orb,
				CORBA::Object_ptr	service,
				CORBA::Object_ptr	channel,
				CORBA::Object_ptr	admin,
				const char*		event_type,
				const char*		topic,
				int			prop_count,
				const POCO_NameValue	props[]);

	static void unsubscribe(CORBA::Object_ptr	proxy);

	static CORBA::Object_ptr publish(
				CORBA::ORB_ptr		orb,
				CORBA::Object_ptr	service,
				CORBA::Object_ptr	channel,
				CORBA::Object_ptr	admin,
				const char*		event_type,
				const char*		topic,
				int			prop_count,
				const POCO_NameValue	props[],
				CORBA::Object_ptr	remote_supplier);

	static CORBA::Object_ptr publish(
				CORBA::ORB_ptr		orb,
				CORBA::Object_ptr	service,
				CORBA::Object_ptr	channel,
				CORBA::Object_ptr	admin,
				const char*		event_type,
				const char*		topic,
				int			prop_count,
				const POCO_NameValue	props[],
				PortableServer::Servant supplier_servant);

	static CORBA::Object_ptr publish(
				CORBA::ORB_ptr		orb,
				CORBA::Object_ptr	service,
				CORBA::Object_ptr	channel,
				CORBA::Object_ptr	admin,
				const char*		event_type,
				const char*		topic,
				int			prop_count,
				const POCO_NameValue	props[],
				void*			native_supplier,
				const char*		class_name);

	static CORBA::Object_ptr publish(
				CORBA::ORB_ptr		orb,
				CORBA::Object_ptr	service,
				CORBA::Object_ptr	channel,
				CORBA::Object_ptr	admin,
				const char*		event_type,
				const char*		topic,
				int			prop_count,
				const POCO_NameValue	props[]);

	static void unpublish(	CORBA::Object_ptr	proxy);

	// to support URL
	static const char* set_url(
				CORBA::ORB_ptr		orb,
				const char*		uri,
				CORBA::Object_ptr	ref);

	static const char* get_url(
				CORBA::ORB_ptr		orb,
				const char*		uri);

	static CORBA::Object_ptr get_ref(
				CORBA::ORB_ptr		orb,
				const char*		uri);

	static void		run(
				CORBA::ORB_ptr		orb);

	// get_facet()
	static CORBA::Object_ptr get_facet(
				CORBA::ORB_ptr		orb,
				CORBA::Object_ptr	composit,
				const char*		repid);

	static PortableServer::POA_ptr create_POA(
				PortableServer::POA_ptr parent_poa,
				const char*		name,
				PortableServer::POAManager_ptr mgr,
				int			prop_count,
				POCO_NameValue		props[]);

	static CORBA::Object_ptr export_object(
				PortableServer::POA_ptr poa,
				const char*		oid,
				const char*		repid,
				PortableServer::Servant servant,
				const char*		uri);

	static CORBA::Object_ptr export_object(
				PortableServer::POA_ptr poa,
				const char*		oid,
				int			count,
				PortableServer::Servant servants[],
				const char*		uri);

	static void		bind_to_ins(
				CORBA::Object_ptr	ctxt,
				const char*		name,
				CORBA::Object_ptr	ref);

	static CORBA::Object_ptr export_object(PortableServer::Servant servant);
};

#endif
