#!/bin/bash

WIDTH=960
HEIGHT=540

for i in {500..6886}
do
	let "x = ($RANDOM % 1920)-100"
	let "y = ($RANDOM % 1080)-100"
	let "width = $RANDOM % $WIDTH"
	let "height = $RANDOM % $HEIGHT"
	let "t = $RANDOM % 50"
	let "op = $RANDOM % 2"

	echo "build/dmtx -t rsa r $x $y $width $height t $t $op -s 14 sourceimages/dmtx/randRSA/rsaSeq_$i.png"
	build/dmtx -t "rsa r $x $y $width $height t $t $op" -s 14 sourceimages/dmtx/randRSA/rsaSeq_$i.png

#	echo "build/dmtx -t \"drawLock -t $t\" -s 14 sourceimages/dmtx/drawLocks/drawLock_$t.png"
#	build/dmtx -t "drawLock -t $t" -s 14 sourceimages/dmtx/drawLocks/drawLock_$t.png
done



