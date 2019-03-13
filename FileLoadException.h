#ifndef FILE_LOAD_EXCEPTION_HEADER
#define FILE_LOAD_EXCEPTION_HEADER

#include <string>

namespace GameException
{
	class FileLoadException
	{
	public:
		FileLoadException(std::string filename) : filename(filename) { }
		virtual std::string getFileName() const { return this->filename; };
	private:
		std::string filename;
	};

	class DataFileLoadException : public FileLoadException
	{
	public:
		DataFileLoadException(std::string filename) : FileLoadException(filename) {}
	};

	class FontFileLoadException : public FileLoadException
	{
	public:
		FontFileLoadException(std::string filename) : FileLoadException(filename) {}
	};

	class SoundFileLoadException : public FileLoadException
	{
	public:
		SoundFileLoadException(std::string filename) : FileLoadException(filename) {}
	};

	class ImageFileLoadException : public FileLoadException
	{
	public:
		ImageFileLoadException(std::string filename) : FileLoadException(filename) {}
	};
}

#endif