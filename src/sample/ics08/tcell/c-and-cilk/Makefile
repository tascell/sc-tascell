grav: grav.cilk
	cilkc -o grav grav.cilk -lm -O2

nqueen: nqueen.cilk nqueen-bt.cilk
	cilkc -o nqueen-ac nqueen.cilk -O2 -DALWAYS_COPY
	cilkc -o nqueen nqueen.cilk -O2
	cilkc -o nqueen-bt nqueen-bt.cilk -O2

pentomino: pentomino.cilk pentomino-bt.cilk
	cilkc -o pentomino-ac pentomino.cilk -O2 -DALWAYS_COPY
	cilkc -o pentomino pentomino.cilk -O2
	cilkc -o pentomino-bt pentomino-bt.cilk -O2
