#include <iostream>
#include <windows.h>
#include <string>


using namespace std;

int main() {
    HANDLE hSem;
    hSem = OpenSemaphore(SEMAPHORE_MODIFY_STATE, FALSE, "MySemaphore1");

    string message;
    HANDLE memory;

    while(1){
        WaitForSingleObject(hSem,INFINITE);
        CloseHandle(memory);
        message.clear();
        message.resize(0);
        fflush(stdin);
        cout << endl << "Input message: ";
        fflush(stdin);
        cin.clear();
        char c;
        c = getchar();
        while(c!=EOF){
            message+=c;
            c = getchar();
        }

        if(message.size()>=4094)
            message="Sorry, very big message";
        memory = CreateFileMappingA(nullptr, nullptr, PAGE_READWRITE, 0, message.size()*sizeof(char),"MEMORY");
        char *to =(char*)::MapViewOfFile(memory, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
        strcpy(to,message.c_str());
        UnmapViewOfFile(to);

        ReleaseSemaphore(hSem , 1, NULL);
        Sleep(1000);
/****************************************************************************************/

    }

    return 0;
}