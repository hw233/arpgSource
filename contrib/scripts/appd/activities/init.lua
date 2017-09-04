ACT_DAILY_GIFT = 1	-- 每日礼包
ACT_LOTTERY = 2		-- 抽奖
ACT_RANK	= 3		-- 排行
ACT_LIMIT_GIFT = 4	-- 期间限定礼包

ActivityBase = require("appd.activities.ActivityBase")

require("appd.activities.ActDailyGift")
require("appd.activities.ActLottery")
require("appd.activities.ActRank")
require("appd.activities.ActLimitGift")

-- 这个一定要放到最后
require("appd.activities.ActivityManager")


