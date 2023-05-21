#include <iostream>
#include <vector>
#include <fstream>
#include "path.hpp"

using namespace std;

void print(const vector<pair<string, int>>& v)
{
    for(int i = 0; i < v.size(); i++) {
        cout << "[" << v[i].first << " | " << v[i].second << "]" << endl;
    }
}

void copyDirectory(const string& source, const string& destination)
{
    if(pathExists(source) && isDirectory(source)) {
        vector<string> paths;
        filesystem::recursive_directory_iterator iter(source);
        int prev_lvl = iter.depth();
        string current;
        for(const auto& i : iter) {
            current = i.path().filename().string();
            if(prev_lvl < iter.depth()) {
                paths.push_back(joinPath(paths.back(), i.path().filename().string()));
            } else if(prev_lvl > iter.depth()) {
                for(int j = prev_lvl - iter.depth(); j > 0; j--) {
                    paths.pop_back();
                }
                if(paths.empty()) {
                    paths.push_back(i.path().filename().string());
                } else if(paths.size() < 2) {
                    paths.back() = i.path().filename().string();
                } else {
                    paths.back() = joinPath(paths[paths.size()-2], i.path().filename().string());
                }
            } else {
                if(paths.empty()) {
                    paths.push_back(i.path().filename().string());
                } else if(paths.size() < 2) {
                    paths.back() = i.path().filename().string();
                } else {
                    paths.back() = joinPath(paths[paths.size()-2], i.path().filename().string());
                }
            }
            current = joinPath(destination, paths.back());
            cout << current << endl;
            if(filesystem::is_directory(i.path())) {
                filesystem::create_directories(current);
            } else {
                std::ifstream src(i.path(), std::ios::binary);
                std::ofstream dst(current, std::ios::binary);
                dst << src.rdbuf();
            }
            prev_lvl = iter.depth();
        }
    } else {
        cerr << "[Error] Path does not exist" << endl;
    }
}

void test(const string& path)
{
    filesystem::recursive_directory_iterator iter(path);
    for(const auto& i : filesystem::recursive_directory_iterator(path)) {
        cout << "Depth: " << iter.depth() << endl;
        cout << i.path().string() << endl;
        cout << endl;
        iter++;
    }
}

int main(int argc, char** argv)
{
    vector<string> args = {"template1"};
    args.assign(argv+1, argv+argc);
    string program_name = argv[0];

    if(args.empty()) {
        return 0;
    }
    
    string template_name = args[0];
    cout << joinPath(getSourcePath(), "Templates/" + template_name) << endl;
    cout << getCurrentPath() << endl;
    copyDirectory(joinPath(getSourcePath(), "Templates/" + template_name), getCurrentPath());
    //test(joinPath(getCurrentPath(), args[0]));
    return 0;
}