#include "Storage.h"
#include <core_obj/GuidObject.h>
#include <fstream>

#ifdef WIN32
#include <filesystem>
namespace fs =  std::tr2::sys;
#else
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
namespace fs = boost::filesystem;
#endif // DEBUG

namespace core_obj {

bool ListDir(string dir,vector<fs::path>& files)
{
	fs::path full_path( dir );
	if(!fs::exists( full_path ))
		return false;
	if(!fs::is_directory(full_path))
		return false;
	fs::directory_iterator end_iter;
	for ( fs::directory_iterator dir_itr( full_path );
		dir_itr != end_iter;
		++dir_itr )
	{
		try
		{
			if ( fs::is_directory( dir_itr->status() ) )
				continue;
			if ( dir_itr->path().extension() != BINLOG_EXT_NAME)
				continue;
			
			files.push_back(dir_itr->path());
		}
		catch ( const std::exception & ex )
		{	
			std::cout << dir_itr->path().filename() << " " << ex.what() << std::endl;
			throw ex;	//生成core吧
		}
	}
	return true;
}

Storage::Storage(CreateFunction f):create_func_(f)
{
}

Storage::~Storage()
{
}

bool Storage::IsHaveFile(string path)
{
	fs::path full_path( path );
	if(fs::exists(full_path))
		return true;
	return false;
}

bool Storage::CreatDir(string path)
{
	fs::path full_path( path );
	if(fs::exists(full_path))
	{
		if(!fs::is_directory(full_path))
			return false;
		return true;
	}
	return fs::create_directories(full_path);	
}

void Storage::RemoveDir(string path)
{
	fs::path full_path( path );
	if(fs::exists(full_path))
	{
		fs::remove_all(full_path);
	}
}

bool Storage::Goto(string f)
{	
	//尝试一下能不能打开，并且是不是文件夹，如果不是则返回错误 
	char tmp[2048];
	strcpy(tmp,f.c_str());
	if(!CreatDir(tmp))
		return false;
	work_directory_ = tmp;
	return true;
}

bool Storage::Load(const string& file_name,vector<GuidObject*> &vec)
{
	string file = work_directory_ + "/" + file_name + BINLOG_EXT_NAME;
	
	std::ifstream infile(file);
	if(!infile.is_open())
		return false;

	string guid,ints,strs;
	while(!infile.eof())
	{
		if(!getline(infile,guid))
			break;
		GuidObject *obj = create_func_(guid);
		if(!getline(infile,ints))
			return false;
		if(!getline(infile,strs))
			return false;

		obj->FromString(ints,strs);
		vec.push_back(obj);
	}
	infile.close();
	return true;
}

bool Storage::AddContent(const string &file_name, const string &content)
{
	string file = work_directory_ + "/" + file_name;
	std::ofstream outfile(file,std::ios::app | std::ios::out);
	if (!outfile.is_open())
		return false;

	outfile << content;
	outfile.flush();
	outfile.close();
	
	return true;
}

//读某文件的内容
bool Storage::ReadFile(const string &file_name, ReadFunction fun)
{
	string file = work_directory_ + "/" + file_name;
	std::ifstream infile(file);
	if (!infile.is_open())
		return false;

	string content;
	while(!infile.eof())
	{
		if(!getline(infile,content))
			break;

		if(!fun(content))
			break;
	}
	return true;
}

bool Storage::Remove(const string& file_name)
{
	//先把不用的先干掉
	fs::path delfilename = work_directory_ + "/" + file_name;
	if(!fs::exists(delfilename))
		return false;
	else
		fs::remove(delfilename);

	return true;
}

//遍历路径下的所有文件（不遍历文件夹）
void Storage::ForEachDir(std::function<void(string , string)> fun)
{
	fs::path full_path(work_directory_);
	if(!fs::exists( full_path ))
		return ;

	fs::recursive_directory_iterator end_iter;
	for(fs::recursive_directory_iterator iter(full_path);iter!=end_iter;iter++)
	{
		try
		{
			if (fs::is_directory(iter->status()))
			{
				//文件夹就不回调了
			}
			else
			{
				string complete_filename = fs::system_complete(iter->path()).string();
#ifdef WIN32
				fun(complete_filename, iter->path().filename());
#else
				fun(complete_filename, iter->path().filename().string());
#endif
			}
		}
		catch ( const std::exception & ex )
		{
			std::cerr << ex.what() << std::endl;
			continue;
		}
	}
}

//获得路径下的所有文件夹名字
void Storage::GetDirs(const string &dir, std::function<void(const string&)> fun)
{
	fs::path full_path(dir);
	if(!fs::exists( full_path ))
		return ;
	if(!fs::is_directory(full_path))
		return ;
	fs::directory_iterator end_iter;
	for (fs::directory_iterator dir_itr(full_path);
		dir_itr != end_iter;
		++dir_itr)
	{
		try
		{
			if (!fs::is_directory(dir_itr->status()))
				continue;
			string name = dir_itr->path().filename().c_str();
			fun(name);
		}
		catch (const std::exception & ex)
		{	
			std::cout << dir_itr->path().filename() << " " << ex.what() << std::endl;
			throw ex;	//生成core吧
		}
	}	
}

//拷贝文件
bool Storage::CopyAFile(string& src, string& dst)
{
	fs::path srcpath(src);
	if(!fs::exists(srcpath))
		return false;
	fs::path dstpath(dst);
	fs::copy_file(srcpath, dstpath, fs::copy_option::overwrite_if_exists);
	return true;
}

//拷贝源路径下的所有文件到目的路径
bool Storage::CopyDirFilse(string& src, string& dst)
{
	RemoveDir(dst);
	fs::path dstpath(dst);
	if(!fs::exists(dstpath))
		fs::create_directories(dstpath);

	fs::path srcpath(src);
	fs::recursive_directory_iterator end_iter;
	for(fs::recursive_directory_iterator iter(srcpath);iter!=end_iter;iter++){
		try{
			if (fs::is_directory( iter->status() ) ){
				continue;
			}else{
				string copy_filename = iter->path().filename().c_str();
//				printf("copy_filename = %s   dst_dir = %s\n", copy_filename.c_str(), dst.c_str());
				string copy_hddir = dst + "/" + copy_filename; 
				fs::path copy_path(copy_hddir);
				fs::copy_file(iter->path(), copy_path, fs::copy_option::overwrite_if_exists);			
			}
		} catch ( const std::exception & ex ){
			std::cerr << ex.what() << std::endl;
			continue;
		}
	}
	return true;
}

//保存一个文件到硬盘
bool Storage::SaveFile(const string& file_name, const string& file_content)
{
	fs::path path(work_directory_);
	if(!fs::exists(path))
	{
		fs::create_directories(path);
	}
	
	string spath = work_directory_+"/"+file_name;
	std::ofstream file(spath,std::ios::trunc | std::ios::out);
	if (!file.is_open())
		return false;

	file << file_content;
	file.flush();
	file.close();
	return true;
}

}
