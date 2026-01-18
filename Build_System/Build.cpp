#include "Build.h"

void Builder::Load_logs(const char* path,vector<arr<string,3>>& FilesLogs,vector<string>& Modif_List, void (Builder::*Exist)())
{
    std::ifstream Logs_File(path);
    char f[20],d[40],l[20];

    for(int a = 0; Logs_File >> f >> d >> l; a++)
    {
        FilesLogs.add();
        FilesLogs[a][0] = f;
        FilesLogs[a][1] = d;
        FilesLogs[a][2] = l;
    }
    Logs_File.close();
    (this->*Exist)();
    Missing_Path_Date(FilesLogs,Modif_List);
}

void Builder::Load_Object_Files()
{
    string list = Command_Result("find ../Build_System/bin -type f -name \"*.o\" ");
    for(int i = 0; i < list.length; i++)
    {
        if(list[i] == ' ')
        {
            int j; 
            for(j = i; (j>=0) && (list[j] != '/'); j--)
            j++;

            ObjectFilesList.add(list.cut(j,i-1));
        }
    }
}

void Builder::Initialise_Object_File_list()
{
    string list = Command_Result("find bin -type f -name \"*.o\"");
    for(int i = 0; i < list.length; i++)
    {
        if(list[i] == ' ')
        {
            int j;
            for(j = i; (j >=0) && (list[j] != '/'); j--);
            j++;

            ObjectFilesList.add(list.cut(j,i-1));
        }
    }
}


void Builder::Load_Options()
{
    std::ifstream Options_File("options.txt");
    char temp[60];
    string file;

    while(Options_File.getline(temp,60)) file += temp;
    Options_File.close();

    file.remove(' ');

    for(int i = 0; i < file.length;)
    {
        if(file[i] == '[')
        {
            Options.add();
            int start = i+2;
            int end = start;
            for(; file[end] != ']'; end++);
            end--;
            Options.last()[0] = file.cut(start-1,end);
            end += 4;
            start = end-1;
            for (; file[end] != '[' && file[end] != '\0' ; end++);
            end--;
            Options.last()[1] = file.cut(start,end);
            end++;
            i = end;
        }
    }

    for(int i = 0; i < Options.size; i++)
    {
        if(Options[i][0] == "sdl")
        {
            Options[i][1].insert(' ',-9);
            Options[i][1].insert(' ',-14);
            Options[i][1].insert(' ',-21);
            Options[i][1].insert(' ',-30);
        }
    }

    for(int i = 0; i < Options.size; i++)
    {
        if(Options[i][0] == "flags")
        {
            int count = 0;
            for(int j = 0;j < Options[i][1].length;)
            {
                if(Options[i][1][j] == '-')
                {
                    if(count == 0) 
                    {
                        count++;
                        j++;
                    }
                    else 
                    {
                        Options[i][1].insert(' ',j);
                        j += 2;
                    }
                }
                else j++;
            }
        }
    }
}

void Builder::Load_Cpp_Files_Dependencies()
{ 
    std::ifstream FD_File("cpp_files_dependencies.txt");
    char temp[30];
    string file;

    while(FD_File.getline(temp,30)) 
    {
        file += temp;
    }
    FD_File.close();

    for(int i = 0; i < file.length;)
    {
        if(file[i] == '[')
        {
            CppFilesDependencies.add();

            int start = i+1;
            int end = start;
            for(; file[end] != ']';end++);
            end--;
            CppFilesDependencies.last().first() = file.cut(start,end);
            end += 2;
            while(file[end] != '[' && end < file.length)
            {
                start = end;
                for (; file[end] != '.';  end++);
                end += 1;

                auto exist = [this] (const string& file_name)
                {
                    for(int i = 0; i < HeaderFilesLogs.size; i++)
                    {
                        if(HeaderFilesLogs[i].Belong(file_name)) return true;
                    }

                    return false;
                };

                if(exist(file.cut(start,end))) CppFilesDependencies.last().second().add(file.cut(start,end));                
                end++;
            }
            i = end;
        }
        else i++; 
    }
    
    for(int i = 0; i < CppFilesDependencies.size; i++)
    {
        bool found = false;
        for(int j = 0; j < CppFilesLogs.size; j++)
        {
            if(CppFilesDependencies[i].first() == CppFilesLogs[j][0])
            {
                found = true;
                break;
            }
        }
        if(!found) CppFilesDependencies.pop(i);
    }

    for(int i = 0; i < CppFilesLogs.size; i++)
    {
        bool found = false;
        for(int j = 0; j < CppFilesDependencies.size; j++)
        {
            if(CppFilesLogs[i][0] == CppFilesDependencies[j].first())
            {
                found = true;
                break;
            }
        }
        if(!found) Add_Cpp_File_Dependencies(CppFilesLogs[i][0]);
    }
}

void Builder::Save_Logs(const char* path,vector<arr<string,3>>& FilesLogs)
{
    std::ofstream FL(path);
    for(int i = 0; i < FilesLogs.size; i++)
    {
        FL << FilesLogs[i][0] + " " << FilesLogs[i][1] + " " << FilesLogs[i][2] << "\n";
    }
    FL.close();
}

void Builder::Save_Headerfiles_Logs() { Save_Logs("log_files/headers_logs.txt", HeaderFilesLogs); }


void Builder::Save_Cppfiles_Logs() { Save_Logs("log_files/cpp_logs.txt", CppFilesLogs); }
        
void Builder::Save_Cpp_Files_Dependencies()
{
    std::ofstream CFD_File("cpp_files_dependencies.txt");
    for(int i = 0; i < CppFilesDependencies.size; i++)
    {
        CFD_File << "[" + CppFilesDependencies[i].first() + "]" << "\n";
        
        for(int j = 0; j < CppFilesDependencies[i].second().size; j++)
        { CFD_File << CppFilesDependencies[i].second()[j] << "\n"; }

        CFD_File << "\n";
    }
}

void Builder::Missing_Path_Date(vector<arr<string,3>>& FilesLogs,vector<string>& Modif_List)
{
    for(int i = FilesLogs.size-1; i >= 0; i--)
    {
        if(FilesLogs[i][1] == "---")
        {
            FilesLogs[i][1] = Get_File_Path(FilesLogs[i][0]);
        }
    } 

    for(int i = FilesLogs.size-1; i >= 0; i--)
    {
        if(FilesLogs[i][2] == "---")
        {
            Modif_List.add(FilesLogs[i][0]);
            FilesLogs[i][2] = Get_File_Date(FilesLogs[i][1]);
        }
    }

}


string Builder::Find_Option(const string& option)
{
    for(int i = 0; i < Options.size; i++)
    {
        if(Options[i][0] == option) return Options[i][1];
    }
    return "Not found";
}


string Builder::Command_Result(const string& command)
{
    char temp[100];
    string command_result;
    FILE * fp = popen(command.String,"r");
    while (fgets(temp,15,fp)) command_result += temp; 
    command_result.replace('\n',' ');
    return command_result;
}

string Builder::Get_File_Date(const string& path)
{
    string shell = Command_Result("stat -c \"%y\"  " + path);
    shell.replace(' ','_');
    return shell;
}

void Builder::Update_File_Date(const string& file_name,vector<arr<string,3>>& FilesLogs)
{
    for(int i = 0; i < FilesLogs.size; i++)
    {
        if(FilesLogs[i][0] == file_name)
        {
            FilesLogs[i][2] = Get_File_Date(FilesLogs[i][1]);
            break;
        }
    }
}

void Builder::Remove_Dates()
{
    for(int i = 0; i < HeaderFilesLogs.size; i++)
    { if(HeaderFilesLogs[i][0] != "Build.h") HeaderFilesLogs[i][2] = "---"; }

    for(int i = 0; i < CppFilesLogs.size; i++)
    { if(CppFilesLogs[i][0] != "Build.cpp") CppFilesLogs[i][2] = "---"; }
}

void Builder::Update_Files_Dates()
{
    for(int i = 0; i < ModifiedHeaderFilesList.size; i++) { Update_File_Date(ModifiedHeaderFilesList[i],HeaderFilesLogs); }
    for(int i = 0; i < ModifiedCppFilesList.size; i++)    { Update_File_Date(ModifiedCppFilesList[i],CppFilesLogs); }
}

void Builder::Compile_Build_System()
{
    Update_File_Date("Build.cpp",CppFilesLogs);
    Update_File_Date("Build.h",HeaderFilesLogs);

    Save_Cppfiles_Logs();
    Save_Headerfiles_Logs();

    system("clang++ Build.cpp ../externals/string.cpp -o  build.exe");
    system("./build.exe build");
}


void Builder::Check_Dependencies_Exist()
{

    for(int i = 0; i < CppFilesDependencies.size; i++)
    {
        bool found = false;
        for(int j = 0; j < CppFilesLogs.size; j++)
        {
            if(CppFilesDependencies[i].first() == CppFilesLogs[j][0])
            {
                found = true;
                break;
            }
        }
        if(!found) CppFilesDependencies.pop(i);
    }

    for(int i = 0; i < CppFilesDependencies.size; i++)
    {
        if(CppFilesDependencies[i].second().IsEmpty())
        {
            std::cout << "Enter " + CppFilesDependencies[i].first() + " dependeces , enter \"exit\" to stop entering dependencies"  << std::endl;

            auto Is_Header_File = [] (const string& file_name)
            {
                for(int i = 0; i < file_name.length; i++)
                {
                    if((file_name[i] == '.') && (file_name[i+1] == 'h') && (i+2 == file_name.length))
                    return true;                                         
                }
                return false;                                    
            };

            auto Header_File_Exist = [this](const string& file_name)
            {
                for(int i = 0; i < HeaderFilesLogs.size;i++)
                {
                    if(HeaderFilesLogs[i][0] == file_name) return true;                      }
                return false;                                        
            };

            string file; 
            int a = 0;
            while(true)
            {
                if(a == 0) a++;
                else std::cout << "Enter next Dependence" << std::endl;
                std::cin >> file;
                if(file == "exit") break;
                if(Is_Header_File(file) && Header_File_Exist(file))
                {
                    CppFilesDependencies[i].second().add(file);
                }   
                else std::cout << "Invalid or Nonexistant" << std::endl;
            }
        }
        else
        {
            for(int j = 0; j < CppFilesDependencies[i].second().size; j++)
            {
                bool found = false;
                for(int k = 0; k < HeaderFilesLogs.size; k++)
                {
                    if(HeaderFilesLogs[k][0] == CppFilesDependencies[i].second()[j])
                    {
                        found = true;
                        break;
                    }
                }
                if(!found) CppFilesDependencies[i].second().pop(j);
            }
        }
    }
}

void Builder::Initialise_Modified_Files_List()
{
    for(int i = 0; i < HeaderFilesLogs.size; i++)
    {
        if(HeaderFilesLogs[i][2] != Get_File_Date(HeaderFilesLogs[i][1]))
        {
            if(!ModifiedHeaderFilesList.Belong(HeaderFilesLogs[i][0])) ModifiedHeaderFilesList.add(HeaderFilesLogs[i][0]) ;
        }
    }

    for(int i = 0; i < CppFilesLogs.size; i++)
    {
        if(CppFilesLogs[i][2] != Get_File_Date(CppFilesLogs[i][1]))
        {
            if(!ModifiedCppFilesList.Belong(CppFilesLogs[i][0])) ModifiedCppFilesList.add(CppFilesLogs[i][0]);
        }
    }
}

void Builder::Initialise_Cpp_Files_To_Compile_List()
{
    for(int i = 0; i < CppFilesDependencies.size; i++)
    {
        if(ModifiedCppFilesList.Belong(CppFilesDependencies[i].first())) 
        {
            CppFilesToCompile.add();
            CppFilesToCompile.last()[0] = CppFilesDependencies[i].first();
            CppFilesToCompile.last()[1] = Get_Path(CppFilesDependencies[i].first(),CppFilesLogs);
        }
        else
        {
            bool found = false;
            for(int j = 0; j < CppFilesDependencies[i].second().size; j++)
            {
                if(ModifiedHeaderFilesList.Belong(CppFilesDependencies[i].second()[j]))
                {
                    CppFilesToCompile.add();
                    CppFilesToCompile.last()[0] = CppFilesDependencies[i].first();
                    CppFilesToCompile.last()[1] = Get_Path(CppFilesDependencies[i].first(),CppFilesLogs);
                    found = true;
                    break;
                }
            }

            if(!found)
            {
                string temp = CppFilesDependencies[i].first();
                temp.del(-3).insert_at_end('o');

                if(ObjectFilesList.Belong(temp))
                {
                    CppFilesToCompile.add();
                    CppFilesToCompile.last()[0] = CppFilesDependencies[i].first();
                    CppFilesToCompile.last()[1] = Get_Path(CppFilesDependencies[i].first(),CppFilesLogs);
                }
            }
        }
    }
}

void Builder::Header_File_Exist()
{
    auto notExist = [this] (const string& file_name)
    {
        if(Get_File_Path(file_name).IsEmpty()) return false;
        else return true;
    };

    for(int i = 0; i < HeaderFilesLogs.size; i++)
    {
        if(!notExist(HeaderFilesLogs[i][0]))
        {
            std::cout << "File " + HeaderFilesLogs[i][0] + " does not exist" << std::endl;
            HeaderFilesLogs.pop(i); 
        }
        else if(HeaderFilesLogs[i][1] != Get_File_Path(HeaderFilesLogs[i][0]))
        { HeaderFilesLogs[i][1] = Get_File_Path(HeaderFilesLogs[i][0]); }
    }
}

void Builder::Cpp_File_Exist()
{
    auto notExist = [this] (const string& file_name)
    {
        if(Get_File_Path(file_name).IsEmpty()) return false;
        else return true;
    };

    for(int i = 0; i < CppFilesLogs.size; i++)
    {
        if(!notExist(CppFilesLogs[i][0])) 
        {
            std::cout << "File " + CppFilesLogs[i][0] + " does not exist"  << std::endl;
            CppFilesLogs.pop(i);
        }  
        else if(CppFilesLogs[i][1] != Get_File_Path(CppFilesLogs[i][0]))
        { CppFilesLogs[i][1] = Get_File_Path(CppFilesLogs[i][0]); }
    }
}

void Builder::Search_New_Header_Files() 
{ 
    string list = Command_Result("find .. \\( -path \"../externals/SDL\" -o -path \"../externals/SDL3_ttf-3.2.2\"  \\)  -prune -o  -type f -name \"*.h\" -print");
    for(int i = 0; i < list.length; i++)
    {
        if(list[i] == ' ')
        {
            int j;
            for( j = i; (j >= 0) && (list[j] != '/'); j--);
            j++;
            
            bool Not_In_Log = true;
            for(int k = 0; k <HeaderFilesLogs.size;  k++)
            {
                if(HeaderFilesLogs[k][0] == list.cut(j,i-1))
                {
                    Not_In_Log = false;
                    break;
                }
            }

            if(Not_In_Log)
            {
                int k;
                for(k = i-1; (k >= 0) && (list[k] != ' '); k--);
                k++;

                HeaderFilesLogs.add();
                HeaderFilesLogs.last()[0] = list.cut(j,i-1);
                HeaderFilesLogs.last()[1] = list.cut(k,i-1);
                HeaderFilesLogs.last()[2] = Get_File_Date(HeaderFilesLogs.last()[1]);

                ModifiedHeaderFilesList.add(HeaderFilesLogs.last()[0]);

                std::cout << "New Header File with name " + HeaderFilesLogs.last()[0] + " was added " << std::endl;
            }
        }
    }
}


void Builder::Add_Cpp_File_Dependencies(const string& file_name)
{
    CppFilesDependencies.add();
    CppFilesDependencies.last().first() = file_name;

    std::cout << "Enter " + file_name + " dependeces , enter \"exit\" to stop entering dependencies"  << std::endl;
    auto Is_Header_File = [] (const string& file_name)
    {
        for(int i = 0; i < file_name.length; i++)
        {
            if((file_name[i] == '.') && (file_name[i+1] == 'h') && (i+2 == file_name.length))
            return true;
        }
        return false;
    };

    auto Header_File_Exist = [this](const string& file_name)
    {
        for(int i = 0; i < HeaderFilesLogs.size;i++)
        {
            if(HeaderFilesLogs[i][0] == file_name) return true;
        }
        return false;
    };

    string file; 
    int a = 0;
    while(true)
    {
        if(a == 0) a++;
        else std::cout << "Enter next Dependence" << std::endl;

        std::cin >> file;
        if(file == "exit") break;
        if(Is_Header_File(file) && Header_File_Exist(file))
        {
            CppFilesDependencies.last().second().add(file);
        }
        else std::cout << "Invalid or Nonexistant" << std::endl;
    }
}

void Builder::Search_New_Cpp_Files()
{
    string list = Command_Result("find .. \\( -path \"../externals/SDL\" -o -path \"../externals/SDL3_ttf-3.2.2\"  \\) -prune -o  -type f -name \"*.cpp\" -print");
    for(int i = 0; i < list.length; i++)
    {
        if(list[i] == ' ')
        {
            int j;
            for( j = i; (j >= 0) && (list[j] != '/'); j--);
            j++;
            
            bool Not_In_Log = true;
            for(int k = 0; k < CppFilesLogs.size;  k++)
            {
                if(CppFilesLogs[k][0] == list.cut(j,i-1))
                {
                    Not_In_Log = false;
                    break;
                }
            }

            if(Not_In_Log)
            {
                int k;
                for(k = i-1; (k >= 0) && (list[k] != ' '); k--);
                k++;

                CppFilesLogs.add();
                CppFilesLogs.last()[0] = list.cut(j,i-1);
                CppFilesLogs.last()[1] = list.cut(k,i-1);
                CppFilesLogs.last()[2] = Get_File_Date(CppFilesLogs.last()[1]);

                ModifiedCppFilesList.add(CppFilesLogs.last()[0]);

                std::cout << "New Cpp File with name " + CppFilesLogs.last()[0] + " was added " << std::endl;
                Add_Cpp_File_Dependencies(CppFilesLogs.last()[0]);
            }
        }
    }
}


void Builder::Compile_File(string file_name,const string& path)
{
    string options;
    string sd;

    for(int i = 0; i < Options.size; i++)
    {
        if((Options[i][0] != "path") && (Options[i][0] != "sdl") && (Options[i][0] != "program"))
        {
            options += Options[i][1];
            options += ' ';
        }
    }
    string command = options + " -c " + path + " " + sd + " -o " + "bin/" + file_name.del(-3).insert_at_end('o');
    // std::cout << "command = " << command << std::endl;
    system(command);
}


void Builder::Complie_Object_Files()
{
    string list;
    string options;
    int position = 0, sdl = 0;

    for(int i = 0; i < ObjectFilesList.size; i++) 
    { list += " bin/" + ObjectFilesList[i]; }

    for(int i = 0; i < Options.size; i++)
    {
        if(Options[i][0] == "program") position = i;
        else if(Options[i][0] == "sdl") sdl = i;
        else if (Options[i][0] != "path") 
        {
            options += Options[i][1];
            options += ' ';
        }
    }

    system(options + list + " " + Options[sdl][1] + " -o " + "bin/" + Options[position][1]);
}

string Builder::Get_Path(const string& file_name,vector<arr<string,3>>& det)
{
    for(int i = 0; i < det.size; i++)
    {
        if(det[i][0] == file_name) return det[i][1];
    }
    std::cout << "File named \"" + file_name + "\" " + "does not exist" << std::endl;
    return string();
}

string Get_O(const string&);

string Builder::Get_Date(const string& file_name,vector<arr<string,3>>& det)
{
    for(int i = 0; i < det.size; i++)
    {
        if(det[i][0] == file_name) return det[i][1];
    }
    std::cout << "File named \"" + file_name + "\" " + "does not exist" << std::endl;
    return string();
}


void Builder::Compile_Project()
{
    for(int i = 0; i < CppFilesToCompile.size; i++) 
    { 
        Compile_File(CppFilesToCompile[i][0],CppFilesToCompile[i][1]); 
        string objFile = CppFilesToCompile[i][0].del(-3).insert_at_end('o');
        if(!ObjectFilesList.Belong(objFile)) ObjectFilesList.add(objFile);
    }
    
    Complie_Object_Files();
}

bool Builder::Verfy_If_Build_Files_Modified()
{
    if((ModifiedCppFilesList.Belong("Build.cpp") || ModifiedHeaderFilesList.Belong("Build.h")))
    return true;
    else return false;
}

void Builder::Compile()
{

    Update_Files_Dates();

    Compile_Project();

    Save_Headerfiles_Logs();
    Save_Cppfiles_Logs();
    Save_Cpp_Files_Dependencies();
}

void Builder::build()
{
    Load_Headerfiles_Logs();
    Search_New_Header_Files();
    Load_Cppfiles_Logs();
    Load_Cpp_Files_Dependencies();
    Search_New_Cpp_Files();
    Initialise_Modified_Files_List();
    if(Verfy_If_Build_Files_Modified()) Compile_Build_System();
    else
    {
        Check_Dependencies_Exist();
        Initialise_Cpp_Files_To_Compile_List();
        Initialise_Object_File_list();
        Load_Options();
        Compile();
    }
}

void Builder::run()
{
    Load_Options();
    string program_name;
    for(int i = 0; i < Options.size; i++)
    {
        if(Options[i][0] == "program") 
        {
            program_name = Options[i][1];
            break;
        }
    }
    system("bin/" + program_name);
}

string Builder::Get_File_Path(const string& file_name) { return Command_Result("find ..  \\( -path \"../externals/SDL\" -o -path \"../externals/SDL3_ttf-3.2.2\"  \\)  -prune -o  -name " + file_name + " -print").cut(0,-2); }

void Builder::Load_Headerfiles_Logs() { Load_logs("log_files/headers_logs.txt",HeaderFilesLogs,ModifiedHeaderFilesList,&Builder::Header_File_Exist); }

void Builder::Load_Cppfiles_Logs() { Load_logs("log_files/cpp_logs.txt",CppFilesLogs,ModifiedCppFilesList,&Builder::Cpp_File_Exist); }


int main(int argc, const char* argv[])
{
    string option = argv[1];
    if(option == "build")
    {
        Builder builder;
        builder.build();
    }
    else if(option == "run")
    {
        Builder builder;
        builder.run();
    }
    else { std::cout << "Invalid option" << std::endl; }
    return 0;
}