if [[ $1 == "" ]]
then
	echo "Usage: sh bool.sh CHAMPION"
else
	./vm $1 -verbose > bin/my
	./resources/corewar $1 -v 31 > bin/origin
	if [ "$(diff -u bin/my bin/origin)" == "" ]
	then
		echo "OK"
		num=$(cat bin/origin | grep "It is now cycle" | tail -1 | cut -c 17-)
		#echo Last cycle: $(($num-1))
		sh bin/dump.sh $1 $(($num-1))
	else
		echo "KO"
	fi
fi