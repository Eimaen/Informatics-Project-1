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
