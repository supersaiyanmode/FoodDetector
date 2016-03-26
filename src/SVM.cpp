#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <iterator>

#include <SVM.h>
#include <utils.h>


SVMCore::SVMCore() {

}

SVMCore::~SVMCore() {

}

void SVMCore::train(const std::string& input, const std::string& output) {
	std::string command = "./svm/svm_learn " + input + " " + output;
	std::cout<<"Executing: "<<command<<std::endl;
	(void)system(command.c_str());
}

void SVMCore::classify(const std::string& input, const std::string& model, const std::string& output) {
	std::string command = "./svm/svm_classify " + input + " " + model + " " + output;
	std::cout<<"Executing: "<<command<<std::endl;
	(void)system(command.c_str());
}

SVM::SVM(const std::vector<std::string>& cl, const std::string& wd): 
			Classifier(cl), working_dir(wd) {
	cache_dir = wd + "/_cache";
	test_dir = wd + "/test";

	truncate_dir(test_dir);
}

SVM::~SVM() {
}

void SVM::train(const Dataset& data) {
	truncate_dir(working_dir);
	truncate_dir(cache_dir);

	SVMCore svm_core;
	for (Dataset::const_iterator it = data.begin(); it != data.end(); it++) {
		std::string cur_food_name = it->first;
		const std::vector<std::string>& cur_files = it->second;
		std::cout<<"Processing: "<<cur_food_name<<std::endl;

		std::string inputFile = working_dir + "/" + cur_food_name + ".input.svm";
		std::ofstream out(inputFile.c_str());

		//Positive examples
		for (std::vector<std::string>::const_iterator it1 = cur_files.begin(); it1 != cur_files.end(); it1++) {
			std::vector<double> features = get_feature_vector_cache(*it1, true);
			write_svm_vector(features, out, true);
		}
		
		//Negative examples
		for (Dataset::const_iterator it1 = data.begin(); it1 != data.end(); it1++) {
			if (it1->first == cur_food_name)
				continue;
			
			for (std::vector<std::string>::const_iterator it2 = it1->second.begin(); 
									it2 != it1->second.end(); it2++) {
				std::vector<double> features = get_feature_vector_cache(*it2, true);
				write_svm_vector(features, out, false);
			}
		}
		out.close();

		std::string outputModel = working_dir + "/" + cur_food_name + ".model.svm";
		svm_core.train(inputFile, outputModel);
	}
}

/*
 * Implements a file system based cache.
 */
std::vector<double> SVM::get_feature_vector_cache(const std::string& filename, bool train) {
	std::string cached_name = filename;

	//Reference: http://stackoverflow.com/a/2896627/227884
	std::replace(cached_name.begin(), cached_name.end(), '/', '_');

	std::ifstream inFile((cache_dir + "/" + cached_name).c_str());
	std::vector<double> result;
	if (inFile.good()) {
		//Reference: http://stackoverflow.com/a/19602431/227884
		std::istream_iterator<double> it(inFile);
		std::copy(it, std::istream_iterator<double>(), std::back_inserter(result));
		return result;
	}
	inFile.close();

	result = get_feature_vector(filename, train);
	std::ofstream outFile((cache_dir + "/" + cached_name).c_str());
	//Reference: http://stackoverflow.com/a/6406411/227884
	std::copy(result.begin(), result.end(), std::ostream_iterator<double>(outFile, " "));
	outFile.close();
	return result;
}

void SVM::write_svm_vector(const std::vector<double>& features, std::ostream& out, bool example_kind) {
	int feature_pos = 1;
	out<<(example_kind? "+1 ":"-1 ");
	for (std::vector<double>::const_iterator it3 = features.begin(); it3 != features.end(); it3++) {
		out<<feature_pos++<<":"<<*it3<<" ";
	}
	out<<std::endl;
}

void SVM::load_model() {

}

std::string SVM::classify(const std::string& filename) {
	SVMCore svm_core;
	std::string replaced = filename;
	std::replace(replaced.begin(), replaced.end(), '/', '_');

	std::string clasify_wd = test_dir + "/" + replaced;
	truncate_dir(clasify_wd);

	std::string output_class = "";
	double prediction_value = -10e100;

	for (std::vector<std::string>::iterator it = Classifier::class_list.begin(); 
						it != Classifier::class_list.end(); it++) {
		std::string inputFile = clasify_wd + "/" + *it + ".input.svm";
		std::ofstream out(inputFile.c_str());
		std::vector<double> features = get_feature_vector(filename, false);
		write_svm_vector(features, out, false); //Last param does not matter.
		out.close();

		std::string cur_model_file = working_dir + "/" + *it + ".model.svm";
		std::string output_file = clasify_wd + "/" + *it + ".output.svm";

		svm_core.classify(inputFile, cur_model_file, output_file);

		std::ifstream in(output_file.c_str());
		double value;
		in>>value;

		if (value > prediction_value) {
			prediction_value = value;
			output_class = *it;
		}
	}
	return output_class;
}

