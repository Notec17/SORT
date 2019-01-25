md dependencies
wget http://45.63.123.194/sort_dependencies/win/easy_profiler.zip -OutFile easy_profiler.zip
Expand-Archive .\easy_profiler.zip -DestinationPath .\dependencies\
mkdir _out
cd _out
cmake -DCMAKE_GENERATOR_PLATFORM=x64 ..