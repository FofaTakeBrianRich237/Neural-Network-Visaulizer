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

        //------------Build-Parameters-------------------/
        vector<arr<string,3>> HeaderFilesLogs; // Stores details about Headers files(name,path,last_modificaton_date)
        vector<arr<string,3>> CppFilesLogs;   // Stores details about Cpp  files(name,path,last_modificaton_date)
        vector<table<string,vector<string>,1>> CppFilesDependencies; // Stores the list of Header files each Cpp files depends of
        vector<arr<string,2>> Options; // Compilation options like flags , details about compiler and c++ version
        vector<string> ModifiedHeaderFilesList; // Stores a list of Header files that have been modified
        vector<string> ModifiedCppFilesList; // Stores a list of Cpp files that have been modified
        vector<arr<string,2>> CppFilesToCompile; // Stores a the list of Cpp files that have to be compiled
        vector<string> ObjectFilesList; // Stores the list of existing Object files 
        //-----------------------------------------------/

    public:
        void build();
        void run();

    private:

        //---------Data Loading form files functions-----------/
        void Load_Cpp_Files_Dependencies(); // Initialises CppFilesDependecies from cpp_file_dependencies.txt
        void Load_Headerfiles_Logs(); // Initialises HeaderFilesLogs from log_files/headers_logs.txt
        void Load_Cppfiles_Logs(); // Initialises CppFilesLogs from log_files/cpp_logs.txt
        void Load_Options(); // Initialises Options from options.txt
        //----------------------------------------------------/

        
        //----------Save Data into files functions------------/
        void Save_Cpp_Files_Dependencies(); // Saves the content of CppfilesDpendencies into cpp_file_dependencies.txt
        void Save_Headerfiles_Logs(); // Saves the content of HeaderFilesLogs into log_files/headers_logs.txt
        void Save_Cppfiles_Logs();  // Saves the content of CppFilesLogs into log_files/cpp_logs.txt
        //----------------------------------------------------/


        //------------Check files existance functions---------/
        void Check_Dependencies_Exist(); // Checks if Cpp files and the Header files they depend on stored in CppFilesDependecies  exist
        void Header_File_Exist(); // Checks if Header files stored in HeaderFilesLogs exist
        void Cpp_File_Exist(); // Checks if Cpp files stored in CppFilesLogs exist
        //----------------------------------------------------/


        //-----------Search new files fucntions---------------/
        void Search_New_Header_Files(); // Search for existing Header files not stored in HeaderFilesLogs
        void Search_New_Cpp_Files(); // Search for existing Header files not stored in HeaderFilesLogs
        //----------------------------------------------------/


        //------------Data not loaded from files.txt----------/
        void Initialise_Object_File_list(); // Finds and stores in ObjectFilesList all obj files found in bin
        void Initialise_Modified_Files_List(); // Initialises ModifiedHeaderFilesList and ModifiedCppFilesList with cpp files and header files that were modified
        void Initialise_Cpp_Files_To_Compile_List(); // Initialises CppFilesToCompile with cpp files name that have to be compiled 
        //----------------------------------------------------/
        

        //-------------Compilation functions------------------/
        void Compile(); // Compiles object file
        void Compile_Project(); // Compiles Project(except Build.cpp)
        void Compile_Build_System(); // Compile Build.cpp if Build.h or/and Build.cpp were modified
        void Complie_Object_Files(); // Compiles Cpp files that have to be compile and produce their obj files
        //----------------------------------------------------/

        bool Verfy_If_Build_Files_Modified();

    private:
        ///@brief Loads a list of details about files of a particular type(cpp,hpp)
        void Load_logs(const char*,vector<arr<string,3>>&,vector<string>&,void (Builder::*Exist)());

        ///@brief Saves a list details about files of a particular type into a give file
        void Save_Logs(const char*,vector<arr<string,3>>&);

        ///@brief Update a file's last modification date
        void Update_File_Date(const string&,vector<arr<string,3>>&);

        ///@brief Finds the last modfication date of a file
        void Missing_Path_Date(vector<arr<string,3>>&,vector<string>&);

        ///@brief Add a Cpp file and its Header files dependencies in CppFilesDependencies
        void Add_Cpp_File_Dependencies(const string&);

        ///@brief Compiles a cpp file to an obj file
        void Compile_File(string,const string&);
        


        ///@brief Returns the last modfication date of a file
        string Get_File_Date(const string&);

        ///@brief Returns the path to a file
        string Get_File_Path(const string&);

        ///@brief Gets and returns last modifcation date of a file from a variable containing details abaut files
        string Get_Date(const string&,vector<arr<string,3>> &);

        ///@brief Gets and returns path to a file from a variable containing details abaut files
        string Get_Path(const string&,vector<arr<string,3>> &);



        /// @brief finds an info in Options
        string Find_Option(const string&);

        /// @brief returns the result of a command
        string Command_Result(const string&);
        

};

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
#endif