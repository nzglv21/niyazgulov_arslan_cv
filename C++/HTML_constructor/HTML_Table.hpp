#include "HTML_Element.hpp"
#include <cstdarg>

class HTML_Table_Row: public HTML_Element{
    friend class HTML_Table;
    int col_count;
    char **row;
    void render(std::ostream& f = std::cout) override;
public:
    HTML_Table_Row(int, char**);
    ~HTML_Table_Row();
};

class HTML_Table_ColumnsNames: public HTML_Element{
    friend class HTML_Table;
    int col_count;
    char** col_names;
    void render(std::ostream& f = std::cout) override;
public:
    HTML_Table_ColumnsNames(int, char** col_names_);
    ~HTML_Table_ColumnsNames();
};

class HTML_Table:public HTML_Element{
    friend class HTML_Div;
    friend class HTML;
    HTML_Table_ColumnsNames* col_names;
    HTML_Table_Row** rows;
    char* caption;
    int row_count;
    int col_count;
    int rows_size;

    void addRow_(char**);
    void render(std::ostream& f = std::cout) override;
public:
    HTML_Table(int col_count_);
    ~HTML_Table();
    HTML_Table& setCol_names(...);
    HTML_Table& addRow(...);
    HTML_Table& setTableName(const char *tableName);
};