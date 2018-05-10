#include <iostream>
#include <list>
#include <windows.h>
#include <stdlib.h>
using namespace std;
HANDLE hMutex;



void* Thread(void* pParams){

    cout << "Create thread " <<  GetCurrentThreadId() << endl;
    while (true)
    {
        WaitForSingleObject( hMutex, INFINITE );                        // заблоикровать
        cout << "THREAD " <<  GetCurrentThreadId() << endl;
        ReleaseMutex( hMutex );                                         // разблокировать
        Sleep(100);
    }
}

int main(int argc, char *argv[])
{
    list<HANDLE> threadIDlist;
    hMutex=CreateMutex( NULL, FALSE, NULL );
    while(true) {
        WaitForSingleObject( hMutex, INFINITE );                       // заблоикровать
        char symbol;
        cout << "Print symbol" << endl;
        //сичтать символ
        cin.get(symbol);

        switch(symbol) {
            //добавить процесс-шестерку
            case '+': {
                HANDLE hThr;
                //создание нового потока
                if((hThr=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Thread,NULL,0,NULL))==0){
                    cout << "Thread create error" << endl;
                    break;
                }
                //добавить ID потока-шестерки в лист потоков
                threadIDlist.push_back(hThr);
                Sleep(1000);
                SuspendThread(hThr);

            }break;

                //удалить процесс-шестерку
            case '-': {
                if (!threadIDlist.empty()) {
                    // удаляем поток
                    TerminateThread(threadIDlist.back(),NO_ERROR);
                    CloseHandle(threadIDlist.back());
                    Sleep(1000);
                    //удалить ID потока-шестерки из листа процессов
                    threadIDlist.pop_back();
                } else {
                    cout <<  "List is empty." << endl;
                }
            } break;
                //выйти удалив все потоки-шестерки
            case 'q':
                if(!threadIDlist.empty())
                {
                    for(HANDLE &childID: threadIDlist) {
                        //послать в последний созданный поток-шестерку сигнал вежливого завершиния
                        CloseHandle(childID);
                    }
                    //очистить лист
                    threadIDlist.clear();
                } exit(EXIT_SUCCESS);   // завершить программу с кодом 0
            default:
                continue;
        }
        for(HANDLE &childID: threadIDlist) {
            ResumeThread(childID);
            ReleaseMutex( hMutex );                                        // освободить
            Sleep(100);
            WaitForSingleObject( hMutex, INFINITE );                       // заблоикровать
            SuspendThread(childID);
        }
        cin.ignore();
        ReleaseMutex( hMutex );                              // освободить
        //Sleep(1000);
    }

    return 0;
}