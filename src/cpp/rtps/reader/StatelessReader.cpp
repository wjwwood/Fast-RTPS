/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima Fast RTPS is licensed to you under the terms described in the
 * FASTRTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/*
 * @file StatelessReader.cpp
 *             	
 */

#include <fastrtps/rtps/reader/StatelessReader.h>
#include <fastrtps/rtps/history/ReaderHistory.h>
#include <fastrtps/rtps/reader/ReaderListener.h>
#include <fastrtps/utils/RTPSLog.h>
#include <fastrtps/rtps/common/CacheChange.h>


#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/lock_guard.hpp>

namespace eprosima {
namespace fastrtps{
namespace rtps {

static const char* const CLASS_NAME = "StatelessReader";

StatelessReader::~StatelessReader()
{
	const char* const METHOD_NAME = "~StatelessReader";
	logInfo(RTPS_READER,"Removing reader "<<this->getGuid());
}

StatelessReader::StatelessReader(RTPSParticipantImpl* pimpl,GUID_t& guid,
		ReaderAttributes& att,ReaderHistory* hist,ReaderListener* listen):
						RTPSReader(pimpl,guid,att,hist, listen)
{

}



bool StatelessReader::matched_writer_add(RemoteWriterAttributes& wdata)
{
	const char* const METHOD_NAME = "matched_writer_add";
	boost::lock_guard<boost::recursive_mutex> guard(*mp_mutex);
	for(auto it = m_matched_writers.begin();it!=m_matched_writers.end();++it)
	{
		if((*it).guid == wdata.guid)
			return false;
	}
	logInfo(RTPS_READER,"Writer " << wdata.guid << " added to "<<m_guid.entityId);
	m_matched_writers.push_back(wdata);
	m_acceptMessagesFromUnkownWriters = false;
	return true;
}
bool StatelessReader::matched_writer_remove(RemoteWriterAttributes& wdata)
{
	const char* const METHOD_NAME = "matched_writer_remove";
	boost::lock_guard<boost::recursive_mutex> guard(*mp_mutex);
	for(auto it = m_matched_writers.begin();it!=m_matched_writers.end();++it)
	{
		if((*it).guid == wdata.guid)
		{
			logInfo(RTPS_READER,"Writer " <<wdata.guid<< " removed from "<<m_guid.entityId);
			m_matched_writers.erase(it);
			return true;
		}
	}
	return false;
}

bool StatelessReader::matched_writer_is_matched(RemoteWriterAttributes& wdata)
{
	boost::lock_guard<boost::recursive_mutex> guard(*mp_mutex);
	for(auto it = m_matched_writers.begin();it!=m_matched_writers.end();++it)
	{
		if((*it).guid == wdata.guid)
		{
			return true;
		}
	}
	return false;
}

bool StatelessReader::change_received(CacheChange_t* change,WriterProxy* prox)
{
	boost::unique_lock<boost::recursive_mutex> lock(*mp_mutex);

	if(mp_history->received_change(change))
	{
		if(getListener()!=nullptr)
		{
            lock.unlock();
			getListener()->onNewCacheChangeAdded((RTPSReader*)this,change);
		}
        lock.lock();
		mp_history->postSemaphore();
		return true;
	}
	return false;
}

bool StatelessReader::nextUntakenCache(CacheChange_t** change,WriterProxy** wpout)
{
	//const char* const METHOD_NAME = "nextUntakenCache";
	boost::lock_guard<boost::recursive_mutex> guard(*mp_mutex);
	return mp_history->get_min_change(change);
}


bool StatelessReader::nextUnreadCache(CacheChange_t** change,WriterProxy** wpout)
{
	const char* const METHOD_NAME = "nextUnreadCache";
	boost::lock_guard<boost::recursive_mutex> guard(*mp_mutex);
	//m_reader_cache.sortCacheChangesBySeqNum();
	bool found = false;
	std::vector<CacheChange_t*>::iterator it;
	//TODO PROTEGER ACCESO A HISTORIA AQUI??? YO CREO QUE NO, YA ESTA EL READER PROTEGIDO
	for(it = mp_history->changesBegin();
			it!=mp_history->changesEnd();++it)
	{
		if(!(*it)->isRead)
		{
			found = true;
			break;
		}
	}
	if(found)
	{
		*change = *it;
		return true;
	}
	logInfo(RTPS_READER,"No Unread elements left");
	return false;
}


bool StatelessReader::change_removed_by_history(CacheChange_t*ch,WriterProxy*prox)
{
	return true;
}

bool StatelessReader::acceptMsgFrom(GUID_t& writerId,WriterProxy** wp)
{
    boost::lock_guard<boost::recursive_mutex> guard(*mp_mutex);
	if(this->m_acceptMessagesFromUnkownWriters)
	{
		return true;
	}
	else
	{
		if(writerId.entityId == this->m_trustedWriterEntityId)
			return true;
		for(auto it = m_matched_writers.begin();it!=m_matched_writers.end();++it)
		{
			if((*it).guid == writerId)
			{
				return true;
			}
		}
	}
	return false;
}

}
} /* namespace rtps */
} /* namespace eprosima */


