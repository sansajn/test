// GSlist sample
#include <stdio.h>
#include <glib.h>

void print_list(GSList * list) {
	printf("List contents: ");
	for (GSList * iter = list; iter != NULL; iter = g_slist_next(iter)) {
		printf("%d ", GPOINTER_TO_INT(iter->data));
	}
	printf("\n");
}

int main(int argc, char * argv[argc+1]) {
	// append
	GSList * list = NULL;
	list = g_slist_append(list, GINT_TO_POINTER(10));
	list = g_slist_append(list, GINT_TO_POINTER(20));
	list = g_slist_append(list, GINT_TO_POINTER(30));
	print_list(list);  //= 10 20 30

	// prepend
	list = g_slist_prepend(list, GINT_TO_POINTER(5));
	print_list(list);  //= 5 10 20 30

	// insert
	GSList * node = g_slist_find(list, GINT_TO_POINTER(20));
	if (node)
		list = g_slist_insert_before(list, node, GINT_TO_POINTER(25));
	print_list(list);  //= 5 10 25 20 30

	// find remove
	list = g_slist_remove(list, GINT_TO_POINTER(30));
	print_list(list);  //= 5 10 25 20

	g_slist_free(list);

	return 0;
}