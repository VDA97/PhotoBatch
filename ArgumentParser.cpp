#include "ArgumentParser.h"
#include <algorithm>

std::string ToLower(std::string str) {
  std::transform(std::begin(str), std::end(str), std::begin(str),
                 [](unsigned char c) { return std::tolower(c); });
  return str;
}

void ArgumentParser::RegisterFlag(const std::string &flag) {

  if (!flag.empty()) {
    m_Flags[flag] = false;
  }
}

bool ArgumentParser::GetFlag(const std::string &flag) const {

  if (!flag.empty()) {
    auto flagIt = m_Flags.find(flag);
    if (flagIt != std::end(m_Flags)) {
      return flagIt->second;
    }
  }
  return false;
}

void ArgumentParser::Parse(int argc, char *argv[]) {
  if (argc > 1) {
    for (int i = 1; i < argc; ++i) {

      std::string arg = ToLower(argv[i]);
      // Argumentos que ter pelo menos 3 caracters, --n
      if (arg.length() >= 3) {
        if (arg[0] == '-' && arg[1] == '-') {
          //--rename, remove o sufixo --
          arg = arg.substr(2);
          if (arg.find_first_of('=') != std::string::npos) {

          } else {
            // é uma flag
            auto flagIt = m_Flags.find(arg);
            if (flagIt != std::end(m_Flags)) {
              // é uma flag registrada
              flagIt->second = true;
            }
          }
        }
      }
    }
  }
}
