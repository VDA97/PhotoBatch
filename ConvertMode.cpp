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
		std::cout << GetModeName() << "Convertendo" << filepath << std::endl;
		//Stb library will require these variables
		int width = 0;
		int height = 0;
		int numComp = 0;//
		const int numReqComp = 3;//Usually images could RGB, or RGBA (some png extensions uses RGBA). jpg does not support transparency, so Alfa could be ignored.
		//It is pretty common to see unsigned char being used for image processing. Each pixel has an RGB. Each RGB values goes from 0 to 255, same as char.
		std::cout << GetModeName() << "Variables" << filepath << std::endl;
		if (unsigned char* data = stbi_load(filepath.string().c_str(), &width, &height, &numComp, numReqComp)) {
			std::cout << GetModeName() << "IsLoadProblem ?" << filepath << std::endl;
			std::filesystem::path destFilePath = filepath;
			destFilePath.replace_extension(ToString(m_ToFormat));

			int writeResult = 0;
			switch (m_ToFormat)
			{
			case ConvertMode::Format::JPG:
				std::cout << GetModeName() << "IsJpg ?" << filepath << std::endl;
				writeResult = stbi_write_jpg(destFilePath.string().c_str(), width, height, numComp, data, 50);//Las parameters is regarding jpg quality. Could be a global variable to be defined by user. Todo
			//	std::cout << "Ending for" << std::endl;
				break;
			case ConvertMode::Format::PNG:
				std::cout << GetModeName() << "IsPngProblem ?" << filepath << std::endl;
				writeResult = stbi_write_png(destFilePath.string().c_str(), width, height, numComp, data, 0);
			//	std::cout << "Ending for" << std::endl;
				break;
			default:
				break;
			}

			//writeResult = 1;
			if (writeResult == 0) {
				std::cout << GetModeName() << "Erro ao converter" << filepath << std::endl;
			}
			stbi_image_free(data);
		}
		else {
			std::cout << GetModeName() << "Erro ao carregar" << filepath << std::endl;
		}
		std::cout << "Ending for" << std::endl;
	}

}
