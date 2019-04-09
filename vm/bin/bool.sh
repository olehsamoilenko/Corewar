if [[ $1 == "" ]]
then
	echo "Usage: sh bool.sh CHAMPION"
else
	./corewar $1 -verbose > bin/my
	./resources/corewar $1 -v 31 -a > bin/origin
	if [ "$(diff -u bin/my bin/origin)" == "" ]
	then
		num=$(cat bin/origin | grep "It is now cycle" | tail -1 | cut -c 17-)
		sh bin/dump.sh $1 $(($num-1))
	else
		echo "VERBOSE KO"
	fi
fi