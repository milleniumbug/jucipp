#pragma once
#include <boost/filesystem.hpp>
#include <gtkmm.h>
#include <string>
#include <vector>

class Dialog {
public:
  static std::string open_folder(const boost::filesystem::path &path);
  static std::string open_file(const boost::filesystem::path &path);
  static std::string new_file(const boost::filesystem::path &path);
  static std::string new_folder(const boost::filesystem::path &path);
  static std::string save_file_as(const boost::filesystem::path &path);

  class Message : public Gtk::Window {
  public:
    Message(const std::string &text);

  protected:
    bool on_delete_event(GdkEventAny *event) override;
  };

private:
  static std::string gtk_dialog(const boost::filesystem::path &path, const std::string &title,
                                const std::vector<std::pair<std::string, Gtk::ResponseType>> &buttons,
                                Gtk::FileChooserAction gtk_options);
};
