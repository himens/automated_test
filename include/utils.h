#ifndef UTILS_H
#define UTILS_H

/***********/
/* Utils */
/***********/
namespace utils
{
  // convert string to digit
  auto is_digit = [] (const std::string str)
  {
    return !str.empty() && (str.find_first_not_of("0123456789.") == std::string::npos);
  }; 

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

  // get first token
  auto first = [] (const std::vector<std::string> &tokens)
  {
    return (tokens.size() != 0 ? tokens.front() : "");
  };

  // get all except first token
  auto remove_first = [] (const std::vector<std::string> &tokens)
  {
    auto args = tokens;
    if (tokens.size() > 1) args.erase(args.begin());
    else args = {};

    return args;
  };
};
#endif
