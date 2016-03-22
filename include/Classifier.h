#ifndef __CLASSIFIER_H
#define __CLASSIFIER_H

#include <defs.h>
#include <string>
#include <vector>

class Classifier
{
public:
  Classifier(const std::vector<std::string> &_class_list) : class_list(_class_list) {}
  virtual ~Classifier() {}

  // Run training on a given dataset.
  virtual void train(const Dataset &filenames) = 0;

  // Classify a single image.
  virtual std::string classify(const std::string &filename) = 0;

  // Load in a trained model.
  virtual void load_model() = 0;

  void test(const Dataset&);
protected:
  std::vector<std::string> class_list;
};

#endif
