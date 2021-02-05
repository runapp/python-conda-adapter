
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define CONDA_BASE_PATH L"D:\\apps\\Anaconda3"
wchar_t target_cmdline[4096] = CONDA_BASE_PATH L"\\python.exe";
//wchar_t target_cmdline[4096] = CONDA_BASE_PATH L"\\Scripts\\pip.exe";
//wchar_t target_cmdline[4096] = CONDA_BASE_PATH L"\\Scripts\\conda.exe";

/*
CONDA_BAT = CONDA_BASE_PATH L"\\condabin\\conda.bat"
CONDA_DEFAULT_ENV = base
CONDA_EXE = CONDA_BASE_PATH L"\\Scripts\\conda.exe"
CONDA_PREFIX = CONDA_BASE_PATH
CONDA_PROMPT_MODIFIER = (base)
CONDA_PYTHON_EXE = CONDA_BASE_PATH L"\\python.exe"
CONDA_SHLVL = 1
Path = CONDA_BASE_PATH L";" CONDA_BASE_PATH L"\\Library\\mingw-w64\\bin;" CONDA_BASE_PATH L"\\Library\\usr\\bin;" CONDA_BASE_PATH L"\\Library\\bin;" CONDA_BASE_PATH L"\\Scripts;" CONDA_BASE_PATH L"\\bin;"  CONDA_BASE_PATH L"\\condabin;"
*/
const wchar_t PATH_PREFIX[] =
CONDA_BASE_PATH L";"
CONDA_BASE_PATH L"\\Library\\mingw-w64\\bin;"
CONDA_BASE_PATH L"\\Library\\usr\\bin;"
CONDA_BASE_PATH L"\\Library\\bin;"
CONDA_BASE_PATH L"\\Scripts;"
CONDA_BASE_PATH L"\\bin;"
CONDA_BASE_PATH L"\\condabin;"
;

// We try to emulate what CommandLineToArgvW does.
LPCWSTR skip_one_argument(LPCWSTR cmd) {
	bool in_quote = false;
	int num_bs = 0;
	for (LPCWSTR t = cmd; ; t++) {
		switch (*t) {
		case L' ':
			if (!in_quote)return t;
			break;
		case L'\\':
			num_bs++;
			break;
		case 0:
			return t;
		case L'"':
			if ((num_bs & 1) == 0) {
				in_quote = !in_quote;
			}
		default:
			num_bs = 0;
		}
	}
}

int main()
{
	// Substitute the exe filename by another
	LPCWSTR ori_cmdline = GetCommandLineW();
	int ori_cmdline_len = wcslen(ori_cmdline) + wcslen(target_cmdline) + 1;

	LPWSTR cmdline = new wchar_t[ori_cmdline_len + 1];
	LPCWSTR ori_cmdline_args = skip_one_argument(ori_cmdline);
	wsprintfW(cmdline, L"%s%s", target_cmdline, ori_cmdline_args);

	// Set environments
	bool env_success = true;
	env_success = env_success && (SetEnvironmentVariableW(L"CONDA_BAT", CONDA_BASE_PATH L"\\condabin\\conda.bat") != 0);
	env_success = env_success && (SetEnvironmentVariableW(L"CONDA_DEFAULT_ENV", L"base") != 0);
	env_success = env_success && (SetEnvironmentVariableW(L"CONDA_EXE", CONDA_BASE_PATH L"\\Scripts\\conda.exe") != 0);
	env_success = env_success && (SetEnvironmentVariableW(L"CONDA_PREFIX", CONDA_BASE_PATH) != 0);
	env_success = env_success && (SetEnvironmentVariableW(L"CONDA_PROMPT_MODIFIER", L"(base)") != 0);
	env_success = env_success && (SetEnvironmentVariableW(L"CONDA_PYTHON_EXE", CONDA_BASE_PATH L"\\python.exe") != 0);
	env_success = env_success && (SetEnvironmentVariableW(L"CONDA_SHLVL", L"1") != 0);

	DWORD path_env_len = GetEnvironmentVariableW(L"Path", 0, 0);
	if (path_env_len) {
		DWORD PATH_PREFIX_LEN = wcslen(PATH_PREFIX);
		LPWSTR path_env = new wchar_t[PATH_PREFIX_LEN + path_env_len];
		wcscpy_s(path_env, PATH_PREFIX_LEN + path_env_len, PATH_PREFIX);
		if (GetEnvironmentVariableW(L"Path", path_env + PATH_PREFIX_LEN, path_env_len) != path_env_len - 1) {
			printf("Warning! GetEnvironmentVariable behaviours wired.");
		}
		SetEnvironmentVariableW(L"Path", path_env);
		delete[] path_env;
	}
	else {
		env_success = false;
	}

	if (!env_success) {
		printf("Warning! Setting environments failed.\n");
	}



	// Call CreateProcess
	STARTUPINFOW sinfo;
	ZeroMemory(&sinfo, sizeof(sinfo));
	sinfo.cb = sizeof(sinfo);
	sinfo.dwFlags = STARTF_USESTDHANDLES;
	sinfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	sinfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	sinfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);

	PROCESS_INFORMATION pinfo;

	if (CreateProcessW(0, cmdline, 0, 0, TRUE, 0, 0, 0, &sinfo, &pinfo) != TRUE) {
		printf("Failed to spawn target! err=%d\n", GetLastError());
		return -1;
	};

	CloseHandle(pinfo.hThread);
	WaitForSingleObject(pinfo.hProcess, INFINITE);

	DWORD exitcode = 0;
	if (GetExitCodeProcess(pinfo.hProcess, &exitcode) != TRUE) {
		printf("Failed to get exit code! err=%d\n", GetLastError());
		return -2;
	}
	return exitcode;
}
