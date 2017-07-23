#pragma once

#include "Types.h"

namespace Ink
{
	/// The InkList is the underlying type that's used to store an instance of a
	/// list in ink. It's not used for the _definition_ of the list, but for a list
	/// value that's stored in a variable.
	/// Somewhat confusingly, it's backed by a C# Dictionary, and has nothing to
	/// do with a C# List!
	class IInkList // is_a Dictionary<InkListItem, int>
	{
		/// Create a new empty ink list.
		virtual void Construct_InkList() = 0;

		/// Create a new ink list that contains the same contents as another list.
		virtual void Construct_InkList(InkListPtr otherList) = 0;

		/// Create a new empty ink list that's intended to hold items from a particular origin
		/// list definition. The origin Story is needed in order to be able to look up that definition.
		virtual void Construct_InkList(const std::string& singleOriginListName, StoryPtr originStory) = 0;

		/// Adds the given item to the ink list. Note that the item must come from a list definition that
		/// is already "known" to this list, so that the item's value can be looked up. By "known", we mean
		/// that it already has items in it from that source, or it did at one point - it can't be a 
		/// completely fresh empty list, or a list that only contains items from a different list definition.
		virtual void AddItem(InkListItemPtr item) = 0;

		/// Adds the given item to the ink list, attempting to find the origin list definition that it belongs to.
		/// The item must therefore come from a list definition that is already "known" to this list, so that the
		/// item's value can be looked up. By "known", we mean that it already has items in it from that source, or
		/// it did at one point - it can't be a completely fresh empty list, or a list that only contains items from
		/// a different list definition.
		virtual void AddItem(const std::string& itemName) = 0;

		/// Returns true if this ink list contains an item with the given short name
		/// (ignoring the original list where it was defined).
		virtual bool ContainsItemNamed(const std::string& itemName) = 0;

// TODO needed?
		/// Get the maximum item in the list, equivalent to calling LIST_MAX(list) in ink.
		//virtual KeyValuePair<InkListItemPtr, int> get_maxItem() = 0;

		/// The inverse of the list, equivalent to calling LIST_INVERSE(list) in ink
		virtual InkListPtr get_inverse() = 0;

		/// The list of all items from the original list definition, equivalent to calling
		/// LIST_ALL(list) in ink.
		virtual InkListPtr get_all() = 0;

		/// Returns a new list that is the combination of the current list and one that's
		/// passed in. Equivalent to calling (list1 + list2) in ink.
		virtual InkListPtr Union(InkListPtr otherList) = 0;

		/// Returns a new list that is the intersection of the current list with another
		/// list that's passed in - i.e. a list of the items that are shared between the
		/// two other lists. Equivalent to calling (list1 ^ list2) in ink.
		virtual InkListPtr Intersect(InkListPtr otherList) = 0;

		/// Returns a new list that's the same as the current one, except with the given items
		/// removed that are in the passed in list. Equivalent to calling (list1 - list2) in ink.
		/// <param name="listToRemove">List to remove.</param>
		virtual InkListPtr Without(InkListPtr listToRemove) = 0;

		/// Returns true if the current list contains all the items that are in the list that
		/// is passed in. Equivalent to calling (list1 ? list2) in ink.
		/// <param name="otherList">Other list.</param>
		virtual bool Contains(InkListPtr otherList) = 0;

		/// Returns true if all the item values in the current list are greater than all the
		/// item values in the passed in list. Equivalent to calling (list1 > list2) in ink.
		virtual bool GreaterThan(InkListPtr otherList) = 0;

		/// Returns true if the item values in the current list overlap or are all greater than
		/// the item values in the passed in list. None of the item values in the current list must
		/// fall below the item values in the passed in list. Equivalent to (list1 >= list2) in ink,
		/// or LIST_MIN(list1) >= LIST_MIN(list2) &amp;&amp; LIST_MAX(list1) >= LIST_MAX(list2).
		virtual bool GreaterThanOrEquals(InkListPtr otherList) = 0;

		/// Returns true if all the item values in the current list are less than all the
		/// item values in the passed in list. Equivalent to calling (list1 &lt; list2) in ink.
		virtual bool LessThan(InkListPtr otherList) = 0;

		/// Returns true if the item values in the current list overlap or are all less than
		/// the item values in the passed in list. None of the item values in the current list must
		/// go above the item values in the passed in list. Equivalent to (list1 &lt;= list2) in ink,
		/// or LIST_MAX(list1) &lt;= LIST_MAX(list2) &amp;&amp; LIST_MIN(list1) &lt;= LIST_MIN(list2).
		virtual bool LessThanOrEquals(InkListPtr otherList) = 0;

		/// Returns true if the passed object is also an ink list that contains
		/// the same items as the current list, false otherwise.
		virtual bool Equals(ObjectPtr other) = 0;

		/// Return the hash code for this object, used for comparisons and inserting into dictionaries.;
		virtual int GetHashCode() = 0;

		/// Returns a string in the form "a, b, c" with the names of the items in the list, without
		/// the origin list definition names. Equivalent to writing {list} in ink.
		virtual const std::string ToString() = 0;
	};
};