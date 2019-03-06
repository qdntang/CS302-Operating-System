
function getdir(){
	echo $1
	for file in $1/*
	do
		if test -f $file
		then
			echo $file
			arr=(${arr[*]} $file)
		else
			getdir $file
		fi
	done
}
getdir ../test
echo ${arr[@]}
