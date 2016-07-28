#include <gtk/gtk.h>

void activate(GtkApplication * app, gpointer user_data)
{
	GtkWidget * win = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(win), "entry-sample");

	GtkWidget * hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_container_add(GTK_CONTAINER(win), hbox);

	GtkWidget * entry = gtk_entry_new();
	gtk_container_add(GTK_CONTAINER(hbox), entry);

	gtk_widget_show_all(win);
}

int main(int argc, char * argv[])
{
	GtkApplication * app = gtk_application_new("enty.sample", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	return 0;
}
