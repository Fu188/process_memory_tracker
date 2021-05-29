//#include <iostream>
//#include <string>
//
//#include "uninitializedMem.hpp"
//
//using namespace std;
//
//string FILEPATHRECORD = "../leakDetector/Output";
//vector<Mem_info> memAllocated;
//vector<Mem_info> memFreed;
//
//void updateMemStat(){
//    DIR* dir = opendir(FILEPATHRECORD.c_str());
//    dirent* p = NULL;
//    string format = "_Mem.txt";
//    while((p = readdir(dir)) != NULL)
//    {
//        if(p->d_name[0] != '.')
//        {
//            string name = FILEPATHRECORD + "/" + string(p->d_name);
//            if(strstr(name.c_str(), format.c_str()) != NULL){
//
//                FILE *f = fopen(name.c_str(), "r");
//                if(f!=NULL) {
//                    int mode = 0;
//                    void *address;
//                    unsigned int size;
//                    string file_name;
//                    unsigned int line;
//                    time_t start;
//
//                    string str = "";
//                    char in[100];
//                    while (NULL != fgets(in, 99, f)) {
//                        if (in[0] == '\n')
//                            continue;
//                        str = in;
//                        if (str == "[Allocate Memory]\n") {
//                            mode = 1;
//                            continue;
//                        } else if (str == "[Free Memory]\n") {
//                            mode = -1;
//                            continue;
//                        } else if (str == "       Memory Leak Summary\n")
//                            break;
//                        str.erase(remove(str.begin(), str.end(), '\n'), str.end());
//                        if (str.substr(0, 7) == "Address")
//                            address = (void *) (atoi(str.substr(11, 8).c_str()));
//                        else if (str.substr(0, 4) == "File") {
//                            file_name = str.substr(6);
//                        } else if (str.substr(0, 4) == "Line") {
//                            line = atoi(str.substr(6).c_str());
//                        } else if (str.substr(0, 4) == "Size") {
//                            size = atoi(str.substr(6).c_str());
//                        } else if (str.substr(0, 5) == "Start") {
//                            start = atol(str.substr(7).c_str());
//                            if (mode == 1) {
//                                Mem_info mem_info;
//                                memset(&mem_info, 0, sizeof(mem_info));
//                                mem_info.address = address;
//                                mem_info.size = size;
//                                mem_info.file_name = file_name;
//                                mem_info.line = line;
//                                mem_info.start = start;
//                                memAllocated.push_back(mem_info);
//                            }
//                            else if(mode == -1){
//                                int find = -1;
//                                for (int i = 0; i < memAllocated.size(); i++) {
//                                    if (memAllocated[i].address == address) {
//                                        find = i;
//                                        break;
//                                    }
//                                }
//                                if (find > -1){
//                                    memFreed.push_back(memAllocated[find]);
//                                    memAllocated.erase(memAllocated.begin() + find);
//                                }
//                            }
//                        }
//                    }
//                    fclose(f);
//                }
//            }
//        }
//    }
//    closedir(dir);
//}
//
//vector<Mem_info> getAllMemAllocated(){
//    updateMemStat();
//    return memAllocated;
//}
//
//vector<Mem_info> getAllMemFreed(){
//    updateMemStat();
//    return memFreed;
//}
//
//template<typename T> int checkAssign(T* address){
//    int size = sizeof(T); // sizeof(*address) is also ok
//    int start =
//    updateMemStat();
//    Mem_info mem;
//    int flag = 0;
//    for(int i = 0; i < memAllocated.size(); i++){
//        mem = memAllocated[i];
//        if(mem.address<=start)
//    }
//}
//
