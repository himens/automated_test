#include "test.h"

/************/
/* Run test */
/************/
void Test::run()
{
  /* Create report file */
  std::ofstream file("report_" + _name + ".md", std::ios::out);
  file << "# Test: '" + _name << "' \n";

  /* Run test */
  if (_steps.size() > 0)
  {
    Utils::print_banner("Run test: '" + _name + "'");

    for (auto step : _steps) 
    { 
      step.run();
      step.write_report(file);
    }

    Utils::print_banner("Test done!");
  }
  else
  {
    Utils::print_banner("Test '" + _name + "' does not contain any step!");
    file << "## Test '" << _name << "' does not contain any step! \n";
  }

  file.close();
}


/***********************/
/* Make command object */
/***********************/
std::shared_ptr<Command> Test::make_command(const std::string name)
{
  auto cmd = make_predefined_cmd(name);

  if (cmd == nullptr) // check if cmd name is an user-command...
  {
    auto it = std::find_if(_user_commands.begin(), _user_commands.end(), 
	[&] (UserCmd cmd) { return name == cmd.get_name(); });

    if (it != _user_commands.end()) 
    {
      std::vector<std::shared_ptr<Command>> commands = {}; // allocate sub-commands

      for (auto cmd : it->get_commands())
      {
	auto c = make_command( cmd->get_name() );
	c->set_args( cmd->get_args() );
	commands.push_back(c);
      }

      cmd = std::make_shared<UserCmd>(it->get_name(), it->get_placeholders(), commands);
    }
    else 
    {
      throw Error("Unknown command '" + name + "'!");
    }
  }

  return cmd;
}


/******************/
/* Read test file */
/******************/
void Test::read(const std::string filename)
{
  // utility function: remove comments and tabs from line
  auto strip_tabs_and_comments = [] (std::string &line)
  {
    auto pos = line.find('#');
    if (pos != std::string::npos) line = line.substr(0, pos); // remove comments
    Utils::strip_char('\t', line); // remove tabs
  };

  // utility function: tell if line is comment
  auto is_comment = [&] (std::string line)
  {
    strip_tabs_and_comments(line); 

    return Utils::tokens(line).size() == 0;
  };

  // utility function: check end section in line
  auto is_end_of_section = [] (const std::string line)
  {
    auto tokens = Utils::tokens(line);

    return tokens.size() > 0 && tokens.front() == "\\end";
  };

  // utility function:perform some sanity checks on body line
  auto check_body_line = [&] (const std::string line)
  {
    if (line.empty() || is_end_of_section(line) || is_comment(line)) return;

    if (line.front() == '\\') // section inside body
    {
      throw SyntaxError("'" + line + "' section declaration inside body! Maybe '\\end' is missing?");
    }

    auto tabs = std::count(line.begin(), line.end(), '\t'); // 1-tab indentation if not a comment line
    if (!tabs) 
    {
      throw SyntaxError("line '" + line + "' not indented (tab missing)!");
    }
    else if (tabs > 1) 
    {
      throw SyntaxError("line '" + line + "' not indented (" + std::to_string(tabs) + " tabs)!");
    }
  };

  // utility function: replace variables with their values
  auto replace_variable_with_val = [&] (std::vector<std::string> &args)
  {
    for (auto &arg : args) 
    {
      if (!arg.empty() && arg.front() == '$') 
      {
	auto it = std::find(_variables.begin(), _variables.end(), Variable{arg});
	if (it != _variables.end()) 
	{
	  arg = it->get_value();
	}
	else 
	{
	  throw SyntaxError("variable '" + arg + "' not defined!");
	}
      }
    }
  };

  /* Open file */
  Utils::print_banner("Read file: '" + filename + "'");

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
    Utils::erase_front(tokens);
    auto sect_args = tokens;

    /* '\step' section */
    if (section == "\\step")
    {
      if (sect_args.size() == 0)
      {
	throw SyntaxError("'" + line + "' missing step name!");
      }

      auto name = sect_args[0];
      std::vector<std::shared_ptr<Command>> commands = {};

      /* Parse body */
      while (std::getline(file, line) && !is_end_of_section(line))
      {
	if (line.empty() || is_comment(line)) continue;
	check_body_line(line);
	strip_tabs_and_comments(line);

	auto tokens = Utils::tokens(line);
	auto cmd_name = tokens[0];
	Utils::erase_front(tokens);
	auto cmd_args = tokens;

	replace_variable_with_val(cmd_args);

	auto cmd = make_command(cmd_name);
	cmd->set_args(cmd_args);
	commands.push_back(cmd);
      } 

      if (!is_end_of_section(line)) 
      {
	throw SyntaxError("cannot find '\\end' of '" + section + " " + name + "'!"); 
      }

      _steps.push_back({name, commands});
    }

    /* '\user_cmd' section */
    else if (section ==  "\\user_cmd")
    {
      if (sect_args.size() == 0)
      {
	throw SyntaxError("'" + line + "' missing command name!");
      }

      auto name = sect_args[0];
      Utils::erase_front(sect_args);
      auto placeholders = sect_args;
      std::vector<std::shared_ptr<Command>> commands = {};

      if (std::find_if(_user_commands.begin(), _user_commands.end(), 
	    [&] (UserCmd cmd) { return cmd.get_name() == name; }) != _user_commands.end())
      {
        throw Error("user command '" + name + "' already defined!"); 
      }

      /* Parse body */ 
      while (std::getline(file, line) && !is_end_of_section(line))
      {
	if (line.empty() || is_comment(line)) continue;
	check_body_line(line);
	strip_tabs_and_comments(line);

	auto tokens = Utils::tokens(line);
	auto cmd_name = tokens[0];
	Utils::erase_front(tokens);
	auto cmd_args = tokens;
	
	replace_variable_with_val(cmd_args);

	auto cmd = make_command(cmd_name);
	cmd->set_args(cmd_args);
	commands.push_back(cmd);
      } 
      
      if (!is_end_of_section(line)) 
      {
	throw SyntaxError("cannot find '\\end' of '" + section + " " + name + "'!"); 
      }

      _user_commands.push_back({name, placeholders, commands});
    }
	
    /* '\include' section */
    else if (section == "\\include")
    {
      if (sect_args.size() == 0)
      {
	throw SyntaxError("'" + line + "' missing filename!");
      }	

      read(sect_args[0]); // recursive parsing
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

      auto it = std::find(_variables.begin(), _variables.end(), Variable{name});
      if (it != _variables.end()) 
      {
	throw SyntaxError("variable '" + name + "' already defined!");
      }

      _variables.push_back({name, value});
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

      auto it = std::find(_variables.begin(), _variables.end(), Variable{name});
      if (it != _variables.end()) 
      {
	it->set_value(value);
      }
      else
      {
	throw SyntaxError("variable '" + name + "' not defined! Cannot set its value!");
      }
    }

    /* Unknown section */
    else 
    {
      throw SyntaxError("unknown section '" + section + "'!");
    }	
  } // end parse file

  std::cout << "File '" + filename + "' read successfully! \n";
}

