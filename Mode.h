#pragma once
#include <string>
#include <memory>
#include <filesystem>
#include <vector>

class Mode
{
public:
    Mode(const std::string &filter, const std::string &folder) : m_Filter(filter), m_Folder(folder) {}
    const std::string &GetFilter() const; // first const means that the caller cannot modify the returned object.
    const std::string &GetFolder() const; // second const means that the function does not the state of the object.

    // Abstract methods
    virtual const std::string &GetModeName() const = 0;
    void Run();

protected:
    virtual void RunImpl() = 0; // The function, which will heritage from interface, should implement these virtual functions std::string m_Filter;
    std::vector<std::filesystem::path>GetFiles(const std::filesystem::path & extension = "") const;

private:
    std::string m_Filter;
    std::string m_Folder;
};

namespace Args
{
    namespace Flags
    {
        static constexpr const char *Rename = "rename";
        static constexpr const char *Convert = "convert";
        static constexpr const char *Resize = "resize";
        static constexpr const char *Scale = "scale";
        static constexpr const char *Help = "help";
    } // namespace Flags
    namespace Options
    {
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

} // namespace Args

class ArgumentParser;
std::unique_ptr<Mode> CreateMode(const ArgumentParser &argParser);
