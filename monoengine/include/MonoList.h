#pragma once

#include "MonoTypes.h"
#include "MonoObject.h"

#include <mono/jit/jit.h>

// TODO: should this be a Mono::ObjectPtr only list for now?

namespace Mono
{
	struct ListObjectWrapper
	{
		ObjectPtr Wrap(ObjectPtr obj)
		{
			return obj;
		}
	};

	template <typename TItem, class TWrap = ListObjectWrapper>
	class List
	{
	public:
		List(MonoObject* list);

		// Gets or sets the total number of elements the internal data structure can hold without resizing.
		int GetCapacity() const;

		// Gets the number of elements contained in the List<T>.
		int GetCount() const;

		// Gets or sets the element at the specified index.
		TItem GetItem(int index);

		// Gets or sets the element at the specified index.
		void SetItem(int index, TItem item);

	private:
		ObjectPtr m_list;
	};

	template<typename TItem, typename TWrap>
	inline List<TItem, TWrap>::List(MonoObject* list)
	{
		m_list = ObjectPtr(new Object(list));
	}

	template<typename TItem, typename TWrap>
	inline int List<TItem, TWrap>::GetCapacity() const
	{
		if (m_list.get() == nullptr)
		{
			return -1;
		}
		return m_list->GetProperty<int>("Capacity");
	}

	template<typename TItem, typename TWrap>
	inline int List<TItem, TWrap>::GetCount() const
	{
		if (m_list.get() == nullptr)
		{
			return -1;
		}
		return m_list->GetProperty<int>("Count");
	}

	template<typename TItem, typename TWrap>
	inline TItem List<TItem, TWrap>::GetItem(int index)
	{
		if (m_list.get() == nullptr)
		{
			return TItem();
		}
		Mono::Args args;
		args.Add(&index);
		auto item = m_list->CallMethod<ObjectPtr>("get_Item", args);
		if(item)
		{
			return TWrap().Wrap(item);
		}
		return TItem();
	}

	template<typename TItem, typename TWrap>
	inline void List<TItem, TWrap>::SetItem(int index, TItem item)
	{
	}
}
