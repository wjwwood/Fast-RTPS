/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima Fast RTPS is licensed to you under the terms described in the
 * FASTRTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/**
 * @file RTPS_messages.h	
 */

#ifndef RTPS_MESSAGES_H_
#define RTPS_MESSAGES_H_
#ifndef DOXYGEN_SHOULD_SKIP_THIS_PUBLIC
#include "../common/Types.h"
#include "../common/Guid.h"

#include <iostream>
 #include <bitset>
namespace eprosima{
namespace fastrtps{
namespace rtps{

// //!@brief Enumeration of the different Submessages types
#define	PAD 0x01
#define	ACKNACK 0x06
#define	HEARTBEAT 0x07
#define	GAP 0x08
#define	INFO_TS 0x09
#define	INFO_SRC 0x0c
#define	INFO_REPLY_IP4 0x0d
#define	INFO_DST 0x0e
#define	INFO_REPLY 0x0f
#define	NACK_FRAG 0x12
#define	HEARTBEAT_FRAG 0x13
#define	DATA 0x15
#define	DATA_FRAG 0x16

//!@brief Structure Header_t, RTPS Message Header Structure.
//!@ingroup COMMON_MODULE
 struct Header_t{
	//!Protocol version
	ProtocolVersion_t version;
	//!Vendor ID
	VendorId_t vendorId;;
	//!GUID prefix
	GuidPrefix_t guidPrefix;
	Header_t():
		version(c_ProtocolVersion)
	{
				set_VendorId_eProsima(vendorId);
	}
	~Header_t(){
	}
};

/**
* @param output
* @param h
* @return
*/
inline std::ostream& operator<<(std::ostream& output,const Header_t& h){
	output << "RTPS HEADER of Version: " << (int)h.version.m_major << "." << (int)h.version.m_minor;
	output << "  || VendorId: " <<std::hex<< (int)h.vendorId[0] << "." <<(int)h.vendorId[1] << std::dec;
	output << "GuidPrefix: " << h.guidPrefix;
	return output;
}

//!@brief Structure SubmessageHeader_t, used to contain the header information of a submessage.
 struct SubmessageHeader_t{
	octet submessageId;
	uint16_t submessageLength;
	uint32_t submsgLengthLarger;
	SubmessageFlag flags;
};

using std::cout;
using std::endl;
using std::bitset;

/**
* @param output
* @param sh
* @return
*/
inline std::ostream& operator<<(std::ostream& output,const SubmessageHeader_t& sh){
	output << "Submessage Header, ID: " <<std::hex<< (int)sh.submessageId << std::dec;
	output << " length: " << (int)sh.submessageLength << " flags " << (bitset<8>) sh.flags;
	return output;
}
}
}
}


#endif
#endif /* RTPS_MESSAGES_H_ */
