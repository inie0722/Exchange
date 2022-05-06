#pragma once

#include "struct.hpp"

#include <map>
#include <list>
#include <vector>
#include <utility>

#include <queue>

#include <iostream>

namespace exchange
{
    class Core
    {
    private:
        using OrderMap_t = std::map<double, std::list<std::uint64_t>>;

        std::vector<std::pair<Order, std::list<std::uint64_t>::iterator>> orders_;

        OrderMap_t buy_map_;
        OrderMap_t sell_map_;

        std::queue<Trader> trader_queue_;
        std::queue<OrderTrader> orders_trader_queue_;

        void add_buy_order(const Order &order)
        {
            orders_.push_back({order, buy_map_[order.price].end()});
            auto order_it = orders_.back().first;

            //查看卖队列
            for (auto it = sell_map_.begin(); it != sell_map_.end() && it->first <= order.price; ++it)
            {
                for (auto &&i : it->second)
                {
                    auto trader_quantity = orders_[i].first.quantity - orders_[i].first.trader_quantity;
                    trader_quantity = order_it.quantity - order_it.trader_quantity < trader_quantity ? order_it.quantity - order_it.trader_quantity : trader_quantity;

                    orders_[i].first.trader_quantity += trader_quantity;
                    order_it.trader_quantity += trader_quantity;

                    trader_queue_.push({false, order_it.price, trader_quantity});

                    //订单全部成交
                    if (orders_[i].first.quantity - orders_[i].first.trader_quantity == 0)
                    {
                        it->second.erase(orders_[i].second);
                        orders_trader_queue_.push({orders_[i].first.price, orders_[i].first.trader_quantity, OrderStatusType::ALL_TRADED});
                    }

                    if (order_it.quantity - order_it.trader_quantity == 0)
                    {
                        orders_trader_queue_.push({order_it.price, order_it.trader_quantity, OrderStatusType::ALL_TRADED});
                        return;
                    }
                }
            }

                        buy_map_[order.price].push_back(orders_.size() - 1);
            orders_.back().second = --buy_map_[order.price].end();
        }

        void add_sell_order(const Order &order)
        {
            orders_.push_back({order, sell_map_[order.price].end()});
            auto order_it = orders_.back().first;

            //查看买队列
            for (auto it = buy_map_.rbegin(); it != buy_map_.rend() && it->first >= order.price; ++it)
            {
                for (auto i : it->second)
                {
                    auto trader_quantity = orders_[i].first.quantity - orders_[i].first.trader_quantity;
                    trader_quantity = order_it.quantity - order_it.trader_quantity < trader_quantity ? order_it.quantity - order_it.trader_quantity : trader_quantity;

                    orders_[i].first.trader_quantity += trader_quantity;
                    order_it.trader_quantity += trader_quantity;

                    trader_queue_.push({false, order_it.price, trader_quantity});
                    //订单全部成交
                    if (orders_[i].first.quantity - orders_[i].first.trader_quantity == 0)
                    {
                        it->second.erase(orders_[i].second);
                        orders_trader_queue_.push({orders_[i].first.price, orders_[i].first.trader_quantity, OrderStatusType::ALL_TRADED});
                    }

                    if (order_it.quantity - order_it.trader_quantity == 0)
                    {
                        orders_trader_queue_.push({order_it.price, order_it.trader_quantity, OrderStatusType::ALL_TRADED});
                        return;
                    }
                }
            }

            sell_map_[order.price].push_back(orders_.size() - 1);
            orders_.back().second = --sell_map_[order.price].end();
        }

    public:
        ~Core()
        {
            auto s = trader_queue_.size();
            for (size_t i = 0; i < s; i++)
            {
                auto c = trader_queue_.front();

                std::cout << __LINE__ << " " << c.price << " " << c.quantity << std::endl;

                trader_queue_.pop();
            }
            
        }

        void add_order(const Order & order)
        {
            if(order.operate_type == OperateType::BUY)
            {
                add_buy_order(order);
            }
            else
            {
                add_sell_order(order);
            }
        }
    };
} // namespace exchange
