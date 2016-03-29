#include <algorithm>
#include <fstream>
#include <iterator>
#include <cstdlib>
#include <CImg.h>
#include <DeepSVM.h>
using namespace cimg_library;

DeepSVM::DeepSVM(const std::vector<std::string>& cl, const std::string& wd) :
		SVM(cl, wd) {

}

DeepSVM::~DeepSVM() {

}

void DeepSVM::preprocess(const Dataset& data) {

}

std::vector<double> DeepSVM::get_feature_vector(const std::string& filename,
		bool) {
	std::string replaced = filename;
	CImg<double> input(filename.c_str());
	CImg<double> resized = input.resize(231, 231, 1, 3);
	resized.save("temp.jpg");
	std::replace(replaced.begin(), replaced.end(), '/', '_');
	std::string out_file = working_dir + "/" + replaced;
	std::string cmd = "./overfeat/bin/linux_64/overfeat -L 12 temp.jpg > " + out_file;
	std::system(cmd.c_str());

	int n, h, w;
	std::ifstream in(out_file.c_str());
	in >> n >> h >> w;
	std::vector<double> values;
	values.reserve(n * h * w);
	values.insert(values.begin(), std::istream_iterator<double>(in),
			std::istream_iterator<double>());
	return values;
}
