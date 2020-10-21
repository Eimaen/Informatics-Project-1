#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main()
{
    ifstream fin("input.txt"); // основной текст
    ifstream fi_n("symbol.txt"); // то, что мы ищем через пробел, по типо "но то из..."
    string s, s1, s2, s3;
    getline(fin, s);
    getline(fi_n, s1);
    s += " ";
    s1 += " ";
    while(s.size() > 0){
        s3 = s1.substr(0, s1.find(' '));
        s1.erase(0, s1.find(' ') + 1);
        cout << s3 << endl;
        s2 = s;
        int k = 0;
        while(s2.find(s3) != -1){
            k++;
            s2.erase(s2.find(s3), s3.size());
        }
        cout << k << endl;
    }
    return 0;
}
