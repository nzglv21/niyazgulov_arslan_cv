#include "HTML_Head.hpp"


HTML_Head::HTML_Head(){ 
    Title = new HTML_Title();
}

HTML_Head::HTML_Head(const char* title){
    Title = new HTML_Title(title);
}


void HTML_Head::render(std::ostream& f){
    f  << "<head> \n";
    Title->render(f);
    f   << "  <style>\n"
        << "    body {\n"
        << "      background-color: #f8f9fa;\n"
        << "      font-family: Arial, sans-serif;\n"
        << "      font-size: 16px;\n"
        << "      line-height: 1.5;\n"
        << "      color: #333;\n"
        << "      margin: 0;\n"
        << "      padding: 0;\n"
        << "    }\n"
        << "    a {\n"
        << "      color: #007bff;\n"
        << "      text-decoration: none;\n"
        << "    }\n"
        << "    a:hover {\n"
        << "      text-decoration: underline;\n"
        << "    }\n"
        << "    h1, h2, h3, h4, h5, h6{\n"
        << "      color: #343a40;\n"
        << "      text-align: center;\n"
        << "    }\n"
        << "    table {\n"
        << "      width: 70%;\n"
        << "      border-collapse: collapse;\n"
        << "      border-radius: 10px;\n"
        << "      overflow: hidden;\n"
        << "      background-color: #fff;\n"
        << "      box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1);\n"
        << "    }\n"
        << "    th, td {\n"
        << "      padding: 15px;\n"
        << "      text-align: left;\n"
        << "      border: 1px solid #ddd;\n"
        << "    }\n"
        << "    th {\n"
        << "      background-color: #007bff;\n"
        << "      color: #fff;\n"
        << "      font-weight: bold;\n"
        << "    }\n"
        << "    td:last-child {\n"
        << "      border-right: none;\n"
        << "    }\n"
        << "    tr:hover {\n"
        << "      background-color: #e9ecef;\n"
        << "    }\n"
        << "    img {\n"
        << "      display: block;\n"
        << "      margin: 0 auto;\n"
        << "      border: 2px solid #343a40;\n"
        << "      transition: transform 0.3s ease-in-out;\n"
        << "    }\n"
        << "    img:hover {\n"
        << "      transform: scale(1.1);\n"
        << "    }\n"
        << "    div {\n"
        << "      background-color: #f8f9fa;\n"
        << "      padding: 20px;\n"
        << "      border-radius: 10px;\n"
        << "      box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1);\n"
        << "      margin: 20px auto;\n"
        << "      text-align: center;\n"
        << "      max-width: 1000px;\n"
        << "    }\n"
        << "    div:hover {\n"
        << "      transform: scale(1.05);\n"
        << "      transition: transform 0.3s ease-in-out;\n"
        << "      cursor: pointer;\n"
        << "    }\n"
        << "  </style>\n";
    f  << "</head>\n";
}



HTML_Head::~HTML_Head(){
    delete Title;
}

