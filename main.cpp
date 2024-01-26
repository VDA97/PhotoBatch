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
} // namespace Flags

namespace Options {
static constexpr const char *Folder = "folder";
static constexpr const char *Filter = "filter";
static constexpr const char *Width = "width";
static constexpr const char *Height = "height";
} // namespace Options
} // namespace Args

void ValidateArguments(const ArgumentParser &argParser) {
  // Ler as flags que o ArgumentParser identificou
  const bool bRenameMood = argParser.GetFlag(Args::Flags::Rename);
  const bool bConvertMood = argParser.GetFlag(Args::Flags::Convert);
  const bool bResizeMood = argParser.GetFlag(Args::Flags::Resize);
  const bool bScaleMood = argParser.GetFlag(Args::Flags::Scale);

  const std::array<bool, 4> modes = {bRenameMood, bConvertMood, bResizeMood, bScaleMood};
  const std::ptrdiff_t numActiveModes = std::count(std::begin(modes), std::end(modes), true);
  // Verificar se somente um modo do PhotoBatch esta ativo, utilizand o OU EXCLUSIVO "^"
  // if (!(bRenameMood ^ bConvertMood ^ bResizeMood ^ bScaleMood)) {
  if (numActiveModes != 1) {
    // Se houver mais de um modo ativo, lança uma excessão
    throw std::invalid_argument("Somente um modo pode estar ativo");
  }
  // Verificar se o caminho da pasta é valido, opção folder
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
  if (!filter.empty()) {
    const std::string invalidCharacters = "\\/*\"?<>|"; // Escapar \ para construir esse tipo de string
    if (filter.find_first_of(invalidCharacters) != std::string::npos) {
      throw std::invalid_argument("O filtro não pode conter " + invalidCharacters);
    }
  }
  // Validar o modo Resize
  if (bResizeMood) {
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
};

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "pt_BR");
  setlocale(LC_NUMERIC, "en_US");
  ArgumentParser argParser;
  argParser.RegisterFlag(Args::Flags::Rename);
  argParser.RegisterFlag(Args::Flags::Convert);
  argParser.RegisterFlag(Args::Flags::Resize);
  argParser.RegisterFlag(Args::Flags::Scale);
  argParser.RegisterOption(Args::Options::Folder);
  argParser.RegisterOption(Args::Options::Filter);
  argParser.RegisterOption(Args::Options::Height);
  argParser.RegisterOption(Args::Options::Width);

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