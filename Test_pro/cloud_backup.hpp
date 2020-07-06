#pragma once
#include<cstdio>
#include<string>
#include<vector>
#include<unordered_map>
#include<fstream>
#include<boost/filesystem.hpp>
#include<boost/algorithm/string.hpp>
#include<zlib.h>
#include"httplib.h"
#include<pthread.h>

namespace _cloud_sys{

class Fileutil{
public:
    static bool Read(const std::string& name,std::string* body)
    {
        std::ifstream fs(name,std::ios::binary);
        if(fs.is_open()==false)
        {
            std::cout<<"open file: "<<name<<" failed"<<std::endl;
            return false;
        }
        int64_t size=boost::filesystem::file_size(name);
        body->resize(size);
        fs.read(body->c_str(),size);
        if(fs.good()==false)
        {
            std::cout<<"file: "<<name<<" read data filed"<<std::endl;
            return false;
        }
        fs.close();
        return true;
    }
    static bool Write(const std::string& name,const std::string& body)
    {
        std::ofstream ofs(name,std::ios::binary);
        if(ofs.is_open()==false)
        {
            std::cout<<"open file: "<<name<<" failed"<<std::endl;
            return false;
        }
        size_t size=body.size();
        ofs.write(body.c_str(),size);
        if(ofs.good()==false)
        {
            std::cout<<"file: "<<name<<"write data filed"<<std::endl;
            return false;
        }
        ofs.close();
        return true;
    }
};



}

