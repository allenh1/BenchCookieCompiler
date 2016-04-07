/**
 * Benchcookie Compiler
 * 
 * This file sets up the compiler to generate the machine code.
 * The contents of this file are architecture independent.
 */
#include "command.h"

/**
 * @brief      Add a command to print a number.
 * 
 * This function adds the requested variable name to the vector holding
 * all printed integers and informs the execution order vector that it
 * should print this integer.
 *
 * @param      _int  Variable name to be printed.
 */
void Command::addPrintInt(char * _int)
{
  m_print_ints.push_back(std::string(_int));
  m_execOrder.push_back(cmd_type::PRINT_NUM);
}

/**
 * @brief      Add a string to be printed.
 *
 * @param      arg   Variable name to be printed.
 * @see        addPrintInt
 */
void Command::addPrintString(char * arg)
{
  m_print_strings.push_back(std::string(arg));
  m_execOrder.push_back(cmd_type::PRINT_STR);
}

/**
 * @brief      Add a literal to be printed.
 *
 * @param      arg   Variable name to be printed.
 */
void Command::addPrintLiteral(char * arg)
{
  m_literals.push_back(std::string(arg));
  m_execOrder.push_back(cmd_type::PRINT);
}

/**
 * @brief      Add a literal to be printed.
 *
 * This differs from addPrintLiteral in that this literal
 * is not to be printed. This adds a literal for expressions,
 * such as x <-- op = "+" ?.
 * 
 * @param      arg   String to be printed
 */
void Command::addLiteral(char * arg)
{
  m_literals.push_back(std::string(arg));
  m_execOrder.push_back(cmd_type::NOPRINT);
}

/**
 * @brief      Add a command to read an integer.
 *
 * @param      arg   Variable to store the read integer.
 */
void Command::addReadInt(char * arg)
{
  m_int_vars.push_back(std::string(arg));
  m_execOrder.push_back(cmd_type::READ_INT);
}

/**
 * @brief      Add a read-line command.
 *
 * @param      arg   Variable to store the read line.
 */
void Command::addReadLine(char * arg)
{
  m_string_vars.push_back(std::string(arg));
  m_execOrder.push_back(cmd_type::READ_LINE);
}

/**
 * @brief      Adds a variable to a function's argument list.
 * 
 * This is currently just used for C Functions.
 *
 * @param      type  typename
 * @param      name  name of the parameter.
 */
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

/**
 * @brief      Mark the end of an expression.
 * 
 * This function marks the end of the expression and clears the
 * currentExpression stack. We clear this because it will be
 * used in setting up the next expression evaluation. Before
 * clearing, the expression is added to the list of all evals.
 */
void Command::markEndOfExpression()
{
  m_evaluations.push_back(new std::stack<math_expression>(m_current_stack));
  for(;!m_current_stack.empty();m_current_stack.pop());
}

/**
 * @brief      Declare an integer variable.
 *
 * @param      arg   Name of the variable.
 */
void Command::declInt(char * arg)
{
  m_int_declarations.push_back(std::string(arg));
  m_int_vars.push_back(std::string(arg));
  m_execOrder.push_back(cmd_type::DECL_INT);
}

/**
 * @brief      Declare a string variable.
 *
 * @param      arg   Name of the variable.
 */
void Command::declString(char * arg)
{
  m_string_declarations.push_back(std::string(arg));
  m_string_vars.push_back(std::string(arg));
  m_execOrder.push_back(cmd_type::DECL_STRING);
}

/**
 * @brief      Declare a pointer.
 * 
 * The pointer will be pointed to cod and stored
 * in the variable dom.
 *
 * @param      dom   Variable holding the pointer.
 * @param      cod   Variable to which we are pointing.
 */
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

/**
 * @brief      Start a function's body.
 *
 * @todo This is still to be implemented.
 * 
 * @param      arg   Function name.
 */
void Command::startFunctionBody(char * arg)
{

}

/**
 * @brief      Mark the beginning of an if block.
 */
void Command::startIfBlock()
{
  m_execOrder.push_back(cmd_type::BEGIN_IF);
}

/**
 * @brief      Mark the end of an if block.
 */
void Command::endIfBlock() { m_execOrder.push_back(cmd_type::END_IF); }

/**
 * @brief      Mark the beginning of a for loop.
 *
 * @param      arg   Variable upon which this loop depends.
 */
void Command::markForLoop(char * arg)
{
  m_for_deps.push_back(std::string(arg));
  m_execOrder.push_back(cmd_type::BEGIN_FOR);
}

/**
 * @brief      Mark the end of a for loop.
 */
void Command::markEndFor() { m_execOrder.push_back(cmd_type::END_FOR); }

/**
 * @brief      Mark the end of a function.
 */
void Command::markEndOfFunction() { m_execOrder.push_back(cmd_type::ENDFUNC); }

/**
 * @brief      Add a variable to the current function's return list.
 *
 * @param      arg   The variable to return.
 */
void Command::addToReturnList(char * arg)
{
  m_current_returns.push_back(arg);
}

/**
 * @brief      Set the current function's name.
 *
 * @param      arg   Name of the function.
 */
void Command::setFunctionName(char * arg)
{ m_function_name = std::string(arg); }

/**
 * @brief      Add to expression stack.
 *
 * @param      arg   Component to add to expression.
 */
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

/**
 * @brief      Add a read string command.
 *
 * @param      arg   The variable in which to store the string.
 */
void Command::addReadString(char * arg)
{
  m_string_vars.push_back(std::string(arg));
  m_execOrder.push_back(cmd_type::READ_STRING);
}

/**
 * @brief      Add int assignment command.
 *
 * The command evaluates the top of the expression stack,
 * then puts the result in the variable varname.
 * 
 * @param      varname  Name of the variable.
 */
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
const char * architecture = "ARM";
#elif  __X86__
#include "x86_compiler.cpp"
const char * architecture = "x86";
#else
#include "x86_64_compiler.cpp"
const char * architecture = "x86_64";
#endif

const char * version = "0.0.1 pre-Alpha";

int yyparse(void);

Command Command::cmd;
ssize_t Command::line_num = 0;

int main(int argc, char ** argv)
{
  if (argc != 2) {
    std::cerr<<"Invalid arguments!"<<std::endl;
    std::cerr<<"Usage: bcc output.s"<<std::endl;
    return 1;
  } if (std::string(argv[1]) == "-v" || std::string(argv[1]) == "--version") {
    std::cout<<"Bench Cookie Compiler, version "<<version<<"."<<std::endl;
    std::cout<<"Compiled for "<<architecture<<"."<<std::endl;
    return 0;
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
