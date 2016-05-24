#include <gtk/gtk.h>

void print_hello(GtkWidget * widget, gpointer data)
{
	g_print("Hello world\n");
}

void activate(GtkApplication * app, gpointer user_data)
{
	GtkWidget * window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Window");
	gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

	GtkWidget * button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_container_add(GTK_CONTAINER(window), button_box);
	
	GtkWidget * button = gtk_button_new_with_label("Hello world");
	g_signal_connect(button, "clicked", G_CALLBACK(print_hello), nullptr);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), window);
	gtk_container_add(GTK_CONTAINER(button_box), button);

	gtk_widget_show_all(window);
}

int main(int argc, char * argv[])
{
	GtkApplication * app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	int status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	
	return status;
}
