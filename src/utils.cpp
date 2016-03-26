#include <cstdlib>
#include <dirent.h>

#include <utils.h>
#include <defs.h>

std::vector<std::string> list_files(const std::string &directory, bool prepend_directory)
{
	std::vector<std::string> file_list;
	::DIR *dir = opendir(directory.c_str());
	if(!dir)
		throw std::string("Can't find directory " + directory);

	::dirent *dirent;
	while ((dirent = readdir(dir))) 
		if(dirent->d_name[0] != '.')
			file_list.push_back((prepend_directory?(directory+"/"):"")+dirent->d_name);

	closedir(dir);
	return file_list;
}

void truncate_dir(const std::string& path) {
	std::string cmd = "rm -rf " + path;
	std::string cmd2 = "mkdir -p " + path;
	(void)::system(cmd.c_str());
	(void)::system(cmd2.c_str());
}
