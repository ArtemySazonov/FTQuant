/** @file SyntaxParser.cpp
 * This file contains the implementation of the SyntaxParser methods.
 * @brief Implementation of SyntaxParser class.
 * @see SyntaxParser
 * @see tests/u_SyntaxParser.cpp
 */

#include "SyntaxParser.hpp"

bool is_double(const std::string& str) {
  std::istringstream iss(str);
  double d;
  iss >> std::noskipws >> d;
  return iss.eof() && !iss.fail();
}

std::string Command::to_json() const {
  std::string json = "{";
  json += "code: " + std::to_string(_code) + ", " + "keyNumbers: ";
  for (const auto& [key, value] : _key_numbers)
    json += '[' + key + "] = " + value + "; ";
  json += '}';

  return json;
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
      if (is_double(w) || temp == "FILE")
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

std::vector<double> readStocksFromFile(std::string fname) {
  std::string line;
  std::ifstream in(fname);
  std::vector<double> ans;
  if (in.is_open()) {
    while (std::getline(in, line)) {
      if (is_double(line))
        ans.push_back(std::stod(line));
      else {
        ans.push_back(-1);
        in.close();
        return ans;
      }
    }
  } else {
    ans.push_back(-1);
    in.close();
    return ans;
  }
  in.close();
  return ans;
}

std::vector<std::vector<double>> read2DFromFile(std::string fname) {
  std::string line;
  std::ifstream in(fname);
  std::vector<std::vector<double>> ans;
  if (in.is_open()) {
    while (std::getline(in, line)) {
      std::stringstream ss(line);
      std::vector<double> numbers;
      std::string word;

      int flag = 0;

      while (ss >> word) {
        if (is_double(word)) {
          numbers.push_back(std::stod(word));
        } else {
          numbers.push_back(-1);
        }
      }
      ans.push_back(numbers);
    }
  } else {
    std::vector<double> num{-1};
    ans.push_back(num);
    in.close();
    return ans;
  }
  in.close();
  return ans;
}

int Execution::execute(Command C) {
  double r = 0.;
  double sigma = 1.;

  traj_generated = 0;
  isBS = 1;

  std::vector<std::vector<double>> w;
  std::vector<double> T;
  std::vector<double> y;

  std::vector<std::vector<double>> trajs;

  BlackScholes BSmodel(r, sigma);
  switch (C._code) {
    case INVALID_COMMAND: {
      std::cout << "Invalid"
                << " \n";
      return -1;
      break;
    }

    case BLACK_SCHOLES: {
      // std::cout << "BlackScholes model(" << C._key_numbers.at("INTEREST_RATE") << " \n";
      BSmodel = BlackScholes(std::stod(C._key_numbers.at("INTEREST_RATE")),
                             std::stod(C._key_numbers.at("SIGMA")));
      isBS = 1;
      // std::cout << "BlackScholes model(" << _key_numbers.at("INTEREST_RATE"] << " \n";
      //  "_key_numbers.at(\"SIGMA\"]); \n";
      break;
    }

    case BLACK_SCHOLES_F: {
      // std::cout << "BlackScholes model(" << C._key_numbers.at("FILE") << " \n";
      auto stocks = readStocksFromFile(C._key_numbers.at("FILE"));
      for (auto& s : stocks) {
        if (s < 0)
          return -1;
      }
      BSmodel.calibrate(stocks);

      isBS = 1;
      // std::cout << "BlackScholes model(_key_numbers.at(\"INTEREST_RATE\"], "
      //              "_key_numbers.at(\"SIGMA\"]); \n";
      break;
    }

    case LOCVOL: {
      // std::cout << "LOCVOL model(" << C._key_numbers.at("FILE_T") << " \n";
      T = readStocksFromFile(C._key_numbers.at("FILE_T"));
      for (auto& s : T) {
        if (s < 0)
          return -1;
      }

      y = readStocksFromFile(C._key_numbers.at("FILE_y"));
      for (auto& s : y) {
        if (s < 0)
          return -1;
      }

      w = read2DFromFile(C._key_numbers.at("FILE_w"));
      for (auto& s1 : w) {
        for (auto& s2 : s1) {
          if (s2 < 0)
            return -1;
        }
      }

      LVmodel.calibrate_dupire(w, T, y,
                               std::stod(C._key_numbers.at("SPOT_PRICE")));
      isBS = 0;
      break;
    }

    case GENERATE_TRAJECTORIES: {
      if (isBS) {
        // std::cout << "BS Tr num, steps: " << std::stod(C._key_numbers.at("TRAJECTORIES_NUMBER")) << std::stod(C._key_numbers.at("STEPS_NUMBER")) << std::endl;
        traj = BSmodel.generate_paths(
            std::stod(C._key_numbers.at("TRAJECTORIES_NUMBER")),
            std::stod(C._key_numbers.at("SPOT_PRICE")),
            std::stod(C._key_numbers.at("STEPS_NUMBER")),
            std::stod(C._key_numbers.at("EXP_T")));
        traj_generated = 1;
      } else {
        // std::cout << "LV Tr num, steps: " << std::stod(C._key_numbers.at("TRAJECTORIES_NUMBER")) << std::stod(C._key_numbers.at("STEPS_NUMBER")) << std::endl;
        traj = LVmodel.generate_paths(
            std::stod(C._key_numbers.at("TRAJECTORIES_NUMBER")),
            std::stod(C._key_numbers.at("STEPS_NUMBER")),
            std::stod(C._key_numbers.at("EXP_T")),
            std::stod(C._key_numbers.at("SPOT_PRICE")));
        traj_generated = 1;
      }
      break;
    }
    default: {
      return -1;
      break;
    }
  }
  return 0;
}

std::ostream& operator<<(std::ostream& os, const Command& C) {
  std::cout << "code: " << C._code << std::endl;
  std::cout << "keyNumbers: ";
  for (const auto& [key, value] : C._key_numbers)
    std::cout << '[' << key << "] = " << value << "; ";

  std::cout << std::endl;

  return os;
}