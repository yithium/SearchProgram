objects = main.o Cryptography.o

CCFlag = -ansi -pedantic -Werror -Wall 

compile = g++ $(CCFlag)

searchProgram : $(objects)
	$(compile) -o searchProgram $(objects) 
	
main.o : main.cpp Cryptography.h SearchClass.inl SearchClass.h Container2D.h Container2D.inl
	$(compile) -c main.cpp
	
Cryptography.o : Cryptography.cpp Cryptography.h
	$(compile) -c Cryptography.cpp  

.PHONY : clean
clean : 
	-rm $(objects) searchProgram
