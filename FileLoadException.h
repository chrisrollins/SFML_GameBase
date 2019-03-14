#ifndef FILELOADEXCEPTION_H
#define FILELOADEXCEPTION_H

#include <string>

namespace GameException
{
	class FileLoadException
	{
	public:
		FileLoadException(std::string fileName) : fileName(fileName) { }
		virtual std::string getFileName() const { return this->fileName; };
		virtual ~FileLoadException() {}
	private:
		std::string fileName;
	};

	class DataFileLoadException : public FileLoadException
	{
	public:
		DataFileLoadException(std::string fileName) : FileLoadException(fileName) {}
		virtual ~DataFileLoadException() {}
	};

	class FontFileLoadException : public FileLoadException
	{
	public:
		FontFileLoadException(std::string fileName) : FileLoadException(fileName) {}
		virtual ~FontFileLoadException() {}
	};

	class SoundFileLoadException : public FileLoadException
	{
	public:
		SoundFileLoadException(std::string fileName) : FileLoadException(fileName) {}
		virtual ~SoundFileLoadException() {}
	};

	class ImageFileLoadException : public FileLoadException
	{
	public:
		ImageFileLoadException(std::string fileName) : FileLoadException(fileName) {}
		virtual ~ImageFileLoadException() {}
	};
}

#endif