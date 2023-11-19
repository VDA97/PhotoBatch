#pragma once

#include <map>
#include <string>

/**
 * Classe usada para ler argumentos da linha de comando
 * Exemplo:
 *  ArgumentParser argParser;
 *  argParser.RegisterFlag("rename");
 *  argParser.Parse(arc, argv);
 *  argParser.GetFlag("rename");
 */

class ArgumentParser {
public:
  void RegisterFlag(const std::string &flag);
  bool GetFlag(const std::string &flag) const;
  void Parse(int argc, char *argv[]);

private:
  std::map<std::string, bool> m_Flags;
};
