if [[ $1 == "" ]]
then
	echo "Usage: sh verbose.sh CHAMPION"
else
	./resources/corewar $1 -v 31 > bin/origin
	./vm $1 -verbose > bin/my
	diff -y bin/my bin/origin | less
fi