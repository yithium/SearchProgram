objects = main.o Cryptography.o Container2D.o SearchClass.o

CCFlag = -ansi -pedantic -Werror -Wall 

compile = g++ $(CCFlag)

searchProgram : $(objects)
	$(compile) -o searchProgram $(objects) 
	
main.o : main.cpp Cryptography.h
	$(compile) -c main.cpp
	
Cryptography.o : Cryptography.cpp Cryptography.h
	$(compile) -c Cryptography.cpp  

Container2D.o: Container2D.cpp Container2D.h
	$(compile) -c Container2D.cpp

SearchClass.o: SearchClass.cpp SearchClass.h
	$(compile) -c SearchClass.cpp

.PHONY : clean
clean : 
	-rm $(objects) searchProgram
