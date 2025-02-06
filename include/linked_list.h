#ifndef linked_list_h
#define linked_list_h

#define dll_init(node) \
	do { \
		node->next = NULL; \
		node->previous = NULL; \
	} while (0)

#endif
