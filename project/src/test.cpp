#include"cloud_backup.hpp"
#include<thread>
void test_compress(char* argv[])
{
	_cloud_sys::CompressUtil::Compress(argv[1],argv[2]);
	std::string s=argv[2];
	s+=".txt";
	_cloud_sys::CompressUtil::UnCompress(argv[2],s);
}

/*void test_data()
{	
	_cloud_sys::DataManager data_manage("./test.txt");
	data_manage.Insert("a.txt","a.txt");
	data_manage.Insert("b.txt","b.txt.gz");
	data_manage.Insert("c.txt","c.txt.gz");
	data_manage.Insert("d.txt","d.txt.gz");
	data_manage.Storage();
	data_manage.InitLoad();
	std::vector<std::string> list;
	data_manage.GetAllName(&list);
	for(auto& e : list)
	{
		std::cout<<e<<" ";	
	}
	std::cout<<std::endl;
	list.clear();
	data_manage.NonCompress(&list);
	for(auto& e : list)
	{
		std::cout<<e<<" ";	
	}
	std::cout<<std::endl;
}
*/
void m_non_compress()
{
	_cloud_sys::NonHotCompress ncom(GZFILE_DIR,BACKUP_DIR);
	ncom.Start();
	return;
}
void thr_http_server()
{
    _cloud_sys::Server srv;
    srv.Start();
    return;

}

int main(int argc,char* argv[])
{
	if(boost::filesystem::exists(GZFILE_DIR)==false)
	{
		boost::filesystem::create_directory(GZFILE_DIR);
	}
	if(boost::filesystem::exists(BACKUP_DIR)==false)
	{
		boost::filesystem::create_directory(BACKUP_DIR);	
	}
	
	
	std::thread th_compress(m_non_compress);
    std::thread th_server(thr_http_server);
	th_compress.join();
    th_server.join();
	return 0;
}
