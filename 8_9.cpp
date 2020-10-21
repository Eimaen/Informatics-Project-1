#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <Windows.h>

using namespace std;

string text;
ifstream fin("input.txt");
ofstream fout("output.xls");

class TableRenderer
{
public:
    void addRow(vector<string> row);
    string render();
    void write(ostream& stream);
    TableRenderer();
    ~TableRenderer();

private:
    vector<vector<string>> _table;
    size_t _szTable;
    size_t _szRowCount;
};

// Add row to the table
void TableRenderer::addRow(vector<string> row)
{
    _table.push_back(row);
    _szRowCount++;
}

// New versions of Excel warns, that this format is unsafe
string TableRenderer::render()
{
    string excelTableData = "<?xml version=\"1.0\"?>\n";
    excelTableData += "<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\" ";
    excelTableData += "xmlns:o=\"urn:schemas-microsoft-com:office:office\" ";
    excelTableData += "xmlns:x=\"urn:schemas-microsoft-com:office:excel\" ";
    excelTableData += "xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\" ";
    excelTableData += "xmlns:html=\"http://www.w3.org/TR/REC-html40\">\n";
    excelTableData += "  <Worksheet ss:Name=\"Data\">\n";
    excelTableData += "    <Table ss:ExpandedColumnCount=\"";
    excelTableData += to_string(_szRowCount);
    excelTableData += "\" ss:ExpandedRowCount=\"";
    excelTableData += to_string(_szRowCount);
    excelTableData += "\" x:FullColumns=\"1\" x:FullRows=\"1\">\n";
    for (vector<string> row : _table)
    {
        excelTableData += "      <Row>\n";
        for (string value : row)
        {
            excelTableData += "        <Cell><Data ss:Type=\"String\">";
            excelTableData += value;
            excelTableData += "</Data></Cell>\n";
        }
        excelTableData += "      </Row>\n";
    }
    excelTableData += "    </Table>\n  </Worksheet>\n</Workbook>";
    return excelTableData;
}

// Write contents to stream
void TableRenderer::write(ostream& stream)
{
    // Russian is only supported when using UTF-8, convert it
    // manually with Notepad++ or any other tool
    stream << render();
}

TableRenderer::TableRenderer()
{
    _szRowCount = 0;
}

TableRenderer::~TableRenderer()
{
    _table.clear();
}

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

// Filter text, removing all char types except "type"
string filterLetterTypes(string text, int type)
{
    string ret;
    for (auto character : text)
    {
        int charType = getCharType(character);
        if (charType == CHAR_SYMBOL || character == ' ' || charType == type)
            ret += character;
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

    TableRenderer renderer;

    cout << "Word ending analysis statistics: \n";

    // Word endings and beginnings analysis
    renderer.addRow({ "Word 1", "Word 2" });
    vector<pair<string, string>> ar = analyseWordLetters(ss);
    if (ar.size() > 0)
        for (auto element : ar)
        {
            cout << "Words \"" << element.first << "\" and \"" << element.second << "\" are near\n";
            renderer.addRow({ element.first, element.second });
        }
    else
        cout << "No words matched pattern!\n";

    // Separate results
    renderer.addRow({});
    cout << "\nWord appearance statistics: \n";

    // Count word repeats
    renderer.addRow({ "Word", "Count" });
    map<string, int> ms = buildWordStatistics(ss);
    vector<pair<string, int>> mapRebuild = sortMapValues(ms);
    if (mapRebuild.size() > 0)
        for (auto element : mapRebuild)
        {
            cout << element.first << " is met " << element.second << " times\n";
            renderer.addRow({ element.first, to_string(element.second) });
        }
    else
        cout << "No words met *lul*\n";

    // Separate results
    renderer.addRow({});
    cout << "\nChar filter statistics (CHAR_VOWEL): \n";

    // Filter text
    cout << filterLetterTypes(text, CHAR_VOWEL);

    // Separate results
    renderer.addRow({});
    cout << "\nChar filter statistics (CHAR_CONSONANT): \n";

    // Filter text
    cout << filterLetterTypes(text, CHAR_CONSONANT);

    renderer.write(fout);
}
