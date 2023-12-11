#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

/**
 * Struct list_node_t
 *
 * This struct represents a node in a linked list. It is designed to store a pointer to the next node in the list, creating a chain of nodes,
 * and to hold a value, which is a pointer to the object that the node is intended to contain. 
 *
 * @version 2.2.2 
 * @author Faiz Suleimanov
 */
typedef struct list_node_t {
	struct list_node_t* next;
	void* value;
} list_node_t;

/**
 * @brief Struct list_t
 *
 * @details This struct is used to manage a linked list by maintaining pointers to the first and last nodes, which represent the head and tail of the list, respectively.
 * By tracking these two nodes, the list can easily perform operations at both ends, such as adding or removing elements. 
 *
 * @version 2.2.2 The version number of the struct, suggesting that it has been updated or refined from previous versions.
 * @author Faiz Suleimanov
 */
typedef struct list_t {
	list_node_t *first;
	list_node_t *last;
} list_t;


/**
 * Create new list
 *
 * This function initializes a list by creating the first node with the provided object and setting it as both the head and tail of the list.
 * If the object is NULL, the function returns NULL, indicating that the list cannot be created without an initial object.
 *
 * @param void* object The first object to be stored in the list. If this is NULL, the list is not created.
 * @return list_t* A pointer to the newly created list structure, or NULL if creation was not possible.
 * @author Faiz Suleimanov
 * @version 1.0.0 
 */
list_t* list_new(void* object);

/**
 * Add an object to the list
 *
 * This function appends a new node, containing the specified object, to the end of the list. It ensures the list is not null before proceeding.
 * If the list is empty (the last node is null), the new node becomes both the first and last node of the list. Otherwise, it is linked to the
 * current last node and becomes the new last node. The operation returns a boolean indicating the success of the operation.
 *
 * @param list_t* list The list to which the object is to be added. This should not be NULL.
 * @param void* object The object to add to the list, encapsulated within a new list node.
 * @return bool The result of the operation: true (1) if the object was added successfully, or false (0) otherwise.
 * @version 1.0.0 
 * @author Faiz Suleimanov
 */
bool list_add_object_back(list_t* list, void* object);

/**
 * List's free function
 *
 * This function is responsible for deallocating the entire list. It traverses the list, using a provided function pointer to free the objects contained
 * in each node before freeing the nodes themselves. After all nodes have been processed and freed, the list structure itself is also freed.
 *
 * @param list_t* list The list to be freed. This should be a pointer to a list structure.
 * @param void (*object_free_func)(void*) A function pointer to a function that knows how to properly free the objects stored within the list nodes.
 * @version 1.0.0 
 * @author Faiz Suleimanov
 */
void list_free(list_t* list, void (*object_free_func)(void*));

/**
 * List's apply foreach function
 *
 * This function iterates over each element in the list and applies a given function to the object contained in each node. 
 * It is a means of abstracting an operation and applying it to the contents of a list without exposing the internal structure of the list nodes.
 *
 * @param list_t* list The list whose elements are to be processed. This parameter should be a pointer to an initialized list.
 * @param void(*func)(void* object) A function pointer to the function that will be applied to each object in the list.
 * @version 1.0.0 
 * @author Faiz Suleimanov
 */
void list_apply_foreach(list_t* list, void (*func)(void* object));

/**
 * List's add unique object function
 *
 * This function attempts to add an object to the list only if it is not already contained within the list. It utilizes an equivalence 
 * function to test whether the object is unique with respect to the objects already in the list. The function iterates through the list,
 * and uses the equivalence function to compare the new object with each existing object. If an equivalent object is found, the function
 * returns false and does not add the new object. If no equivalent object is found, the new object is added to the end of the list.
 *
 * @param list_t* list The list to which the object may be added. This should be a pointer to an initialized list structure.
 * @param void* object The object to potentially add to the list. Its uniqueness is determined by the equal_func.
 * @param bool (*equal_func)(void*, void*) A function pointer to the function that compares two objects for equivalence.
 * @return bool The result of the operation: true (1) if the object was added as unique, or false (0) if it was not added because an equivalent object is already present.
 * @version 1.0.0 
 * @author Faiz Suleimanov
 */
bool list_add_unique(list_t* list, void* object, bool (*equal_func)(void* o1, void* o2));

/**
 * Create new node for the list
 *
 * This function allocates memory for a new list node and initializes it with the provided object. The newly created node's 'next' pointer is set to NULL,
 * indicating that it is the last node in the sequence until linked with others. 
 *
 * @param void* object The object to be encapsulated within the list node. This could be any type of data since the pointer is to type 'void'.
 * @return list_node_t* A pointer to the newly allocated and initialized list node.
 * @version 1.0.0 
 * @author Faiz Suleimanov
 */
list_node_t* list_new_node(void* object);

#endif
