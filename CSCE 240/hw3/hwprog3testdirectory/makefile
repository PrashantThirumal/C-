GPP = g++ -O3 -Wall -std=c++11

UTILS = ../../Utilities

A = main.o
#G = globals.o
M = dabnamespace.o
E = pullet16interpreter.o
S = scanner.o
SL = scanline.o
U = utils.o

Aprog: $A $G $M $E $H $S $(SL) $U
	$(GPP) -o Aprog $A $G $M $E $H $S $(SL) $U

main.o: main.h main.cc
	$(GPP) -c main.cc

#globals.o: globals.h globals.cc
#	$(GPP) -c globals.cc

dabnamespace.o: dabnamespace.h dabnamespace.cc
	$(GPP) -c dabnamespace.cc

pullet16interpreter.o: pullet16interpreter.h pullet16interpreter.cc
	$(GPP) -c pullet16interpreter.cc
#	$(GPP) -c -DEBUG pullet16interpreter.cc

scanner.o: $(UTILS)/scanner.h $(UTILS)/scanner.cc
	$(GPP) -c $(UTILS)/scanner.cc

scanline.o: $(UTILS)/scanline.h $(UTILS)/scanline.cc
	$(GPP) -c $(UTILS)/scanline.cc

utils.o: $(UTILS)/utils.h $(UTILS)/utils.cc
	$(GPP) -c $(UTILS)/utils.cc
