if [[ $1 == "" ]]
then
	echo "Usage: sh bool.sh CHAMPION"
else
	./asm/asm -d $1 >&-
	filename=$(basename $1 .cor)
	mv resources/champs/cor/$filename.s resources/champs/dis_test
	./asm/asm resources/champs/dis_test/$filename.s >&-
	if [ "$(diff -u $1 resources/champs/dis_test/$filename.cor)" == "" ]
	then
		echo "\033[1;32mOK\033[0m"
	else
		echo "\033[31mKO\033[0m"
	fi
fi
