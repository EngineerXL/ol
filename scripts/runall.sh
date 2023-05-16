cp solution tests/solution
# cp main.py tests/main.py
cd tests
for (( t = 1; t <= $1; t++ ));
do
	a=`cat $t.in`
	if [[ ("$a" = "") ]];
	then
		continue
	fi
    # python3 main.py < "$t.in" > out.txt
	utime="$( TIMEFORMAT='%lU';time ( ./solution < "$t.in" > /dev/null ) 2>&1 1>/dev/null )"
	echo "Test $t $utime"
done
