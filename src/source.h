#pragma once
#include "source_diff.h"
#include "source_spellcheck.h"
#include "tooltips.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace Source {
  /// Workaround for buggy Gsv::LanguageManager::get_default()
  class LanguageManager {
  public:
    static Glib::RefPtr<Gsv::LanguageManager> get_default();
  };
  /// Workaround for buggy Gsv::StyleSchemeManager::get_default()
  class StyleSchemeManager {
  public:
    static Glib::RefPtr<Gsv::StyleSchemeManager> get_default();
  };

  Glib::RefPtr<Gsv::Language> guess_language(const boost::filesystem::path &file_path);

  class Offset {
  public:
    Offset() = default;
    Offset(unsigned line, unsigned index, boost::filesystem::path file_path_ = "") : line(line), index(index), file_path(std::move(file_path_)) {}
    operator bool() { return !file_path.empty(); }
    bool operator==(const Offset &o) { return (line == o.line && index == o.index); }

    unsigned line;
    unsigned index;
    boost::filesystem::path file_path;
  };

  class FixIt {
  public:
    enum class Type { INSERT, REPLACE, ERASE };

    FixIt(std::string source_, std::pair<Offset, Offset> offsets_);

    std::string string(const Glib::RefPtr<Gtk::TextBuffer> &buffer);

    Type type;
    std::string source;
    std::pair<Offset, Offset> offsets;
  };

  class View : public SpellCheckView, public DiffView {
  public:
    static std::set<View *> non_deleted_views;
    static std::set<View *> views;

    View(const boost::filesystem::path &file_path, const Glib::RefPtr<Gsv::Language> &language, bool is_generic_view = false);
    ~View() override;

    bool save() override;
    void configure() override;

    std::function<void()> non_interactive_completion;
    std::function<void(bool)> format_style;
    std::function<Offset()> get_declaration_location;
    std::function<Offset()> get_type_declaration_location;
    std::function<std::vector<Offset>()> get_implementation_locations;
    std::function<std::vector<Offset>()> get_declaration_or_implementation_locations;
    std::function<std::vector<std::pair<Offset, std::string>>()> get_usages;
    std::function<std::string()> get_method;
    std::function<std::vector<std::pair<Offset, std::string>>()> get_methods;
    std::function<std::vector<std::string>()> get_token_data;
    std::function<std::string()> get_token_spelling;
    std::function<void(const std::string &text)> rename_similar_tokens;
    std::function<void()> goto_next_diagnostic;
    std::function<std::vector<FixIt>()> get_fix_its;
    std::function<void()> toggle_comments;
    std::function<std::tuple<Source::Offset, std::string, size_t>()> get_documentation_template;
    std::function<void(int)> toggle_breakpoint;

    void hide_tooltips() override;
    void hide_dialogs() override;

    bool soft_reparse_needed = false;
    bool full_reparse_needed = false;
    virtual void soft_reparse(bool delayed = false) { soft_reparse_needed = false; }
    virtual void full_reparse() { full_reparse_needed = false; }

  protected:
    std::atomic<bool> parsed;
    Tooltips diagnostic_tooltips;
    Tooltips type_tooltips;
    sigc::connection delayed_tooltips_connection;

    Glib::RefPtr<Gtk::TextTag> similar_symbol_tag;
    sigc::connection delayed_tag_similar_symbols_connection;
    virtual void apply_similar_symbol_tag() {}
    bool similar_symbol_tag_applied = false;
    Glib::RefPtr<Gtk::TextTag> clickable_tag;
    sigc::connection delayed_tag_clickable_connection;
    virtual void apply_clickable_tag(const Gtk::TextIter &iter) {}
    bool clickable_tag_applied = false;

    virtual void show_diagnostic_tooltips(const Gdk::Rectangle &rectangle) { diagnostic_tooltips.show(rectangle); }
    void add_diagnostic_tooltip(const Gtk::TextIter &start, const Gtk::TextIter &end, bool error, std::function<void(const Glib::RefPtr<Gtk::TextBuffer> &)> &&set_buffer);
    void clear_diagnostic_tooltips();
    std::set<int> diagnostic_offsets;
    void place_cursor_at_next_diagnostic();
    virtual void show_type_tooltips(const Gdk::Rectangle &rectangle) {}
    gdouble on_motion_last_x = 0.0;
    gdouble on_motion_last_y = 0.0;

    Gtk::TextIter find_non_whitespace_code_iter_backward(Gtk::TextIter iter);
    /// If closing bracket is found, continues until the open bracket.
    /// Returns if open bracket is found that has no corresponding closing bracket.
    /// Else, return at start of line.
    Gtk::TextIter get_start_of_expression(Gtk::TextIter iter);
    bool find_open_curly_bracket_backward(Gtk::TextIter iter, Gtk::TextIter &found_iter);
    bool find_close_symbol_forward(Gtk::TextIter iter, Gtk::TextIter &found_iter, unsigned int positive_char, unsigned int negative_char);
    long symbol_count(Gtk::TextIter iter, unsigned int positive_char, unsigned int negative_char);
    bool is_templated_function(Gtk::TextIter iter, Gtk::TextIter &parenthesis_end_iter);

    bool on_key_press_event(GdkEventKey *key) override;
    bool on_key_press_event_basic(GdkEventKey *key);
    bool on_key_press_event_bracket_language(GdkEventKey *key);
    bool on_key_press_event_smart_brackets(GdkEventKey *key);
    bool on_key_press_event_smart_inserts(GdkEventKey *key);
    bool on_button_press_event(GdkEventButton *event) override;
    bool on_motion_notify_event(GdkEventMotion *motion_event) override;

    /// After autocomplete, arguments could be marked so that one can use tab to select the next argument
    bool keep_argument_marks = false;
    bool interactive_completion = true;

  private:
    void setup_signals();
    void setup_format_style(bool is_generic_view);

    Gsv::DrawSpacesFlags parse_show_whitespace_characters(const std::string &text);

    sigc::connection renderer_activate_connection;

    bool use_fixed_continuation_indenting = true;
    bool is_cpp = false;
    guint previous_non_modifier_keyval = 0;

    bool multiple_cursors_signals_set = false;
    std::vector<std::pair<Glib::RefPtr<Gtk::TextBuffer::Mark>, int>> multiple_cursors_extra_cursors;
    Glib::RefPtr<Gtk::TextBuffer::Mark> multiple_cursors_last_insert;
    int multiple_cursors_erase_backward_length;
    int multiple_cursors_erase_forward_length;
    bool on_key_press_event_multiple_cursors(GdkEventKey *key);

    Glib::RefPtr<Gtk::TextTag> link_tag; /// Used in tooltips
  };

  class GenericView : public View {
  private:
    class CompletionBuffer : public Gtk::TextBuffer {
    public:
      static Glib::RefPtr<CompletionBuffer> create() { return Glib::RefPtr<CompletionBuffer>(new CompletionBuffer()); }
    };

  public:
    GenericView(const boost::filesystem::path &file_path, const Glib::RefPtr<Gsv::Language> &language);

    void parse_language_file(Glib::RefPtr<CompletionBuffer> &completion_buffer, bool &has_context_class, const boost::property_tree::ptree &pt);
  };
} // namespace Source
