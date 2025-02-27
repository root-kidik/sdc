#pragma once

namespace uservice_dynconf::sql
{

inline constexpr std::string_view kSelectSettingsForCache = R"~(
SELECT (service, config_name), config_value, updated_at
FROM uservice_dynconf.configs
)~";

inline constexpr std::string_view kInsertConfigValue = R"~(
INSERT INTO uservice_dynconf.configs
(service, config_name, config_value)
SELECT $1, d.key, d.value::jsonb
FROM jsonb_each_text($2) as d
ON CONFLICT (service, config_name)
DO UPDATE SET
config_value = EXCLUDED.config_value,
updated_at = NOW();
)~";

inline constexpr std::string_view kDeleteConfigValues = R"~(
DELETE FROM uservice_dynconf.configs
WHERE service = $1 and config_name IN (SELECT unnest($2));
)~";

} // namespace uservice_dynconf::sql
