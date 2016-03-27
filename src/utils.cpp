#include <cstdlib>
#include <dirent.h>
#include <fstream>

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

void write_2dvec(const std::string& file, const std::vector<std::vector<double> >& dv) {
	std::ofstream out(file.c_str());
	out<<dv.size()<<std::endl;
	for (size_t i=0; i<dv.size(); i++) {
		out<<dv[i].size()<<" ";
		for (size_t j=0; j<dv[i].size(); j++) {
			out<<dv[i][j]<<" ";
		}
		out<<std::endl;
	}
	out.close();
}

std::vector<std::vector<double> > read_2dvec(const std::string& file) {
	std::ifstream in(file.c_str());
	std::vector<std::vector<double> > result;
	int lines;
	in>>lines;
	for (int i=0; i<lines; i++) {
		int count;
		in>>count;
		std::vector<double> res;
		for (int j=0; j<count; j++) {
			double val;
			in>>val;
			res.push_back(val);
		}
		result.push_back(res);
	}
	return result;
}

