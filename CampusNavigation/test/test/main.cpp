#include "Graph.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iomanip>
#include <limits>
#include <algorithm>
// 原有头文件保留，新增以下
#include "VoiceBroadcast.h"
#include <algorithm> // 用于std::reverse
// 用户类型枚举
enum UserType { STUDENT, TEACHER, VISITOR };

// 用户信息结构体
struct UserInfo {
    std::string id;           // 学号/工号
    std::string name;         // 姓名
    UserType type;            // 用户类型
    std::string password;     // 密码
    std::string major;        // 专业（学生）
    std::string department;   // 部门（教职工）
    std::vector<std::string> savedRoutes;  // 保存的路线
};

// 函数声明（放在最前面）
bool checkFirstTime();
void loadUserData();
void saveUserData();
void createDefaultUsers();
void initializeCampusMap();
void displayWelcome();
void firstTimeSetup();
void registerUser();
bool loginUser();
void displayUserInfo();
void pathPlanning();
void viewSavedRoutes();
void displayMainMenu();
void mainControlLoop();

// 全局变量
Graph campusMap;
std::map<std::string, UserInfo> userDatabase;  // 用户数据库
UserInfo currentUser;                          // 当前登录用户
bool isLoggedIn = false;                       // 登录状态
bool isFirstTime = false;                      // 是否为首次使用
const std::string USER_DATA_FILE = "user_data.dat";  // 用户数据文件

// 检查是否为首次使用
bool checkFirstTime() {
    // 检查数据文件是否存在
    std::ifstream checkFile(USER_DATA_FILE);
    if (!checkFile.good()) {
        return true;  // 文件不存在，第一次使用
    }
    checkFile.close();

    // 检查文件中是否有数据
    std::ifstream file(USER_DATA_FILE, std::ios::binary);
    if (!file.is_open()) return true;

    size_t userCount;
    file.read(reinterpret_cast<char*>(&userCount), sizeof(userCount));

    if (userCount == 0) {
        file.close();
        return true;
    }

    file.close();
    return false;
}

// 加载用户数据
void loadUserData() {
    std::ifstream file(USER_DATA_FILE, std::ios::binary);
    if (!file.is_open()) {
        // 如果没有数据文件，创建默认用户
        createDefaultUsers();
        return;
    }

    size_t userCount;
    file.read(reinterpret_cast<char*>(&userCount), sizeof(userCount));

    for (size_t i = 0; i < userCount; i++) {
        UserInfo user;

        // 读取ID
        size_t idSize;
        file.read(reinterpret_cast<char*>(&idSize), sizeof(idSize));
        user.id.resize(idSize);
        file.read(&user.id[0], idSize);

        // 读取姓名
        size_t nameSize;
        file.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
        user.name.resize(nameSize);
        file.read(&user.name[0], nameSize);

        // 读取用户类型
        int typeInt;
        file.read(reinterpret_cast<char*>(&typeInt), sizeof(typeInt));
        user.type = static_cast<UserType>(typeInt);

        // 读取密码
        size_t passwordSize;
        file.read(reinterpret_cast<char*>(&passwordSize), sizeof(passwordSize));
        user.password.resize(passwordSize);
        file.read(&user.password[0], passwordSize);

        // 读取专业
        size_t majorSize;
        file.read(reinterpret_cast<char*>(&majorSize), sizeof(majorSize));
        user.major.resize(majorSize);
        file.read(&user.major[0], majorSize);

        // 读取部门
        size_t departmentSize;
        file.read(reinterpret_cast<char*>(&departmentSize), sizeof(departmentSize));
        user.department.resize(departmentSize);
        file.read(&user.department[0], departmentSize);

        // 读取保存的路线
        size_t routeCount;
        file.read(reinterpret_cast<char*>(&routeCount), sizeof(routeCount));
        user.savedRoutes.resize(routeCount);
        for (size_t j = 0; j < routeCount; j++) {
            size_t routeSize;
            file.read(reinterpret_cast<char*>(&routeSize), sizeof(routeSize));
            user.savedRoutes[j].resize(routeSize);
            file.read(&user.savedRoutes[j][0], routeSize);
        }

        userDatabase[user.id] = user;
    }

    file.close();
}

// 保存用户数据
void saveUserData() {
    std::ofstream file(USER_DATA_FILE, std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        std::cout << "警告：无法保存用户数据！\n";
        return;
    }

    size_t userCount = userDatabase.size();
    file.write(reinterpret_cast<const char*>(&userCount), sizeof(userCount));

    for (const auto& pair : userDatabase) {
        const UserInfo& user = pair.second;

        // 保存ID
        size_t idSize = user.id.size();
        file.write(reinterpret_cast<const char*>(&idSize), sizeof(idSize));
        file.write(user.id.c_str(), idSize);

        // 保存姓名
        size_t nameSize = user.name.size();
        file.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        file.write(user.name.c_str(), nameSize);

        // 保存用户类型
        int typeInt = static_cast<int>(user.type);
        file.write(reinterpret_cast<const char*>(&typeInt), sizeof(typeInt));

        // 保存密码
        size_t passwordSize = user.password.size();
        file.write(reinterpret_cast<const char*>(&passwordSize), sizeof(passwordSize));
        file.write(user.password.c_str(), passwordSize);

        // 保存专业
        size_t majorSize = user.major.size();
        file.write(reinterpret_cast<const char*>(&majorSize), sizeof(majorSize));
        file.write(user.major.c_str(), majorSize);

        // 保存部门
        size_t departmentSize = user.department.size();
        file.write(reinterpret_cast<const char*>(&departmentSize), sizeof(departmentSize));
        file.write(user.department.c_str(), departmentSize);

        // 保存路线
        size_t routeCount = user.savedRoutes.size();
        file.write(reinterpret_cast<const char*>(&routeCount), sizeof(routeCount));
        for (const auto& route : user.savedRoutes) {
            size_t routeSize = route.size();
            file.write(reinterpret_cast<const char*>(&routeSize), sizeof(routeSize));
            file.write(route.c_str(), routeSize);
        }
    }

    file.close();
}

// 创建默认用户（用于第一次使用）
void createDefaultUsers() {
    // 学生用户
    userDatabase["202301001"] = { "202301001", "张三", STUDENT, "123456", "计算机科学与技术", "", {} };
    userDatabase["202301002"] = { "202301002", "李四", STUDENT, "654321", "软件工程", "", {} };

    // 教职工用户
    userDatabase["T1001"] = { "T1001", "赵老师", TEACHER, "teacher123", "", "信息科学与工程学院", {} };

    // 访客账号（无密码，方便直接使用）
    userDatabase["guest"] = { "guest", "访客", VISITOR, "", "", "", {} };

    saveUserData();  // 保存默认用户数据
}

// 初始化校园地图
void initializeCampusMap() {
    // 添加校园地点
    campusMap.addVertex("宿舍楼");
    campusMap.addVertex("图书馆");
    campusMap.addVertex("食堂");
    campusMap.addVertex("教室");
    campusMap.addVertex("里仁学院");
    campusMap.addVertex("信息馆");
    campusMap.addVertex("超市");
    campusMap.addVertex("浴池");
    campusMap.addVertex("行政楼");
    campusMap.addVertex("停车场");
    campusMap.addVertex("体育馆");
    campusMap.addVertex("医务室");
    campusMap.addVertex("教学楼A");
    campusMap.addVertex("教学楼B");
    campusMap.addVertex("实验室");
    campusMap.addVertex("运动场");

    // 添加道路连接（无向边，权重为距离米数）
    campusMap.addEdge("宿舍楼", "食堂", 230);
    campusMap.addEdge("宿舍楼", "图书馆", 1500);
    campusMap.addEdge("宿舍楼", "浴池", 400);
    campusMap.addEdge("食堂", "超市", 500);
    campusMap.addEdge("食堂", "教室", 400);
    campusMap.addEdge("图书馆", "信息馆", 2000);
    campusMap.addEdge("教室", "信息馆", 100);
    campusMap.addEdge("教室", "里仁学院", 600);
    campusMap.addEdge("超市", "浴池", 250);
    campusMap.addEdge("行政楼", "图书馆", 800);
    campusMap.addEdge("行政楼", "停车场", 150);
    campusMap.addEdge("停车场", "信息馆", 300);
    campusMap.addEdge("体育馆", "食堂", 700);
    campusMap.addEdge("医务室", "宿舍楼", 600);
    campusMap.addEdge("医务室", "行政楼", 450);
    campusMap.addEdge("教学楼A", "教室", 200);
    campusMap.addEdge("教学楼B", "教室", 300);
    campusMap.addEdge("实验室", "信息馆", 150);
    campusMap.addEdge("运动场", "体育馆", 100);
    campusMap.addEdge("运动场", "宿舍楼", 1200);
}

// 显示欢迎界面
void displayWelcome() {
    std::cout << "\n========================================\n";
    std::cout << "   欢迎使用燕山大学校园导航系统\n";
    std::cout << "========================================\n";
    std::cout << "  版本: 3.0 (支持用户登录与个性化)\n";
    std::cout << "  开发团队: 数据结构课程项目组\n";
    std::cout << "  日期: 2025年12月26日\n";
    std::cout << "========================================\n\n";
}

// 用户注册功能
void registerUser() {
    std::cout << "\n=== 用户注册 ===\n";

    std::string id, name, password, confirmPassword;
    int userType;

    std::cout << "请选择用户类型:\n";
    std::cout << "1. 学生\n";
    std::cout << "2. 教职工\n";
    std::cout << "3. 访客\n";
    std::cout << "请选择 (1-3): ";
    std::cin >> userType;

    if (userType < 1 || userType > 3) {
        std::cout << "无效选择！\n";
        return;
    }

    std::cin.ignore(); // 清除输入缓冲区

    if (userType != 3) { // 访客不需要ID
        std::cout << "请输入学号/工号: ";
        std::getline(std::cin, id);

        // 检查用户是否已存在
        if (userDatabase.find(id) != userDatabase.end()) {
            std::cout << "该学号/工号已被注册！\n";
            return;
        }
    }
    else {
        id = "guest_" + std::to_string(userDatabase.size() + 1);
    }

    std::cout << "请输入姓名: ";
    std::getline(std::cin, name);

    if (userType != 3) { // 访客不需要密码
        std::cout << "请设置密码: ";
        std::getline(std::cin, password);

        std::cout << "请确认密码: ";
        std::getline(std::cin, confirmPassword);

        if (password != confirmPassword) {
            std::cout << "两次输入的密码不一致！\n";
            return;
        }
    }

    // 创建新用户
    UserInfo newUser;
    newUser.id = id;
    newUser.name = name;
    if (userType != 3) newUser.password = password;

    switch (userType) {
    case 1:  // 学生
        newUser.type = STUDENT;
        std::cout << "请输入专业: ";
        std::getline(std::cin, newUser.major);
        break;
    case 2:  // 教职工
        newUser.type = TEACHER;
        std::cout << "请输入部门: ";
        std::getline(std::cin, newUser.department);
        break;
    case 3:  // 访客
        newUser.type = VISITOR;
        break;
    }

    // 保存到数据库
    userDatabase[id] = newUser;
    saveUserData();  // 保存到文件

    std::cout << "\n注册成功！欢迎 " << name;
    switch (newUser.type) {
    case STUDENT: std::cout << " 同学"; break;
    case TEACHER: std::cout << " 老师"; break;
    case VISITOR: std::cout << " 访客"; break;
    default: break;
    }
    std::cout << "\n";

    // 询问是否立即登录
    std::cout << "是否立即登录？(y/n): ";
    char choice;
    std::cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        currentUser = newUser;
        isLoggedIn = true;
    }
}

// 用户登录功能
bool loginUser() {
    std::cout << "\n=== 用户登录 ===\n";

    std::string id, password;
    std::cin.ignore(); // 清除输入缓冲区

    std::cout << "请输入学号/工号: ";
    std::getline(std::cin, id);

    // 查找用户
    auto it = userDatabase.find(id);
    if (it == userDatabase.end()) {
        std::cout << "用户不存在！\n";
        return false;
    }

    if (it->second.type != VISITOR) {  // 访客不需要密码
        std::cout << "请输入密码: ";
        std::getline(std::cin, password);

        // 验证密码
        if (it->second.password != password) {
            std::cout << "密码错误！\n";
            return false;
        }
    }

    // 登录成功
    currentUser = it->second;
    isLoggedIn = true;

    std::cout << "\n登录成功！欢迎 ";
    switch (currentUser.type) {
    case STUDENT:
        std::cout << currentUser.name << " 同学 (" << currentUser.major << ")";
        break;
    case TEACHER:
        std::cout << currentUser.name << " 老师 (" << currentUser.department << ")";
        break;
    case VISITOR:
        std::cout << currentUser.name << " (访客)";
        break;
    }
    std::cout << "\n";

    return true;
}

// 首次使用引导
void firstTimeSetup() {
    std::cout << "\n════════════════════════════════════════\n";
    std::cout << "  欢迎首次使用燕山大学校园导航系统！\n";
    std::cout << "════════════════════════════════════════\n";

    std::cout << "\n检测到您是第一次使用本系统，建议您：\n";
    std::cout << "1. 注册一个新账号（学生或教职工）\n";
    std::cout << "2. 使用访客账号直接体验\n";
    std::cout << "3. 使用默认测试账号\n";

    std::cout << "\n请选择：\n";
    std::cout << "1. 注册新账号\n";
    std::cout << "2. 使用访客账号登录\n";
    std::cout << "3. 使用默认测试账号\n";
    std::cout << "请选择 (1-3): ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(); // 清除输入缓冲区

    switch (choice) {
    case 0: // 处理语音开关
        enableVoice = !enableVoice;
        std::cout << "语音播报已" << (enableVoice ? "开启" : "关闭") << "！\n";
        break;
    case 1:
        registerUser();
        break;
    case 2:
        // 使用访客账号
        currentUser = { "guest", "访客", VISITOR, "", "", "", {} };
        isLoggedIn = true;
        std::cout << "\n已使用访客账号登录，欢迎！\n";
        std::cout << "注意：访客账号功能有限，建议注册正式账号。\n";
        break;
    case 3:
        std::cout << "\n默认测试账号：\n";
        std::cout << "1. 学生账号：学号 202301001，密码 123456\n";
        std::cout << "2. 教职工账号：工号 T1001，密码 teacher123\n";
        std::cout << "\n请选择测试账号 (1-2): ";
        int testChoice;
        std::cin >> testChoice;
        if (testChoice == 1) {
            currentUser = userDatabase["202301001"];
            isLoggedIn = true;
            std::cout << "已登录为学生：张三\n";
        }
        else if (testChoice == 2) {
            currentUser = userDatabase["T1001"];
            isLoggedIn = true;
            std::cout << "已登录为教职工：赵老师\n";
        }
        else {
            std::cout << "无效选择，将使用访客账号。\n";
            currentUser = { "guest", "访客", VISITOR, "", "", "", {} };
            isLoggedIn = true;
        }
        break;
    default:
        std::cout << "无效选择，将使用访客账号。\n";
        currentUser = { "guest", "访客", VISITOR, "", "", "", {} };
        isLoggedIn = true;
    }
}

// 显示用户信息
void displayUserInfo() {
    if (!isLoggedIn) {
        std::cout << "请先登录！\n";
        return;
    }

    std::cout << "\n=== 用户信息 ===\n";
    std::cout << "姓名: " << currentUser.name << "\n";
    std::cout << "身份: ";
    switch (currentUser.type) {
    case STUDENT: std::cout << "学生"; break;
    case TEACHER: std::cout << "教职工"; break;
    case VISITOR: std::cout << "访客"; break;
    }
    std::cout << "\n";

    if (currentUser.type == STUDENT && !currentUser.major.empty()) {
        std::cout << "专业: " << currentUser.major << "\n";
    }

    if (currentUser.type == TEACHER && !currentUser.department.empty()) {
        std::cout << "部门: " << currentUser.department << "\n";
    }

    if (currentUser.type != VISITOR) {
        std::cout << "学号/工号: " << currentUser.id << "\n";
    }

    std::cout << "保存的路线数: " << currentUser.savedRoutes.size() << "\n";
}

// 路径规划功能（支持途经点）
void pathPlanning() {
    std::cout << "\n=== 路径规划 ===\n";

    // 显示所有地点
    std::cout << "校园地点列表:\n";
    for (size_t i = 0; i < campusMap.vertices.size(); ++i) {
        std::cout << std::setw(2) << i << ". " << campusMap.getVertexName(static_cast<int>(i)) << "\n";
    }

    // 1. 选择起点
    int startIdx, endIdx;
    std::cout << "\n请选择起点编号: ";
    std::cin >> startIdx;

    // 2. 选择是否添加途经点
    std::vector<int> waypoints; // 存储途经点索引
    char addWaypoint;
    std::cout << "是否添加途经点？(y/n): ";
    std::cin >> addWaypoint;

    while (addWaypoint == 'y' || addWaypoint == 'Y') {
        int wpIdx;
        std::cout << "请选择途经点编号: ";
        std::cin >> wpIdx;

        // 校验途经点有效性
        int verticesSize = static_cast<int>(campusMap.vertices.size());
        if (wpIdx < 0 || wpIdx >= verticesSize) {
            std::cout << "无效编号，跳过该途经点！\n";
        }
        else if (wpIdx == startIdx) {
            std::cout << "途经点不能与起点相同，跳过！\n";
        }
        else {
            waypoints.push_back(wpIdx);
            std::cout << "已添加途经点: " << campusMap.getVertexName(wpIdx) << "\n";
        }

        std::cout << "是否继续添加途经点？(y/n): ";
        std::cin >> addWaypoint;
    }

    // 3. 选择终点
    std::cout << "请选择终点编号: ";
    std::cin >> endIdx;

    // 校验起点/终点有效性
    int verticesSize = static_cast<int>(campusMap.vertices.size());
    if (startIdx < 0 || startIdx >= verticesSize ||
        endIdx < 0 || endIdx >= verticesSize) {
        std::cout << "输入编号无效！\n";
        return;
    }

    // 4. 构建完整路径节点序列（起点 → 途经点1 → 途经点2 → ... → 终点）
    std::vector<int> pathNodes;
    pathNodes.push_back(startIdx);
    pathNodes.insert(pathNodes.end(), waypoints.begin(), waypoints.end());
    pathNodes.push_back(endIdx);

    // 5. 逐段计算最短路径并拼接
    std::vector<std::string> fullPath; // 存储完整路径的地点名称
    int totalDistance = 0;             // 总距离
    bool isPathValid = true;           // 路径是否有效（所有节点可达）

    for (size_t i = 0; i < pathNodes.size() - 1; ++i) {
        int current = pathNodes[i];
        int next = pathNodes[i + 1];

        std::vector<int> prev(campusMap.vertices.size(), -1);
        std::vector<int> dist = campusMap.dijkstraWithPath(current, prev);

        if (dist[next] == std::numeric_limits<int>::max()) {
            std::cout << "无法从 " << campusMap.getVertexName(current)
                << " 到达 " << campusMap.getVertexName(next) << "\n";
            isPathValid = false;
            break;
        }

        // 拼接当前段的路径（反向遍历prev，再反转）
        std::vector<std::string> segmentPath;
        int at = next;
        while (at != -1) {
            segmentPath.push_back(campusMap.getVertexName(at));
            at = prev[at];
        }
        std::reverse(segmentPath.begin(), segmentPath.end());

        // 合并到完整路径（避免重复添加起点）
        if (i == 0) {
            fullPath = segmentPath;
        }
        else {
            fullPath.insert(fullPath.end(), segmentPath.begin() + 1, segmentPath.end());
        }

        // 累加距离
        totalDistance += dist[next];
    }

    // 6. 输出结果
    if (!isPathValid) {
        std::cout << "路径规划失败：部分节点不可达！\n";
        return;
    }

    std::cout << "\n========================================\n";
    std::cout << "路径规划结果:\n";
    std::cout << "起点: " << campusMap.getVertexName(startIdx) << "\n";
    if (!waypoints.empty()) {
        std::cout << "途经点: ";
        for (size_t i = 0; i < waypoints.size(); ++i) {
            std::cout << campusMap.getVertexName(waypoints[i]);
            if (i != waypoints.size() - 1) std::cout << " → ";
        }
        std::cout << "\n";
    }
    std::cout << "终点: " << campusMap.getVertexName(endIdx) << "\n";
    std::cout << "总距离: " << totalDistance << " 米\n";
    std::cout << "推荐路径: ";
    for (size_t i = 0; i < fullPath.size(); ++i) {
        std::cout << fullPath[i];
        if (i != fullPath.size() - 1) std::cout << " → ";
    }
    std::cout << "\n";

    // 个性化提示（保留原有逻辑）
    if (isLoggedIn) {
        if (currentUser.type == STUDENT) {
            std::cout << "\n[学生提示] 建议提前10分钟出发，避开上课高峰期\n";
        }
        else if (currentUser.type == TEACHER) {
            std::cout << "\n[教职工提示] 行政楼附近有停车场\n";
        }
        else if (currentUser.type == VISITOR) {
            std::cout << "\n[访客提示] 如需帮助可前往行政楼咨询处\n";
        }
    }
    if (enableVoice) { // 仅当语音开启时询问
        char voiceChoice;
        std::cout << "是否语音播报路径？(y/n，默认y): ";
        std::cin >> voiceChoice;
        if (voiceChoice != 'n' && voiceChoice != 'N') {
            std::cout << "正在语音播报...\n";
            // 1. 播报路径信息
            std::string voiceText = pathToVoiceText(fullPath, totalDistance);
            voiceBroadcast(voiceText);
            // 2. 播报个性化提示（如果已登录）
            if (isLoggedIn) {
                broadcastUserTip(static_cast<int>(currentUser.type));
            }
        }
    }
    if (isLoggedIn) {
        if (currentUser.type == STUDENT) {
            std::cout << "\n[学生提示] 建议提前10分钟出发，避开上课高峰期\n";
        }
        else if (currentUser.type == TEACHER) {
            std::cout << "\n[教职工提示] 行政楼附近有停车场\n";
        }
        else if (currentUser.type == VISITOR) {
            std::cout << "\n[访客提示] 如需帮助可前往行政楼咨询处\n";
        }
    }
    // 保存路线（保留原有逻辑，适配多节点路径）
    if (isLoggedIn && currentUser.type != VISITOR) {
        std::cout << "\n是否保存此路线？(y/n): ";
        char saveChoice;
        std::cin >> saveChoice;
        if (saveChoice == 'y' || saveChoice == 'Y') {
            std::string routeName;
            std::cout << "请输入路线名称: ";
            std::cin.ignore();
            std::getline(std::cin, routeName);

            // 构建路线信息字符串（包含起点、途经点、终点、总距离）
            std::string routeInfo = routeName + ": ";
            routeInfo += campusMap.getVertexName(startIdx);
            for (int wp : waypoints) {
                routeInfo += " → " + campusMap.getVertexName(wp);
            }
            routeInfo += " → " + campusMap.getVertexName(endIdx) + " (" + std::to_string(totalDistance) + "米)";

            currentUser.savedRoutes.push_back(routeInfo);
            userDatabase[currentUser.id] = currentUser;
            saveUserData();
            std::cout << "路线已保存！\n";
        }
    }
}

// 查看保存的路线
void viewSavedRoutes() {
    if (!isLoggedIn) {
        std::cout << "请先登录！\n";
        return;
    }

    if (currentUser.type == VISITOR) {
        std::cout << "访客账号不支持保存路线功能！\n";
        return;
    }

    std::cout << "\n=== 保存的路线 ===\n";

    if (currentUser.savedRoutes.empty()) {
        std::cout << "暂无保存的路线。\n";
        return;
    }

    for (size_t i = 0; i < currentUser.savedRoutes.size(); ++i) {
        std::cout << i + 1 << ". " << currentUser.savedRoutes[i] << "\n";
    }
}

// 显示主菜单
void displayMainMenu() {
    std::cout << "\n========================================\n";
    std::cout << "  燕山大学校园导航系统\n";

    if (isLoggedIn) {
        std::cout << "  当前用户: " << currentUser.name;
        switch (currentUser.type) {
        case STUDENT: std::cout << " (学生)"; break;
        case TEACHER: std::cout << " (教职工)"; break;
        case VISITOR: std::cout << " (访客)"; break;
        }
    }
    else {
        std::cout << "  未登录 (访客模式)";
    }

    std::cout << "\n========================================\n";
    std::cout << "1. 路径规划与导航\n";
    std::cout << "2. 查看校园地图\n";

    if (isLoggedIn) {
        std::cout << "3. 查看用户信息\n";
        if (currentUser.type != VISITOR) {
            std::cout << "4. 查看保存的路线\n";
        }
        std::cout << "5. 退出登录\n";
        std::cout << "6. 退出系统\n";
    }
    else {
        std::cout << "3. 用户登录\n";
        std::cout << "4. 用户注册\n";
        std::cout << "5. 退出系统\n";
    }

    std::cout << "========================================\n";
    if (isLoggedIn) {
        std::cout << "请选择功能 (0-6): ";
    }
    else {
        std::cout << "请选择功能 (0-5): ";
    }
}

// 主控制循环
void mainControlLoop() {
    // 检查是否为首次使用
    isFirstTime = checkFirstTime();

    // 加载用户数据
    loadUserData();

    // 初始化校园地图
    initializeCampusMap();

    // 显示欢迎界面
    displayWelcome();

    // 如果是第一次使用，显示引导界面
    if (isFirstTime) {
        firstTimeSetup();
    }
    else {
        // 提示用户登录或继续使用访客模式
        std::cout << "\n是否立即登录？(y/n，输入n将使用访客模式): ";
        char choice;
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            loginUser();
        }
        else {
            currentUser = { "guest", "访客", VISITOR, "", "", "", {} };
            isLoggedIn = true;
            std::cout << "已使用访客账号登录。\n";
        }
    }

    // 主循环
    while (true) {
        displayMainMenu();

        int choice;
        std::cin >> choice;

        if (!isLoggedIn) {
            // 未登录状态菜单
            switch (choice) {
            case 1:
                pathPlanning();
                break;
            case 2:
                campusMap.printGraph();
                break;
            case 3:
                loginUser();
                break;
            case 4:
                registerUser();
                break;
            case 5:
                std::cout << "\n感谢使用燕山大学校园导航系统！再见！\n";
                return;
            default:
                std::cout << "无效选择，请重新输入！\n";
            }
        }
        else {
            // 已登录状态菜单
            switch (choice) {
            case 1:
                pathPlanning();
                break;
            case 2:
                campusMap.printGraph();
                break;
            case 3:
                displayUserInfo();
                break;
            case 4:
                if (currentUser.type != VISITOR) {
                    viewSavedRoutes();
                }
                else {
                    std::cout << "无效选择！\n";
                }
                break;
            case 5:
                isLoggedIn = false;
                std::cout << "已退出登录。\n";
                break;
            case 6:
                std::cout << "\n感谢使用燕山大学校园导航系统！再见，"
                    << currentUser.name << "！\n";
                return;
            default:
                std::cout << "无效选择，请重新输入！\n";
            }
        }

        std::cout << "\n按回车键继续...";
        std::cin.ignore();
        std::cin.get();
    }
}

int main() {
    mainControlLoop();
    return 0;
}