#ifndef JUCI_NOTEBOOK_H_
#define JUCI_NOTEBOOK_H_

#include <iostream>
#include "gtkmm.h"
#include "entry.h"
#include "source.h"
#include "directories.h"
#include <boost/algorithm/string/case_conv.hpp>
#include <type_traits>
#include <map>
#include <sigc++/sigc++.h>
#include "clangmm.h"

namespace Notebook {
  class Model {
  public:
    Model();
    std::string cc_extension_;
    std::string h_extension_;
    int scrollvalue_;
  };
  class View {
  public:
    View();
    Gtk::Paned& view() {return view_;}
    Gtk::Notebook& notebook() {return notebook_; }
  protected:
    Gtk::Paned view_;
    Gtk::Notebook notebook_;
  };
  class Controller {
  public:
    Controller(Gtk::Window* window, Keybindings::Controller& keybindings,
               Source::Config& config,
               Directories::Config& dir_cfg);
    ~Controller();
    Glib::RefPtr<Gtk::TextBuffer> Buffer(Source::Controller &source);
    Source::View& CurrentTextView();
    int CurrentPage();
    Gtk::Box& entry_view();
    Gtk::Notebook& Notebook();
    std::string CurrentPagePath();
    void OnBufferChange();
    void BufferChangeHandler(Glib::RefPtr<Gtk::TextBuffer>
                                               buffer);
    void OnCloseCurrentPage();
    std::string GetCursorWord();
    void OnEditCopy();
    void OnEditCut();
    void OnEditPaste();
    void OnEditSearch();
    void OnFileNewCCFile();
    void OnFileNewEmptyfile();
    void OnFileNewHeaderFile();
    void OnFileOpenFolder();
    bool OnSaveFile();
    bool OnSaveFile(std::string path);
    void OnDirectoryNavigation(const Gtk::TreeModel::Path& path,
                               Gtk::TreeViewColumn* column);
    void OnNewPage(std::string name);
    void OnOpenFile(std::string filename);
    void OnCreatePage();
    bool ScrollEventCallback(GdkEventScroll* scroll_event);
    void MapBuffers(std::map<std::string, std::string> *buffers);
    clang::Index* index() { return &index_; }
    int Pages();
    Directories::Controller& directories() { return directories_; }
    Gtk::Paned& view();
    bool GeneratePopup(int key);
    void Search(bool forward);
    Source::Config& source_config() { return source_config_; }
    bool OnMouseRelease(GdkEventButton* button);
    bool OnKeyRelease(GdkEventKey* key);
    std::string OnSaveFileAs();
    bool LegalExtension(std::string extension);

  protected:
    void TextViewHandlers(Gtk::TextView& textview);
    void PopupSelectHandler(Gtk::Dialog &popup,
                            Gtk::ListViewText &listview,
                            std::map<std::string, std::string>
                            *items);
  private:
    void CreateKeybindings(Keybindings::Controller& keybindings);
    void FindPopupPosition(Gtk::TextView& textview,
                           int popup_x,
                           int popup_y,
                           int &x,
                           int &y);
    void PopupSetSize(Gtk::ScrolledWindow& scroll,
                      int &current_x,
                      int &current_y);
    void AskToSaveDialog();
    Glib::RefPtr<Gtk::Builder> m_refBuilder;
    Glib::RefPtr<Gio::SimpleActionGroup> refActionGroup;
    Source::Config source_config_;
    Directories::Controller directories_;
    View view_;
    Model model_;
    bool is_new_file_;
    Entry::Controller entry_;

    std::vector<std::unique_ptr<Source::Controller> > text_vec_;
    std::vector<Gtk::ScrolledWindow*> scrolledtext_vec_;
    std::vector<Gtk::HBox*> editor_vec_;
    std::list<Gtk::TargetEntry> listTargets_;
    Gtk::TextIter search_match_end_;
    Gtk::TextIter search_match_start_;
    Glib::RefPtr<Gtk::Clipboard> refClipboard_;
    bool ispopup;
    Gtk::Dialog popup_;
    Gtk::Window* window_;
    clang::Index index_;
  };  // class controller
}  // namespace Notebook
#endif  // JUCI_NOTEBOOK_H_