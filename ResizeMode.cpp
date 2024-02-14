#pragma once
#include "ResizeMode.h"
#include <iostream>
#include <vector>
extern "C" {
//#define STB_IMAGE_IMPLEMENTATION -> commented because only one define should work. Avoid duplicate symbols
#include <stb_image.h>
//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize2.h>
}

#include "Mode.h"

ResizeMode::ResizeMode(const std::string& filter,
	const std::string& folder,
	int width,
	int height)
	: Mode(filter, folder)
	, m_Width(width)
	, m_Height(height)
{
}

const std::string& ResizeMode::GetModeName() const {
	static const std::string ResizeModeName = "[Resize]: ";
		return ResizeModeName;
}

void ResizeMode::RunImpl() {
	std::cout << GetModeName() << "Modo		:	Resize" << std::endl;
	std::cout << GetModeName() << "Folder	:	" << GetFolder() << std::endl;
	std::cout << GetModeName() << "Filter	:	" << GetFilter() << std::endl;
	std::cout << GetModeName() << "Width	:	" << m_Width << std::endl;
	std::cout << GetModeName() << "Height	:	" << m_Height << std::endl;

	if (m_Width > 0 && m_Height > 0) {
		for (const std::filesystem::path& filepath : GetFiles()) {
			std::cout << GetModeName() << "Resizing: " << filepath << std::endl;
			ResizeImage(filepath, m_Width, m_Height);//Why pass m_Width and m_Height if they are Class members ?Because this method will be used in ScaleMode.
			
		}	
	}
}

void ResizeMode::ResizeImage(const std::filesystem::path& filepath, int newWidth, int newHeight)const {
	//Read Image
	//Resize memory
	//Write the image
	int inputWidth = 0;
	int inputHeight = 0;
	int inputNumComp = 0;
	int numCompReq = 4;
	if (unsigned char* inputData = stbi_load(filepath.string().c_str(), &inputWidth, &inputHeight, &inputNumComp, numCompReq)) {
		//Calculate how much pixels we need in the output
		const int numOutputPixels = newWidth * newHeight * numCompReq; 
		//Allocate memory. Note it could be used std::vector<std::uint8_t> outputData(numOutputPixels, 0)
		std::vector<unsigned char> outputData(numOutputPixels, 0);//It better than using malloc. This case vector will delete the memory
		//Resize 
		std::cout << "Berfore Resize Result 2" << std::endl;
		if (unsigned char* resizeResult = stbir_resize_uint8_srgb(inputData, inputWidth, inputHeight,
			0, outputData.data(), newWidth, newHeight,
			0, stbir_pixel_layout::STBIR_4CHANNEL)) {
			int writeResult = 1;
			const std::filesystem::path extension = filepath.extension();
			if (extension == ".jpg") {
				writeResult = stbi_write_jpg(filepath.string().c_str(), newWidth, newHeight, numCompReq, outputData.data(), 50);
			}
			else if (extension == ".png") {
				writeResult = stbi_write_png(filepath.string().c_str(), newWidth, newHeight, numCompReq, outputData.data(), 0);
			}
			else {
				std::cout << GetModeName() << "Formato não suportado" << filepath << std::endl;
			}
			if (writeResult == 0) {
				std::cout << GetModeName() << "Error when writing filepath" << filepath << std::endl;
			}
		}
		else {
			std::cout << GetModeName() << "Erro ao Redimensionar" << filepath << std::endl;
		}
		stbi_image_free(inputData);
	}
	else {
		std::cout << GetModeName() << "Erro ao Carregar " << filepath << std::endl;
	}
}

