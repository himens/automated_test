#include "test.h"

/**********/
/* Constr */
/**********/
Test::Test() {};

Test::Test(const std::string name) 
{ 
  set_name(name); 
};


/***********/
/* Set/get */
/***********/
void Test::add_user_command(const UserCmd &cmd)
{
  if (_user_command_map.count(cmd.get_name()) > 0) 
  {
    throw Error("Test::add_user_command: user-command '" + cmd.get_name() + "' already defined!"); 
  }

  _user_command_map[cmd.get_name()] = cmd;
}

void Test::add_step(const Step &step) 
{ 
  if (_step_map.count(step.get_name()) > 0)
  {
    throw Error("Test::add_step: step '" + step.get_name() + "' already defined!"); 
  }

  _step_map[step.get_name()] = step; 
}

void Test::set_name(const std::string name) 
{ 
  if (name.empty()) 
  {
    throw Error("Test::set_name: name is empty!");
  }

  _name = name; 
}


/************/
/* Run test */
/************/
void Test::run()
{
  /* Create report file */
  std::ofstream file("report_" + _name + ".md", std::ios::out);
  file << "# Test: '" + _name << "' \n";

  /* Run test */
  if (_step_map.size() > 0)
  {
    Utils::print_banner("Run test: '" + _name + "'");

    for (auto &p : _step_map) 
    { 
      auto step = p.second;
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
  std::shared_ptr<Command> cmd = nullptr;

  if (name == "set_thrust") cmd = std::make_shared<SetThrustCmd>();
  else if (name == "insert_pds") cmd = std::make_shared<InsertPdsCmd>();
  else if (name == "check") cmd = std::make_shared<CheckCmd>();
  else if (_user_command_map.count(name) > 0) // check if cmd name is an user-command...
  {
    auto usr_cmd = _user_command_map[name];
    auto commands = usr_cmd.get_commands();

    for (auto &cmd : commands)
    {
      std::vector<std::string> values;
      for (auto arg : cmd->get_args()) 
      {
	values.push_back( arg.get_value() );
      }
      
      cmd = make_command( cmd->get_name() );
      cmd->set_args_values(values);
    }

    usr_cmd.set_commands(commands);
    cmd = std::make_shared<UserCmd>(usr_cmd);
  }
  else 
  {
    throw Error("Unknown command '" + name + "'!");
  }

  return cmd;
}


/******************/
/* Read test file */
/******************/
void Test::read_test_file(const std::string filename)
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
  auto is_end_of_section = [&] (std::string line)
  {
    strip_tabs_and_comments(line); 

    auto tokens = Utils::tokens(line);

    return tokens.size() > 0 && tokens.front() == "\\end";
  };

  // utility function: perform some sanity checks on body line
  auto check_body_line = [&] (const std::string line)
  {
    if (line.empty() || is_end_of_section(line) || is_comment(line)) return;

    if (line.front() == '\\') // section inside body
    {
      throw Error("syntax error: '" + line + "' section declaration inside body! Maybe '\\end' is missing?");
    }

    auto tabs = std::count(line.begin(), line.end(), '\t'); // 1-tab indentation if not a comment line
    if (!tabs) 
    {
      throw Error("syntax error: line '" + line + "' not indented (tab missing)!");
    }
    else if (tabs > 1) 
    {
      throw Error("syntax error: line '" + line + "' not indented (" + std::to_string(tabs) + " tabs)!");
    }
  };

  // utility function: add lines after carriage return 
  auto add_carriage_return_lines = [&] (std::ifstream &file, std::string &line)
  {
    std::string new_line = line;

    while (new_line.find("\\") != std::string::npos)
    {
      std::getline(file, new_line);
      strip_tabs_and_comments(new_line); 
      
      line += new_line;
      Utils::strip_char('\\', line);
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

    tokens.erase( tokens.begin() );
    auto sect_args = tokens;

    /* '\step' section */
    if (section == "\\step")
    {
      if (sect_args.size() == 0)
      {
	throw Error("syntax error: '" + line + "' missing step name!");
      }

      auto name = sect_args[0];
      Step step{name};

      /* Parse body */
      while (std::getline(file, line) && !is_end_of_section(line))
      {
	if (line.empty() || is_comment(line)) continue;
	check_body_line(line);
	strip_tabs_and_comments(line);
	add_carriage_return_lines(file, line);

	auto tokens = Utils::tokens(line);
	auto cmd_name = tokens[0];

	tokens.erase( tokens.begin() );
	auto cmd_args = tokens;

	auto cmd = make_command(cmd_name);
	cmd->set_args_values(cmd_args);

        step.add_command(cmd);
      } 

      if (!is_end_of_section(line)) 
      {
	throw Error("syntax error: cannot find '\\end' of '" + section + " " + name + "'!"); 
      }

      add_step(step);
    }

    /* '\user_cmd' section */
    else if (section ==  "\\user_cmd")
    {
      if (sect_args.size() == 0)
      {
	throw Error("syntax error: '" + line + "' missing command name!");
      }

      auto name = sect_args[0];
      sect_args.erase( sect_args.begin() );

      UserCmd usr_cmd{name, sect_args};

      /* Parse body */ 
      while (std::getline(file, line) && !is_end_of_section(line))
      {
	if (line.empty() || is_comment(line)) continue;
	check_body_line(line);
	strip_tabs_and_comments(line);
	add_carriage_return_lines(file, line);

	auto tokens = Utils::tokens(line);
	auto cmd_name = tokens[0];
    
	tokens.erase( tokens.begin() );
	auto cmd_args = tokens;
	
	auto cmd = make_command(cmd_name);
	cmd->set_args_values(cmd_args);

	usr_cmd.add_command(cmd);
      } 
      
      if (!is_end_of_section(line)) 
      {
	throw Error("syntax error: cannot find '\\end' of '" + section + " " + name + "'!"); 
      }
      
      add_user_command(usr_cmd);
    }
	
    /* '\include' section */
    else if (section == "\\include")
    {
      if (sect_args.size() == 0)
      {
	throw Error("syntax error: '" + line + "' missing filename!");
      }	

      read_test_file(sect_args[0]); // recursive parsing
    }
    
    /* Unknown section */
    else 
    {
      throw Error("syntax error: unknown section '" + section + "'!");
    }	
  } // end parse file

  std::cout << "File '" + filename + "' read successfully! \n";
}

