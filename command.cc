/**
 * Benchcookie compiler
 *
 * Ehhhh... So dis works.
 */

#include "command.h"

void Command::addPrintInt(char * _int)
{
  std::string as_string(_int);
  bool has = false;
  
  auto contains = [&has, this] (std::string s) -> bool {
    for (int x = 0; x < m_int_vars.size(); ++x) {
      if (m_int_vars[x] == s) {
	has = true;
	return true;
      }
    } return has = false;
  };

  auto p = contains(as_string);
  if (!has) {
    std::cerr<<"Error: variable "<<as_string;
    std::cerr<<" has not been declared!"<<std::endl;
    exit(1);
  }
}

void Command::addPrintString(char * arg){}

void Command::addPrintLiteral(char * arg)
{ m_literals.push_back(std::string(arg) + "\\n"); }

void Command::addReadInt(char * arg){}

void Command::addReadString(char * arg){}

void Command::doBSS(std::ostream & file)
{
  file<<"\t.bss"<<std::endl<<std::endl;
}

void Command::doData(std::ostream & file)
{
  file<<"\t.data"<<std::endl<<std::endl;
  for (int x = 0; x < m_literals.size(); ++x) {
    file<<"S"<<x<<":\t.ascii \""<<m_literals[x]<<"\\0\""<<std::endl;
  }
}

void Command::doMain(std::ostream & file)
{
  file<<"main:"<<std::endl;
  for (int x = 0; x < m_literals.size(); ++x) {
    file<<"\tldr %r0, =S"<<x<<std::endl;
    file<<"\tbl printf"<<std::endl<<std::endl;
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
  
  file<<std::endl<<std::endl;
  
  doData(file);
  
  file<<std::endl<<std::endl;
  file<<"\t.text"<<std::endl<<std::endl;
  file<<"\t.global main"<<std::endl;
  file<<"\t.global printf"<<std::endl<<std::endl;
  
  doMain(file);
  
  file<<std::endl<<std::endl;
  file<<"\tmov %r7, $1"<<std::endl;


  file<<"\tswi $0"<<std::endl;
  file<<"\t.end"<<std::endl;
  file.close();
}

int yyparse(void);

Command Command::cmd;

int main(int argc, char ** argv)
{
  if (argc != 2) goto error;

  // get the filename
  Command::cmd.setFilename(std::string(argv[1]));

  //parse the code
  yyparse();

  Command::cmd.writeAssembly();
 
  return 0;
  
 error:
  std::cerr<<"Invalid arguments!"<<std::endl;
  std::cerr<<"Usage: bcc output.s"<<std::endl;
  return 1;
}

