#pragma once

// Experimental GLM definition
#define GLM_ENABLE_EXPERIMENTAL

// C/C++ default libraries
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <filesystem>
#include <stdio.h>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <fstream>
#include <array>

// SpdLog
#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/basic_file_sink.h>


// GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/fast_square_root.hpp>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif