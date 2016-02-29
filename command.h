#ifndef command_h
#define command_h
#include <functional>
#include <typeinfo>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>

class Command
{
public:
  Command(){};

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
 private:
  void doBSS(std::ostream & file);
  void doData(std::ostream & file);
  void doMain(std::ostream & file);

  std::string m_filename;

  std::vector<std::string> m_string_vars;
  std::vector<std::string> m_int_vars;
  std::vector<std::string> m_literals;

  std::vector<std::string> m_lines;
  std::vector<std::string> m_fileContents;
};
#endif
