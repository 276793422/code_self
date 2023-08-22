// GetWindowPath.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>

HWND GetTopParent(HWND hWnd)
{
    HWND hDesktop = GetDesktopWindow();
    HWND hTop = hWnd;
    int i = 0;
    for (i = 0; i < 20; i++)
    {
        HWND hTemp = GetParent(hTop);
        if (hTemp == NULL)
        {
            DWORD dwError = GetLastError();
            if (dwError == 0)
            {
                break;
            }
            else
            {
                return NULL;
            }
        }
        if (hTemp == hDesktop)
        {
            break;
        }
        hTop = hTemp;
    }

    if (i >= 20)
    {
        return NULL;
    }

    //  是顶级父窗口
    //  hTop 

    char szBuffer[1024] = "";
    if (GetClassNameA(hTop, szBuffer, 1024) == 0)
    {
        return NULL;
    }

    if (strcmp(szBuffer, "CabinetWClass") != 0)
    {
        return NULL;
    }

    return hTop;
}

int GetWndText(HWND hWnd, LPSTR lpstr, int nLen)
{
    memset(lpstr, 0, nLen);
    LRESULT lr = SendMessageA(hWnd, WM_GETTEXT, nLen, (LPARAM)lpstr);
    return strlen(lpstr);
}

BOOL GetSubComboboxText(HWND hWnd, LPSTR lpcsName)
{
#if 1

    int n = GetWndText(hWnd, lpcsName, 1024);
    if (n == 0)
    {
        return FALSE;
    }

    return TRUE;
#else

    struct WINDOW_STRUCT
    {
        HWND hParent;
        LPSTR lpcsName;
        HWND hRet;
    };
    WINDOW_STRUCT winReturn = { hWnd, lpcsName, NULL };

    EnumChildWindows(hWnd, [](HWND hTemp, LPARAM lParam) -> BOOL {

        WINDOW_STRUCT* pThis = (WINDOW_STRUCT*)lParam;

        char szBuffer[1024] = "";
        if (GetClassNameA(hTemp, szBuffer, 1024) == 0)
        {
            return TRUE;
        }

        if (strcmp(szBuffer, "Edit") != 0)
        {
            return TRUE;
        }

        //int nLen = GetWindowTextA(hTemp, pThis->lpcsName, 1024);
        int nLen = GetWndText(hTemp, pThis->lpcsName, 1024);
        printf("Get Combobox Text : %30s : 0x%08X : %d \n", pThis->lpcsName, (DWORD)hTemp, nLen);
        if (nLen != 0)
        {
            return TRUE;
        }

        pThis->hRet = hTemp;
        return FALSE;
        }, (LPARAM)&winReturn);

    //printf("Find Window : %30s : 0x%08X \n", lpcsName, (DWORD)winReturn.hRet);
    if (winReturn.lpcsName[0])
    {
        return TRUE;
    }
    return FALSE;

#endif // 0
}

HWND GetSubWorkerAsName1(HWND hWnd, LPCSTR lpcsName)
{
    struct WINDOW_STRUCT
    {
        HWND hParent;
        LPCSTR lpcsName;
        HWND hRet;
    };
    WINDOW_STRUCT winReturn = { hWnd, lpcsName, NULL };

    EnumChildWindows(hWnd, [](HWND hTemp, LPARAM lParam) -> BOOL {
        WINDOW_STRUCT* pThis = (WINDOW_STRUCT*)lParam;
        if (GetParent(hTemp) != pThis->hParent)
        {
            return TRUE;
        }

        char szBuffer[1024] = "";
        int nLen = GetWindowTextA(hTemp, szBuffer, 1024);
        if (nLen != 0)
        {
            return TRUE;
        }

        if (GetClassNameA(hTemp, szBuffer, 1024) == 0)
        {
            return TRUE;
        }

        if (strcmp(szBuffer, pThis->lpcsName) != 0)
        {
            return TRUE;
        }

        pThis->hRet = hTemp;
        return FALSE;
        }, (LPARAM)&winReturn);

    printf("Find Window : %30s : 0x%08X \n", lpcsName, (DWORD)winReturn.hRet);
    return winReturn.hRet;
}

HWND GetSubCombobox(HWND hWnd)
{
    struct WINDOW_STRUCT
    {
        HWND hParent;
        LPCSTR lpcsName;
        HWND hRet;
    };
    WINDOW_STRUCT winReturn = { hWnd, "ComboBoxEx32", NULL };

    EnumChildWindows(hWnd, [](HWND hTemp, LPARAM lParam) -> BOOL {
        WINDOW_STRUCT* pThis = (WINDOW_STRUCT*)lParam;
        if (GetParent(hTemp) != pThis->hParent)
        {
            return TRUE;
        }

        char szBuffer[1024] = "";
        if (GetClassNameA(hTemp, szBuffer, 1024) == 0)
        {
            return TRUE;
        }

        if (strcmp(szBuffer, pThis->lpcsName) != 0)
        {
            return TRUE;
        }

        pThis->hRet = hTemp;
        return FALSE;
        }, (LPARAM)&winReturn);

    printf("Find Window : %30s : 0x%08X \n", winReturn.lpcsName, (DWORD)winReturn.hRet);
    return winReturn.hRet;
}

HWND GetSubWorkerWindow(HWND hWnd)
{
    LPCSTR lpszName = "WorkerW";
    HWND hRet = GetSubWorkerAsName1(hWnd, lpszName);
    return hRet;
}

HWND GetSubWorkerReBar(HWND hWnd)
{
    LPCSTR lpszName = "ReBarWindow32";
    HWND hRet = GetSubWorkerAsName1(hWnd, lpszName);
    return hRet;
}

HWND GetSubWorkerAddress(HWND hWnd)
{
    LPCSTR lpszName = "Address Band Root";
    HWND hRet = GetSubWorkerAsName1(hWnd, lpszName);
    return hRet;
}

HWND GetSubWorkerProgress(HWND hWnd)
{
    LPCSTR lpszName = "msctls_progress32";
    HWND hRet = GetSubWorkerAsName1(hWnd, lpszName);
    return hRet;
}

std::string GetWindowPath1(HWND hWnd)
{
    DWORD dwFrame = 0;

    HWND hTop = GetTopParent(hWnd);
    printf("%5u : 0x%08X \n", dwFrame++, (DWORD)hTop);
    if (hTop == NULL)
    {
        return "";
    }

    HWND hWindowWorker = GetSubWorkerWindow(hTop);
    printf("%5u : 0x%08X \n", dwFrame++, (DWORD)hWindowWorker);
    if (hWindowWorker == NULL)
    {
        return "";
    }

    HWND hReBar = GetSubWorkerReBar(hWindowWorker);
    printf("%5u : 0x%08X \n", dwFrame++, (DWORD)hReBar);
    if (hReBar == NULL)
    {
        return "";
    }

    HWND hAddress = GetSubWorkerAddress(hReBar);
    printf("%5u : 0x%08X \n", dwFrame++, (DWORD)hAddress);
    if (hAddress == NULL)
    {
        return "";
    }

    HWND hProgress = GetSubWorkerProgress(hAddress);
    printf("%5u : 0x%08X \n", dwFrame++, (DWORD)hProgress);
    if (hProgress == NULL)
    {
        return "";
    }

    HWND hCombobox = GetSubCombobox(hProgress);
    printf("%5u : 0x%08X \n", dwFrame++, (DWORD)hCombobox);
    if (hCombobox == NULL)
    {
        return "";
    }

    char szBuffer[1024] = "";
    BOOL bRet = GetSubComboboxText(hCombobox, szBuffer);
    printf("%5u : 0x%08X : %s\n", dwFrame++, (DWORD)hCombobox, szBuffer);
    if (bRet == FALSE)
    {
        return "";
    }

    return szBuffer;
}


BOOL GetSubBreadcrumbParentText(HWND hWnd, LPSTR lpcsName)
{
    struct WINDOW_STRUCT
    {
        HWND hParent;
        LPSTR lpcsName;
        HWND hRet;
    };
    WINDOW_STRUCT winReturn = { hWnd, lpcsName, NULL };

    EnumChildWindows(hWnd, [](HWND hTemp, LPARAM lParam) -> BOOL {

        WINDOW_STRUCT* pThis = (WINDOW_STRUCT*)lParam;

        char szBuffer[1024] = "";
        if (GetClassNameA(hTemp, szBuffer, 1024) == 0)
        {
            return TRUE;
        }

        if (strcmp(szBuffer, "ToolbarWindow32") != 0)
        {
            return TRUE;
        }

        //int nLen = GetWindowTextA(hTemp, pThis->lpcsName, 1024);
        int nLen = GetWndText(hTemp, pThis->lpcsName, 1024);
        printf("Get Combobox Text : %30s : 0x%08X : %d \n", pThis->lpcsName, (DWORD)hTemp, nLen);
        if (nLen != 0)
        {
            return TRUE;
        }

        pThis->hRet = hTemp;
        return FALSE;
        }, (LPARAM)&winReturn);

    //printf("Find Window : %30s : 0x%08X \n", lpcsName, (DWORD)winReturn.hRet);
    if (winReturn.lpcsName[0])
    {
        return TRUE;
    }
    return FALSE;
}

HWND GetSubWorkerBread(HWND hWnd)
{
    LPCSTR lpszName = "Breadcrumb Parent";
    HWND hRet = GetSubWorkerAsName1(hWnd, lpszName);
    return hRet;
}

std::string GetWindowPath2(HWND hWnd)
{
    DWORD dwFrame = 0;

    HWND hTop = GetTopParent(hWnd);
    printf("%5u : 0x%08X \n", dwFrame++, (DWORD)hTop);
    if (hTop == NULL)
    {
        return "";
    }

    HWND hWindowWorker = GetSubWorkerWindow(hTop);
    printf("%5u : 0x%08X \n", dwFrame++, (DWORD)hWindowWorker);
    if (hWindowWorker == NULL)
    {
        return "";
    }

    HWND hReBar = GetSubWorkerReBar(hWindowWorker);
    printf("%5u : 0x%08X \n", dwFrame++, (DWORD)hReBar);
    if (hReBar == NULL)
    {
        return "";
    }

    HWND hAddress = GetSubWorkerAddress(hReBar);
    printf("%5u : 0x%08X \n", dwFrame++, (DWORD)hAddress);
    if (hAddress == NULL)
    {
        return "";
    }

    HWND hProgress = GetSubWorkerProgress(hAddress);
    printf("%5u : 0x%08X \n", dwFrame++, (DWORD)hProgress);
    if (hProgress == NULL)
    {
        return "";
    }

    HWND hBread = GetSubWorkerBread(hProgress);
    printf("%5u : 0x%08X \n", dwFrame++, (DWORD)hBread);
    if (hBread == NULL)
    {
        return "";
    }

    char szBuffer[1024] = "";
    BOOL bRet = GetSubBreadcrumbParentText(hBread, szBuffer);
    printf("%5u : 0x%08X : %s\n", dwFrame++, (DWORD)hBread, szBuffer);
    if (bRet == FALSE)
    {
        return "";
    }

    return szBuffer;
}

std::string GetWindowPath(HWND hWnd)
{
    std::string str = "";

    str = GetWindowPath2(hWnd);
    if (str.empty())
    {
        str = GetWindowPath1(hWnd);
    }
    else
    {
        std::string::size_type pos = str.find(": ");
        if (pos != std::string::npos)
        {
            str = str.substr(pos + 2);
        }
    }
    return str;
}

int main(int argc, char** argv)
{
    HWND hWnd = (HWND)0x00061702;

    if (argc == 2)
    {
        hWnd = (HWND)strtoul(argv[1], 0, 16);
    }

    std::cout << GetWindowPath(hWnd) << std::endl;

    system("pause");

    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
