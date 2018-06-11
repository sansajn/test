// combo box sample
#include <iostream>
#include <gtkmm/window.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/liststore.h>

using std::cout;

class example_window : public Gtk::Window
{
public:
	example_window();
	virtual ~example_window() {}

private:
	void on_cell_data_extra(Gtk::TreeModel::const_iterator const & it);
	void on_combo_changed();

	struct model_columns : public Gtk::TreeModel::ColumnRecord
	{
		model_columns()
		{
			add(id);
			add(name);
			add(extra);
		}

		Gtk::TreeModelColumn<int> id;
		Gtk::TreeModelColumn<Glib::ustring> name;
		Gtk::TreeModelColumn<Glib::ustring> extra;
	};

	model_columns _columns;

	Gtk::ComboBox _combo;
	Gtk::CellRendererText _cell;
	Glib::RefPtr<Gtk::ListStore> _ref_tree_model;
};

example_window::example_window()
{
	set_title("combobox example");
	_ref_tree_model = Gtk::ListStore::create(_columns);
	_combo.set_model(_ref_tree_model);

	// fill combobox
	Gtk::TreeModel::Row row = *(_ref_tree_model->append());
	row[_columns.id] = 1;
	row[_columns.name] = "Patrik Jane";
	row[_columns.extra] = "mentalist";
	_combo.set_active(row);

	row = *(_ref_tree_model->append());
	row[_columns.id] = 2;
	row[_columns.name] = "Teresa Lisbon";
	row[_columns.extra] = "CBI agent";

	row = *(_ref_tree_model->append());
	row[_columns.id] = 3;
	row[_columns.name] = "Grace Van Pelt";
	row[_columns.extra] = "CBI agent";

	_combo.pack_start(_columns.id);
	_combo.pack_start(_columns.name);

	// an example of adding a cell renderer manually instead of using
	// pack_start() so we have more control
	_combo.set_cell_data_func(_cell, sigc::mem_fun(*this, &example_window::on_cell_data_extra));
	_combo.pack_start(_cell);

	add(_combo);

	_combo.signal_changed().connect(sigc::mem_fun(*this, &example_window::on_combo_changed));

	show_all_children();
}

void example_window::on_combo_changed()
{
	Gtk::TreeModel::iterator it = _combo.get_active();
	if (it)
	{
		Gtk::TreeModel::Row row = *it;
		if (row)
		{
			int id = row[_columns.id];
			Glib::ustring name = row[_columns.name];
			cout << "  ID=" << id << ", name=" << name << std::endl;
		}
	}
	else
		cout << "invalid iterator" << std::endl;
}

void example_window::on_cell_data_extra(Gtk::TreeModel::const_iterator const & it)
{
	Gtk::TreeModel::Row row = *it;
	Glib::ustring const extra = row[_columns.extra];
	if (extra.empty())
		_cell.property_text() = "(none)";
	else
		_cell.property_text() = "-" + extra + "-";

	_cell.property_foreground() = (extra == "mentalist" ? "red" : "green");
}


int main(int argc, char * argv[])
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "combobox.example");
	example_window w;
	return app->run(w);
}
