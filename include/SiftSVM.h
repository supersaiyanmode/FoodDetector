#ifndef __SIFT_SVM_H__
#define __SIFT_SVM_H__

#include <vector>

#include <SVM.h>

class SiftSVM : public SVM {
	int num_clusters;
	std::vector<std::vector<double> > visual_words;

	std::string cache_dir;

	std::vector<std::vector<double> > get_sift_cache(const std::string&);
public:
	SiftSVM(const std::vector<std::string>&, const std::string&);
	~SiftSVM();

	void load_model();

	std::vector<double> get_feature_vector(const std::string&, bool);

	void preprocess(const Dataset&);
};

#endif
