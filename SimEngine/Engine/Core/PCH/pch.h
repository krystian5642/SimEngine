#pragma once

#define SUPPORT_OPENGL 1
#define ENABLE_TESSELLATION 0

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

#include <glm/glm.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>

#if SUPPORT_OPENGL
#include <GL/glew.h>
#endif

#include "Rendering/Core/Common.h"