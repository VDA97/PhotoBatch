#include "ArgumentParser.h"
#include <algorithm> //It is need for using std::count.
#include <array>
#include <filesystem> //substitui o <dirent.h> usando no linux para verificar pastas do pc local
#include <iomanip>
#include <iostream>

// So, for avoiding typing errors, is highly recommended to organize the string arguments.
// This project will organize this string arguments using namespaces.
// This is also used for centralized the infos. For example, it is easy to translate this App for multiple languages
namespace Args {
namespace Flags {
static constexpr const char *Rename = "rename";
static constexpr const char *Convert = "convert";
static constexpr const char *Resize = "resize";
static constexpr const char *Scale = "scale";
static constexpr const char *Help = "help";
} // namespace Flags

namespace FlagsDescription {
static constexpr const char *Rename = "Rename files";
static constexpr const char *Convert = "Convert between png to jpg or vice-versa";
static constexpr const char *Resize = "Change image size";
static constexpr const char *Scale = "        Change scale size";
static constexpr const char *Help = "        Print info regarding Flags and Options";
} // namespace FlagsDescription

namespace Options {
static constexpr const char *Folder = "folder";
static constexpr const char *Filter = "filter";
static constexpr const char *Width = "width";
static constexpr const char *Height = "height";
static constexpr const char *Amount = "amount";
static constexpr const char *Prefix = "prefix";
static constexpr const char *StartNumber = "startnumber";
static constexpr const char *From = "from";
static constexpr const char *To = "to";

} // namespace Options

namespace OptionsDescription {
static constexpr const char *Folder = "Fill with folder path, i.e /home/victoralmeida/teste ";
static constexpr const char *Filter = "Fill with some filter for searching";
static constexpr const char *Width = "Used by Resize Mode";
static constexpr const char *Height = "Used By Height Mode";
static constexpr const char *Amount = "Used by Scale, float, i.e 1.2";
static constexpr const char *Prefix = "Prefix is used for searching too";
static constexpr const char *StartNumber = "Index of image for applying mode";
static constexpr const char *From = "Define the actual extension of image, i.e png or jpg";
static constexpr const char *To = "Define desired extension for converting image, i.e png or jpg";

} // namespace OptionsDescription

} // namespace Args

void PrintHelp() {
  static const std::string name = "PhotoBatch Help";
  static const std::array<std::string, 5> flags = {Args::Flags::Convert,
                                                   Args::Flags::Help,
                                                   Args::Flags::Rename,
                                                   Args::Flags::Resize,
                                                   Args::Flags::Scale};
  static const std::array<std::string, 5> flagsDescription = {Args::FlagsDescription::Convert,
                                                              Args::FlagsDescription::Help,
                                                              Args::FlagsDescription::Rename,
                                                              Args::FlagsDescription::Resize,
                                                              Args::FlagsDescription::Scale};
  static const std::array<std::string, 9> options = {
      Args::Options::Amount,
      Args::Options::Filter,
      Args::Options::Folder,
      Args::Options::From,
      Args::Options::Height,
      Args::Options::Prefix,
      Args::Options::StartNumber,
      Args::Options::To,
      Args::Options::Width,
  }; // const std::array<std::string, 5> descriptions = {}
  static const std::array<std::string, 9> optionsDescription = {
      Args::OptionsDescription::Amount,
      Args::OptionsDescription::Filter,
      Args::OptionsDescription::Folder,
      Args::OptionsDescription::From,
      Args::OptionsDescription::Height,
      Args::OptionsDescription::Prefix,
      Args::OptionsDescription::StartNumber,
      Args::OptionsDescription::To,
      Args::OptionsDescription::Width,
  }; // const std::array<std::string, 5> descriptions = {}

  std::cout << "Usage:" << name << std::endl;
  std::cout << "Modes:\n";
  for (size_t i = 0; i < flags.size(); i++) {
    std::cout << "  " << flags[i] << "\t\t" << flagsDescription[i] << "\n";
  }
  std::cout << "Options:\n";
  for (size_t i = 0; i < options.size(); i++) {
    std::cout << "  " << options[i] << "\t\t" << optionsDescription[i] << "\n";
  }
}

const std::string &GetInvalidChars() {
  static const std::string invalidCharacters = "\\/*\"?<>|"; // Escapar \ para construir esse tipo de string
  return invalidCharacters;
}
bool HasInvalidChars(const std::string &str) {
  const bool bHasInvalidChars = str.find_first_of(GetInvalidChars()) != std::string::npos;
  return bHasInvalidChars;
}
void ValidateArguments(const ArgumentParser &argParser) {
  // Ler as flags que o ArgumentParser identificou
  const bool bHelpMode = argParser.GetFlag(Args::Flags::Help);
  const bool bRenameMode = argParser.GetFlag(Args::Flags::Rename);
  const bool bConvertMode = argParser.GetFlag(Args::Flags::Convert);
  const bool bResizeMode = argParser.GetFlag(Args::Flags::Resize);
  const bool bScaleMode = argParser.GetFlag(Args::Flags::Scale);

  const std::array<bool, 5> modes = {bRenameMode, bConvertMode, bResizeMode, bScaleMode, bHelpMode};
  const std::ptrdiff_t numActiveModes = std::count(std::begin(modes), std::end(modes), true);
  // Verificar se somente um modo do PhotoBatch esta ativo, utilizand o OU EXCLUSIVO "^"
  // if (!(bRenameMode ^ bConvertMode ^ bResizeMode ^ bScaleMode)) {
  if (bHelpMode) {
    PrintHelp();
    return;
  }
  if (numActiveModes != 1) {
    // Se houver mais de um modo ativo, lança uma excessão
    throw std::invalid_argument("Somente um modo pode estar ativo");
  }
  // Verificar se o caminho da pasta é valido, opção folder
  // Note: No linux, so esta funcionando caso seja low case. exemplo: teste ok. TESTE não ok.
  const std::string folder = argParser.GetOption(Args::Options::Folder);
  if (folder.empty()) {
    throw std::invalid_argument("A pasta não pode estar em branco");
  }
  bool exist = std::filesystem::exists(folder);
  // Funcionalidade do c++ 17 -> Filesystem, verificar se a pasta existe no pc. Note, vai funcionar para pastas da maquina virtual apenas.
  if (!exist) {
    std::cout << folder << std::endl;
    throw std::invalid_argument("A pasta não existe");
  }
  // validar se o filtro é um string valida
  const std::string filter = argParser.GetOption(Args::Options::Filter);
  if (!filter.empty() && HasInvalidChars(filter)) {
    throw std::invalid_argument("O filtro não pode conter " + GetInvalidChars());
  }
  // Validar o modo Resize
  if (bResizeMode) {
    int w{0}, h{0}, width{0}, height{0};
    try {
      width = argParser.GetOptionAs(w, Args::Options::Width);
      height = argParser.GetOptionAs(h, Args::Options::Height);

    } catch (const std::exception &e) {
      throw std::invalid_argument("O valor informado para Width e Height não são números válidos");
    }

    if (width <= 0 || height <= 0) {
      throw std::invalid_argument("Width e Height devem ser maiores que 0");
    }

    if (filter.empty()) {
      throw std::invalid_argument("Filter não pode estar em branco no modo Resize");
    }
  }

  if (bScaleMode) {
    float a{0.0f}; // todo refactor this future, because, this is just for GetOptionAs calls GetOptionAsFloat
    float amount{0.0f};
    try {
      amount = argParser.GetOptionAs(a, Args::Options::Amount);
    } catch (const std::exception &e) {
      throw std::invalid_argument("Valor de amount não é um número válido");
    }
    if (amount <= 0.0f) {
      throw std::invalid_argument("Valor de amount não pode ser menor ou igual a 0.0");
    }
    if (filter.empty()) {
      throw std::invalid_argument("Filter não pode estar em branco no modo Scale");
    }
  }
  if (bRenameMode) {
    int s{0}, startNumber = 0;
    try {
      startNumber = argParser.GetOptionAs(s, Args::Options::StartNumber);
    } catch (const std::exception &e) {
      throw std::invalid_argument("StartNumber deve ser um número válido");
    }

    std::string prefix = argParser.GetOption(Args::Options::Prefix);
    if (startNumber < 0) {
      throw std::invalid_argument("Start number deve ser igual ou maior que 0");
    }
    if (prefix.empty() || HasInvalidChars(prefix)) {
      throw std::invalid_argument("Prefixo não pode estar em branco e não pode conter os caracteres: " + GetInvalidChars());
    }
  }

  if (bConvertMode) {
    const std::string from = argParser.GetOption(Args::Options::From);
    const std::string to = argParser.GetOption(Args::Options::To);
    const std::array<std::string, 2> convertOptions = {"jpg", "png"}; // busca linear, pois são somente duas opçõe
    const bool bIsFromValid = std::find(std::begin(convertOptions), std::end(convertOptions), from) != std::end(convertOptions);
    const bool bIsToValid = std::find(std::begin(convertOptions), std::end(convertOptions), to) != std::end(convertOptions);
    if (!bIsFromValid || !bIsToValid) {
      throw std::invalid_argument("From e To devem ser jpg ou png");
    }

    if (from == to) {
      throw std::invalid_argument("From e to devem ser diferentes");
    }
  }
};

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "pt_BR");
  setlocale(LC_NUMERIC, "en_US");
  ArgumentParser argParser;
  argParser.RegisterFlag(Args::Flags::Rename);
  argParser.RegisterFlag(Args::Flags::Convert);
  argParser.RegisterFlag(Args::Flags::Resize);
  argParser.RegisterFlag(Args::Flags::Scale);
  argParser.RegisterFlag(Args::Flags::Help);
  // Registro de opções
  argParser.RegisterOption(Args::Options::Folder);
  argParser.RegisterOption(Args::Options::Filter);
  argParser.RegisterOption(Args::Options::Height);
  argParser.RegisterOption(Args::Options::Amount);
  argParser.RegisterOption(Args::Options::Prefix);
  argParser.RegisterOption(Args::Options::StartNumber);
  argParser.RegisterOption(Args::Options::From);
  argParser.RegisterOption(Args::Options::To);
  argParser.Parse(argc, argv);

  try {
    ValidateArguments(argParser);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}

// Flags e opções
// Prefixo
// flags: --Rename
// Opções:--Folder="C:\Users\Pictures"

// Argumentos do PhotoBatch
/*
# Modo Renomear
--Rename
--Folder="C:\Users\Pictures"
--Filter=".jpg"
--Prefix ="img_"
--StartNumber=1

# Modo Renomear
--Convert
--Folder="C:\Users\Pictures"
--Filter=".jpg"
--From=jpg
--To=png

# Redimensionar
--Resize
--Folder="C:\Users\Pictures"
--Filter=".jpg"
--Width=1024
--Height=1024

# Modo Escalar
--Scale
--Folder="C:\Users\Pictures"
--Filter=".png"
--Amount=0.5

Mode: string
  Rename
    Prefix: string
    StartNumber int
  Convert
    From: string
    To: string
  Resize
    Width: int
    Height: int
  Scale
    Amount : float
Folder: string
Filter: string

Todas as string são canse-insensitive, aplicar o std::tolower
Mode: "Rename", "Convert", "Resize", "Scale"
Prefix: Não pode conter caracteres inválidos em nomes de arquivos
\/*?<>
From: png, jpg
To: png, jpg
Width: std::stoi, deve ser >0
Height: std::stoi, deve ser >0
Amount:std::stof, deve ser >0
StartNumber: std::stoi, >=0
Folder: Deve ser uma página que existe e não pode conter caracteres inválidos
Filtro: Não pode conter caracteres inválidos em nomes de arquivos
\/*?<>



*/