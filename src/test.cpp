#include "test.h"

/***********/
/* details */
/***********/
namespace detail
{
  // tokenize line
  auto tokens = [] (std::string line, const char delim = ' ') 
  {
    line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());

    std::vector<std::string> tokens;
    std::string token;
    std::istringstream iss(line);

    while (std::getline(iss, token, delim)) tokens.push_back(token);

    return tokens;
  };

  // get tag
  auto get_tag = [] (const std::vector<std::string> &tokens)
  {
    return (tokens.size() != 0 ? tokens.front() : "");
  };

  // get args
  auto get_args = [] (const std::vector<std::string> &tokens)
  {
    auto args = tokens;
    if (tokens.size() > 1) args.erase(args.begin());
    else args = {};

    return args;
  };
};

/************************/
/* get command function */
/************************/
std::function<void()> Test::get_cmd_function(const std::string name, const std::vector<std::string> &args)
{
  std::function<void()> function = {};

  /* '\do_action' */
  if (name == "\\do_action")
  {
    auto action = detail::get_tag(args);
    auto act_args = detail::get_args(args);

    if (action.empty()) 
    {
      std::cout << "WARNING: empty action!" << std::endl;
      return {};
    }

    // insert_pds
    if (action == "insert_pds") function = std::bind(&FunctionStore::insert_pds, &_fun_store); 

    // set_thrust
    else if (action == "set_thrust") 
    {
      if (act_args.size() == 1) function = std::bind(&FunctionStore::set_thrust, &_fun_store, act_args[0]);
      else std::cout << "WARNING: '\\set_thrust': missing value! \n";
    }

    else std::cout << "WARNING: action '" << action << "' not known! \n";
  } // end \do_action

  /* '\check' */
  else if (name == "\\check")
  {
    if (args.size() == 2) function = std::bind(&FunctionStore::check, &_fun_store, args[0], args[1]);
    else std::cout << "WARNING: '\\check': missing arguments! \n";
  }

  /* user function */
  else if (_user_cmd_functions.count(name) > 0)
  {
    function = _user_cmd_functions[name];
  }

  // Parse other stuff...
  else std::cout << "WARNING: function '" << name << "' not found! \n";

  return function;
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
    auto tokens = detail::tokens(line);

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

    auto section = detail::get_tag( detail::tokens(line) );
    auto sect_args = detail::get_args( detail::tokens(line) );

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
	
	auto cmd = detail::get_tag( detail::tokens(line) );
	auto cmd_args = detail::get_args( detail::tokens(line) );
	auto fun = get_cmd_function(cmd, cmd_args);

	if (fun) _functions.push_back(fun);
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

      bool end_section_found = false;
      auto usr_cmd = "\\" + sect_args[0];

      auto usr_args = detail::get_args(sect_args);
      for (auto arg : usr_args) 
      {
	if (arg.front() != '_') 
	{
	  std::cout << "[ERROR] '\\define_cmd': command argument '" << arg << "' should begin with '_'! \n"; 
	  return;
	}
      }

      std::vector<std::function<void()>> cmd_functions = {};

      /* Parse body */ 
      while (std::getline(file, line))
      {
	end_section_found = is_end_section(line); 

	if (end_section_found) break;
	if (is_comment(line)) continue;
	if (is_not_indented(line)) return;

	auto cmd = detail::get_tag( detail::tokens(line) );
	auto cmd_args = detail::get_args( detail::tokens(line) );
	auto fun = get_cmd_function(cmd, cmd_args);

	if (fun) cmd_functions.push_back(fun);
      } 
      
      if (!end_section_found) 
      {
	std::cout << "ERROR: Cannot find '\\end' of '" << section << "' section! \n"; 
	return;
      }
      
      auto usr_cmd_function = [cmd_functions]() { for (auto f : cmd_functions) f(); }; // merge all functions
      _user_cmd_functions[usr_cmd] = usr_cmd_function;
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



