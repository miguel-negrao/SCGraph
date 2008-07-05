#include "texture_pool.h"

#include "options.h"

#include <sstream>
#include <iostream>
#include <cmath>

#include <sys/types.h>
#include <dirent.h>

#include <Magick++.h>

#include <cstring>

Texture::Texture (int width, int height, int channels)
{
	_width = width; _height = height; _channels = channels;

	_data = new unsigned char [width * height * channels];

	memset (_data, 0, width * height * channels);
}

Texture::~Texture ()
{
	delete[] _data;
}

TexturePool *TexturePool::_instance = 0;

TexturePool *TexturePool::get_instance ()
{
	if (!_instance)
		_instance = new TexturePool;

	return _instance;
}

TexturePool::TexturePool ()
{
	Options *options = Options::get_instance ();

	tokenize_path (options->_texture_path, &TexturePool::scan_directory);
}

void TexturePool::tokenize_path (const std::string &path, void (TexturePool::*function)(const std::string &directory))
{
	std::stringstream path_stream (path);

	// FIXME: dirty hack
	char buffer [PATH_MAX];
	
	while (path_stream.getline (buffer, PATH_MAX, ':'))
	{
		if (std::string (buffer) == "")
			continue;

		(*this.*function) (buffer);
	}
}

void TexturePool::scan_directory (const std::string &directory)
{
	try
	{
		traverse_directory (directory, &TexturePool::add_image);
	}
	catch (const char *e)
	{
		std::cout << "[TexturePool]: Failed to traverse directory. Reason: " << e << std::endl;
	}
}

void TexturePool::traverse_directory (const std::string &directory, void (TexturePool::*function)(const std::string &filename))
{
	Options *options = Options::get_instance ();

	if (options->_verbose >= 2)
		std::cout << "[TexturePool]: Adding texture directory: " << directory << std::endl;


	DIR *dir = opendir (directory.c_str ());
	if (!dir)
		throw ("[TexturePool]: Couldn't read directory");


	std::vector<std::string> filenames;
	std::vector<std::string> dirnames;

	struct dirent *dirent;
	while ((dirent = readdir (dir)))
	{
		if ((std::string (dirent->d_name) == std::string (".")) || (std::string (dirent->d_name) == std::string ("..")))
			continue;
 
		if (dirent->d_type == DT_DIR)
		{
			dirnames.push_back (directory + "/" + std::string (dirent->d_name));
		}
		else
		{
			filenames.push_back(directory + "/" + std::string (dirent->d_name));

		}
	}

	std::sort(filenames.begin(), filenames.end());
	std::sort(dirnames.begin(), dirnames.end());

	for (size_t i = 0; i < filenames.size(); ++i)
		(*this.*function) (filenames[i]);

	for (size_t i = 0; i < dirnames.size(); ++i)
		traverse_directory (dirnames[i], function);


	closedir (dir);
}

void TexturePool::add_image (const std::string &filename)
{
	Options *options = Options::get_instance ();

	if (options->_verbose >= 2)
		std::cout << "[TexturePool]: Adding texture image: " << filename << std::endl;

	try
	{
		Magick::Image image;
		image.read (filename);
	
		int im_width, im_height, tex_width, tex_height;
	
		tex_width =  (int)pow(2,(int)ceil(log2(image.columns())));
		tex_height = (int)pow(2,(int)ceil(log2(image.rows())));

		im_width = image.columns();
		im_height = image.rows();

		if (options->_verbose >= 2)
		{
			std::cout << "  [TexturePool]: Texture Width/Height: " << tex_width << "/" << tex_height << std::endl;
			std::cout << "  [TexturePool]: Image source Width/Height: " << im_width << "/" << im_height << std::endl;
		}

		// std::cout << width << " " << height << std::endl;
	
		boost::shared_ptr<Texture> t(new Texture (tex_width, tex_height, 4));

		for (int i = 0; i < im_width; ++i)
		{
			for (int j = 0; j < im_height; ++j)
			{
				//std::cout << (image.pixelColor(i,j).alphaQuantum()/1.0) << std::endl;

				/* swap image */
				Magick::ColorRGB color = image.pixelColor(i,im_height - j);

				t->_data[4 * (tex_width * j + i)]     = (unsigned char) (255.0 * color.red());
				t->_data[4 * (tex_width * j + i) + 1] = (unsigned char) (255.0 * color.green());
				t->_data[4 * (tex_width * j + i) + 2] = (unsigned char) (255.0 * color.blue());
				t->_data[4 * (tex_width * j + i) + 3] = (unsigned char) (255.0 * (1.0 - color.alpha())); 
#if 0
				t->_data[4 * (width * j + i)]     = (unsigned char)(image.pixelColor(i,j).redQuantum());
				t->_data[4 * (width * j + i) + 1] = (unsigned char)(image.pixelColor(i,j).greenQuantum());
				t->_data[4 * (width * j + i) + 2] = (unsigned char)(image.pixelColor(i,j).blueQuantum());
				t->_data[4 * (width * j + i) + 3] = 255 - (unsigned char)(image.pixelColor(i,j).alphaQuantum());
#endif
				
			}
		}
		_textures.push_back (t);
	
		if (options->_verbose >= 2)
			std::cout << "  [TexturePool]: New texture has index: " << _textures.size() - 1 << std::endl;
	
		emit (textures_changed());
	}
	catch (Magick::Exception &e)
	{
		std::cout << "[TexturePool]: Problem loading texture: " << e.what () << std::endl;
	}
	// emit(texture_added(_textures.size () - 1));
}

TexturePool::~TexturePool ()
{
	// TODO: clean up
}

unsigned int TexturePool::get_number_of_textures ()
{
	return _textures.size ();
}

boost::shared_ptr<Texture> TexturePool::get_texture (unsigned int index)
{
	return _textures[index];
}



