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
	std::string command = "./svm/svm_multiclass_learn -c 1.0 " + input + " " + output;
	std::cout<<"Executing: "<<command<<std::endl;
	(void)system(command.c_str());
}

void SVMCore::classify(const std::string& input, const std::string& model, const std::string& output) {
	std::string command = "./svm/svm_multiclass_classify " + input + " " + model + " " + output;
	std::cout<<"Executing: "<<command<<std::endl;
	(void)system(command.c_str());
}

SVM::SVM(const std::vector<std::string>& cl, const std::string& wd): 
			Classifier(cl), working_dir(wd) {
	cache_dir = wd + "/_cache";
	test_dir = wd + "/test";

	truncate_dir(test_dir);

	for (int i=0; i<cl.size(); i++) {
		class_pos[cl[i]] = i;
	}
}

SVM::~SVM() {
}

void SVM::train(const Dataset& data) {
	truncate_dir(working_dir);
	truncate_dir(cache_dir);

	SVMCore svm_core;
	std::ofstream out((working_dir + "/input.svm").c_str());
	for (Dataset::const_iterator it = data.begin(); it != data.end(); it++) {
		std::string cur_food_name = it->first;
		const std::vector<std::string>& cur_files = it->second;

		for (std::vector<std::string>::const_iterator it1 = cur_files.begin(); it1 != cur_files.end(); it1++) {
			std::cout<<"  Processing: "<<*it1<<std::endl;
			std::vector<double> features = get_feature_vector(*it1, true);
			write_svm_vector(features, out, class_pos[cur_food_name]+1);
		}
		
	}

	out.close();

	std::string outputModel = working_dir + "/model.svm";
	svm_core.train(working_dir + "/input.svm", outputModel);
}

void SVM::write_svm_vector(const std::vector<double>& features, std::ostream& out, int cls) {
	int feature_pos = 1;
	out<<cls<<" ";
	for (std::vector<double>::const_iterator it3 = features.begin(); it3 != features.end(); it3++) {
		out<<feature_pos++<<":"<<*it3<<" ";
	}
	out<<std::endl;
}

void SVM::load_model() {

}

std::string SVM::classify(const std::string& filename) {
	SVMCore svm_core;
	std::string output_class = "";

	std::string inputFile = test_dir + "/" + "input.svm";
	std::ofstream out(inputFile.c_str());
	std::vector<double> features = get_feature_vector(filename, false);
	write_svm_vector(features, out, 1); //Last param does not matter.
	out.close();

	std::string cur_model_file = working_dir + "/model.svm";
	std::string output_file = working_dir + "/output.svm";

	svm_core.classify(inputFile, cur_model_file, output_file);

	std::ifstream in(output_file.c_str());
	int value;
	in>>value;
	return Classifier::class_list[value-1];
}
