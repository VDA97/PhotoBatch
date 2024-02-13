#include "ConvertMode.h"
#include <iostream>

std::ostream& operator <<(std::ostream& out, ConvertMode::Format format)
{
	switch (format)
	{
	case ConvertMode::Format::JPG:
		out << "jpg";
		break;
	case ConvertMode::Format::PNG:
		out << "png";
		break;
	default:
		break;
	}

	return out;
}

ConvertMode::ConvertMode(const std::string& filter,
	const std::string& folder,
	Format fromFormat,
	Format toFormat)
	: Mode(filter, folder)
	, m_FromFormat(fromFormat)
	, m_ToFormat(toFormat)
{

}

const std::string& ConvertMode::GetModeName() const {
	static const std::string ConvertModeName = "[Converter]: ";
	return ConvertModeName;
}

void ConvertMode::RunImpl() {
	std::cout << GetModeName() << "Modo		:	Converter" << std::endl;
	std::cout << GetModeName() << "Folder	:	" << GetFolder() << std::endl;
	std::cout << GetModeName() << "Filter	:	" << GetFilter() << std::endl;	
	std::cout << GetModeName() << "From		:	" << m_FromFormat << std::endl;
	std::cout << GetModeName() << "To		:	" << m_ToFormat << std::endl;

}
