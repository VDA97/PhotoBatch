#include <iomanip>
#include <iostream>

#include "ArgumentParser.h"

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "pt_BR");
  setlocale(LC_NUMERIC, "en_US");
  ArgumentParser argParser;
  argParser.RegisterFlag("rename");
  argParser.RegisterFlag("convert");
  argParser.RegisterFlag("resize");
  argParser.RegisterFlag("scale");
  argParser.RegisterOption("folder");
  argParser.RegisterOption("amount");

  argParser.Parse(argc, argv);

  std::cout << std::boolalpha << "Rename  :" << argParser.GetFlag("rename") << std::endl;
  std::cout << std::boolalpha << "Convert :" << argParser.GetFlag("convert") << std::endl;
  std::cout << std::boolalpha << "Resize  :" << argParser.GetFlag("resize") << std::endl;
  std::cout << std::boolalpha << "Scale   :" << argParser.GetFlag("scale") << std::endl;
  std::cout << std::boolalpha << "Empty   :" << argParser.GetFlag("empty") << std::endl;
  std::cout << "Folder   :" << argParser.GetOption("folder") << std::endl;
  std::cout << "Amount (float)   :" << argParser.GetOptionAsFloat("amount") << std::endl;
  std::cout << "Amount (int)   :" << argParser.GetOptionAsInt("amount") << std::endl;
  // std::cout << "Amount   :" << argParser.GetOption("amount") << std::endl;
  // std::cout << "Amount   :" << argParser.GetOption("amount") << std::endl;

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