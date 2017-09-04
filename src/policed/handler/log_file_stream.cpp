#include <comm/common_stl.h>
#include <protocol/external.h>
#include <object/SharedDef.h>
#include <comm/b64.h>
#include "policed_app.h"
#include "log_file_stream.h"


LogFileStream::LogFileStream(const char *filename, const string &spit, const string &splt_key):m_file(NULL)
{
	m_file = new fstream;
	m_file->open(filename, ios::app | ios::out);
	m_splt = spit;
	m_splt_key = splt_key;
}

LogFileStream::~LogFileStream()
{
	safe_delete(m_file);
}

void LogFileStream::Open(const char *filename)
{
	m_file->close();
	m_file->open(filename, ios::app | ios::out);
}

void LogFileStream::Flush()
{
	m_file->flush();
}

bool LogFileStream::Empty()
{
	//m_file->seekg( 0L, ios::beg );
	//fstream::pos_type beg_pos = m_file->tellg();
	m_file->seekg( 0L, ios::end );
	fstream::pos_type end_pos = m_file->tellg();
#if 0
	return 0 == end_pos.seekpos();
#else
	return 0 == end_pos;	
#endif
}

void LogFileStream::WriteBase64(const char *content, uint32 len)
{
	if(base64_encode(content, len, m_base64, 5000) == 0)
		m_file_ss << m_base64;
	else
		m_file_ss << content;
}

void LogFileStream::WriteDealGoods(vector<deal_goods>& goods, bool base64)
{
	if(base64)
	{
		if (!goods.empty())
		{
			stringstream ss;
			for (vector<deal_goods>::iterator it = goods.begin(); it != goods.end(); ++it)
			{
				ss << "id:" << it->guid << ",goodid:" << it->entryid << ",goodtype:" << it->goodtype << ",num:" << it->num << ";";
			}
			WriteBase64(ss.str().c_str(), ss.str().length());
		}
	}
	else
	{
		if (goods.empty())
		{
			m_file_ss << "NULL";
		}
		else
		{
			for (vector<deal_goods>::iterator it = goods.begin(); it != goods.end(); ++it)
			{
				m_file_ss << "id:" << it->guid << ",goodid:" << it->entryid << ",goodtype:" << it->goodtype << ",num:" << it->num << ";";
			}
		}
	}
	m_file_ss << m_splt;
}

