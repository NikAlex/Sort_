#include <iostream> 
#include <fstream> 
#include <string> 
#include <vector> 
#include <locale> 
#include <iterator> 
#include <algorithm> 
#include <cstdio> 
#include <queue> 

using namespace std;

struct Stroka {
public:
	ifstream *f;
	string str;
	Stroka(const string& s, ifstream* f_) : str(s), f(f_) {}
	bool operator < (const Stroka& s) const
	{
		return (str > s.str);
	}
};

class Runner {
public:
	Runner(string name_main_file,size_t buff_size);
	auto division()->void;
	auto file_size(string name_file)->size_t;
	auto make_file(string name_file)->void;
	auto file_sort()->void;
	auto remove_temp_files()->void;
	~Runner();
private:
	fstream file;
	size_t buffer, count_of_files, closed_files;
	vector<string> lines;
	vector<string> file_names;
	priority_queue<Stroka> end_sorting;
};

inline Runner::~Runner() {
	file_names.clear();
}

inline Runner::Runner(string name_main_file,size_t buff_size) :file(name_main_file), buffer(buff_size), count_of_files(0), closed_files(0) {
	if (file.is_open()) {
		division();
	}
};

inline auto Runner::make_file(string name_file)->void {
	file_names.push_back(name_file);
	std::sort(lines.begin(), lines.end());
	ofstream temp(name_file);
	for (auto i : lines)
	{
		temp << i;
		if (i != *(--lines.end())) temp << endl;
	}
	temp.close();
	lines.clear();
}

inline auto Runner::file_size(string name_file)->size_t {
	long fsize;
	ifstream temp(name_file);
	temp.seekg(0, ios::end);
	fsize = temp.tellg();
	temp.close();
	return fsize;
}

inline auto Runner::remove_temp_files()->void {
	for (int i = 0; i < file_names.size(); ++i) {
		if (remove(file_names[i].c_str()) == -1) {
			throw;
		}
	else {
			cout << "Gj";
		}
	}
}

inline auto Runner::file_sort()->void {
	ofstream f12("out.txt");	
	string str;
	for (int i = 0; i < count_of_files; ++i) {
		ifstream* f_ = new ifstream(file_names[i]);
		getline(*f_, str);
		Stroka ff(str, f_);
		end_sorting.push(ff);
	}
	while (!end_sorting.empty()) {
		Stroka ff = end_sorting.top();
		end_sorting.pop();
		if (ff.str != "") f12 << ff.str << endl;
		if (!(*ff.f).eof())
		{
			getline(*ff.f, ff.str);
			end_sorting.push(ff);
		}
		else
		{
			(*(ff.f)).close();
		}
	}
	f12.close();
	remove_temp_files();
}

inline auto Runner::division()->void {
	string line_of_file;
	size_t temp_size_files = 0;
	while (!file.eof()) {
		getline(file, line_of_file);
		temp_size_files += line_of_file.size();
		if (temp_size_files <= buffer) {
			lines.push_back(line_of_file);
		}
		else {
			count_of_files++;
			make_file(to_string(count_of_files) + ".txt");
			lines.push_back(line_of_file);
			temp_size_files = line_of_file.size();
		}
	}
	file.close();

	if (lines.size()) {
		count_of_files++;
		make_file(to_string(count_of_files) + ".txt");
	}

	file_sort();
}  
