#ifndef _WORLD_PACKET_H_
#define _WORLD_PACKET_H_

#include <svrcore-internal.h>
#include <core_obj/ByteArray.h>

//内部包路由枚举
enum EInternalRoutingType
{
	INTERNAL_ROUTING_TYPE_ALL_SERVER	= 0,		//路由到所有的服务器
	INTERNAL_ROUTING_TYPE_ALL_SCENED	= 1,		//路由到所有的场景服
	INTERNAL_ROUTING_TYPE_TO_FD			= 2,		//路由到指定的连接
	MAX_INTERNAL_ROUTING_TYPE,
};

class WorldPacketException
{
    public:
		WorldPacketException(bool _add, uint32 _pos, uint32 _esize, uint32 _size)
            : add(_add), pos(_pos), esize(_esize), size(_size)
        {
            PrintPosError();
        }

        void PrintPosError() const
        {
            /*fprintf(stderr,"Attempted to %s in ByteBuffer (pos: " SIZEFMTD " size: "SIZEFMTD") value with size: " SIZEFMTD,
                (add ? "put" : "get"), pos, size, esize);*/
        }
    private:
        bool add;
        uint32 pos;
		uint32 esize;
		uint32 size;
};


class WorldPacket
{
	enum PacketControls
	{
		PACKET_FREE_NONE = 0,
		PACKET_TO_POOL,
		PACKET_MUST_FREE
	};
public:
	WorldPacket():m_must_free(PACKET_MUST_FREE)
	{
		packet_init(&m_pkt);
	}
	WorldPacket(packet& pkt):m_must_free(PACKET_FREE_NONE)
	{
		m_pkt = &pkt;
	}
	
	// constructor
	WorldPacket(uint16 optcode):m_must_free(PACKET_TO_POOL)
	{
		m_pkt = external_protocol_new_packet(optcode);
	}
	~WorldPacket()
	{
		if(m_must_free == PACKET_MUST_FREE)
			packet_free(m_pkt);
		else if(m_must_free == PACKET_TO_POOL)
			external_protocol_free_packet(m_pkt);
	}

	packet& GetPkt(){return *m_pkt;}
	packet* GetPktPtr(){return m_pkt;}

	// copy constructor
	//WorldPacket(const WorldPacket &buf): _rpos(buf._rpos), _wpos(buf._wpos), _storage(buf._storage) { }

	template <typename T> void append(T value)
	{
		//            EndianConvert(value);
		packet_write(m_pkt,(char*)&value,sizeof(value));
	}

	WorldPacket &operator<<(uint8 value)
	{
		append<uint8>(value);
		return *this;
	}

	WorldPacket &operator<<(uint16 value)
	{
		append<uint16>(value);
		return *this;
	}

	WorldPacket &operator<<(uint32 value)
	{
		append<uint32>(value);
		return *this;
	}

	WorldPacket &operator<<(uint64 value)
	{
		append<uint64>(value);
		return *this;
	}

	// signed as in 2e complement
	WorldPacket &operator<<(int8 value)
	{
		append<int8>(value);
		return *this;
	}

	WorldPacket &operator<<(int16 value)
	{
		append<int16>(value);
		return *this;
	}

	WorldPacket &operator<<(int32 value)
	{
		append<int32>(value);
		return *this;
	}

	WorldPacket &operator<<(int64 value)
	{
		append<int64>(value);
		return *this;
	}

	// floating points
	WorldPacket &operator<<(float value)
	{
		append<float>(value);
		return *this;
	}

	WorldPacket &operator<<(double value)
	{
		append<double>(value);
		return *this;
	}

	WorldPacket &operator<<(const std::string &value)
	{
		packet_write_str(m_pkt,value.c_str());
		return *this;
	}

	WorldPacket &operator<<(const char *str)
	{
		packet_write_str(m_pkt,str);
		return *this;
	}

	WorldPacket &operator<<(char *str)
	{
		packet_write_str(m_pkt,str);
		return *this;
	}

	WorldPacket &operator<<(core_obj::ByteArray &buff)
	{
		packet_write(m_pkt,(char*)buff.cur_data(),buff.bytesAvailable());
		return *this;
	}

	WorldPacket &operator>>(bool &value)
	{
		value = read<char>() > 0 ? true : false;
		return *this;
	}

	WorldPacket &operator>>(uint8 &value)
	{
		value = read<uint8>();
		return *this;
	}

	WorldPacket &operator>>(uint16 &value)
	{
		value = read<uint16>();
		return *this;
	}

	WorldPacket &operator>>(uint32 &value)
	{
		value = read<uint32>();
		return *this;
	}

	WorldPacket &operator>>(uint64 &value)
	{
		value = read<uint64>();
		return *this;
	}

	//signed as in 2e complement
	WorldPacket &operator>>(int8 &value)
	{
		value = read<int8>();
		return *this;
	}

	WorldPacket &operator>>(int16 &value)
	{
		value = read<int16>();
		return *this;
	}

	WorldPacket &operator>>(int32 &value)
	{
		value = read<int32>();
		return *this;
	}

	WorldPacket &operator>>(int64 &value)
	{
		value = read<int64>();
		return *this;
	}

	WorldPacket &operator>>(float &value)
	{
		value = read<float>();
		return *this;
	}

	WorldPacket &operator>>(double &value)
	{
		value = read<double>();
		return *this;
	}

	WorldPacket &operator>>(std::string& value)
	{
		value.clear();
		char *str = NULL;
		if(packet_read_str(m_pkt,&str))
			throw WorldPacketException(false, m_pkt->rpos, m_pkt->rpos, size());		
		value = str;
		return *this;
	}

	template<typename T>
	void read_skip() { read_skip(sizeof(T)); }

	void read_skip(size_t skip)
	{
		if(packet_read_skip(m_pkt,skip))
			throw WorldPacketException(false, m_pkt->rpos, skip, size());		
	}

	template <typename T> T read()
	{
		T r;
		read((uint8*)&r,sizeof(r));
		
		return r;
	}

	void read(uint8 *dest, size_t len)
	{
		if(packet_read(m_pkt,(char*)dest,len))
			throw WorldPacketException(false, m_pkt->rpos, len, size());		
	}

	size_t size() const { return  m_pkt->wpos; }
	bool empty() const { return  m_pkt->wpos == 0; }


	void append(const std::string& str)
	{
		packet_write_str(m_pkt,str.c_str());
	}

	void append(const char *src, size_t cnt)
	{
		return append((const uint8 *)src, cnt);
	}

	template<class T> void append(const T *src, size_t cnt)
	{
		return append((const uint8 *)src, cnt * sizeof(T));
	}

	void append(const uint8 *src, size_t cnt)
	{
		if (!cnt)
			return;

		ASSERT(size() < 10000000);

		packet_write(m_pkt,(char*)src,cnt);
	}

	void put(size_t pos, const uint8 *src, size_t cnt)
	{
		if(pos + cnt > size())
			throw WorldPacketException(true, pos, cnt, size());
		memcpy(&m_pkt->content[pos], src, cnt);
	}
#if 0
	void print_storage() const
	{
		if(!sLog.IsOutDebug())                          // optimize disabled debug output
			return;

		sLog.outDebug("STORAGE_SIZE: %lu", (unsigned long)size() );
		for (uint32 i = 0; i < size(); ++i)
			sLog.outDebugInLine("%u - ", read<uint8>(i) );
		sLog.outDebug(" ");
	}

	void textlike() const
	{
		if(!sLog.IsOutDebug())                          // optimize disabled debug output
			return;

		sLog.outDebug("STORAGE_SIZE: %lu", (unsigned long)size() );
		for (uint32 i = 0; i < size(); ++i)
			sLog.outDebugInLine("%c", read<uint8>(i) );
		sLog.outDebug(" ");
	}

	void hexlike() const
	{
		if(!sLog.IsOutDebug())                          // optimize disabled debug output
			return;

		uint32 j = 1, k = 1;
		sLog.outDebug("STORAGE_SIZE: %lu", (unsigned long)size() );

		for (uint32 i = 0; i < size(); ++i)
		{
			if ((i == (j * 8)) && ((i != (k * 16))))
			{
				if (read<uint8>(i) < 0x10)
				{
					sLog.outDebugInLine("| 0%X ", read<uint8>(i) );
				}
				else
				{
					sLog.outDebugInLine("| %X ", read<uint8>(i) );
				}
				++j;
			}
			else if (i == (k * 16))
			{
				if (read<uint8>(i) < 0x10)
				{
					sLog.outDebugInLine("\n");

					sLog.outDebugInLine("0%X ", read<uint8>(i) );
				}
				else
				{
					sLog.outDebugInLine("\n");

					sLog.outDebugInLine("%X ", read<uint8>(i) );
				}

				++k;
				++j;
			}
			else
			{
				if (read<uint8>(i) < 0x10)
				{
					sLog.outDebugInLine("0%X ", read<uint8>(i) );
				}
				else
				{
					sLog.outDebugInLine("%X ", read<uint8>(i) );
				}
			}
		}
		sLog.outDebugInLine("\n");
	}
#endif
	//////////////////////////////////////////////////////////////////////////
	//linbc add on 20100712
	template<typename T> 
	WorldPacket &operator>>(T& value)
	{
		value = read<T>();
		//read((uint8*)&value,sizeof(T));
		return *this;
	}

	template<typename T>
	void read_pos(T*& _p)
	{
		_p = (T*) &(m_pkt->content[m_pkt->rpos]);		
		m_pkt->rpos += sizeof(T);
	}

	template<typename T>
	WorldPacket &operator>>(T*& value)
	{
		read_pos(value);
		return *this;
	}

	template<typename T>
	WorldPacket &operator<<(T value)
	{
		append<T>(value);
		return *this;
	}

protected:
	PacketControls m_must_free;
	packet *m_pkt;
};

template <typename T>
inline WorldPacket &operator<<(WorldPacket &b, std::vector<T> v)
{
	b << (uint16)v.size();
	for (typename std::vector<T>::iterator i = v.begin(); i != v.end(); ++i)
	{
		b << *i;
	}
	return b;
}

template <typename T>
inline WorldPacket &operator>>(WorldPacket &b, std::vector<T> &v)
{
	uint16 vsize;
	b >> vsize;
	v.clear();
	while(vsize--)
	{
		T t;
		b >> t;
		v.push_back(t);
	}
	return b;
}

template <typename T>
inline WorldPacket &operator<<(WorldPacket &b, std::list<T> v)
{
	b << (uint16)v.size();
	for (typename std::list<T>::iterator i = v.begin(); i != v.end(); ++i)
	{
		b << *i;
	}
	return b;
}

template <typename T>
inline WorldPacket &operator>>(WorldPacket &b, std::list<T> &v)
{
	uint16 vsize;
	b >> vsize;
	v.clear();
	while(vsize--)
	{
		T t;
		b >> t;
		v.push_back(t);
	}
	return b;
}

template <typename T>
inline WorldPacket &operator<<(WorldPacket &b, std::set<T> &v)
{
	b << (uint16)v.size();
	for (typename std::set<T>::iterator i = v.begin(); i != v.end(); ++i)
	{
		b << *i;
	}
	return b;
}

template <typename T>
inline WorldPacket &operator>>(WorldPacket &b, std::set<T> &v)
{
	uint16 vsize;
	b >> vsize;
	v.clear();
	while(vsize--)
	{
		T t;
		b >> t;
		v.insert(t);
	}
	return b;
}

template <typename K, typename V>
inline WorldPacket &operator<<(WorldPacket &b, std::map<K, V> &m)
{
	b << (uint16)m.size();
	for (typename std::map<K, V>::iterator i = m.begin(); i != m.end(); ++i)
	{
		b << i->first << i->second;
	}
	return b;
}

template <typename K, typename V>
inline WorldPacket &operator>>(WorldPacket &b, std::map<K, V> &m)
{
	uint16 msize;
	b >> msize;
	m.clear();
	while(msize--)
	{
		K k;
		V v;
		b >> k >> v;
		m.insert(make_pair(k, v));
	}
	return b;
}

// TODO: Make a WorldPacket.cpp and move all this inlining to it.
template<> inline std::string WorldPacket::read<std::string>()
{
	std::string tmp;
	*this >> tmp;
	return tmp;
}

template<>
inline void WorldPacket::read_skip<char*>()
{
	std::string temp;
	*this >> temp;
}

template<>
inline void WorldPacket::read_skip<char const*>()
{
	read_skip<char*>();
}

template<>
inline void WorldPacket::read_skip<std::string>()
{
	read_skip<char*>();
}
#endif

