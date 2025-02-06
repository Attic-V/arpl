#ifndef linked_list_h
#define linked_list_h

#define dll_init(node) \
	do { \
		node->next = NULL; \
		node->previous = NULL; \
	} while (0)

#define dll_insert(prev, node) \
	do { \
		if (prev != NULL) { \
			prev->next = node; \
		} \
		node->previous = prev; \
	} while (0)

#endif
