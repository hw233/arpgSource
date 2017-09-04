svn up
cd build
cmake ..
make clean
make -j8
cp bin/cow_appd ../contrib/bin/appd
cp bin/cow_scened ../contrib/bin/scened
cp bin/cow_logind ../contrib/bin/logind
cp bin/cow_policed ../contrib/bin/policed
cp bin/cow_robotd ../contrib/bin/robotd

cd ..
pwd
cp -r contrib/bin/ arpg/
cp -r contrib/etc/ arpg/
cp -r contrib/maps/ arpg/
cp -r contrib/protocol/ arpg/
cp -r contrib/scripts/ arpg/
cp -r contrib/template arpg/

cd arpg
git add .
git commit -am 'public version'
git push origin master
git push net master
