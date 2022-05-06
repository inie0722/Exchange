#pragma once

#include <cstdint>

namespace exchange
{
    enum class OrderStatusType
    {
        /// 空状态
        NONE = 0,
        /// 等待成交
        WAIT_TRADED,
        /// 全部成交
        ALL_TRADED,
        /// 已撤单
        CANCELED,
        /// 已拒绝
        REJECTED,
    };

    enum class OperateType
    {
        BUY = 0,
        SELL
    };

    enum class OrderType
    {
        LIMIT = 0,
    };

    enum class TargetType
    {
        STOCK = 0,
    };

    struct Order
    {
        std::uint64_t id;
        OperateType operate_type;
        OrderType order_type;
        TargetType target_type;
        double price;
        std::int64_t quantity;
        std::int64_t trader_quantity;
        OrderStatusType status;
    };

    struct OrderTrader
    {
        double price;
        std::int64_t trader_quantity;
        OrderStatusType status;
    };

    struct Trader
    {
        bool is_cancel;
        double price;
        std::int64_t quantity;
    };
} // namespace exchange