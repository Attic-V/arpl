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
		if (node != NULL) { \
			node->previous = prev; \
		} \
	} while (0)

#define dll_shove(prev, node) \
	do { \
		dll_insert(prev, node); \
		prev = node; \
	} while (0)

#define dll_wind(node) \
	for (; node != NULL && node->next != NULL; node = node->next);

#define dll_rewind(node) \
	for (; node != NULL && node->previous != NULL; node = node->previous)

#endif
