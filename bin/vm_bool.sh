if [[ $1 == "" ]]
then
	echo "Usage: sh bin/vm_bool.sh CHAMPION"
else
	./vm/corewar $1 -verbose > bin/my
	./resources/corewar $1 -v 31 -a > bin/origin
	if [ "$(diff -u bin/my bin/origin)" == "" ]
	then
		num=$(cat bin/origin | grep "It is now cycle" | tail -1 | cut -c 17-)
		sh bin/dump.sh $1 $(($num-1))
	else
		echo "\033[31mVERBOSE KO\033[0m"
	fi
fi