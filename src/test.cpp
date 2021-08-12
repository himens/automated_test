#include "test.h"

/************/
/* Run test */
/************/
void Test::run()
{
  print_banner("Run test!");

  for (auto cmd : _commands)
  {
    std::cout << "Run '" << cmd->get_name() << "': \n";
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
  // utility function: strip comment from line
  auto strip_comment = [] (std::string &line)
  {
    line = line.substr(0, line.find('#')); 
  };

  // utility function: tell if line is comment
  auto is_comment = [&] (std::string line)
  {
    strip_comment(line); 
    Utils::strip_char('\t', line);
    Utils::strip_char(' ', line);

    return line.length() == 0;
  };

  // utility function: check end section in line
  auto is_end_of_section = [] (const std::string line)
  {
    auto tokens = Utils::tokens(line);
    return tokens.size() == 1 && tokens.front() == "\\end";
  };

  // utility function: check if body line is formatted
  auto check_body_indent = [&] (const std::string line)
  {
    if (is_end_of_section(line) || is_comment(line)) return; // skip comment and end of section 

    auto tabs = std::count(line.begin(), line.end(), '\t'); // 1-tab indentation if not a comment line
    if (!tabs) throw SyntaxError("line '" + line + "' not indented (tab missing)!");
    else if (tabs > 1) throw SyntaxError("line '" + line + "' not indented (" + std::to_string(tabs) + " tabs)!");
  };

  // utility function: replace variables
  auto replace_variables = [&] (std::vector<std::string> &args)
  {
    for (auto &arg : args) 
    {
      if (arg.front() == '$') 
      {
	Utils::strip_char('$', arg);

	if (_variables.count(arg) > 0) arg = _variables[arg]->get_value();
	else 
	{
	  throw SyntaxError("variable '" + arg + "' not defined!");
	}
      }
    }
  };

  print_banner("Read file: " + filename);

  /* Open file */
  std::ifstream file(filename, std::ios::in);
  if (!file.good()) 
  {
    throw Error("cannot open file '" + filename + "'");
  }

  /* Parse file */
  std::string line;
  while (std::getline(file, line))
  {   
    if (line.empty() || is_comment(line)) continue;

    auto tokens = Utils::tokens(line);
    auto section = tokens[0];
    auto sect_args = Utils::remove_first(tokens);

    /* '\step' section */
    if (section == "\\step")
    {
      bool end_of_section_found = false;

      /* Parse body */
      while (std::getline(file, line))
      {
	if (line.empty() || is_comment(line)) continue;

	end_of_section_found = is_end_of_section(line); 
	if (end_of_section_found) break;

	check_body_indent(line);

	Utils::strip_char('\t', line);
	strip_comment(line);

	auto tokens = Utils::tokens( line );
	if (tokens[0].front() != '@')
	{
	  throw SyntaxError("'" + line + "' not a command statement!");
	}

	Utils::strip_char('@', tokens[0]);

	auto cmd_name = tokens[0];
	auto cmd_args = Utils::remove_first( tokens );
	auto cmd = get_command(cmd_name);
	if (cmd) 
	{
	  replace_variables(cmd_args);
	  cmd->set_args(cmd_args);
	  _commands.push_back(cmd);
	}
      } 

      if (!end_of_section_found) 
      {
	throw SyntaxError("cannot find '\\end' of '" + section + "'!"); 
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
      auto placeholders = Utils::remove_first(sect_args);
      std::vector<std::shared_ptr<Command>> commands = {};

      if (_user_command_map.count(usr_cmd_name) > 0)
      {
        throw Error("user command '" + usr_cmd_name + "' already defined!"); 
      }

      bool end_of_section_found = false;

      /* Parse body */ 
      while (std::getline(file, line))
      {
	if (line.empty() || is_comment(line)) continue;

	end_of_section_found = is_end_of_section(line); 
	if (end_of_section_found) break;

	check_body_indent(line);

	Utils::strip_char('\t', line);
	strip_comment(line);

	auto tokens = Utils::tokens(line);
	if (tokens[0].front() != '@')
	{
	  throw SyntaxError("'" + line + "' not a command statement!");
	}

	Utils::strip_char('@', tokens[0]);
	
	auto cmd_name = tokens[0];
	auto cmd_args = Utils::remove_first(tokens);
	auto cmd = get_command(cmd_name);
	if (cmd) 
	{
	  replace_variables(cmd_args);
	  cmd->set_args(cmd_args);
	  commands.push_back(cmd);
	}
      } 
      
      if (!end_of_section_found) 
      {
	throw SyntaxError("cannot find '\\end' of '" + section + "'!"); 
      }

      _user_command_map[usr_cmd_name] = std::make_shared<UserCmd>(usr_cmd_name, placeholders, commands);
      //_user_command_map[usr_cmd_name] = {usr_cmd_name, placeholders, commands};
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

      auto name = sect_args[0];
      auto value = sect_args[2];
      _variables[name] = std::make_shared<Variable>(name, value);
    }

    /* '\set' section */
    else if (section == "\\set")
    {
      if (sect_args.size() != 3)
      {
	throw SyntaxError("'" + line + "' missing arguments!");
      }

      if (sect_args[1] != ":=") 
      {
	throw SyntaxError("'" + line + "' missing assignment operator ':='!");
      }

      auto name = sect_args[0];
      auto value = sect_args[2];
      if (_variables.count(name) > 0) 
      {
	_variables[name]->set_value(value);
      }
      else
      {
	throw SyntaxError("variable '" + name + "' not defined!");
      }
    }

    /* '\alias' section */
    else if (section == "\\alias")
    {
      if (sect_args.size() < 3)
      {
	throw SyntaxError("'" + line + "' missing arguments!");
      }

      if (sect_args[1] != ":=") 
      {
	throw SyntaxError("'" + line + "' missing assignment operator ':='!");
      }

      auto alias = sect_args[0];
      auto tgt = sect_args[2];

      if (tgt.front() == '$') // alias to variable
      {
	if (sect_args.size() != 3)
	{
	  throw SyntaxError("Too many arguments specified for alias to variable: '" + line + "'!");
	}

	Utils::strip_char('$', tgt);

	if (_variables.count(tgt) > 0) 
	{
	  _variables[alias] = _variables[tgt];
	}
	else
	{
	  throw SyntaxError("'" + line + "' points to unknown variable!");
	}
      }

      else if (tgt.front() == '@') // alias to command
      {
	Utils::strip_char('@', tgt);

	auto cmd = get_command(tgt);
	if (cmd)
	{ 
	  _user_command_map[alias] = cmd;
	  _user_command_map[alias]->set_name(alias);
	}
      }
    }

    /* Unknown section */
    else 
    {
      throw SyntaxError("unknown section '" + section + "'!");
    }	
  } // end parse file

  std::cout << "File: " + filename + " read successfully! \n";
}



