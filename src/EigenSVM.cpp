#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>

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
	average_image.save("average.png");

	CImg<double> covmat = (all_data.get_transpose() * all_data) / total_images;
	covmat.symmetric_eigen(this->eigen_values, this->eigen_vector);
	
	std::cout<<"Covmat dimension: "<<covmat.width()<<" "<<covmat.height()<<std::endl;
	std::cout<<"Eigen vector size: "<<this->eigen_vector.width()<<" "<<this->eigen_vector.height()<<std::endl;
	std::cout<<"Eigen values size: "<<this->eigen_values.width()<<" "<<this->eigen_values.height()<<std::endl;

	
}

std::vector<double> EigenSVM::get_feature_vector(const std::string& filename, bool is_train_mode) {
	return std::vector<double>();
}

void EigenSVM::load_model() {

}
