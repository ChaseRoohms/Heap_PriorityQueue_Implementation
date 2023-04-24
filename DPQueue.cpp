// FILE: DPQueue.cpp
// IMPLEMENTS: p_queue (see DPQueue.h for documentation.)
//
// INVARIANT for the p_queue class:
//   1. The number of items in the p_queue is stored in the member
//      variable used.
//   2. The items themselves are stored in a dynamic array (partially
//      filled in general) organized to follow the usual heap storage
//      rules.
//      2.1 The member variable heap stores the starting address
//          of the array (i.e., heap is the array's name). Thus,
//          the items in the p_queue are stored in the elements
//          heap[0] through heap[used - 1].
//      2.2 The member variable capacity stores the current size of
//          the dynamic array (i.e., capacity is the maximum number
//          of items the array currently can accommodate).
//          NOTE: The size of the dynamic array (thus capacity) can
//                be resized up or down where needed or appropriate
//                by calling resize(...).
// NOTE: Private helper functions are implemented at the bottom of
// this file along with their precondition/postcondition contracts.

#include <cassert>   // provides assert function
#include <iostream>  // provides cin, cout
#include <iomanip>   // provides setw
#include <cmath>     // provides log2
#include "DPQueue.h"

using namespace std;

namespace CS3358_SP2023_A7
{
   // EXTRA MEMBER FUNCTIONS FOR DEBUG PRINTING
   void p_queue::print_tree(const char message[], size_type i) const
   // Pre:  (none)
   // Post: If the message is non-empty, it has first been written to
   //       cout. After that, the portion of the heap with root at
   //       node i has been written to the screen. Each node's data
   //       is indented 4*d, where d is the depth of the node.
   //       NOTE: The default argument for message is the empty string,
   //             and the default argument for i is zero. For example,
   //             to print the entire tree of a p_queue p, with a
   //             message of "The tree:", you can call:
   //                p.print_tree("The tree:");
   //             This call uses the default argument i=0, which prints
   //             the whole tree.
   {
      const char NO_MESSAGE[] = "";
      size_type depth;

      if (message[0] != '\0')
         cout << message << endl;

      if (i >= used)
         cout << "(EMPTY)" << endl;
      else
      {
         depth = size_type( log( double(i+1) ) / log(2.0) + 0.1 );
         if (2*i + 2 < used)
            print_tree(NO_MESSAGE, 2*i + 2);
         cout << setw(depth*3) << "";
         cout << heap[i].data;
         cout << '(' << heap[i].priority << ')' << endl;
         if (2*i + 1 < used)
            print_tree(NO_MESSAGE, 2*i + 1);
      }
   }

   void p_queue::print_array(const char message[]) const
   // Pre:  (none)
   // Post: If the message is non-empty, it has first been written to
   //       cout. After that, the contents of the array representing
   //       the current heap has been written to cout in one line with
   //       values separated one from another with a space.
   //       NOTE: The default argument for message is the empty string.
   {
      if (message[0] != '\0')
         cout << message << endl;

      if (used == 0)
         cout << "(EMPTY)" << endl;
      else
         for (size_type i = 0; i < used; i++)
            cout << heap[i].data << ' ';
   }


//Constructor, Verifies capacity to be valid, if not overwrites with default
   p_queue::p_queue(size_type initial_capacity) : capacity(initial_capacity),
                                                  used(0)
   {
       if (initial_capacity < 1){
           capacity = DEFAULT_CAPACITY;
      }
      heap = new ItemType[capacity];
   }

//Copy Constructor, Creates heap of same capacity, and same used, copies data
   p_queue::p_queue(const p_queue& src) : capacity(src.capacity), used(src.used)
   {
       heap = new ItemType[capacity];
       for (size_type i = 0; i < capacity; i++){
            heap[i] = src.heap[i];
      }

   }

//Destructor
   p_queue::~p_queue()
   {
      delete [] heap;
      heap = 0;
   }



   // MODIFICATION MEMBER FUNCTIONS


// = Operator overload
   p_queue& p_queue::operator=(const p_queue& rhs)
   {
       if (this != &rhs)
        {
    //Create a new dynamic array of initial capacity set to that of the to be
    //copied arrays capacity
            ItemType* newHeap = new ItemType[rhs.capacity];

    //Cycles through the arrays, copying each value over to the new array
            for (size_type i = 0; i < rhs.used; i++){
                newHeap[i] = rhs.heap[i];
            }
    //Remove data from old array
            delete [] heap;

    //Transfer data from new array to old array
            heap = newHeap;
            capacity = rhs.capacity;
            used = rhs.used;
            }
   return *this;
   }


   void p_queue::push(const value_type& entry, size_type priority)
   {
        if (used >= capacity){
            //Increases the capacity by roughly 1.5 times or at least by 1
            resize(size_type(1.5*capacity) + 1);
        }
        //Adds new value to the end of the array
        heap[used].data = entry;
        heap[used].priority = priority;
        size_type i = used;
        used++;


        //ReHeapify the SemiHeap
        //(While not at Root, if current node has higher priority than its
        //parent, swap!
        while(i !=0 && heap[i].priority > parent_priority(i)){
            swap_with_parent(i);
            i = parent_index(i);

        }

   }

   void p_queue::pop(){
        assert(size() > 0);
        //If only one item, no swapping necessary
        if (used == 1){
            used--;
            return;
        }

        //Replace first item with last item
        heap[0].data = heap[used-1].data;
        heap[0].priority = heap[used-1].priority;
        used--;

        //Helper indexes
        size_type index_parent = 0,
                  index_child = 0;

        //If still more than one item
        if(used > 1){
            //While not a leaf, and priority is smaller than a child, swap with
            //the biggest child
            while(!is_leaf(index_parent) && heap[index_parent].priority
                                       <= big_child_priority(index_parent)){
                index_child = big_child_index(index_parent);
                swap_with_parent(big_child_index(index_parent));
                index_parent = index_child;
            }
        }
   }

   // CONSTANT MEMBER FUNCTIONS

   p_queue::size_type p_queue::size() const
   {
      return used;
   }

   bool p_queue::empty() const
   {
      return (used == 0);
   }

   p_queue::value_type p_queue::front() const
   {
      assert(size() > 0);
      return heap[0].data;
   }

   // PRIVATE HELPER FUNCTIONS
   void p_queue::resize(size_type new_capacity)
   // Pre:  (none)
   // Post: The size of the dynamic array pointed to by heap (thus
   //       the capacity of the p_queue) has been resized up or down
   //       to new_capacity, but never less than used (to prevent
   //       loss of existing data).
   //       NOTE: All existing items in the p_queue are preserved and
   //             used remains unchanged.
   {
       //Ensure valid value
       if (new_capacity < used){
            new_capacity = used;
       }
        //Temporary heap
         ItemType* newHeap = new ItemType[new_capacity];

      // Deep copy items
      for(size_type i = 0; i < used; ++i){
         newHeap[i] = heap[i];
      }
      //Replace old heap with temp heap
      delete [] heap;
      heap = newHeap;
      capacity = new_capacity;

   }

   bool p_queue::is_leaf(size_type i) const
   // Pre:  (i < used)
   // Post: If the item at heap[i] has no children, true has been
   //       returned, otherwise false has been returned.
   {
       assert(i <used);
       return (i > ((used/2)-1));
   }

   p_queue::size_type
   p_queue::parent_index(size_type i) const
   // Pre:  (i > 0) && (i < used)
   // Post: The index of "the parent of the item at heap[i]" has
   //       been returned.
   {
       assert ((i > 0) && (i < used));
       return (i-1)/2;
   }

   p_queue::size_type
   p_queue::parent_priority(size_type i) const
   // Pre:  (i > 0) && (i < used)
   // Post: The priority of "the parent of the item at heap[i]" has
   //       been returned.
   {
       assert ((i > 0) && (i < used));
       return heap[parent_index(i)].priority;
   }

   p_queue::size_type
   p_queue::big_child_index(size_type i) const
   // Pre:  is_leaf(i) returns false
   // Post: The index of "the bigger child of the item at heap[i]"
   //       has been returned.
   //       (The bigger child is the one whose priority is no smaller
   //       than that of the other child, if there is one.)
   {
       assert (!is_leaf(i));
       //Holder for biggest child
       size_type big = 0;
       //Indexes of potential left and right child
       size_type left = (2*i)+1;
       size_type right = (2*i)+2;

       //Check validity, if no right child biggest is the left child
       if (right >= used){
            big = left;
       }
       else{
            //Find biggest and set big to that index
            if (heap[left].priority > heap[right].priority){
                big = left;
            }
            else{
                big = right;
            }
       }
       return big;
   }

   p_queue::size_type
   p_queue::big_child_priority(size_type i) const
   // Pre:  is_leaf(i) returns false
   // Post: The priority of "the bigger child of the item at heap[i]"
   //       has been returned.
   //       (The bigger child is the one whose priority is no smaller
   //       than that of the other child, if there is one.)
   {
      return heap[big_child_index(i)].priority;
   }

   void p_queue::swap_with_parent(size_type i)
   // Pre:  (i > 0) && (i < used)
   // Post: The item at heap[i] has been swapped with its parent.
   {
        assert ((i > 0) && (i < used));
        // Find parent index
        size_type parentIndex = parent_index(i);

        // Store parent item
        ItemType temp_item = heap[parentIndex];

        // Set parent to child item
        heap[parentIndex] = heap[i];

        // Set child to parent item
        heap[i] = temp_item;

   }
}

