objects = obj/mainfile.o obj/f_beta.o obj/f_utility.o obj/mtrand.o obj/readfrmfile.o obj/f_parameters.o
bin/neuronSpikeSimulator: $(objects)
	g++ obj/mainfile.o obj/f_beta.o obj/f_utility.o obj/mtrand.o obj/readfrmfile.o obj/f_parameters.o -o bin/neuronSpikeSimulator

obj/mainfile.o : src/mainfile.cpp src/mtrand.h src/super.h
	g++ -c src/mainfile.cpp -o obj/mainfile.o 

obj/f_beta.o : src/f_beta.cpp src/mtrand.h src/super.h
	g++ -c src/f_beta.cpp -o obj/f_beta.o 

obj/f_utility.o : src/f_utility.cpp src/mtrand.h src/super.h
	g++ -c src/f_utility.cpp -o obj/f_utility.o 

obj/mtrand.o : src/mtrand.cpp src/mtrand.h
	g++ -c src/mtrand.cpp -o obj/mtrand.o

obj/readfrmfile.o : src/readfrmfile.cpp src/mtrand.h src/super.h
	g++ -c src/readfrmfile.cpp -o obj/readfrmfile.o 

obj/f_parameters.o : src/f_parameters.cpp src/mtrand.h src/super.h
	g++ -c src/f_parameters.cpp -o obj/f_parameters.o 

clean:
	rm obj/*.o
