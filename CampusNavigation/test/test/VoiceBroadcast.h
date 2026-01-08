#pragma once
#ifndef VOICEBROADCAST_H
#define VOICEBROADCAST_H

#include <string>
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <vector>

// 全局语音开关（默认开启）
extern bool enableVoice;

/**
 * @brief 语音播报核心函数（跨平台）
 * @param text 要播报的文本
 */
void voiceBroadcast(const std::string& text);

/**
 * @brief 将路径转换为语音可读的文本
 * @param path 完整路径的地点列表
 * @param totalDistance 总距离（米）
 * @return 语音播报文本
 */
std::string pathToVoiceText(const std::vector<std::string>& path, int totalDistance);

/**
 * @brief 播报个性化提示（学生/教职工/访客）
 * @param userType 用户类型（STUDENT/TEACHER/VISITOR）
 */
void broadcastUserTip(int userType);

#endif // VOICEBROADCAST_H