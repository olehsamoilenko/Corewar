if [[ $1 == "" ]]
then
	echo "Usage: sh bin/disassembler.sh CHAMPION.cor"
else
	cp $1 bin
	filename=$(basename $1 .cor)
	./asm/asm -d bin/$filename.cor > bin/tmp
	./asm/asm bin/$filename.s > bin/tmp
	hexdump -vC bin/$filename.cor > bin/my
	hexdump -vC $1 > bin/origin
	if [ "$(diff -u bin/my bin/origin)" == "" ]
	then
		echo "\033[1;32m$filename OK\033[0m"
	else
		echo "\033[31m$filename KO\033[0m"
	fi
	rm bin/$filename.cor bin/$filename.s bin/tmp
fi
