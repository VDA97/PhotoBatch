#include "ArgumentParser.h"
#include "Mode.h"
#include <iomanip>
#include <iostream>

// So, for avoiding typing errors, is highly recommended to organize the string arguments.
// This project will organize this string arguments using namespaces.
// This is also used for centralized the infos. For example, it is easy to translate this App for multiple languages
int main(int argc, const char *argv[])
{
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

  try
  {
    if (std::unique_ptr<Mode> PhotoBatchMode = CreateMode(argParser))
    {
      PhotoBatchMode->Run();
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }

  // Mode m;

  // return 0;
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