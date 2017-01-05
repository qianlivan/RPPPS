user=`whoami`
a=`ps augx | grep $user | grep prepfold | wc -l`
while(( "$a" != "1"))
do
sleep 2
((a--))
echo "Folding: $line in total, $a left."
a=`ps augx | grep $user | grep prepfold | wc -l`
done
