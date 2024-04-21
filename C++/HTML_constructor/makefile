all: executed

executed: main.o HTML.o HTML_Title.o HTML_H.o HTML_Paragraph.o HTML_Image.o exception.o HTML_Link.o HTML_Text.o HTML_Table.o HTML_Div.o HTML_Head.o HTML_Body.o
	g++ -o $@ $^


%.o: %.cpp
	g++ -c $< -o $@


clean:
	rm -f *.o *.html executed

clean_o:
	rm -f *.o executed
test:
	./executed 1
	./executed 2
	./executed 3