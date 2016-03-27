#ifndef __UTILS__H
#define __UTILS__H

#include <vector>
#include <string>

std::vector<std::string> list_files(const std::string&, bool=false);
void truncate_dir(const std::string& path);


std::vector<std::vector<double> > read_2dvec(const std::string& file);

void write_2dvec(const std::string& file, const std::vector<std::vector<double> >& vec);

#endif
