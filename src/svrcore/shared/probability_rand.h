#ifndef _PROBABILITY_RAND_H_
#define _PROBABILITY_RAND_H_
#include "comm/common_stl.h"

//概率随机类
template<typename T = int>
class ProbabilityRand
{
public:	
	struct rand_member
	{		
		uint16 probaqbility;
		T values;
	};
	typedef vector<rand_member> RandVec;
	typedef bool (*needDelChoiceFunc)(rand_member* rm,void *param);

	ProbabilityRand():m_total_probaqbility(0)
	{
	}
	~ProbabilityRand()
	{		
	}
public:

	void AddRandPossibility(uint16 probaqbility,T values) //增加需要随机的成员
	{
		//添加新成员
		m_rand_members.resize(m_rand_members.size()+1);
		typename RandVec::reverse_iterator it = m_rand_members.rbegin();
		it->probaqbility = probaqbility;
		it->values = values;

		//概率存根
		m_probaqbility_stub.push_back(probaqbility);
		//总概率统计
		m_total_probaqbility+=probaqbility;
	}

	T Rand(bool is_repeat = true);							//根据概率随机出值,默认可重复出随机的
	bool empty() {return m_rand_members.empty();};			//是否空的
	void Reset()											//重置成员概率
	{
		m_total_probaqbility = 0;
		for (uint32 i =0;i<m_rand_members.size();i++)
		{
			m_total_probaqbility+=m_probaqbility_stub[i];
			m_rand_members[i]->probaqbility = m_probaqbility_stub[i];
		}
	}
	void clear()
	{		
		m_rand_members.clear();
		m_probaqbility_stub.clear();
		m_total_probaqbility = 0;
	}

	void SetProbaqbility(needDelChoiceFunc delChoiceFunc,void *param)
	{
		for (uint32 i =0;i<m_rand_members.size();i++)
		{
			if(delChoiceFunc(&m_rand_members[i],param))
			{
				m_total_probaqbility -= m_rand_members[i].probaqbility;
				m_rand_members[i].probaqbility = 0;
			}
		}
	}

private:
	uint32 m_total_probaqbility;						//总概率
	RandVec m_rand_members;								//随机成员
	std::vector<uint16> m_probaqbility_stub;			//所有成员的概率存根，用于重置
};

//根据概率随机出值,多次随机可能随机出重复的
template<typename T>
T ProbabilityRand<T>::Rand(bool is_repeat)	
{
	ASSERT(m_rand_members.size());
	//随机一个数在0~总概率之间
	uint32 rand_num = urand(0,m_total_probaqbility - 1);
	//接下来判断随机到哪一个成员的地盘 并且返回值
	//因为要知道此时到了什么下标位置，用这种方法遍历会方便些
	for (uint32 i = 0;i<m_rand_members.size();i++)
	{
		if(rand_num < m_rand_members[i].probaqbility)
		{		
			//不可重复 概率置0 总概率降低
			if(!is_repeat)
			{
				m_total_probaqbility -= m_rand_members[i].probaqbility;
				m_rand_members[i].probaqbility=0;
			}
			return m_rand_members[i].values;
		}
		rand_num -= m_rand_members[i].probaqbility;
	}
	//到这边没返回就是错误了
	ASSERT(false);
	return T();
}

#endif 

