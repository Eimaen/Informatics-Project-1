#include <iostream>
#include <fstream>
#include<string>
using namespace std;

int main()
{
    string s, s1;
    ifstream fin("input.txt");
    getline(fin, s);
    s1 = ",:;'";
    int i = 0;
    while(i < s.size()){
        if(s1.find(s[i]) != -1){
            s.erase(i, 1);
            i--;
        }
        i++;
    }
    s += " ";
    int k = 0, k1 = 0, z = 0;
    for(int j = 0; j < s.size(); j++){
        if(s[j] == '.' || s[j] == '!' || s[j] == '?'){
           z++;
           k1 = 0;
        }
        else{
            if(s[j] == ' '){
                k++;
            }
            k1++;
        }
    }
    cout << "kol_vo bukv v slov = " << (s.size() - k - z) / k << "    kol_vo bukv v predloj = " << (s.size() - k - z) / z;
    return 0;
}
