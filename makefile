objectsUntimed = main.o
objectsTimed = mainTimed.o
objectsMinPrint = mainMinPrint.o
common = Cryptography.o

CCFlag = -std=c++0x -O2 -DNDEBUG -ansi -pedantic -Werror -Wall 

compile = g++ $(CCFlag)

searchProgram : $(objectsUntimed) $(common)
	$(compile) -o searchProgram $(objectsUntimed) $(common)

searchProgramTimed : $(objectsTimed) $(common)
	$(compile) -o searchProgramTimed $(objectsTimed) $(common)

searchProgramMinPrint : $(objectsMinPrint) $(common)
	$(compile) -o searchProgramMinPrint $(objectsMinPrint) $(common)

main.o : main.cpp SearchClass.inl SearchClass.h Container2D.h Container2D.inl
	$(compile) -c main.cpp

mainTimed.o : main.cpp SearchClass.inl SearchClass.h Container2D.h Container2D.inl
	$(compile) -DMEASURESPEED -c main.cpp -o mainTimed.o

mainMinPrint.o : main.cpp SearchClass.inl SearchClass.h Container2D.h Container2D.inl
	$(compile) -DLESSPRINT -DMEASURESPEED -c main.cpp -o mainMinPrint.o

Cryptography.o : Cryptography.cpp Cryptography.h
	$(compile) -c Cryptography.cpp  

	
	
encrypt: EncryptProgram.cpp Cryptography.cpp Cryptography.h
	g++ EncryptProgram.cpp Cryptography.cpp -o encrypt

inputGen: GenerateUserInput.cpp
	g++ GenerateUserInput.cpp -o inputGen

generate2D: Generate2DInt.cpp
	g++ Generate2DInt.cpp -o generate2D
	
.PHONY : clean
clean : 
	-rm $(objectsUntimed) $(common) searchProgram
	
.PHONY : cleanall
cleanall : 
	-rm $(objectsUntimed) $(objectsTimed) $(objectsMinPrint) $(common) \
	searchProgram searchProgramTimed searchProgramMinPrint encrypt inputGen generate2D
#-rm $(objects) searchProgram searchProgramTimed searchProgramMinPrint
