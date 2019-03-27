if [ $1 == "" ]
then
	echo "Usage: sh chech.sh CHAMPION"
else
	./vm $1 -verbose > bin/my
	./resources/corewar $1 -v 31 > bin/origin
	diff -y bin/my bin/origin | less
fi