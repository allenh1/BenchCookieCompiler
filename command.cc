/**
 * Benchcookie compiler
 *
 * Ehhhh... So dis works.
 */

#include "command.h"

void Command::addPrintInt(char * _int)
{ m_print_ints.push_back(std::string(_int)); m_execOrder.push_back(cmd_type::PRINT_NUM); }

void Command::addPrintString(char * arg)
{ m_print_strings.push_back(std::string(arg)); m_execOrder.push_back(cmd_type::PRINT_STR); }

void Command::addPrintLiteral(char * arg)
{      m_literals.push_back(std::string(arg)); m_execOrder.push_back(cmd_type::PRINT); }

void Command::addLiteral(char * arg)
{ m_literals.push_back(std::string(arg)); m_execOrder.push_back(cmd_type::NOPRINT); }

void Command::addReadInt(char * arg)
{ m_int_vars.push_back(std::string(arg)); m_execOrder.push_back(cmd_type::READ_INT); }

void Command::addReadLine(char * arg)
{ m_string_vars.push_back(std::string(arg)); m_execOrder.push_back(cmd_type::READ_LINE); }
void Command::addToArgList(char * type, char * name)
{
  std::string stype = std::string(type);
  std::string sname = std::string(name);

  if (stype == std::string("int")) {
    m_int_vars.push_back(sname);
  } else if (stype == std::string("pchar")) {
    m_char_pointers.push_back(sname);
  } else if (stype == std::string("pint")) {
    m_int_pointers.push_back(sname);
  }
}

void Command::markEndOfExpression()
{
  m_evaluations.push_back(new std::stack<math_expression>(m_current_stack));
  for(;!m_current_stack.empty();m_current_stack.pop());
}

void Command::declInt(char * arg)
{
  m_int_declarations.push_back(std::string(arg));
  m_int_vars.push_back(std::string(arg));
  m_execOrder.push_back(cmd_type::DECL_INT);
}

void Command::declString(char * arg)
{
  m_string_declarations.push_back(std::string(arg));
  m_string_vars.push_back(std::string(arg));
  m_execOrder.push_back(cmd_type::DECL_STRING);
}

void Command::declPointer(char * dom, char * cod)
{
  std::string point_to = std::string(cod);
  int x;

  for (x = 0; x < m_int_vars.size(); ++x) {
    if (m_int_vars[x] == point_to) break;
  } if (x != m_int_vars.size()) {
    m_int_pointers.push_back(std::string(dom));
    pointer_assignment _push;
    _push.vardex   = x;
    _push.pointdex = m_int_pointers.size() - 1;
    _push.var_type = var_type::HINT;
    m_pointer_assigns.push_back(_push);
    m_execOrder.push_back(cmd_type::PTRGETS);
    return;
  }

  for (x = 0; x < m_string_vars.size(); ++x) {
    if (m_string_vars[x] == point_to) break;
  } if (x != m_string_vars.size()) {
    m_char_pointers.push_back(std::string(dom));
    pointer_assignment _push;
    _push.vardex   = x;
    _push.pointdex = m_char_pointers.size() - 1;
    _push.var_type = var_type::HCHAR;
    m_pointer_assigns.push_back(_push);
    m_execOrder.push_back(cmd_type::PTRGETS);
    return;
  }

  std::cerr<<"Error! Could not resolve pointer target \""<<point_to;
  std::cerr<<"\" in pointer assignment."<<std::endl;
  exit(7);
}

void Command::startFunctionBody(char * arg)
{

}

void Command::startIfBlock()
{
  m_execOrder.push_back(cmd_type::BEGIN_IF);
}

void Command::endIfBlock() { m_execOrder.push_back(cmd_type::END_IF); }

void Command::markForLoop(char * arg)
{
  m_for_deps.push_back(std::string(arg));
  m_execOrder.push_back(cmd_type::BEGIN_FOR);
}

void Command::markEndFor() { m_execOrder.push_back(cmd_type::END_FOR); }

void Command::markEndOfFunction() { m_execOrder.push_back(cmd_type::ENDFUNC); }

void Command::addToReturnList(char * arg)
{
  m_current_returns.push_back(arg);
}

void Command::setFunctionName(char * arg)
{ m_function_name = std::string(arg); }

void Command::addToExpressionStack(char * arg)
{
  std::string as_string(arg);
  math_expression toPush;
  bool is_integer;

  auto p = [&is_integer] (std::string arg) -> bool {
    return !arg.empty() && std::find_if(arg.begin(),
        arg.end(), [](char c) { return !std::isdigit(c); }) == arg.end();
  };

  if (as_string == "+") {
    toPush.expr_type = exp_type::ADD;
  } else if (as_string == "-") {
    toPush.expr_type = exp_type::SUB;
  } else if (as_string == "*") {
    toPush.expr_type = exp_type::MUL;
  } else if (as_string == "/") {
    toPush.expr_type = exp_type::DIV;
  } else if (as_string == "%") {
    toPush.expr_type = exp_type::MOD;
  } else if (as_string == "==") {
    toPush.expr_type = exp_type::EQ;
  } else if (as_string == "!=") {
    toPush.expr_type = exp_type::NEQ;
  } else if (as_string == "<=") {
    toPush.expr_type = exp_type::LEQ;
  } else if (as_string == ">=") {
    toPush.expr_type = exp_type::GEQ;
  } else if (as_string == ">") {
    toPush.expr_type = exp_type::GT;
  } else if (as_string == "<") {
    toPush.expr_type = exp_type::LT;
  } else if (as_string == "!") {
    toPush.expr_type = exp_type::LOGNOT;
  } else if (as_string == "&&") {
    toPush.expr_type = exp_type::LOGAND;
  } else if (as_string == "&") {
    toPush.expr_type = exp_type::BITAND;
  } else if (as_string == "|") {
    toPush.expr_type = exp_type::BITOR;
  } else if (as_string == "^^") {
    toPush.expr_type = exp_type::LOGXOR;
  } else if (as_string == "^") {
    toPush.expr_type = exp_type::BITXOR;
  } else if (as_string == "||") {
    toPush.expr_type = exp_type::LOGOR;
  } else if (as_string == "deref") {
    toPush.expr_type = exp_type::PTRDEREF;
  } else if (as_string == "literal") {
    toPush.expr_type = exp_type::LITERAL;
    m_exp_literals.push(m_literals.size() - 1);
  } else if (p(as_string)) {
    toPush.expr_type = exp_type::AN_INT;
    toPush.int_arg  = std::stoi(as_string);
  } else {
    toPush.expr_type   = exp_type::VAR;
    toPush.pirate_name = as_string;
  }

  m_current_stack.push(toPush);
}

void Command::addReadString(char * arg)
{
  m_string_vars.push_back(std::string(arg));
  m_execOrder.push_back(cmd_type::READ_STRING);
}

void Command::addIntAssignment(char * varname) {
  std::string var(varname);
  // If we have made it to this point, then yacc has
  // already done the expression. So, we just mark the top.
  for (int x = 0; x < m_int_vars.size(); ++x) {
    if (m_int_vars[x] == var) {
      m_int_assigns.push_back(var);
      m_execOrder.push_back(cmd_type::INTGETS);
      return;
    }
  }

  for (int x = 0; x < m_string_vars.size(); ++x) {
    if (m_string_vars[x] == var) {
      m_string_assigns.push_back(var);
      m_execOrder.push_back(cmd_type::STRINGGETS);
      return;
    }
  }

  std::cerr<<"Error! Variable name \""<<var<<"\" could not be resolved!\n";
  exit(3);
}

#ifdef __arm__
#include "arm_compiler.cpp"
#elif  __X86__
#include "x86_compiler.cpp"
#else
#include "x86_64_compiler.cpp"
#endif

int yyparse(void);

Command Command::cmd;
ssize_t Command::line_num = 0;

int main(int argc, char ** argv)
{
  if (argc != 2) {
    std::cerr<<"Invalid arguments!"<<std::endl;
    std::cerr<<"Usage: bcc output.s"<<std::endl;
    return 1;
  }
  // get the filename
  Command::cmd.setFilename(std::string(argv[1]));

  //parse the code
  yyparse();

  Command::cmd.writeAssembly();

  // fork you
  int ret = fork();

  if (ret == 0) {
    const char *exec_args[5];

    char * output = strndup(argv[1], strstr(argv[1], ".") - argv[1]);

  
    exec_args[0] = "gcc";
  
    if (Command::cmd.isCCallable()) {
      // Create a c-callable function.
      std::string out2 = std::string(output);
      out2 += std::string(".o");
      free(output); // output was malloc'd
      output = strdup(out2.c_str());
      exec_args[1] = "-c";
      exec_args[2] = argv[1];
      exec_args[3] = "-o";
      exec_args[4] = output;
      exec_args[5] = NULL;
    } else {
      exec_args[1] = argv[1];
      exec_args[2] = "-o";
      exec_args[3] = output;
      exec_args[4] = NULL;
    }
    //execute the compile command.
    execvp(exec_args[0], (char * const *) (exec_args));
    free(output); // output was malloc'd
  } else if (ret < 0) {
    std::cerr<<"WARNING: Failed to fork process for GCC!"<<std::endl;
  } else {
    return 0;
  }
}
