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

#define NONHOT_TIME 10   //10秒未访问就是非热点文件
#define INTERVAL_TIME 30 //没30秒检测一次，间隔时间
#define BACKUP_DIR "./backup/" //备份文件路径
#define GZFILE_DIR "./gzfile/" //压缩文件路径
#define DATA_FILE "./list.backup" //数据管理模块备份文件名称
namespace _cloud_sys{

	class FileUtil{
	public:
		static bool Read(const std::string& name, std::string *body)
		{
			std::ifstream fs(name,std::ios::binary);
			if(fs.is_open()==false)
			{
				std::cout<<"open file: "<<name<<"failed"<<std::endl;
				return false;
			}
			int64_t fsize=boost::filesystem::file_size(name);
			body->resize(fsize);
			fs.read(&(*body)[0],fsize);
			if(fs.good()==false)
			{
				std::cout<<"file: "<<name<<"read data failed"<<std::endl;
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
				std::cout<<"open file: "<<name<<"fialed"<<std::endl;
				return false;
			}
			ofs.write(&body[0],body.size());
			if(ofs.good()==false)
			{
				std::cout<<"file: "<<name<<"wirte data failed"<<std::endl;
				return false;
			}
			ofs.close();
			return true;
		}
	};
	class CompressUtil{
	public:
		//文件压缩
		static bool Compress(const std::string& src,const std::string& dst)
		{
			std::string body;
			FileUtil::Read(src,&body);
			gzFile gf=gzopen(dst.c_str(),"wb"); //打开压缩包
			if(gf==NULL)
			{
				std::cout<<"open file: "<<dst<<"failed"<<std::endl;
				return false;
			}
			int wlen=0;
			while(wlen<body.size())
			{
				//若一次没有将数据压缩完，则从上次未压缩的位置开始继续压缩
				int ret=gzwrite(gf,&body[wlen],body.size()-wlen);
				if(ret==0)
				{
					std::cout<<"file"<<dst<<"write compress data failed"<<std::endl;
					return false;
				}
				wlen+=ret;
			}
			gzclose(gf);
			return true;
		}
		//文件解压缩
		static bool UnCompress(const std::string& src,const std::string& dst)
		{
			std::ofstream ofs(dst,std::ios::binary);
			if(ofs.is_open()==false)
			{
				std::cout<<"open file: "<<dst<<"failed"<<std::endl;
				return false;
			}
			gzFile gf=gzopen(src.c_str(),"rb");
			if(gf==NULL)
			{
				std::cout<<"open file"<<src<<"failed"<<std::endl;
				ofs.close();
				return false;
			}
			int ret;
			char temp[4096]={0};
			while((ret=gzread(gf,temp,4096))>0)
			{
				ofs.write(temp,ret);	
			}
			ofs.close();
			gzclose(gf);
			return true;
		}
			
	};

	class DataManager{
	public:
			
		DataManager(const std::string& path):_back_file(path){
			pthread_rwlock_init(&_rwlock,NULL);
		}
		~DataManager()
		{
			pthread_rwlock_destroy(&_rwlock);	
		}
		bool Exists(const std::string& name) //判断文件是否存在
		{
			pthread_rwlock_rdlock(&_rwlock);
			auto it=_file_list.find(name);
			if(it==_file_list.end())
			{
				pthread_rwlock_unlock(&_rwlock);
				return false;	
			}
			pthread_rwlock_unlock(&_rwlock);
			return true;
		}
		bool IsCompress(const std::string& name) //判断文件是否被压缩
		{
		  	//压缩后文件名与包名不同
			//相同则表示不是压缩文件
			pthread_rwlock_rdlock(&_rwlock);	
			auto it=_file_list.find(name);
			if(it==_file_list.end())
			{
				pthread_rwlock_unlock(&_rwlock);
				return false;
			}
			if(it->first==it->second)
			{
				pthread_rwlock_unlock(&_rwlock);
				return false;
			}
			pthread_rwlock_unlock(&_rwlock);
			return true;
		}
		bool NonCompress(std::vector<std::string> *list) //获取未压缩文件列表
		{
			//遍历文件列表，将未压缩的文件写入list中
			pthread_rwlock_rdlock(&_rwlock);
			auto it=_file_list.begin();
			while(it!=_file_list.end())
			{
				if(it->first==it->second)
					list->push_back(it->first);
				it++;
			}
			pthread_rwlock_unlock(&_rwlock);
			return true;
			
		}
		bool Insert(const std::string& src,const std::string& dst) //插入更新数据
		{
			pthread_rwlock_wrlock(&_rwlock);
			_file_list[src]=dst;
			pthread_rwlock_unlock(&_rwlock);
			Storage();
			return true;
		}
		bool GetAllName(std::vector<std::string> *list) //获取所有文件名称
		{
			pthread_rwlock_rdlock(&_rwlock);
			auto it=_file_list.begin();
			while(it!=_file_list.end())
			{
				list->push_back(it->first);
				it++;
			}
			pthread_rwlock_unlock(&_rwlock);
			return true;
				
		}
        //根据源文件获取压缩包名称
        bool GetGzName(const std::string& src, std::string* dst)
        {
            pthread_rwlock_rdlock(&_rwlock);
            auto it = _file_list.find(src);
            if(it == _file_list.end())
            {
                pthread_rwlock_unlock(&_rwlock);
                return false;
            }
            *dst = it -> second;
            pthread_rwlock_unlock(&_rwlock);
            return true;
        }
		bool Storage()  //数据改变后持久化存储
		{
			pthread_rwlock_wrlock(&_rwlock);
			std::stringstream tmp;
			auto it=_file_list.begin();
			while(it!=_file_list.end())
			{
				tmp<<it->first<<" "<<it->second<<"\r\n";
				it++;
			}
			pthread_rwlock_unlock(&_rwlock);
			FileUtil::Write(_back_file,tmp.str());
			return true;
		}
		bool InitLoad() //启动时数据加载
		{
            pthread_rwlock_wrlock(&_rwlock);
			std::string body;
			if(FileUtil::Read(_back_file,&body)==false)
            {
                pthread_rwlock_unlock(&_rwlock);
                return false;
            }
            //对字符串进行处理
			//boost::split(vector,src,sep,flag)
			std::vector<std::string> list;
			boost::split(list,body,boost::is_any_of("\r\n"),boost::token_compress_off);
			for(auto e:list)
			{
				size_t pos=e.find(" ");
				if(pos==std::string::npos)
					continue;
				std::string key=e.substr(0,pos);
				std::string val=e.substr(pos+1);
				Insert(key,val);
			}
			return true;
		}
		
	private:
		std::string _back_file; //持久化数据存储文件名称
		std::unordered_map<std::string,std::string> _file_list; //数据管理容器
		pthread_rwlock_t _rwlock; //读写锁
	};
	
	DataManager data_manage(DATA_FILE);
	class NonHotCompress{
	public:
		NonHotCompress(const std::string gz_dir,const std::string bu_dir)
		:_gz_dir(gz_dir)
		,_bu_dir(bu_dir)
		{}

		bool Start() //开始压缩模块
		{
			while(1)
			{
				std::vector<std::string> list;
				data_manage.NonCompress(&list);
				for(int i=0;i<list.size();i++)
				{
					bool ret=FileIsHot(_bu_dir+list[i]);
					if(ret == false)
					{
						std::cout<<"is not hot file"<<std::endl;
						std::string src_filename=list[i];   //源文件名称
						std::string dst_filename=list[i]+".gz"; //压缩包名称
						
						std::string src_name=_bu_dir+src_filename; //源文件路径名称
						std::string dst_name=_gz_dir+dst_filename;  //压缩包路径名称
						
						if(CompressUtil::Compress(src_name,dst_name)==true)
						{
							data_manage.Insert(src_filename,dst_filename); //更新数据信息
							unlink(src_name.c_str());				//删除源文件
						}
					}
				}
				sleep(INTERVAL_TIME);	
			}
			return true;
		}
	private:
		bool FileIsHot(const std::string& name) //判断文件是否是热点文件
		{
			time_t cur_t = time(NULL);	
			struct stat st;
			if(stat(name.c_str(),&st) < 0)
			{
				std::cout<<"get file: "<<name<<" stat failed"<<std::endl;
				return false;
			}
			if(cur_t - st.st_atime > NONHOT_TIME)
				return false;
			return true;
		}
	private:
		std::string _gz_dir;  //压缩后的文件存储路径
		std::string _bu_dir;  //压缩前的文件存储路径
	};

	class Server
	{
	public:
		bool Start() //启动网络通信模块
        {
            _server.Put("/(.*)",Upload);
            _server.Get("/list",List);
            _server.Get("/download/(.*)",Download);

            _server.listen("0.0.0.0",9000);
            return true;

        }
	private:
		//文件上传
		static void Upload(const httplib::Request &req,httplib::Response &rsp)
        {
            std::string filename=req.matches[1];  //纯文件名
            std::string pathname=BACKUP_DIR+filename;  //文件存储路径名
            FileUtil::Write(pathname,req.body);
            data_manage.Insert(filename,filename);
            rsp.status=200;
            return;
        }
		//文件浏览
		static void List(const httplib::Request &req,httplib::Response &rsp)
        {
            std::vector<std::string> list;
            data_manage.GetAllName(&list);
            std::stringstream tmp;
            tmp<<"<html><body><hr />";
            for(int i=0;i<list.size();i++)
            {
                tmp<<"<a href='/download/"<<list[i]<<"'>"<<list[i]<<"</a>";
                tmp<<"<hr />";
            }
            tmp<<"<hr /></body></html>";
            rsp.set_content(tmp.str().c_str(),tmp.str().size(),"text/html");
            rsp.status=200;
            return ;
        }
		//文件下载
		static void Download(const httplib::Request &req,httplib::Response &rsp)
        {
            std::string filename=req.matches[1];
            if(data_manage.Exists(filename)==false)
            {
                rsp.status=404;
                return;
            }
            std::string pathname=BACKUP_DIR+filename;
            if(data_manage.IsCompress(pathname)!=false)
            {
                std::string gzfile;
                data_manage.GetGzName(filename,&gzfile);
                std::string gzpathname=GZFILE_DIR+gzfile;
                CompressUtil::UnCompress(gzpathname,pathname);
                unlink(gzpathname.c_str());
                data_manage.Insert(filename,filename);
            }
            FileUtil::Read(pathname,&rsp.body);
            rsp.set_header("Content-Type","application/octet-stream");
            rsp.status=200;
            return;
            
        }
	private:
		std::string _file_dir; //文件上传备份路径
		httplib::Server _server; 
	};
	
}
