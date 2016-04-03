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
#include <queue>
#include <stack>
// For the forks
#include <unistd.h>
#include <string.h>

const char * version = "0.0.1 pre-Alpha";

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

struct string_assign {
  std::string m_name;
  std::string new_value;
};

struct math_expression {
  int int_arg;
  double double_arg;
  char char_arg;
  std::string pirate_name;
  int expr_type = -1;
};

struct pointer_assignment {
  ssize_t vardex;
  ssize_t pointdex;
  int var_type;
};

class Command
{
public:
  Command(){};

  enum cmd_type { READ_STRING, READ_INT, PRINT, PRINT_STR, PRINT_NUM, INTGETS,
		  EXPR, DECL_INT, DECL_DOUBLE, DECL_STRING, DECL_BOOL, STRINGGETS,
		  PRINT_BOOL, BEGIN_IF, END_IF, BEGIN_FOR, END_FOR, PTRGETS, ENDFUNC,
                  NOPRINT, READ_LINE };
 
  enum exp_type { ADD, SUB, MUL, DIV, MOD, AN_INT, VAR, ASIGN, RESULT, LOGOR,
		  LOGXOR, LOGAND, BITOR, BITXOR, NEQ, BITAND, EQ, GT, GEQ, LT,
		  LEQ, SHIFTR, SHIFTL, LOGNOT, COMP, UPLUS, UMINUS, PTRDEREF, LITERAL };

  enum var_type { HINT, DINT, HDOUBLE, HFLOAT, HBOOL, LSTRING, DSTRING, HCHAR, DCHAR };

  void addPrintInt(char * arg);
  void addPrintString(char * arg);
  void addPrintLiteral(char * arg);
  void addToArgList(char *, char *);
  void addLiteral(char * arg);
  void addToReturnList(char *);
  
  void addReadInt(char * arg);
  void addReadString(char * arg);
  void addReadLine(char * arg);

  void declDouble(char * arg);
  void declString(char * arg);
  void declBool(char * arg);
  void declInt(char * arg);
  void declPointer(char * dom, char * cod);
  
  void addIntAssignment(char * arg);
  void addToExpressionStack(char * arg);

  void markFunctionEnding();
  void markEndOfExpression();
  void markForLoop(char * arg);
  void markEndFor();

  void startFunctionBody(char * arg);
  void markEndOfFunction();

  void setFunctionName(char * arg);

  void setFilename(const std::string & _filename)
  { m_filename = _filename; }

  void setCFunc(const bool & _is_c_callable)
  { m_is_c_callable = _is_c_callable; }

  void writeAssembly();

  void startIfBlock();
  void endIfBlock();

  const bool & isCCallable(){ return m_is_c_callable; }
 
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
  void push_variable(std::string var_name, std::stack<unsigned short> & vartypes, std::ostream & file);
  void doData(std::ostream & file);
  void doMain(std::ostream & file);

  void evaluate_expression(std::ostream & file);

  var_type getVarType(std::string varname);
  std::string m_filename;

  std::vector<std::string> m_string_vars;
  std::vector<std::string> m_int_vars;
  std::vector<std::string> m_literals;

  func m_main;

  std::string m_function_name;

  std::vector<std::string> m_print_strings;
  std::queue<ssize_t> m_exp_literals;
  std::vector<std::string> m_print_ints;
  std::vector<std::string> m_print_bools;
  std::vector<std::string> m_lines;
  std::vector<std::string> m_int_assigns;
  std::vector<std::string> m_bool_assigns;
  std::vector<std::string> m_string_assigns;
  std::vector<pointer_assignment> m_pointer_assigns;
  std::vector<std::string> m_if_deps;
  std::vector<std::string> m_for_deps;
  std::vector<std::string> m_bool_vars;
  std::vector<std::string> m_fileContents;
  std::vector<std::string> m_int_declarations;
  std::vector<std::string> m_string_declarations;
  std::vector<std::string> m_int_pointers;
  std::vector<std::string> m_char_pointers;
  std::vector<cmd_type> m_execOrder;
  std::stack<math_expression> m_current_stack;
  std::vector<std::string> m_current_args;
  std::vector<std::string> m_current_returns;
  std::vector<std::stack<math_expression> *> m_evaluations;

  bool m_is_c_callable = false;
};
#endif
