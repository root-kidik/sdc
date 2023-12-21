#pragma once

#include <boost/functional/hash.hpp>
#include <chrono>
#include <iterator>
#include <string>

#include "userver/formats/json/value.hpp"
#include "userver/storages/postgres/io/chrono.hpp"
#include "userver/utils/strong_typedef.hpp"
namespace uservice_dynconf::models
{

using ConfigId = userver::utils::StrongTypedef<class ConfigIdTag, int64_t, userver::utils::StrongTypedefOps::kCompareTransparent>;

struct Key
{
    std::string service;
    std::string config_name;

    bool operator==(const Key& other) const
    {
        return std::tie(service, config_name) == std::tie(other.service, other.config_name);
    }
};

struct Config
{
    Key                                      key;
    userver::formats::json::Value            config_value;
    userver::storages::postgres::TimePointTz updated_at;
};
} // namespace uservice_dynconf::models

namespace std
{

template <>
struct hash<uservice_dynconf::models::Key>
{
    size_t operator()(const uservice_dynconf::models::Key& param) const
    {
        size_t seed = 0;
        boost::hash_combine(seed, param.service);
        boost::hash_combine(seed, param.config_name);
        return seed;
    }
};
} // namespace std
