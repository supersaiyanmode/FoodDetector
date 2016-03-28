#include <algorithm>
#include <fstream>
#include <iterator>
#include <cstdlib>

#include <DeepSVM.h>


DeepSVM::DeepSVM(const std::vector<std::string>& cl, const std::string& wd): SVM(cl, wd) {

}

DeepSVM::~DeepSVM() {

}

void DeepSVM::preprocess(const Dataset& data) {

}

std::vector<double> DeepSVM::get_feature_vector(const std::string& filename, bool) {
	std::string replaced = filename;
	std::replace(replaced.begin(), replaced.end(), '/', '_');
	std::string out_file = working_dir + "/" + replaced;
	std::string cmd = "./overfeat/bin/linux_64/overfeat -L 12 " + filename + " > " + out_file;
	std::system(cmd.c_str());

	int n, h, w;
	std::ifstream in(out_file.c_str());
	in>>n>>h>>w;
	std::vector<double> values;
	values.reserve(n*h*w);
	values.insert(values.begin(), std::istream_iterator<double>(in), std::istream_iterator<double>());
	return values;
}
