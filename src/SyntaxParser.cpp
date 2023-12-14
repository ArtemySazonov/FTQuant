/** @file SyntaxParser.cpp
 * This file contains the implementation of the SyntaxParser methods.
 * @brief Implementation of SyntaxParser class.
 * @see SyntaxParser
 * @see tests/u_SyntaxParser.cpp
 */

#ifndef FTQUANT_SYNTAXPARSER_CPP
#define FTQUANT_SYNTAXPARSER_CPP

#include <SyntaxParser.hpp>

bool isDouble(const std::string& str) {
  std::istringstream iss(str);
  double d;
  iss >> std::noskipws >> d;
  return iss.eof() && !iss.fail();
}

Command::Command(std::string com) {
  std::stringstream ss(com);
  std::vector<std::string> words;

  std::string temp;

  int flag = 0;
  std::string word;
  ss >> word;
  if (Commands.count(word)) {
    _code = Commands[word];
  } else {
    _code = Commands["INVALID_COMMAND"];
    return;
  }

  while (ss >> word) {
    words.push_back(word);
  }

  for (const auto& w : words) {
    if (Fields.count(w)) {
      if (!flag) {
        _key_numbers[w] = "0";
        flag = 1;
        temp = w;
      }
      continue;
    }

    if (flag == 1) {
      if (isDouble(w) || temp == "FILE")
        _key_numbers[temp] = w;
      else {
        // std::cout << "cant convert " << w << " in double \n";
        _code = INVALID_COMMAND;
        return;
      }
      flag = 0;
    }
  }

  for (const auto& w : RequiredFields[_code]) {
    if (!_key_numbers.count(w)) {
      _code = INVALID_COMMAND;
    }
  }
}

int Command::code() const {
  return _code;
}

std::vector<double> readStocksFromFile(std::string fname)
{
  std::string line;
  std::ifstream in(fname); 
  std::vector<double> ans;
    if (in.is_open())
    {
        while (std::getline(in, line))
        {
          if(isDouble(line))
            ans.push_back(std::stod(line));
          else {
            ans.push_back(-1);
            in.close();
            return ans;
          }
        }
    }
    else {
      ans.push_back(-1);
      in.close();
      return ans;
      }
    in.close();
    return ans;
}

std::vector<std::vector<double>> read2DFromFile(std::string fname)
{
    std::string line;
    std::ifstream in(fname);
    std::vector<std::vector<double>> ans;
    if (in.is_open())
    {
        while (std::getline(in, line))
        {
            std::stringstream ss(line);
            std::vector<double> numbers;
            std::string word;

            int flag = 0;

              while (ss >> word) {
                if(isDouble(word)){
                    numbers.push_back(std::stod(word));
                }
                else {
                    numbers.push_back(-1);
                }
              }
            ans.push_back(numbers);
        }
    }
    else
    {
        std::vector<double> num{-1};
        ans.push_back(num);
        in.close();
        return ans;
    }
    in.close();
    return ans;
}

int Command::execute() const {
  double r = 0;
  double sigma = 1;
  int trajectories_generated = 0;
  int isBS = 1;
  // bool antith = true;
  std::vector<std::vector<double>> traj;
  std::vector<std::vector<double>> stocks;

  std::vector<std::vector<double>> w;
  std::vector<double> T;
  std::vector<double> y;

  double simple(double x, double y){ return 0; }

  BlackScholes BSmodel(r, sigma);
  LocalVolatility LVmodel(r, simple);
  switch (_code) {
    case INVALID_COMMAND:
      return -1;
      break;

    case BLACK_SCHOLES:
      BSmodel = BlackScholes(std::stod(_key_numbers["INTEREST_RATE"]), std::stod(_key_numbers["SIGMA"]));
      isBS = 1;
      // std::cout << "BlackScholes model(_key_numbers[\"INTEREST_RATE\"], "
      //              "_key_numbers[\"SIGMA\"]); \n";
      break;

    case BLACK_SCHOLES_F:
      stocks = readStocksFromFile(_key_numbers["FILE"]);
      for (auto & s : stocks){
        if (s < 0) return -1;
      }
      BSmodel.calibrate(stocks);

      isBS = 1;
      // std::cout << "BlackScholes model(_key_numbers[\"INTEREST_RATE\"], "
      //              "_key_numbers[\"SIGMA\"]); \n";
      break;

    case LOCVOL:

      T = readStocksFromFile(_key_numbers["FILE_T"]);
      for (auto & s : T){
        if (s < 0) return -1;
      }

      y = readStocksFromFile(_key_numbers["FILE_y"]);
      for (auto & s : y){
        if (s < 0) return -1;
      }

      w = read2DFromFile(_key_numbers["FILE_w"]);
      for (auto & s1 : w){
        for (auto & s2 : w){
          if (s2 < 0) return -1;
      }
      }

      LVmodel.calibrate_dupire(w, T, y, std::stod(_key_numbers["SPOT_PRICE"]));
      isBS = 0;
      break;

    case GENERATE_TRAJECTORIES:
    if(isBS) {
      traj = BSmodel.generate_paths(
            std::stod(_key_numbers["TRAJECTORIES_NUMBER"]), 
            std::stod(_key_numbers["SPOT_PRICE"]), 
            std::stod(_key_numbers["STEPS_NUMBER"]), 
            std::stod(_key_numbers["EXP_T"]));
        trajectories_generated = 1;
      }
      else {
        traj = LVmodel.generate_paths(
            std::stod(_key_numbers["TRAJECTORIES_NUMBER"]), 
            std::stod(_key_numbers["STEPS_NUMBER"]), 
            std::stod(_key_numbers["EXP_T"]), 
            std::stod(_key_numbers["SPOT_PRICE"]), 
        )
      }
      break;

    case EURO_PUT:
      std::cout << "EuroPut(_key_numbers[\"STOCK_PRICE\"], "
                   "_key_numbers[\"STRIKE\"]) \n";
      if (!trajectories_generated) {
        std::cout << "model.generateTrajectories(100, 1000)\n";
      }
      std::cout << "EuroPut.price(traj) \n";
      break;

    case EURO_CALL:
      std::cout << "EuroPut(_key_numbers[\"STOCK_PRICE\"], "
                   "_key_numbers[\"STRIKE\"]) \n";
      if (!trajectories_generated) {
        std::cout << "model.generateTrajectories(100, 1000)\n";
      }
      std::cout << "EuroPut.price(traj) \n";
      break;

    default:
      return -1;
      break;
  }
  return 0;
}

std::string Command::to_json() const {
  std::string json = "{";
  json += "code: " + std::to_string(_code) + ", " + "keyNumbers: ";
  for (const auto& [key, value] : _key_numbers)
    json += '[' + key + "] = " + value + "; ";

  json += '}';

  return json;
}

std::ostream& operator<<(std::ostream& os, const Command& C) {
  std::cout << "code: " << C._code << std::endl;
  std::cout << "keyNumbers: ";
  for (const auto& [key, value] : C._key_numbers)
    std::cout << '[' << key << "] = " << value << "; ";

  std::cout << std::endl;

  return os;
}

#endif  //FTQUANT_SYNTAXPARSER_HPP
