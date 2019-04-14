if [[ $1 = "" || $2 = "" ]];
then
	echo "Usage: sh dump.sh CHAMPION CYCLE"
else
	./vm/corewar $1 -dump $2 > bin/my
	./resources/corewar $1 -d $2 > bin/origin
	if [ "$(diff -u bin/my bin/origin)" == "" ]
	then
		echo "\033[1;32mDUMP $2 OK\033[0m"
	else
		echo "\033[31mDUMP $2 KO\033[0m"
	fi
fi