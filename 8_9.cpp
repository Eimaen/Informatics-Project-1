#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

string text;
ifstream fin("input.txt");
ifstream fout("output.txt");

// Split string by words (separated by "divider")
vector<string> split(string s, string divider)
{
    vector<string> ret;
    int pos = 0;
    string token;
    while ((pos = s.find(divider)) != string::npos) {
        token = s.substr(0, pos);
        ret.push_back(token);
        s.erase(0, pos + divider.length());
    }
    return ret;
}

// Build word usage statistics
map<string,int> buildWordStatistics(vector<string> ss)
{
    map<string, int> ret;
    for (int i = 0; i < ss.size(); i++)
        ret[ss[i]]++;
    return ret;
}

// Sort values and, using comparator, output them
bool comparator(const pair<string, int>& a, const pair<string, int>& b) { return a.second > b.second; }
vector<pair<string, int>> sortMapValues(map<string, int> ms)
{
    vector<pair<string, int>> mapRebuild(ms.begin(), ms.end());
    sort(mapRebuild.begin(), mapRebuild.end(), comparator);
    return mapRebuild;
}

// Types of symbols
enum letter_type {
    CHAR_UNDEFINED,
    CHAR_SYMBOL,
    CHAR_VOWEL,
    CHAR_CONSONANT
};

// Get type of a given symbol
int getCharType(char c)
{
    string vowel = "аоуыэяёюиеАОУЫЭЯЁЮИЕ";
    string consonant = "бвгджзйклмнпрстфхцчшщБВГДЖЗЙКЛМНПРСТФХЦЧШЩ";
    string symblos = ":,.-ъЪьЬ";
    if (vowel.find(c) != string::npos)
        return CHAR_VOWEL;
    else if (consonant.find(c) != string::npos)
        return CHAR_CONSONANT;
    else if (symblos.find(c) != string::npos)
        return CHAR_SYMBOL;
    else
        return CHAR_UNDEFINED;
}

// Analysis for words
vector<pair<string, string>> analyseWordLetters(vector<string> ss)
{
    vector<pair<string, string>> ret;
    for (int i = 0; i < ss.size() - 1; i++)
    {
        int firstWordLength = ss[i].length(), secondWordLength = ss[i + 1].length(); // Get word lengths
        if (firstWordLength < 2 || secondWordLength < 2 || firstWordLength < 1 || secondWordLength < 1)
            continue; // If length is 1 or less, we couldn't get ss[i][length - 2] element, so, we don't need theese words
        bool wordOk1 = getCharType(ss[i][firstWordLength - 1]) == CHAR_VOWEL && getCharType(ss[i][firstWordLength - 1]) == CHAR_VOWEL;
        bool wordOk2 = getCharType(ss[i + 1][0]) == CHAR_CONSONANT && getCharType(ss[i + 1][1]) == CHAR_CONSONANT;
        if (wordOk1 && wordOk2)
            ret.push_back(make_pair(ss[i], ss[i + 1]));
    }
    return ret;
}

// Application's entry point
int main()
{
    // Set output codepage to Russian and read the text
    setlocale(LC_ALL, "Russian"); 
    getline(fin, text); 

    // Split the text using spaces as separators
    vector<string> ss = split(text, " ");

    cout << "Word ending analysis statistics: \n";

    // Word endings and beginnings analysis
    vector<pair<string, string>> ar = analyseWordLetters(ss);
    if (ar.size() > 0)
        for (int i = 0; i < ar.size(); i++)
            cout << "Words \"" << ar[i].first << "\" and \"" << ar[i].second << "\" are near\n";
    else
        cout << "No words matched pattern!\n";

    // Separate results
    cout << "\nWord appearance statistics: \n";

    // Count word repeats
    map<string, int> ms = buildWordStatistics(ss);
    vector<pair<string, int>> mapRebuild = sortMapValues(ms);
    if (mapRebuild.size() > 0)
        for (int i = 0; i < mapRebuild.size(); i++)
            cout << mapRebuild[i].first << " is met " << mapRebuild[i].second << " times\n";
    else
        cout << "No words met *lul*\n";
}
