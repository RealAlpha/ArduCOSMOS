/*
 * This file contains a simple implementation of a linked list, and will be used in the code when the C++ Standard Library has not been marked as available by the user.
 * If your toolchain supports the C++ Standard Library, and more specifically, std::vector, then please uncomment the WITH_STD_LIB define in arducosmos.h, which will result in that being used instead of this library.
 * 
 * NOTE: This linked list was primarily implemented as a way to avoid (unrequired) third-party libraries, more specifically, ArduinoSTL (which is a great library by the way, but depending on it for something as simple as this was considered not necissery)
 */

#ifndef ARDUCOSMOS_LINKEDLIST_H_
#define ARDUCOSMOS_LINKEDLIST_H_
// LinkedList may be used by other libraries too, so we are wrapping it in the ArduCOSMOS namespace.
namespace NS_ArduCOSMOS
{
	// NOTE: We'll only be implementing the required functions (add to and iteration at the time of writing this comment), so this isn't a fully-fledged linked list function. Templating used since this is 
	template <typename T> class LinkedList
	{
	public:
		// Simple struct to store the required data and a pointer to the next node. If the pointer to the next node is null, it is the final node in the list.
		struct ListNode
		{
		public:
			T data;
			ListNode *next = nullptr;

			// Is the current node the last item in the list?
			bool IsLast()
			{
				// A node is the last in the list if there isn't a next element -> next is a nullptr
				return next == nullptr;
			}

		};
		
		// Iterator class
		/// NOTE: Helps abstract things and avoid pointers // helps a more streamlined interface
		class Iterator
		{
		public:
			// Default constructor; takes in a node: the node at which this iterator is pointing.
			Iterator(ListNode *Node)
			{
				this->Node = Node;
			}

			// Overload the ++ operator so we can use it to iterate.
			Iterator operator++(int)
			{
				// Is the current node valid, or not? (for example: is this beyond the end of the list?)
				if (!Node) { return Iterator(nullptr); }

				// Return an iterator to the next node
				return Iterator(Node->next);
			}

			// Overload the dereference operator(s) to return the data so we can use it with the same code as std::vector
			T &operator->()
			{
				// Is the current node valid, or not? (for example: is this beyond the end of the list?)
				/// NOTE: If there are any exceptions about a constructor of a type not existing, make sure you have a default no-parameter constructor available in case you iterate over the end of a list // it needs to return a blank item.
				/// NOTE II: The below is generally Bad Practive / "Evil", but Node should never be a nullptr anyway, or the user'll have iterated too far and still requested the data.
				if (!Node)
				{
					T badPracticeButRequired = T();
					return badPracticeButRequired;
				}

				// Since the current node is valid, we can safely return it's data
				return Node->data;
			}
			// NOTE: You may need to use **it to first dereference the ListNode, and then hit this operator to get the data. You won't hit this when using
			T &operator*()
			{
				// Is the current node valid, or not? (for example: is this beyond the end of the list?)
				/// NOTE: If there are any exceptions about a constructor of a type not existing, make sure you have a default no-parameter constructor available in case you iterate over the end of a list // it needs to return a blank item.
				/// NOTE II: The below is generally Bad Practive / "Evil", but Node should never be a nullptr anyway, or the user'll have iterated too far and still requested the data.
				if (!Node)
				{
					T badPracticeButRequired = T();
					return badPracticeButRequired;
				}

				// Since the current node is valid, we can safely return it's data
				return Node->data;
			}

			// Gets the "low-level" list node "representation. Used for passing to erase.
			ListNode *GetLowLevel()
			{
				return Node;
			}

			// TODO: Override the is valid (ie if (someIterator)) operator so we don't go beyond the end.
			operator bool() const
			{
					return Node != nullptr;
			}

		private:
			// The node this iterator is currently pointing at.
			ListNode *Node;
		};

	public:
		// Returns an iterator made from the first node in the list.
		/// NOTE: small b used to be virtually identical to the std::vector library.
		Iterator begin()
		{
			return Iterator(firstNode);
		}

		// Appends an item (to the start of) the list.
		void push_back(T data)
		{
			// Allocate some memory for the new node.
			ListNode *newNode = new ListNode();
			if (!newNode) { return; }

			// Set the data of the new item to the requested data
			newNode->data = data;

			// Insert it into the linked list at the front
			newNode->next = firstNode;
			firstNode = newNode;
		}

		// Erase the whole linked list
		void erase()
		{
			// A wrapper for the recursive erase function. NOTE: firstNode can safely be a nullptr.
			erase_r(firstNode);
		}

		// Erase a single item in a linked list.
		void erase(ListNode *node)
		{
			// Keep track of the current and previous node.
			ListNode *previous = nullptr;
			ListNode *trav = firstNode;
			while (trav != nullptr)
			{
				// Is this the node we should erase?
				if (trav == node)
				{
					// We need to erase this node!

					// Is there a previous node? (wecan assume trav is not a nullptr -> is valid)
					if (previous != nullptr)
					{
						// Since we'll be removing this node, we want to make sre the previous node links to the next node so we don't loose any data.
						previous->next = trav->next;

						// We don't need to do anything else because this is NOT a doubly linked list and the list can still be iterated. We'll delete/remove this node down below.
					}
					else
					{
						// No previous node, so we need to erase the first item in the list -> delete this node, and set the first node to the next item.
						/// NOTE: If next is a nullptr that causes no issues, so we don't need any checking.
						firstNode = node->next;
					}

					// Now that we've safely stored the rest of the list, delete this node to free up the memory
					delete node;

					// Finished all of the required things, so return out of the function.
					return;
				}

				// Update the loop to go to the next node in the list.
				previous = trav;
				trav = trav->next;
			}
		}
private:
		// Erase the linked list including the specified node and all nodes following it.
		/// NOTE: Private because we don't want half the linked list to be erased and next not being a nullptr -> cause segfaults.
		void erase_r(ListNode *node)
		{
			// Is this the last item? (if so no need to erase or continue further down)
			if (node == nullptr)
			{
				return;
			}
			else
			{
				// Traverse down first deleting child nodes, and then delete itself.
				erase_r(node->next);
				delete node;
			}
		}

		// TODO: Deal with cleanup in the deconstructor!

	private:
		ListNode *firstNode = nullptr;
	};
};

#endif
