#ifndef __SVM_H__
#define __SVM_H__

#include <map>

#include <Classifier.h>

class SVMCore {
	std::string working_dir;
public:
	SVMCore(const std::string&, bool);
	~SVMCore();
};

class SVM : public Classifier {
	std::map<std::string, SVMCore*> svm_cores;
public:
	SVM(const std::vector<std::string>&);
	virtual ~SVM();

	void train(const Dataset&);
	void load_model();
	std::string classify(const std::string&);

	virtual std::vector<double> get_feature_vector(const std::string& filename);
};

#endif
