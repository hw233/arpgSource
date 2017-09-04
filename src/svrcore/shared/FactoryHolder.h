#ifndef _FACTORY_HOLDER_H_
#define _FACTORY_HOLDER_H_

#include "Policies.h"
#include <string>
#include <vector>
#include <map>

namespace Tea
{
	/** ObjectRegistry holds all registry item of the same type
	*/
	template<class T, class Key = std::string>
	class ObjectRegistry
	{
	public:
		typedef std::map<Key, T *> RegistryMapType;

		/// Returns a registry item
		const T* GetRegistryItem(Key key) const
		{
			typename RegistryMapType::const_iterator iter = i_registeredObjects.find(key);
			return( iter == i_registeredObjects.end() ? NULL : iter->second );
		}

		/// Inserts a registry item
		bool InsertItem(T *obj, Key key, bool override = false)
		{
			typename RegistryMapType::iterator iter = i_registeredObjects.find(key);
			if( iter != i_registeredObjects.end() )
			{
				if( !override )
					return false;
				delete iter->second;
				i_registeredObjects.erase(iter);
			}

			i_registeredObjects[key] = obj;
			return true;
		}

		/// Removes a registry item
		void RemoveItem(Key key, bool delete_object = true)
		{
			typename RegistryMapType::iterator iter = i_registeredObjects.find(key);
			if( iter != i_registeredObjects.end() )
			{
				if( delete_object )
					delete iter->second;
				i_registeredObjects.erase(iter);
			}
		}

		/// Returns true if registry contains an item
		bool HasItem(Key key) const
		{
			return (i_registeredObjects.find(key) != i_registeredObjects.end());
		}

		/// Inefficiently return a vector of registered items
		unsigned int GetRegisteredItems(std::vector<Key> &l) const
		{
			unsigned int sz = l.size();
			l.resize(sz + i_registeredObjects.size());
			for (typename RegistryMapType::const_iterator iter = i_registeredObjects.begin(); iter != i_registeredObjects.end(); ++iter)
				l[sz++] = iter->first;
			return i_registeredObjects.size();
		}

		/// Return the map of registered items
		RegistryMapType const &GetRegisteredItems() const
		{
			return i_registeredObjects;
		}

	private:
		RegistryMapType i_registeredObjects;
		friend class Tea::OperatorNew<ObjectRegistry<T, Key> >;

		// protected for friend use since it should be a singleton
		ObjectRegistry() {}
		~ObjectRegistry()
		{
			for (typename RegistryMapType::iterator iter=i_registeredObjects.begin(); iter != i_registeredObjects.end(); ++iter)
				delete iter->second;
			i_registeredObjects.clear();
		}
	};

	/** FactoryHolder holds a factory object of a specific type
	 */
	template<class T, class Key = std::string,class CreatePolicy = OperatorNew<T> >
	class FactoryHolder
	{
	public:
		typedef ObjectRegistry<FactoryHolder<T, Key >, Key > FactoryHolderRegistry;
		typedef Tea::Singleton<FactoryHolderRegistry> FactoryHolderRepository;

		FactoryHolder(Key k) : i_key(k) {}
		virtual ~FactoryHolder() {}
		inline Key key() const { return i_key; }

		void RegisterSelf(void) { FactoryHolderRepository::Instance().InsertItem(this, i_key); }
		void DeregisterSelf(void) { FactoryHolderRepository::Instance().RemoveItem(this, false); }

		/// Abstract Factory create method
		virtual T* Create(void *data = NULL) const = 0;
	private:
		Key i_key;
	};

	/** Permissible is a classic way of letting the object decide
	 * whether how good they handle things.  This is not retricted
	 * to factory selectors.
	 */
	template<class T>
	class Permissible
	{
	public:
		virtual ~Permissible() {}
		virtual int Permit(const T *) const = 0;
	};

}

#endif

