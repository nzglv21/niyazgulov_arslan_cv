#include "HTML_Body.hpp"
#include "HTML_Head.hpp"
#include "HTML_H.hpp"
#include "HTML_Paragraph.hpp"
#include "HTML_Image.hpp"
#include "HTML_Div.hpp"
#include "HTML_Table.hpp"
#include "HTML_Text.hpp"
#include "HTML_Link.hpp"
#include "exception.hpp"

class HTML: public HTML_Body, public HTML_Head{
public:
    HTML ();
    HTML(const char* title);
    HTML_Body& getBody();
    HTML_Head& getHead();
    void convert_to_file(const char* );
    void render(std::ostream& f = std::cout);
};