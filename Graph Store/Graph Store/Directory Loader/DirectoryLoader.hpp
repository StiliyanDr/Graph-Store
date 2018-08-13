
template <class Function>
void DirectoryLoader::loadApplyingFunctionToEachGraph(const String& path, const Function& function)
{
	openDirectory(path);
	
	std::unique_ptr<Graph> graph;

	while (thereAreFilesLeftToLoad())
	{
		graph = loadCurrentFile();

		if (graph != nullptr)
		{
			function(std::move(graph));
		}

		goToNextFile();
	}
}
