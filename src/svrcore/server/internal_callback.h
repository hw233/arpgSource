#ifndef _INTERNAL_CALLBACK_H_
#define _INTERNAL_CALLBACK_H_

#include <comm/common_stl.h>
#include <functional>


template<typename PARAM>
struct InternalCallback
{
	typedef std::function<void(string,PARAM& pkt)> func;
	typedef std::pair<uint32,func> pair;
	typedef vector<pair> holders;	

	InternalCallback():m_callback_index(0){}

	holders m_holders;
	uint32 m_callback_index;	
	inline uint32 reg(func f)
	{
		m_callback_index++;
		typename holders::iterator it = std::lower_bound(m_holders.begin(),m_holders.end(),m_callback_index,
			[](const pair& left,const uint32 right){return left.first < right;}
		);
		m_holders.insert(it,std::make_pair(m_callback_index,f));
		return m_callback_index;
	}

	inline void call(uint32 i,const string &guidlow,PARAM& p)
	{
		typename holders::iterator it = std::lower_bound(m_holders.begin(),m_holders.end(),i,
			[](const pair& left,const uint32 right){return left.first < right;});
		if(it != m_holders.end() && it->first == i)
		{
			it->second(guidlow,p);		//注意了，你不能再去读key了，key已经被读出了
			m_holders.erase(it);
		}
	}
};


#endif
