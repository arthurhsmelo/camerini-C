C=gcc

BUILD_DIR = ./build
SRC_DIR = ./src
 
all: main 

####################################################
#                                                  #
#                  Header Files                    #
#                                                  #
####################################################

Models.o: ${SRC_DIR}/models/Models.h ${SRC_DIR}/models/Models.c
	${C} -c ${SRC_DIR}/models/Models.c -g -o ${BUILD_DIR}/Models.o

DataHandlers.o: ${SRC_DIR}/utility/DataHandlers.h ${SRC_DIR}/utility/DataHandlers.c
	${C} -c ${SRC_DIR}/utility/DataHandlers.c -g -o ${BUILD_DIR}/DataHandlers.o

####################################################
#                                                  #
#                     MAIN                         #
#                                                  #
####################################################

tp2.o: ${SRC_DIR}/models/Models.h ${SRC_DIR}/utility/DataHandlers.h ${SRC_DIR}/tp2.h
	${C} -c ${SRC_DIR}/tp2.c -g -o ${BUILD_DIR}/tp2.o

main: Models.o DataHandlers.o tp2.o
	${C} -g -o ${BUILD_DIR}/tp2 -lm main.c ${BUILD_DIR}/*.o
 
# Rule for cleaning files generated during compilation. 
# Call 'make clean' to use it 
clean: 
	rm -rf ${BUILD_DIR}/*.o    

clean_windows:
	del /S "%dir%\build\*.o"    

run:
	./build/tp2 ${citiesFile}

run_windows:
	./build/tp2.exe

