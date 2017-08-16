#pragma once
// ********************************************************
// TBD
// ********************************************************

#include "../Types.h"

namespace Ink
{
	/// The underlying type for a list item in ink. It stores the original list definition
	/// name as well as the item name, but without the value of the item. When the value is
	/// stored, it's stored in a KeyValuePair of InkListItem and int.
	class IInkListItem
	{
	public:
		/// The name of the list where the item was originally defined.
		virtual const std::string& GetOriginName() = 0;

		/// The main name of the item as defined in ink.
		virtual const std::string& GetItemName() = 0;

		/// Create an item with the given original list definition name, and the name of this item.
		virtual void Construct_InkListItem(const std::string& originName, const std::string& itemName) = 0;

		/// Create an item from a dot-separated string of the form "listDefinitionName.listItemName".
		virtual void Construct_InkListItem(const std::string& fullName) = 0;

		/// Get the full dot-separated name of the item, in the form "listDefinitionName.itemName".
		virtual const std::string& GetFullName() const = 0;

		/// Get the full dot-separated name of the item, in the form "listDefinitionName.itemName".
		/// Calls fullName internally.
		virtual const std::string& ToString() = 0;

		/// Is this item the same as another item?
		virtual bool Equals(ContentObjectPtr obj) = 0;

		/// Get the hash code for an item.
		virtual int GetHashCode() = 0;
	};
};