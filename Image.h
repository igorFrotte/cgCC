#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <string>
#include "Color.h"

template<class ColorType>
class Image{
	protected:
	int _width = 0, _height = 0;
	std::vector<ColorType> pixels;

	public:

	// Construtor padrão, imagem vazia
	Image() = default;

	// Construtor com tamanho da imagem
	Image(int w, int h):
		_width(w), _height(h), pixels(w*h)
	{}

	// Largura da imagem
	int width() const{ return _width; }

	// Altura da imagem
	int height() const{ return _height; }

	// Número de pixels
	int size() const{ return pixels.size(); }

	// Acesso a um pixel
	ColorType& operator()(int x, int y){
		return pixels[y*_width + x];
	}
	
	// Acesso a um pixel
	const ColorType& operator()(int x, int y) const{
		return pixels[y*_width + x];
	}

	// Preenche a imagem com uma cor
	void fill(ColorType color){
		pixels.assign(pixels.size(), color);
	}

	// Obtém um ponteiro para os dados dos pixels
	const ColorType* data() const{ return pixels.data(); }
};

#include "stb_image_write.h"
#include "stb_image.h"

template<int N>
class ImageCol : public Image<Color<N>>{
	public:
	// Usa construtores da classe Image
	using Image<Color<N>>::Image;

	// Construtor para carregar um arquivo de imagem
	ImageCol(std::string filename){
		int nrChannels;
		stbi_set_flip_vertically_on_load(true);
		Color<N>* image = (Color<N>*)stbi_load(filename.c_str(), &this->_width, &this->_height, &nrChannels, N);
		if(image){
			this->pixels.assign(image, image+this->_width*this->_height);
			stbi_image_free(image);
		}
	}
	
	// Salva a imagem num arquivo PNG
	void savePNG(std::string filename){
		stbi_flip_vertically_on_write(true);
		stbi_write_png(filename.c_str(), this->_width, this->_height, N, this->pixels.data(), 0);
	}

	// Salva a imagem num arquivo BMP
	void saveBMP(std::string filename){
		stbi_flip_vertically_on_write(true);
		stbi_write_bmp(filename.c_str(), this->_width, this->_height, N, this->pixels.data());
	}

	void save_frame(int index, std::string base, std::string ext){
		char filename[50];
		snprintf(filename, 50, "%s%03d.%s", base.c_str(), index, ext.c_str());
		puts(filename);
		if(ext == "bmp")
			saveBMP(filename);
		else if(ext == "png")
			savePNG(filename);
	}
};

using ImageRGB = ImageCol<3>;
using ImageRGBA = ImageCol<4>;

#endif
