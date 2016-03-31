// allocate enough space for drew-sized strings.
#ifndef ARM_COMPILER_SOURCE
#define ARM_COMPILER_SOURCE
#define MAX_STRING_SIZE 100
void Command::doBSS(std::ostream & file)
{
  file<<"\t.bss"<<std::endl<<std::endl;
  for (int x = 0; x < m_string_vars.size(); ++x) {
    file<<"\t.lcomm IS"<<x<<", "<<MAX_STRING_SIZE<<std::endl;
  }

  /** Place locals' linked list **/

  file<<"\t.lcomm locals, 400";
  file<<std::endl;
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

  for (int x = 0; x < m_int_vars.size() - m_int_declarations.size(); ++x) {
    file<<"I"<<x<<":\t.word 0"<<std::endl;
  }
  
  file<<std::endl<<"string_fmt:\t.ascii \"%s\\0\""<<std::endl;
  file<<"num_fmt:\t.ascii \"%d\\0\""<<std::endl;
  file<<"TRUE_STORY:\t.ascii \"true\\0\""<<std::endl;
  file<<"FALSE:\t\t.ascii \"false\\0\""<<std::endl;
}
#define INTGR 67
#define STRNG 68

void Command::evaluate_expression(std::ostream & file)
{
  static ssize_t nszcount = 0;
  static ssize_t divcount = 0;
  static ssize_t exprdex  = 0;
  
  unsigned int t1, t2;

  /** Begin Stack Evaluation **/
  std::stack<math_expression> * expr = m_evaluations[exprdex++];
  std::stack<unsigned short> arg_types;
  ssize_t stack_depth = 0;

  std::stack<math_expression> eval;// = *expr;
  for(; !expr->empty(); eval.push(expr->top()), expr->pop(), 1);
  std::stack<math_expression> curr;
 
  for (; eval.size();) {
    math_expression a = eval.top(); eval.pop();
    Command::exp_type type = static_cast<Command::exp_type>(a.expr_type);
    exp_type aExpType = static_cast<Command::exp_type>(a.expr_type);
    if (aExpType == VAR)  {
      push_variable(a.pirate_name, arg_types, file);
      stack_depth += 4;
      continue;
    } else if (aExpType == AN_INT) {
      file<<"\tmov %r1, $"<<a.int_arg<<std::endl;
      file<<"\tpush {%r1}"<<std::endl;
      stack_depth += 4;
      arg_types.push(INTGR);
      continue;
    } else if (aExpType == LITERAL) {
      arg_types.push(STRNG);
      ssize_t idx = m_exp_literals.front();
      m_exp_literals.pop();
      file<<"\tldr %r1, =S"<<idx<<std::endl;
      file<<"\tpush {%r1}"<<std::endl;
      stack_depth += 4;
      continue;
    } else if (aExpType == PTRDEREF) {
      file<<"\tpop {%r1}"<<std::endl;
      file<<"\tldr %r1, [%r1]"<<std::endl;
      file<<"\tpush {%r1}"<<std::endl;
      continue;
    } else if (aExpType == LOGNOT) {
      file<<"\tpop %{r1}"<<std::endl;
      file<<"\tcmp %r1, $0"<<std::endl;
      file<<"\tmoveq %r0, $0"<<std::endl;
      file<<"\tmovneq %r0, $1"<<std::endl;
      file<<"\tpush {%r0}"<<std::endl;
      ++nszcount;
      continue;
    }
    
    if (stack_depth >= 8) {
      file<<"\tpop {%r1, %r2}"<<std::endl;
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
      file<<"\tsub %r0, %r2, %r1"<<std::endl;
      goto do_default;
    case MUL:
      file<<"\tmul %r0, %r1, %r2"<<std::endl;
      goto do_default;
    case DIV:
      /* @todo case divide by zero */
      // Do we need to move 0 into r0? commenting
      // out for now.
      file<<"\teor %r0, %r0, %r0"<<std::endl;
      file<<"\tmov %r5, $1"<<std::endl;
      file<<"\tmov %r3, #-1"<<std::endl;
      file<<"\tcmp %r1, $0"<<std::endl;
      file<<"\tmulle %r5, %r5, %r3"<<std::endl;
      file<<"\tmulle %r1, %r1, %r3"<<std::endl;
      file<<"\tcmp %r2, $0"<<std::endl;
      file<<"\tmulle %r5, %r3, %r5"<<std::endl;
      file<<"\tmulle %r2, %r3, %r2"<<std::endl;
      file<<"DIV"<<divcount++<<":\tcmp %r1, %r2"<<std::endl;
      file<<"\tbgt DDIV"<<divcount-1<<std::endl;
      file<<"\tsub %r2, %r2, %r1"<<std::endl;
      file<<"\tadd %r0, $1"<<std::endl;
      file<<"\tb DIV"<<divcount-1<<std::endl;
      file<<"DDIV"<<divcount-1<<":";
      file<<"\tmul %r0, %r0, %r5"<<std::endl;
      goto do_default;
    case MOD:
      file<<"\tudiv %r0, %r2, %r1"<<std::endl;
      file<<"\tmls %r2, %r1, %r0, %r2"<<std::endl;
      goto do_default;
    case GT:
      t1 = arg_types.top(); arg_types.pop();
      t2 = arg_types.top(); arg_types.pop();
      if (t1 == STRNG && t2 == STRNG) {
	file<<"\tmov %r0, %r1"<<std::endl;
	file<<"\tmov %r1, %r2"<<std::endl;
	file<<"\tbl strcmp"<<std::endl;
	file<<"\tcmp %r0, $0"<<std::endl;
	file<<"\tmovgt %r0, $1"<<std::endl;
	file<<"\tmovle %r0, $0"<<std::endl;
      } else {
	file<<"\tcmp %r2, %r1"<<std::endl;
	file<<"\tmovgt %r0, $1"<<std::endl;
	file<<"\tmovle %r0, $0"<<std::endl;
      }
      nszcount++;
      goto do_default;
    case GEQ:
      t1 = arg_types.top(); arg_types.pop();
      t2 = arg_types.top(); arg_types.pop();
      if (t1 == STRNG && t2 == STRNG) {
	file<<"\tmov %r0, %r1"<<std::endl;
	file<<"\tmov %r1, %r2"<<std::endl;
	file<<"\tbl strcmp"<<std::endl;
	file<<"\tcmp %r0, $0"<<std::endl;
	file<<"\tmovge %r0, $1"<<std::endl;
	file<<"\tmovlt %r0, $0"<<std::endl;
      } else {
	file<<"\tcmp %r2, %r1"<<std::endl;
	file<<"\tmovge %r0, $1"<<std::endl;
	file<<"\tmovle %r0, $0"<<std::endl;
      }
      nszcount++;
      goto do_default;
    case LT:
      t1 = arg_types.top(); arg_types.pop();
      t2 = arg_types.top(); arg_types.pop();
      if (t1 == STRNG && t2 == STRNG) {
	file<<"\tmov %r0, %r1"<<std::endl;
	file<<"\tmov %r1, %r2"<<std::endl;
	file<<"\tbl strcmp"<<std::endl;
	file<<"\tcmp %r0, $0"<<std::endl;
	file<<"\tmovlt %r0, $1"<<std::endl;
	file<<"\tmovge %r0, $0"<<std::endl;
      } else {
	file<<"\tcmp %r2, %r1"<<std::endl;
	file<<"\tmovlt %r0, $1"<<std::endl;
	file<<"\tmovge %r0, $0"<<std::endl;
      }
      nszcount++;
      goto do_default;
    case LEQ:
      t1 = arg_types.top(); arg_types.pop();
      t2 = arg_types.top(); arg_types.pop();
      if (t1 == STRNG && t2 == STRNG) {
	file<<"\tmov %r0, %r1"<<std::endl;
	file<<"\tmov %r1, %r2"<<std::endl;
	file<<"\tbl strcmp"<<std::endl;
	file<<"\tcmp %r0, $0"<<std::endl;
	file<<"\tmovle %r0, $1"<<std::endl;
	file<<"\tmovgt %r0, $0"<<std::endl;
      } else {
	file<<"\tcmp %r1, %r2"<<std::endl;
	file<<"\tmovle %r0, $1"<<std::endl;
	file<<"\tmovgt %r0, $0"<<std::endl;
      }
      nszcount++;
      goto do_default;
    case EQ:
      t1 = arg_types.top(); arg_types.pop();
      t2 = STRNG;// arg_types.top(); arg_types.pop();
      if (t1 == STRNG && t2 == STRNG) {
	file<<"\tmov %r0, %r1"<<std::endl;
	file<<"\tmov %r1, %r2"<<std::endl;
	file<<"\tbl strcmp"<<std::endl;
	file<<"\tcmp %r0, $0"<<std::endl;
	file<<"\tmoveq %r0, $1"<<std::endl;
	file<<"\tmovne %r0, $0"<<std::endl;
      } else {
	file<<"\tcmp %r1, %r2"<<std::endl;
	file<<"\tmoveq %r0, $1"<<std::endl;
	file<<"\tmovne %r0, $0"<<std::endl;
      }
      nszcount++;
      goto do_default;
    case NEQ:
      t1 = arg_types.top(); arg_types.pop();
      t2 = STRNG;// arg_types.top(); arg_types.pop();
      if (t1 == STRNG && t2 == STRNG) {
	file<<"\tmov %r0, %r1"<<std::endl;
	file<<"\tmov %r1, %r2"<<std::endl;
	file<<"\tbl strcmp"<<std::endl;
	file<<"\tcmp %r0, $0"<<std::endl;
	file<<"\tmovne %r0, $1"<<std::endl;
	file<<"\tmoveq %r0, $0"<<std::endl;
      } else {	
	file<<"\tcmp %r1, %r2"<<std::endl;
	file<<"\tmovne %r0, $1"<<std::endl;
	file<<"\tmoveq %r0 $0"<<std::endl;
      }
      nszcount++;
      goto do_default;
    case LOGAND:
      file<<"\tmov %r0, $1"<<std::endl;
      file<<"\tcmp %r1, $0"<<std::endl;
      file<<"\tbeq AND"<<nszcount<<std::endl;
      file<<"\tcmp %r2, $0"<<std::endl;
      file<<"\tbeq AND"<<nszcount<<std::endl;
      file<<"\tb AND"<<nszcount+1<<std::endl;
      file<<"AND"<<nszcount<<":\tmov %r0, $0"<<std::endl;
      file<<"AND"<<nszcount+1<<":"<<std::endl;
      nszcount += 2;
      goto do_default;
    case LOGOR:
      file<<"\tadd %r1, %r1, %r2"<<std::endl;
      file<<"\tmov %r0, $1"<<std::endl;
      file<<"\tcmp %r1, $0"<<std::endl;
      file<<"\tbne NSZ"<<nszcount<<std::endl;
      file<<"\tmov %r0, $0"<<std::endl;
      file<<"NSZ"<<nszcount<<": @ label used to set as true"<<std::endl;
      nszcount++;
      goto do_default;
    case LOGXOR:
      file<<"\tmul %r3, %r1, %r2"<<std::endl;
      file<<"\tadd %r4, %r1, %r2"<<std::endl;
      file<<"\tmov %r1, $1"<<std::endl;
      file<<"\tmov %r2, $1"<<std::endl;
      file<<"\tcmp %r3, $0"<<std::endl;
      file<<"\tbne NSZ"<<nszcount<<std::endl;
      file<<"\tmov %r1, $0"<<std::endl;
      file<<"NSZ"<<nszcount++<<": @ r1 <-- a && b"<<std::endl;
      file<<"\tcmp %r4, $0"<<std::endl;
      file<<"\tbne NSZ"<<nszcount<<std::endl;
      file<<"\tmov %r2, $0"<<nszcount<<std::endl;
      file<<"NSZ"<<nszcount<<": @ r1 <-- a || b"<<std::endl;
      file<<"\teor %r0, %r1, %r2"<<std::endl;
      nszcount++;
      goto do_default;
    default:
      std::cerr<<"Welp. You did it. You tried it."<<std::endl;
      std::cerr<<"This is what happens. You get mad."<<std::endl;
      std::cerr<<"Don't use things I didn't implement yet."<<std::endl;
      exit(101);
    do_default:
      file<<"\tpush {%r0}"<<std::endl;
      stack_depth += 4;
    }
  }
}

void Command::push_variable(std::string var_name, std::stack<unsigned short> & vartype,
			    std::ostream & file)
{
  std::string varname = var_name;
  for (int x = 0; x < m_int_declarations.size(); ++x) {
    if (m_int_declarations[x] == varname) {
      file<<"\tmov %r1, %r9"<<std::endl;
      file<<"\tldr %r1, [%r9, #"<<4 * x<<"]"<<std::endl;
      file<<"\tldr %r1, [%r1]"<<std::endl;
      file<<"\tpush {%r1}"<<std::endl;
      vartype.push(INTGR);
      return;
    }
  }
 
  for (int x = 0; x < m_int_vars.size(); ++x) {
    if (m_int_vars[x] == varname) {
      file<<"\tldr %r1, =I"<<x<<std::endl;
      file<<"\tldr %r1, [%r1]"<<std::endl;
      file<<"\tpush {%r1}"<<std::endl;
      vartype.push(INTGR);
      return;
    }
  }
  
  for (int x = 0; x < m_string_vars.size(); ++x) {
    if (m_string_vars[x] == varname) {
      file<<"\tldr %r1, =IS"<<x<<std::endl;
      file<<"\tpush {%r1}"<<std::endl;
      vartype.push(STRNG);
      return;
    }
  }

  std::cerr<<"Error: could not resolve variable \"";
  std::cerr<<varname<<"\"."<<std::endl;
  exit(6);
}

void Command::doMain(std::ostream & file)
{
  int y;
  const size_t local_count = 0;// Idk, yo. Just... somewhere.
  if (m_is_c_callable) {
    file<<m_function_name<<":"<<std::endl;
    file<<"\tpush {%lr}"<<std::endl;
  }
  else file<<"main:"<<std::endl;
  
  file<<"\tldr %r9, =locals"<<std::endl;
  file<<"\t@ Make space for locals"<<std::endl;
  /**
   * @todo use main's function calls for subroutines.
   */

  for (int x = 0, intassdex = 0, stringdex = 0, exprdex = 0,
	 intdex = 0, pintdex = 0, pstringdex = 0, litdex = 0,
	 pbooldex = 0, sints = 0, ifndex = 0, forndex = 0, nszcount = 0;
      x < m_execOrder.size(); ++x) {

    if (m_execOrder[x] == cmd_type::READ_STRING) {
      file<<"\tldr %r0, =string_fmt"<<std::endl;
      file<<"\tldr %r1, =IS"<<stringdex<<std::endl;
      file<<"\tbl scanf"<<std::endl;
      file<<std::endl;

      ++stringdex;
    } else if (m_execOrder[x] == cmd_type::READ_LINE) {
      /**
       * @todo This causes a warning. That's not good.
       */

      file<<"\tldr %r0, =IS"<<stringdex<<std::endl;
      file<<"\tbl gets"<<std::endl;
      file<<std::endl;
      
      ++stringdex;
    } else if (m_execOrder[x] == cmd_type::DECL_INT) {
      file<<"\tmov %r0, $4"<<std::endl;
      file<<"\tbl malloc"<<std::endl;
      file<<"\tstr %r0, [%r9, #"<<sints * 4<<"]"<<std::endl;

      file<<std::endl;
      m_int_vars.push_back(m_int_declarations[sints++]);
    } else if (m_execOrder[x] == cmd_type::BEGIN_IF) {
      // find the variable we depend on!
      evaluate_expression(file);
      file<<"\tpop {%r0}"<<std::endl;
      file<<"IF"<<ifndex<<":\tcmp %r0, $0"<<std::endl;
      file<<"\tbeq END_IF"<<ifndex<<std::endl;
      ++ifndex;
    } else if (m_execOrder[x] == cmd_type::BEGIN_FOR) {
      std::string var_name = m_for_deps[forndex];
      for (y = 0; y < m_int_vars.size(); ++y) {
	if (m_int_vars[y] == var_name) break;
      } int z = 0;
      for (; z < m_int_declarations.size(); ++z) {
	if (m_int_declarations[z] == var_name) break;
      } if (z < m_int_declarations.size()) {
	file<<"FOR"<<forndex<<":";
	file<<"\tmov %r8, %r9"<<std::endl;
	file<<"\tldr %r8, [%r8, #"<<4 * z<<"]"<<std::endl;
	file<<"\tldr %r8, [%r8]"<<std::endl;
      } else if (y < m_int_vars.size()) {
	file<<"FOR"<<forndex<<":";
	file<<"\tmov %r8, =I"<<y<<std::endl;
	file<<"\tldr %r8, [%r8]"<<std::endl;
      } else {
	std::cerr<<"Error: for-dependent variable "<<var_name;
	std::cerr<<" not yet declared!"<<std::endl;
	exit(7);
      }

      file<<"\tcmp %r8, $0"<<std::endl;
      file<<"\tbeq END_FOR"<<forndex<<std::endl;
    } else if (m_execOrder[x] == cmd_type::END_FOR) {
      file<<"b FOR"<<forndex<<std::endl;
      file<<"END_FOR"<<forndex++<<":"<<std::endl;
    } else if (m_execOrder[x] == cmd_type::END_IF) {
      file<<"END_IF"<<ifndex-1<<":"<<std::endl;
    } else if (m_execOrder[x] == cmd_type::READ_INT) {
      file<<"\tldr %r0, =num_fmt"<<std::endl;
      file<<"\tldr %r1, =I"<<intdex<<std::endl;
      file<<"\tbl scanf"<<std::endl;
      file<<std::endl;

      ++intdex;
    } else if (m_execOrder[x] == cmd_type::NOPRINT) {
      ++litdex;
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
      } int z;
      for (z = 0; z < m_int_declarations.size(); ++z) {
	if (m_print_ints[pintdex] == m_int_declarations[z]) break;
      } if (z != m_int_declarations.size()) {
	file<<"\tldr %r1, =locals"<<std::endl;
	file<<"\tldr %r1, [%r1, #"<<4 * z<<"]"<<std::endl;
	file<<"\tldr %r1, [%r1]"<<std::endl;
      } else {
	file<<"\tldr %r1, =I"<<y<<std::endl;
	file<<"\tldr %r1, [%r1]"<<std::endl;
      } file<<"\tbl printf"<<std::endl;
      ++pintdex;
    } else if (m_execOrder[x] == cmd_type::PRINT_BOOL) {
      for (y = 0; y < m_bool_vars.size(); ++y) {
	if (m_bool_vars[y] == m_print_bools[pbooldex]) break;
      } if (y == m_bool_vars.size()) {
	std::cerr<<"Error: variable "<<m_bool_vars[y]<<" was not declared!"<<std::endl;
      }
      file<<"\tldr %r1, =B"<<y<<std::endl;
      file<<"\tldr %r1, [%r1]"<<std::endl;
      file<<"\tcmp %r1, $0"<<std::endl;
      file<<"\tbne LIES"<<nszcount++<<std::endl;
      file<<"\tldr %r0, =TRUE_STORY"<<std::endl;
      file<<"\tb NOISTRUE"<<nszcount-1<<std::endl;
      file<<"LIES"<<nszcount-1<<":"<<std::endl;
      file<<"\tldr %r0, =FALSE"<<std::endl;
      file<<"NOISTRUE"<<nszcount-1<<":"<<std::endl;
      file<<"\tbl printf"<<std::endl;
      ++pbooldex;
    } else if (m_execOrder[x] == cmd_type::INTGETS) {
      std::string int_gets = m_int_assigns[intassdex];
      ssize_t offset; 
      for (y = 0; y < m_int_vars.size(); ++y) {
	if (m_int_vars[y] == m_int_assigns[intassdex]) break;
      } int z; bool on_stack = false; int * last_z = new int;
      for (z = 0; z < m_int_declarations.size(); ++z) {
	if (m_int_vars[y] == m_int_declarations[z]) { on_stack = true; break; }
      } offset = (z != m_int_declarations.size()) ? 4 * z : y;
      if (y == m_int_vars.size()) {
	std::cerr<<"Error: variable "<<m_int_vars[y];
	std::cerr<<" was not declared!"<<std::endl;
	exit(3);
      } *last_z = z;

      evaluate_expression(file);

      file<<"\tpop {%r0}"<<std::endl;
   
      if (!on_stack) {
        int x;
       for (x = 0; x < m_int_vars.size(); ++x) {
        if (m_int_vars[x] == int_gets) break;
       }
	     file<<"\tldr %r3, =I"<<x<<std::endl;
      } else {
	     file<<"\tmov %r3, %r9"<<std::endl;
	     file<<"\tldr %r3, [%r3, #"<<*last_z * 4<<"]"<<std::endl;
	     delete last_z;
      }
      file<<"\tstr %r0, [%r3]"<<std::endl;
      file<<std::endl; intassdex++;
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
  file <<" *"<<std::endl;
  file <<" * Generated by Bench Cookie Compiler!"<<std::endl;
  file <<" * Bench Cookie is... Experimental. Don't be mad."<<std::endl;
  file <<" */"<<std::endl;

  doBSS(file);

  file<<std::endl;

  doData(file);

  file<<std::endl<<std::endl;
  file<<"\t.text"<<std::endl;
  if (!m_is_c_callable) file<<"\t.global main"<<std::endl;
  file<<"\t.global printf"<<std::endl;
  file<<"\t.global scanf"<<std::endl;
  file<<"\t.global malloc"<<std::endl;

  if (m_is_c_callable) file<<"\t.global "<<m_function_name<<std::endl;
  
  doMain(file);

  /**
   * Free locals
   *
   * @todo Free locals when last used.
   *       Maybe change file to a list
   *       and search for last usage?
   */
  if (m_int_declarations.size()) file<<"\t@ Free local vars"<<std::endl;
  for (int x = 0; x < m_int_declarations.size(); ++x) {
    file<<"\tldr %r0, =locals"<<std::endl;
    file<<"\tldr %r0, [%r0, #"<<4 * x<<"]"<<std::endl;
    file<<"\tbl free"<<std::endl<<std::endl;
  }

  if (m_int_declarations.size()) file<<"\t@ locals are free"<<std::endl<<std::endl;

  file<<std::endl;
  file<<"\tmov %r0, $0"<<std::endl;
  /**
   * @DrewBarthel: printf is a toilet.
   */
  file<<"\tbl fflush"<<std::endl;

  /** Toilet has been flushed **/

  if (!m_is_c_callable) {
    file<<"\tmov %r7, $1"<<std::endl;

    file<<"\tswi $0"<<std::endl;
    file<<"\t.end"<<std::endl;
    file.close();
  } else {
    if (m_current_returns.size() > 3) {
      /** @todo **/
    } else {
      /**
       * Load the values into r0, r1, r2, r3 as necessary.
       */
      if (m_current_returns.size()) file<<"\tldr %r9, =locals"<<std::endl;
      for (int x = 0; x < m_current_returns.size(); ++x) {
	int y, z;
	for (y = 0; y < m_int_vars.size(); ++y) {
            if (m_int_vars[y] == m_current_returns[x]) break;
          } if (y == m_int_vars.size()) {
            std::cerr<<"Error: returned variable "<<m_current_returns[x]
		     <<" was not declared!"<<std::endl;
            exit(9);
          } for (z = 0; z < m_int_declarations.size(); ++z) {
	    if (m_int_declarations[z] == m_int_vars[y]) break;
	  } if (z != m_int_declarations.size()) {
	    file<<"\tmov %r6, %r9"<<std::endl;
	    file<<"\tldr %r6, [%r9, #"<<4 * z<<"]"<<std::endl;
	    file<<"\tldr %r6, [%r6]"<<std::endl;
	  } else {
	    file<<"\tldr %r6, =I"<<y<<std::endl;
	    file<<"\tldr %r6, [%r6]"<<std::endl;
	  }
	  file<<"\tmov %r"<<x<<", %r6"<<std::endl;
      }
    }
    file<<"\tpop {%lr}\t@ Restore return register"<<std::endl;
    file<<"\tbx lr"<<std::endl;
    file<<"\t.end"<<std::endl;
  }
}// allocate enough space for drew-sized strings.
#endif
