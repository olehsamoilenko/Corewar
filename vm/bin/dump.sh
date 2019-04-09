if [[ $1 = "" || $2 = "" ]];
then
	echo "Usage: sh dump.sh CHAMPION CYCLE"
else
	./corewar $1 -dump $2 > bin/my
	./resources/corewar $1 -d $2 > bin/origin
	if [ "$(diff -u bin/my bin/origin)" == "" ]
	then
		echo DUMP $2 OK
	else
		echo DUMP $2 KO
	fi
fi