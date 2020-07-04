#pragma once
#include"httplib.h"
#include<iostream>
#include<sstream>
#include<fstream>
#include<string>
#include<vector>
#include<unordered_map>
#include<boost/filesystem.hpp>
#include<boost/algorithm/string.hpp>


class FileUtil{
public:
	static bool Read(const std::string& name, std::string *body)
	{
		std::ifstream fs(name, std::ios::binary);
		if (fs.is_open() == false)
		{
			std::cout << "open file: " << name << "failed" << std::endl;
			return false;
		}
		int64_t fsize = boost::filesystem::file_size(name);
		body->resize(fsize);
		fs.read(&(*body)[0], fsize);
		if (fs.good() == false)
		{
			std::cout << "file: " << name << "read data failed" << std::endl;
			return false;
		}
		fs.close();
		return true;
	}
	static bool Write(const std::string& name, const std::string& body)
	{
		std::ofstream ofs(name, std::ios::binary);
		if (ofs.is_open() == false)
		{
			std::cout << "open file: " << name << "fialed" << std::endl;
			return false;
		}
		ofs.write(&body[0], body.size());
		if (ofs.good() == false)
		{
			std::cout << "file: " << name << "wirte data failed" << std::endl;
			return false;
		}
		ofs.close();
		return true;
	}
};

class DataManager{
public:
	DataManager(const std::string& filename)
		:_store_file(filename)
	{}
	bool Insert(const std::string& key, const std::string& val)
	{
		_backup_list[key] = val;
		Storage();
		return true;
	}
	bool GetEtag(const std::string& key, std::string* val)
	{
		auto it = _backup_list.find(key);
		if (it == _backup_list.end())
			return false;
		*val = it->second;
		return false;
	}
	bool Storage()
	{
		std::stringstream tmp;
		auto it = _backup_list.begin();
		while (it != _backup_list.end())
		{
			tmp << it->first << " " << it->second << "\r\n";
			it++;
		}
		FileUtil::Write(_store_file, tmp.str());
		return true;
	}
	bool InitLoad()
	{
		std::string body;
		if (FileUtil::Read(_store_file, &body) == false)
			return false;
		std::vector<std::string> list;
		boost::split(list, body, boost::is_any_of("\r\n"), boost::token_compress_off);
		for (auto e : list)
		{
			size_t pos = e.find(" ");
			std::string key = e.substr(0, pos);
			std::string val = e.substr(pos + 1);
			Insert(key, val);
		}
		return true;

	}
private:
	std::string _store_file;	//持久化存储文件名称
	std::unordered_map<std::string, std::string> _backup_list;  //备份的文件信息
};

class CloudClent{
public:
	CloudClent(const std::string& filename, const std::string& store_file, const std::string& srv_ip,
		uint16_t srv_port)
		:_listen_dir(filename)
		, data_manage(store_file)
		, _srv_ip(srv_ip)
		, _srv_port(srv_port)
	{}
	bool Start()
	{
		data_manage.InitLoad();
		while (1)
		{

			std::vector<std::string> list;
			GetBackupFileList(&list);
			for (int i = 0; i < list.size(); i++)
			{
				std::string name = list[i];
				std::string pathname = _listen_dir + name;
				std::cout << pathname << "is need to backup!" << std::endl;
				//获取正文
				std::string body;
				FileUtil::Read(pathname, &body);
				//实例化Client对象
				httplib::Client client(_srv_ip.c_str(), _srv_port);
				std::string req_path = "/" + name;
				auto rsp = client.Put(req_path.c_str(), body, "application/octet-stream");
				if (rsp == NULL || (rsp != NULL&&rsp->status != 200))
				{
					std::cout << pathname << "backup failed!" << std::endl;
					continue;
				}
				std::string etag;
				GetEtag(pathname, &etag);
				data_manage.Insert(name, etag);
				std::cout << pathname << "backup success!" << std::endl;
			}
			Sleep(1000);
		}
		return true;

	}
	bool GetBackupFileList(std::vector<std::string>* list)
	{
		boost::filesystem::directory_iterator begin(_listen_dir);
		boost::filesystem::directory_iterator end;
		for (; begin != end; ++begin)
		{
			if (boost::filesystem::is_directory(begin->status()))
				continue;
			std::string pathname = begin->path().string();
			std::string name = begin->path().filename().string();

			std::string cur_etag, old_etag;
			GetEtag(pathname, &cur_etag);
			data_manage.GetEtag(name, &old_etag);
			if (cur_etag != old_etag)
				list->push_back(name);
		}
		return true;
	}
	bool GetEtag(const std::string& pathname, std::string* etag)
	{
		int64_t fsize = boost::filesystem::file_size(pathname);
		time_t mtime = boost::filesystem::last_write_time(pathname);
		*etag = std::to_string(fsize) + "-" + std::to_string(mtime);
		return true;
	}
private:

	std::string _listen_dir;
	DataManager data_manage;
	std::string _srv_ip;
	uint16_t _srv_port;
};