# Task 9 - HTML Конструктор

** Реализация HTML конструктора с использование чисто виртуального класса HTML_Element. HTML разметка
состоит из ```head``` и ```body```. В ```head``` содержится информация о сайте, а в ```body``` то, что мы видим.
    Главным объектом является `HTML`. Все остальные объекты добавляются в `HTML`.
**

### Описание каждого объекта

>   `HTML_Element` - Виртуальный класс, а также базовый класс для многих объектов с методом render, который принимает в качестве параметра файловый дескриптор и переводит класс в HTML разметку.

>   `HTML_H` - Базовый Класс для вывода текста-заголовка различных уровней, является произодным класса HTML_Element. Произодные классы H(i), i = 1,...6. H(i) - заголовки i-го уровня.
>>  `H(i)` - при инициализации следует передавать указатель на строку.

>   Классы для работы с текстом. При создании принимает указатель на строку.
>>   `HTML_p` - текст
>>   `HTML_b` - жирный текст.
>>   `HTML_i` - курсивный текст. 
>>   `HTML_u` - подчеркнутый текст.
>>   `HTML_s` - вычеркнутый текст.

>   `HTML_Image` - Класс для добавления изображений. В качестве обязательного аргумента принимает расположение изображения в сети или на локальной машине, далее три необязательных - `widht=200`, `hight=200`, `title=nullptr`.

>   `HTML_Link` - Класс для создания гипертекста. Принимает ссылку и текст.

>   `HTML_Table` - Класс для создания Таблиц. Принимает количество столбцов.
**Методы**
>>  `HTML_Table& setTableName(const char *tableName)` - Устанавливает название таблицы.
>>  `HTML_Table& setCol_names(...)` - Устанавливает название столбцов, названия следует передавать через запятую (table.setCol_names("first", "second",...)). Важно, чтобы количество названий колонок в параметрах функции совпало с количеством столбцов, указанный при инициализации.

>>  `HTML_Table& addRow(...)` - Добавляет строку в таблицу: (table.addRow("1", "2", ..., "{col_count}")). Важно, чтобы количесвто аргументов переданные в функцию совпало с количесвтом столбцов.

>  `HTML_Div` - Класс-контейнер, куда можно добавить другие объекты.
**Методы**
>> `HTML_Div& addElement(HTML_Element& el)` - Добавлет элемент el в контейнер, где el - заголовок, картинка, таблица, гипертекст или любой другой контейнер.

> `HTML` - Главный класс, куда помещаются все другие элементы. Наследует классы HTML_Body, HTML_HEAD. Все остальные объекты следует добавлять в HTML.
**Методы**
>>  `HTML_Body& addElement(HTML_Element &p)` - добаляет объекты в HTML файл.
>> `void setTitle(char* new_title)` - утсанавливает название сайта.



### Makefile
> all - создает исполняемый файл executed
>>
> rm_all - удалает *.o, *.html
>>
> test - запуоскает тестовые функции
>>

## Пример программы
int main{
    HTML site;

    HTML_Div div_h;
    H1 h1("Это текст H1");
    H2 h2("Это текст H2");
    H3 h3("Это текст H3");
    H4 h4("Это текст H4");
    H5 h5("Это текст H5");
    H6 h6("Это текст H6");
    div_h.addElement(h1).addElement(h2).addElement(h3).addElement(h4).addElement(h5).addElement(h6);

    HTML_Div div_text;
    HTML_b b("Жирный текст\n");
    HTML_i i("Курсивный текст\n");
    HTML_u u("Подчеркнутый текст\n");
    HTML_strike strike("Зачеркнутый текст\n");
    HTML_p par("Это просто текст\n");
    div_text.addElement(b).addElement(i).addElement(u).addElement(strike).addElement(par);


    HTML_Div div_link;
    HTML_Link link1("Сайт ВМК МГУ", "https://cs.msu.ru/"),
        link2("ВК ВМК МГУ", "https://vk.com/cmcmsu");
    HTML_Image im_vmk("https://i.ytimg.com/vi/8C-mqp0sb_U/maxresdefault.jpg");
    div_link.addElement(im_vmk).addElement(link1).addElement(link2);

    HTML_Div div_in1, div_in2, div_in3, div_in4;
    HTML_p in1("div 1го уровня"),
        in2("div 2го уровня"),
        in3("div 3го уровня"),
        in4("div 4го уровня");

    div_in4.addElement(in4);
    div_in3.addElement(in3).addElement(div_in4);
    div_in2.addElement(in2).addElement(div_in3);
    div_in1.addElement(in1).addElement(div_in2);


    HTML_Div mul_div;
    HTML_Table table(10);
    table.setTableName("Таблица умножения");
    table.setCol_names(" ","1", "2", "3", "4", "5", "6", "7", "8", "9");
    table.addRow("1", "1", "2", "3", "4", "5", "6", "7", "8", "9");
    table.addRow("2", "2", "4", "6", "8", "10", "12", "14", "16", "18");
    table.addRow("3", "3", "6", "9", "12", "15", "18", "21", "24", "27");
    table.addRow("4", "4", "8", "12", "16", "20", "24", "28", "32", "36");
    table.addRow("5", "5", "10", "15", "20", "25", "30", "35", "40", "45");
    table.addRow("6", "6", "12", "18", "24", "30", "36", "42", "48", "54");
    table.addRow("7", "7", "14", "21", "28", "35", "42", "49", "56", "63");
    table.addRow("8", "8", "16", "24", "32", "40", "48", "56", "64", "72");
    table.addRow("9", "9", "18", "27", "36", "45", "54", "63", "72", "81");
    mul_div.addElement(table);



    site.addElement(div_h).addElement(div_text).addElement(div_link).addElement(div_in1).addElement(mul_div);
    site.convert_to_file("test3.html");

}