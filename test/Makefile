CC=clang++
CFLAGS= -I cute_lib -std=c++11
DEPS = 
OBJ = System.o Ship.o Projectile.o Weapon.o Cartridge.o Crate.o Entity.o AI.o

a.out: ShipTest.o $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS)

b.out: SystemTest.o $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS)

ShipTest.o: ShipTest.cpp $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

SystemTest.o: SystemTest.cpp $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

AI.o: ../src/Entities/Components/AI.cpp $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

Ship.o: ../src/Entities/Ship.cpp $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

Projectile.o: ../src/Entities/Projectile.cpp $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

Weapon.o: ../src/Entities/Components/Weapon.cpp $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

Cartridge.o: ../src/Entities/Components/Cartridge.cpp $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

Crate.o: ../src/Entities/Crate.cpp $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

Entity.o: ../src/Entities/Entity.cpp $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

System.o: ../src/System.cpp $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)



clean:
	rm -f *.o a.out
	    

