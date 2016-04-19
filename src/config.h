#ifndef JUCI_CONFIG_H_
#define JUCI_CONFIG_H_
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
#include <unordered_map>
#include <string>
#include <utility>
#include <vector>
#include <memory>

// shared_member
//
// Returns a shared_ptr that shares ownership with a passed pointer, but
// points to a member object of the pointed object. If the passed shared_ptr is
// empty, returns nullptr. If the passed pointer to member object is null,
// the behaviour is undefined.
template<typename T, typename Member>
std::shared_ptr<Member> shared_member(std::shared_ptr<T> ptr, Member T::*member)
{
  // using std::shared_ptr's aliasing constructor
  // see https://www.justsoftwaresolutions.co.uk/cplusplus/shared-ptr-secret-constructor.html
  if(ptr)
    return std::shared_ptr<Member>(ptr, &(ptr.get()->*member));
  else
    return nullptr;
}

class Config {
public:
  class Menu {
  public:
    std::unordered_map<std::string, std::string> keys;
  };
  
  class Window {
  public:
    std::string theme_name;
    std::string theme_variant;
    std::string version;
    std::pair<int, int> default_size;
  };
  
  class Terminal {
  public:
    std::string clang_format_command;
    int history_size;
    std::string font;
    
#ifdef _WIN32
    boost::filesystem::path msys2_mingw_path;
#endif
  };
  
  class Project {
  public:
    std::string default_build_path;
    std::string debug_build_path;
    std::string cmake_command;
    std::string make_command;
    bool save_on_compile_or_run;
    bool clear_terminal_on_compile;
  };
  
  class Source {
  public:
    class DocumentationSearch {
    public:
      std::string separator;
      std::unordered_map<std::string, std::string> queries;
    };
    
    std::string style;
    std::string font;
    std::string spellcheck_language;
    
    bool cleanup_whitespace_characters;
    
    bool show_map;
    std::string map_font_size;
    
    bool auto_tab_char_and_size;
    char default_tab_char;
    unsigned default_tab_size;
    bool wrap_lines;
    bool highlight_current_line;
    bool show_line_numbers;
    std::unordered_map<std::string, std::string> clang_types;
    std::string clang_format_style;
    
    std::unordered_map<std::string, DocumentationSearch> documentation_searches;
  };
private:
  Config();
public:
  static std::shared_ptr<Config> get() {
    static auto ptr = std::shared_ptr<Config>(new Config());
    return ptr;
  }
  
  void load();
  
  Menu menu;
  Window window;
  Terminal terminal;
  Project project;
  Source source;
  
  const boost::filesystem::path& juci_home_path() const { return home; }

private:
  void find_or_create_config_files();
  void retrieve_config();
  bool add_missing_nodes(const boost::property_tree::ptree &default_cfg, std::string parent_path="");
  bool remove_deprecated_nodes(const boost::property_tree::ptree &default_cfg, boost::property_tree::ptree &config_cfg, std::string parent_path="");
  void update_config_file();
  void get_source();

  boost::property_tree::ptree cfg;
  boost::filesystem::path home;
};
#endif
