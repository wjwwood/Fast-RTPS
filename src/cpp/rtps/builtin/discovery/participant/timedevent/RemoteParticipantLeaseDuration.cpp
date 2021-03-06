/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima Fast RTPS is licensed to you under the terms described in the
 * FASTRTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/**
 * @file RTPSParticipantLeaseDuration.cpp
 *
 */

#include <fastrtps/rtps/builtin/discovery/participant/timedevent/RemoteParticipantLeaseDuration.h>

#include <fastrtps/rtps/builtin/discovery/participant/PDPSimple.h>
#include <fastrtps/rtps/builtin/data/ParticipantProxyData.h>

#include "../../../../participant/RTPSParticipantImpl.h"

#include <fastrtps/utils/RTPSLog.h>



namespace eprosima {
namespace fastrtps{
namespace rtps {

static const char* const CLASS_NAME = "RemoteParticipantLeaseDuration";

RemoteParticipantLeaseDuration::RemoteParticipantLeaseDuration(PDPSimple* p_SPDP,
		ParticipantProxyData* pdata,
		double interval):
				TimedEvent(p_SPDP->getRTPSParticipant()->getIOService(),interval),
				mp_PDP(p_SPDP),
				mp_participantProxyData(pdata)
{

}

RemoteParticipantLeaseDuration::~RemoteParticipantLeaseDuration()
{
	stop_timer();
}

void RemoteParticipantLeaseDuration::event(EventCode code, const char* msg)
{
	const char* const METHOD_NAME = "event";
	if(code == EVENT_SUCCESS)
	{
		logInfo(RTPS_LIVELINESS,"Checking RTPSParticipant: "
				<< mp_participantProxyData->m_participantName << " with GUID: "
				<< mp_participantProxyData->m_guid,C_MAGENTA);
		if(mp_participantProxyData->isAlive)
			mp_participantProxyData->isAlive = false;
		else
		{
			logInfo(RTPS_LIVELINESS,"RTPSParticipant no longer ALIVE, trying to remove: "
					<< mp_participantProxyData->m_guid,C_MAGENTA);
			mp_PDP->removeRemoteParticipant(mp_participantProxyData->m_guid);
			return;
		}
		this->restart_timer();
	}
	else if(code == EVENT_ABORT)
	{
		logInfo(RTPS_LIVELINESS," Stopped for "<<mp_participantProxyData->m_participantName
				<< " with ID: "<< mp_participantProxyData->m_guid.guidPrefix,C_MAGENTA);
		this->stopSemaphorePost();
	}
	else
	{
		logInfo(RTPS_LIVELINESS,"boost message: " <<msg,C_MAGENTA);
	}
}

}
} /* namespace rtps */
} /* namespace eprosima */
