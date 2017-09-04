/************************************************************************
 * file	: Policies.h                                                                    
 * desc	: 该文件定义了对象管理的若干策略
 *			单例,对象实例,线程锁
 *
 * author:	linbc
 * date:	20110528
 * version:
 ************************************************************************/
#ifndef _POLICIES_H_
#define _POLICIES_H_

#include <cstdlib>
#include <exception>
#include <stdexcept>

namespace Tea
{
	//////////////////////////////////////////////////////////////////////////
	//对象创建策略

	/**
	 * OperatorNew policy creates an object on the heap using new.
	 */
	template <class T>
	class OperatorNew
	{
	public:
		static T* Create(void) { return (new T); }
		static void Destroy(T *obj) { delete obj; }
	};

	/**
	 * LocalStaticCreation policy creates an object on the stack
	 * the first time call Create.
	 */
	template <class T>
	class LocalStaticCreation
	{
		union MaxAlign
		{
			char t_[sizeof(T)];
			short int shortInt_;
			int int_;
			long int longInt_;
			float float_;
			double double_;
			long double longDouble_;
			struct Test;
			int Test::* pMember_;
			int (Test::*pMemberFn_)(int);
		};
	public:
		static T* Create(void)
		{
			static MaxAlign si_localStatic;
			return new(&si_localStatic) T;
		}

		static void Destroy(T *obj) { obj->~T(); }
	};

	/**
	 * CreateUsingMalloc by pass the memory manger.
	 */
	template<class T>
	class CreateUsingMalloc
	{
	public:
		static T* Create()
		{
			void* p = ::malloc(sizeof(T));
			if (!p) return 0;
			return new(p) T;
		}

		static void Destroy(T* p)
		{
			p->~T();
			::free(p);
		}
	};

	/**
	 * CreateOnCallBack creates the object base on the call back.
	 */
	template<class T, class CALL_BACK>
	class CreateOnCallBack
	{
	public:
		static T* Create()
		{
			return CALL_BACK::CreateCallBack();
		}

		static void Destroy(T *p)
		{
			CALL_BACK::DestroyCallBack(p);
		}
	};

	//////////////////////////////////////////////////////////////////////////
	//多线程策略
	template<typename MUTEX>
	class GeneralLock
	{
	public:
		GeneralLock(MUTEX &m) : i_mutex(m)
		{
			nd_mutex_lock(&i_mutex);
		}

		~GeneralLock()
		{
			nd_mutex_unlock(&i_mutex);
		}
	private:
		GeneralLock(const GeneralLock &);
		GeneralLock& operator=(const GeneralLock &);
		MUTEX& i_mutex;
	};

	template <class T>
	class SingleThreaded
	{
	public:

		struct Lock                                     // empty object
		{
			Lock() {}
			Lock(const T &) {}
			Lock(const SingleThreaded<T> &)             // for single threaded we ignore this
			{
			}
		};

		typedef T VolatileType;
	};	

	//////////////////////////////////////////////////////////////////////////
	//单例

	template<typename T,class ThreadingModel = SingleThreaded<T>,class CreatePolicy = OperatorNew<T> >
	class Singleton
	{
	public:		

		static T& Instance()
		{
			if( !si_instance )
			{
				// double-checked Locking pattern
				Guard();
				if( !si_instance )
				{
					if( si_destroyed )
					{
						si_destroyed = false;
						throw std::runtime_error("Dead Reference");
					}
					si_instance = CreatePolicy::Create();
					std::atexit(&DestroySingleton);
				}
			}

			return *si_instance;
		}

	protected:
		Singleton() {};

	private:

		// Prohibited actions...this does not prevent hijacking.
		Singleton(const Singleton &);
		Singleton& operator=(const Singleton &);

		// Singleton Helpers
		static void DestroySingleton()
		{
			CreatePolicy::Destroy(si_instance);
			si_instance = NULL;
			si_destroyed = true;
		}

		// data structure
		typedef typename ThreadingModel::Lock Guard;
		static T *si_instance;
		static bool si_destroyed;
	};

	//////////////////////////////////////////////////////////////////////////
	//对象关系管理		
#if 0
	template <typename LT,typename RT>
	class RefManager
	{
	public:
		struct relation
		{
			LT* left;
			RT* right;

			void link()
			{
				if(left->OnAddRef != NULL)
					left->OnAddRef();
			}

			void unlink()
			{

			}
		};

		typedef vector<relation> relations;

		class IRefable
		{
		protected:
			relation& m_ref;
			
			void OnAddRef();		
			void OnDelRef();
		};

	public:
		void AddRef(LT* lt,RT* rt)
		{

		}

		void DelRef();

	protected:
		typename relations m_relation;
	};

#endif

}


#define INSTANTIATE_SINGLETON_1(TYPE) \
	template class Tea::Singleton<TYPE, Tea::SingleThreaded<TYPE>, Tea::OperatorNew<TYPE> >; \
	template<> TYPE* Tea::Singleton<TYPE, Tea::SingleThreaded<TYPE>, Tea::OperatorNew<TYPE> >::si_instance = 0; \
	template<> bool Tea::Singleton<TYPE, Tea::SingleThreaded<TYPE>, Tea::OperatorNew<TYPE> >::si_destroyed = false


#define INSTANTIATE_SINGLETON_2(TYPE, THREADINGMODEL) \
	template class Tea::Singleton<TYPE, THREADINGMODEL, Tea::OperatorNew<TYPE> >; \
	template<> TYPE* Tea::Singleton<TYPE, THREADINGMODEL, Tea::OperatorNew<TYPE> >::si_instance = 0; \
	template<> bool Tea::Singleton<TYPE, THREADINGMODEL, Tea::OperatorNew<TYPE> >::si_destroyed = false

#define INSTANTIATE_SINGLETON_3(TYPE, THREADINGMODEL, CREATIONPOLICY ) \
	template class Tea::Singleton<TYPE, THREADINGMODEL, CREATIONPOLICY>; \
	template<> TYPE* Tea::Singleton<TYPE, THREADINGMODEL, CREATIONPOLICY>::si_instance = 0; \
	template<> bool Tea::Singleton<TYPE, THREADINGMODEL, CREATIONPOLICY >::si_destroyed = false

#endif

