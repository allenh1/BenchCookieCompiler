// allocate enough space for drew-sized strings.
#define MAX_STRING_SIZE 100
void Command::doBSS(std::ostream & file)
{
  file<<"\t.lcomm locals, 400";
  file<<std::endl;
}

void Command::doData(std::ostream & file)
{
  file<<"\t.data"<<std::endl<<std::endl;

  /** Place the string literals **/

  for (int x = 0; x < m_literals.size(); ++x) {
    file<<".data"<<std::endl;
    file<<"S"<<x<<":\t.ascii \""<<m_literals[x]<<"\\0\""<<std::endl;
    std::endl;
  }

  std::cout<<std::endl;

  /** Place the Input Strings **/

  for (int x =0; x < m_string_vars.size(); ++x) {
    file<<"IS"<<x<<":\t.space "<<MAX_STRING_SIZE<<std::endl;
  }
  /** Place the integers. **/

  for (int x = 0; x < m_int_vars.size() - m_int_declarations.size(); ++x) {
    file<<"I"<<x<<":\t.word 0"<<std::endl;
  }

  /** Mark space for locals **/
  file<<"locals:\t.word 400"<<std::endl;
  
  // file<<std::endl<<"string_fmt:\t.ascii \"%s\\0\""<<std::endl;
  // file<<"num_fmt:\t.ascii \"%d\\0\""<<std::endl;
  file<<"TRUE_STORY:\t.ascii \"true\\0\""<<std::endl;
  file<<"FALSE:\t.ascii \"false\\0\""<<std::endl;
}

void Command::doMain(std::ostream & file)
{
  int y; size_t divcount = 0, nszcount = 0;
  const size_t local_count = 0;// Idk, yo. Just... somewhere.
  file<<"main:"<<std::endl;
  file<<"\tldr %r9, =locals"<<std::endl;
  file<<"\t@ Make space for locals"<<std::endl;
  /**
   * @todo use main's function calls for subroutines.
   */

  for (int x = 0, intassdex = 0, stringdex = 0, exprdex = 0,
	 intdex = 0, pintdex = 0, pstringdex = 0, litdex = 0,
	 pbooldex = 0, sints = 0, ifndex = 0, forndex = 0;
      x < m_execOrder.size(); ++x) {

    if (m_execOrder[x] == cmd_type::READ_STRING) {
      file<<"\tldr %r0, =string_fmt"<<std::endl;
      file<<"\tldr %r1, =IS"<<stringdex<<std::endl;
      file<<"\tbl scanf"<<std::endl;
      file<<std::endl;

      ++stringdex;
    } else if (m_execOrder[x] == cmd_type::DECL_INT) {
      file<<"\tli %a0, 4"<<std::endl;
      file<<"\tbl malloc"<<std::endl;
      file<<"\tstr %r0, [%r9, #"<<sints * 4<<"]"<<std::endl;

      file<<std::endl;
      m_int_vars.push_back(m_int_declarations[sints++]);
    } else if (m_execOrder[x] == cmd_type::BEGIN_IF) {
      // find the variable we depend on!
      std::string var_name = m_if_deps[ifndex];
      for (y = 0; y < m_int_vars.size(); ++y) {
	if (m_int_vars[y] == var_name) break;
      } int z = 0;
      for (z = 0; z < m_int_declarations.size(); ++z) {
	if (m_int_declarations[z] == var_name) break;
      } if (z < m_int_declarations.size()) {
	file<<"\tmov %r1, %r9"<<std::endl;
	file<<"\tldr %r1, [%r1, #"<<4*z<<"]"<<std::endl;
	file<<"\tldr %r0, [%r1]"<<std::endl;
      } else if (y < m_int_vars.size())  {
	file<<"\tldr %r0, =I"<<y<<std::endl;
	file<<"\tldr %r0, [%r0]"<<std::endl;
      } else {
	std::cerr<<"Error: if-dependent variable "<<var_name;
	std::cerr<<" not yet declared!"<<std::endl;
	exit(6);
      }

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
          } int z;
	  for (z = 0; z < m_int_declarations.size(); ++z) {
	    if (m_int_declarations[z] == m_int_vars[y]) break;
	  } if (z != m_int_declarations.size()) {
	    file<<"\tmov %r1, %r9"<<std::endl;
	    file<<"\tldr %r1, [%r9, #"<<4 * z<<"]"<<std::endl;
	    file<<"\tldr %r1, [%r1]"<<std::endl;
	  } else {
	    file<<"\tldr %r1, =I"<<y<<std::endl;
	    file<<"\tldr %r1, [%r1]"<<std::endl;
	  }
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
            file<<"\tsub %r0, %r2, %r1"<<std::endl;
            goto do_default;
          case MUL:
            file<<"\tmul %r0, %r1, %r2"<<std::endl;
            goto do_default;
          case DIV:
	    /* @todo case divide by zero */
	    file<<"\tmov %r0, $0"<<std::endl<<std::endl;
	    file<<"DIVIDE"<<divcount++<<": cmp %r1, %r2"<<std::endl;
	    file<<"\t bgt DONEDIVIDE"<<divcount-1<<std::endl;
	    file<<"\t sub %r2, %r2, %r1"<<std::endl;
	    file<<"\t add %r0, $1"<<std::endl;
            file<<"\t b DIVIDE"<<divcount-1<<std::endl;
	    file<<"DONEDIVIDE"<<divcount-1<<":"<<std::endl;
            goto do_default;
	  case GT:
	    file<<"\tmov %r0, $1"<<std::endl;
	    file<<"\tcmp %r2, %r1"<<"\t@ Backwards because stack"<<std::endl;
	    file<<std::endl;
	    file<<"\tbgt NOSETZERO"<<nszcount<<std::endl;
	    file<<"\tmov %r0, $0"<<std::endl;
	    file<<"NOSETZERO"<<nszcount<<": # label used to set as true"<<std::endl;
	    nszcount++;
	    goto do_default;
	  case GEQ:
	    file<<"\tmov %r0, $1"<<std::endl;
	    file<<"\tcmp %r2, %r1"<<std::endl;
	    file<<"\tbge NOSETZERO"<<nszcount<<std::endl;
	    file<<"\tmov %r0, $0"<<std::endl;
	    file<<"NOSETZERO"<<nszcount<<": # label used to set as true"<<std::endl;
	    nszcount++;
	    goto do_default;
	  case LT:
	    file<<"\tmov %r0, $1"<<std::endl;
	    file<<"\tcmp %r2, %r1"<<std::endl;
	    file<<"\tblt NOSETZERO"<<nszcount<<std::endl;
	    file<<"\tmov %r0, $0"<<std::endl;
	    file<<"NOSETZERO"<<nszcount<<": # label used to set as true"<<std::endl;
	    nszcount++;
	    goto do_default;
	  case LEQ:
	    file<<"\tmov %r0, $1"<<std::endl;
	    file<<"\tcmp %r2, %r1"<<std::endl;
	    file<<"\tble NOSETZERO"<<nszcount<<std::endl;
	    file<<"\tmov %r0, $0"<<std::endl;
	    file<<"NOSETZERO"<<nszcount<<": # label used to set as true"<<std::endl;
	    nszcount++;
	    goto do_default;
          default:
	    std::cerr<<"Welp. You did it. You tried it."<<std::endl;
	    std::cerr<<"This is what happens. You get mad."<<std::endl;
	    std::cerr<<"Don't use things I didn't implement yet."<<std::endl;
	    exit(101);
          do_default:
            file<<"\tstr %r0, -4($sp)"<<std::endl;
	    file<<"\tsubi $sp, 4"<<std::endl;
	    stack_depth += 4;
        }
      }

      file<<"\tldr %r0, [%sp, #0]"<<std::endl;
      file<<"\tadd %sp, %sp, $0"<<std::endl;
      stack_depth -= 4;
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
      if (stack_depth) file<<"\tadd %sp, $"<<stack_depth<<"\t@ Destroy stack"<<std::endl;
      else file<<"\t@ Yayy! Stack has nothing left."<<std::endl<<"\t@ You should be happy. I am."<<std::endl;
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
  file <<" *"<<m_filename<<std::endl;
  file <<" *"<<std::endl;
  file <<" * Generated by Bench Cookie Compiler!"<<std::endl;
  file <<" * Bench Cookie is... Experimental. Don't be mad."<<std::endl;
  file <<" */"<<std::endl;

  doBSS(file);

  file<<std::endl;

  doData(file);

  file<<std::endl<<std::endl;
  file<<"\t.text"<<std::endl;
  //  file<<"\t.global main"<<std::endl;
  //  file<<"\t.global printf"<<std::endl;
  //  file<<"\t.global scanf"<<std::endl;
  //  file<<"\t.global malloc"<<std::endl;

  doMain(file);

  /**
   * Free locals
   *
   * @todo Free locals when last used.
   *       Maybe change file to a list
   *       and search for last usage?
   */
  file<<"\t@ Free local vars"<<std::endl;
  for (int x = 0; x < m_int_declarations.size(); ++x) {
    file<<"\tldr %r0, =locals"<<std::endl;
    file<<"\tldr %r0, [%r0, #"<<4 * x<<"]"<<std::endl;
    file<<"\tbl free"<<std::endl<<std::endl;
  }

  file<<"\t@ locals are free"<<std::endl<<std::endl;

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
}// allocate enough space for drew-sized strings.
