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
  void RegisterOption(const std::string &option);
  bool GetFlag(const std::string &flag) const;
  const std::string &GetOption(const std::string &option) const;
  float GetOptionAsFloat(const std::string &option) const;
  int GetOptionAsInt(const std::string &option) const;
  void Parse(int argc, char *argv[]);

private:
  std::map<std::string, bool> m_Flags;
  std::map<std::string, std::string> m_Options;
};
