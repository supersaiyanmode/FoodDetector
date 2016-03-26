#ifndef __UTILS__H
#define __UTILS__H

#include <vector>
#include <string>

std::vector<std::string> list_files(const std::string&, bool=false);
void truncate_dir(const std::string& path);

#endif
