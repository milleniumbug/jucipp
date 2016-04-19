#ifndef JUCI_MENU_H_
#define JUCI_MENU_H_

#include <string>
#include <unordered_map>
#include <gtkmm.h>
#include "config.h"

class Menu {
private:
  Menu();
public:
  static Menu &get() {
    static Menu singleton;
    return singleton;
  }
  
  void add_action(const std::string &name, std::function<void()> action);
  std::unordered_map<std::string, Glib::RefPtr<Gio::SimpleAction> > actions;
  void set_keys();
  
  void build();
  
  Glib::RefPtr<Gio::Menu> juci_menu;
  Glib::RefPtr<Gio::Menu> window_menu;
  
private:
  Glib::RefPtr<Gtk::Builder> builder;
  std::string ui_xml;
  std::shared_ptr<Config::Menu> menu_config;
};
#endif  // JUCI_MENU_H_
