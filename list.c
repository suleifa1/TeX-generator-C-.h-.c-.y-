#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"


list_t* list_new(void* object) {
	list_t* list;
	list_node_t* node;
	if (object == NULL) return NULL;

	list = malloc(sizeof(list_t));
	
	node = list_new_node(object);
	list->first = node;
	list->last = node;
	return list;
}



list_node_t* list_new_node(void* object) {
	list_node_t* node;
	node = malloc(sizeof(list_node_t));
	node->next = NULL;
	node->value = object;
	return node;
}


bool list_add_object_back(list_t* list, void* object) {
	list_node_t* node;
	if (list == NULL) return false;
	

	node = list_new_node(object);

	if (list->last == NULL) {
		list->last = node;
		list->first = node;
		return true;
	}

	list->last->next = node;
	list->last = node;
	return true;
}


void list_free(list_t* list, void (*object_free_func)(void*)) {
	list_node_t* p;
	list_node_t* prev;

	p = list->first;
	while (p != NULL) {
		prev = p;
		p = p->next;
		object_free_func(prev->value);
		free(prev);
	}
	free(list);
}



void list_apply_foreach(list_t* list, void (*func)(void* object)) {
	list_node_t* p;
	for(p = list->first; p != NULL; p = p->next) {
		func(p->value);
	}
}


bool list_add_unique(list_t* list, void* object, bool(*equal_func)(void*, void*)) {
	list_node_t* p;
	
	if (list == NULL) return NULL;
	
	for(p = list->first; p != NULL; p = p->next) {
		bool result = equal_func(p->value, object);
		if (result) return false;
	}
	return list_add_object_back(list, object);
}
