#pragma once

#include <iostream>
#include <ostream>
#include <fstream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <functional>
#include <stdexcept>
#include <algorithm>
#include <execution>
#include <random>
#include <array>
#include <optional>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Rendering/Core/Common.h"