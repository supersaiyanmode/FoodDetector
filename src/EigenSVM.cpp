#include <iostream>
#include <algorithm>
#include <fstream>

#include <CImg.h>
#include <EigenSVM.h>

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

	int resizeX = 40, resizeY = 40;
	int cur_row = 0;

	CImg<double> average_image(resizeX, resizeY);
	CImg<double> all_data(total_images, resizeX * resizeY);
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
				all_data(cur_row, i) = unrolled(i, 0);
			}
			cur_row ++;
		}
	}
	average_image /= (double)total_images;

	this->average_image = average_image;

	CImg<double> temp = average_image.resize(5, 5).unroll('x');
	for (int i=0; i<temp.width(); i++)
		std::cout<<temp(i, 0);
	
	CImg<double> covmat = all_data * all_data.get_transpose();
	covmat.symmetric_eigen(this->eigen_values, this->eigen_vector);
	
	std::cout<<"Covmat dimension: "<<covmat.width()<<" "<<covmat.height()<<std::endl;
	std::cout<<"Eigen vector size: "<<this->eigen_vector.size()<<std::endl;

}

std::vector<double> EigenSVM::get_feature_vector(const std::string& filename, bool is_train_mode) {
	return std::vector<double>();
}

void EigenSVM::load_model() {

}
