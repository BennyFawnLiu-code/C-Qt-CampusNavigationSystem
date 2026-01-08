#include "VoiceBroadcast.h"
#include <iostream>  
#include <algorithm> 

// 定义全局语音开关（需与头文件的extern对应）
bool enableVoice = true;

void voiceBroadcast(const std::string& text) {
    if (!enableVoice) return; // 关闭则直接返回

    // 清理文本中的特殊字符（替换全角空格）
    std::string cleanText = text;

// 新替换代码（添加）：
    for (char& c : cleanText) {
        if (c == '　') { // 匹配全角空格
            c = ' ';    // 替换为半角空格
        }
    }

#ifdef _WIN32
    std::string command = "powershell -Command \"Add-Type -AssemblyName System.Speech; "
        "$synth = New-Object System.Speech.Synthesis.SpeechSynthesizer; "
        "$synth.Speak([string]'" + cleanText + "')\"";
#endif
    // 执行语音命令，捕获错误
    int ret = std::system(command.c_str());
    if (ret != 0) {
        std::cout << "[提示] 语音播报失败：未检测到系统语音引擎！\n";
    }
}

std::string pathToVoiceText(const std::vector<std::string>& path, int totalDistance) {
    std::ostringstream oss;
    oss << "已为您规划路径，总距离" << totalDistance << "米。";
    oss << "路径为：";
    for (size_t i = 0; i < path.size(); ++i) {
        oss << path[i];
        if (i != path.size() - 1) {
            oss << "，然后前往";
        }
    }
    oss << "。到达终点。";
    return oss.str();
}
void broadcastUserTip(int userType) {
    std::string tipText;
    switch (userType) {
    case 0: // STUDENT
        tipText = "学生提示：建议提前10分钟出发，避开上课高峰期。";
        break;
    case 1: // TEACHER
        tipText = "教职工提示：行政楼附近有停车场。";
        break;
    case 2: // VISITOR
        tipText = "访客提示：如需帮助可前往行政楼咨询处。";
        break;
    default:
        return;
    }
    voiceBroadcast(tipText);
}