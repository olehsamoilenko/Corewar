if [[ $1 == "" ]]
then
	echo "Usage: sh bool.sh CHAMPION"
else
	cp $1 bin
	filename=$(basename $1 .cor)
	./asm/asm -d bin/$filename.cor
	./asm/asm bin/$filename.s
	if [ "$(diff -u $1 bin/$filename.cor)" == "" ]
	then
		echo "\033[1;32mOK\033[0m"
	else
		echo "\033[31mKO\033[0m"
	fi
	rm bin/$filename.cor bin/$filename.s
fi
