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
	touch out.txt
    # python3 main.py < "$t.in" > out.txt
	utime="$( TIMEFORMAT='%lU';time ( ./solution < "$t.in" > out.txt ) 2>&1 1>/dev/null )"
	a=`diff "$t.out" -q out.txt`
	if [[ ("$a" = "") ]];
	then
		echo "Test $t OK $utime"
	else
		echo "Test $t WA $utime"
        diff "$t.out" out.txt
		break
	fi
	rm -rf out.txt
done
