#!/bin/bash
function tree() {
	IFS=$(echo -en "\n\b")
	dircount=0
	filecount=0
	array=("$1")
	i=0
	j=1
	while [ ${#array[@]} -ne 0 ]
	do
		p=${array[$i]}
		unset array[$i]
		echo "[`basename $p`]"
		for file in $(ls $p)
		do
			workpath=$(realpath $p)/$(basename $file)
			echo "$workpath"
			if [ -d "$workpath" ]
			then
				((dircount++))
				array[j]=$workpath
				((j++))
			elif [ -f "$workpath" ]
			then
				((filecount++))
			fi
		done
		((i++))
		echo ""
	done
	echo "[Directories Count]:$dircount"
	echo "[Files Count]:$filecount"
}
if [ ! -f "$2" ]
then
	mkdir -p `dirname $2`
fi
tree $1 > $2
