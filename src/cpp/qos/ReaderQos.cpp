/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima Fast RTPS is licensed to you under the terms described in the
 * FASTRTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/**
 * @file ReaderQos.cpp
 *
 */

#include <fastrtps/qos/ReaderQos.h>
#include <fastrtps/utils/RTPSLog.h>

namespace eprosima {
namespace fastrtps {

static const char* const CLASS_NAME = "ReaderQos";

void ReaderQos::setQos( ReaderQos& qos, bool first_time)
{
	if(first_time)
	{
		m_durability = qos.m_durability;
		m_durability.hasChanged = true;
	}
	if(first_time || m_deadline.period != qos.m_deadline.period)
	{
		m_deadline = qos.m_deadline;
		m_deadline.hasChanged = true;
	}
	if(m_latencyBudget.duration != qos.m_latencyBudget.duration)
	{
		m_latencyBudget = qos.m_latencyBudget;
		m_latencyBudget.hasChanged = true;
	}
	if(m_liveliness.lease_duration != qos.m_liveliness.lease_duration)
	{
		m_liveliness.lease_duration = qos.m_liveliness.lease_duration;
		m_liveliness.hasChanged = true;
	}
	if(first_time)
	{
		m_liveliness = qos.m_liveliness;
		m_liveliness.hasChanged = true;
	}
	if(first_time)
	{
		m_reliability = qos.m_reliability;
		m_reliability.hasChanged = true;
	}
	if(first_time)
	{
		m_ownership = qos.m_ownership;
		m_ownership.hasChanged = true;
	}
	if(m_destinationOrder.kind != qos.m_destinationOrder.kind)
	{
		m_destinationOrder = qos.m_destinationOrder;
		m_destinationOrder.hasChanged = true;
	}
	if (m_userData.getDataVec() != qos.m_userData.getDataVec())
	{
		m_userData = qos.m_userData;
		m_userData.hasChanged = true;
	}
	if(m_timeBasedFilter.minimum_separation != qos.m_timeBasedFilter.minimum_separation )
	{
		m_timeBasedFilter = qos.m_timeBasedFilter;
		m_timeBasedFilter.hasChanged = true;
	}
	if(first_time || m_presentation.access_scope != qos.m_presentation.access_scope ||
			m_presentation.coherent_access != qos.m_presentation.coherent_access ||
			m_presentation.ordered_access != qos.m_presentation.ordered_access)
	{
		m_presentation = qos.m_presentation;
		m_presentation.hasChanged = true;
	}
	if(qos.m_partition.getNames().size()>0)
	{
		m_partition = qos.m_partition;
		m_partition.hasChanged = true;
	}
	if (m_topicData.getValue() != qos.m_topicData.getValue())
	{
		m_topicData = qos.m_topicData;
		m_topicData.hasChanged = true;
	}
	if(m_groupData.getValue() != qos.m_groupData.getValue() )
	{
		m_groupData = qos.m_groupData;
		m_groupData.hasChanged = true;
	}
	if(first_time || m_durabilityService.history_kind != qos.m_durabilityService.history_kind ||
			m_durabilityService.history_depth != qos.m_durabilityService.history_depth ||
			m_durabilityService.max_instances != qos.m_durabilityService.max_instances ||
			m_durabilityService.max_samples != qos.m_durabilityService.max_samples||
			m_durabilityService.max_samples_per_instance != qos.m_durabilityService.max_samples_per_instance ||
			m_durabilityService.service_cleanup_delay != qos.m_durabilityService.service_cleanup_delay
	)
	{
		m_durabilityService = qos.m_durabilityService;
		m_durabilityService.hasChanged = true;
	}
	if(m_lifespan.duration != qos.m_lifespan.duration )
	{
		m_lifespan = qos.m_lifespan;
		m_lifespan.hasChanged = true;
	}
}


bool ReaderQos::checkQos()
{
	const char* const METHOD_NAME = "checkQos";
	if(m_durability.kind == TRANSIENT_DURABILITY_QOS)
	{
		logError(RTPS_QOS_CHECK,"TRANSIENT Durability not supported");
		return false;
	}
	if(m_durability.kind == PERSISTENT_DURABILITY_QOS)
	{
		logError(RTPS_QOS_CHECK,"PERSISTENT Durability not supported");
		return false;
	}
	if(m_destinationOrder.kind == BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS)
	{
		logError(RTPS_QOS_CHECK,"BY SOURCE TIMESTAMP DestinationOrder not supported");
		return false;
	}
	if(m_reliability.kind == BEST_EFFORT_RELIABILITY_QOS && m_ownership.kind == EXCLUSIVE_OWNERSHIP_QOS)
	{
		logError(RTPS_QOS_CHECK,"BEST_EFFORT incompatible with EXCLUSIVE ownership");
		return false;
	}
	return true;
}

bool ReaderQos::canQosBeUpdated(ReaderQos& qos)
{
	const char* const METHOD_NAME = "canQosBeUpdated";
	bool updatable = true;
	if(	m_durability.kind != qos.m_durability.kind)
	{
		updatable = false;
		logWarning(RTPS_QOS_CHECK,"Durability kind cannot be changed after the creation of a subscriber.");
	}

	if(m_liveliness.kind !=  qos.m_liveliness.kind)
	{
		updatable = false;
		logWarning(RTPS_QOS_CHECK,"Liveliness Kind cannot be changed after the creation of a subscriber.");
	}

	if(m_reliability.kind != qos.m_reliability.kind)
	{
		updatable = false;
		logWarning(RTPS_QOS_CHECK,"Reliability Kind cannot be changed after the creation of a subscriber.");
	}
	if(m_ownership.kind != qos.m_ownership.kind)
	{
		updatable = false;
		logWarning(RTPS_QOS_CHECK,"Ownership Kind cannot be changed after the creation of a subscriber.");
	}
	if(m_destinationOrder.kind != qos.m_destinationOrder.kind)
	{
		updatable = false;
		logWarning(RTPS_QOS_CHECK,"Destination order Kind cannot be changed after the creation of a subscriber.");
	}
	return updatable;
}

} /* namespace pubsub */
} /* namespace eprosima */
