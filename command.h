#ifndef command_h
#define command_h
#include <functional>
#include <typeinfo>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>

struct func {
  func(std::string _name = std::string("main")) {
    m_name = _name;
  }

  std::string m_name;
  std::vector<func> m_calls;
};

struct string_var {
  ssize_t cmd_num = -1;
  std::string m_name;
};

class Command
{
public:
  Command(){};

  enum cmd_type { READ_STRING, READ_INT, PRINT, PRINT_STR, PRINT_NUM };

  void addPrintInt(char * arg);
  void addPrintString(char * arg);
  void addPrintLiteral(char * arg);
  
  void addReadInt(char * arg);
  void addReadString(char * arg);
  
  void setFilename(const std::string & _filename)
  { m_filename = _filename; }

  void writeAssembly();

  static std::string current_string;
  static Command cmd;
  static ssize_t line_num;
private:
  void doBSS(std::ostream & file);
  void doData(std::ostream & file);
  void doMain(std::ostream & file);

  std::string m_filename;

  std::vector<std::string> m_string_vars;
  std::vector<std::string> m_int_vars;
  std::vector<std::string> m_literals;

  func m_main;
  std::vector<std::string> m_print_strings;
  std::vector<std::string> m_print_ints;
  std::vector<std::string> m_lines;
  std::vector<std::string> m_fileContents;
  std::vector<cmd_type> m_execOrder;
};
#endif
