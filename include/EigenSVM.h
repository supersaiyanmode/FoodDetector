#ifndef __EIGEN_SVM_H__
#define __EIGEN_SVM_H__

#include <vector>

#include <SVM.h>
#include <CImg.h>

class EigenSVM : public SVM {
	cimg_library::CImg<double> average_image;
	std::vector<cimg_library::CImg<double> > eigen_vectors;
    
public:
    EigenSVM(const std::vector<std::string>&, const std::string&);
    ~EigenSVM();
    
    void preprocess(const Dataset&);
    void load_model();
    std::vector<double> get_feature_vector(const std::string&, bool);
};

#endif
