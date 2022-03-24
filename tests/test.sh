#!/bin/bash
set -o pipefail

ANY_FAIL=0

for I in *.in
do
	B=$(basename "$I" .in)
	ARX=apertium-eng-spa.spa-eng.arx
	if [[ $B == "empty" ]] ; then
		ARX=empty.arx
	fi
	cat "$B.in" | $1 $ARX > "$B.tmp"
	diff -wb "$B.tmp" "$B.out" > "$B.diff"
	if [[ $? -eq 0 ]] ; then
		echo "SUCCESS $B"
		rm -f "$B.tmp" "$B.diff"
	else
		echo "FAIL $B - see $B.tmp and $B.diff for details"
		ANY_FAIL=1
	fi
done

exit $ANY_FAIL
