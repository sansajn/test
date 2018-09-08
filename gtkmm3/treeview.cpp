#include <iostream>
#include <gtkmm.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeviewcolumn.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/application.h>

using std::cout;
using Glib::RefPtr;
using Glib::ustring;

class example_window : public Gtk::Window
{
public:
	example_window();

private:
	void on_button_quit();
	void on_treeview_row_activated(Gtk::TreeModel::Path const & path, Gtk::TreeViewColumn * column);

	struct model_columns : public Gtk::TreeModel::ColumnRecord
	{
		model_columns() {add(column_id); add(column_name);}
		Gtk::TreeModelColumn<int> column_id;
		Gtk::TreeModelColumn<ustring> column_name;
	};

	model_columns _columns;
	Gtk::Box _vbox;
	Gtk::ScrolledWindow _scrolled;
	Gtk::TreeView _view;
	RefPtr<Gtk::TreeStore> _model;
	Gtk::ButtonBox _button_box;
	Gtk::Button _quit;

};

example_window::example_window()
	: _vbox{Gtk::ORIENTATION_VERTICAL}
	, _quit{"Quit"}
{
	set_title("Gtk::TreeView (TreeStore) example");
	set_border_width(5);
	set_default_size(400, 200);

	add(_vbox);

	_scrolled.add(_view);

	_vbox.pack_start(_scrolled);
	_vbox.pack_start(_button_box, Gtk::PACK_SHRINK);

	_button_box.pack_start(_quit, Gtk::PACK_SHRINK);
	_button_box.set_border_width(5);
	_button_box.set_layout(Gtk::BUTTONBOX_END);
	_quit.signal_clicked().connect(sigc::mem_fun(*this, &example_window::on_button_quit));

	_model = Gtk::TreeStore::create(_columns);
	_view.set_model(_model);
	_view.set_reorderable();

	Gtk::TreeModel::Row row = *(_model->append());
	row[_columns.column_id] = 1;
	row[_columns.column_name] = "Billy Bob";

	Gtk::TreeModel::Row child_row = *(_model->append(row.children()));
	child_row[_columns.column_id] = 11;
	child_row[_columns.column_name] = "Billy Bob Junior";

	child_row = *(_model->append(row.children()));
	child_row[_columns.column_id] = 12;
	child_row[_columns.column_name] = "Sue Bob";

	child_row = *(_model->append());
	child_row[_columns.column_id] = 2;
	child_row[_columns.column_name] = "Joey Jojo";

	child_row = *(_model->append());
	child_row[_columns.column_id] = 3;
	child_row[_columns.column_name] = "Rob McRoberts";

	child_row = *(_model->append(child_row.children()));
	child_row[_columns.column_id] = 31;
	child_row[_columns.column_name] = "Xavier McRoberts";

	_view.append_column("ID", _columns.column_id);
	_view.append_column("Name", _columns.column_name);

	_view.signal_row_activated().connect(sigc::mem_fun(*this, &example_window::on_treeview_row_activated));

	show_all_children();
}

void example_window::on_button_quit()
{
	hide();
}

void example_window::on_treeview_row_activated(Gtk::TreeModel::Path const & path, Gtk::TreeViewColumn * column)
{
	Gtk::TreeModel::iterator it = _model->get_iter(path);
	if (it)
	{
		Gtk::TreeModel::Row row = *it;
		cout << "Row activated: ID=" << row[_columns.column_id] << ", Name=" << row[_columns.column_name] << std::endl;
	}
}

int main(int argc, char * argv[])
{
	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
	example_window w;
	return app->run(w);
}
