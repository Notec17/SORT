/*
    This file is a part of SORT(Simple Open Ray Tracing), an open-source cross
    platform physically based renderer.
 
    Copyright (c) 2011-2018 by Cao Jiayin - All rights reserved.
 
    SORT is a free software written for educational purpose. Anyone can distribute
    or modify it under the the terms of the GNU General Public License Version 3 as
    published by the Free Software Foundation. However, there is NO warranty that
    all components are functional in a perfect manner. Without even the implied
    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License for more details.
 
    You should have received a copy of the GNU General Public License along with
    this program. If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
 */

#include "log.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>
#include <string>

static std::vector<std::unique_ptr<LogDispatcher>> logDispatcher;
static bool logLevel = true;
static bool logType = true;
static bool logTime = true;
static bool logLineInfo = false;
static LOG_LEVEL logDefaultLevel = LOG_LEVEL::LOG_DEBUG;     // By default, debug information is avoided.

void addLogDispatcher( LogDispatcher* logdispatcher ){
    logDispatcher.push_back( std::unique_ptr<LogDispatcher>(logdispatcher) );
}

void sortLog( LOG_LEVEL level , LOG_TYPE type , const std::string& str , const char* file , const int line ){
    if( level < logDefaultLevel )
        return;
    for( const auto& it : logDispatcher )
        it->dispatch( level , type , str.c_str() , file , line );
}

void LogDispatcher::dispatch( LOG_LEVEL level , LOG_TYPE type , const char* str , const char* file , const int line ){
    output(format( level , type , str , file , line ));
}

const std::string logTimeString(){
    if( !logTime )
        return "";
    
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm t;
    ::localtime_s(&t, &now);
    char s[128] = { 0 };
    std::strftime(s, 128, "[%Y-%m-%d %H:%M:%S]", &t);
    return std::string(s);
}

const std::string levelToString( LOG_LEVEL level ){
    return !logLevel ? "" :
    ( LOG_LEVEL::LOG_DEBUG == level ) ? "[Debug]" :
    ( LOG_LEVEL::LOG_INFO == level ) ? "[Info]" :
    ( LOG_LEVEL::LOG_WARNING == level ) ? "[Warning]" :
    ( LOG_LEVEL::LOG_ERROR == level ) ? "[Error]" :
    ( LOG_LEVEL::LOG_CRITICAL == level ) ? "[Critical]" : "";
}

const std::string typeToString( LOG_TYPE type ){
    return !logType ? "" :
    ( LOG_TYPE::LOG_GENERAL == type ) ? "[General]" :
    ( LOG_TYPE::LOG_SPATIAL_ACCELERATOR == type ) ? "[Spatial Accelerator]":
    ( LOG_TYPE::LOG_PERFORMANCE == type ) ? "[Performance]" :
    ( LOG_TYPE::LOG_INTEGRATOR == type ) ? "[Integrator]" :
    ( LOG_TYPE::LOG_LIGHT == type ) ? "[Light]" :
    ( LOG_TYPE::LOG_MATERIAL == type ) ? "[Material]" :
    ( LOG_TYPE::LOG_IMAGE == type ) ? "[Image]" :
    ( LOG_TYPE::LOG_SAMPLING == type ) ? "[Sampling]" :
    ( LOG_TYPE::LOG_CAMERA == type ) ? "[Camera]" : 
    ( LOG_TYPE::LOG_SHAPE == type ) ? "[Shape]" :
    ( LOG_TYPE::LOG_STREAM == type ) ? "[Stream]" :
    ( LOG_TYPE::LOG_RESOURCE == type ) ? "[Resource]" :
    ( LOG_TYPE::LOG_TASK == type ) ? "[Task]" : "[Unknown]";
}

const std::string lineInfoString( const char* file , int line ){
    if( !logLineInfo )
        return "";
    return "[File:" + std::string(file) + "  Line:" + std::to_string(line) + "]";
}

// format log header
const std::string LogDispatcher::formatHead( LOG_LEVEL level , LOG_TYPE type , const char* file , const int line ) const{
    return logTimeString() + levelToString(level) + typeToString( type ) + lineInfoString( file , line ) + "\t";
}

const std::string LogDispatcher::format( LOG_LEVEL level , LOG_TYPE type , const char* str , const char* file , const int line ) const{
    return formatHead( level , type , file , line ) + std::string(str);
}

void StdOutLogDispatcher::output( const std::string& s ){
    std::cout<<s<<std::endl;
}

FileLogDispatcher::FileLogDispatcher( const std::string& filename ){
    file.open( filename.c_str() );
}
FileLogDispatcher::~FileLogDispatcher(){
    file.close();
}

void FileLogDispatcher::output( const std::string& s ){
    if( !file.is_open() )
        return;
    file<<s<<std::endl;
}
