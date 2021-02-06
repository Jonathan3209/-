OBJ = main.o Errors.o AssistingSecondTravel.o FirstTravel.o SecondTravel.o ToolsFunctions.o

assembler: main.o Errors.o AssistingSecondTravel.o FirstTravel.o SecondTravel.o ToolsFunctions.o
	gcc -g -ansi -Wall -pedantic main.o Errors.o AssistingSecondTravel.o FirstTravel.o SecondTravel.o ToolsFunctions.o -o assembler

main.o: main.c
	gcc -c -g -ansi -Wall -pedantic main.c -o main.o

Errors.o: Errors.c
	gcc -c -g -ansi -Wall -pedantic Errors.c -o Errors.o

AssistingSecondTravel.o: AssistingSecondTravel.c
	gcc -c -g -ansi -Wall -pedantic AssistingSecondTravel.c -o AssistingSecondTravel.o

FirstTravel.o: FirstTravel.c
	gcc -c -g -ansi -Wall -pedantic FirstTravel.c -o FirstTravel.o

SecondTravel.o: SecondTravel.c
	gcc -c -g -ansi -Wall -pedantic SecondTravel.c -o SecondTravel.o

ToolsFunctions.o: ToolsFunctions.c
	gcc -c -g -ansi -Wall -pedantic ToolsFunctions.c -o ToolsFunctions.o

clean:
	-rm -f $(OBJ)
	-rm -f assembler
