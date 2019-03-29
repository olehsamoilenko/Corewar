if [[ $1 == "" ]]
then
	echo "Usage: sh verbose.sh CHAMPION"
else
	./vm $1 -verbose > bin/my
	./resources/corewar $1 -v 31 > bin/origin
	if [ "$(diff -u bin/my bin/origin)" == "" ]
	then
		echo "OK"
	else
		echo "KO"
	fi
fi