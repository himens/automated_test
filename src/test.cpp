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
void Test::add_user_command(const UserCmd &usr_cmd)
{
  if (std::find_if(_user_commands.begin(), _user_commands.end(), 
	[&] (UserCmd cmd) { return cmd.get_name() == usr_cmd.get_name(); }) != _user_commands.end())
  {
    throw Error("Test::add_user_command: user-command '" + usr_cmd.get_name() + "' already defined!"); 
  }

  _user_commands.push_back(usr_cmd);
}

void Test::add_step(const Step &step) 
{ 
  if (std::find_if(_steps.begin(), _steps.end(), 
	[&] (Step s) { return s.get_name() == step.get_name(); }) != _steps.end())
  {
    throw Error("Test::add_step: step '" + step.get_name() + "' already defined!"); 
  }

  _steps.push_back(step); 
}

void Test::set_name(const std::string name) 
{ 
  if (name.empty()) 
  {
    throw Error("Test::set_name: name is empty!");
  }

  _name = name; 
}

void Test::set_steps(const std::vector<Step> &steps) 
{ 
  for (const auto &step : steps) add_step(step); 
}

void Test::set_user_commands(const std::vector<UserCmd> &commands)
{
  for (const auto &cmd : commands) add_user_command(cmd);
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
  std::shared_ptr<Command> cmd = nullptr;

  auto it = std::find_if(_user_commands.begin(), _user_commands.end(), 
      [&] (UserCmd cmd) { return name == cmd.get_name(); });

  if (name == "set_thrust") cmd = std::make_shared<SetThrustCmd>();
  else if (name == "insert_pds") cmd = std::make_shared<InsertPdsCmd>();
  else if (name == "check") cmd = std::make_shared<CheckCmd>();
  else if (it != _user_commands.end()) // check if cmd name is an user-command...
  {
    std::vector<std::string> args_names;
    for (auto arg : it->get_args()) 
    {
      args_names.push_back( arg.get_name() );
    }

    UserCmd usr_cmd = {it->get_name(), args_names};

    for (auto cmd : it->get_commands())
    {
      auto c = make_command( cmd->get_name() );

      std::vector<std::string> values;
      for (auto arg : cmd->get_args()) 
      {
	values.push_back( arg.get_value() );
      }
      c->set_args_values(values);

      usr_cmd.add_command(c);
    }

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

	auto tokens = Utils::tokens(line);
	auto cmd_name = tokens[0];
	Utils::erase_front(tokens);
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
      Utils::erase_front(sect_args);

      UserCmd usr_cmd{name, sect_args};

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

