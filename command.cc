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

void Command::addReadInt(char * arg)
{ m_int_vars.push_back(std::string(arg)); m_execOrder.push_back(cmd_type::READ_INT); }

void Command::markEndOfExpression()
{ m_evaluations.push_back(new std::stack<math_expression>(m_current_stack)); for(;!m_current_stack.empty();m_current_stack.pop()); }

void Command::addToExpressionStack(char * arg)
{
  std::string as_string(arg);
  math_expression toPush;
  bool is_integer;

  auto p = [&is_integer] (std::string arg) -> bool{
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
      int_assign push; push.m_name = var;
      ssize_t idx = m_evaluations.size();
      if (idx < 0) exitWithBug();;
      m_int_assigns.push_back(x);
      m_execOrder.push_back(cmd_type::INTGETS);
      return;
    }
  }

  std::cerr<<"Error! Variable name \""<<var<<"\" could not be resolved!\n";
  exit(3);
}

// allocate enough space for drew-sized strings.
#define MAX_STRING_SIZE 100
void Command::doBSS(std::ostream & file)
{
  file<<"\t.bss"<<std::endl<<std::endl;
  for (int x = 0; x < m_string_vars.size(); ++x) {
    file<<"\t.lcomm IS"<<x<<", "<<MAX_STRING_SIZE<<std::endl;
  }
}

void Command::doData(std::ostream & file)
{
  file<<"\t.data"<<std::endl<<std::endl;

  /** Place the string literals **/

  for (int x = 0; x < m_literals.size(); ++x) {
    file<<"S"<<x<<":\t.ascii \""<<m_literals[x]<<"\\0\""<<std::endl;
  }

  std::cout<<std::endl;

  /** Place the integers. **/

  for (int x = 0; x < m_int_vars.size(); ++x) {
    file<<"I"<<x<<":\t.word 0"<<std::endl;
  }
  file<<std::endl<<"string_fmt:\t.ascii \"%s\\0\""<<std::endl;
  file<<"num_fmt:\t.ascii \"%d\\0\""<<std::endl;
}

void Command::doMain(std::ostream & file)
{
  int y;      
  file<<"main:"<<std::endl;
  /**
   * @todo use main's function calls for subroutines.
   */

  for (int x = 0, intassdex = 0, stringdex = 0, exprdex = 0,
      intdex = 0, pintdex = 0, pstringdex = 0, litdex = 0;
      x < m_execOrder.size(); ++x) {

    if (m_execOrder[x] == cmd_type::READ_STRING) {
      file<<"\tldr %r0, =string_fmt"<<std::endl;
      file<<"\tldr %r1, =IS"<<stringdex<<std::endl;
      file<<"\tbl scanf"<<std::endl;
      file<<std::endl;

      ++stringdex;
    } else if (m_execOrder[x] == cmd_type::READ_INT) {
      file<<"\tldr %r0, =num_fmt"<<std::endl;
      file<<"\tldr %r1, =I"<<intdex<<std::endl;
      file<<"\tbl scanf"<<std::endl;
      file<<std::endl;

      ++intdex;
    } else if (m_execOrder[x] == cmd_type::PRINT) {
      file<<"\tldr %r0, =S"<<litdex++<<std::endl;
      file<<"\tbl printf"<<std::endl<<std::endl;
    } else if (m_execOrder[x] == cmd_type::PRINT_STR) {
      file<<"\tldr %r0, =string_fmt"<<std::endl;
      for (y = 0; y < m_string_vars.size(); ++y) {
        if (m_string_vars[y] == m_print_strings[pstringdex]) break;
      } if (y == m_string_vars.size()) {
        std::cerr<<"Error: variable "<<m_string_vars[y]<<" was not declared!"<<std::endl;
        exit(3);
      }
      /** 'y' am I a dumbass? **/
      file<<"\tldr %r1, =IS"<<y<<std::endl;
      file<<"\tbl printf"<<std::endl;
      ++pstringdex;
    } else if (m_execOrder[x] == cmd_type::PRINT_NUM) {
      file<<"\tldr %r0, =num_fmt"<<std::endl;
      for (y = 0; y < m_int_vars.size(); ++y) {
        if (m_int_vars[y] == m_print_ints[pintdex]) break;
      } if (y == m_int_vars.size()) {
        std::cerr<<"Error: variable "<<m_int_vars[y]<<" was not declared!"<<std::endl;
        exit(3);
      }
      file<<"\tldr %r1, =I"<<y<<std::endl;
      file<<"\tldr %r1, [%r1]"<<std::endl;
      file<<"\tbl printf"<<std::endl;
      ++pintdex;
    } else if (m_execOrder[x] == cmd_type::INTGETS) {
      size_t int_gets = m_int_assigns[intassdex++];
            if (y == m_int_vars.size()) {
        std::cerr<<"Error: variable "<<m_int_vars[y];
        std::cerr<<" was not declared!"<<std::endl;
        exit(3);
      }

      /** Begin Stack Evaluation **/
      std::stack<math_expression> * expr = m_evaluations[exprdex++];

      ssize_t stack_depth = 0;

      std::stack<math_expression> eval;// = *expr;
      for(; !expr->empty(); eval.push(expr->top()), expr->pop(), 1);
      std::stack<math_expression> curr;
          
      for (; eval.size();) {
        math_expression a = eval.top(); eval.pop();
	Command::exp_type type = static_cast<Command::exp_type>(a.expr_type);
	exp_type aExpType = static_cast<Command::exp_type>(a.expr_type);
        
        if (aExpType == VAR)  {
          for (y = 0; y < m_int_vars.size(); ++y) {
            if (m_int_vars[y] == a.pirate_name) break;
          } if (y == m_int_vars.size()) {
            std::cerr<<"Error: variable "<<a.pirate_name<<" was not declared!"<<std::endl;
            exit(3);
          }
	  file<<"\tldr %r1, =I"<<y<<std::endl;
	  file<<"\tldr %r1, [%r1]"<<std::endl;
          file<<"\tstr %r1, [%sp, #-4]"<<std::endl;
	  file<<"\tsub %sp, %sp, $4"<<std::endl;
	  stack_depth += 4; continue;
        } else if (aExpType == AN_INT) {
          file<<"\tmov %r1, $"<<a.int_arg<<std::endl;
	  file<<"\tstr %r1, [%sp, #-4]"<<std::endl;
	  file<<"\tsub %sp, %sp, $4"<<std::endl;
	  stack_depth += 4;
	  continue;
        }

	if (stack_depth >= 8) {
          file<<std::endl<<"\tldr %r1, [%sp]"<<std::endl;
	  file<<"\tldr %r2, [%sp, #4]"<<std::endl;
	  file<<"\tadd %sp, %sp, $8"<<std::endl;
	  stack_depth -= 8;
	} else continue;
        switch (type) {
          case ADD:
            /**
             * Oh, how I want to use a comma expression...
             * You know, the stack_push((b=stack_pop(),a=stack_pop(),a+b))
             * kind of comma expresion.
             */
            file<<"\tadd %r0, %r1, %r2"<<std::endl;
            goto do_default;
          case SUB:
            file<<"\tsub %r0, %r1, %r2"<<std::endl;
            goto do_default;
          case MUL:
            file<<"\tmul %r0, %r1, %r2"<<std::endl;
            goto do_default;
          case DIV:
            file<<"\tdiv %r0, %r1, %r2"<<std::endl;
            goto do_default;
          default:
          do_default:
            file<<"\tstr %r0, [%sp, #-4]"<<std::endl;
	    file<<"\tsub %sp, %sp, $4"<<std::endl;
	    stack_depth += 4;
        }
      }

      file<<"\tldr %r0, [%sp, #0]"<<std::endl;
      file<<"\tadd %sp, %sp, $0"<<std::endl;
      stack_depth -= 4;
      file<<"\tldr %r3, =I"<<int_gets<<std::endl;
      file<<"\tstr %r0, [%r3]"<<std::endl;
      if (stack_depth) file<<"\tadd %sp, $"<<stack_depth<<"\t@ Destroy stack"<<std::endl;
      else file<<"\t@ Yayy! Stack has nothing left."<<std::endl<<"\t@ You should be happy. I am."<<std::endl;
      file<<std::endl;
    }
  }
}

void Command::writeAssembly()
{
  std::ofstream file;
  file.open(m_filename);
  if (!file.is_open()) {
    std::cerr<<"Error: unable to open output file!";
    exit(2);
  }
  file <<"/**"<<std::endl;
  file <<" * "<<m_filename<<std::endl;
  file <<" * "<<std::endl;
  file <<" * Generated by Bench Cookie Compiler!"<<std::endl;
  file <<" * Bench Cookie is... Experimental. Don't be mad."<<std::endl;
  file <<" */"<<std::endl;

  doBSS(file);

  file<<std::endl;

  doData(file);

  file<<std::endl<<std::endl;
  file<<"\t.text"<<std::endl;
  file<<"\t.global main"<<std::endl;
  file<<"\t.global printf"<<std::endl;

  doMain(file);

  file<<std::endl;
  file<<"\tmov %r0, $0"<<std::endl;
  /**
   * @DrewBarthel: printf is a toilet.
   */
  file<<"\tbl fflush"<<std::endl;
  file<<"\tmov %r7, $1"<<std::endl;

  /** Toilet has been flushed **/

  file<<"\tswi $0"<<std::endl;
  file<<"\t.end"<<std::endl;
  file.close();
}

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
    exec_args[1] = argv[1];
    exec_args[2] = "-o";
    exec_args[3] = output;
    exec_args[4] = NULL;

    //execute the compile command.
    execvp(exec_args[0],(char* const*) exec_args);
    free(output);
  } else if (ret < 0) {
    std::cerr<<"WARNING: Failed to fork process for GCC!"<<std::endl;
  } else {
    return 0;
  }
}
