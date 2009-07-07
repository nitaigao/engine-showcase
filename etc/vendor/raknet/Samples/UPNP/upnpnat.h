// KevinJ: Sourced from http://www.codeproject.com/KB/IP/port-forwarding.aspx

#ifndef UPNPNAT_H
#define UPNPNAT_H

#include <string>
#include <vector>
#include "RakNetTime.h"

// KevinJ:
#include "WindowsIncludes.h"

#pragma   warning(disable:   4251) 

class __declspec (dllexport) UPNPNAT
{
public:
 
	bool init(RakNetTime time_out=250); //init
	bool discovery();//find router

	/****
	 **** _description: port mapping name
	 **** _destination_ip: internal ip address
	 **** _port_ex:external: external listen port
	 **** _destination_port: internal port
	 **** _protocal: TCP or UDP
	 ****
	 **** KevinJ: I think _destination_ip is OUR internal address, not that of the remote destination
	 **** KevinJ: I think _port_ex is whatever port the remote system will be sending to
	 **** KevinJ: I think _destination_port is whatever port we bound RakNet to
	 ***/
	bool add_port_mapping(const char * _description, const char * _destination_ip, unsigned short int _port_ex, unsigned short int _destination_port, char * _protocal);//add port mapping

	const char * get_last_error(){ return last_error.c_str();}//get last error
private:
	bool get_description();			//
	bool parser_description();		//
	bool tcp_connect(const char * _addr,unsigned short int _port);
	bool parse_mapping_info();
	int udp_socket_fd;
	int tcp_socket_fd;
	typedef enum 
	{
		NAT_INIT=0,
		NAT_FOUND,
		NAT_TCP_CONNECTED,
		NAT_GETDESCRIPTION,
		NAT_GETCONTROL,
		NAT_ADD,
		NAT_DEL,
		NAT_GET,
		NAT_ERROR
	} NAT_STAT;
	NAT_STAT status;
    RakNetTime time_out;
	std::string service_type;
	std::string describe_url;
	std::string control_url;
	std::string base_url;
	std::string service_describe_url;
	std::string description_info;
	std::string last_error;
	std::string mapping_info;
};

#endif

