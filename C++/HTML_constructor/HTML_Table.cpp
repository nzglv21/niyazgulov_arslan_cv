#include "HTML_Table.hpp"

HTML_Table_ColumnsNames::HTML_Table_ColumnsNames(int col_count_, char** col_names_){
    col_count = col_count_;
    col_names = col_names_;
}

HTML_Table_ColumnsNames::~HTML_Table_ColumnsNames(){
    for (int i = 0; i < col_count; ++i)
        delete[] col_names[i];
    delete[] col_names;
}

void HTML_Table_ColumnsNames::render(std::ostream& f){
    f  << "<thead>\n <tr align = \"center\">\n";
    
    for (int i = 0; i < col_count; ++i)
        f  << "<th>" << col_names[i] << "</th>\n";
    
    f  << "</tr>\n </thead>\n";
}

HTML_Table_Row::HTML_Table_Row(int col_count_, char** argv){
    col_count = col_count_;
    row = argv;
}

HTML_Table_Row::~HTML_Table_Row(){

    for (int i = 0; i< col_count; ++i)
        delete[] row[i];

    delete[] row;
}

void HTML_Table_Row::render(std::ostream& f){
    f  << "<tr align=\"center\">\n";

    for (int i = 0; i < col_count; ++i){
        f  << "<td>" << row[i] << "</td>\n";
    }
    f  << "</tr>";
}


HTML_Table::HTML_Table(int col_count_){
    row_count = 0;
    rows_size = 0;
    col_count = col_count_;
    col_names = nullptr;
    caption = nullptr;
    rows = new HTML_Table_Row*[5];
}

HTML_Table::~HTML_Table(){
    delete col_names;
    if (caption != nullptr)
        delete[] caption;
    
    for (int i = 0; i < row_count; ++i)
        delete rows[i];

    delete[] rows;
}

HTML_Table& HTML_Table::setCol_names(...){
    va_list args;
    va_start(args, col_count);
    char** argv;
    argv = new char*[col_count];
    for (int i  = 0; i < col_count; ++i)
        argv[i] = strCpy(va_arg(args, char*));
    
    col_names = new HTML_Table_ColumnsNames(col_count, argv);
    va_end(args);
    return *this;
};

void HTML_Table::addRow_(char ** row_el){
    if (row_count < rows_size){
        rows[row_count] = new HTML_Table_Row(col_count, row_el);
        row_count+=1;
    }
    else{
        rows_size+=5;
        HTML_Table_Row** new_rows = new HTML_Table_Row*[rows_size];

        for (int i = 0; i < row_count; ++i)
            new_rows[i] = rows[i];
        
        delete[] rows;
        rows = new_rows;

        rows[row_count] = new HTML_Table_Row(col_count, row_el);
        row_count+=1;
    }
}

HTML_Table& HTML_Table::addRow(...){
    va_list args;
    va_start(args, col_count);
    char** argv;
    argv = new char*[col_count];

    for (int i  = 0; i < col_count; ++i)
        argv[i] = strCpy(va_arg(args, char*));
    
    addRow_(argv);
    return *this;
}

void HTML_Table::render(std::ostream& f){
    f  << "<table border=\"1\" align=\"center\">\n";
    if (caption != nullptr)
        f  << "<caption>" << caption << "</caption>";
    if (col_names != nullptr)
        col_names->render(f);
    for (int i = 0; i < row_count; ++i)
        rows[i]->render(f);

    f  << "</table>";

}

HTML_Table& HTML_Table::setTableName(const char* tabeName_){
    if (caption != nullptr)
        delete[] caption;
    caption = strCpy(tabeName_);
    return *this;
}
