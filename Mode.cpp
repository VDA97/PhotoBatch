#include "Mode.h"
#include "RenameMode.h"
#include "ConvertMode.h"
#include <iostream>
#include "ArgumentParser.h"
#include <algorithm> //It is need for using std::count.
#include <array>
#include <filesystem> //substitui o <dirent.h> usando no linux para verificar pastas do pc local
#include <chrono>

const std::string &Mode::GetFilter() const
{
    return m_Filter;
}

const std::string &Mode::GetFolder() const
{
    return m_Folder;
}

void Mode::Run()
{
    using ClockT = std::chrono::high_resolution_clock;
    // This method will be used to check how long the operation took.
    ClockT::time_point startTime = ClockT::now();
    RunImpl(); // Read time in this point //Calculate the difference
    ClockT::time_point endTime = ClockT::now();
    ClockT::duration elapsedTime = endTime - startTime;
    std::chrono::milliseconds elapsedTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime);

    std::cout << GetModeName() << "Process finished in : " << elapsedTimeMs.count() << "ms" << std::endl;
}

const std::string &GetInvalidChars()
{
    static const std::string invalidCharacters = "\\/*\"?<>|"; // Escapar \ para construir esse tipo de string
    return invalidCharacters;
}
bool HasInvalidChars(const std::string &str)
{
    const bool bHasInvalidChars = str.find_first_of(GetInvalidChars()) != std::string::npos;
    return bHasInvalidChars;
}

void PrintHelp()
{
    static const std::string help = "Todo Help information";
    std::cout << help << std::endl;
}

std::unique_ptr<Mode> CreateMode(const ArgumentParser &argParser)
{
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
    if (bHelpMode)
    {
        PrintHelp();
        return nullptr;
    }
    if (numActiveModes != 1)
    {
        // Se houver mais de um modo ativo, lança uma excessão
        throw std::invalid_argument("Somente um modo pode estar ativo");
    }
    // Verificar se o caminho da pasta é valido, opção folder
    // Note: No linux, so esta funcionando caso seja low case. exemplo: teste ok. TESTE não ok.
    const std::string folder = argParser.GetOption(Args::Options::Folder);
    if (folder.empty())
    {
        throw std::invalid_argument("A pasta não pode estar em branco");
    }
    bool exist = std::filesystem::exists(folder);
    // Funcionalidade do c++ 17 -> Filesystem, verificar se a pasta existe no pc. Note, vai funcionar para pastas da maquina virtual apenas.
    if (!exist)
    {
        std::cout << folder << std::endl;
        throw std::invalid_argument("A pasta não existe");
    }
    // validar se o filtro é um string valida
    const std::string filter = argParser.GetOption(Args::Options::Filter);
    if (!filter.empty() && HasInvalidChars(filter))
    {
        throw std::invalid_argument("O filtro não pode conter " + GetInvalidChars());
    }
    // Validar o modo Resize
    if (bResizeMode)
    {
        int w{0}, h{0}, width{0}, height{0};
        try
        {
            width = argParser.GetOptionAs(w, Args::Options::Width);
            height = argParser.GetOptionAs(h, Args::Options::Height);
        }
        catch (const std::exception &e)
        {
            throw std::invalid_argument("O valor informado para Width e Height não são números válidos");
        }

        if (width <= 0 || height <= 0)
        {
            throw std::invalid_argument("Width e Height devem ser maiores que 0");
        }

        if (filter.empty())
        {
            throw std::invalid_argument("Filter não pode estar em branco no modo Resize");
        }
    }

    if (bScaleMode)
    {
        float a{0.0f}; // todo refactor this future, because, this is just for GetOptionAs calls GetOptionAsFloat
        float amount{0.0f};
        try
        {
            amount = argParser.GetOptionAs(a, Args::Options::Amount);
        }
        catch (const std::exception &e)
        {
            throw std::invalid_argument("Valor de amount não é um número válido");
        }
        if (amount <= 0.0f)
        {
            throw std::invalid_argument("Valor de amount não pode ser menor ou igual a 0.0");
        }
        if (filter.empty())
        {
            throw std::invalid_argument("Filter não pode estar em branco no modo Scale");
        }
    }
    if (bRenameMode)
    {
        int s{0}, startNumber = 0;
        try
        {
            startNumber = argParser.GetOptionAs(s, Args::Options::StartNumber);
        }
        catch (const std::exception &e)
        {
            throw std::invalid_argument("StartNumber deve ser um número válido");
        }

        std::string prefix = argParser.GetOption(Args::Options::Prefix);
        if (startNumber < 0)
        {
            throw std::invalid_argument("Start number deve ser igual ou maior que 0");
        }
        if (prefix.empty() || HasInvalidChars(prefix))
        {
            throw std::invalid_argument("Prefixo não pode estar em branco e não pode conter os caracteres: " + GetInvalidChars());
        }

        return std::make_unique<RenameMode>(filter, folder, prefix, startNumber);
    }

    if (bConvertMode)
    {
        const std::string from = argParser.GetOption(Args::Options::From);
        const std::string to = argParser.GetOption(Args::Options::To);
        const std::array<std::string, 2> convertOptions = {"jpg", "png"}; // busca linear, pois são somente duas opçõe
        const bool bIsFromValid = std::find(std::begin(convertOptions), std::end(convertOptions), from) != std::end(convertOptions);
        const bool bIsToValid = std::find(std::begin(convertOptions), std::end(convertOptions), to) != std::end(convertOptions);
        if (!bIsFromValid || !bIsToValid)
        {
            throw std::invalid_argument("From e To devem ser jpg ou png");
        }

        if (from == to)
        {
            throw std::invalid_argument("From e to devem ser diferentes");
        }

        const std::map<std::string, ConvertMode::Format> convertOptionsMap = {
            {"jpg", ConvertMode::Format::JPG},
            {"png", ConvertMode::Format::PNG}
        };

        return std::make_unique<ConvertMode>(filter, folder, convertOptionsMap.at(from), convertOptionsMap.at(to));
    }

    return nullptr;
};
