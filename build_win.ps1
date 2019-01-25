mkdir dependencies
wget http://45.63.123.194/sort_dependencies/win/easy_profiler.zip -O easy_profiler.zip

Add-Type -AssemblyName System.IO.Compression.FileSystem
function Unzip
{
    param([string]$zipfile, [string]$outpath)

    [System.IO.Compression.ZipFile]::ExtractToDirectory($zipfile, $outpath)
}

Unzip ".\easy_profiler.zip" ".\dependencies\"