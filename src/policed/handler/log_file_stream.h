#ifndef _LOG_FILE_STREAM_
#define _LOG_FILE_STREAM_

using namespace std;
#include <iostream>
#include <fstream>
#include <shared/log_handler.h>

#define LOG_SINGLE_END "\n"

class LogFileStream
{
public:
	LogFileStream(const char *filename, const string &spit = " ", const string &splt_key = " \t\n\r\x0B");
	~LogFileStream();
public:
	inline LogFileStream& operator << (const char * val)
	{
		ASSERT(val);
		if(strcmp(val, LOG_SINGLE_END) == 0)
		{
			strcpy(m_temp_str, m_file_ss.str().c_str());
			if(m_file_ss.str().length() > 0)
			{
				if(strcmp(m_splt.c_str(), m_file_ss.str().c_str() + (m_file_ss.str().length() - 1)) == 0)
					memset(m_temp_str + (strlen(m_temp_str) - 1), 0, 1);
				(*m_file) << m_temp_str << val;
				m_file_ss.str("");
			}
		}
		else
		{
			strcpy(m_temp_str, val);
			uint32 len = strlen(m_temp_str);
			if(len == 0)
				*m_file << "NULL";
			else
			{
				for (uint32 i = 0; i < len; i++)
				{
					if(strchr(m_splt_key.c_str(), val[i]) != NULL)
					{
						m_temp_str[i] = '-';
					}
				}
			}
			m_file_ss << m_temp_str << m_splt;
		}
		return *this;
	}
	inline LogFileStream& operator << (const string& val)
	{
		if(val.empty())
			(*this) << "NULL";
		else
			(*this) << val.c_str();
		return *this;
	}
	inline LogFileStream& operator << (double val)
	{
		(*this) << (int64)val;
		return *this;
	}
	template <class T>
	inline LogFileStream& operator << (const T& val)
	{
		m_file_ss << val << m_splt;
		return *this;
	}
	template <class T>
	inline void WriteVector(vector<T>& v)
	{
		if (v.empty())
		{
			m_file_ss << "NULL";
		}
		else
		{
			for (typename vector<T>::iterator it = v.begin(); it != v.end(); ++it)
			{
				m_file_ss << *it << ";";
			}
		}
		m_file_ss << m_splt;
	}
	void WriteBase64(const char *content, uint32 len);
	void WriteDealGoods(vector<deal_goods>& goods, bool base64 = false);
	void Open(const char *filename);
	void Flush();
	bool Empty();
protected:
	fstream *m_file;
	string m_splt;
	string m_splt_key;
	stringstream m_file_ss;
	char m_temp_str[10000];
	char m_base64[5000];
};

#endif