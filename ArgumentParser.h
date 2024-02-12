#pragma once

#include <iostream>
#include <map>
#include <string>
#include <typeinfo>

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
  bool IsFlagRegistered(const std::string &flag) const;
  bool IsOptionRegistered(const std::string &option) const;
  void RegisterFlag(const std::string &flag);
  void RegisterOption(const std::string &option);
  bool GetFlag(const std::string &flag) const;
  void Parse(int argc, const char *argv[]);

  const std::string &GetOption(const std::string &option) const;

  template <typename T>
  T GetOptionAs(T type, const std::string &option) const {
    if ((typeid(type)) == (typeid(int))) {
      // std::cout << "Returning as int: " << option << std::endl;
      return GetOptionAsInt(option);
    } else if ((typeid(type)) == (typeid(float))) {
      //   std::cout << "Returning as Float :" << option << std::endl;
      return GetOptionAsFloat(option);
    } else {
      return -1;
    }
  };
  // float GetOptionAs<float>(const std::string &option) const;
  // int GetOptionAs<const std::string &>(const std::string &option) const;

private:
  std::map<std::string, bool> m_Flags;
  std::map<std::string, std::string> m_Options;
  float GetOptionAsFloat(const std::string &option) const;
  int GetOptionAsInt(const std::string &option) const;
};
