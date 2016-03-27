#include <algorithm>
#include <iterator>
#include <fstream>

#include <CImg.h>
#include <Sift.h>

#include <SiftSVM.h>
#include <utils.h>
#include <Config.h>

using namespace cimg_library;

namespace {
	std::vector<double> to_vec(SiftDescriptor s) {
		std::vector<double> res;
		res.reserve(128);
		for (int i=0; i<128; i++)
			res.push_back(s.descriptor[i]);
		return res;
	}

	SiftDescriptor to_sift(const std::vector<double>& v) {
		SiftDescriptor s;
		for (std::vector<double>::const_iterator it = v.begin(); it != v.end(); it++)
			s.descriptor.push_back(*it);
		return s;
	}

	std::vector<double> avg_veclist(const std::vector<std::vector<double> >& list) {
		std::vector<double> res(list[0].size(), 0);

		for (std::vector<std::vector<double> >::const_iterator it = list.begin(); it != list.end(); it++) {
			for (size_t i=0; i<it->size(); i++) {
				res[i] += (*it)[i] / (double)list.size();
			}
		}
		return res;
	}
	
	double dist(const std::vector<double>& v1, const std::vector<double>& v2) {
		double d = 0.0;
		for (size_t i=0; i<v1.size(); i++) {
			d += (v1[i] - v2[i]) * (v1[i] - v2[i]);
		}
		return std::sqrt(d);
	}

	int get_closest_vector(const std::vector<double>& v, std::vector<std::vector<double> > vecs) {
		double min_dist = 10e100;
		int min_pos = -1;
		for (size_t i=0; i<vecs.size(); i++) {
			double d = dist(v, vecs[i]);
			if (d < min_dist) {
				min_pos = i;
				min_dist = d;
			}
		}
		return min_pos;
	}

	std::vector<std::vector<double> > kmeans2(const std::vector<std::vector<double> >& data, int k) {
		typedef std::vector<double> vec;
		typedef std::vector<vec> veclist;

		veclist centroids;
		for (int i=0; i<k; i++) {
			centroids.push_back(data[i]);
		}

		int iterations = 0;

		std::vector<veclist> cur_assignments;
		cur_assignments.resize(k);
		cur_assignments[0] = data;

		while (iterations < 10) {
			std::vector<veclist> new_assignments;
			new_assignments.resize(cur_assignments.size());

			for (std::vector<veclist>::iterator it = cur_assignments.begin(); it != cur_assignments.end(); it++){
				for (veclist::iterator it1 = it->begin(); it1 != it->end(); it1 ++) {
					int pos = get_closest_vector(*it1, centroids);
					//std::cout<<"Closest pos: "<<pos<<std::endl;
					new_assignments[pos].push_back(*it1);
				}
			}

			for (size_t i=0; i<new_assignments.size(); i++) {
				centroids[i] = avg_veclist(new_assignments[i]);
			}

			iterations++;
		}
		return centroids;
	}
}

SiftSVM::SiftSVM(const std::vector<std::string>& cl, const std::string& wd): SVM(cl, wd) {
	cache_dir = wd + "/_cache";
	truncate_dir(cache_dir);

	num_clusters = config.get<int>("sift.kmeans.clustersize");
}

SiftSVM::~SiftSVM() {

}

std::vector<std::vector<double> > SiftSVM::get_sift_cache(const std::string& path) {
	std::string replaced = path;
	std::replace(replaced.begin(), replaced.end(), '/', '_');

	std::string cached_file = cache_dir + "/" + replaced;
	std::cout<<"Checking for cache: "<<cached_file<<std::endl;
	std::ifstream in(cached_file.c_str());
	std::string temp;
	if (in>>temp) {
		in.close();
		std::cout<<"    ....Loading from cache."<<std::endl;
		return read_2dvec(cached_file);
	}
	in.close();

	CImg<double> img(path.c_str());
	CImg<double> grey = img.spectrum() == 1 ? img : img.get_RGBtoHSI().get_channel(2);
	std::vector<SiftDescriptor> desc = Sift::compute_sift(grey);
	std::vector<std::vector<double> > dv;
	dv.reserve(desc.size());
	for (std::vector<SiftDescriptor>::iterator it = desc.begin(); it != desc.end(); it++)
		dv.push_back(to_vec(*it));
	
	write_2dvec(cached_file, dv);
	return dv;
}

void SiftSVM::preprocess(const Dataset& data) {
	std::vector<std::vector<double> > all_descriptors;
	for (Dataset::const_iterator it = data.begin(); it != data.end(); it++ ) {
		const std::vector<std::string>& files = it->second;

		for (std::vector<std::string>::const_iterator it1 = files.begin(); it1 != files.end(); it1++) {
			std::cout<<"  Preprocessing: "<<*it1<<std::endl;
			std::vector<std::vector<double> > dv = get_sift_cache(*it1);
			all_descriptors.insert(all_descriptors.end(), dv.begin(), dv.end());
		}
	}
	std::cout<<"Running K-Means."<<std::endl;
	this->visual_words = kmeans2(all_descriptors, num_clusters);

	std::cout<<"WOrking dir: "<<working_dir<<std::endl;
	std::string visual_words_file = "visual_words";
	std::cout<<"Writing model to file: "<<visual_words_file<<std::endl;
	write_2dvec(visual_words_file, this->visual_words); 
}

void SiftSVM::load_model() {
	this->visual_words = read_2dvec("visual_words");
	std::cout<<"Model loaded."<<std::endl;
}

std::vector<double> SiftSVM::get_feature_vector(const std::string& filename, bool) {
	std::vector<std::vector<double> > descv = get_sift_cache(filename);
	std::vector<double> histogram(num_clusters, 0);
	
	for (std::vector<std::vector<double> >::iterator it = descv.begin(); it != descv.end(); it++) {
		int min_pos = get_closest_vector(*it, visual_words);
		histogram[min_pos]++;
	}

	return histogram;
}

