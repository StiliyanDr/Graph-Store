#ifndef __DIRECTORY_LOADER_HEADER_INCLUDED__
#define __DIRECTORY_LOADER_HEADER_INCLUDED__

#include "../Graph Builder/GraphBuilder.h"

class GraphCollection;

namespace GraphIO
{
	class DirectoryLoader
	{
		class ExtensionFilter
		{
		public:
			explicit ExtensionFilter(String extension) :
				extension(std::move(extension))
			{
			}

			bool operator()(const String& extension) const
			{
				return extension == this->extension;
			}

		private:
			String extension;
		};

	public:
		GraphCollection load(const String& path);
	
	private:
		static const String EXTENSION;

	private:
		GraphBuilder graphBuilder;
	};
}

#endif //__DIRECTORY_LOADER_HEADER_INCLUDED__