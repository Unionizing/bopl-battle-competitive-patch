
#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
#include <windows.h>
#include <wininet.h>
#include <unordered_map>
#include <tlhelp32.h>
#pragma comment(lib, "urlmon.lib")
#include <tlhelp32.h>
#include <thread>
#include <tchar.h>

bool IsProcessRunning(const TCHAR* const executableName) {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (!Process32First(snapshot, &entry)) {
        CloseHandle(snapshot);
        return false;
    }

    do {
        if (!_tcsicmp(entry.szExeFile, executableName)) {
            CloseHandle(snapshot);
            return true;
        }
    } while (Process32Next(snapshot, &entry));

    CloseHandle(snapshot);
    return false;
}

std::string getFromUrl(std::string url) {
    DWORD state = URLDownloadToFileA(NULL, url.c_str(), "_temp", 0, NULL);
    if (state != S_OK)
    {
        return NULL;
    }

    std::ifstream file("_temp");
    std::string result((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    DeleteFileA("_temp");
    return result;
}

void runGame()
{
    system("BoplBattle.bin");
}

int main()
{
    SetConsoleTitleA("Bopl Battle Launcher");
    if (IsProcessRunning(L"cheatengine-x86_64.exe")) {
        return EXIT_FAILURE;
    }

    if (IsProcessRunning(L"Cheat Engine.exe")) {
        return EXIT_FAILURE;
    }

    if (IsProcessRunning(L"Nopdeengine-x86_64.exe")) {
        return EXIT_FAILURE;
    }

    std::string dll = getFromUrl("https://github.com/Unionizing/bopl-battle-competitive-patch/raw/main/Assembly-CSharp.dll");

    DWORD state = URLDownloadToFileA(NULL, "https://raw.githubusercontent.com/Unionizing/bopl-battle-competitive-patch/main/game.bin", "BoplBattle.bin", 0, NULL);
    if (state != S_OK)
    {
        return EXIT_FAILURE;
    }
    //BoplBattle_Data\Managed\Assembly-CSharp.dll"
    std::ifstream file("BoplBattle_Data\Managed\Assembly-CSharp.dll");
    std::string result((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    std::size_t h1 = std::hash<std::string>{}(result);
    std::cout << h1;

    std::cout << "\n";

    std::size_t h2 = std::hash<std::string>{}(dll);
    std::cout << h2;

    if (std::to_string(h1) == std::to_string(h2)) {
        std::cout << "PATCH IS RUNNING\n";
    }
 
    std::thread gameThread(runGame);
    Sleep(500);
    HWND HWND = FindWindow(nullptr, L"BoplBattle");
    SetWindowText(HWND, L"Bopl Battle | 2.1.2 | Patch Version 1.0.1");
    std::cout << "Changed title of bopl battle\n";
    system("pause");
    gameThread.detach();
    system("taskkill /F /T /IM BoplBattle.bin");
}
