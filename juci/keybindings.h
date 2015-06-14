//juCi++ class that holds every keybinding.
#ifndef JUCI_KEYBINDINGS_H_
#define JUCI_KEYBINDINGS_H_

#include <iostream>
#include "gtkmm.h"
#include <unordered_map>
//#include "config.h" //TODO :: remove?

namespace Keybindings {

  class Config {
  public:
    Config(Config &original);
    Config();
    std::string& menu_xml() { return menu_xml_; }
    std::unordered_map<std::string, std::string>& key_map() { return key_map_; }
    void AppendXml(std::string &child);
    void SetMenu(std::string &menu_xml);
    void SetKeyMap(std::unordered_map<std::string, std::string> &key_map);
  private:
    std::unordered_map<std::string, std::string> key_map_;
    std::string menu_xml_;
    std::string hidden_ui_string_;
  };//Config
    
  class Model {
  public:
    Model(Keybindings::Config &config);
    virtual ~Model();
    std::string menu_ui_string() { return menu_ui_string_; }
    std::string hidden_ui_string() { return hidden_ui_string_; }
    //private:
    std::string menu_ui_string_;
    std::string hidden_ui_string_;
  }; // Model
  
  class Controller {
  public:
    explicit Controller(Keybindings::Config &config);
    virtual ~Controller();
    Glib::RefPtr<Gtk::ActionGroup> action_group_menu() {
      return action_group_menu_;
    };
    Glib::RefPtr<Gtk::UIManager> ui_manager_menu() {
      return ui_manager_menu_;
    };
    Glib::RefPtr<Gtk::ActionGroup> action_group_hidden() {
      return action_group_hidden_;
    };
    Glib::RefPtr<Gtk::UIManager> ui_manager_hidden() {
      return ui_manager_hidden_;
    };
    void BuildMenu();
    void BuildHiddenMenu();
    // protected:
    Glib::RefPtr<Gtk::UIManager> ui_manager_menu_;
    Glib::RefPtr<Gtk::ActionGroup> action_group_menu_;
    Glib::RefPtr<Gtk::UIManager> ui_manager_hidden_;
    Glib::RefPtr<Gtk::ActionGroup> action_group_hidden_;
    //  private:
    Keybindings::Config config_;
    Keybindings::Model model_;   
  };//Controller
}
#endif  // JUCI_KEYBINDINGS_H_