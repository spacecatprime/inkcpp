#pragma once

#include "Types.h"

namespace Ink
{
	/// The underlying type for a list item in ink. It stores the original list definition
	/// name as well as the item name, but without the value of the item. When the value is
	/// stored, it's stored in a KeyValuePair of InkListItem and int.
	struct IInkListItem
	{
	public:
		/// The name of the list where the item was originally defined.
		virtual const std::string& GetOriginName() = 0;

		/// The main name of the item as defined in ink.
		virtual const std::string& GetItemName() = 0;

		/// Create an item with the given original list definition name, and the name of this item.
		virtual void Construct_InkListItem(const std::string& originName, const std::string& itemName) = 0;

		/// Create an item from a dot-separted string of the form "listDefinitionName.listItemName".
		virtual void Construct_InkListItem(const std::string& fullName) = 0;

		/// Get the full dot-separated name of the item, in the form "listDefinitionName.itemName".
		virtual const std::string& GetFullName() const = 0;

		/// Get the full dot-separated name of the item, in the form "listDefinitionName.itemName".
		/// Calls fullName internally.
		virtual const std::string& ToString() = 0;

		/// Is this item the same as another item?
		virtual bool Equals(IObject* obj) = 0;

		/// Get the hashcode for an item.
		virtual int GetHashCode() = 0;
	};

	/// The InkList is the underlying type that's used to store an instance of a
	/// list in ink. It's not used for the *definition* of the list, but for a list
	/// value that's stored in a variable.
	/// Somewhat confusingly, it's backed by a C# Dictionary, and has nothing to
	/// do with a C# List!
	class IInkList // : Dictionary<InkListItem, int>
	{
		/// Create a new empty ink list.
		virtual void Construct_InkList() = 0;

		/// Create a new ink list that contains the same contents as another list.
		virtual void Construct_InkList(IInkList* otherList) = 0;

		/// Create a new empty ink list that's intended to hold items from a particular origin
		/// list definition. The origin Story is needed in order to be able to look up that definition.
		virtual void Construct_InkList(const std::string& singleOriginListName, IStory* originStory) = 0;

		/// Adds the given item to the ink list. Note that the item must come from a list definition that
		/// is already "known" to this list, so that the item's value can be looked up. By "known", we mean
		/// that it already has items in it from that source, or it did at one point - it can't be a 
		/// completely fresh empty list, or a list that only contains items from a different list definition.
		virtual void AddItem(IInkListItem* item) = 0;

		/// Adds the given item to the ink list, attempting to find the origin list definition that it belongs to.
		/// The item must therefore come from a list definition that is already "known" to this list, so that the
		/// item's value can be looked up. By "known", we mean that it already has items in it from that source, or
		/// it did at one point - it can't be a completely fresh empty list, or a list that only contains items from
		/// a different list definition.
		virtual void AddItem(const std::string& itemName) = 0;

		/// Returns true if this ink list contains an item with the given short name
		/// (ignoring the original list where it was defined).
		virtual bool ContainsItemNamed(const std::string& itemName) = 0;

		/// Get the maximum item in the list, equivalent to calling LIST_MAX(list) in ink.
		//virtual KeyValuePair<IInkListItem*, int> get_maxItem() = 0;

		/// The inverse of the list, equivalent to calling LIST_INVERSE(list) in ink
		virtual IInkList* get_inverse() = 0;

		/// The list of all items from the original list definition, equivalent to calling
		/// LIST_ALL(list) in ink.
		virtual IInkList* get_all() = 0;

		/// Returns a new list that is the combination of the current list and one that's
		/// passed in. Equivalent to calling (list1 + list2) in ink.
		virtual IInkList* Union(IInkList* otherList) = 0;

		/// Returns a new list that is the intersection of the current list with another
		/// list that's passed in - i.e. a list of the items that are shared between the
		/// two other lists. Equivalent to calling (list1 ^ list2) in ink.
		virtual IInkList* Intersect(IInkList* otherList) = 0;

		/// Returns a new list that's the same as the current one, except with the given items
		/// removed that are in the passed in list. Equivalent to calling (list1 - list2) in ink.
		/// <param name="listToRemove">List to remove.</param>
		virtual IInkList* Without(IInkList* listToRemove) = 0;

		/// Returns true if the current list contains all the items that are in the list that
		/// is passed in. Equivalent to calling (list1 ? list2) in ink.
		/// <param name="otherList">Other list.</param>
		virtual bool Contains(IInkList* otherList) = 0;

		/// Returns true if all the item values in the current list are greater than all the
		/// item values in the passed in list. Equivalent to calling (list1 > list2) in ink.
		virtual bool GreaterThan(IInkList* otherList) = 0;

		/// Returns true if the item values in the current list overlap or are all greater than
		/// the item values in the passed in list. None of the item values in the current list must
		/// fall below the item values in the passed in list. Equivalent to (list1 >= list2) in ink,
		/// or LIST_MIN(list1) >= LIST_MIN(list2) &amp;&amp; LIST_MAX(list1) >= LIST_MAX(list2).
		virtual bool GreaterThanOrEquals(IInkList* otherList) = 0;

		/// Returns true if all the item values in the current list are less than all the
		/// item values in the passed in list. Equivalent to calling (list1 &lt; list2) in ink.
		virtual bool LessThan(IInkList* otherList) = 0;

		/// Returns true if the item values in the current list overlap or are all less than
		/// the item values in the passed in list. None of the item values in the current list must
		/// go above the item values in the passed in list. Equivalent to (list1 &lt;= list2) in ink,
		/// or LIST_MAX(list1) &lt;= LIST_MAX(list2) &amp;&amp; LIST_MIN(list1) &lt;= LIST_MIN(list2).
		virtual bool LessThanOrEquals(IInkList* otherList) = 0;

		/// Returns true if the passed object is also an ink list that contains
		/// the same items as the current list, false otherwise.
		virtual bool Equals(IObject* other) = 0;

		/// Return the hashcode for this object, used for comparisons and inserting into dictionaries.;
		virtual int GetHashCode() = 0;

		/// Returns a string in the form "a, b, c" with the names of the items in the list, without
		/// the origin list definition names. Equivalent to writing {list} in ink.
		virtual const std::string ToString() = 0;
	};

	/// Exception that represents an error when running a Story at runtime.
	/// An exception being thrown of this type is typically when there's
	/// a bug in your ink, rather than in the ink engine itself!
	class IStoryException //: System.Exception
	{
		virtual const std::string& GetMessage() = 0;
	};
};