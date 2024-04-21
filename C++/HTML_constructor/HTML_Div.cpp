#include "HTML_Div.hpp"

HTML_Div::HTML_Div(){
    id = nullptr;
    div_elements = new HTML_Element*[5];
    div_counts = 0;
    array_size = 5;
}

HTML_Div::HTML_Div(const char* id_){
    id = strCpy(id_);
    div_elements = new HTML_Element*[5];
    div_counts = 0;
    array_size = 5;
}

HTML_Div::~HTML_Div(){
    delete[] id;
    delete[] div_elements;
}

HTML_Div& HTML_Div::addElement(HTML_Element& el){

    cast(el);

    if (div_counts < array_size){
        div_elements[div_counts] = &el;
        div_counts += 1;
    }
    else{
        array_size += 5;
        HTML_Element** new_array = new HTML_Element*[array_size];
        for (int i = 0; i < div_counts; ++i)
            new_array[i] = div_elements[i];
        delete[] div_elements;
        div_elements = new_array;

        div_elements[div_counts] = &el;
        div_counts += 1;
    }
    return *this;
}

void HTML_Div::render(std::ostream& f){
    f  << "<div";
    if (id != nullptr)
        f  << " id = " << id;
    f  << ">";

    for (int i = 0; i < div_counts; ++i)
        div_elements[i]->render(f);

    f  << "</div>\n";
}