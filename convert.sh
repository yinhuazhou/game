#!/bin/sh

convert(){
files=`find . -name "*.${1}"`

for i in $files;do
if [ -d "${i}" ]; then
continue
fi 

cp ${i} "${i}.mabk"
iconv -f SHIFT-JIS -t utf-8 $i > "${i}.ma"
# echo "iconv -f SHIFT-JIS -t utf-8 $i -o ${i}.ma"

if [ $? -eq 0 ];then
cp ${i}.ma ${i}
echo "ok: ${i}"
else
echo "error：${i}"
echo $i >> ./err.log
#todo color
fi
# exit
done
}

convert cpp
convert h
convert config
convert cs
convert settings
convert sh
convert txt
convert XML