#ifndef __EIGEN_SVM_H__
#define __EIGEN_SVM_H__

#include <SVM.h>
#include <CImg.h>

class EigenSVM : public SVM {
public:
	cimg_library::CImg<double> average_image;
	cimg_library::CImg<double> eigen_values, eigen_vector;
    std::vector<double> avg;
    
    EigenSVM(const std::vector<std::string>&, const std::string&);
    ~EigenSVM();
    
    void preprocess(const Dataset&);
    void load_model();
    std::vector<double> get_feature_vector(const std::string&, bool);
};

#endif
