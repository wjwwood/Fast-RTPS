/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima Fast RTPS is licensed to you under the terms described in the
 * FASTRTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/**
 * @file TimedEvent.cpp
 *
 */

#include <fastrtps/rtps/resources/TimedEvent.h>
#include "TimedEventImpl.h"



namespace eprosima {
namespace fastrtps{
namespace rtps {

TimedEvent::TimedEvent(boost::asio::io_service* serv,double milliseconds)
{
	mp_impl = new TimedEventImpl(this,serv,boost::posix_time::microseconds((int64_t)(milliseconds*1000)));
}

TimedEvent::~TimedEvent()
{
	delete(mp_impl);
}

void TimedEvent::restart_timer()
{
	mp_impl->restart_timer();
}

void TimedEvent::stop_timer()
{
	mp_impl->stop_timer();
}

bool TimedEvent::update_interval(const Duration_t& inter)
{
	return mp_impl->update_interval(inter);
}

bool TimedEvent::update_interval_millisec(double time_millisec)
{
	return mp_impl->update_interval_millisec(time_millisec);
}

double TimedEvent::getIntervalMilliSec()
{
	return mp_impl->getIntervalMsec();
}

bool TimedEvent::isWaiting()
{
	return mp_impl->m_isWaiting;
}

double TimedEvent::getRemainingTimeMilliSec()
{
	return mp_impl->getRemainingTimeMilliSec();
}

void TimedEvent::stopSemaphorePost()
{
	return mp_impl->mp_stopSemaphore->post();
}


}
} /* namespace rtps */
} /* namespace eprosima */
