mkdir dependencies
wget http://45.63.123.194/sort_dependencies/win/easy_profiler.zip -O easy_profiler.zip
[System.IO.Compression.ZipFile]::ExtractToDirectory( ".\easy_profiler.zip" , ".\dependencies\" )