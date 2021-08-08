#include "test.h"

/************/
/* Run test */
/************/
void Test::run()
{
  print_banner("Run test!");

  for (auto cmd : _commands)
  {
    std::cout << "Run command: '" << cmd->get_name() << "'\n";
    cmd->run();
    std::cout << "\n";
  }
  
  print_banner("Test done!");
}


/****************/
/* Print banner */
/****************/
void Test::print_banner(const std::string str, size_t length)
{
  if (length == 0) length = str.size() + 2;

  std::cout << "\n";
  std::cout << std::string(length, '*') << "\n";
  std::cout << str << "\n";
  std::cout << std::string(length, '*') << "\n";
}


/*******************/
/* Parse test file */
/*******************/
void Test::parse_test(const std::string filename)
{
  // utility function: skip lines
  auto is_comment = [](std::string line)
  {
    line.erase(std::remove(line.begin(), line.end(), '\t'), line.end()); // remove tabs

    return line.empty() || line.front() == '#'; // skip line with comment or empty
  };

  // utility function: check if line is indented
  auto check_formatting = [&](std::string line)
  {
    if (line == "\\end" || is_comment(line)) return; // if comment or end section, it's ok

    auto num_tabs = std::count(line.begin(), line.end(), '\t'); // require 1-tab indentation
    if (!num_tabs) throw SyntaxError("line '" + line + "' not indented (tab missing)!");
    if (num_tabs > 1) throw SyntaxError("line '" + line + "' not indented (" + std::to_string(num_tabs) + " tabs)!");

    line.erase(std::remove(line.begin(), line.end(), '\t'), line.end()); // body lines should begin with '\\'
    if (line.front() != '\\') throw SyntaxError("line '" + line + "' should begin with '\\'!");
  };

  // utility function: check end section in line
  auto is_end_section = [] (const std::string line)
  {
    auto tokens = Utils::tokens(line);
    return tokens.size() == 1 && tokens.front() == "\\end";
  };

  // replace variables
  auto replace_variables = [&](std::vector<std::string> args)
  {
    for (auto &arg : args) // check if variable has been defined
      if (Variable::is_tagged(arg)) 
	if (std::none_of(_variables.begin(), _variables.end(), 
	      [&] (std::pair<Variable, std::string> p) { return p.first == Variable(arg); }))
	    throw Error("variable '" + arg + "' not defined!");

    for (auto &var : _variables)
      for (auto &arg : args) 
	if (var.first.to_string() == arg) arg = var.second;

    return args;
  };
  
  print_banner("Read file: " + filename);

  /* Open file */
  std::ifstream file(filename, std::ios::in);
  if (!file.good()) 
  {
    throw Error("Cannot open file '" + filename + "'");
  }

  /* Parse file */
  std::string line;
  while (std::getline(file, line))
  {   
    if (is_comment(line)) continue;

    auto section = Utils::first( Utils::tokens(line) );
    auto sect_args = Utils::remove_first( Utils::tokens(line) );

    /* '\step' section */
    if (section == "\\step")
    {
      bool end_section_found = false;

      /* Parse body */
      while (std::getline(file, line))
      {
	check_formatting(line);

	end_section_found = is_end_section(line); 
	if (end_section_found) break;
	if (is_comment(line)) continue;

	auto cmd_name = Utils::first( Utils::tokens(line) );
	auto cmd_args = Utils::remove_first( Utils::tokens(line) );
	auto cmd = get_command(cmd_name);
	if (cmd) 
	{
	  cmd->set_args( replace_variables(cmd_args) );
	  _commands.push_back(cmd);
	}
      } 

      if (!end_section_found) 
      {
	throw SyntaxError("Cannot find '\\end' of '" + section + "'!"); 
      }
    }

    /* '\user_cmd' section */
    else if (section ==  "\\user_cmd")
    {
      if (sect_args.size() == 0)
      {
	throw SyntaxError("'" + line + "' missing command name!");
      }

      auto usr_cmd_name = sect_args[0];
      auto usr_cmd_args = Utils::remove_first(sect_args);

      if (_user_command_map.count("\\" + usr_cmd_name) > 0)
      {
        throw Error("user command '" + usr_cmd_name + "' already defined!"); 
      }

      for (auto &arg : usr_cmd_args) 
      {
        if (!Placeholder::is_tagged(arg)) 
        {
          throw SyntaxError("'" + line + "' missing placeholder! Try: '_" + arg + "'!"); 
        }
      }

      std::vector<std::shared_ptr<Command>> commands = {};
      std::vector<Placeholder> placeholders = {};
      for (auto &arg : usr_cmd_args) placeholders.push_back(arg);

      bool end_section_found = false;

      /* Parse body */ 
      while (std::getline(file, line))
      {
	check_formatting(line);

	end_section_found = is_end_section(line); 
	if (end_section_found) break;
	if (is_comment(line)) continue;

	auto cmd_name = Utils::first( Utils::tokens(line) );
	auto cmd_args = Utils::remove_first( Utils::tokens(line) );
	auto cmd = get_command(cmd_name);
	if (cmd) 
	{
	  cmd->set_args( replace_variables(cmd_args) );
	  commands.push_back(cmd);
	}
      } 
      
      if (!end_section_found) 
      {
	throw SyntaxError("Cannot find '\\end' of '" + section + "'!"); 
      }

      _user_command_map["\\" + usr_cmd_name] = {usr_cmd_name, placeholders, commands};
    }
	
    /* '\include' section */
    else if (section == "\\include")
    {
      if (sect_args.size() == 0)
      {
	throw SyntaxError("'" + line + "' missing filename!");
      }	

      parse_test(sect_args[0]); // recursive parsing
    }
    
    /* '\var' section */
    else if (section == "\\var")
    {
      if (sect_args.size() != 3)
      {
	throw SyntaxError("'" + line + "' missing arguments!");
      }

      if (sect_args[1] != ":=") 
      {
	throw SyntaxError("'" + line + "' missing assignment operator ':='!");
      }

      auto var_name = sect_args[0];
      auto var_value = sect_args[2];
      _variables.push_back({"$" + var_name, var_value});
    }

    /* Unknown section */
    else 
    {
      throw Error("Unknown section '" + section + "'!");
    }	
  } // end parse file
}



