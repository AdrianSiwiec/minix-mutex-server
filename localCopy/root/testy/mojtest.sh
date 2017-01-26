#!/bin/sh

. test_utils

make || {
	echo Make failed.
	exit 2
}
echo Test 4: grupa testow jak test 3 \(3 x \(tic tac tac\)\)
 
echo -n run 1 ""
$BIN/tictactac 3 > $OUT/u2t4r1.g1.out &
$BIN/tictactac 3 > $OUT/u2t4r1.g2.out &
$BIN/tictactac 3 > $OUT/u2t4r1.g3.out &

wait

mdiff u2t4r1.g1.out u2t4r1.out
mdiff u2t4r1.g2.out u2t4r1.out
mdiff u2t4r1.g2.out u2t4r1.out

