#read plugin_name
#a= $plugin name
#while [ $a -ne 2 ]
 #   do
 #    {
		echo "enter the name of plugin:"
#
#	}
#done
read a
while [  -d $a ]              #check if the same folder exits 
  do
	{
	echo " plugin exist, enter some other name:"
	read a
	}
done

mkdir $a
path=$(pwd)
cp -r sample/* $path/$a
cd $a
sed -i "s/sample/$a/g" * $path/$a/*

mv sample.cpp $a.cpp
mv sample.h $a.h
mv sample.json $a.json
mv sample.vcxproj $a.vcxproj 
mv sample.pro $a.pro
exit
