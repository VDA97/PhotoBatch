#include <algorithm> //It is need for using std::count.
#include <array>
#include <iomanip>
#include <iostream>

#include "ArgumentParser.h"

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
};

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "pt_BR");
  setlocale(LC_NUMERIC, "en_US");
  ArgumentParser argParser;
  argParser.RegisterFlag(Args::Flags::Rename);
  argParser.RegisterFlag(Args::Flags::Convert);
  argParser.RegisterFlag(Args::Flags::Resize);
  argParser.RegisterFlag(Args::Flags::Scale);
  argParser.RegisterOption("folder");
  argParser.RegisterOption("amount");

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