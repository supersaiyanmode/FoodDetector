#ifndef __SVM_H__
#define __SVM_H__

#include <map>
#include <fstream>

#include <Classifier.h>

class SVMCore {
	std::string working_dir;
public:
	SVMCore();
	~SVMCore();

	void train(const std::string&, const std::string&);
	void classify(const std::string&, const std::string&, const std::string&);
};

class SVM : public Classifier {
	std::string working_dir, cache_dir, test_dir;

	std::vector<double> get_feature_vector_cache(const std::string& filename, bool);
	void write_svm_vector(const std::vector<double>&, std::ostream&, int);

	std::map<std::string, int> class_pos;
public:
	SVM(const std::vector<std::string>&, const std::string&);
	virtual ~SVM();

	void train(const Dataset&);
	void load_model();
	std::string classify(const std::string&);

	virtual std::vector<double> get_feature_vector(const std::string& filename, bool) = 0;
};

#endif
