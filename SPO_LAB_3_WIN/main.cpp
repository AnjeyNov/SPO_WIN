#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

int main() {

    HANDLE hSem;
    hSem = CreateSemaphore(NULL, 0, 1, "MySemaphore1");

    char *str = (char *) malloc(sizeof(char) * 100);
    strcpy(str, "C:\\Users\\nikol\\CLionProjects\\SPO_LAB_3_CHILD_WIN\\cmake-build-debug\\SPO_LAB_3_CHILD_WIN.exe");

    STARTUPINFO stInfo;
    PROCESS_INFORMATION prInfo;

    ZeroMemory(&stInfo, sizeof(STARTUPINFO));
    stInfo.cb = sizeof(STARTUPINFO);

    if (!CreateProcess(NULL,       // нмя исполняемого модуля
                       str,        // командная строка
                       NULL,       // атрибуты защиты процесса
                       NULL,       // атрибуты защиты потока
                       FALSE,      // признак наследования дескрипторов
                       NULL,       // флаги создания процесса
                       NULL,       // блок новой среды окружения
                       NULL,       // текущий каталог
                       &stInfo,    // вид главного окна
                       &prInfo     // информация о процессе
    )) {
        exit(1);
    }
    Sleep(1000);

    while (1) {

        WaitForSingleObject(hSem, INFINITE);

        HANDLE memory = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "MEMORY");
        char *out = (char *) ::MapViewOfFile(memory, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

        cout << "Server: " << out << " END" << endl;
        UnmapViewOfFile(out);
        CloseHandle(memory);
        ReleaseSemaphore(hSem, -1, NULL);
        Sleep(1500);
/***************************************************************************************/
    }

    WaitForSingleObject(prInfo.hProcess, INFINITE);
    CloseHandle(prInfo.hProcess);
    CloseHandle(prInfo.hThread);


    return 0;
}
