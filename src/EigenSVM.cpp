#include <algorithm>
#include <utils.h>
#include <CImg.h>
#include <EigenSVM.h>

EigenSVM::EigenSVM(const std::vector<std::string>& cl, const std::string& wd): SVM(cl, wd) {
    
}

EigenSVM::~EigenSVM() {
    
}

void EigenSVM::preprocess(const Dataset& data) {
    
    cout<<"Hey";
    
    string dir = string("./train");
    
    int w=40;
    int eigenmax=50;
    
    vector<string> allpathnfiles = vector<string>();
    vector<vector<double> > trainingvecs=vector<vector<double> >();
    Dataset data=filenames;
    
    for (Dataset::const_iterator it = data.begin(); it != data.end(); it++ ) {
        const std::vector<std::string>& files = it->second;
        
        for (std::vector<std::string>::const_iterator it1 = files.begin(); it1 != files.end(); it1++) {
            std::cout<<"  Preprocessing: "<<*it1<<std::endl;
            //std::vector<std::vector<double> > dv = get_sift_cache(*it1);
            //all_descriptors.insert(all_descriptors.end(), dv.begin(), dv.end());
            allpathnfiles.push_back(*it1);
        }
    }
    
    //function into vector of vectors
    //getdirandfiles(dir,allpathnfiles);
    for(unsigned int i=0;i<allpathnfiles.size();i++)
    {
        CImg<double> input(allpathnfiles[i].c_str());
        CImg<double> resized = input.resize(w, w, 1, 3);
        
        CImg<double> grey = resized.spectrum() == 1? resized: resized.get_RGBtoHSI().get_channel(2);
        std::vector<double> result(grey.width() * grey.height(), 0);
        std::copy(grey.begin(), grey.end(), result.begin());
        //cout<<grey.size()<<endl;
        trainingvecs.push_back(result);
        
        //till here
    }
    cout<<trainingvecs.size();
    vector<double> avg=vector<double>(trainingvecs[0].size(),0);
    int len=trainingvecs.size();
    for(unsigned int i=0;i<avg.size();i++)
        cout<<avg[i]<<" ";
    
    for(int i=0;i<len;i++)
    {cout<<i<<endl;
        for(unsigned int j=0;j<trainingvecs[i].size();j++)
            
        {
            //cout<<trainingvecs[i][j]<<" ";
            
            avg[j]=avg[j]+(trainingvecs[i][j]/len);}
        //cout<<endl;
    }
    for(int i=0;i<len;i++)
    {//cout<<i<<endl;
        for(unsigned int j=0;j<trainingvecs[i].size();j++)
            
        {
            //cout<<trainingvecs[i][j]<<" ";
            
            trainingvecs[i][j]=trainingvecs[i][j]-avg[j];}
        //cout<<endl;
    }
    int datasize=trainingvecs.size();
    int vecsize=avg.size();
    CImg<double> mat(datasize,vecsize);
    //cout<<mat.width()<<" "<<mat.height()<<" "<<mat.depth()<<" "<<mat.spectrum<<endl;
    //cout<<mat.width()<<endl;
    //cout<<mat.height()<<endl;
    
    cout<<"Transform vec to CIMG"<<endl;
    for(int i=0;i<datasize;i++)
    {
        for(int j=0;j<vecsize;j++)
            mat(i,j)=trainingvecs[i][j];
    }
    CImg<double> t(mat,false);
    t.transpose();
    CImg<double> covmat=mat*t;
    CImg<float> egval;
    CImg<float> egvec;
    
    covmat.symmetric_eigen(egval,egvec);
    
    
    /*for(int i=0;i<100;i++)
     {
     for(int j=0;j<100;j++)
     cout<<egvec(i,j)<<"**";
     cout<<endl;
     }
     */
    /* to save the eigen vector imagesss*/
    /*start here*/
    for(int i=0;i<eigenmax;i++)
    {   cout<<i<<" "<<egval(0,i)<<endl;
        
        double maxpixel=egvec(0,i);
        CImg<double> eigenfood(w,w);
        
        for(int j=0;j<w;j++)
            for(int k=0;k<w;k++)
            {
                eigenfood(j,k)=egvec(j*10+k,i);
                cout<<"eigenfood"<<egvec(j*10+k,i)<<endl;
                if(maxpixel<eigenfood(j,k))
                    maxpixel=eigenfood(j,k);
                //cout<<j<<" "<<k<<" "<<j*10+k<<" "<<i<<" "<<egvec(j*10+k,i)<<" ";
            }
        
        
        ;
        // cout<<"Maxpixel:"<<maxpixel<<endl;
        for(int j=0;j<w;j++)
            for(int k=0;k<w;k++)
            {eigenfood(j,k)=eigenfood(j,k)*255/maxpixel;
            }
        
        
        
        stringstream ss;
        ss << i;
        string s=ss.str()+".png";
        //s.c_str()
        
        
        eigenfood.save(s.c_str());
        
        
        
        //cout<<endl;
        
    }
    
    /*end here*/
    
    
    
    /*create a new Cimg for solver to use*/
    /////here start
    
    CImg<double> eigenvecforsolve(eigenmax,w*w);
    for(int j=0;j<eigenmax;j++)
        for(int i=0;i<w*w;i++)
            eigenvecforsolve(j,i)=egvec(j,i);
    
    
    std::vector<std::vector<double> > eigenvecforsolveimg;
    for(int i=0;i<eigenvecforsolve.width();i++)
    {
        std::vector<double>eigenrow;
        for(int j=0;j<eigenvecforsolve.height();j++)
        {
            eigenrow.push_back(eigenvecforsolve(i,j));
        }
        eigenvecforsolveimg.push_back(eigenrow);
        
    }
    std::string eigen_file = "eigenvecforsolve";
    std::cout<<"Writing model to file: "<<eigen_file<<std::endl;
    write_2dvec(eigen_file,eigenvecforsolveimg );
    
    
    std::vector<std::vector<double> > average;
    average.push_back(avg);
    std::string averagefile = "average";
    std::cout<<"Writing model to file: "<<averagefile<<std::endl;
    write_2dvec(averagefile,average );
    
    this->eigenvecforsolve=eigenvecforsolve;
    this->avg=avg;
}

void EigenSVM::load_model() {
    std::vector<std::vector<double> > eigenvecforsolveimg= read_2dvec("eigenvecforsolve");
    
    std::vector<std::vector<double> > average=read_2dvec("average");
    
    CImg e(eigenvecforsolveimg.size(),eigenvecforsolveimg[0].size());
    for(unsigned int i=0;i<eigenvecforsolveimg.size();i++)
    {
        //std::vector<double>eigenrow;
        for(unsigned int j=0;j<eigenvecforsolveimg[i].size();j++)
        {
            //eigenrow.push_back(eigenvecforsolve(i,j));
            e(i,j)=e[i][j];
        }
        //eigenvecforsolveimg.push_back(eigenrow);
        
    }
    this->avg=average[0];
    
    this->eigenvecforsolve->e;
    std::cout<<"Model loaded."<<std::endl;
    
}
std::vector<double> EigenSVM::get_feature_vector(const std::string& filename1, bool is_train_mode) {
    //return std::vector<double>();
    //std::string filename1="./train/bagel/106970.jpg";
    int w=40;
    int eigenmax=50;
    CImg<double> input(filename1.c_str());
    CImg<double> resized = input.resize(w, w, 1, 3);
    
    CImg<double> grey = resized.spectrum() == 1? resized: resized.get_RGBtoHSI().get_channel(2);
    //std::vector<double> result(grey.width() * grey.height(), 0);
    //std::copy(grey.begin(), grey.end(), result.begin());
    CImg<double> lhs(1,w*w);
    
    
    for(int i=0;i<w;i++)
        for(int j=0;j<w;j++)
        {
            lhs(0,i*10+j)=grey(i,j)-(this->avg[i*10+j]);
        }
    CImg<double> ans=lhs.solve(this->eigenvecforsolve);
    
    
    std::vector<double> finalresult(ans.width() * ans.height(), 0);
    
    std::copy(ans.begin(), ans.end(), finalresult.begin());
    cout<<"finalresult"<<endl;
    
    return finalresult;
    
}
