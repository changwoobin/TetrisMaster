#pragma once

struct StageData {
	int speed;
	int stickRate;
	int clearLine;
};

namespace Stage {
	constexpr StageData data[10] = {
		{40, 20, 1},
		{38, 18, 1},
		{35, 18, 20},
		{30, 17, 20},
		{25, 16, 20},
		{20, 14, 20},
		{15, 14, 20},
		{10, 13, 20},
		{6, 12, 20},
		{4, 11, 9999}
	};
}