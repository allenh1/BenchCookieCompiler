#ifndef command_h
#define command_h
#include <functional>
#include <algorithm>
#include <typeinfo>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <stack>
// For the forks
#include <unistd.h>
#include <string.h>

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

struct int_assign {
  std::string m_name;
  int new_value;
};

struct math_expression {
  int int_arg;
  double double_arg;
  std::string pirate_name;
  int expr_type = -1;
};

class Command
{
public:
  Command(){};

  enum cmd_type { READ_STRING, READ_INT, PRINT, PRINT_STR, PRINT_NUM, INTGETS, EXPR, DECLARE };
  enum exp_type { ADD, SUB, MUL, DIV, MOD, AN_INT, VAR, ASIGN, RESULT };

  enum var_decl { DECL_INT, DECL_DOUBLE, DECL_STRING };
  
  void addPrintInt(char * arg);
  void addPrintString(char * arg);
  void addPrintLiteral(char * arg);

  void addReadInt(char * arg);
  void addReadString(char * arg);

  void declDouble(char * arg);
  void declString(char * arg);
  void declBool(char * arg);

  void addIntAssignment(char * arg);
  void addToExpressionStack(char * arg);
  
  void markEndOfExpression();

  void setFilename(const std::string & _filename)
  { m_filename = _filename; }

  void writeAssembly();

  static std::string current_string;
  static Command cmd;
  static ssize_t line_num;

  void exitWithBug() {
    std::cerr<<"Something is very wrong..."<<std::endl;
    std::cerr<<"Please file a full bug report: go to "<<std::endl;
    std::cerr<<"\thttps:/github.com/allenh1/BenchCookieCompiler"<<std::endl;
    std::cerr<<"and file an issue request including your full source"<<std::endl;
    std::cerr<<"as well as compiler generated source, if applicable."<<std::endl;
    exit(69);
  }
  
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
  std::vector<size_t> m_int_assigns;
  std::vector<std::string> m_fileContents;
  std::vector<var_decl> m_var_declarations;
  std::vector<cmd_type> m_execOrder;
  std::stack<math_expression> m_current_stack;
  std::vector<std::stack<math_expression> *> m_evaluations;
};
#endif
