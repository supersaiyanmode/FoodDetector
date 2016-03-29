#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>

#include <CImg.h>
#include <EigenSVM.h>
#include <Config.h>

using namespace cimg_library;

EigenSVM::EigenSVM(const std::vector<std::string>& cl, const std::string& wd): SVM(cl, wd) {

}

EigenSVM::~EigenSVM() {

}

void EigenSVM::preprocess(const Dataset& data) {
	int total_images = 0;
	for (Dataset::const_iterator it = data.begin(); it != data.end(); it++) {
		total_images += it->second.size();
	}

	int resizeX = config.get<int>("eigen.resizeX"), resizeY = config.get<int>("eigen.resizeY");
	int cur_row = 0;

	CImg<double> average_image(resizeX, resizeY);
	CImg<double> all_data(resizeX * resizeY, total_images);
	average_image.fill(0.0);

	for (Dataset::const_iterator it = data.begin(); it != data.end(); it++) {
		std::string cur_food_name = it->first;
		std::vector<std::string> files = it->second;
		for (std::vector<std::string>::const_iterator it1 = files.begin(); it1 != files.end(); it1++) {
			std::cout<<"  Preprocessing: "<<*it1<<std::endl;
			//Calculate average
			CImg<double> image(it1->c_str());
			CImg<double> grey = image.spectrum() == 1? image : image.get_RGBtoHSI().get_channel(2);
			CImg<double> cur_image = grey.resize(resizeX, resizeY);
			average_image += cur_image;

			//Put the image in the all_data variable.
			CImg<double> unrolled = cur_image.unroll('x');
			for (int i=0; i<unrolled.width(); i++) {
				all_data(i, cur_row) = unrolled(i, 0);
			}
			cur_row ++;
		}
	}
	std::cout<<"Done with preprocess....\n";
	average_image /= (double)total_images;

	average_image.display();

	this->average_image = average_image;
	average_image.save((SVM::working_dir + "/average.png").c_str());

	CImg<double> covmat = (all_data.get_transpose() * all_data) / total_images;

	CImg<double> eigen_values, eigen_vector;
	covmat.symmetric_eigen(eigen_values, eigen_vector);
	
	std::cout<<"Covmat dimension: "<<covmat.width()<<" "<<covmat.height()<<std::endl;
	std::cout<<"Eigen vector size: "<<eigen_vector.width()<<" "<<eigen_vector.height()<<std::endl;
	std::cout<<"Eigen values size: "<<eigen_values.width()<<" "<<eigen_values.height()<<std::endl;

	std::vector<std::pair<double, CImg<double> > > eigen_pairs;
	for (size_t i=0; i<eigen_values.size(); i++) {
		CImg<double> res(resizeX, resizeY, 1, 1);
		CImg<double> col(eigen_vector.get_column(i));
		CImg<double>::iterator dest_it = res.begin();
		for (CImg<double>::iterator it = col.begin(); it != col.end(); it ++) {
			*dest_it = *it;
			dest_it ++;
		}
		eigen_pairs.push_back(std::make_pair(eigen_values[i], eigen_vector.get_column(i)));
	}
	std::sort(eigen_pairs.rbegin(), eigen_pairs.rend());

	int k = config.get<int>("eigen.k");
	this->eigen_vectors = std::vector<CImg<double> >();
	for (int i=0; i < k; i++) {
		eigen_pairs[i].second.save((working_dir + "/eigen-vector.png").c_str(), i, 2);
		this->eigen_vectors.push_back(eigen_pairs[i].second);
	}
	std::cout<<"Saved eigen food images."<<std::endl;
}

std::vector<double> EigenSVM::get_feature_vector(const std::string& filename, bool is_train_mode) {
	std::cout<<"  Processing: "<<filename<<std::endl;
	int resizeX = config.get<int>("eigen.resizeX"), resizeY = config.get<int>("eigen.resizeY");
	std::vector<double> result;
	
	for (std::vector<CImg<double> >::iterator it = eigen_vectors.begin(); it != eigen_vectors.end(); it++) {
		CImg<double> input(filename.c_str());
		input = input.resize(resizeX, resizeY);
		input = input.spectrum() == 1? input: input.get_RGBtoHSI().get_channel(2);
		result.push_back((input - average_image).unroll('x').dot((it->unroll('x'))));
	}

	return result;
}

void EigenSVM::load_model() {

}
