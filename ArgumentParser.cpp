#include "ArgumentParser.h"
#include "Utils.h"

bool ArgumentParser::IsFlagRegistered(const std::string &flag) const
{
  if (!flag.empty())
  {
    return m_Flags.count(flag) == 1;
  }
  return false;
}

bool ArgumentParser::IsOptionRegistered(const std::string &option) const
{
  if (!option.empty())
  {
    return m_Options.count(option) == 1;
  }
  return false;
}

void ArgumentParser::RegisterFlag(const std::string &flag)
{

  if (!flag.empty() && !Utils::HasWhiteSpace(flag))
  {
    m_Flags[flag] = false;
  }
}

void ArgumentParser::RegisterOption(const std::string &option)
{

  if (!option.empty() && !Utils::HasWhiteSpace(option))
  {
    m_Options[option] = "";
  }
}

bool ArgumentParser::GetFlag(const std::string &flag) const
{

  if (!flag.empty())
  {
    auto flagIt = m_Flags.find(flag);
    if (flagIt != std::end(m_Flags))
    {
      return flagIt->second;
    }
  }
  return false;
}

const std::string &ArgumentParser::GetOption(const std::string &option) const
{
  if (!option.empty())
  {
    auto optionIt = m_Options.find(option);
    if (optionIt != std::end(m_Options))
    {
      return optionIt->second;
    }
  }
  static std::string EmptyOption = "";
  return EmptyOption;
}

float ArgumentParser::GetOptionAsFloat(const std::string &option) const
{
  const std::string &optionValue = GetOption(option);
  if (!optionValue.empty())
  {
    return std::stof(optionValue);
  }
  return -1;
}

int ArgumentParser::GetOptionAsInt(const std::string &option) const
{
  const std::string &optionValue = GetOption(option);
  if (!optionValue.empty())
  {
    return std::stoi(optionValue);
  }
  return -1;
}

void ArgumentParser::Parse(int argc, const char *argv[])
{
  if (argc > 1 && argv != nullptr)
  {
    for (int i = 1; i < argc; ++i)
    {

      std::string arg = Utils::ToLower(argv[i]);
      // Argumentos que ter pelo menos 3 caracters, --n
      if (arg.length() >= 3)
      {
        if (arg[0] == '-' && arg[1] == '-')
        {
          //--rename, remove o sufixo --
          arg = arg.substr(2);
          if (arg.find_first_of('=') != std::string::npos)
          {
            // isso é uma opção
            const size_t equalSignPos = arg.find('=');
            if (equalSignPos != std::string::npos)
            {
              // Dividir a opção em chave e valor
              std::string optionName = arg.substr(0, equalSignPos);
              std::string optionValue = arg.substr(equalSignPos + 1);
              auto optionIt = m_Options.find(optionName);
              if (optionIt != std::end(m_Options))
              {
                // Achamos uma opção registrada
                optionIt->second = optionValue;
              }
            }
          }
          else
          {
            // é uma flag
            auto flagIt = m_Flags.find(arg);
            if (flagIt != std::end(m_Flags))
            {
              // é uma flag registrada
              flagIt->second = true;
            }
          }
        }
      }
    }
  }
}
