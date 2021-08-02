#include "test.h"

/***************/
/* Get command */
/***************/
Command* Test::get_command(const std::string name, const std::vector<std::string> &args)
{
  Command *cmd = nullptr;

  if (name == "\\set_thrust") cmd = new SetThrustCmd();
  else if (name == "\\insert_pds") cmd = new InsertPdsCmd();
  else if (name == "\\check") cmd = new CheckCmd();
  else if (_user_command_map.count(name) > 0) 
  {
    cmd = new UserCmd(_user_command_map[name]); // check if object is copied!!!
  }
  else std::cout << "ERROR: Unknown command '" << name << "'! \n";

  cmd->set_args(args);

  return cmd;
}

/*******************/
/* Parse test file */
/*******************/
void Test::parse_test(const std::string filename)
{
  // utility function: skip lines
  auto is_comment = [](const std::string line)
  {
    return line.empty() ||   // skip empty line
           (line.front() == '#' || line.front() == '"'); // skip line with comment
  };

  // utility function: check if line is indented
  auto is_not_indented = [](const std::string line)
  {
    auto num_tabs = std::count(line.begin(), line.end(), '\t');
    bool is_not_indented = line != "\\end" && (line.front() != '\t' || num_tabs != 1); 

    if (is_not_indented) 
    {
      if (!num_tabs) std::cout << "ERROR: line '" << line << "' not indented (tab missing)! \n";
      else std::cout << "ERROR: line '" << line << "' not indented (" << num_tabs << " tab)! \n";
    }

    return is_not_indented;
  };

  // utility function: check end section in line
  auto is_end_section = [] (const std::string line)
  {
    auto tokens = utils::tokens(line);

    return tokens.size() == 1 && tokens.front() == "\\end";
  };
  
  /* Open file */
  std::ifstream file(filename, std::ios::in);
  if (!file.good()) 
  {
    std::cout << "ERROR: Cannot open file '" << filename << "' \n";
    return; 
  }

  /* Parse file */
  std::string line;
  while (std::getline(file, line))
  {   
    if (is_comment(line)) continue;

    auto section = utils::first( utils::tokens(line) );
    auto sect_args = utils::remove_first( utils::tokens(line) );

    /* '\begin_step' section */
    if (section == "\\begin_step")
    {
      bool end_section_found = false;

      /* Parse body */
      while (std::getline(file, line))
      {
	end_section_found = is_end_section(line); 

	if (end_section_found) break;
	if (is_comment(line)) continue;
	if (is_not_indented(line)) return;

	auto cmd = utils::first( utils::tokens(line) );
	auto cmd_args = utils::remove_first( utils::tokens(line) );

	auto new_cmd = get_command(cmd, cmd_args);
	if (new_cmd) _commands.push_back(new_cmd);
      } 

      if (!end_section_found) 
      {
	std::cout << "ERROR: Cannot find '\\end' of '" << section << "' section! \n"; 
	return;
      }
    }

    /* '\define_cmd' section */
    else if (section ==  "\\define_cmd")
    {
      if (sect_args.size() == 0)
      {
	std::cout << "ERROR: '\\define_cmd': missing command name! \n";
	return;
      }

      auto usr_cmd = "\\" + utils::first(sect_args);
      auto usr_args = utils::remove_first(sect_args);

      if (_user_command_map.count(usr_cmd) > 0)
      {
	  std::cout << "[ERROR] '\\define_cmd': user command '" << usr_cmd << "' already defined! \n"; 
	  return;
      }

      for (auto &arg : usr_args) 
      {
	if (!Placeholder::is_placeholder(arg)) 
	{
	  std::cout << "[ERROR] '\\define_cmd': placeholder '" << arg << "' should begin with '_'! \n"; 
	  return;
	}
      }

      std::vector<Command*> commands = {};
      bool end_section_found = false;

      /* Parse body */ 
      while (std::getline(file, line))
      {
	end_section_found = is_end_section(line); 

	if (end_section_found) break;
	if (is_comment(line)) continue;
	if (is_not_indented(line)) return;

	auto cmd = utils::first( utils::tokens(line) );
	auto cmd_args = utils::remove_first( utils::tokens(line) );
	
	auto new_cmd = get_command(cmd, cmd_args); 
	if (new_cmd) commands.push_back(new_cmd);
      } 
      
      if (!end_section_found) 
      {
	std::cout << "ERROR: Cannot find '\\end' of '" << section << "' section! \n"; 
	return;
      }

      _user_command_map[usr_cmd] = {usr_cmd, usr_args, commands};
    }
	
    /* '\\include' section */
    else if (section == "\\include")
    {
      if (sect_args.size() == 0)
      {
	std::cout << "ERROR: '\\include': missing filename! \n";
	return;
      }	

      parse_test(sect_args[0]); // recursive parsing
    }
    
    /* Unknown section */
    else std::cout << "WARNING: unknown section '" << section << "'! \n";
  } // end parse file
}



