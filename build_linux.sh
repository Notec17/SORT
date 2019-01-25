case "$(uname -s)" in

Linux)
rm -rf dependencies
mkdir dependencies
cd dependencies
apt-get install unzip
wget http://45.63.123.194/sort_dependencies/linux/easy_profiler.zip
unzip easy_profiler.zip
rm easy_profiler.zip
cd ..
ls
mkdir _out
cd _out
;;

*)
echo 'other OS'
;;
esac
