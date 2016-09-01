#include <iostream>
#include <gtk/gtk.h>

using std::clog;

void startup(GtkApplication * app, gpointer user_data);
void shutdown(GtkApplication * app, gpointer user_data);
void activate(GtkApplication * app, gpointer user_data);

int main(int argc, char * argv[])
{
	GtkApplication * app = gtk_application_new(
		"sk.test.application", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "startup", G_CALLBACK(startup), NULL);
	g_signal_connect(app, "shutdown", G_CALLBACK(shutdown), NULL);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	return 0;
}

void startup(GtkApplication * app, gpointer user_data)
{
	clog << "startup()" << std::endl;
}

void shutdown(GtkApplication * app, gpointer user_data)
{
	clog << "shutdown()" << std::endl;
}

void activate(GtkApplication * app, gpointer user_data)
{
	clog << "activte()" << std::endl;
	
	GtkWidget * win = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(win), "application test");
	
	gtk_widget_show_all(win);
}
