#include <iostream>
#include <string>
#include <string.h>
#include <jsoncpp/json/json.h>

using namespace std;

void make(Json::Value &root, string str, int x)
{
    root[str.c_str()] = x;
}

int main()
{
    // Json::FastWriter write;
    Json::StyledWriter write;
    Json::Reader read;
    Json::Value wroot;
    Json::Value rroot;
    make(wroot, "C++", 0);
    make(wroot, "zhutian", 1);
    make(wroot, "awa", 2);
    make(wroot, "ovo", 3);
    make(wroot, "qwq", 4);
    make(wroot, ">_<", 5);
    string wt = write.write(wroot);
    cout << "root: " << wt << endl;
    read.parse(wt, rroot);
    cout << "C++:" << rroot["C++"] << endl;
    return 0;
}