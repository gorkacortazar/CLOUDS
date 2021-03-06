#/bin/bash

cd CloudsLibrary/src/VisualSystemsLibrary
git checkout master
git add *.cpp *.h
git commit -m "auto commit changes"
git push origin master
cd ../../../

cd CloudsLibrary/src/VisualSystems

for i in `ls .` ; do
    echo
    echo
    echo
  if [ -e "$i/.git" ]; then
	echo "UPDATING $i"
	cd $i
    git fetch origin
	git checkout master
    git add *
  	git commit -m "updating presets"
    git pull
	if [ $? -eq 0 ]; then
	  git push origin master
 	fi
    cd ..
  fi
done

