#include "../include/MacdCalculator.h"
#include <algorithm>
#include <numeric>
#include <regex>
#include <limits>

using namespace Finance;

MACDCalculator::MACDCalculator(int shortPeriod, int longPeriod, int signalPeriod) noexcept
    : m_shortPeriod(shortPeriod)
    , m_longPeriod(longPeriod)
    , m_signalPeriod(signalPeriod) {

    // 参数验证在setParameters中完成，这里只做基本检查
    if (m_shortPeriod <= 0) m_shortPeriod = 12;
    if (m_longPeriod <= 0) m_longPeriod = 26;
    if (m_signalPeriod <= 0) m_signalPeriod = 9;
}

bool MACDCalculator::loadDataFromCSV(const std::string& filename, bool hasHeader) {
    // 使用二进制模式打开文件，避免编码问题
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件: " + filename);
    }

    clearData();

    std::string line;
    size_t lineNumber = 0;

    // 读取BOM头（如果存在）- 处理UTF-8 BOM
    char bom[3];
    file.read(bom, 3);
    if (bom[0] != '\xEF' || bom[1] != '\xBB' || bom[2] != '\xBF') {
        // 不是UTF-8 BOM，回到文件开头
        file.seekg(0);
    }
    else {
        // 如果是BOM，我们已经跳过了，继续正常读取
        std::cout << "检测到UTF-8 BOM，已跳过" << std::endl;
    }

    // 读取表头（如果存在）
    if (hasHeader) {
        if (!std::getline(file, line)) {
            std::cerr << "警告: 文件为空或无法读取表头" << std::endl;
            return false;
        }
        // 可以在这里处理表头，但我们现在不需要它
        std::cout << "跳过表头: " << line << std::endl;
    }

    // 读取数据行
    while (std::getline(file, line)) {
        ++lineNumber;

        // 跳过空行
        if (line.empty() || std::all_of(line.begin(), line.end(), ::isspace)) {
            continue;
        }

        std::string processedLine = line;

        // 不再需要行内BOM检测，因为文件开头的BOM已经处理过了

        std::istringstream iss(processedLine);
        std::string dateStr, priceStr;

        // 检测分隔符
        char delimiter = detectDelimiter(processedLine);

        if (!std::getline(iss, dateStr, delimiter) || !std::getline(iss, priceStr)) {
            std::cerr << "警告: 第" << lineNumber << "行格式错误，跳过" << std::endl;
            continue;
        }

        // 清理字符串
        auto trim = [](const std::string& str) {
            size_t start = str.find_first_not_of(" \t\r\n\"");
            if (start == std::string::npos) return std::string();
            size_t end = str.find_last_not_of(" \t\r\n\"");
            return str.substr(start, end - start + 1);
        };

        dateStr = trim(dateStr);
        priceStr = trim(priceStr);

        // 跳过空字段
        if (dateStr.empty() || priceStr.empty()) {
            std::cerr << "警告: 第" << lineNumber << "行有空字段，跳过" << std::endl;
            continue;
        }

        try {
            // 解析日期 - 现在支持UTF-8编码的中文日期
            std::string normalizedDate = parseChineseDateUTF8(dateStr);

            // 解析价格
            double price = std::stod(priceStr);
            price = validatePrice(price);

            if (!std::isnan(price)) {
                m_dates.push_back(std::move(normalizedDate));
                m_prices.push_back(price);
            }
            else {
                std::cerr << "警告: 第" << lineNumber << "行价格无效: " << priceStr << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "警告: 第" << lineNumber << "行数据解析失败: " << e.what()
                << " (日期: " << dateStr << ", 价格: " << priceStr << ")" << std::endl;
        }
    }

    if (m_prices.empty()) {
        std::cerr << "错误: 未读取到有效数据" << std::endl;
        return false;
    }

    std::cout << "成功读取 " << m_prices.size() << " 条数据" << std::endl;
    std::cout << "第一行数据示例 - 日期: " << m_dates[0] << ", 价格: " << m_prices[0] << std::endl;
    if (m_prices.size() > 1) {
        std::cout << "最后一行数据示例 - 日期: " << m_dates.back() << ", 价格: " << m_prices.back() << std::endl;
    }

    return true;
}

char MACDCalculator::detectDelimiter(const std::string& line) {
    // 统计各种分隔符的出现次数
    std::map<char, int> delimiterCount = {
        {',', 0},
        {'\t', 0},
        {';', 0},
        {'|', 0}
    };

    for (char c : line) {
        if (delimiterCount.find(c) != delimiterCount.end()) {
            delimiterCount[c]++;
        }
    }

    // 返回出现次数最多的分隔符
    char bestDelimiter = ',';
    int maxCount = -1;

    for (const auto& [delim, count] : delimiterCount) {
        if (count > maxCount) {
            maxCount = count;
            bestDelimiter = delim;
        }
    }

    return bestDelimiter;
}

std::string MACDCalculator::parseChineseDateUTF8(const std::string& dateStr) {
    // 如果已经是标准格式，直接返回
    if (dateStr.find('-') != std::string::npos || dateStr.find('/') != std::string::npos) {
        return dateStr;
    }

    // 方法1: 使用UTF-8编码的正则表达式（需要确保源代码文件也是UTF-8编码）
    try {
        // UTF-8编码的中文字符正则表达式
        std::regex chinesePattern(u8"(\\d{4})年(\\d{1,2})月(\\d{1,2})日");
        std::smatch match;

        if (std::regex_match(dateStr, match, chinesePattern) && match.size() == 4) {
            std::string year = match[1].str();
            std::string month = match[2].str();
            std::string day = match[3].str();

            if (month.length() == 1) month = "0" + month;
            if (day.length() == 1) day = "0" + day;

            return year + "-" + month + "-" + day;
        }
    }
    catch (const std::exception& e) {
        // 如果正则表达式失败，使用手动解析
        std::cerr << "正则表达式解析失败，尝试手动解析: " << e.what() << std::endl;
    }

    // 方法2: 手动解析UTF-8中文日期（更可靠的方法）
    std::string result = manuallyParseChineseDateUTF8(dateStr);
    if (!result.empty()) {
        return result;
    }

    // 如果无法解析，返回原字符串
    std::cerr << "警告: 无法解析日期格式: " << dateStr << "，将使用原格式" << std::endl;
    return dateStr;
}

std::string MACDCalculator::manuallyParseChineseDateUTF8(const std::string& dateStr) {
    // 手动解析UTF-8中文日期格式
    // UTF-8编码："年" = \xE5\xB9\xB4, "月" = \xE6\x9C\x88, "日" = \xE6\x97\xA5

    std::string utf8Year = "\xE5\xB9\xB4";  // 年
    std::string utf8Month = "\xE6\x9C\x88"; // 月  
    std::string utf8Day = "\xE6\x97\xA5";   // 日

    size_t yearPos = dateStr.find(utf8Year);
    size_t monthPos = dateStr.find(utf8Month);
    size_t dayPos = dateStr.find(utf8Day);

    if (yearPos != std::string::npos && monthPos != std::string::npos && dayPos != std::string::npos &&
        yearPos < monthPos && monthPos < dayPos) {

        // 提取年份（年字前面的4位数字）
        if (yearPos >= 4) {
            std::string year = dateStr.substr(yearPos - 4, 4);

            // 提取月份（年和月之间的数字）
            if (monthPos > yearPos + 3) {
                std::string month = dateStr.substr(yearPos + 3, monthPos - (yearPos + 3));

                // 提取日期（月和日之间的数字）
                if (dayPos > monthPos + 3) {
                    std::string day = dateStr.substr(monthPos + 3, dayPos - (monthPos + 3));

                    // 验证都是数字
                    auto isDigits = [](const std::string& str) {
                        return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
                    };

                    if (isDigits(year) && isDigits(month) && isDigits(day)) {
                        // 格式化月份和日期
                        if (month.length() == 1) month = "0" + month;
                        if (day.length() == 1) day = "0" + day;

                        return year + "-" + month + "-" + day;
                    }
                }
            }
        }
    }

    return ""; // 解析失败
}

//bool MACDCalculator::loadDataFromCSV(const std::string& filename, bool hasHeader) {
//    // 使用二进制模式打开文件，避免编码问题
//    std::ifstream file(filename, std::ios::binary);
//    if (!file.is_open()) {
//        throw std::runtime_error("无法打开文件: " + filename);
//    }
//
//    clearData();
//
//    std::string line;
//    size_t lineNumber = 0;
//
//    // 读取BOM头（如果存在）
//    char bom[3];
//    file.read(bom, 3);
//    if (bom[0] != '\xEF' || bom[1] != '\xBB' || bom[2] != '\xBF') {
//        // 不是UTF-8 BOM，回到文件开头
//        file.seekg(0);
//    }
//
//    // 读取表头（如果存在）
//    if (hasHeader) {
//        if (!std::getline(file, line)) {
//            std::cerr << "警告: 文件为空或无法读取表头" << std::endl;
//            return false;
//        }
//        // 可以在这里处理表头，但我们现在不需要它
//    }
//
//    // 读取数据行
//    while (std::getline(file, line)) {
//        ++lineNumber;
//
//        // 跳过空行
//        if (line.empty() || std::all_of(line.begin(), line.end(), ::isspace)) {
//            continue;
//        }
//
//        // 处理不同编码：尝试从GB2312转换或直接处理
//        std::string processedLine = line;
//
//        // 尝试检测并移除可能的BOM
//        if (line.length() >= 3 &&
//            static_cast<unsigned char>(line[0]) == 0xEF &&
//            static_cast<unsigned char>(line[1]) == 0xBB &&
//            static_cast<unsigned char>(line[2]) == 0xBF) {
//            processedLine = line.substr(3);
//        }
//
//        std::istringstream iss(processedLine);
//        std::string dateStr, priceStr;
//
//        // 尝试多种分隔符：Tab、逗号、分号等
//        char delimiter = '\t'; // 默认使用Tab
//
//        // 检测分隔符：优先使用Tab，如果没有Tab则使用逗号
//        if (processedLine.find('\t') == std::string::npos) {
//            if (processedLine.find(',') != std::string::npos) {
//                delimiter = ',';
//            }
//            else if (processedLine.find(';') != std::string::npos) {
//                delimiter = ';';
//            }
//        }
//
//        if (!std::getline(iss, dateStr, delimiter) || !std::getline(iss, priceStr)) {
//            std::cerr << "警告: 第" << lineNumber << "行格式错误，跳过" << std::endl;
//            continue;
//        }
//
//        // 清理字符串（去除可能的引号、空格等）
//        auto trim = [](const std::string& str) {
//            size_t start = str.find_first_not_of(" \t\r\n\"");
//            if (start == std::string::npos) return std::string();
//            size_t end = str.find_last_not_of(" \t\r\n\"");
//            return str.substr(start, end - start + 1);
//        };
//
//        dateStr = trim(dateStr);
//        priceStr = trim(priceStr);
//
//        // 跳过空字段
//        if (dateStr.empty() || priceStr.empty()) {
//            std::cerr << "警告: 第" << lineNumber << "行有空字段，跳过" << std::endl;
//            continue;
//        }
//
//        try {
//            // 解析日期（处理中文日期格式）
//            std::string normalizedDate = parseChineseDate(dateStr);
//
//            // 解析价格
//            double price = std::stod(priceStr);
//            price = validatePrice(price);
//
//            if (!std::isnan(price)) {
//                m_dates.push_back(std::move(normalizedDate));
//                m_prices.push_back(price);
//            }
//            else {
//                std::cerr << "警告: 第" << lineNumber << "行价格无效: " << priceStr << std::endl;
//            }
//        }
//        catch (const std::exception& e) {
//            std::cerr << "警告: 第" << lineNumber << "行数据解析失败: " << e.what()
//                << " (日期: " << dateStr << ", 价格: " << priceStr << ")" << std::endl;
//        }
//    }
//
//    if (m_prices.empty()) {
//        std::cerr << "错误: 未读取到有效数据" << std::endl;
//        return false;
//    }
//
//    std::cout << "成功读取 " << m_prices.size() << " 条数据" << std::endl;
//    std::cout << "第一行数据示例 - 日期: " << m_dates[0] << ", 价格: " << m_prices[0] << std::endl;
//    if (m_prices.size() > 1) {
//        std::cout << "最后一行数据示例 - 日期: " << m_dates.back() << ", 价格: " << m_prices.back() << std::endl;
//    }
//
//    return true;
//}

void MACDCalculator::loadDataFromVector(const std::vector<double>& prices,
    const std::vector<std::string>& dates) {
    clearData();

    if (prices.empty()) {
        throw std::invalid_argument("价格数据不能为空");
    }

    m_prices = prices;

    // 处理日期数据
    if (!dates.empty()) {
        if (dates.size() != prices.size()) {
            throw std::invalid_argument("日期和价格数据数量不匹配");
        }
        m_dates = dates;
    }
    else {
        // 生成默认日期序列
        m_dates.reserve(prices.size());
        for (size_t i = 0; i < prices.size(); ++i) {
            m_dates.push_back("第" + std::to_string(i + 1) + "日");
        }
    }
}

MACDResult MACDCalculator::calculate() const {
    MACDResult result;

    if (m_prices.empty() || m_prices.size() < static_cast<size_t>(m_longPeriod + m_signalPeriod)) {
        std::cerr << "错误: 数据不足，需要至少 "
            << (m_longPeriod + m_signalPeriod)
            << " 个数据点，当前只有 " << m_prices.size() << std::endl;
        return result;
    }

    if (m_shortPeriod >= m_longPeriod) {
        std::cerr << "错误: 短期周期必须小于长期周期" << std::endl;
        return result;
    }

    // 初始化结果向量
    const size_t dataSize = m_prices.size();
    result.dates = m_dates;
    result.prices = m_prices;
    result.EMA12.resize(dataSize, std::numeric_limits<double>::quiet_NaN());
    result.EMA26.resize(dataSize, std::numeric_limits<double>::quiet_NaN());
    result.DIF.resize(dataSize, std::numeric_limits<double>::quiet_NaN());
    result.DEA.resize(dataSize, std::numeric_limits<double>::quiet_NaN());
    result.Histogram.resize(dataSize, std::numeric_limits<double>::quiet_NaN());
    result.HistogramColors.resize(dataSize, MACDResult::ColorIntensity::ZERO); // 初始化颜色强度

    try {
        // 1. 计算短期EMA（12日）
        std::cout << "计算EMA12..." << std::endl;
        result.EMA12 = calculateEMA(m_prices, m_shortPeriod);

        // 2. 计算长期EMA（26日）
        std::cout << "计算EMA26..." << std::endl;
        result.EMA26 = calculateEMA(m_prices, m_longPeriod);

        // 调试输出：显示EMA计算情况
        int ema12Count = std::count_if(result.EMA12.begin(), result.EMA12.end(),
            [](double x) { return !std::isnan(x); });
        int ema26Count = std::count_if(result.EMA26.begin(), result.EMA26.end(),
            [](double x) { return !std::isnan(x); });

        std::cout << "EMA12有效值数量: " << ema12Count << "/" << result.EMA12.size() << std::endl;
        std::cout << "EMA26有效值数量: " << ema26Count << "/" << result.EMA26.size() << std::endl;

        // 3. 计算DIF线（差离值）= 短期EMA - 长期EMA
        std::cout << "计算DIF线..." << std::endl;
        const int difStartIndex = m_longPeriod - 1; // 从有完整长期EMA的位置开始

        for (int i = difStartIndex; i < static_cast<int>(dataSize); i++) {
            if (!std::isnan(result.EMA12[i]) && !std::isnan(result.EMA26[i])) {
                result.DIF[i] = result.EMA12[i] - result.EMA26[i];
            }
        }

        int difCount = std::count_if(result.DIF.begin(), result.DIF.end(),
            [](double x) { return !std::isnan(x); });
        std::cout << "DIF有效值数量: " << difCount << "/" << result.DIF.size() << std::endl;

        if (difCount == 0) {
            std::cerr << "错误: 无法计算DIF值，请检查EMA12和EMA26数据" << std::endl;
            return result;
        }

        // 4. 计算DEA线（信号线）= DIF的EMA（9日）
        std::cout << "计算DEA线..." << std::endl;

        // 使用改进的EMA计算函数计算DEA
        std::vector<double> deaValues = calculateEMA(result.DIF, m_signalPeriod);

        // 将DEA值复制到结果中
        for (size_t i = 0; i < dataSize; i++) {
            result.DEA[i] = deaValues[i];
        }

        int deaCount = std::count_if(result.DEA.begin(), result.DEA.end(),
            [](double x) { return !std::isnan(x); });
        std::cout << "DEA有效值数量: " << deaCount << "/" << result.DEA.size() << std::endl;

        if (deaCount == 0) {
            std::cerr << "错误: 无法计算DEA值，DIF序列可能有问题" << std::endl;

            // 调试信息：显示DIF序列的前40个值
            std::cout << "DIF序列前40个值: ";
            for (int i = 0; i < 40 && i < static_cast<int>(result.DIF.size()); i++) {
                if (!std::isnan(result.DIF[i])) {
                    std::cout << result.DIF[i] << " ";
                }
                else {
                    std::cout << "NaN ";
                }
            }
            std::cout << std::endl;
        }

        // 5. 计算柱状图 = DIF - DEA
        std::cout << "计算MACD柱状图..." << std::endl;
        const int histogramStartIndex = difStartIndex + m_signalPeriod - 1;

        for (int i = histogramStartIndex; i < static_cast<int>(dataSize); i++) {
            if (!std::isnan(result.DIF[i]) && !std::isnan(result.DEA[i])) {
                result.Histogram[i] = result.DIF[i] - result.DEA[i];
            }
        }

        int histogramCount = std::count_if(result.Histogram.begin(), result.Histogram.end(),
            [](double x) { return !std::isnan(x); });
        std::cout << "MACD柱状图有效值数量: " << histogramCount << "/" << result.Histogram.size() << std::endl;

        // 6. 分析柱状图颜色强度
        std::cout << "分析MACD柱状图颜色强度..." << std::endl;
        analyzeHistogramColorIntensity(result);

        // 输出计算进度和关键指标
        std::cout << "\n=== MACD计算完成 ===" << std::endl;
        std::cout << "数据总量: " << dataSize << " 个" << std::endl;
        std::cout << "DIF开始计算位置: 第" << (difStartIndex + 1) << "个数据点" << std::endl;
        std::cout << "DEA开始计算位置: 第" << (histogramStartIndex + 1) << "个数据点" << std::endl;

        // 显示最后几个有效值用于验证
        int lastValidIndex = -1;
        for (int i = static_cast<int>(dataSize) - 1; i >= 0; i--) {
            if (!std::isnan(result.Histogram[i])) {
                lastValidIndex = i;
                break;
            }
        }

        if (lastValidIndex >= 0) {
            std::cout << "\n最新MACD值:" << std::endl;
            std::cout << "日期: " << result.dates[lastValidIndex] << std::endl;
            std::cout << "收盘价: " << result.prices[lastValidIndex] << std::endl;
            std::cout << "EMA12: " << result.EMA12[lastValidIndex] << std::endl;
            std::cout << "EMA26: " << result.EMA26[lastValidIndex] << std::endl;
            std::cout << "DIF: " << result.DIF[lastValidIndex] << std::endl;
            std::cout << "DEA: " << result.DEA[lastValidIndex] << std::endl;
            std::cout << "MACD柱: " << result.Histogram[lastValidIndex] << std::endl;

            // 显示颜色强度
            if (lastValidIndex < static_cast<int>(result.HistogramColors.size())) {
                std::cout << "颜色强度: " << colorIntensityToString(result.HistogramColors[lastValidIndex]) << std::endl;
            }
        }
        else {
            std::cout << "警告: 未找到有效的MACD柱状图值" << std::endl;
        }

    }
    catch (const std::exception& e) {
        std::cerr << "计算MACD时发生错误: " << e.what() << std::endl;
        return result;
    }

    return result;
}

MACDResult MACDCalculator::getLatestValues() const noexcept {
    MACDResult result;

    if (!hasSufficientData()) {
        return result;
    }

    try {
        auto fullResult = calculate();
        const int lastIndex = static_cast<int>(m_prices.size()) - 1;

        // 只返回最后一天的数据
        if (lastIndex >= 0) {
            result.dates.push_back(fullResult.dates[lastIndex]);
            result.prices.push_back(fullResult.prices[lastIndex]);
            result.EMA12.push_back(fullResult.EMA12[lastIndex]);
            result.EMA26.push_back(fullResult.EMA26[lastIndex]);
            result.DIF.push_back(fullResult.DIF[lastIndex]);
            result.DEA.push_back(fullResult.DEA[lastIndex]);
            result.Histogram.push_back(fullResult.Histogram[lastIndex]);
        }
    }
    catch (...) {
        // 忽略所有异常，返回空结果
    }

    return result;
}

void MACDCalculator::printResults() const {
    if (!hasSufficientData()) {
        std::cout << "数据不足，无法计算MACD" << std::endl;
        return;
    }

    try {
        auto result = calculate();

        std::cout << std::fixed << std::setprecision(4);
        std::cout << "\n" << std::string(80, '=') << std::endl;
        std::cout << "MACD完整计算结果（包含颜色强度分析）" << std::endl;
        std::cout << std::string(80, '=') << std::endl;
        std::cout << "日期\t\t收盘价\t\tEMA12\t\tEMA26\t\tDIF\t\tDEA\t\tMACD柱\t\t颜色强度" << std::endl;
        std::cout << std::string(80, '-') << std::endl;

        for (size_t i = 0; i < result.prices.size(); ++i) {
            std::cout << result.dates[i] << "\t";
            std::cout << result.prices[i] << "\t\t";

            if (!std::isnan(result.EMA12[i])) {
                std::cout << result.EMA12[i] << "\t\t";
            }
            else {
                std::cout << "-\t\t";
            }

            if (!std::isnan(result.EMA26[i])) {
                std::cout << result.EMA26[i] << "\t\t";
            }
            else {
                std::cout << "-\t\t";
            }

            if (!std::isnan(result.DIF[i])) {
                std::cout << result.DIF[i] << "\t\t";
            }
            else {
                std::cout << "-\t\t";
            }

            if (!std::isnan(result.DEA[i])) {
                std::cout << result.DEA[i] << "\t\t";
            }
            else {
                std::cout << "-\t\t";
            }

            if (!std::isnan(result.Histogram[i])) {
                std::cout << result.Histogram[i] << "\t\t";

                // 显示颜色强度
                if (i < result.HistogramColors.size()) {
                    std::cout << colorIntensityToString(result.HistogramColors[i]);
                }
                else {
                    std::cout << "未知";
                }
            }
            else {
                std::cout << "-\t\t-";
            }
            std::cout << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "计算失败: " << e.what() << std::endl;
    }
}

void MACDCalculator::printSummary() const {
    if (!hasSufficientData()) {
        std::cout << "数据不足，无法计算MACD" << std::endl;
        return;
    }

    try {
        auto result = calculate();
        const int displayCount = std::min(10, static_cast<int>(result.prices.size()));
        const int startIndex = static_cast<int>(result.prices.size()) - displayCount;

        std::cout << std::fixed << std::setprecision(4);
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "MACD最近" << displayCount << "个交易日摘要（包含颜色强度）" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        std::cout << "日期\t\t收盘价\tDIF\tDEA\tMACD柱\t颜色强度" << std::endl;
        std::cout << std::string(60, '-') << std::endl;

        for (int i = startIndex; i < static_cast<int>(result.prices.size()); ++i) {
            std::cout << result.dates[i] << "\t";
            std::cout << result.prices[i] << "\t";

            if (!std::isnan(result.DIF[i])) {
                std::cout << result.DIF[i] << "\t";
            }
            else {
                std::cout << "-\t";
            }

            if (!std::isnan(result.DEA[i])) {
                std::cout << result.DEA[i] << "\极t";
            }
            else {
                std::cout << "-\t";
            }

            if (!std::isnan(result.Histogram[i])) {
                std::cout << result.Histogram[i] << "\t";

                // 显示颜色强度
                if (i < result.HistogramColors.size()) {
                    std::cout << colorIntensityToString(result.HistogramColors[i]);
                }
                else {
                    std::cout << "未知";
                }
            }
            else {
                std::cout << "-\t-";
            }
            std::cout << std::endl;
        }

        // 打印最新值
        const int lastIndex = static_cast<int>(result.prices.size()) - 1;
        if (!std::isnan(result.Histogram[lastIndex])) {
            std::cout << std::string(60, '-') << std::endl;
            std::cout << "最新MACD值: DIF=" << result.DIF[lastIndex]
                << ", DEA=" << result.DEA[lastIndex]
                << ", MACD柱=" << result.Histogram[lastIndex]
                << ", 颜色强度=" << colorIntensityToString(result.HistogramColors[lastIndex]) << std::endl;

            // 在printSummary函数的动能分析部分添加颜色切换的情况
            auto intensity = result.HistogramColors[lastIndex];
            auto prevIntensity = (lastIndex > 0) ? result.HistogramColors[lastIndex - 1] : intensity;

            std::cout << "动能分析: ";
            switch (intensity) {
            case MACDResult::ColorIntensity::DARK_GREEN:
                if (prevIntensity == MACDResult::ColorIntensity::DARK_RED ||
                    prevIntensity == MACDResult::ColorIntensity::LIGHT_RED ||
                    prevIntensity == MACDResult::ColorIntensity::ZERO) {
                    std::cout << "多空转换，上涨动能开始增强";
                }
                else {
                    std::cout << "上涨动能增强，多头力量强劲";
                }
                break;
            case MACDResult::ColorIntensity::LIGHT_GREEN:
                std::cout << "上涨动能减弱，多头力量衰减";
                break;
            case MACDResult::ColorIntensity::DARK_RED:
                if (prevIntensity == MACDResult::ColorIntensity::DARK_GREEN ||
                    prevIntensity == MACDResult::ColorIntensity::LIGHT_GREEN ||
                    prevIntensity == MACDResult::ColorIntensity::ZERO) {
                    std::cout << "多空转换，下跌动能开始增强";
                }
                else {
                    std::cout << "下跌动能增强，空头力量强劲";
                }
                break;
            case MACDResult::ColorIntensity::LIGHT_RED:
                std::cout << "下跌动能减弱，空头力量衰减";
                break;
            case MACDResult::ColorIntensity::ZERO:
                std::cout << "多空力量平衡，市场观望";
                break;
            default:
                break;
            }
        }

    }
    catch (const std::exception& e) {
        std::cerr << "计算失败: " << e.what() << std::endl;
    }
}

void MACDCalculator::setParameters(int shortP, int longP, int signalP) {
    if (shortP <= 0 || longP <= 0 || signalP <= 0) {
        throw std::invalid_argument("MACD参数必须为正整数");
    }
    if (shortP >= longP) {
        throw std::invalid_argument("短期周期必须小于长期周期");
    }

    m_shortPeriod = shortP;
    m_longPeriod = longP;
    m_signalPeriod = signalP;
}

void MACDCalculator::clearData() noexcept {
    m_dates.clear();
    m_prices.clear();
}

std::vector<double> MACDCalculator::calculateEMA(const std::vector<double>& data, int period) const {
    if (data.empty() || period <= 0) {
        return std::vector<double>(data.size(), std::numeric_limits<double>::quiet_NaN());
    }

    std::vector<double> ema(data.size(), std::numeric_limits<double>::quiet_NaN());
    const double multiplier = 2.0 / (period + 1.0);

    // 寻找第一个有效数据点
    int firstValidIndex = -1;
    for (int i = 0; i < static_cast<int>(data.size()); i++) {
        if (!std::isnan(data[i])) {
            firstValidIndex = i;
            break;
        }
    }

    // 如果没有有效数据点，返回全NaN
    if (firstValidIndex == -1) {
        std::cerr << "警告: 计算EMA" << period << "时，未找到任何有效数据点" << std::endl;
        return ema;
    }

    // 检查是否有足够的数据点来计算EMA
    if (static_cast<int>(data.size()) - firstValidIndex < period) {
        std::cerr << "警告: 计算EMA" << period << "时，有效数据点不足" << period << "个" << std::endl;
        return ema;
    }

    // 计算第一个EMA值（使用第一个有效值作为初始值）
    ema[firstValidIndex] = data[firstValidIndex];

    // 递归计算后续EMA值
    for (int i = firstValidIndex + 1; i < static_cast<int>(data.size()); i++) {
        if (std::isnan(data[i])) {
            // 如果当前数据点为NaN，EMA也为NaN
            ema[i] = std::numeric_limits<double>::quiet_NaN();
        }
        else if (std::isnan(ema[i - 1])) {
            // 如果前一个EMA为NaN，重新初始化EMA
            ema[i] = data[i];
        }
        else {
            // 正常计算EMA: EMA = (价格 - 前一日EMA) × 乘数 + 前一日EMA
            ema[i] = (data[i] - ema[i - 1]) * multiplier + ema[i - 1];
        }
    }

    return ema;
}

std::string MACDCalculator::parseChineseDate(const std::string& dateStr) {
    // 简单的日期格式处理
    // 处理格式如："2025年9月1日", "2025-09-01", "2025/09/01"等

    // 如果已经是标准格式，直接返回
    if (dateStr.find('-') != std::string::npos || dateStr.find('/') != std::string::npos) {
        return dateStr;
    }

    // 处理中文日期格式："2025年9月1日"
    std::regex chinesePattern(R"((\d{4})年(\d{1,2})月(\d{1,2})日)");
    std::smatch match;

    if (std::regex_match(dateStr, match, chinesePattern) && match.size() == 4) {
        std::string year = match[1].str();
        std::string month = match[2].str();
        std::string day = match[3].str();

        // 格式化月份和日期为两位数
        if (month.length() == 1) month = "0" + month;
        if (day.length() == 1) day = "0" + day;

        return year + "-" + month + "-" + day;
    }

    // 如果无法解析，返回原字符串
    std::cerr << "警告: 无法解析日期格式: " << dateStr << "，将使用原格式" << std::endl;
    return dateStr;
}

//void MACDCalculator::analyzeHistogramColorIntensity(MACDResult& result) const {
//    if (result.Histogram.empty()) {
//        return;
//    }
//
//    // 寻找第一个有效MACD柱的索引
//    int firstValidIndex = -1;
//    for (int i = 0; i < static_cast<int>(result.Histogram.size()); i++) {
//        if (!std::isnan(result.Histogram[i])) {
//            firstValidIndex = i;
//            break;
//        }
//    }
//
//    if (firstValidIndex == -1) {
//        return; // 没有有效数据
//    }
//
//    // 第一个有效柱子：根据数值大小判断初始颜色强度
//    if (!std::isnan(result.Histogram[firstValidIndex])) {
//        if (result.Histogram[firstValidIndex] > 0.001) {
//            result.HistogramColors[firstValidIndex] = MACDResult::ColorIntensity::DARK_GREEN;
//        }
//        else if (result.Histogram[firstValidIndex] < -0.001) {
//            result.HistogramColors[firstValidIndex] = MACDResult::ColorIntensity::DARK_RED;
//        }
//        else {
//            result.HistogramColors[firstValidIndex] = MACDResult::ColorIntensity::ZERO;
//        }
//    }
//
//    // 分析后续柱子的颜色强度
//    for (int i = firstValidIndex + 1; i < static_cast<int>(result.Histogram.size()); i++) {
//        if (std::isnan(result.Histogram[i]) || std::isnan(result.Histogram[i - 1])) {
//            // 当前或前一个值为NaN，无法比较
//            if (!std::isnan(result.Histogram[i])) {
//                // 当前值有效但前一个无效，作为新的开始
//                if (result.Histogram[i] > 0.001) {
//                    result.HistogramColors[i] = MACDResult::ColorIntensity::DARK_GREEN;
//                }
//                else if (result.Histogram[i] < -0.001) {
//                    result.HistogramColors[i] = MACDResult::ColorIntensity::DARK_RED;
//                }
//                else {
//                    result.HistogramColors[i] = MACDResult::ColorIntensity::ZERO;
//                }
//            }
//            continue;
//        }
//
//        // 获取当前和前一个柱子的值
//        double currentValue = result.Histogram[i];
//        double prevValue = result.Histogram[i - 1];
//
//        // 判断颜色切换情况
//        bool colorChanged = false;
//        bool switchedToRed = false;
//        bool switchedToGreen = false;
//
//        // 使用颜色切换检测函数
//        colorChanged = detectColorChange(prevValue, currentValue, switchedToRed, switchedToGreen);
//
//        //// 检查是否发生了颜色切换
//        //if ((prevValue <= 0 && currentValue > 0.001) ||
//        //    (prevValue >= 0 && currentValue < -0.001)) {
//        //    colorChanged = true;
//        //    switchedToRed = (currentValue > 0.001);
//        //    switchedToGreen = (currentValue < -0.001);
//        //}
//
//        // 计算当前柱子和前一个柱子的绝对值（长度）
//        double currentAbs = std::abs(currentValue);
//        double prevAbs = std::abs(prevValue);
//
//        // 判断颜色强度
//        if (currentValue > 0.001) {
//            // 绿柱
//            if (colorChanged && switchedToRed) {
//                // 颜色切换到绿色，视为动能增强，深绿色
//                result.HistogramColors[i] = MACDResult::ColorIntensity::DARK_GREEN;
//            }
//            else if (currentAbs > prevAbs) {
//                // 柱子变长，动能增强，深绿色
//                result.HistogramColors[i] = MACDResult::ColorIntensity::DARK_GREEN;
//            }
//            else {
//                // 柱子变短，动能减弱，浅绿色
//                result.HistogramColors[i] = MACDResult::ColorIntensity::LIGHT_GREEN;
//            }
//        }
//        else if (currentValue < -0.001) {
//            // 红柱
//            if (colorChanged && switchedToGreen) {
//                // 颜色切换到红色，视为动能增强，深红色
//                result.HistogramColors[i] = MACDResult::ColorIntensity::DARK_RED;
//            }
//            else if (currentAbs > prevAbs) {
//                // 柱子变长，动能增强，深红色
//                result.HistogramColors[i] = MACDResult::ColorIntensity::DARK_RED;
//            }
//            else {
//                // 柱子变短，动能减弱，浅红色
//                result.HistogramColors[i] = MACDResult::ColorIntensity::LIGHT_RED;
//            }
//        }
//        else {
//            // 接近零轴
//            result.HistogramColors[i] = MACDResult::ColorIntensity::ZERO;
//        }
//    }
//}

void MACDCalculator::analyzeHistogramColorIntensity(MACDResult& result) const {
    if (result.Histogram.empty()) {
        return;
    }

    // 寻找第一个有效MACD柱的索引
    int firstValidIndex = -1;
    for (int i = 0; i < static_cast<int>(result.Histogram.size()); i++) {
        if (!std::isnan(result.Histogram[i])) {
            firstValidIndex = i;
            break;
        }
    }

    if (firstValidIndex == -1) {
        return; // 没有有效数据
    }

    // 第一个有效柱子：视为动能开始，使用深色
    if (!std::isnan(result.Histogram[firstValidIndex])) {
        if (result.Histogram[firstValidIndex] > 0.001) {
            result.HistogramColors[firstValidIndex] = MACDResult::ColorIntensity::DARK_GREEN;
        }
        else if (result.Histogram[firstValidIndex] < -0.001) {
            result.HistogramColors[firstValidIndex] = MACDResult::ColorIntensity::DARK_RED;
        }
        else {
            result.HistogramColors[firstValidIndex] = MACDResult::ColorIntensity::ZERO;
        }
    }

    // 分析后续柱子的颜色强度
    for (int i = firstValidIndex + 1; i < static_cast<int>(result.Histogram.size()); i++) {
        if (std::isnan(result.Histogram[i]) || std::isnan(result.Histogram[i - 1])) {
            // 当前或前一个值为NaN，无法比较
            if (!std::isnan(result.Histogram[i])) {
                // 当前值有效但前一个无效，作为新的开始，使用深色
                if (result.Histogram[i] > 0.001) {
                    result.HistogramColors[i] = MACDResult::ColorIntensity::DARK_GREEN;
                }
                else if (result.Histogram[i] < -0.001) {
                    result.HistogramColors[i] = MACDResult::ColorIntensity::DARK_RED;
                }
                else {
                    result.HistogramColors[i] = MACDResult::ColorIntensity::ZERO;
                }
            }
            continue;
        }

        // 获取当前和前一个柱子的值
        double currentValue = result.Histogram[i];
        double prevValue = result.Histogram[i - 1];

        // 检测颜色切换
        bool colorChanged = false;
        bool switchedToRed = false;
        bool switchedToGreen = false;

        // 定义零轴阈值
        const double zeroThreshold = 0.001;

        // 检查颜色切换
        if ((prevValue < -zeroThreshold && currentValue > zeroThreshold) ||  // 红→绿
            (prevValue > zeroThreshold && currentValue < -zeroThreshold) ||  // 绿→红
            (std::abs(prevValue) <= zeroThreshold && currentValue > zeroThreshold) ||  // 零→绿
            (std::abs(prevValue) <= zeroThreshold && currentValue < -zeroThreshold)) {   // 零→红
            colorChanged = true;
            switchedToRed = (currentValue > zeroThreshold);
            switchedToGreen = (currentValue < -zeroThreshold);
        }

        // 计算当前柱子和前一个柱子的绝对值（长度）
        double currentAbs = std::abs(currentValue);
        double prevAbs = std::abs(prevValue);

        // 判断颜色强度
        if (currentValue > zeroThreshold) {
            // 绿柱
            if (colorChanged && switchedToRed) {
                // 颜色切换到绿色，视为动能增强，深绿色
                result.HistogramColors[i] = MACDResult::ColorIntensity::DARK_GREEN;
            }
            else if (currentAbs > prevAbs) {
                // 柱子变长，动能增强，深绿色
                result.HistogramColors[i] = MACDResult::ColorIntensity::DARK_GREEN;
            }
            else {
                // 柱子变短，动能减弱，浅绿色
                result.HistogramColors[i] = MACDResult::ColorIntensity::LIGHT_GREEN;
            }
        }
        else if (currentValue < -zeroThreshold) {
            // 红柱
            if (colorChanged && switchedToGreen) {
                // 颜色切换到红色，视为动能增强，深红色
                result.HistogramColors[i] = MACDResult::ColorIntensity::DARK_RED;
            }
            else if (currentAbs > prevAbs) {
                // 柱子变长，动能增强，深红色
                result.HistogramColors[i] = MACDResult::ColorIntensity::DARK_RED;
            }
            else {
                // 柱子变短，动能减弱，浅红色
                result.HistogramColors[i] = MACDResult::ColorIntensity::LIGHT_RED;
            }
        }
        else {
            // 接近零轴
            result.HistogramColors[i] = MACDResult::ColorIntensity::ZERO;
        }
    }
}

std::string MACDCalculator::colorIntensityToString(MACDResult::ColorIntensity intensity) {
    switch (intensity) {
    case MACDResult::ColorIntensity::DARK_RED:
        return "深红柱";
    case MACDResult::ColorIntensity::LIGHT_RED:
        return "浅红柱";
    case MACDResult::ColorIntensity::DARK_GREEN:
        return "深绿柱";
    case MACDResult::ColorIntensity::LIGHT_GREEN:
        return "浅绿柱";
    case MACDResult::ColorIntensity::ZERO:
        return "零轴";
    default:
        return "未知";
    }
}


bool MACDCalculator::detectColorChange(double prevValue, double currentValue,
    bool& switchedToRed, bool& switchedToGreen) const {
    switchedToRed = false;
    switchedToGreen = false;

    // 定义零轴附近的阈值
    const double zeroThreshold = 0.001;

    // 检查是否从绿色切换到红色
    if (prevValue < -zeroThreshold && currentValue > zeroThreshold) {
        switchedToRed = true;
        return true;
    }

    // 检查是否从红色切换到绿色
    if (prevValue > zeroThreshold && currentValue < -zeroThreshold) {
        switchedToGreen = true;
        return true;
    }

    // 检查是否从零轴附近切换到红色
    if (std::abs(prevValue) <= zeroThreshold && currentValue > zeroThreshold) {
        switchedToRed = true;
        return true;
    }

    // 检查是否从零轴附近切换到绿色
    if (std::abs(prevValue) <= zeroThreshold && currentValue < -zeroThreshold) {
        switchedToGreen = true;
        return true;
    }

    return false;
}