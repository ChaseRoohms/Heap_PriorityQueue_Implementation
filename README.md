# Heap_PriorityQueue_Implementation
Header and implementation files for implementing a Priority Queue using a C++ Heap. Header file was given by professor, implementation was to be filled out by the student.
These files come from an assignment in my Data Structures and Algorithms class, taught by Dr. Lee S. Koh, at Texas State University.

These files show not only my understanding of one of the building block structures in C++, but also my ability to code and comment in a neat and concise fashion.


Implements various heap functions for testing and utilization of the priority queue, such as:

print_tree - Accessor
Displays the heap in tree form for debugging

print_array - Accessor
Displays the heap in array form for debugging

push - Mutator
Adds a new item onto the heap, maintaining both completeness, and priority order

pop - Mutator
Removes the highest priority item from the heap, maintaining both completeness, and priority order

front - Accessor
Returns the highest priority item in the heap

is_leaf - Accessor
Returns a boolean value, true if the given index is a leaf, false if not

parent_index - Accessor
Returns the index of the parent of the given index

parent_priority - Accessor
Returns the priority of the parent of the given index

big_child_index - Accessor
Returns the index of the largest child of the given index

big_child_priority - Accessor
Returns the priority of the largest child of the given index

swap_with_parent - Mutator
Swaps the given index with its parent


Also includes:
Constructors with default values in case of non valid user values
Asignment operator overloading
Dynamic memory through the use of a resize function
Size and empty functions for use in other functions

