#ifndef __BUILD_H__
#define __BUILD_H__

#include <iostream>
#include <fstream>
#include "../externals/vector.h"
#include "../externals/string.h"
#include "../externals/array.h"
#include "../externals/table.h"

class Builder
{
    public:
        Builder() {}

    private:
        vector<arr<string,3>> HeaderFilesLogs;
        vector<arr<string,3>> CppFilesLogs;
        vector<table<string,vector<string>,1>> CppFilesDependencies;
        vector<arr<string,2>> Options;
        vector<string> ModifiedHeaderFilesList;
        vector<string> ModifiedCppFilesList;
        vector<arr<string,2>> CppFilesToCompile;
        vector<string> ObjectFilesList;

    public:
        void build();
        void run();

    private:
        void Load_Headerfiles_Logs();
        void Load_Cppfiles_Logs();
        void Load_Cpp_Files_Dependencies();
        void Load_Options();

        void Save_Headerfiles_Logs();
        void Save_Cppfiles_Logs(); 
        void Save_Cpp_Files_Dependencies();

        void Header_File_Exist();
        void Cpp_File_Exist();
        void Check_Dependencies_Exist();

        void Search_New_Header_Files();
        void Search_New_Cpp_Files();

        void Initialise_Object_File_list();
        void Initialise_Modified_Files_List();
        void Initialise_Cpp_Files_To_Compile_List();

        void Update_Files_Dates();
        
        void Compile();
        void Compile_Project();
        void Compile_Build_System();
        void Complie_Object_Files();

    private:
        void Load_logs(const char*,vector<arr<string,3>>&,vector<string>&,void (Builder::*Exist)());
        void Save_Logs(const char*,vector<arr<string,3>>&);
        void Update_File_Date(const string&,vector<arr<string,3>>&);
        void Missing_Path_Date(vector<arr<string,3>>&,vector<string>&);
        void Remove_Dates();

        void Compile_File(string,const string&,const bool& sdl_or_not = false);
        
        string Get_File_Date(const string&);
        string Get_File_Path(const string&);
        string Get_Date(const string&,vector<arr<string,3>> &);
        string Get_Path(const string&,vector<arr<string,3>> &);

        string Find_Option(const string&);
        string Command_Result(const string&);
        

};

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
#endif