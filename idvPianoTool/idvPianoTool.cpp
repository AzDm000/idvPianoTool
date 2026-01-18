#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "json.hpp"
#include "RandomDelay.h"
#include "KeyInput.h"
#include "Level2Vk.h"
#include "Note.h"
#include "KeyList.h"

//main1
using json = nlohmann::json;

// 静态变量定义与初始化
std::mt19937 RandomDelay::gen((unsigned int)time(nullptr));
std::normal_distribution<double> RandomDelay::dis(0.0, 3.04);// 期望为0.0，标准差为3.04的正态分布(90%落在[-5, 5]区间)

int Note::scales[11] = { 0 };//存储各音阶音符数
std::vector<Note> NoteList;

int Level2Vk::mode = 1;
std::map<std::pair<char, int>, WORD> Level2Vk::mp1;
std::map<std::pair<char, int>, WORD> Level2Vk::mp2;//另一种类的乐器，暂时留空

std::string path = "C:\\Users\\Moonlight\\Desktop";

bool IsRunAsAdmin() {
	BOOL fRet = FALSE;
	HANDLE hToken = NULL;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
		TOKEN_ELEVATION elevation;
		DWORD dwSize;

		if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize)) {
			fRet = elevation.TokenIsElevated;
		}
	}
	if (hToken) {
		CloseHandle(hToken);
	}
	return fRet;
}

int reachfile(const std::string& command, const std::string& targetpath, json& j) {
	int flag = std::system(command.c_str());
	if (flag != 0) {
		std::cerr << "flp转换失败.\n";
		return -1;
	}
	std::ifstream ijson(targetpath);
	if (!ijson.is_open()) {
		std::cerr << "Can't open file: " << targetpath << '\n';
		return -1;
	}
	try {
		ijson >> j;
	}
	catch (json::parse_error& e) {
		std::cerr << "Parse_error: " << e.what() << '\n';
		return -1;
	}
	ijson.close();
	return 0;
}

int reachfile(const std::string& targetpath, json& j) {
	std::ifstream ijson(targetpath);
	if (!ijson.is_open()) {
		std::cerr << "Can't open file: " << targetpath << '\n';
		return -1;
	}
	try {
		ijson >> j;
	}
	catch (json::parse_error& e) {
		std::cerr << "Parse_error: " << e.what() << '\n';
		return -1;
	}
	ijson.close();
	return 0;
}

int playOnce() {

	NoteList.clear();//清除音符列表

	std::string exporterpath = "flp_exporter.exe";
	std::string outpath = "out.json";

	int reachMod; //flp2json
	std::cout << "input your target file:\n";
	std::string get;
	std::cin >> get;
	int searchGet = 0;
	if (get.size() <= 4) {
		std::cerr << "文件扩展名有误\n";
		return -1;
	}
	if (get[get.size() - 3] == 'f' && get[get.size() - 2] == 'l' && get[get.size() - 1] == 'p') {
		reachMod = 0;
	}
	else if (get[get.size() - 4] == 'j' && get[get.size() - 3] == 's' && get[get.size() - 2] == 'o' && get[get.size() - 1] == 'n') {
		reachMod = 1;
	}
	else {
		std::cerr << "文件扩展名有误\n";
		return -1;
	}
	json j;
	if (reachMod == 0) {
		if (reachfile((exporterpath + ' ' + get + ' ' + outpath).c_str(), outpath, j) != 0) {
			std::cerr << "程序已停止运行\n";
			return -1;
		}
	}
	else {
		if (reachfile(outpath, j) != 0) {
			std::cerr << "程序已停止运行\n";
			return -1;
		}
	}

	KeyList keys;

	//print all json contents;
	std::cout << "json内容(pitch, start/ms, end/ms)：\n(小写字母代表升调)\n";
	for (int i = 0; i < j.size(); i++) {
		std::cout << j[i]["pitch"] << ' ' << j[i]["start_ms"] << ' ' << j[i]["end_ms"] << '\n';
	}
	std::cout << "---------\n";
	for (int i = 0; i < j.size(); i++) {
		Note tmp(j[i]["pitch"], j[i]["start_ms"], j[i]["end_ms"]);
		//tmp.printNote();
		NoteList.push_back(tmp);
	}
	std::cout << "各音阶音符数(按音阶序号排序):\n";
	for (int i = 0; i < 11; i++) { std::cout << Note::scales[i] << ' '; }
	int Most = 0;
	for (int i = 1; i < 11; i++) {
		if (Note::scales[i] > Note::scales[Most])Most = i;
	}
	if (Most <= 1)Most = 2;
	else if (Most >= 9)Most = 8;

	if (Note::scales[Most + 2] != 0 && Note::scales[Most + 2] >= Note::scales[Most - 2] && Note::scales[Most + 1] >= Note::scales[Most - 1]) {
		Most++;
	}
	else if (Note::scales[Most - 2] != 0 && Note::scales[Most + 2] <= Note::scales[Most - 2] && Note::scales[Most + 1] <= Note::scales[Most - 1]) {
		Most--;
	}
	std::cout << "\nMost: " << Most << '\n';
	//return 0;
	//mp1的按键映射,取Most为中，越界区域将被划入同阶
	for (int i = 0; i < Most; i++) {
		Level2Vk::mp1.insert({ {'C', i}, VK_OEM_COMMA });// ','
		Level2Vk::mp1.insert({ {'D', i}, VK_OEM_PERIOD });// '.'
		Level2Vk::mp1.insert({ {'E', i}, VK_OEM_2 });// '/'
		Level2Vk::mp1.insert({ {'F', i}, 'I' });
		Level2Vk::mp1.insert({ {'G', i}, 'O' });
		Level2Vk::mp1.insert({ {'A', i}, 'P' });
		Level2Vk::mp1.insert({ {'B', i}, VK_OEM_4 }); // '['

		Level2Vk::mp1.insert({ {'c', i}, 'L' });
		Level2Vk::mp1.insert({ {'d', i}, VK_OEM_1 });// ';'
		Level2Vk::mp1.insert({ {'f', i}, '9' });
		Level2Vk::mp1.insert({ {'g', i}, '0' });
		Level2Vk::mp1.insert({ {'a', i}, VK_OEM_MINUS });// '-'
	}
	for (int i = Most; i < Most + 1; i++) {
		Level2Vk::mp1.insert({ {'C', i}, 'Z' });
		Level2Vk::mp1.insert({ {'D', i}, 'X' });
		Level2Vk::mp1.insert({ {'E', i}, 'C' });
		Level2Vk::mp1.insert({ {'F', i}, 'V' });
		Level2Vk::mp1.insert({ {'G', i}, 'B' });
		Level2Vk::mp1.insert({ {'A', i}, 'N' });
		Level2Vk::mp1.insert({ {'B', i}, 'M' });

		Level2Vk::mp1.insert({ {'c', i}, 'S' });
		Level2Vk::mp1.insert({ {'d', i}, 'D' });
		Level2Vk::mp1.insert({ {'f', i}, 'G' });
		Level2Vk::mp1.insert({ {'g', i}, 'H' });
		Level2Vk::mp1.insert({ {'a', i}, 'J' });
	}
	for (int i = Most + 1; i < 11; i++) {
		Level2Vk::mp1.insert({ {'C', i}, 'Q' });
		Level2Vk::mp1.insert({ {'D', i}, 'W' });
		Level2Vk::mp1.insert({ {'E', i}, 'E' });
		Level2Vk::mp1.insert({ {'F', i}, 'R' });
		Level2Vk::mp1.insert({ {'G', i}, 'T' });
		Level2Vk::mp1.insert({ {'A', i}, 'Y' });
		Level2Vk::mp1.insert({ {'B', i}, 'U' });

		Level2Vk::mp1.insert({ {'c', i}, '2' });
		Level2Vk::mp1.insert({ {'d', i}, '3' });
		Level2Vk::mp1.insert({ {'f', i}, '5' });
		Level2Vk::mp1.insert({ {'g', i}, '6' });
		Level2Vk::mp1.insert({ {'a', i}, '7' });
	}

	//将音符压入keys序列
	for (auto& i : NoteList) {
		keys.pushInput(i);
	}

	//return 0;
	std::cout << "--------\n虚拟键码序列(排序前):\n";
	keys.printKeyList();
	std::cout << "-------\n已按delay排序:";
	keys.sortself();
	keys.printKeyList();
	std::cout << "----------\n准备寻找第五人格进程\n";
	HWND hd = FindWindowW(nullptr, L"第五人格");
	if (!IsWindow(hd)) {
		std::cerr << "Window not found.\n";
		return -1;
	}
	else {
		std::cout << "Success!\n";
		keys.print2hd(hd);
	}
	return 0;
}

int main() {
	std::cout << "钢琴工具，支持输入flp文件与json文件\nflp文件将导出为out.json\n\n";
	if (!IsRunAsAdmin()) {
		std::cerr << "进程未拥有管理员权限，请重新启动\n";
		return 0;
	}
	while (1) {
		std::cout << "输入1进入选择，输入0退出：";
		int get;
		std::cin >> get;
		if (get == 0) {
			break;
		}
		else if (get == 1) {
			if (playOnce() != 0) {
				std::cerr << "本次执行失败。\n\n";
			}
			else {
				std::cout << "本次执行完毕。\n\n";
			}
		}
		else {
			continue;
		}
	}
}