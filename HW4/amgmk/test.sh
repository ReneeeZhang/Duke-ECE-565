for i in {0..99}
do
    if `./AMGMk | grep -i error` 
    then
        echo WRONG!!
    else
        echo $i
    fi
done