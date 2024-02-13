#include "ConvertMode.h"
#include <iostream>
#define	STB_IMAGE_IMPLEMENTATION 
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

std::string ToString(ConvertMode::Format format) {
	switch (format)
	{
	case ConvertMode::Format::JPG:
		return "jpg";
	case ConvertMode::Format::PNG:
		return "png";
	default:
		break;
	}

	return "";
}

std::ostream& operator <<(std::ostream& out, ConvertMode::Format format)
{
	out << ToString(format);
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

	const std::filesystem::path fromExtension = "." + ToString(m_FromFormat);
	
	for (const std::filesystem::path& filepath : GetFiles(fromExtension)) {
		std::cout << GetModeName() << filepath << std::endl;
	}

}
