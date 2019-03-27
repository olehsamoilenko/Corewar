if [[ $1 = "" || $2 = "" ]];
then
	echo "Usage: sh dump.sh CHAMPION CYCLE"
else
	./vm $1 -dump $2 > bin/my
	./resources/corewar $1 -d $2 > bin/origin
	if [ "$(diff -y bin/my bin/origin | grep "|")" == "" ]
	then
		echo "OK"
	else
		echo "KO"
	fi
fi