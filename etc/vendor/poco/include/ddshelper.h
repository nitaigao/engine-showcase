/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef poco_ddshelper_h
# define poco_ddshelper_h

#include "corbahelper.h"

#ifndef _POCO_DDS_EXPORT
#ifdef WIN32
# ifdef BUILD_POCO_DDS_DLL
#  define _POCO_DDS_EXPORT __declspec(dllexport)
# else
#  define _POCO_DDS_EXPORT __declspec(dllimport)
# endif
#else
# define _POCO_DDS_EXPORT
#endif
#endif

#include "dds_dcps_c.hh"

#ifndef PARTICIPANT_QOS_DEFAULT
_POCO_DDS_EXPORT const DDS::DomainParticipantQos& 
	poco_dds_get_participant_qos_default();
# define PARTICIPANT_QOS_DEFAULT poco_dds_get_participant_qos_default()
#endif

#ifndef TOPIC_QOS_DEFAULT
_POCO_DDS_EXPORT const DDS::TopicQos&
	poco_dds_get_topic_qos_default();
# define TOPIC_QOS_DEFAULT poco_dds_get_topic_qos_default()
#endif

#ifndef SUBSCRIBER_QOS_DEFAULT
_POCO_DDS_EXPORT const DDS::SubscriberQos&
	poco_dds_get_subscriber_qos_default();
# define SUBSCRIBER_QOS_DEFAULT poco_dds_get_subscriber_qos_default()
#endif

#ifndef PUBLISHER_QOS_DEFAULT
_POCO_DDS_EXPORT const DDS::PublisherQos&
	poco_dds_get_publisher_qos_default();
# define PUBLISHER_QOS_DEFAULT poco_dds_get_publisher_qos_default()
#endif

#ifndef DATAREADER_QOS_DEFAULT
_POCO_DDS_EXPORT const DDS::DataReaderQos&
	poco_dds_get_datareader_qos_default();
# define DATAREADER_QOS_DEFAULT poco_dds_get_datareader_qos_default()
#endif

#ifndef DATAWRITER_QOS_DEFAULT
_POCO_DDS_EXPORT const DDS::DataWriterQos&
	poco_dds_get_datawriter_qos_default();
# define DATAWRITER_QOS_DEFAULT poco_dds_get_datawriter_qos_default()
#endif

inline DDS::DomainParticipantFactory_ptr poco_dds_get_factory(
		CORBA::ORB_ptr	orb)
{
	CORBA::Object_var ref = orb->resolve_initial_references("DataDistributionService");

	return DDS::DomainParticipantFactory::_narrow(ref.in());
}

inline DDS::DomainParticipant_ptr poco_dds_create_participant(
		DDS::DomainParticipantFactory_ptr fac,
		DDS::DomainId_t			domain,
		const DDS::DomainParticipantQos* qos,
		CORBA::Object_ptr	lsner) 
{
	DDS::DomainParticipantListener_var _lsner 
		= DDS::DomainParticipantListener::_narrow(lsner);

	const DDS::DomainParticipantQos& _qos 
		= qos? *qos:PARTICIPANT_QOS_DEFAULT;

	return fac->create_participant(domain, _qos, _lsner.in(), 0);
}

inline DDS::DomainParticipant_ptr poco_dds_create_participant(
		DDS::DomainParticipantFactory_ptr fac,
		DDS::DomainId_t			domain,
		const DDS::DomainParticipantQos* qos,
		PortableServer::Servant		lsner) 
{
	if( lsner == NULL ) {
		return poco_dds_create_participant(fac, domain, qos, 
				(CORBA::Object_ptr)NULL);
	}

	CORBA::Object_var ref = POCO_CORBAHelper::export_object(lsner);
	DDS::DomainParticipantListener_var _lsner
		= DDS::DomainParticipantListener::_narrow(ref.in()); 

	return poco_dds_create_participant(fac, domain, qos, _lsner.in());
}

inline DDS::Topic_ptr poco_dds_create_topic(
		DDS::DomainParticipant_ptr	part,
		const char*			topic_name,
		const char*			type_name,
		const DDS::TopicQos*		qos,
		CORBA::Object_ptr		lsner) 
{
	DDS::TopicListener_var _lsner
		= DDS::TopicListener::_narrow(lsner);

	const DDS::TopicQos& _qos 
		= qos? *qos:TOPIC_QOS_DEFAULT;

	return part->create_topic(topic_name, type_name, _qos, _lsner.in(), 0);
}

inline DDS::Topic_ptr poco_dds_create_topic(
		DDS::DomainParticipant_ptr	part,
		const char*			topic_name,
		const char*			type_name,
		const DDS::TopicQos* 		qos,
		PortableServer::Servant		lsner) 
{
	if( lsner == NULL ) {
		return poco_dds_create_topic(part, topic_name, type_name, qos, 
				(CORBA::Object_ptr)NULL);
	}

	CORBA::Object_var _lsner = POCO_CORBAHelper::export_object(lsner);

	return poco_dds_create_topic(part, topic_name, type_name, qos, _lsner.in());
}

inline DDS::Subscriber_ptr poco_dds_create_subscriber(
		DDS::DomainParticipant_ptr	part,
		const DDS::SubscriberQos*	qos,
		CORBA::Object_ptr		lsner) 
{
	DDS::SubscriberListener_var _lsner
		= DDS::SubscriberListener::_narrow(lsner);

	const DDS::SubscriberQos& _qos 
		= qos? *qos:SUBSCRIBER_QOS_DEFAULT;

	return part->create_subscriber(_qos, _lsner.in(), 0);
}

inline DDS::Subscriber_ptr poco_dds_create_subscriber(
		DDS::DomainParticipant_ptr	part,
		const DDS::SubscriberQos*	qos,
		PortableServer::Servant		lsner) 
{
	if( lsner == NULL ) {
		return poco_dds_create_subscriber(part, qos, 
				(CORBA::Object_ptr)NULL);
	}

	CORBA::Object_var _lsner = POCO_CORBAHelper::export_object(lsner);

	return poco_dds_create_subscriber(part, qos, _lsner.in());
}

inline DDS::Publisher_ptr poco_dds_create_publisher(
		DDS::DomainParticipant_ptr	part,
		const DDS::PublisherQos*	qos,
		CORBA::Object_ptr		lsner) 
{
	DDS::PublisherListener_var _lsner
		= DDS::PublisherListener::_narrow(lsner);

	const DDS::PublisherQos& _qos 
		= qos? *qos:PUBLISHER_QOS_DEFAULT;

	return part->create_publisher(_qos, _lsner.in(), 0);
}

inline DDS::Publisher_ptr poco_dds_create_publisher(
		DDS::DomainParticipant_ptr	part,
		const DDS::PublisherQos* 	qos,
		PortableServer::Servant		lsner) 
{
	if( lsner == NULL ) {
		return poco_dds_create_publisher(part, qos, 
				(CORBA::Object_ptr)NULL);
	}

	CORBA::Object_var _lsner = POCO_CORBAHelper::export_object(lsner);

	return poco_dds_create_publisher(part, qos, _lsner.in());
}

inline DDS::DataReader_ptr poco_dds_create_datareader(
		DDS::Subscriber_ptr		sub,
		DDS::TopicDescription_ptr	topic,
		const DDS::DataReaderQos*	qos,
		CORBA::Object_ptr		lsner) 
{
	DDS::DataReaderListener_var _lsner
		= DDS::DataReaderListener::_narrow(lsner);

	const DDS::DataReaderQos& _qos 
		= qos? *qos:DATAREADER_QOS_DEFAULT;

	return sub->create_datareader(topic, _qos, _lsner.in(), 0);
}

inline DDS::DataReader_ptr poco_dds_create_datareader(
		DDS::Subscriber_ptr		sub,
		DDS::TopicDescription_ptr	topic,
		const DDS::DataReaderQos*	qos,
		PortableServer::Servant		lsner) 
{
	if( lsner == NULL ) {
		return poco_dds_create_datareader(sub, topic, qos, 
				(CORBA::Object_ptr)NULL);
	}

	CORBA::Object_var _lsner = POCO_CORBAHelper::export_object(lsner);

	return poco_dds_create_datareader(sub, topic, qos, _lsner.in());
}

inline DDS::DataWriter_ptr poco_dds_create_datawriter(
		DDS::Publisher_ptr		pub,
		DDS::Topic_ptr			topic,
		const DDS::DataWriterQos*	qos,
		CORBA::Object_ptr		lsner) 
{
	DDS::DataWriterListener_var _lsner
		= DDS::DataWriterListener::_narrow(lsner);

	const DDS::DataWriterQos& _qos 
		= qos? *qos:DATAWRITER_QOS_DEFAULT;

	return pub->create_datawriter(topic, _qos, _lsner.in(), 0);
}

inline DDS::DataWriter_ptr poco_dds_create_datawriter(
		DDS::Publisher_ptr		pub,
		DDS::Topic_ptr			topic,
		const DDS::DataWriterQos*	qos,
		PortableServer::Servant		lsner) 
{
	if( lsner == NULL ) {
		return poco_dds_create_datawriter(pub, topic, qos, 
				(CORBA::Object_ptr)NULL);
	}

	CORBA::Object_var _lsner = POCO_CORBAHelper::export_object(lsner);

	return poco_dds_create_datawriter(pub, topic, qos, _lsner.in());
}

#define POCO_DDS_DELETE_PARTICIPANT(part) \
		delete_participant((part->delete_contained_entities(), part))
#define POCO_DDS_DELETE_TOPIC(topic) \
		delete_topic(topic)
#define POCO_DDS_DELETE_SUBSCRIBER(sub) \
		delete_subscriber((sub->delete_contained_entities(), sub))
#define POCO_DDS_DELETE_PUBLISHER(pub) \
		delete_publisher((pub->delete_contained_entities(), pub))
#define POCO_DDS_DELETE_DATAREADER(rder) delete_datareader(rder)
#define POCO_DDS_DELETE_DATAWRITER(wter) delete_datawriter(wter)

#endif
