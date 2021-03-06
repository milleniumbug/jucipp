#include "tooltips.h"

Gdk::Rectangle Tooltips::drawn_tooltips_rectangle = Gdk::Rectangle();

Tooltip::Tooltip(Gtk::TextView *text_view,
                 Glib::RefPtr<Gtk::TextBuffer::Mark> start_mark, Glib::RefPtr<Gtk::TextBuffer::Mark> end_mark,
                 std::function<void(const Glib::RefPtr<Gtk::TextBuffer> &)> create_tooltip_buffer) : text_view(text_view) {}

Tooltip::~Tooltip() {}

void Tooltips::show(Gdk::Rectangle const &, bool) {}

void Tooltips::show(bool) {}

void Tooltips::hide(const std::pair<int, int> &, const std::pair<int, int> &) {}
