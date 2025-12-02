#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <stdexcept>
#include <limits>  // 添加limits头文件
#include <codecvt>
#include <locale>
#include <map>

namespace Finance {

    /**
     * @brief MACD计算结果结构体
     */
    struct MACDResult {
        std::vector<std::string> dates;      // 日期序列
        std::vector<double> prices;          // 价格序列
        std::vector<double> EMA12;           // 12日EMA
        std::vector<double> EMA26;           // 26日EMA
        std::vector<double> DIF;             // DIF线
        std::vector<double> DEA;             // DEA线（信号线）
        std::vector<double> Histogram;       // MACD柱状图

        // 新增：柱状图颜色强度信息
        enum class ColorIntensity {
            DARK_RED,    // 深红柱 - 动能增强
            LIGHT_RED,   // 浅红柱 - 动能减弱  
            DARK_GREEN,  // 深绿柱 - 动能增强
            LIGHT_GREEN, // 浅绿柱 - 动能减弱
            ZERO         // 零轴
        };
        std::vector<ColorIntensity> HistogramColors; // 每个柱子的颜色强度

        // 清空所有数据
        void clear() {
            dates.clear();
            prices.clear();
            EMA12.clear();
            EMA26.clear();
            DIF.clear();
            DEA.clear();
            Histogram.clear();
            HistogramColors.clear();
        }

        // 检查是否有有效数据
        [[nodiscard]] bool empty() const noexcept {
            return prices.empty() || DIF.empty();
        }
    };

    /**
     * @brief 专业级MACD计算器类
     *
     * 符合现代C++规范，支持从Excel/CSV文件读取数据
     * 实现专业的MACD指标计算
     */
    class MACDCalculator {
    public:
        /**
         * @brief 构造函数，可设置MACD参数
         * @param shortPeriod 短期EMA周期，默认12
         * @param longPeriod 长期EMA周期，默认26
         * @param signalPeriod 信号线周期，默认9
         */
        explicit MACDCalculator(int shortPeriod = 12, int longPeriod = 26, int signalPeriod = 9) noexcept;

        virtual ~MACDCalculator() = default;

        // 禁止拷贝和移动以保持数据安全
        MACDCalculator(const MACDCalculator&) = delete;
        MACDCalculator& operator=(const MACDCalculator&) = delete;
        MACDCalculator(MACDCalculator&&) = delete;
        MACDCalculator& operator=(MACDCalculator&&) = delete;

        // 读取csv.
        std::string manuallyParseChineseDateUTF8(const std::string& dateStr);
        std::string parseChineseDateUTF8(const std::string& dateStr);
        char detectDelimiter(const std::string& line);

        /**
         * @brief 从CSV文件读取价格数据
         * @param filename 文件名，支持相对或绝对路径
         * @param hasHeader 是否包含表头，默认true
         * @return 读取成功返回true，失败返回false
         * @throws std::runtime_error 当文件无法打开或格式错误时
         */
        bool loadDataFromCSV(const std::string& filename, bool hasHeader = true);

        /**
         * @brief 从向量直接加载数据
         * @param prices 价格数据向量
         * @param dates 对应的日期向量（可选）
         */
        void loadDataFromVector(const std::vector<double>& prices,
            const std::vector<std::string>& dates = {});

        /**
         * @brief 计算MACD指标
         * @return 包含完整MACD计算结果的结构体
         * @throws std::invalid_argument 当数据不足时
         */
        [[nodiscard]] MACDResult calculate() const;

        /**
         * @brief 获取最近的有效MACD值
         * @return 包含最新MACD值的结构体，如果无数据则返回空值
         */
        [[nodiscard]] MACDResult getLatestValues() const noexcept;

        /**
         * @brief 打印完整的MACD计算结果
         */
        void printResults() const;

        /**
         * @brief 打印简化的MACD摘要（最后10个交易日）
         */
        void printSummary() const;

        /**
         * @brief 设置MACD计算参数
         * @param shortP 短期周期
         * @param longP 长期周期
         * @param signalP 信号周期
         * @throws std::invalid_argument 当参数无效时
         */
        void setParameters(int shortP, int longP, int signalP);

        /**
         * @brief 获取当前数据状态
         * @return 数据点数量
         */
        [[nodiscard]] size_t getDataCount() const noexcept { return m_prices.size(); }

        /**
         * @brief 检查是否有足够数据计算MACD
         * @return 是否可计算
         */
        [[nodiscard]] bool hasSufficientData() const noexcept {
            return m_prices.size() >= static_cast<size_t>(m_longPeriod + m_signalPeriod);
        }

        /**
         * @brief 清空所有数据
         */
        void clearData() noexcept;

    private:
        std::vector<std::string> m_dates;
        std::vector<double> m_prices;
        int m_shortPeriod;
        int m_longPeriod;
        int m_signalPeriod;

        /**
         * @brief 专业级EMA计算
         * @param data 输入数据序列
         * @param period EMA周期
         * @return EMA计算结果向量
         */
        //[[nodiscard]] static std::vector<double> calculateEMA(const std::vector<double>& data, int period);
        std::vector<double> calculateEMA(const std::vector<double>& data, int period) const;

        /**
         * @brief 解析日期字符串（处理中文日期格式）
         * @param dateStr 日期字符串，格式如"2025年11月18日"
         * @return 标准化后的日期字符串
         */
        [[nodiscard]] static std::string parseChineseDate(const std::string& dateStr);

        /**
         * @brief 验证和清理价格数据
         * @param price 原始价格
         * @return 清理后的有效价格，无效返回NaN
         */
        [[nodiscard]] static double validatePrice(double price) noexcept {
            if (price <= 0 || price > 10000 || std::isinf(price)) {
                return std::numeric_limits<double>::quiet_NaN();
            }
            return price;
        }

        /**
         * @brief 分析MACD柱状图的颜色强度
         * @param result MACD计算结果
         */
        void analyzeHistogramColorIntensity(MACDResult& result) const;

        /**
         * @brief 将颜色强度枚举转换为字符串描述
         * @param intensity 颜色强度
         * @return 字符串描述
         */
        [[nodiscard]] static std::string colorIntensityToString(MACDResult::ColorIntensity intensity);

        /**
        * @brief 检测MACD柱状图颜色切换
        * @param prevValue 前一个柱子的值
        * @param currentValue 当前柱子的值
        * @param[out] switchedToRed 是否切换到红色
        * @param[out] switchedToGreen 是否切换到绿色
        * @return 是否发生颜色切换
        */
        bool detectColorChange(double prevValue, double currentValue,
            bool& switchedToRed, bool& switchedToGreen) const;
    };

} // namespace Finance