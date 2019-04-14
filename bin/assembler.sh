if [[ $1 == "" ]]
then
	echo "Usage: sh bin/assembler.sh CHAMPION.s"
else
	cp $1 bin/my.s
	cp $1 bin/origin.s
	./asm/asm bin/my.s > bin/tmp
	./resources/asm bin/origin.s > bin/tmp
	hexdump -vC bin/my.s > bin/my
	hexdump -vC bin/origin.s > bin/origin
	if [ "$(diff -u bin/my bin/origin)" == "" ]
	then
		echo "\033[1;32m$(basename $1) OK\033[0m"
	else
		echo "\033[31m$(basename $1) KO\033[0m"
	fi
	rm bin/my.s bin/my.cor bin/origin.s bin/origin.cor bin/tmp
fi
