#ifndef PCH_H
#define PCH_H
#include <random>
#include <vector>
#include <array>
#include <set>
#include <unordered_map>
#include <map>
#include <bitset>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/hash.hpp>

#include"GLFW/glfw3.h"
#include "vulkan/vulkan.h"

//#define VMA_VULKAN_VERSION 1002000 // Vulkan 1.2
#define VMA_VULKAN_VERSION 1001000 // Vulkan 1.1
//#define VMA_VULKAN_VERSION 1000000 // Vulkan 1.0
#include "Graphics/RenderBackEnd/vk_mem_alloc.h"

#include <cstdint>
#include <bitset>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <optional>

#include "Core/EventSystem/EventSystem.h"
#endif
#define MAX_POPULATION 1024
//MAX_FRAMES_IN_FLIGHT = 2;
