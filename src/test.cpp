#include "test.h"

/************/
/* Run test */
/************/
void Test::run()
{
  print_banner("Run test!");

  for (auto cmd : _commands)
  {
    std::cout << "\nRunning test: '" << cmd->get_name() << "'\n";
    cmd->run();
  }
  
  print_banner("Test done!");
}


/****************/
/* Print banner */
/****************/
void Test::print_banner(const std::string str, size_t length)
{
  if (length == 0) length = str.size() + 10;

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

    return line.empty() ||   // skip empty line
           (line.front() == '#' || line.front() == '"'); // skip line with comment
  };

  // utility function: check if line is indented
  auto check_formatting = [&](std::string line)
  {
    auto num_tabs = std::count(line.begin(), line.end(), '\t');
    line.erase(std::remove(line.begin(), line.end(), '\t'), line.end()); // remove tabs

    bool bad_line = (line != "\\end") && !is_comment(line) && ((num_tabs != 1) || (line.front() != '\\')); 

    if (bad_line) 
    {
      if (!num_tabs) throw Error("Line '" + line + "' not indented (tab missing)!");
      else if (num_tabs > 1) throw Error("Line '" + line + "' not indented (" + std::to_string(num_tabs) + " tabs)!");
      else if (line.front() != '\\') throw Error("Line '" + line + "' should begin with '\\'!");
    }
  };

  // utility function: check end section in line
  auto is_end_section = [] (const std::string line)
  {
    auto tokens = Utils::tokens(line);

    return tokens.size() == 1 && tokens.front() == "\\end";
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

    /* '\begin_step' section */
    if (section == "\\begin_step")
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
	  cmd->set_args(cmd_args);
	  _commands.push_back(cmd);
	}
      } 

      if (!end_section_found) 
      {
	throw Error("Cannot find '\\end' of '" + section + "' section!"); 
      }
    }

    /* '\define_cmd' section */
    else if (section ==  "\\define_cmd")
    {
      if (sect_args.size() == 0)
      {
	throw Error("'\\define_cmd': missing command name!");
      }

      auto usr_cmd_name = Utils::first(sect_args);
      auto placeholders = Utils::remove_first(sect_args);

      if (_user_command_map.count("\\" + usr_cmd_name) > 0)
      {
        throw Error("'\\define_cmd': user command '" + usr_cmd_name + "' already defined!"); 
      }

      for (auto &p : placeholders) 
      {
	if (!Placeholder::is_placeholder(p)) 
	{
	  throw Error("'\\define_cmd': placeholder '" + p + "' should begin with '_'!"); 
	}
      }

      bool end_section_found = false;
      std::vector<std::shared_ptr<Command>> commands = {};

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
	  cmd->set_args(cmd_args);
	  commands.push_back(cmd);
	}
      } 
      
      if (!end_section_found) 
      {
	throw Error("Cannot find '\\end' of '" + section + "' section!"); 
      }

      _user_command_map["\\" + usr_cmd_name] = {usr_cmd_name, placeholders, commands};
    }
	
    /* '\\include' section */
    else if (section == "\\include")
    {
      if (sect_args.size() == 0)
      {
	throw Error("\\include': missing filename!");
      }	

      parse_test(sect_args[0]); // recursive parsing
    }
    
    /* Unknown section */
    else 
    {
      throw Error("Unknown section '" + section + "'!");
    }	
  } // end parse file
}



