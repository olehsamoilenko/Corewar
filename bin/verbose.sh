if [[ $1 == "" ]]
then
	echo "Usage: sh bin/verbose.sh CHAMPION"
else
	./resources/corewar $1 -v 31 -a > bin/origin
	./vm/corewar $1 -verbose > bin/my
	diff -y bin/my bin/origin | less
fi