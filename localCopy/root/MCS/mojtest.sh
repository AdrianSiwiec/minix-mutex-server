
. test_utils

make || {
	echo Make failed.
	exit 2
}

rm -f out/*

echo Test 3: wiele procesow, jeden mutex \(trwa ok 30s\)

echo -n run 1 ""
$BIN/many_procs 5 5 | sort > $OUT/u1t3r1.out
mdiff u1t3r1.out u1t3.out
