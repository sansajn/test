#include <iostream>
#include <gtk/gtk.h>

int main(int argc, char * argv[])
{
	gtk_init(&argc, &argv);
	GtkWidget * win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win), "sample window");
	gtk_widget_show_all(win);
	gtk_main();
	return 0;
}

