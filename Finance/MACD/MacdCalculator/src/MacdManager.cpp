#include "MacdManager.h"

using namespace Finance;

MacdManager::MacdManager()
{
}

MacdManager::~MacdManager()
{
}

std::unordered_map<MacdManager::Period, std::string> MacdManager::dataMap_ = {
    {Period::Day, "日线"},
    {Period::Week, "周线"},
};

std::unordered_map<MacdManager::Stock, std::string> MacdManager::stockMap_ = {
    {Stock::SSE_600018, "上港集团"},
};

void MacdManager::start()
{
    std::cout << "MACD技术指标分析系统" << std::endl << std::endl;
    

    try {
        // 演示主要功能
        std::cout << "========== " + stockMap_.at(Stock::SSE_600018) + "-日线数据分析 ==========" << std::endl;
        demonstrateMACDCalculator(Stock::SSE_600018, Period::Day);

        std::cout << "========== " + stockMap_.at(Stock::SSE_600018) + "-周线数据分析 ==========" << std::endl;
        demonstrateMACDCalculator(Stock::SSE_600018, Period::Week);

        std::cout << "\nMACD分析结束." << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "系统错误: " << e.what() << std::endl;
        return;
    }

    std::cout << "\nMACD计算完成！" << std::endl;

    // 在Windows下暂停，以便查看输出结果
#ifdef _WIN32
    std::cout << "\n按Enter键退出..." << std::endl;
    std::cin.ignore();
    std::cin.get();
#endif
}

/**
 * @brief 演示MACD计算器的使用
 */
void MacdManager::demonstrateMACDCalculator(Stock stock, Period period) {
    std::cout << "=== 专业级MACD计算器演示 ===" << std::endl;

    try {
        // 创建MACD计算器实例
        MACDCalculator calculator(12, 26, 9);

        std::cout << "1. 尝试从CSV文件读取数据..." << std::endl;

        // 尝试从CSV文件读取数据
        bool fileLoaded = false;
        try {
            std::string file = "../../../data/" + stockMap_.at(stock) + "_" + dataMap_.at(period) + ".csv";
            if (calculator.loadDataFromCSV(file)) {
                std::cout << "✓ 成功从" + file +"文件读取数据" << std::endl;
                fileLoaded = true;
            }
        }
        catch (const std::exception& e) {
            std::cout << "✗ 文件读取失败: " << e.what() << std::endl;
            std::cout << "将使用内置示例数据" << std::endl;
        }

        // 如果没有成功加载文件，使用示例数据
        if (!fileLoaded) {
            std::cout << "2. 使用内置示例数据..." << std::endl;
            std::vector<double> examplePrices = {
                5.59, 5.61, 5.56, 5.56, 5.53, 5.59, 5.57, 5.56, 5.59, 5.58,
                5.59, 5.61, 5.67, 5.56, 5.56, 5.53, 5.56, 5.54, 5.51, 5.48,
                5.47, 5.45, 5.53, 5.55, 5.52, 5.58, 5.57, 5.59, 5.57, 5.55,
                5.57, 5.60, 5.67, 5.62, 5.67, 5.67, 5.65, 5.67, 5.58, 5.60,
                5.59, 5.60, 5.61, 5.62, 5.72, 5.69, 5.70, 5.67, 5.69, 5.64, 5.59
            };

            // 生成对应的日期
            std::vector<std::string> exampleDates;
            for (size_t i = 0; i < examplePrices.size(); ++i) {
                exampleDates.push_back("示例日期-" + std::to_string(i + 1));
            }

            calculator.loadDataFromVector(examplePrices, exampleDates);
        }

        std::cout << "数据加载完成，共 " << calculator.getDataCount() << " 个数据点" << std::endl;

        // 检查数据是否足够
        if (!calculator.hasSufficientData()) {
            std::cerr << "错误: 数据不足，需要至少 "
                << (26 + 9) << " 个数据点，当前只有 "
                << calculator.getDataCount() << " 个" << std::endl;
            return;
        }

        std::cout << "3. 计算MACD指标..." << std::endl;

        // 显示摘要信息
        calculator.printSummary();

        // 询问用户是否显示完整结果
        //std::cout << "\n4. 是否显示完整计算结果？(y/n): ";
        //char choice;
        //std::cin >> choice;

        std::cout << "4. 完整计算结果" << std::endl;
        calculator.printResults();

        //if (choice == 'y' || choice == 'Y') {
        //    calculator.printResults();
        //}

        // 获取并显示最新的MACD值
        std::cout << "\n5. 最新MACD值和交易信号分析..." << std::endl;
        auto latest = calculator.getLatestValues();
        if (!latest.empty()) {
            std::cout << "=========================================" << std::endl;
            std::cout << "最新交易数据:" << std::endl;
            std::cout << "-----------------------------------------" << std::endl;
            std::cout << "日期: " << latest.dates[0] << std::endl;
            std::cout << "收盘价: " << latest.prices[0] << std::endl;
            std::cout << "DIF线: " << latest.DIF[0] << std::endl;
            std::cout << "DEA线: " << latest.DEA[0] << std::endl;
            std::cout << "MACD柱: " << latest.Histogram[0] << std::endl;
            std::cout << "-----------------------------------------" << std::endl;

            // 交易信号分析
            if (latest.Histogram[0] > 0.001) {
                std::cout << "交易信号: 看多信号 (绿柱)" << std::endl;
                std::cout << "说明: DIF线上穿DEA线，上涨动能较强" << std::endl;
            }
            else if (latest.Histogram[0] < -0.001) {
                std::cout << "交易信号: 看空信号 (红柱)" << std::endl;
                std::cout << "说明: DIF线下穿DEA线，下跌动能较强" << std::endl;
            }
            else {
                std::cout << "交易信号: 观望信号 (接近零轴)" << std::endl;
                std::cout << "说明: 多空力量平衡，建议观望" << std::endl;
            }
            std::cout << "=========================================" << std::endl;
        }

        std::cout << std::endl << std::endl << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "程序运行出错: " << e.what() << std::endl;
    }
}
