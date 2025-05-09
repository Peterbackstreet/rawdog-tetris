flag=g++

main : main.cpp
	$(flag) -o main .\main.cpp -L .\lib\ -lraylib -lopengl32 -lgdi32 -lwinmm

clear : 
	rm main 
