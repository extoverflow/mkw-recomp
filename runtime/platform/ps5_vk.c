// Written by the port's generator script from RADV's vulkan_core.h
// (VK_HEADER_VERSION 354); do not edit. PS5 build only: see that script for what this is.
#ifdef __PROSPERO__
#include <vulkan/vulkan.h>

PFN_vkVoidFunction vk_icdGetInstanceProcAddr(VkInstance instance, const char *name);
static VkInstance ps5_vk_instance;   // the runtime's instance, once it has one

static PFN_vkVoidFunction ps5_vk_proc(const char *name) {
    return vk_icdGetInstanceProcAddr(ps5_vk_instance, name);
}

static PFN_vkCreateInstance p_vkCreateInstance;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance) {
    if (!p_vkCreateInstance) p_vkCreateInstance = (PFN_vkCreateInstance)ps5_vk_proc("vkCreateInstance");
    VkResult r = p_vkCreateInstance(pCreateInfo, pAllocator, pInstance);
    if (r == VK_SUCCESS) ps5_vk_instance = *pInstance;
    return r;
}
static PFN_vkDestroyInstance p_vkDestroyInstance;
VKAPI_ATTR void VKAPI_CALL vkDestroyInstance(VkInstance instance, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyInstance) p_vkDestroyInstance = (PFN_vkDestroyInstance)ps5_vk_proc("vkDestroyInstance");
    p_vkDestroyInstance(instance, pAllocator);
}
static PFN_vkEnumeratePhysicalDevices p_vkEnumeratePhysicalDevices;
VKAPI_ATTR VkResult VKAPI_CALL vkEnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices) {
    if (!p_vkEnumeratePhysicalDevices) p_vkEnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices)ps5_vk_proc("vkEnumeratePhysicalDevices");
    return p_vkEnumeratePhysicalDevices(instance, pPhysicalDeviceCount, pPhysicalDevices);
}
static PFN_vkGetPhysicalDeviceFeatures p_vkGetPhysicalDeviceFeatures;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures) {
    if (!p_vkGetPhysicalDeviceFeatures) p_vkGetPhysicalDeviceFeatures = (PFN_vkGetPhysicalDeviceFeatures)ps5_vk_proc("vkGetPhysicalDeviceFeatures");
    p_vkGetPhysicalDeviceFeatures(physicalDevice, pFeatures);
}
static PFN_vkGetPhysicalDeviceFormatProperties p_vkGetPhysicalDeviceFormatProperties;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties) {
    if (!p_vkGetPhysicalDeviceFormatProperties) p_vkGetPhysicalDeviceFormatProperties = (PFN_vkGetPhysicalDeviceFormatProperties)ps5_vk_proc("vkGetPhysicalDeviceFormatProperties");
    p_vkGetPhysicalDeviceFormatProperties(physicalDevice, format, pFormatProperties);
}
static PFN_vkGetPhysicalDeviceImageFormatProperties p_vkGetPhysicalDeviceImageFormatProperties;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties) {
    if (!p_vkGetPhysicalDeviceImageFormatProperties) p_vkGetPhysicalDeviceImageFormatProperties = (PFN_vkGetPhysicalDeviceImageFormatProperties)ps5_vk_proc("vkGetPhysicalDeviceImageFormatProperties");
    return p_vkGetPhysicalDeviceImageFormatProperties(physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
}
static PFN_vkGetPhysicalDeviceProperties p_vkGetPhysicalDeviceProperties;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties) {
    if (!p_vkGetPhysicalDeviceProperties) p_vkGetPhysicalDeviceProperties = (PFN_vkGetPhysicalDeviceProperties)ps5_vk_proc("vkGetPhysicalDeviceProperties");
    p_vkGetPhysicalDeviceProperties(physicalDevice, pProperties);
}
static PFN_vkGetPhysicalDeviceQueueFamilyProperties p_vkGetPhysicalDeviceQueueFamilyProperties;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties) {
    if (!p_vkGetPhysicalDeviceQueueFamilyProperties) p_vkGetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)ps5_vk_proc("vkGetPhysicalDeviceQueueFamilyProperties");
    p_vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}
static PFN_vkGetPhysicalDeviceMemoryProperties p_vkGetPhysicalDeviceMemoryProperties;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties) {
    if (!p_vkGetPhysicalDeviceMemoryProperties) p_vkGetPhysicalDeviceMemoryProperties = (PFN_vkGetPhysicalDeviceMemoryProperties)ps5_vk_proc("vkGetPhysicalDeviceMemoryProperties");
    p_vkGetPhysicalDeviceMemoryProperties(physicalDevice, pMemoryProperties);
}
static PFN_vkGetInstanceProcAddr p_vkGetInstanceProcAddr;
VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance instance, const char* pName) {
    if (!p_vkGetInstanceProcAddr) p_vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)ps5_vk_proc("vkGetInstanceProcAddr");
    return p_vkGetInstanceProcAddr(instance, pName);
}
static PFN_vkGetDeviceProcAddr p_vkGetDeviceProcAddr;
VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(VkDevice device, const char* pName) {
    if (!p_vkGetDeviceProcAddr) p_vkGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)ps5_vk_proc("vkGetDeviceProcAddr");
    return p_vkGetDeviceProcAddr(device, pName);
}
static PFN_vkCreateDevice p_vkCreateDevice;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice) {
    if (!p_vkCreateDevice) p_vkCreateDevice = (PFN_vkCreateDevice)ps5_vk_proc("vkCreateDevice");
    return p_vkCreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice);
}
static PFN_vkDestroyDevice p_vkDestroyDevice;
VKAPI_ATTR void VKAPI_CALL vkDestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyDevice) p_vkDestroyDevice = (PFN_vkDestroyDevice)ps5_vk_proc("vkDestroyDevice");
    p_vkDestroyDevice(device, pAllocator);
}
static PFN_vkEnumerateInstanceExtensionProperties p_vkEnumerateInstanceExtensionProperties;
VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) {
    if (!p_vkEnumerateInstanceExtensionProperties) p_vkEnumerateInstanceExtensionProperties = (PFN_vkEnumerateInstanceExtensionProperties)ps5_vk_proc("vkEnumerateInstanceExtensionProperties");
    return p_vkEnumerateInstanceExtensionProperties(pLayerName, pPropertyCount, pProperties);
}
static PFN_vkEnumerateDeviceExtensionProperties p_vkEnumerateDeviceExtensionProperties;
VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) {
    if (!p_vkEnumerateDeviceExtensionProperties) p_vkEnumerateDeviceExtensionProperties = (PFN_vkEnumerateDeviceExtensionProperties)ps5_vk_proc("vkEnumerateDeviceExtensionProperties");
    return p_vkEnumerateDeviceExtensionProperties(physicalDevice, pLayerName, pPropertyCount, pProperties);
}
static PFN_vkEnumerateInstanceLayerProperties p_vkEnumerateInstanceLayerProperties;
VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties) {
    if (!p_vkEnumerateInstanceLayerProperties) p_vkEnumerateInstanceLayerProperties = (PFN_vkEnumerateInstanceLayerProperties)ps5_vk_proc("vkEnumerateInstanceLayerProperties");
    return p_vkEnumerateInstanceLayerProperties(pPropertyCount, pProperties);
}
static PFN_vkEnumerateDeviceLayerProperties p_vkEnumerateDeviceLayerProperties;
VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties) {
    if (!p_vkEnumerateDeviceLayerProperties) p_vkEnumerateDeviceLayerProperties = (PFN_vkEnumerateDeviceLayerProperties)ps5_vk_proc("vkEnumerateDeviceLayerProperties");
    return p_vkEnumerateDeviceLayerProperties(physicalDevice, pPropertyCount, pProperties);
}
static PFN_vkGetDeviceQueue p_vkGetDeviceQueue;
VKAPI_ATTR void VKAPI_CALL vkGetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) {
    if (!p_vkGetDeviceQueue) p_vkGetDeviceQueue = (PFN_vkGetDeviceQueue)ps5_vk_proc("vkGetDeviceQueue");
    p_vkGetDeviceQueue(device, queueFamilyIndex, queueIndex, pQueue);
}
static PFN_vkQueueSubmit p_vkQueueSubmit;
VKAPI_ATTR VkResult VKAPI_CALL vkQueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) {
    if (!p_vkQueueSubmit) p_vkQueueSubmit = (PFN_vkQueueSubmit)ps5_vk_proc("vkQueueSubmit");
    return p_vkQueueSubmit(queue, submitCount, pSubmits, fence);
}
static PFN_vkQueueWaitIdle p_vkQueueWaitIdle;
VKAPI_ATTR VkResult VKAPI_CALL vkQueueWaitIdle(VkQueue queue) {
    if (!p_vkQueueWaitIdle) p_vkQueueWaitIdle = (PFN_vkQueueWaitIdle)ps5_vk_proc("vkQueueWaitIdle");
    return p_vkQueueWaitIdle(queue);
}
static PFN_vkDeviceWaitIdle p_vkDeviceWaitIdle;
VKAPI_ATTR VkResult VKAPI_CALL vkDeviceWaitIdle(VkDevice device) {
    if (!p_vkDeviceWaitIdle) p_vkDeviceWaitIdle = (PFN_vkDeviceWaitIdle)ps5_vk_proc("vkDeviceWaitIdle");
    return p_vkDeviceWaitIdle(device);
}
static PFN_vkAllocateMemory p_vkAllocateMemory;
VKAPI_ATTR VkResult VKAPI_CALL vkAllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) {
    if (!p_vkAllocateMemory) p_vkAllocateMemory = (PFN_vkAllocateMemory)ps5_vk_proc("vkAllocateMemory");
    return p_vkAllocateMemory(device, pAllocateInfo, pAllocator, pMemory);
}
static PFN_vkFreeMemory p_vkFreeMemory;
VKAPI_ATTR void VKAPI_CALL vkFreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkFreeMemory) p_vkFreeMemory = (PFN_vkFreeMemory)ps5_vk_proc("vkFreeMemory");
    p_vkFreeMemory(device, memory, pAllocator);
}
static PFN_vkMapMemory p_vkMapMemory;
VKAPI_ATTR VkResult VKAPI_CALL vkMapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) {
    if (!p_vkMapMemory) p_vkMapMemory = (PFN_vkMapMemory)ps5_vk_proc("vkMapMemory");
    return p_vkMapMemory(device, memory, offset, size, flags, ppData);
}
static PFN_vkUnmapMemory p_vkUnmapMemory;
VKAPI_ATTR void VKAPI_CALL vkUnmapMemory(VkDevice device, VkDeviceMemory memory) {
    if (!p_vkUnmapMemory) p_vkUnmapMemory = (PFN_vkUnmapMemory)ps5_vk_proc("vkUnmapMemory");
    p_vkUnmapMemory(device, memory);
}
static PFN_vkFlushMappedMemoryRanges p_vkFlushMappedMemoryRanges;
VKAPI_ATTR VkResult VKAPI_CALL vkFlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) {
    if (!p_vkFlushMappedMemoryRanges) p_vkFlushMappedMemoryRanges = (PFN_vkFlushMappedMemoryRanges)ps5_vk_proc("vkFlushMappedMemoryRanges");
    return p_vkFlushMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
}
static PFN_vkInvalidateMappedMemoryRanges p_vkInvalidateMappedMemoryRanges;
VKAPI_ATTR VkResult VKAPI_CALL vkInvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) {
    if (!p_vkInvalidateMappedMemoryRanges) p_vkInvalidateMappedMemoryRanges = (PFN_vkInvalidateMappedMemoryRanges)ps5_vk_proc("vkInvalidateMappedMemoryRanges");
    return p_vkInvalidateMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
}
static PFN_vkGetDeviceMemoryCommitment p_vkGetDeviceMemoryCommitment;
VKAPI_ATTR void VKAPI_CALL vkGetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) {
    if (!p_vkGetDeviceMemoryCommitment) p_vkGetDeviceMemoryCommitment = (PFN_vkGetDeviceMemoryCommitment)ps5_vk_proc("vkGetDeviceMemoryCommitment");
    p_vkGetDeviceMemoryCommitment(device, memory, pCommittedMemoryInBytes);
}
static PFN_vkBindBufferMemory p_vkBindBufferMemory;
VKAPI_ATTR VkResult VKAPI_CALL vkBindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) {
    if (!p_vkBindBufferMemory) p_vkBindBufferMemory = (PFN_vkBindBufferMemory)ps5_vk_proc("vkBindBufferMemory");
    return p_vkBindBufferMemory(device, buffer, memory, memoryOffset);
}
static PFN_vkBindImageMemory p_vkBindImageMemory;
VKAPI_ATTR VkResult VKAPI_CALL vkBindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) {
    if (!p_vkBindImageMemory) p_vkBindImageMemory = (PFN_vkBindImageMemory)ps5_vk_proc("vkBindImageMemory");
    return p_vkBindImageMemory(device, image, memory, memoryOffset);
}
static PFN_vkGetBufferMemoryRequirements p_vkGetBufferMemoryRequirements;
VKAPI_ATTR void VKAPI_CALL vkGetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) {
    if (!p_vkGetBufferMemoryRequirements) p_vkGetBufferMemoryRequirements = (PFN_vkGetBufferMemoryRequirements)ps5_vk_proc("vkGetBufferMemoryRequirements");
    p_vkGetBufferMemoryRequirements(device, buffer, pMemoryRequirements);
}
static PFN_vkGetImageMemoryRequirements p_vkGetImageMemoryRequirements;
VKAPI_ATTR void VKAPI_CALL vkGetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements) {
    if (!p_vkGetImageMemoryRequirements) p_vkGetImageMemoryRequirements = (PFN_vkGetImageMemoryRequirements)ps5_vk_proc("vkGetImageMemoryRequirements");
    p_vkGetImageMemoryRequirements(device, image, pMemoryRequirements);
}
static PFN_vkGetImageSparseMemoryRequirements p_vkGetImageSparseMemoryRequirements;
VKAPI_ATTR void VKAPI_CALL vkGetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) {
    if (!p_vkGetImageSparseMemoryRequirements) p_vkGetImageSparseMemoryRequirements = (PFN_vkGetImageSparseMemoryRequirements)ps5_vk_proc("vkGetImageSparseMemoryRequirements");
    p_vkGetImageSparseMemoryRequirements(device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}
static PFN_vkGetPhysicalDeviceSparseImageFormatProperties p_vkGetPhysicalDeviceSparseImageFormatProperties;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties) {
    if (!p_vkGetPhysicalDeviceSparseImageFormatProperties) p_vkGetPhysicalDeviceSparseImageFormatProperties = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties)ps5_vk_proc("vkGetPhysicalDeviceSparseImageFormatProperties");
    p_vkGetPhysicalDeviceSparseImageFormatProperties(physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);
}
static PFN_vkQueueBindSparse p_vkQueueBindSparse;
VKAPI_ATTR VkResult VKAPI_CALL vkQueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence) {
    if (!p_vkQueueBindSparse) p_vkQueueBindSparse = (PFN_vkQueueBindSparse)ps5_vk_proc("vkQueueBindSparse");
    return p_vkQueueBindSparse(queue, bindInfoCount, pBindInfo, fence);
}
static PFN_vkCreateFence p_vkCreateFence;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) {
    if (!p_vkCreateFence) p_vkCreateFence = (PFN_vkCreateFence)ps5_vk_proc("vkCreateFence");
    return p_vkCreateFence(device, pCreateInfo, pAllocator, pFence);
}
static PFN_vkDestroyFence p_vkDestroyFence;
VKAPI_ATTR void VKAPI_CALL vkDestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyFence) p_vkDestroyFence = (PFN_vkDestroyFence)ps5_vk_proc("vkDestroyFence");
    p_vkDestroyFence(device, fence, pAllocator);
}
static PFN_vkResetFences p_vkResetFences;
VKAPI_ATTR VkResult VKAPI_CALL vkResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences) {
    if (!p_vkResetFences) p_vkResetFences = (PFN_vkResetFences)ps5_vk_proc("vkResetFences");
    return p_vkResetFences(device, fenceCount, pFences);
}
static PFN_vkGetFenceStatus p_vkGetFenceStatus;
VKAPI_ATTR VkResult VKAPI_CALL vkGetFenceStatus(VkDevice device, VkFence fence) {
    if (!p_vkGetFenceStatus) p_vkGetFenceStatus = (PFN_vkGetFenceStatus)ps5_vk_proc("vkGetFenceStatus");
    return p_vkGetFenceStatus(device, fence);
}
static PFN_vkWaitForFences p_vkWaitForFences;
VKAPI_ATTR VkResult VKAPI_CALL vkWaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) {
    if (!p_vkWaitForFences) p_vkWaitForFences = (PFN_vkWaitForFences)ps5_vk_proc("vkWaitForFences");
    return p_vkWaitForFences(device, fenceCount, pFences, waitAll, timeout);
}
static PFN_vkCreateSemaphore p_vkCreateSemaphore;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) {
    if (!p_vkCreateSemaphore) p_vkCreateSemaphore = (PFN_vkCreateSemaphore)ps5_vk_proc("vkCreateSemaphore");
    return p_vkCreateSemaphore(device, pCreateInfo, pAllocator, pSemaphore);
}
static PFN_vkDestroySemaphore p_vkDestroySemaphore;
VKAPI_ATTR void VKAPI_CALL vkDestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroySemaphore) p_vkDestroySemaphore = (PFN_vkDestroySemaphore)ps5_vk_proc("vkDestroySemaphore");
    p_vkDestroySemaphore(device, semaphore, pAllocator);
}
static PFN_vkCreateQueryPool p_vkCreateQueryPool;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) {
    if (!p_vkCreateQueryPool) p_vkCreateQueryPool = (PFN_vkCreateQueryPool)ps5_vk_proc("vkCreateQueryPool");
    return p_vkCreateQueryPool(device, pCreateInfo, pAllocator, pQueryPool);
}
static PFN_vkDestroyQueryPool p_vkDestroyQueryPool;
VKAPI_ATTR void VKAPI_CALL vkDestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyQueryPool) p_vkDestroyQueryPool = (PFN_vkDestroyQueryPool)ps5_vk_proc("vkDestroyQueryPool");
    p_vkDestroyQueryPool(device, queryPool, pAllocator);
}
static PFN_vkGetQueryPoolResults p_vkGetQueryPoolResults;
VKAPI_ATTR VkResult VKAPI_CALL vkGetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) {
    if (!p_vkGetQueryPoolResults) p_vkGetQueryPoolResults = (PFN_vkGetQueryPoolResults)ps5_vk_proc("vkGetQueryPoolResults");
    return p_vkGetQueryPoolResults(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
}
static PFN_vkCreateBuffer p_vkCreateBuffer;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) {
    if (!p_vkCreateBuffer) p_vkCreateBuffer = (PFN_vkCreateBuffer)ps5_vk_proc("vkCreateBuffer");
    return p_vkCreateBuffer(device, pCreateInfo, pAllocator, pBuffer);
}
static PFN_vkDestroyBuffer p_vkDestroyBuffer;
VKAPI_ATTR void VKAPI_CALL vkDestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyBuffer) p_vkDestroyBuffer = (PFN_vkDestroyBuffer)ps5_vk_proc("vkDestroyBuffer");
    p_vkDestroyBuffer(device, buffer, pAllocator);
}
static PFN_vkCreateImage p_vkCreateImage;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage) {
    if (!p_vkCreateImage) p_vkCreateImage = (PFN_vkCreateImage)ps5_vk_proc("vkCreateImage");
    return p_vkCreateImage(device, pCreateInfo, pAllocator, pImage);
}
static PFN_vkDestroyImage p_vkDestroyImage;
VKAPI_ATTR void VKAPI_CALL vkDestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyImage) p_vkDestroyImage = (PFN_vkDestroyImage)ps5_vk_proc("vkDestroyImage");
    p_vkDestroyImage(device, image, pAllocator);
}
static PFN_vkGetImageSubresourceLayout p_vkGetImageSubresourceLayout;
VKAPI_ATTR void VKAPI_CALL vkGetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) {
    if (!p_vkGetImageSubresourceLayout) p_vkGetImageSubresourceLayout = (PFN_vkGetImageSubresourceLayout)ps5_vk_proc("vkGetImageSubresourceLayout");
    p_vkGetImageSubresourceLayout(device, image, pSubresource, pLayout);
}
static PFN_vkCreateImageView p_vkCreateImageView;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView) {
    if (!p_vkCreateImageView) p_vkCreateImageView = (PFN_vkCreateImageView)ps5_vk_proc("vkCreateImageView");
    return p_vkCreateImageView(device, pCreateInfo, pAllocator, pView);
}
static PFN_vkDestroyImageView p_vkDestroyImageView;
VKAPI_ATTR void VKAPI_CALL vkDestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyImageView) p_vkDestroyImageView = (PFN_vkDestroyImageView)ps5_vk_proc("vkDestroyImageView");
    p_vkDestroyImageView(device, imageView, pAllocator);
}
static PFN_vkCreateCommandPool p_vkCreateCommandPool;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) {
    if (!p_vkCreateCommandPool) p_vkCreateCommandPool = (PFN_vkCreateCommandPool)ps5_vk_proc("vkCreateCommandPool");
    return p_vkCreateCommandPool(device, pCreateInfo, pAllocator, pCommandPool);
}
static PFN_vkDestroyCommandPool p_vkDestroyCommandPool;
VKAPI_ATTR void VKAPI_CALL vkDestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyCommandPool) p_vkDestroyCommandPool = (PFN_vkDestroyCommandPool)ps5_vk_proc("vkDestroyCommandPool");
    p_vkDestroyCommandPool(device, commandPool, pAllocator);
}
static PFN_vkResetCommandPool p_vkResetCommandPool;
VKAPI_ATTR VkResult VKAPI_CALL vkResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) {
    if (!p_vkResetCommandPool) p_vkResetCommandPool = (PFN_vkResetCommandPool)ps5_vk_proc("vkResetCommandPool");
    return p_vkResetCommandPool(device, commandPool, flags);
}
static PFN_vkAllocateCommandBuffers p_vkAllocateCommandBuffers;
VKAPI_ATTR VkResult VKAPI_CALL vkAllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) {
    if (!p_vkAllocateCommandBuffers) p_vkAllocateCommandBuffers = (PFN_vkAllocateCommandBuffers)ps5_vk_proc("vkAllocateCommandBuffers");
    return p_vkAllocateCommandBuffers(device, pAllocateInfo, pCommandBuffers);
}
static PFN_vkFreeCommandBuffers p_vkFreeCommandBuffers;
VKAPI_ATTR void VKAPI_CALL vkFreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) {
    if (!p_vkFreeCommandBuffers) p_vkFreeCommandBuffers = (PFN_vkFreeCommandBuffers)ps5_vk_proc("vkFreeCommandBuffers");
    p_vkFreeCommandBuffers(device, commandPool, commandBufferCount, pCommandBuffers);
}
static PFN_vkBeginCommandBuffer p_vkBeginCommandBuffer;
VKAPI_ATTR VkResult VKAPI_CALL vkBeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) {
    if (!p_vkBeginCommandBuffer) p_vkBeginCommandBuffer = (PFN_vkBeginCommandBuffer)ps5_vk_proc("vkBeginCommandBuffer");
    return p_vkBeginCommandBuffer(commandBuffer, pBeginInfo);
}
static PFN_vkEndCommandBuffer p_vkEndCommandBuffer;
VKAPI_ATTR VkResult VKAPI_CALL vkEndCommandBuffer(VkCommandBuffer commandBuffer) {
    if (!p_vkEndCommandBuffer) p_vkEndCommandBuffer = (PFN_vkEndCommandBuffer)ps5_vk_proc("vkEndCommandBuffer");
    return p_vkEndCommandBuffer(commandBuffer);
}
static PFN_vkResetCommandBuffer p_vkResetCommandBuffer;
VKAPI_ATTR VkResult VKAPI_CALL vkResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) {
    if (!p_vkResetCommandBuffer) p_vkResetCommandBuffer = (PFN_vkResetCommandBuffer)ps5_vk_proc("vkResetCommandBuffer");
    return p_vkResetCommandBuffer(commandBuffer, flags);
}
static PFN_vkCmdCopyBuffer p_vkCmdCopyBuffer;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) {
    if (!p_vkCmdCopyBuffer) p_vkCmdCopyBuffer = (PFN_vkCmdCopyBuffer)ps5_vk_proc("vkCmdCopyBuffer");
    p_vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
}
static PFN_vkCmdCopyImage p_vkCmdCopyImage;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) {
    if (!p_vkCmdCopyImage) p_vkCmdCopyImage = (PFN_vkCmdCopyImage)ps5_vk_proc("vkCmdCopyImage");
    p_vkCmdCopyImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}
static PFN_vkCmdCopyBufferToImage p_vkCmdCopyBufferToImage;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) {
    if (!p_vkCmdCopyBufferToImage) p_vkCmdCopyBufferToImage = (PFN_vkCmdCopyBufferToImage)ps5_vk_proc("vkCmdCopyBufferToImage");
    p_vkCmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}
static PFN_vkCmdCopyImageToBuffer p_vkCmdCopyImageToBuffer;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) {
    if (!p_vkCmdCopyImageToBuffer) p_vkCmdCopyImageToBuffer = (PFN_vkCmdCopyImageToBuffer)ps5_vk_proc("vkCmdCopyImageToBuffer");
    p_vkCmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}
static PFN_vkCmdUpdateBuffer p_vkCmdUpdateBuffer;
VKAPI_ATTR void VKAPI_CALL vkCmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) {
    if (!p_vkCmdUpdateBuffer) p_vkCmdUpdateBuffer = (PFN_vkCmdUpdateBuffer)ps5_vk_proc("vkCmdUpdateBuffer");
    p_vkCmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
}
static PFN_vkCmdFillBuffer p_vkCmdFillBuffer;
VKAPI_ATTR void VKAPI_CALL vkCmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) {
    if (!p_vkCmdFillBuffer) p_vkCmdFillBuffer = (PFN_vkCmdFillBuffer)ps5_vk_proc("vkCmdFillBuffer");
    p_vkCmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
}
static PFN_vkCmdPipelineBarrier p_vkCmdPipelineBarrier;
VKAPI_ATTR void VKAPI_CALL vkCmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) {
    if (!p_vkCmdPipelineBarrier) p_vkCmdPipelineBarrier = (PFN_vkCmdPipelineBarrier)ps5_vk_proc("vkCmdPipelineBarrier");
    p_vkCmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}
static PFN_vkCmdBeginQuery p_vkCmdBeginQuery;
VKAPI_ATTR void VKAPI_CALL vkCmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) {
    if (!p_vkCmdBeginQuery) p_vkCmdBeginQuery = (PFN_vkCmdBeginQuery)ps5_vk_proc("vkCmdBeginQuery");
    p_vkCmdBeginQuery(commandBuffer, queryPool, query, flags);
}
static PFN_vkCmdEndQuery p_vkCmdEndQuery;
VKAPI_ATTR void VKAPI_CALL vkCmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) {
    if (!p_vkCmdEndQuery) p_vkCmdEndQuery = (PFN_vkCmdEndQuery)ps5_vk_proc("vkCmdEndQuery");
    p_vkCmdEndQuery(commandBuffer, queryPool, query);
}
static PFN_vkCmdResetQueryPool p_vkCmdResetQueryPool;
VKAPI_ATTR void VKAPI_CALL vkCmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) {
    if (!p_vkCmdResetQueryPool) p_vkCmdResetQueryPool = (PFN_vkCmdResetQueryPool)ps5_vk_proc("vkCmdResetQueryPool");
    p_vkCmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
}
static PFN_vkCmdWriteTimestamp p_vkCmdWriteTimestamp;
VKAPI_ATTR void VKAPI_CALL vkCmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) {
    if (!p_vkCmdWriteTimestamp) p_vkCmdWriteTimestamp = (PFN_vkCmdWriteTimestamp)ps5_vk_proc("vkCmdWriteTimestamp");
    p_vkCmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
}
static PFN_vkCmdCopyQueryPoolResults p_vkCmdCopyQueryPoolResults;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) {
    if (!p_vkCmdCopyQueryPoolResults) p_vkCmdCopyQueryPoolResults = (PFN_vkCmdCopyQueryPoolResults)ps5_vk_proc("vkCmdCopyQueryPoolResults");
    p_vkCmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
}
static PFN_vkCmdExecuteCommands p_vkCmdExecuteCommands;
VKAPI_ATTR void VKAPI_CALL vkCmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) {
    if (!p_vkCmdExecuteCommands) p_vkCmdExecuteCommands = (PFN_vkCmdExecuteCommands)ps5_vk_proc("vkCmdExecuteCommands");
    p_vkCmdExecuteCommands(commandBuffer, commandBufferCount, pCommandBuffers);
}
static PFN_vkCreateEvent p_vkCreateEvent;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent) {
    if (!p_vkCreateEvent) p_vkCreateEvent = (PFN_vkCreateEvent)ps5_vk_proc("vkCreateEvent");
    return p_vkCreateEvent(device, pCreateInfo, pAllocator, pEvent);
}
static PFN_vkDestroyEvent p_vkDestroyEvent;
VKAPI_ATTR void VKAPI_CALL vkDestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyEvent) p_vkDestroyEvent = (PFN_vkDestroyEvent)ps5_vk_proc("vkDestroyEvent");
    p_vkDestroyEvent(device, event, pAllocator);
}
static PFN_vkGetEventStatus p_vkGetEventStatus;
VKAPI_ATTR VkResult VKAPI_CALL vkGetEventStatus(VkDevice device, VkEvent event) {
    if (!p_vkGetEventStatus) p_vkGetEventStatus = (PFN_vkGetEventStatus)ps5_vk_proc("vkGetEventStatus");
    return p_vkGetEventStatus(device, event);
}
static PFN_vkSetEvent p_vkSetEvent;
VKAPI_ATTR VkResult VKAPI_CALL vkSetEvent(VkDevice device, VkEvent event) {
    if (!p_vkSetEvent) p_vkSetEvent = (PFN_vkSetEvent)ps5_vk_proc("vkSetEvent");
    return p_vkSetEvent(device, event);
}
static PFN_vkResetEvent p_vkResetEvent;
VKAPI_ATTR VkResult VKAPI_CALL vkResetEvent(VkDevice device, VkEvent event) {
    if (!p_vkResetEvent) p_vkResetEvent = (PFN_vkResetEvent)ps5_vk_proc("vkResetEvent");
    return p_vkResetEvent(device, event);
}
static PFN_vkCreateBufferView p_vkCreateBufferView;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView) {
    if (!p_vkCreateBufferView) p_vkCreateBufferView = (PFN_vkCreateBufferView)ps5_vk_proc("vkCreateBufferView");
    return p_vkCreateBufferView(device, pCreateInfo, pAllocator, pView);
}
static PFN_vkDestroyBufferView p_vkDestroyBufferView;
VKAPI_ATTR void VKAPI_CALL vkDestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyBufferView) p_vkDestroyBufferView = (PFN_vkDestroyBufferView)ps5_vk_proc("vkDestroyBufferView");
    p_vkDestroyBufferView(device, bufferView, pAllocator);
}
static PFN_vkCreateShaderModule p_vkCreateShaderModule;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) {
    if (!p_vkCreateShaderModule) p_vkCreateShaderModule = (PFN_vkCreateShaderModule)ps5_vk_proc("vkCreateShaderModule");
    return p_vkCreateShaderModule(device, pCreateInfo, pAllocator, pShaderModule);
}
static PFN_vkDestroyShaderModule p_vkDestroyShaderModule;
VKAPI_ATTR void VKAPI_CALL vkDestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyShaderModule) p_vkDestroyShaderModule = (PFN_vkDestroyShaderModule)ps5_vk_proc("vkDestroyShaderModule");
    p_vkDestroyShaderModule(device, shaderModule, pAllocator);
}
static PFN_vkCreatePipelineCache p_vkCreatePipelineCache;
VKAPI_ATTR VkResult VKAPI_CALL vkCreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) {
    if (!p_vkCreatePipelineCache) p_vkCreatePipelineCache = (PFN_vkCreatePipelineCache)ps5_vk_proc("vkCreatePipelineCache");
    return p_vkCreatePipelineCache(device, pCreateInfo, pAllocator, pPipelineCache);
}
static PFN_vkDestroyPipelineCache p_vkDestroyPipelineCache;
VKAPI_ATTR void VKAPI_CALL vkDestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyPipelineCache) p_vkDestroyPipelineCache = (PFN_vkDestroyPipelineCache)ps5_vk_proc("vkDestroyPipelineCache");
    p_vkDestroyPipelineCache(device, pipelineCache, pAllocator);
}
static PFN_vkGetPipelineCacheData p_vkGetPipelineCacheData;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) {
    if (!p_vkGetPipelineCacheData) p_vkGetPipelineCacheData = (PFN_vkGetPipelineCacheData)ps5_vk_proc("vkGetPipelineCacheData");
    return p_vkGetPipelineCacheData(device, pipelineCache, pDataSize, pData);
}
static PFN_vkMergePipelineCaches p_vkMergePipelineCaches;
VKAPI_ATTR VkResult VKAPI_CALL vkMergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches) {
    if (!p_vkMergePipelineCaches) p_vkMergePipelineCaches = (PFN_vkMergePipelineCaches)ps5_vk_proc("vkMergePipelineCaches");
    return p_vkMergePipelineCaches(device, dstCache, srcCacheCount, pSrcCaches);
}
static PFN_vkCreateComputePipelines p_vkCreateComputePipelines;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) {
    if (!p_vkCreateComputePipelines) p_vkCreateComputePipelines = (PFN_vkCreateComputePipelines)ps5_vk_proc("vkCreateComputePipelines");
    return p_vkCreateComputePipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}
static PFN_vkDestroyPipeline p_vkDestroyPipeline;
VKAPI_ATTR void VKAPI_CALL vkDestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyPipeline) p_vkDestroyPipeline = (PFN_vkDestroyPipeline)ps5_vk_proc("vkDestroyPipeline");
    p_vkDestroyPipeline(device, pipeline, pAllocator);
}
static PFN_vkCreatePipelineLayout p_vkCreatePipelineLayout;
VKAPI_ATTR VkResult VKAPI_CALL vkCreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) {
    if (!p_vkCreatePipelineLayout) p_vkCreatePipelineLayout = (PFN_vkCreatePipelineLayout)ps5_vk_proc("vkCreatePipelineLayout");
    return p_vkCreatePipelineLayout(device, pCreateInfo, pAllocator, pPipelineLayout);
}
static PFN_vkDestroyPipelineLayout p_vkDestroyPipelineLayout;
VKAPI_ATTR void VKAPI_CALL vkDestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyPipelineLayout) p_vkDestroyPipelineLayout = (PFN_vkDestroyPipelineLayout)ps5_vk_proc("vkDestroyPipelineLayout");
    p_vkDestroyPipelineLayout(device, pipelineLayout, pAllocator);
}
static PFN_vkCreateSampler p_vkCreateSampler;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler) {
    if (!p_vkCreateSampler) p_vkCreateSampler = (PFN_vkCreateSampler)ps5_vk_proc("vkCreateSampler");
    return p_vkCreateSampler(device, pCreateInfo, pAllocator, pSampler);
}
static PFN_vkDestroySampler p_vkDestroySampler;
VKAPI_ATTR void VKAPI_CALL vkDestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroySampler) p_vkDestroySampler = (PFN_vkDestroySampler)ps5_vk_proc("vkDestroySampler");
    p_vkDestroySampler(device, sampler, pAllocator);
}
static PFN_vkCreateDescriptorSetLayout p_vkCreateDescriptorSetLayout;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) {
    if (!p_vkCreateDescriptorSetLayout) p_vkCreateDescriptorSetLayout = (PFN_vkCreateDescriptorSetLayout)ps5_vk_proc("vkCreateDescriptorSetLayout");
    return p_vkCreateDescriptorSetLayout(device, pCreateInfo, pAllocator, pSetLayout);
}
static PFN_vkDestroyDescriptorSetLayout p_vkDestroyDescriptorSetLayout;
VKAPI_ATTR void VKAPI_CALL vkDestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyDescriptorSetLayout) p_vkDestroyDescriptorSetLayout = (PFN_vkDestroyDescriptorSetLayout)ps5_vk_proc("vkDestroyDescriptorSetLayout");
    p_vkDestroyDescriptorSetLayout(device, descriptorSetLayout, pAllocator);
}
static PFN_vkCreateDescriptorPool p_vkCreateDescriptorPool;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) {
    if (!p_vkCreateDescriptorPool) p_vkCreateDescriptorPool = (PFN_vkCreateDescriptorPool)ps5_vk_proc("vkCreateDescriptorPool");
    return p_vkCreateDescriptorPool(device, pCreateInfo, pAllocator, pDescriptorPool);
}
static PFN_vkDestroyDescriptorPool p_vkDestroyDescriptorPool;
VKAPI_ATTR void VKAPI_CALL vkDestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyDescriptorPool) p_vkDestroyDescriptorPool = (PFN_vkDestroyDescriptorPool)ps5_vk_proc("vkDestroyDescriptorPool");
    p_vkDestroyDescriptorPool(device, descriptorPool, pAllocator);
}
static PFN_vkResetDescriptorPool p_vkResetDescriptorPool;
VKAPI_ATTR VkResult VKAPI_CALL vkResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) {
    if (!p_vkResetDescriptorPool) p_vkResetDescriptorPool = (PFN_vkResetDescriptorPool)ps5_vk_proc("vkResetDescriptorPool");
    return p_vkResetDescriptorPool(device, descriptorPool, flags);
}
static PFN_vkAllocateDescriptorSets p_vkAllocateDescriptorSets;
VKAPI_ATTR VkResult VKAPI_CALL vkAllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) {
    if (!p_vkAllocateDescriptorSets) p_vkAllocateDescriptorSets = (PFN_vkAllocateDescriptorSets)ps5_vk_proc("vkAllocateDescriptorSets");
    return p_vkAllocateDescriptorSets(device, pAllocateInfo, pDescriptorSets);
}
static PFN_vkFreeDescriptorSets p_vkFreeDescriptorSets;
VKAPI_ATTR VkResult VKAPI_CALL vkFreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets) {
    if (!p_vkFreeDescriptorSets) p_vkFreeDescriptorSets = (PFN_vkFreeDescriptorSets)ps5_vk_proc("vkFreeDescriptorSets");
    return p_vkFreeDescriptorSets(device, descriptorPool, descriptorSetCount, pDescriptorSets);
}
static PFN_vkUpdateDescriptorSets p_vkUpdateDescriptorSets;
VKAPI_ATTR void VKAPI_CALL vkUpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies) {
    if (!p_vkUpdateDescriptorSets) p_vkUpdateDescriptorSets = (PFN_vkUpdateDescriptorSets)ps5_vk_proc("vkUpdateDescriptorSets");
    p_vkUpdateDescriptorSets(device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
}
static PFN_vkCmdBindPipeline p_vkCmdBindPipeline;
VKAPI_ATTR void VKAPI_CALL vkCmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) {
    if (!p_vkCmdBindPipeline) p_vkCmdBindPipeline = (PFN_vkCmdBindPipeline)ps5_vk_proc("vkCmdBindPipeline");
    p_vkCmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
}
static PFN_vkCmdBindDescriptorSets p_vkCmdBindDescriptorSets;
VKAPI_ATTR void VKAPI_CALL vkCmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) {
    if (!p_vkCmdBindDescriptorSets) p_vkCmdBindDescriptorSets = (PFN_vkCmdBindDescriptorSets)ps5_vk_proc("vkCmdBindDescriptorSets");
    p_vkCmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}
static PFN_vkCmdClearColorImage p_vkCmdClearColorImage;
VKAPI_ATTR void VKAPI_CALL vkCmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) {
    if (!p_vkCmdClearColorImage) p_vkCmdClearColorImage = (PFN_vkCmdClearColorImage)ps5_vk_proc("vkCmdClearColorImage");
    p_vkCmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
}
static PFN_vkCmdDispatch p_vkCmdDispatch;
VKAPI_ATTR void VKAPI_CALL vkCmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
    if (!p_vkCmdDispatch) p_vkCmdDispatch = (PFN_vkCmdDispatch)ps5_vk_proc("vkCmdDispatch");
    p_vkCmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
}
static PFN_vkCmdDispatchIndirect p_vkCmdDispatchIndirect;
VKAPI_ATTR void VKAPI_CALL vkCmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) {
    if (!p_vkCmdDispatchIndirect) p_vkCmdDispatchIndirect = (PFN_vkCmdDispatchIndirect)ps5_vk_proc("vkCmdDispatchIndirect");
    p_vkCmdDispatchIndirect(commandBuffer, buffer, offset);
}
static PFN_vkCmdSetEvent p_vkCmdSetEvent;
VKAPI_ATTR void VKAPI_CALL vkCmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) {
    if (!p_vkCmdSetEvent) p_vkCmdSetEvent = (PFN_vkCmdSetEvent)ps5_vk_proc("vkCmdSetEvent");
    p_vkCmdSetEvent(commandBuffer, event, stageMask);
}
static PFN_vkCmdResetEvent p_vkCmdResetEvent;
VKAPI_ATTR void VKAPI_CALL vkCmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) {
    if (!p_vkCmdResetEvent) p_vkCmdResetEvent = (PFN_vkCmdResetEvent)ps5_vk_proc("vkCmdResetEvent");
    p_vkCmdResetEvent(commandBuffer, event, stageMask);
}
static PFN_vkCmdWaitEvents p_vkCmdWaitEvents;
VKAPI_ATTR void VKAPI_CALL vkCmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) {
    if (!p_vkCmdWaitEvents) p_vkCmdWaitEvents = (PFN_vkCmdWaitEvents)ps5_vk_proc("vkCmdWaitEvents");
    p_vkCmdWaitEvents(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}
static PFN_vkCmdPushConstants p_vkCmdPushConstants;
VKAPI_ATTR void VKAPI_CALL vkCmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) {
    if (!p_vkCmdPushConstants) p_vkCmdPushConstants = (PFN_vkCmdPushConstants)ps5_vk_proc("vkCmdPushConstants");
    p_vkCmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
}
static PFN_vkCreateGraphicsPipelines p_vkCreateGraphicsPipelines;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) {
    if (!p_vkCreateGraphicsPipelines) p_vkCreateGraphicsPipelines = (PFN_vkCreateGraphicsPipelines)ps5_vk_proc("vkCreateGraphicsPipelines");
    return p_vkCreateGraphicsPipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}
static PFN_vkCreateFramebuffer p_vkCreateFramebuffer;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) {
    if (!p_vkCreateFramebuffer) p_vkCreateFramebuffer = (PFN_vkCreateFramebuffer)ps5_vk_proc("vkCreateFramebuffer");
    return p_vkCreateFramebuffer(device, pCreateInfo, pAllocator, pFramebuffer);
}
static PFN_vkDestroyFramebuffer p_vkDestroyFramebuffer;
VKAPI_ATTR void VKAPI_CALL vkDestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyFramebuffer) p_vkDestroyFramebuffer = (PFN_vkDestroyFramebuffer)ps5_vk_proc("vkDestroyFramebuffer");
    p_vkDestroyFramebuffer(device, framebuffer, pAllocator);
}
static PFN_vkCreateRenderPass p_vkCreateRenderPass;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) {
    if (!p_vkCreateRenderPass) p_vkCreateRenderPass = (PFN_vkCreateRenderPass)ps5_vk_proc("vkCreateRenderPass");
    return p_vkCreateRenderPass(device, pCreateInfo, pAllocator, pRenderPass);
}
static PFN_vkDestroyRenderPass p_vkDestroyRenderPass;
VKAPI_ATTR void VKAPI_CALL vkDestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyRenderPass) p_vkDestroyRenderPass = (PFN_vkDestroyRenderPass)ps5_vk_proc("vkDestroyRenderPass");
    p_vkDestroyRenderPass(device, renderPass, pAllocator);
}
static PFN_vkGetRenderAreaGranularity p_vkGetRenderAreaGranularity;
VKAPI_ATTR void VKAPI_CALL vkGetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity) {
    if (!p_vkGetRenderAreaGranularity) p_vkGetRenderAreaGranularity = (PFN_vkGetRenderAreaGranularity)ps5_vk_proc("vkGetRenderAreaGranularity");
    p_vkGetRenderAreaGranularity(device, renderPass, pGranularity);
}
static PFN_vkCmdSetViewport p_vkCmdSetViewport;
VKAPI_ATTR void VKAPI_CALL vkCmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) {
    if (!p_vkCmdSetViewport) p_vkCmdSetViewport = (PFN_vkCmdSetViewport)ps5_vk_proc("vkCmdSetViewport");
    p_vkCmdSetViewport(commandBuffer, firstViewport, viewportCount, pViewports);
}
static PFN_vkCmdSetScissor p_vkCmdSetScissor;
VKAPI_ATTR void VKAPI_CALL vkCmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) {
    if (!p_vkCmdSetScissor) p_vkCmdSetScissor = (PFN_vkCmdSetScissor)ps5_vk_proc("vkCmdSetScissor");
    p_vkCmdSetScissor(commandBuffer, firstScissor, scissorCount, pScissors);
}
static PFN_vkCmdSetLineWidth p_vkCmdSetLineWidth;
VKAPI_ATTR void VKAPI_CALL vkCmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) {
    if (!p_vkCmdSetLineWidth) p_vkCmdSetLineWidth = (PFN_vkCmdSetLineWidth)ps5_vk_proc("vkCmdSetLineWidth");
    p_vkCmdSetLineWidth(commandBuffer, lineWidth);
}
static PFN_vkCmdSetDepthBias p_vkCmdSetDepthBias;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) {
    if (!p_vkCmdSetDepthBias) p_vkCmdSetDepthBias = (PFN_vkCmdSetDepthBias)ps5_vk_proc("vkCmdSetDepthBias");
    p_vkCmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}
static PFN_vkCmdSetBlendConstants p_vkCmdSetBlendConstants;
VKAPI_ATTR void VKAPI_CALL vkCmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) {
    if (!p_vkCmdSetBlendConstants) p_vkCmdSetBlendConstants = (PFN_vkCmdSetBlendConstants)ps5_vk_proc("vkCmdSetBlendConstants");
    p_vkCmdSetBlendConstants(commandBuffer, blendConstants);
}
static PFN_vkCmdSetDepthBounds p_vkCmdSetDepthBounds;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) {
    if (!p_vkCmdSetDepthBounds) p_vkCmdSetDepthBounds = (PFN_vkCmdSetDepthBounds)ps5_vk_proc("vkCmdSetDepthBounds");
    p_vkCmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
}
static PFN_vkCmdSetStencilCompareMask p_vkCmdSetStencilCompareMask;
VKAPI_ATTR void VKAPI_CALL vkCmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) {
    if (!p_vkCmdSetStencilCompareMask) p_vkCmdSetStencilCompareMask = (PFN_vkCmdSetStencilCompareMask)ps5_vk_proc("vkCmdSetStencilCompareMask");
    p_vkCmdSetStencilCompareMask(commandBuffer, faceMask, compareMask);
}
static PFN_vkCmdSetStencilWriteMask p_vkCmdSetStencilWriteMask;
VKAPI_ATTR void VKAPI_CALL vkCmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) {
    if (!p_vkCmdSetStencilWriteMask) p_vkCmdSetStencilWriteMask = (PFN_vkCmdSetStencilWriteMask)ps5_vk_proc("vkCmdSetStencilWriteMask");
    p_vkCmdSetStencilWriteMask(commandBuffer, faceMask, writeMask);
}
static PFN_vkCmdSetStencilReference p_vkCmdSetStencilReference;
VKAPI_ATTR void VKAPI_CALL vkCmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) {
    if (!p_vkCmdSetStencilReference) p_vkCmdSetStencilReference = (PFN_vkCmdSetStencilReference)ps5_vk_proc("vkCmdSetStencilReference");
    p_vkCmdSetStencilReference(commandBuffer, faceMask, reference);
}
static PFN_vkCmdBindIndexBuffer p_vkCmdBindIndexBuffer;
VKAPI_ATTR void VKAPI_CALL vkCmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) {
    if (!p_vkCmdBindIndexBuffer) p_vkCmdBindIndexBuffer = (PFN_vkCmdBindIndexBuffer)ps5_vk_proc("vkCmdBindIndexBuffer");
    p_vkCmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
}
static PFN_vkCmdBindVertexBuffers p_vkCmdBindVertexBuffers;
VKAPI_ATTR void VKAPI_CALL vkCmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) {
    if (!p_vkCmdBindVertexBuffers) p_vkCmdBindVertexBuffers = (PFN_vkCmdBindVertexBuffers)ps5_vk_proc("vkCmdBindVertexBuffers");
    p_vkCmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
}
static PFN_vkCmdDraw p_vkCmdDraw;
VKAPI_ATTR void VKAPI_CALL vkCmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
    if (!p_vkCmdDraw) p_vkCmdDraw = (PFN_vkCmdDraw)ps5_vk_proc("vkCmdDraw");
    p_vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}
static PFN_vkCmdDrawIndexed p_vkCmdDrawIndexed;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {
    if (!p_vkCmdDrawIndexed) p_vkCmdDrawIndexed = (PFN_vkCmdDrawIndexed)ps5_vk_proc("vkCmdDrawIndexed");
    p_vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}
static PFN_vkCmdDrawIndirect p_vkCmdDrawIndirect;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
    if (!p_vkCmdDrawIndirect) p_vkCmdDrawIndirect = (PFN_vkCmdDrawIndirect)ps5_vk_proc("vkCmdDrawIndirect");
    p_vkCmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
}
static PFN_vkCmdDrawIndexedIndirect p_vkCmdDrawIndexedIndirect;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
    if (!p_vkCmdDrawIndexedIndirect) p_vkCmdDrawIndexedIndirect = (PFN_vkCmdDrawIndexedIndirect)ps5_vk_proc("vkCmdDrawIndexedIndirect");
    p_vkCmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
}
static PFN_vkCmdBlitImage p_vkCmdBlitImage;
VKAPI_ATTR void VKAPI_CALL vkCmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) {
    if (!p_vkCmdBlitImage) p_vkCmdBlitImage = (PFN_vkCmdBlitImage)ps5_vk_proc("vkCmdBlitImage");
    p_vkCmdBlitImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
}
static PFN_vkCmdClearDepthStencilImage p_vkCmdClearDepthStencilImage;
VKAPI_ATTR void VKAPI_CALL vkCmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) {
    if (!p_vkCmdClearDepthStencilImage) p_vkCmdClearDepthStencilImage = (PFN_vkCmdClearDepthStencilImage)ps5_vk_proc("vkCmdClearDepthStencilImage");
    p_vkCmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
}
static PFN_vkCmdClearAttachments p_vkCmdClearAttachments;
VKAPI_ATTR void VKAPI_CALL vkCmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) {
    if (!p_vkCmdClearAttachments) p_vkCmdClearAttachments = (PFN_vkCmdClearAttachments)ps5_vk_proc("vkCmdClearAttachments");
    p_vkCmdClearAttachments(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
}
static PFN_vkCmdResolveImage p_vkCmdResolveImage;
VKAPI_ATTR void VKAPI_CALL vkCmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) {
    if (!p_vkCmdResolveImage) p_vkCmdResolveImage = (PFN_vkCmdResolveImage)ps5_vk_proc("vkCmdResolveImage");
    p_vkCmdResolveImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}
static PFN_vkCmdBeginRenderPass p_vkCmdBeginRenderPass;
VKAPI_ATTR void VKAPI_CALL vkCmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) {
    if (!p_vkCmdBeginRenderPass) p_vkCmdBeginRenderPass = (PFN_vkCmdBeginRenderPass)ps5_vk_proc("vkCmdBeginRenderPass");
    p_vkCmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
}
static PFN_vkCmdNextSubpass p_vkCmdNextSubpass;
VKAPI_ATTR void VKAPI_CALL vkCmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) {
    if (!p_vkCmdNextSubpass) p_vkCmdNextSubpass = (PFN_vkCmdNextSubpass)ps5_vk_proc("vkCmdNextSubpass");
    p_vkCmdNextSubpass(commandBuffer, contents);
}
static PFN_vkCmdEndRenderPass p_vkCmdEndRenderPass;
VKAPI_ATTR void VKAPI_CALL vkCmdEndRenderPass(VkCommandBuffer commandBuffer) {
    if (!p_vkCmdEndRenderPass) p_vkCmdEndRenderPass = (PFN_vkCmdEndRenderPass)ps5_vk_proc("vkCmdEndRenderPass");
    p_vkCmdEndRenderPass(commandBuffer);
}
static PFN_vkEnumerateInstanceVersion p_vkEnumerateInstanceVersion;
VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceVersion(uint32_t* pApiVersion) {
    if (!p_vkEnumerateInstanceVersion) p_vkEnumerateInstanceVersion = (PFN_vkEnumerateInstanceVersion)ps5_vk_proc("vkEnumerateInstanceVersion");
    return p_vkEnumerateInstanceVersion(pApiVersion);
}
static PFN_vkBindBufferMemory2 p_vkBindBufferMemory2;
VKAPI_ATTR VkResult VKAPI_CALL vkBindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) {
    if (!p_vkBindBufferMemory2) p_vkBindBufferMemory2 = (PFN_vkBindBufferMemory2)ps5_vk_proc("vkBindBufferMemory2");
    return p_vkBindBufferMemory2(device, bindInfoCount, pBindInfos);
}
static PFN_vkBindImageMemory2 p_vkBindImageMemory2;
VKAPI_ATTR VkResult VKAPI_CALL vkBindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) {
    if (!p_vkBindImageMemory2) p_vkBindImageMemory2 = (PFN_vkBindImageMemory2)ps5_vk_proc("vkBindImageMemory2");
    return p_vkBindImageMemory2(device, bindInfoCount, pBindInfos);
}
static PFN_vkGetDeviceGroupPeerMemoryFeatures p_vkGetDeviceGroupPeerMemoryFeatures;
VKAPI_ATTR void VKAPI_CALL vkGetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) {
    if (!p_vkGetDeviceGroupPeerMemoryFeatures) p_vkGetDeviceGroupPeerMemoryFeatures = (PFN_vkGetDeviceGroupPeerMemoryFeatures)ps5_vk_proc("vkGetDeviceGroupPeerMemoryFeatures");
    p_vkGetDeviceGroupPeerMemoryFeatures(device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
}
static PFN_vkCmdSetDeviceMask p_vkCmdSetDeviceMask;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) {
    if (!p_vkCmdSetDeviceMask) p_vkCmdSetDeviceMask = (PFN_vkCmdSetDeviceMask)ps5_vk_proc("vkCmdSetDeviceMask");
    p_vkCmdSetDeviceMask(commandBuffer, deviceMask);
}
static PFN_vkEnumeratePhysicalDeviceGroups p_vkEnumeratePhysicalDeviceGroups;
VKAPI_ATTR VkResult VKAPI_CALL vkEnumeratePhysicalDeviceGroups(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties) {
    if (!p_vkEnumeratePhysicalDeviceGroups) p_vkEnumeratePhysicalDeviceGroups = (PFN_vkEnumeratePhysicalDeviceGroups)ps5_vk_proc("vkEnumeratePhysicalDeviceGroups");
    return p_vkEnumeratePhysicalDeviceGroups(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
}
static PFN_vkGetImageMemoryRequirements2 p_vkGetImageMemoryRequirements2;
VKAPI_ATTR void VKAPI_CALL vkGetImageMemoryRequirements2(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
    if (!p_vkGetImageMemoryRequirements2) p_vkGetImageMemoryRequirements2 = (PFN_vkGetImageMemoryRequirements2)ps5_vk_proc("vkGetImageMemoryRequirements2");
    p_vkGetImageMemoryRequirements2(device, pInfo, pMemoryRequirements);
}
static PFN_vkGetBufferMemoryRequirements2 p_vkGetBufferMemoryRequirements2;
VKAPI_ATTR void VKAPI_CALL vkGetBufferMemoryRequirements2(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
    if (!p_vkGetBufferMemoryRequirements2) p_vkGetBufferMemoryRequirements2 = (PFN_vkGetBufferMemoryRequirements2)ps5_vk_proc("vkGetBufferMemoryRequirements2");
    p_vkGetBufferMemoryRequirements2(device, pInfo, pMemoryRequirements);
}
static PFN_vkGetImageSparseMemoryRequirements2 p_vkGetImageSparseMemoryRequirements2;
VKAPI_ATTR void VKAPI_CALL vkGetImageSparseMemoryRequirements2(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) {
    if (!p_vkGetImageSparseMemoryRequirements2) p_vkGetImageSparseMemoryRequirements2 = (PFN_vkGetImageSparseMemoryRequirements2)ps5_vk_proc("vkGetImageSparseMemoryRequirements2");
    p_vkGetImageSparseMemoryRequirements2(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}
static PFN_vkGetPhysicalDeviceFeatures2 p_vkGetPhysicalDeviceFeatures2;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFeatures2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures) {
    if (!p_vkGetPhysicalDeviceFeatures2) p_vkGetPhysicalDeviceFeatures2 = (PFN_vkGetPhysicalDeviceFeatures2)ps5_vk_proc("vkGetPhysicalDeviceFeatures2");
    p_vkGetPhysicalDeviceFeatures2(physicalDevice, pFeatures);
}
static PFN_vkGetPhysicalDeviceProperties2 p_vkGetPhysicalDeviceProperties2;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties) {
    if (!p_vkGetPhysicalDeviceProperties2) p_vkGetPhysicalDeviceProperties2 = (PFN_vkGetPhysicalDeviceProperties2)ps5_vk_proc("vkGetPhysicalDeviceProperties2");
    p_vkGetPhysicalDeviceProperties2(physicalDevice, pProperties);
}
static PFN_vkGetPhysicalDeviceFormatProperties2 p_vkGetPhysicalDeviceFormatProperties2;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFormatProperties2(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties) {
    if (!p_vkGetPhysicalDeviceFormatProperties2) p_vkGetPhysicalDeviceFormatProperties2 = (PFN_vkGetPhysicalDeviceFormatProperties2)ps5_vk_proc("vkGetPhysicalDeviceFormatProperties2");
    p_vkGetPhysicalDeviceFormatProperties2(physicalDevice, format, pFormatProperties);
}
static PFN_vkGetPhysicalDeviceImageFormatProperties2 p_vkGetPhysicalDeviceImageFormatProperties2;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties) {
    if (!p_vkGetPhysicalDeviceImageFormatProperties2) p_vkGetPhysicalDeviceImageFormatProperties2 = (PFN_vkGetPhysicalDeviceImageFormatProperties2)ps5_vk_proc("vkGetPhysicalDeviceImageFormatProperties2");
    return p_vkGetPhysicalDeviceImageFormatProperties2(physicalDevice, pImageFormatInfo, pImageFormatProperties);
}
static PFN_vkGetPhysicalDeviceQueueFamilyProperties2 p_vkGetPhysicalDeviceQueueFamilyProperties2;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties2(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties) {
    if (!p_vkGetPhysicalDeviceQueueFamilyProperties2) p_vkGetPhysicalDeviceQueueFamilyProperties2 = (PFN_vkGetPhysicalDeviceQueueFamilyProperties2)ps5_vk_proc("vkGetPhysicalDeviceQueueFamilyProperties2");
    p_vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}
static PFN_vkGetPhysicalDeviceMemoryProperties2 p_vkGetPhysicalDeviceMemoryProperties2;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties) {
    if (!p_vkGetPhysicalDeviceMemoryProperties2) p_vkGetPhysicalDeviceMemoryProperties2 = (PFN_vkGetPhysicalDeviceMemoryProperties2)ps5_vk_proc("vkGetPhysicalDeviceMemoryProperties2");
    p_vkGetPhysicalDeviceMemoryProperties2(physicalDevice, pMemoryProperties);
}
static PFN_vkGetPhysicalDeviceSparseImageFormatProperties2 p_vkGetPhysicalDeviceSparseImageFormatProperties2;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties) {
    if (!p_vkGetPhysicalDeviceSparseImageFormatProperties2) p_vkGetPhysicalDeviceSparseImageFormatProperties2 = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties2)ps5_vk_proc("vkGetPhysicalDeviceSparseImageFormatProperties2");
    p_vkGetPhysicalDeviceSparseImageFormatProperties2(physicalDevice, pFormatInfo, pPropertyCount, pProperties);
}
static PFN_vkTrimCommandPool p_vkTrimCommandPool;
VKAPI_ATTR void VKAPI_CALL vkTrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) {
    if (!p_vkTrimCommandPool) p_vkTrimCommandPool = (PFN_vkTrimCommandPool)ps5_vk_proc("vkTrimCommandPool");
    p_vkTrimCommandPool(device, commandPool, flags);
}
static PFN_vkGetDeviceQueue2 p_vkGetDeviceQueue2;
VKAPI_ATTR void VKAPI_CALL vkGetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue) {
    if (!p_vkGetDeviceQueue2) p_vkGetDeviceQueue2 = (PFN_vkGetDeviceQueue2)ps5_vk_proc("vkGetDeviceQueue2");
    p_vkGetDeviceQueue2(device, pQueueInfo, pQueue);
}
static PFN_vkGetPhysicalDeviceExternalBufferProperties p_vkGetPhysicalDeviceExternalBufferProperties;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceExternalBufferProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties) {
    if (!p_vkGetPhysicalDeviceExternalBufferProperties) p_vkGetPhysicalDeviceExternalBufferProperties = (PFN_vkGetPhysicalDeviceExternalBufferProperties)ps5_vk_proc("vkGetPhysicalDeviceExternalBufferProperties");
    p_vkGetPhysicalDeviceExternalBufferProperties(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
}
static PFN_vkGetPhysicalDeviceExternalFenceProperties p_vkGetPhysicalDeviceExternalFenceProperties;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceExternalFenceProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties) {
    if (!p_vkGetPhysicalDeviceExternalFenceProperties) p_vkGetPhysicalDeviceExternalFenceProperties = (PFN_vkGetPhysicalDeviceExternalFenceProperties)ps5_vk_proc("vkGetPhysicalDeviceExternalFenceProperties");
    p_vkGetPhysicalDeviceExternalFenceProperties(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
}
static PFN_vkGetPhysicalDeviceExternalSemaphoreProperties p_vkGetPhysicalDeviceExternalSemaphoreProperties;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceExternalSemaphoreProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties) {
    if (!p_vkGetPhysicalDeviceExternalSemaphoreProperties) p_vkGetPhysicalDeviceExternalSemaphoreProperties = (PFN_vkGetPhysicalDeviceExternalSemaphoreProperties)ps5_vk_proc("vkGetPhysicalDeviceExternalSemaphoreProperties");
    p_vkGetPhysicalDeviceExternalSemaphoreProperties(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
}
static PFN_vkCmdDispatchBase p_vkCmdDispatchBase;
VKAPI_ATTR void VKAPI_CALL vkCmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
    if (!p_vkCmdDispatchBase) p_vkCmdDispatchBase = (PFN_vkCmdDispatchBase)ps5_vk_proc("vkCmdDispatchBase");
    p_vkCmdDispatchBase(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}
static PFN_vkCreateDescriptorUpdateTemplate p_vkCreateDescriptorUpdateTemplate;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDescriptorUpdateTemplate(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) {
    if (!p_vkCreateDescriptorUpdateTemplate) p_vkCreateDescriptorUpdateTemplate = (PFN_vkCreateDescriptorUpdateTemplate)ps5_vk_proc("vkCreateDescriptorUpdateTemplate");
    return p_vkCreateDescriptorUpdateTemplate(device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
}
static PFN_vkDestroyDescriptorUpdateTemplate p_vkDestroyDescriptorUpdateTemplate;
VKAPI_ATTR void VKAPI_CALL vkDestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyDescriptorUpdateTemplate) p_vkDestroyDescriptorUpdateTemplate = (PFN_vkDestroyDescriptorUpdateTemplate)ps5_vk_proc("vkDestroyDescriptorUpdateTemplate");
    p_vkDestroyDescriptorUpdateTemplate(device, descriptorUpdateTemplate, pAllocator);
}
static PFN_vkUpdateDescriptorSetWithTemplate p_vkUpdateDescriptorSetWithTemplate;
VKAPI_ATTR void VKAPI_CALL vkUpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) {
    if (!p_vkUpdateDescriptorSetWithTemplate) p_vkUpdateDescriptorSetWithTemplate = (PFN_vkUpdateDescriptorSetWithTemplate)ps5_vk_proc("vkUpdateDescriptorSetWithTemplate");
    p_vkUpdateDescriptorSetWithTemplate(device, descriptorSet, descriptorUpdateTemplate, pData);
}
static PFN_vkGetDescriptorSetLayoutSupport p_vkGetDescriptorSetLayoutSupport;
VKAPI_ATTR void VKAPI_CALL vkGetDescriptorSetLayoutSupport(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) {
    if (!p_vkGetDescriptorSetLayoutSupport) p_vkGetDescriptorSetLayoutSupport = (PFN_vkGetDescriptorSetLayoutSupport)ps5_vk_proc("vkGetDescriptorSetLayoutSupport");
    p_vkGetDescriptorSetLayoutSupport(device, pCreateInfo, pSupport);
}
static PFN_vkCreateSamplerYcbcrConversion p_vkCreateSamplerYcbcrConversion;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateSamplerYcbcrConversion(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) {
    if (!p_vkCreateSamplerYcbcrConversion) p_vkCreateSamplerYcbcrConversion = (PFN_vkCreateSamplerYcbcrConversion)ps5_vk_proc("vkCreateSamplerYcbcrConversion");
    return p_vkCreateSamplerYcbcrConversion(device, pCreateInfo, pAllocator, pYcbcrConversion);
}
static PFN_vkDestroySamplerYcbcrConversion p_vkDestroySamplerYcbcrConversion;
VKAPI_ATTR void VKAPI_CALL vkDestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroySamplerYcbcrConversion) p_vkDestroySamplerYcbcrConversion = (PFN_vkDestroySamplerYcbcrConversion)ps5_vk_proc("vkDestroySamplerYcbcrConversion");
    p_vkDestroySamplerYcbcrConversion(device, ycbcrConversion, pAllocator);
}
static PFN_vkResetQueryPool p_vkResetQueryPool;
VKAPI_ATTR void VKAPI_CALL vkResetQueryPool(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) {
    if (!p_vkResetQueryPool) p_vkResetQueryPool = (PFN_vkResetQueryPool)ps5_vk_proc("vkResetQueryPool");
    p_vkResetQueryPool(device, queryPool, firstQuery, queryCount);
}
static PFN_vkGetSemaphoreCounterValue p_vkGetSemaphoreCounterValue;
VKAPI_ATTR VkResult VKAPI_CALL vkGetSemaphoreCounterValue(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) {
    if (!p_vkGetSemaphoreCounterValue) p_vkGetSemaphoreCounterValue = (PFN_vkGetSemaphoreCounterValue)ps5_vk_proc("vkGetSemaphoreCounterValue");
    return p_vkGetSemaphoreCounterValue(device, semaphore, pValue);
}
static PFN_vkWaitSemaphores p_vkWaitSemaphores;
VKAPI_ATTR VkResult VKAPI_CALL vkWaitSemaphores(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) {
    if (!p_vkWaitSemaphores) p_vkWaitSemaphores = (PFN_vkWaitSemaphores)ps5_vk_proc("vkWaitSemaphores");
    return p_vkWaitSemaphores(device, pWaitInfo, timeout);
}
static PFN_vkSignalSemaphore p_vkSignalSemaphore;
VKAPI_ATTR VkResult VKAPI_CALL vkSignalSemaphore(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo) {
    if (!p_vkSignalSemaphore) p_vkSignalSemaphore = (PFN_vkSignalSemaphore)ps5_vk_proc("vkSignalSemaphore");
    return p_vkSignalSemaphore(device, pSignalInfo);
}
static PFN_vkGetBufferDeviceAddress p_vkGetBufferDeviceAddress;
VKAPI_ATTR VkDeviceAddress VKAPI_CALL vkGetBufferDeviceAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) {
    if (!p_vkGetBufferDeviceAddress) p_vkGetBufferDeviceAddress = (PFN_vkGetBufferDeviceAddress)ps5_vk_proc("vkGetBufferDeviceAddress");
    return p_vkGetBufferDeviceAddress(device, pInfo);
}
static PFN_vkGetBufferOpaqueCaptureAddress p_vkGetBufferOpaqueCaptureAddress;
VKAPI_ATTR uint64_t VKAPI_CALL vkGetBufferOpaqueCaptureAddress(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) {
    if (!p_vkGetBufferOpaqueCaptureAddress) p_vkGetBufferOpaqueCaptureAddress = (PFN_vkGetBufferOpaqueCaptureAddress)ps5_vk_proc("vkGetBufferOpaqueCaptureAddress");
    return p_vkGetBufferOpaqueCaptureAddress(device, pInfo);
}
static PFN_vkGetDeviceMemoryOpaqueCaptureAddress p_vkGetDeviceMemoryOpaqueCaptureAddress;
VKAPI_ATTR uint64_t VKAPI_CALL vkGetDeviceMemoryOpaqueCaptureAddress(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) {
    if (!p_vkGetDeviceMemoryOpaqueCaptureAddress) p_vkGetDeviceMemoryOpaqueCaptureAddress = (PFN_vkGetDeviceMemoryOpaqueCaptureAddress)ps5_vk_proc("vkGetDeviceMemoryOpaqueCaptureAddress");
    return p_vkGetDeviceMemoryOpaqueCaptureAddress(device, pInfo);
}
static PFN_vkCmdDrawIndirectCount p_vkCmdDrawIndirectCount;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
    if (!p_vkCmdDrawIndirectCount) p_vkCmdDrawIndirectCount = (PFN_vkCmdDrawIndirectCount)ps5_vk_proc("vkCmdDrawIndirectCount");
    p_vkCmdDrawIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
static PFN_vkCmdDrawIndexedIndirectCount p_vkCmdDrawIndexedIndirectCount;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
    if (!p_vkCmdDrawIndexedIndirectCount) p_vkCmdDrawIndexedIndirectCount = (PFN_vkCmdDrawIndexedIndirectCount)ps5_vk_proc("vkCmdDrawIndexedIndirectCount");
    p_vkCmdDrawIndexedIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
static PFN_vkCreateRenderPass2 p_vkCreateRenderPass2;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateRenderPass2(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) {
    if (!p_vkCreateRenderPass2) p_vkCreateRenderPass2 = (PFN_vkCreateRenderPass2)ps5_vk_proc("vkCreateRenderPass2");
    return p_vkCreateRenderPass2(device, pCreateInfo, pAllocator, pRenderPass);
}
static PFN_vkCmdBeginRenderPass2 p_vkCmdBeginRenderPass2;
VKAPI_ATTR void VKAPI_CALL vkCmdBeginRenderPass2(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo) {
    if (!p_vkCmdBeginRenderPass2) p_vkCmdBeginRenderPass2 = (PFN_vkCmdBeginRenderPass2)ps5_vk_proc("vkCmdBeginRenderPass2");
    p_vkCmdBeginRenderPass2(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}
static PFN_vkCmdNextSubpass2 p_vkCmdNextSubpass2;
VKAPI_ATTR void VKAPI_CALL vkCmdNextSubpass2(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo* pSubpassBeginInfo, const VkSubpassEndInfo* pSubpassEndInfo) {
    if (!p_vkCmdNextSubpass2) p_vkCmdNextSubpass2 = (PFN_vkCmdNextSubpass2)ps5_vk_proc("vkCmdNextSubpass2");
    p_vkCmdNextSubpass2(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}
static PFN_vkCmdEndRenderPass2 p_vkCmdEndRenderPass2;
VKAPI_ATTR void VKAPI_CALL vkCmdEndRenderPass2(VkCommandBuffer commandBuffer, const VkSubpassEndInfo* pSubpassEndInfo) {
    if (!p_vkCmdEndRenderPass2) p_vkCmdEndRenderPass2 = (PFN_vkCmdEndRenderPass2)ps5_vk_proc("vkCmdEndRenderPass2");
    p_vkCmdEndRenderPass2(commandBuffer, pSubpassEndInfo);
}
static PFN_vkGetPhysicalDeviceToolProperties p_vkGetPhysicalDeviceToolProperties;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceToolProperties(VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties) {
    if (!p_vkGetPhysicalDeviceToolProperties) p_vkGetPhysicalDeviceToolProperties = (PFN_vkGetPhysicalDeviceToolProperties)ps5_vk_proc("vkGetPhysicalDeviceToolProperties");
    return p_vkGetPhysicalDeviceToolProperties(physicalDevice, pToolCount, pToolProperties);
}
static PFN_vkCreatePrivateDataSlot p_vkCreatePrivateDataSlot;
VKAPI_ATTR VkResult VKAPI_CALL vkCreatePrivateDataSlot(VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot) {
    if (!p_vkCreatePrivateDataSlot) p_vkCreatePrivateDataSlot = (PFN_vkCreatePrivateDataSlot)ps5_vk_proc("vkCreatePrivateDataSlot");
    return p_vkCreatePrivateDataSlot(device, pCreateInfo, pAllocator, pPrivateDataSlot);
}
static PFN_vkDestroyPrivateDataSlot p_vkDestroyPrivateDataSlot;
VKAPI_ATTR void VKAPI_CALL vkDestroyPrivateDataSlot(VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyPrivateDataSlot) p_vkDestroyPrivateDataSlot = (PFN_vkDestroyPrivateDataSlot)ps5_vk_proc("vkDestroyPrivateDataSlot");
    p_vkDestroyPrivateDataSlot(device, privateDataSlot, pAllocator);
}
static PFN_vkSetPrivateData p_vkSetPrivateData;
VKAPI_ATTR VkResult VKAPI_CALL vkSetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data) {
    if (!p_vkSetPrivateData) p_vkSetPrivateData = (PFN_vkSetPrivateData)ps5_vk_proc("vkSetPrivateData");
    return p_vkSetPrivateData(device, objectType, objectHandle, privateDataSlot, data);
}
static PFN_vkGetPrivateData p_vkGetPrivateData;
VKAPI_ATTR void VKAPI_CALL vkGetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData) {
    if (!p_vkGetPrivateData) p_vkGetPrivateData = (PFN_vkGetPrivateData)ps5_vk_proc("vkGetPrivateData");
    p_vkGetPrivateData(device, objectType, objectHandle, privateDataSlot, pData);
}
static PFN_vkCmdPipelineBarrier2 p_vkCmdPipelineBarrier2;
VKAPI_ATTR void VKAPI_CALL vkCmdPipelineBarrier2(VkCommandBuffer commandBuffer, const VkDependencyInfo* pDependencyInfo) {
    if (!p_vkCmdPipelineBarrier2) p_vkCmdPipelineBarrier2 = (PFN_vkCmdPipelineBarrier2)ps5_vk_proc("vkCmdPipelineBarrier2");
    p_vkCmdPipelineBarrier2(commandBuffer, pDependencyInfo);
}
static PFN_vkCmdWriteTimestamp2 p_vkCmdWriteTimestamp2;
VKAPI_ATTR void VKAPI_CALL vkCmdWriteTimestamp2(VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query) {
    if (!p_vkCmdWriteTimestamp2) p_vkCmdWriteTimestamp2 = (PFN_vkCmdWriteTimestamp2)ps5_vk_proc("vkCmdWriteTimestamp2");
    p_vkCmdWriteTimestamp2(commandBuffer, stage, queryPool, query);
}
static PFN_vkQueueSubmit2 p_vkQueueSubmit2;
VKAPI_ATTR VkResult VKAPI_CALL vkQueueSubmit2(VkQueue queue, uint32_t submitCount, const VkSubmitInfo2* pSubmits, VkFence fence) {
    if (!p_vkQueueSubmit2) p_vkQueueSubmit2 = (PFN_vkQueueSubmit2)ps5_vk_proc("vkQueueSubmit2");
    return p_vkQueueSubmit2(queue, submitCount, pSubmits, fence);
}
static PFN_vkCmdCopyBuffer2 p_vkCmdCopyBuffer2;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyBuffer2(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo) {
    if (!p_vkCmdCopyBuffer2) p_vkCmdCopyBuffer2 = (PFN_vkCmdCopyBuffer2)ps5_vk_proc("vkCmdCopyBuffer2");
    p_vkCmdCopyBuffer2(commandBuffer, pCopyBufferInfo);
}
static PFN_vkCmdCopyImage2 p_vkCmdCopyImage2;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyImage2(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo) {
    if (!p_vkCmdCopyImage2) p_vkCmdCopyImage2 = (PFN_vkCmdCopyImage2)ps5_vk_proc("vkCmdCopyImage2");
    p_vkCmdCopyImage2(commandBuffer, pCopyImageInfo);
}
static PFN_vkCmdCopyBufferToImage2 p_vkCmdCopyBufferToImage2;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyBufferToImage2(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) {
    if (!p_vkCmdCopyBufferToImage2) p_vkCmdCopyBufferToImage2 = (PFN_vkCmdCopyBufferToImage2)ps5_vk_proc("vkCmdCopyBufferToImage2");
    p_vkCmdCopyBufferToImage2(commandBuffer, pCopyBufferToImageInfo);
}
static PFN_vkCmdCopyImageToBuffer2 p_vkCmdCopyImageToBuffer2;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyImageToBuffer2(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) {
    if (!p_vkCmdCopyImageToBuffer2) p_vkCmdCopyImageToBuffer2 = (PFN_vkCmdCopyImageToBuffer2)ps5_vk_proc("vkCmdCopyImageToBuffer2");
    p_vkCmdCopyImageToBuffer2(commandBuffer, pCopyImageToBufferInfo);
}
static PFN_vkGetDeviceBufferMemoryRequirements p_vkGetDeviceBufferMemoryRequirements;
VKAPI_ATTR void VKAPI_CALL vkGetDeviceBufferMemoryRequirements(VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
    if (!p_vkGetDeviceBufferMemoryRequirements) p_vkGetDeviceBufferMemoryRequirements = (PFN_vkGetDeviceBufferMemoryRequirements)ps5_vk_proc("vkGetDeviceBufferMemoryRequirements");
    p_vkGetDeviceBufferMemoryRequirements(device, pInfo, pMemoryRequirements);
}
static PFN_vkGetDeviceImageMemoryRequirements p_vkGetDeviceImageMemoryRequirements;
VKAPI_ATTR void VKAPI_CALL vkGetDeviceImageMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
    if (!p_vkGetDeviceImageMemoryRequirements) p_vkGetDeviceImageMemoryRequirements = (PFN_vkGetDeviceImageMemoryRequirements)ps5_vk_proc("vkGetDeviceImageMemoryRequirements");
    p_vkGetDeviceImageMemoryRequirements(device, pInfo, pMemoryRequirements);
}
static PFN_vkGetDeviceImageSparseMemoryRequirements p_vkGetDeviceImageSparseMemoryRequirements;
VKAPI_ATTR void VKAPI_CALL vkGetDeviceImageSparseMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) {
    if (!p_vkGetDeviceImageSparseMemoryRequirements) p_vkGetDeviceImageSparseMemoryRequirements = (PFN_vkGetDeviceImageSparseMemoryRequirements)ps5_vk_proc("vkGetDeviceImageSparseMemoryRequirements");
    p_vkGetDeviceImageSparseMemoryRequirements(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}
static PFN_vkCmdSetEvent2 p_vkCmdSetEvent2;
VKAPI_ATTR void VKAPI_CALL vkCmdSetEvent2(VkCommandBuffer commandBuffer, VkEvent event, const VkDependencyInfo* pDependencyInfo) {
    if (!p_vkCmdSetEvent2) p_vkCmdSetEvent2 = (PFN_vkCmdSetEvent2)ps5_vk_proc("vkCmdSetEvent2");
    p_vkCmdSetEvent2(commandBuffer, event, pDependencyInfo);
}
static PFN_vkCmdResetEvent2 p_vkCmdResetEvent2;
VKAPI_ATTR void VKAPI_CALL vkCmdResetEvent2(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2 stageMask) {
    if (!p_vkCmdResetEvent2) p_vkCmdResetEvent2 = (PFN_vkCmdResetEvent2)ps5_vk_proc("vkCmdResetEvent2");
    p_vkCmdResetEvent2(commandBuffer, event, stageMask);
}
static PFN_vkCmdWaitEvents2 p_vkCmdWaitEvents2;
VKAPI_ATTR void VKAPI_CALL vkCmdWaitEvents2(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, const VkDependencyInfo* pDependencyInfos) {
    if (!p_vkCmdWaitEvents2) p_vkCmdWaitEvents2 = (PFN_vkCmdWaitEvents2)ps5_vk_proc("vkCmdWaitEvents2");
    p_vkCmdWaitEvents2(commandBuffer, eventCount, pEvents, pDependencyInfos);
}
static PFN_vkCmdBlitImage2 p_vkCmdBlitImage2;
VKAPI_ATTR void VKAPI_CALL vkCmdBlitImage2(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo) {
    if (!p_vkCmdBlitImage2) p_vkCmdBlitImage2 = (PFN_vkCmdBlitImage2)ps5_vk_proc("vkCmdBlitImage2");
    p_vkCmdBlitImage2(commandBuffer, pBlitImageInfo);
}
static PFN_vkCmdResolveImage2 p_vkCmdResolveImage2;
VKAPI_ATTR void VKAPI_CALL vkCmdResolveImage2(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo) {
    if (!p_vkCmdResolveImage2) p_vkCmdResolveImage2 = (PFN_vkCmdResolveImage2)ps5_vk_proc("vkCmdResolveImage2");
    p_vkCmdResolveImage2(commandBuffer, pResolveImageInfo);
}
static PFN_vkCmdBeginRendering p_vkCmdBeginRendering;
VKAPI_ATTR void VKAPI_CALL vkCmdBeginRendering(VkCommandBuffer commandBuffer, const VkRenderingInfo* pRenderingInfo) {
    if (!p_vkCmdBeginRendering) p_vkCmdBeginRendering = (PFN_vkCmdBeginRendering)ps5_vk_proc("vkCmdBeginRendering");
    p_vkCmdBeginRendering(commandBuffer, pRenderingInfo);
}
static PFN_vkCmdEndRendering p_vkCmdEndRendering;
VKAPI_ATTR void VKAPI_CALL vkCmdEndRendering(VkCommandBuffer commandBuffer) {
    if (!p_vkCmdEndRendering) p_vkCmdEndRendering = (PFN_vkCmdEndRendering)ps5_vk_proc("vkCmdEndRendering");
    p_vkCmdEndRendering(commandBuffer);
}
static PFN_vkCmdSetCullMode p_vkCmdSetCullMode;
VKAPI_ATTR void VKAPI_CALL vkCmdSetCullMode(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) {
    if (!p_vkCmdSetCullMode) p_vkCmdSetCullMode = (PFN_vkCmdSetCullMode)ps5_vk_proc("vkCmdSetCullMode");
    p_vkCmdSetCullMode(commandBuffer, cullMode);
}
static PFN_vkCmdSetFrontFace p_vkCmdSetFrontFace;
VKAPI_ATTR void VKAPI_CALL vkCmdSetFrontFace(VkCommandBuffer commandBuffer, VkFrontFace frontFace) {
    if (!p_vkCmdSetFrontFace) p_vkCmdSetFrontFace = (PFN_vkCmdSetFrontFace)ps5_vk_proc("vkCmdSetFrontFace");
    p_vkCmdSetFrontFace(commandBuffer, frontFace);
}
static PFN_vkCmdSetPrimitiveTopology p_vkCmdSetPrimitiveTopology;
VKAPI_ATTR void VKAPI_CALL vkCmdSetPrimitiveTopology(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) {
    if (!p_vkCmdSetPrimitiveTopology) p_vkCmdSetPrimitiveTopology = (PFN_vkCmdSetPrimitiveTopology)ps5_vk_proc("vkCmdSetPrimitiveTopology");
    p_vkCmdSetPrimitiveTopology(commandBuffer, primitiveTopology);
}
static PFN_vkCmdSetViewportWithCount p_vkCmdSetViewportWithCount;
VKAPI_ATTR void VKAPI_CALL vkCmdSetViewportWithCount(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) {
    if (!p_vkCmdSetViewportWithCount) p_vkCmdSetViewportWithCount = (PFN_vkCmdSetViewportWithCount)ps5_vk_proc("vkCmdSetViewportWithCount");
    p_vkCmdSetViewportWithCount(commandBuffer, viewportCount, pViewports);
}
static PFN_vkCmdSetScissorWithCount p_vkCmdSetScissorWithCount;
VKAPI_ATTR void VKAPI_CALL vkCmdSetScissorWithCount(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) {
    if (!p_vkCmdSetScissorWithCount) p_vkCmdSetScissorWithCount = (PFN_vkCmdSetScissorWithCount)ps5_vk_proc("vkCmdSetScissorWithCount");
    p_vkCmdSetScissorWithCount(commandBuffer, scissorCount, pScissors);
}
static PFN_vkCmdBindVertexBuffers2 p_vkCmdBindVertexBuffers2;
VKAPI_ATTR void VKAPI_CALL vkCmdBindVertexBuffers2(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) {
    if (!p_vkCmdBindVertexBuffers2) p_vkCmdBindVertexBuffers2 = (PFN_vkCmdBindVertexBuffers2)ps5_vk_proc("vkCmdBindVertexBuffers2");
    p_vkCmdBindVertexBuffers2(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
}
static PFN_vkCmdSetDepthTestEnable p_vkCmdSetDepthTestEnable;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) {
    if (!p_vkCmdSetDepthTestEnable) p_vkCmdSetDepthTestEnable = (PFN_vkCmdSetDepthTestEnable)ps5_vk_proc("vkCmdSetDepthTestEnable");
    p_vkCmdSetDepthTestEnable(commandBuffer, depthTestEnable);
}
static PFN_vkCmdSetDepthWriteEnable p_vkCmdSetDepthWriteEnable;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthWriteEnable(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) {
    if (!p_vkCmdSetDepthWriteEnable) p_vkCmdSetDepthWriteEnable = (PFN_vkCmdSetDepthWriteEnable)ps5_vk_proc("vkCmdSetDepthWriteEnable");
    p_vkCmdSetDepthWriteEnable(commandBuffer, depthWriteEnable);
}
static PFN_vkCmdSetDepthCompareOp p_vkCmdSetDepthCompareOp;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthCompareOp(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) {
    if (!p_vkCmdSetDepthCompareOp) p_vkCmdSetDepthCompareOp = (PFN_vkCmdSetDepthCompareOp)ps5_vk_proc("vkCmdSetDepthCompareOp");
    p_vkCmdSetDepthCompareOp(commandBuffer, depthCompareOp);
}
static PFN_vkCmdSetDepthBoundsTestEnable p_vkCmdSetDepthBoundsTestEnable;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthBoundsTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) {
    if (!p_vkCmdSetDepthBoundsTestEnable) p_vkCmdSetDepthBoundsTestEnable = (PFN_vkCmdSetDepthBoundsTestEnable)ps5_vk_proc("vkCmdSetDepthBoundsTestEnable");
    p_vkCmdSetDepthBoundsTestEnable(commandBuffer, depthBoundsTestEnable);
}
static PFN_vkCmdSetStencilTestEnable p_vkCmdSetStencilTestEnable;
VKAPI_ATTR void VKAPI_CALL vkCmdSetStencilTestEnable(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) {
    if (!p_vkCmdSetStencilTestEnable) p_vkCmdSetStencilTestEnable = (PFN_vkCmdSetStencilTestEnable)ps5_vk_proc("vkCmdSetStencilTestEnable");
    p_vkCmdSetStencilTestEnable(commandBuffer, stencilTestEnable);
}
static PFN_vkCmdSetStencilOp p_vkCmdSetStencilOp;
VKAPI_ATTR void VKAPI_CALL vkCmdSetStencilOp(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) {
    if (!p_vkCmdSetStencilOp) p_vkCmdSetStencilOp = (PFN_vkCmdSetStencilOp)ps5_vk_proc("vkCmdSetStencilOp");
    p_vkCmdSetStencilOp(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}
static PFN_vkCmdSetRasterizerDiscardEnable p_vkCmdSetRasterizerDiscardEnable;
VKAPI_ATTR void VKAPI_CALL vkCmdSetRasterizerDiscardEnable(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) {
    if (!p_vkCmdSetRasterizerDiscardEnable) p_vkCmdSetRasterizerDiscardEnable = (PFN_vkCmdSetRasterizerDiscardEnable)ps5_vk_proc("vkCmdSetRasterizerDiscardEnable");
    p_vkCmdSetRasterizerDiscardEnable(commandBuffer, rasterizerDiscardEnable);
}
static PFN_vkCmdSetDepthBiasEnable p_vkCmdSetDepthBiasEnable;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthBiasEnable(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) {
    if (!p_vkCmdSetDepthBiasEnable) p_vkCmdSetDepthBiasEnable = (PFN_vkCmdSetDepthBiasEnable)ps5_vk_proc("vkCmdSetDepthBiasEnable");
    p_vkCmdSetDepthBiasEnable(commandBuffer, depthBiasEnable);
}
static PFN_vkCmdSetPrimitiveRestartEnable p_vkCmdSetPrimitiveRestartEnable;
VKAPI_ATTR void VKAPI_CALL vkCmdSetPrimitiveRestartEnable(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) {
    if (!p_vkCmdSetPrimitiveRestartEnable) p_vkCmdSetPrimitiveRestartEnable = (PFN_vkCmdSetPrimitiveRestartEnable)ps5_vk_proc("vkCmdSetPrimitiveRestartEnable");
    p_vkCmdSetPrimitiveRestartEnable(commandBuffer, primitiveRestartEnable);
}
static PFN_vkMapMemory2 p_vkMapMemory2;
VKAPI_ATTR VkResult VKAPI_CALL vkMapMemory2(VkDevice device, const VkMemoryMapInfo* pMemoryMapInfo, void** ppData) {
    if (!p_vkMapMemory2) p_vkMapMemory2 = (PFN_vkMapMemory2)ps5_vk_proc("vkMapMemory2");
    return p_vkMapMemory2(device, pMemoryMapInfo, ppData);
}
static PFN_vkUnmapMemory2 p_vkUnmapMemory2;
VKAPI_ATTR VkResult VKAPI_CALL vkUnmapMemory2(VkDevice device, const VkMemoryUnmapInfo* pMemoryUnmapInfo) {
    if (!p_vkUnmapMemory2) p_vkUnmapMemory2 = (PFN_vkUnmapMemory2)ps5_vk_proc("vkUnmapMemory2");
    return p_vkUnmapMemory2(device, pMemoryUnmapInfo);
}
static PFN_vkGetDeviceImageSubresourceLayout p_vkGetDeviceImageSubresourceLayout;
VKAPI_ATTR void VKAPI_CALL vkGetDeviceImageSubresourceLayout(VkDevice device, const VkDeviceImageSubresourceInfo* pInfo, VkSubresourceLayout2* pLayout) {
    if (!p_vkGetDeviceImageSubresourceLayout) p_vkGetDeviceImageSubresourceLayout = (PFN_vkGetDeviceImageSubresourceLayout)ps5_vk_proc("vkGetDeviceImageSubresourceLayout");
    p_vkGetDeviceImageSubresourceLayout(device, pInfo, pLayout);
}
static PFN_vkGetImageSubresourceLayout2 p_vkGetImageSubresourceLayout2;
VKAPI_ATTR void VKAPI_CALL vkGetImageSubresourceLayout2(VkDevice device, VkImage image, const VkImageSubresource2* pSubresource, VkSubresourceLayout2* pLayout) {
    if (!p_vkGetImageSubresourceLayout2) p_vkGetImageSubresourceLayout2 = (PFN_vkGetImageSubresourceLayout2)ps5_vk_proc("vkGetImageSubresourceLayout2");
    p_vkGetImageSubresourceLayout2(device, image, pSubresource, pLayout);
}
static PFN_vkCopyMemoryToImage p_vkCopyMemoryToImage;
VKAPI_ATTR VkResult VKAPI_CALL vkCopyMemoryToImage(VkDevice device, const VkCopyMemoryToImageInfo* pCopyMemoryToImageInfo) {
    if (!p_vkCopyMemoryToImage) p_vkCopyMemoryToImage = (PFN_vkCopyMemoryToImage)ps5_vk_proc("vkCopyMemoryToImage");
    return p_vkCopyMemoryToImage(device, pCopyMemoryToImageInfo);
}
static PFN_vkCopyImageToMemory p_vkCopyImageToMemory;
VKAPI_ATTR VkResult VKAPI_CALL vkCopyImageToMemory(VkDevice device, const VkCopyImageToMemoryInfo* pCopyImageToMemoryInfo) {
    if (!p_vkCopyImageToMemory) p_vkCopyImageToMemory = (PFN_vkCopyImageToMemory)ps5_vk_proc("vkCopyImageToMemory");
    return p_vkCopyImageToMemory(device, pCopyImageToMemoryInfo);
}
static PFN_vkCopyImageToImage p_vkCopyImageToImage;
VKAPI_ATTR VkResult VKAPI_CALL vkCopyImageToImage(VkDevice device, const VkCopyImageToImageInfo* pCopyImageToImageInfo) {
    if (!p_vkCopyImageToImage) p_vkCopyImageToImage = (PFN_vkCopyImageToImage)ps5_vk_proc("vkCopyImageToImage");
    return p_vkCopyImageToImage(device, pCopyImageToImageInfo);
}
static PFN_vkTransitionImageLayout p_vkTransitionImageLayout;
VKAPI_ATTR VkResult VKAPI_CALL vkTransitionImageLayout(VkDevice device, uint32_t transitionCount, const VkHostImageLayoutTransitionInfo* pTransitions) {
    if (!p_vkTransitionImageLayout) p_vkTransitionImageLayout = (PFN_vkTransitionImageLayout)ps5_vk_proc("vkTransitionImageLayout");
    return p_vkTransitionImageLayout(device, transitionCount, pTransitions);
}
static PFN_vkCmdPushDescriptorSet p_vkCmdPushDescriptorSet;
VKAPI_ATTR void VKAPI_CALL vkCmdPushDescriptorSet(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) {
    if (!p_vkCmdPushDescriptorSet) p_vkCmdPushDescriptorSet = (PFN_vkCmdPushDescriptorSet)ps5_vk_proc("vkCmdPushDescriptorSet");
    p_vkCmdPushDescriptorSet(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
}
static PFN_vkCmdPushDescriptorSetWithTemplate p_vkCmdPushDescriptorSetWithTemplate;
VKAPI_ATTR void VKAPI_CALL vkCmdPushDescriptorSetWithTemplate(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) {
    if (!p_vkCmdPushDescriptorSetWithTemplate) p_vkCmdPushDescriptorSetWithTemplate = (PFN_vkCmdPushDescriptorSetWithTemplate)ps5_vk_proc("vkCmdPushDescriptorSetWithTemplate");
    p_vkCmdPushDescriptorSetWithTemplate(commandBuffer, descriptorUpdateTemplate, layout, set, pData);
}
static PFN_vkCmdBindDescriptorSets2 p_vkCmdBindDescriptorSets2;
VKAPI_ATTR void VKAPI_CALL vkCmdBindDescriptorSets2(VkCommandBuffer commandBuffer, const VkBindDescriptorSetsInfo* pBindDescriptorSetsInfo) {
    if (!p_vkCmdBindDescriptorSets2) p_vkCmdBindDescriptorSets2 = (PFN_vkCmdBindDescriptorSets2)ps5_vk_proc("vkCmdBindDescriptorSets2");
    p_vkCmdBindDescriptorSets2(commandBuffer, pBindDescriptorSetsInfo);
}
static PFN_vkCmdPushConstants2 p_vkCmdPushConstants2;
VKAPI_ATTR void VKAPI_CALL vkCmdPushConstants2(VkCommandBuffer commandBuffer, const VkPushConstantsInfo* pPushConstantsInfo) {
    if (!p_vkCmdPushConstants2) p_vkCmdPushConstants2 = (PFN_vkCmdPushConstants2)ps5_vk_proc("vkCmdPushConstants2");
    p_vkCmdPushConstants2(commandBuffer, pPushConstantsInfo);
}
static PFN_vkCmdPushDescriptorSet2 p_vkCmdPushDescriptorSet2;
VKAPI_ATTR void VKAPI_CALL vkCmdPushDescriptorSet2(VkCommandBuffer commandBuffer, const VkPushDescriptorSetInfo* pPushDescriptorSetInfo) {
    if (!p_vkCmdPushDescriptorSet2) p_vkCmdPushDescriptorSet2 = (PFN_vkCmdPushDescriptorSet2)ps5_vk_proc("vkCmdPushDescriptorSet2");
    p_vkCmdPushDescriptorSet2(commandBuffer, pPushDescriptorSetInfo);
}
static PFN_vkCmdPushDescriptorSetWithTemplate2 p_vkCmdPushDescriptorSetWithTemplate2;
VKAPI_ATTR void VKAPI_CALL vkCmdPushDescriptorSetWithTemplate2(VkCommandBuffer commandBuffer, const VkPushDescriptorSetWithTemplateInfo* pPushDescriptorSetWithTemplateInfo) {
    if (!p_vkCmdPushDescriptorSetWithTemplate2) p_vkCmdPushDescriptorSetWithTemplate2 = (PFN_vkCmdPushDescriptorSetWithTemplate2)ps5_vk_proc("vkCmdPushDescriptorSetWithTemplate2");
    p_vkCmdPushDescriptorSetWithTemplate2(commandBuffer, pPushDescriptorSetWithTemplateInfo);
}
static PFN_vkCmdSetLineStipple p_vkCmdSetLineStipple;
VKAPI_ATTR void VKAPI_CALL vkCmdSetLineStipple(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) {
    if (!p_vkCmdSetLineStipple) p_vkCmdSetLineStipple = (PFN_vkCmdSetLineStipple)ps5_vk_proc("vkCmdSetLineStipple");
    p_vkCmdSetLineStipple(commandBuffer, lineStippleFactor, lineStipplePattern);
}
static PFN_vkCmdBindIndexBuffer2 p_vkCmdBindIndexBuffer2;
VKAPI_ATTR void VKAPI_CALL vkCmdBindIndexBuffer2(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType) {
    if (!p_vkCmdBindIndexBuffer2) p_vkCmdBindIndexBuffer2 = (PFN_vkCmdBindIndexBuffer2)ps5_vk_proc("vkCmdBindIndexBuffer2");
    p_vkCmdBindIndexBuffer2(commandBuffer, buffer, offset, size, indexType);
}
static PFN_vkGetRenderingAreaGranularity p_vkGetRenderingAreaGranularity;
VKAPI_ATTR void VKAPI_CALL vkGetRenderingAreaGranularity(VkDevice device, const VkRenderingAreaInfo* pRenderingAreaInfo, VkExtent2D* pGranularity) {
    if (!p_vkGetRenderingAreaGranularity) p_vkGetRenderingAreaGranularity = (PFN_vkGetRenderingAreaGranularity)ps5_vk_proc("vkGetRenderingAreaGranularity");
    p_vkGetRenderingAreaGranularity(device, pRenderingAreaInfo, pGranularity);
}
static PFN_vkCmdSetRenderingAttachmentLocations p_vkCmdSetRenderingAttachmentLocations;
VKAPI_ATTR void VKAPI_CALL vkCmdSetRenderingAttachmentLocations(VkCommandBuffer commandBuffer, const VkRenderingAttachmentLocationInfo* pLocationInfo) {
    if (!p_vkCmdSetRenderingAttachmentLocations) p_vkCmdSetRenderingAttachmentLocations = (PFN_vkCmdSetRenderingAttachmentLocations)ps5_vk_proc("vkCmdSetRenderingAttachmentLocations");
    p_vkCmdSetRenderingAttachmentLocations(commandBuffer, pLocationInfo);
}
static PFN_vkCmdSetRenderingInputAttachmentIndices p_vkCmdSetRenderingInputAttachmentIndices;
VKAPI_ATTR void VKAPI_CALL vkCmdSetRenderingInputAttachmentIndices(VkCommandBuffer commandBuffer, const VkRenderingInputAttachmentIndexInfo* pInputAttachmentIndexInfo) {
    if (!p_vkCmdSetRenderingInputAttachmentIndices) p_vkCmdSetRenderingInputAttachmentIndices = (PFN_vkCmdSetRenderingInputAttachmentIndices)ps5_vk_proc("vkCmdSetRenderingInputAttachmentIndices");
    p_vkCmdSetRenderingInputAttachmentIndices(commandBuffer, pInputAttachmentIndexInfo);
}
static PFN_vkDestroySurfaceKHR p_vkDestroySurfaceKHR;
VKAPI_ATTR void VKAPI_CALL vkDestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroySurfaceKHR) p_vkDestroySurfaceKHR = (PFN_vkDestroySurfaceKHR)ps5_vk_proc("vkDestroySurfaceKHR");
    p_vkDestroySurfaceKHR(instance, surface, pAllocator);
}
static PFN_vkGetPhysicalDeviceSurfaceSupportKHR p_vkGetPhysicalDeviceSurfaceSupportKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported) {
    if (!p_vkGetPhysicalDeviceSurfaceSupportKHR) p_vkGetPhysicalDeviceSurfaceSupportKHR = (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)ps5_vk_proc("vkGetPhysicalDeviceSurfaceSupportKHR");
    return p_vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, pSupported);
}
static PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR p_vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities) {
    if (!p_vkGetPhysicalDeviceSurfaceCapabilitiesKHR) p_vkGetPhysicalDeviceSurfaceCapabilitiesKHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)ps5_vk_proc("vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
    return p_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, pSurfaceCapabilities);
}
static PFN_vkGetPhysicalDeviceSurfaceFormatsKHR p_vkGetPhysicalDeviceSurfaceFormatsKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats) {
    if (!p_vkGetPhysicalDeviceSurfaceFormatsKHR) p_vkGetPhysicalDeviceSurfaceFormatsKHR = (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)ps5_vk_proc("vkGetPhysicalDeviceSurfaceFormatsKHR");
    return p_vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
}
static PFN_vkGetPhysicalDeviceSurfacePresentModesKHR p_vkGetPhysicalDeviceSurfacePresentModesKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes) {
    if (!p_vkGetPhysicalDeviceSurfacePresentModesKHR) p_vkGetPhysicalDeviceSurfacePresentModesKHR = (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)ps5_vk_proc("vkGetPhysicalDeviceSurfacePresentModesKHR");
    return p_vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, pPresentModeCount, pPresentModes);
}
static PFN_vkCreateSwapchainKHR p_vkCreateSwapchainKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) {
    if (!p_vkCreateSwapchainKHR) p_vkCreateSwapchainKHR = (PFN_vkCreateSwapchainKHR)ps5_vk_proc("vkCreateSwapchainKHR");
    return p_vkCreateSwapchainKHR(device, pCreateInfo, pAllocator, pSwapchain);
}
static PFN_vkDestroySwapchainKHR p_vkDestroySwapchainKHR;
VKAPI_ATTR void VKAPI_CALL vkDestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroySwapchainKHR) p_vkDestroySwapchainKHR = (PFN_vkDestroySwapchainKHR)ps5_vk_proc("vkDestroySwapchainKHR");
    p_vkDestroySwapchainKHR(device, swapchain, pAllocator);
}
static PFN_vkGetSwapchainImagesKHR p_vkGetSwapchainImagesKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) {
    if (!p_vkGetSwapchainImagesKHR) p_vkGetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR)ps5_vk_proc("vkGetSwapchainImagesKHR");
    return p_vkGetSwapchainImagesKHR(device, swapchain, pSwapchainImageCount, pSwapchainImages);
}
static PFN_vkAcquireNextImageKHR p_vkAcquireNextImageKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkAcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) {
    if (!p_vkAcquireNextImageKHR) p_vkAcquireNextImageKHR = (PFN_vkAcquireNextImageKHR)ps5_vk_proc("vkAcquireNextImageKHR");
    return p_vkAcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, pImageIndex);
}
static PFN_vkQueuePresentKHR p_vkQueuePresentKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) {
    if (!p_vkQueuePresentKHR) p_vkQueuePresentKHR = (PFN_vkQueuePresentKHR)ps5_vk_proc("vkQueuePresentKHR");
    return p_vkQueuePresentKHR(queue, pPresentInfo);
}
static PFN_vkGetDeviceGroupPresentCapabilitiesKHR p_vkGetDeviceGroupPresentCapabilitiesKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) {
    if (!p_vkGetDeviceGroupPresentCapabilitiesKHR) p_vkGetDeviceGroupPresentCapabilitiesKHR = (PFN_vkGetDeviceGroupPresentCapabilitiesKHR)ps5_vk_proc("vkGetDeviceGroupPresentCapabilitiesKHR");
    return p_vkGetDeviceGroupPresentCapabilitiesKHR(device, pDeviceGroupPresentCapabilities);
}
static PFN_vkGetDeviceGroupSurfacePresentModesKHR p_vkGetDeviceGroupSurfacePresentModesKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes) {
    if (!p_vkGetDeviceGroupSurfacePresentModesKHR) p_vkGetDeviceGroupSurfacePresentModesKHR = (PFN_vkGetDeviceGroupSurfacePresentModesKHR)ps5_vk_proc("vkGetDeviceGroupSurfacePresentModesKHR");
    return p_vkGetDeviceGroupSurfacePresentModesKHR(device, surface, pModes);
}
static PFN_vkGetPhysicalDevicePresentRectanglesKHR p_vkGetPhysicalDevicePresentRectanglesKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDevicePresentRectanglesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects) {
    if (!p_vkGetPhysicalDevicePresentRectanglesKHR) p_vkGetPhysicalDevicePresentRectanglesKHR = (PFN_vkGetPhysicalDevicePresentRectanglesKHR)ps5_vk_proc("vkGetPhysicalDevicePresentRectanglesKHR");
    return p_vkGetPhysicalDevicePresentRectanglesKHR(physicalDevice, surface, pRectCount, pRects);
}
static PFN_vkAcquireNextImage2KHR p_vkAcquireNextImage2KHR;
VKAPI_ATTR VkResult VKAPI_CALL vkAcquireNextImage2KHR(VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) {
    if (!p_vkAcquireNextImage2KHR) p_vkAcquireNextImage2KHR = (PFN_vkAcquireNextImage2KHR)ps5_vk_proc("vkAcquireNextImage2KHR");
    return p_vkAcquireNextImage2KHR(device, pAcquireInfo, pImageIndex);
}
static PFN_vkGetPhysicalDeviceDisplayPropertiesKHR p_vkGetPhysicalDeviceDisplayPropertiesKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties) {
    if (!p_vkGetPhysicalDeviceDisplayPropertiesKHR) p_vkGetPhysicalDeviceDisplayPropertiesKHR = (PFN_vkGetPhysicalDeviceDisplayPropertiesKHR)ps5_vk_proc("vkGetPhysicalDeviceDisplayPropertiesKHR");
    return p_vkGetPhysicalDeviceDisplayPropertiesKHR(physicalDevice, pPropertyCount, pProperties);
}
static PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR p_vkGetPhysicalDeviceDisplayPlanePropertiesKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties) {
    if (!p_vkGetPhysicalDeviceDisplayPlanePropertiesKHR) p_vkGetPhysicalDeviceDisplayPlanePropertiesKHR = (PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR)ps5_vk_proc("vkGetPhysicalDeviceDisplayPlanePropertiesKHR");
    return p_vkGetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, pPropertyCount, pProperties);
}
static PFN_vkGetDisplayPlaneSupportedDisplaysKHR p_vkGetDisplayPlaneSupportedDisplaysKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays) {
    if (!p_vkGetDisplayPlaneSupportedDisplaysKHR) p_vkGetDisplayPlaneSupportedDisplaysKHR = (PFN_vkGetDisplayPlaneSupportedDisplaysKHR)ps5_vk_proc("vkGetDisplayPlaneSupportedDisplaysKHR");
    return p_vkGetDisplayPlaneSupportedDisplaysKHR(physicalDevice, planeIndex, pDisplayCount, pDisplays);
}
static PFN_vkGetDisplayModePropertiesKHR p_vkGetDisplayModePropertiesKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetDisplayModePropertiesKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties) {
    if (!p_vkGetDisplayModePropertiesKHR) p_vkGetDisplayModePropertiesKHR = (PFN_vkGetDisplayModePropertiesKHR)ps5_vk_proc("vkGetDisplayModePropertiesKHR");
    return p_vkGetDisplayModePropertiesKHR(physicalDevice, display, pPropertyCount, pProperties);
}
static PFN_vkCreateDisplayModeKHR p_vkCreateDisplayModeKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDisplayModeKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode) {
    if (!p_vkCreateDisplayModeKHR) p_vkCreateDisplayModeKHR = (PFN_vkCreateDisplayModeKHR)ps5_vk_proc("vkCreateDisplayModeKHR");
    return p_vkCreateDisplayModeKHR(physicalDevice, display, pCreateInfo, pAllocator, pMode);
}
static PFN_vkGetDisplayPlaneCapabilitiesKHR p_vkGetDisplayPlaneCapabilitiesKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities) {
    if (!p_vkGetDisplayPlaneCapabilitiesKHR) p_vkGetDisplayPlaneCapabilitiesKHR = (PFN_vkGetDisplayPlaneCapabilitiesKHR)ps5_vk_proc("vkGetDisplayPlaneCapabilitiesKHR");
    return p_vkGetDisplayPlaneCapabilitiesKHR(physicalDevice, mode, planeIndex, pCapabilities);
}
static PFN_vkCreateDisplayPlaneSurfaceKHR p_vkCreateDisplayPlaneSurfaceKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDisplayPlaneSurfaceKHR(VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
    if (!p_vkCreateDisplayPlaneSurfaceKHR) p_vkCreateDisplayPlaneSurfaceKHR = (PFN_vkCreateDisplayPlaneSurfaceKHR)ps5_vk_proc("vkCreateDisplayPlaneSurfaceKHR");
    return p_vkCreateDisplayPlaneSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
}
static PFN_vkCreateSharedSwapchainsKHR p_vkCreateSharedSwapchainsKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateSharedSwapchainsKHR(VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains) {
    if (!p_vkCreateSharedSwapchainsKHR) p_vkCreateSharedSwapchainsKHR = (PFN_vkCreateSharedSwapchainsKHR)ps5_vk_proc("vkCreateSharedSwapchainsKHR");
    return p_vkCreateSharedSwapchainsKHR(device, swapchainCount, pCreateInfos, pAllocator, pSwapchains);
}
static PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR p_vkGetPhysicalDeviceVideoCapabilitiesKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceVideoCapabilitiesKHR(VkPhysicalDevice physicalDevice, const VkVideoProfileInfoKHR* pVideoProfile, VkVideoCapabilitiesKHR* pCapabilities) {
    if (!p_vkGetPhysicalDeviceVideoCapabilitiesKHR) p_vkGetPhysicalDeviceVideoCapabilitiesKHR = (PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR)ps5_vk_proc("vkGetPhysicalDeviceVideoCapabilitiesKHR");
    return p_vkGetPhysicalDeviceVideoCapabilitiesKHR(physicalDevice, pVideoProfile, pCapabilities);
}
static PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR p_vkGetPhysicalDeviceVideoFormatPropertiesKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceVideoFormatPropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceVideoFormatInfoKHR* pVideoFormatInfo, uint32_t* pVideoFormatPropertyCount, VkVideoFormatPropertiesKHR* pVideoFormatProperties) {
    if (!p_vkGetPhysicalDeviceVideoFormatPropertiesKHR) p_vkGetPhysicalDeviceVideoFormatPropertiesKHR = (PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR)ps5_vk_proc("vkGetPhysicalDeviceVideoFormatPropertiesKHR");
    return p_vkGetPhysicalDeviceVideoFormatPropertiesKHR(physicalDevice, pVideoFormatInfo, pVideoFormatPropertyCount, pVideoFormatProperties);
}
static PFN_vkCreateVideoSessionKHR p_vkCreateVideoSessionKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateVideoSessionKHR(VkDevice device, const VkVideoSessionCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionKHR* pVideoSession) {
    if (!p_vkCreateVideoSessionKHR) p_vkCreateVideoSessionKHR = (PFN_vkCreateVideoSessionKHR)ps5_vk_proc("vkCreateVideoSessionKHR");
    return p_vkCreateVideoSessionKHR(device, pCreateInfo, pAllocator, pVideoSession);
}
static PFN_vkDestroyVideoSessionKHR p_vkDestroyVideoSessionKHR;
VKAPI_ATTR void VKAPI_CALL vkDestroyVideoSessionKHR(VkDevice device, VkVideoSessionKHR videoSession, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyVideoSessionKHR) p_vkDestroyVideoSessionKHR = (PFN_vkDestroyVideoSessionKHR)ps5_vk_proc("vkDestroyVideoSessionKHR");
    p_vkDestroyVideoSessionKHR(device, videoSession, pAllocator);
}
static PFN_vkGetVideoSessionMemoryRequirementsKHR p_vkGetVideoSessionMemoryRequirementsKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetVideoSessionMemoryRequirementsKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t* pMemoryRequirementsCount, VkVideoSessionMemoryRequirementsKHR* pMemoryRequirements) {
    if (!p_vkGetVideoSessionMemoryRequirementsKHR) p_vkGetVideoSessionMemoryRequirementsKHR = (PFN_vkGetVideoSessionMemoryRequirementsKHR)ps5_vk_proc("vkGetVideoSessionMemoryRequirementsKHR");
    return p_vkGetVideoSessionMemoryRequirementsKHR(device, videoSession, pMemoryRequirementsCount, pMemoryRequirements);
}
static PFN_vkBindVideoSessionMemoryKHR p_vkBindVideoSessionMemoryKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkBindVideoSessionMemoryKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t bindSessionMemoryInfoCount, const VkBindVideoSessionMemoryInfoKHR* pBindSessionMemoryInfos) {
    if (!p_vkBindVideoSessionMemoryKHR) p_vkBindVideoSessionMemoryKHR = (PFN_vkBindVideoSessionMemoryKHR)ps5_vk_proc("vkBindVideoSessionMemoryKHR");
    return p_vkBindVideoSessionMemoryKHR(device, videoSession, bindSessionMemoryInfoCount, pBindSessionMemoryInfos);
}
static PFN_vkCreateVideoSessionParametersKHR p_vkCreateVideoSessionParametersKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateVideoSessionParametersKHR(VkDevice device, const VkVideoSessionParametersCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionParametersKHR* pVideoSessionParameters) {
    if (!p_vkCreateVideoSessionParametersKHR) p_vkCreateVideoSessionParametersKHR = (PFN_vkCreateVideoSessionParametersKHR)ps5_vk_proc("vkCreateVideoSessionParametersKHR");
    return p_vkCreateVideoSessionParametersKHR(device, pCreateInfo, pAllocator, pVideoSessionParameters);
}
static PFN_vkUpdateVideoSessionParametersKHR p_vkUpdateVideoSessionParametersKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkUpdateVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkVideoSessionParametersUpdateInfoKHR* pUpdateInfo) {
    if (!p_vkUpdateVideoSessionParametersKHR) p_vkUpdateVideoSessionParametersKHR = (PFN_vkUpdateVideoSessionParametersKHR)ps5_vk_proc("vkUpdateVideoSessionParametersKHR");
    return p_vkUpdateVideoSessionParametersKHR(device, videoSessionParameters, pUpdateInfo);
}
static PFN_vkDestroyVideoSessionParametersKHR p_vkDestroyVideoSessionParametersKHR;
VKAPI_ATTR void VKAPI_CALL vkDestroyVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyVideoSessionParametersKHR) p_vkDestroyVideoSessionParametersKHR = (PFN_vkDestroyVideoSessionParametersKHR)ps5_vk_proc("vkDestroyVideoSessionParametersKHR");
    p_vkDestroyVideoSessionParametersKHR(device, videoSessionParameters, pAllocator);
}
static PFN_vkCmdBeginVideoCodingKHR p_vkCmdBeginVideoCodingKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdBeginVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoBeginCodingInfoKHR* pBeginInfo) {
    if (!p_vkCmdBeginVideoCodingKHR) p_vkCmdBeginVideoCodingKHR = (PFN_vkCmdBeginVideoCodingKHR)ps5_vk_proc("vkCmdBeginVideoCodingKHR");
    p_vkCmdBeginVideoCodingKHR(commandBuffer, pBeginInfo);
}
static PFN_vkCmdEndVideoCodingKHR p_vkCmdEndVideoCodingKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdEndVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoEndCodingInfoKHR* pEndCodingInfo) {
    if (!p_vkCmdEndVideoCodingKHR) p_vkCmdEndVideoCodingKHR = (PFN_vkCmdEndVideoCodingKHR)ps5_vk_proc("vkCmdEndVideoCodingKHR");
    p_vkCmdEndVideoCodingKHR(commandBuffer, pEndCodingInfo);
}
static PFN_vkCmdControlVideoCodingKHR p_vkCmdControlVideoCodingKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdControlVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoCodingControlInfoKHR* pCodingControlInfo) {
    if (!p_vkCmdControlVideoCodingKHR) p_vkCmdControlVideoCodingKHR = (PFN_vkCmdControlVideoCodingKHR)ps5_vk_proc("vkCmdControlVideoCodingKHR");
    p_vkCmdControlVideoCodingKHR(commandBuffer, pCodingControlInfo);
}
static PFN_vkCmdDecodeVideoKHR p_vkCmdDecodeVideoKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdDecodeVideoKHR(VkCommandBuffer commandBuffer, const VkVideoDecodeInfoKHR* pDecodeInfo) {
    if (!p_vkCmdDecodeVideoKHR) p_vkCmdDecodeVideoKHR = (PFN_vkCmdDecodeVideoKHR)ps5_vk_proc("vkCmdDecodeVideoKHR");
    p_vkCmdDecodeVideoKHR(commandBuffer, pDecodeInfo);
}
static PFN_vkCmdBeginRenderingKHR p_vkCmdBeginRenderingKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdBeginRenderingKHR(VkCommandBuffer commandBuffer, const VkRenderingInfo* pRenderingInfo) {
    if (!p_vkCmdBeginRenderingKHR) p_vkCmdBeginRenderingKHR = (PFN_vkCmdBeginRenderingKHR)ps5_vk_proc("vkCmdBeginRenderingKHR");
    p_vkCmdBeginRenderingKHR(commandBuffer, pRenderingInfo);
}
static PFN_vkCmdEndRenderingKHR p_vkCmdEndRenderingKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdEndRenderingKHR(VkCommandBuffer commandBuffer) {
    if (!p_vkCmdEndRenderingKHR) p_vkCmdEndRenderingKHR = (PFN_vkCmdEndRenderingKHR)ps5_vk_proc("vkCmdEndRenderingKHR");
    p_vkCmdEndRenderingKHR(commandBuffer);
}
static PFN_vkGetPhysicalDeviceFeatures2KHR p_vkGetPhysicalDeviceFeatures2KHR;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFeatures2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures) {
    if (!p_vkGetPhysicalDeviceFeatures2KHR) p_vkGetPhysicalDeviceFeatures2KHR = (PFN_vkGetPhysicalDeviceFeatures2KHR)ps5_vk_proc("vkGetPhysicalDeviceFeatures2KHR");
    p_vkGetPhysicalDeviceFeatures2KHR(physicalDevice, pFeatures);
}
static PFN_vkGetPhysicalDeviceProperties2KHR p_vkGetPhysicalDeviceProperties2KHR;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties) {
    if (!p_vkGetPhysicalDeviceProperties2KHR) p_vkGetPhysicalDeviceProperties2KHR = (PFN_vkGetPhysicalDeviceProperties2KHR)ps5_vk_proc("vkGetPhysicalDeviceProperties2KHR");
    p_vkGetPhysicalDeviceProperties2KHR(physicalDevice, pProperties);
}
static PFN_vkGetPhysicalDeviceFormatProperties2KHR p_vkGetPhysicalDeviceFormatProperties2KHR;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFormatProperties2KHR(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties) {
    if (!p_vkGetPhysicalDeviceFormatProperties2KHR) p_vkGetPhysicalDeviceFormatProperties2KHR = (PFN_vkGetPhysicalDeviceFormatProperties2KHR)ps5_vk_proc("vkGetPhysicalDeviceFormatProperties2KHR");
    p_vkGetPhysicalDeviceFormatProperties2KHR(physicalDevice, format, pFormatProperties);
}
static PFN_vkGetPhysicalDeviceImageFormatProperties2KHR p_vkGetPhysicalDeviceImageFormatProperties2KHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties) {
    if (!p_vkGetPhysicalDeviceImageFormatProperties2KHR) p_vkGetPhysicalDeviceImageFormatProperties2KHR = (PFN_vkGetPhysicalDeviceImageFormatProperties2KHR)ps5_vk_proc("vkGetPhysicalDeviceImageFormatProperties2KHR");
    return p_vkGetPhysicalDeviceImageFormatProperties2KHR(physicalDevice, pImageFormatInfo, pImageFormatProperties);
}
static PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR p_vkGetPhysicalDeviceQueueFamilyProperties2KHR;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties) {
    if (!p_vkGetPhysicalDeviceQueueFamilyProperties2KHR) p_vkGetPhysicalDeviceQueueFamilyProperties2KHR = (PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR)ps5_vk_proc("vkGetPhysicalDeviceQueueFamilyProperties2KHR");
    p_vkGetPhysicalDeviceQueueFamilyProperties2KHR(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}
static PFN_vkGetPhysicalDeviceMemoryProperties2KHR p_vkGetPhysicalDeviceMemoryProperties2KHR;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties) {
    if (!p_vkGetPhysicalDeviceMemoryProperties2KHR) p_vkGetPhysicalDeviceMemoryProperties2KHR = (PFN_vkGetPhysicalDeviceMemoryProperties2KHR)ps5_vk_proc("vkGetPhysicalDeviceMemoryProperties2KHR");
    p_vkGetPhysicalDeviceMemoryProperties2KHR(physicalDevice, pMemoryProperties);
}
static PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR p_vkGetPhysicalDeviceSparseImageFormatProperties2KHR;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties) {
    if (!p_vkGetPhysicalDeviceSparseImageFormatProperties2KHR) p_vkGetPhysicalDeviceSparseImageFormatProperties2KHR = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR)ps5_vk_proc("vkGetPhysicalDeviceSparseImageFormatProperties2KHR");
    p_vkGetPhysicalDeviceSparseImageFormatProperties2KHR(physicalDevice, pFormatInfo, pPropertyCount, pProperties);
}
static PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR p_vkGetDeviceGroupPeerMemoryFeaturesKHR;
VKAPI_ATTR void VKAPI_CALL vkGetDeviceGroupPeerMemoryFeaturesKHR(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) {
    if (!p_vkGetDeviceGroupPeerMemoryFeaturesKHR) p_vkGetDeviceGroupPeerMemoryFeaturesKHR = (PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR)ps5_vk_proc("vkGetDeviceGroupPeerMemoryFeaturesKHR");
    p_vkGetDeviceGroupPeerMemoryFeaturesKHR(device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
}
static PFN_vkCmdSetDeviceMaskKHR p_vkCmdSetDeviceMaskKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDeviceMaskKHR(VkCommandBuffer commandBuffer, uint32_t deviceMask) {
    if (!p_vkCmdSetDeviceMaskKHR) p_vkCmdSetDeviceMaskKHR = (PFN_vkCmdSetDeviceMaskKHR)ps5_vk_proc("vkCmdSetDeviceMaskKHR");
    p_vkCmdSetDeviceMaskKHR(commandBuffer, deviceMask);
}
static PFN_vkCmdDispatchBaseKHR p_vkCmdDispatchBaseKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdDispatchBaseKHR(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
    if (!p_vkCmdDispatchBaseKHR) p_vkCmdDispatchBaseKHR = (PFN_vkCmdDispatchBaseKHR)ps5_vk_proc("vkCmdDispatchBaseKHR");
    p_vkCmdDispatchBaseKHR(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}
static PFN_vkTrimCommandPoolKHR p_vkTrimCommandPoolKHR;
VKAPI_ATTR void VKAPI_CALL vkTrimCommandPoolKHR(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags) {
    if (!p_vkTrimCommandPoolKHR) p_vkTrimCommandPoolKHR = (PFN_vkTrimCommandPoolKHR)ps5_vk_proc("vkTrimCommandPoolKHR");
    p_vkTrimCommandPoolKHR(device, commandPool, flags);
}
static PFN_vkEnumeratePhysicalDeviceGroupsKHR p_vkEnumeratePhysicalDeviceGroupsKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkEnumeratePhysicalDeviceGroupsKHR(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties) {
    if (!p_vkEnumeratePhysicalDeviceGroupsKHR) p_vkEnumeratePhysicalDeviceGroupsKHR = (PFN_vkEnumeratePhysicalDeviceGroupsKHR)ps5_vk_proc("vkEnumeratePhysicalDeviceGroupsKHR");
    return p_vkEnumeratePhysicalDeviceGroupsKHR(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
}
static PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR p_vkGetPhysicalDeviceExternalBufferPropertiesKHR;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceExternalBufferPropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties) {
    if (!p_vkGetPhysicalDeviceExternalBufferPropertiesKHR) p_vkGetPhysicalDeviceExternalBufferPropertiesKHR = (PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR)ps5_vk_proc("vkGetPhysicalDeviceExternalBufferPropertiesKHR");
    p_vkGetPhysicalDeviceExternalBufferPropertiesKHR(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
}
static PFN_vkGetMemoryFdKHR p_vkGetMemoryFdKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetMemoryFdKHR(VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd) {
    if (!p_vkGetMemoryFdKHR) p_vkGetMemoryFdKHR = (PFN_vkGetMemoryFdKHR)ps5_vk_proc("vkGetMemoryFdKHR");
    return p_vkGetMemoryFdKHR(device, pGetFdInfo, pFd);
}
static PFN_vkGetMemoryFdPropertiesKHR p_vkGetMemoryFdPropertiesKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties) {
    if (!p_vkGetMemoryFdPropertiesKHR) p_vkGetMemoryFdPropertiesKHR = (PFN_vkGetMemoryFdPropertiesKHR)ps5_vk_proc("vkGetMemoryFdPropertiesKHR");
    return p_vkGetMemoryFdPropertiesKHR(device, handleType, fd, pMemoryFdProperties);
}
static PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR p_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties) {
    if (!p_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR) p_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR = (PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR)ps5_vk_proc("vkGetPhysicalDeviceExternalSemaphorePropertiesKHR");
    p_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
}
static PFN_vkImportSemaphoreFdKHR p_vkImportSemaphoreFdKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkImportSemaphoreFdKHR(VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo) {
    if (!p_vkImportSemaphoreFdKHR) p_vkImportSemaphoreFdKHR = (PFN_vkImportSemaphoreFdKHR)ps5_vk_proc("vkImportSemaphoreFdKHR");
    return p_vkImportSemaphoreFdKHR(device, pImportSemaphoreFdInfo);
}
static PFN_vkGetSemaphoreFdKHR p_vkGetSemaphoreFdKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetSemaphoreFdKHR(VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd) {
    if (!p_vkGetSemaphoreFdKHR) p_vkGetSemaphoreFdKHR = (PFN_vkGetSemaphoreFdKHR)ps5_vk_proc("vkGetSemaphoreFdKHR");
    return p_vkGetSemaphoreFdKHR(device, pGetFdInfo, pFd);
}
static PFN_vkCmdPushDescriptorSetKHR p_vkCmdPushDescriptorSetKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) {
    if (!p_vkCmdPushDescriptorSetKHR) p_vkCmdPushDescriptorSetKHR = (PFN_vkCmdPushDescriptorSetKHR)ps5_vk_proc("vkCmdPushDescriptorSetKHR");
    p_vkCmdPushDescriptorSetKHR(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
}
static PFN_vkCmdPushDescriptorSetWithTemplateKHR p_vkCmdPushDescriptorSetWithTemplateKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) {
    if (!p_vkCmdPushDescriptorSetWithTemplateKHR) p_vkCmdPushDescriptorSetWithTemplateKHR = (PFN_vkCmdPushDescriptorSetWithTemplateKHR)ps5_vk_proc("vkCmdPushDescriptorSetWithTemplateKHR");
    p_vkCmdPushDescriptorSetWithTemplateKHR(commandBuffer, descriptorUpdateTemplate, layout, set, pData);
}
static PFN_vkCreateDescriptorUpdateTemplateKHR p_vkCreateDescriptorUpdateTemplateKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDescriptorUpdateTemplateKHR(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) {
    if (!p_vkCreateDescriptorUpdateTemplateKHR) p_vkCreateDescriptorUpdateTemplateKHR = (PFN_vkCreateDescriptorUpdateTemplateKHR)ps5_vk_proc("vkCreateDescriptorUpdateTemplateKHR");
    return p_vkCreateDescriptorUpdateTemplateKHR(device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
}
static PFN_vkDestroyDescriptorUpdateTemplateKHR p_vkDestroyDescriptorUpdateTemplateKHR;
VKAPI_ATTR void VKAPI_CALL vkDestroyDescriptorUpdateTemplateKHR(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyDescriptorUpdateTemplateKHR) p_vkDestroyDescriptorUpdateTemplateKHR = (PFN_vkDestroyDescriptorUpdateTemplateKHR)ps5_vk_proc("vkDestroyDescriptorUpdateTemplateKHR");
    p_vkDestroyDescriptorUpdateTemplateKHR(device, descriptorUpdateTemplate, pAllocator);
}
static PFN_vkUpdateDescriptorSetWithTemplateKHR p_vkUpdateDescriptorSetWithTemplateKHR;
VKAPI_ATTR void VKAPI_CALL vkUpdateDescriptorSetWithTemplateKHR(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) {
    if (!p_vkUpdateDescriptorSetWithTemplateKHR) p_vkUpdateDescriptorSetWithTemplateKHR = (PFN_vkUpdateDescriptorSetWithTemplateKHR)ps5_vk_proc("vkUpdateDescriptorSetWithTemplateKHR");
    p_vkUpdateDescriptorSetWithTemplateKHR(device, descriptorSet, descriptorUpdateTemplate, pData);
}
static PFN_vkCreateRenderPass2KHR p_vkCreateRenderPass2KHR;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateRenderPass2KHR(VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) {
    if (!p_vkCreateRenderPass2KHR) p_vkCreateRenderPass2KHR = (PFN_vkCreateRenderPass2KHR)ps5_vk_proc("vkCreateRenderPass2KHR");
    return p_vkCreateRenderPass2KHR(device, pCreateInfo, pAllocator, pRenderPass);
}
static PFN_vkCmdBeginRenderPass2KHR p_vkCmdBeginRenderPass2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo) {
    if (!p_vkCmdBeginRenderPass2KHR) p_vkCmdBeginRenderPass2KHR = (PFN_vkCmdBeginRenderPass2KHR)ps5_vk_proc("vkCmdBeginRenderPass2KHR");
    p_vkCmdBeginRenderPass2KHR(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}
static PFN_vkCmdNextSubpass2KHR p_vkCmdNextSubpass2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfo* pSubpassBeginInfo, const VkSubpassEndInfo* pSubpassEndInfo) {
    if (!p_vkCmdNextSubpass2KHR) p_vkCmdNextSubpass2KHR = (PFN_vkCmdNextSubpass2KHR)ps5_vk_proc("vkCmdNextSubpass2KHR");
    p_vkCmdNextSubpass2KHR(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}
static PFN_vkCmdEndRenderPass2KHR p_vkCmdEndRenderPass2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfo* pSubpassEndInfo) {
    if (!p_vkCmdEndRenderPass2KHR) p_vkCmdEndRenderPass2KHR = (PFN_vkCmdEndRenderPass2KHR)ps5_vk_proc("vkCmdEndRenderPass2KHR");
    p_vkCmdEndRenderPass2KHR(commandBuffer, pSubpassEndInfo);
}
static PFN_vkGetSwapchainStatusKHR p_vkGetSwapchainStatusKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetSwapchainStatusKHR(VkDevice device, VkSwapchainKHR swapchain) {
    if (!p_vkGetSwapchainStatusKHR) p_vkGetSwapchainStatusKHR = (PFN_vkGetSwapchainStatusKHR)ps5_vk_proc("vkGetSwapchainStatusKHR");
    return p_vkGetSwapchainStatusKHR(device, swapchain);
}
static PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR p_vkGetPhysicalDeviceExternalFencePropertiesKHR;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceExternalFencePropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties) {
    if (!p_vkGetPhysicalDeviceExternalFencePropertiesKHR) p_vkGetPhysicalDeviceExternalFencePropertiesKHR = (PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR)ps5_vk_proc("vkGetPhysicalDeviceExternalFencePropertiesKHR");
    p_vkGetPhysicalDeviceExternalFencePropertiesKHR(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
}
static PFN_vkImportFenceFdKHR p_vkImportFenceFdKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkImportFenceFdKHR(VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo) {
    if (!p_vkImportFenceFdKHR) p_vkImportFenceFdKHR = (PFN_vkImportFenceFdKHR)ps5_vk_proc("vkImportFenceFdKHR");
    return p_vkImportFenceFdKHR(device, pImportFenceFdInfo);
}
static PFN_vkGetFenceFdKHR p_vkGetFenceFdKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetFenceFdKHR(VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd) {
    if (!p_vkGetFenceFdKHR) p_vkGetFenceFdKHR = (PFN_vkGetFenceFdKHR)ps5_vk_proc("vkGetFenceFdKHR");
    return p_vkGetFenceFdKHR(device, pGetFdInfo, pFd);
}
static PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR p_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pCounterCount, VkPerformanceCounterKHR* pCounters, VkPerformanceCounterDescriptionKHR* pCounterDescriptions) {
    if (!p_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR) p_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR = (PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR)ps5_vk_proc("vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR");
    return p_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(physicalDevice, queueFamilyIndex, pCounterCount, pCounters, pCounterDescriptions);
}
static PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR p_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(VkPhysicalDevice physicalDevice, const VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo, uint32_t* pNumPasses) {
    if (!p_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR) p_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR = (PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR)ps5_vk_proc("vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR");
    p_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(physicalDevice, pPerformanceQueryCreateInfo, pNumPasses);
}
static PFN_vkAcquireProfilingLockKHR p_vkAcquireProfilingLockKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkAcquireProfilingLockKHR(VkDevice device, const VkAcquireProfilingLockInfoKHR* pInfo) {
    if (!p_vkAcquireProfilingLockKHR) p_vkAcquireProfilingLockKHR = (PFN_vkAcquireProfilingLockKHR)ps5_vk_proc("vkAcquireProfilingLockKHR");
    return p_vkAcquireProfilingLockKHR(device, pInfo);
}
static PFN_vkReleaseProfilingLockKHR p_vkReleaseProfilingLockKHR;
VKAPI_ATTR void VKAPI_CALL vkReleaseProfilingLockKHR(VkDevice device) {
    if (!p_vkReleaseProfilingLockKHR) p_vkReleaseProfilingLockKHR = (PFN_vkReleaseProfilingLockKHR)ps5_vk_proc("vkReleaseProfilingLockKHR");
    p_vkReleaseProfilingLockKHR(device);
}
static PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR p_vkGetPhysicalDeviceSurfaceCapabilities2KHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities) {
    if (!p_vkGetPhysicalDeviceSurfaceCapabilities2KHR) p_vkGetPhysicalDeviceSurfaceCapabilities2KHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR)ps5_vk_proc("vkGetPhysicalDeviceSurfaceCapabilities2KHR");
    return p_vkGetPhysicalDeviceSurfaceCapabilities2KHR(physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
}
static PFN_vkGetPhysicalDeviceSurfaceFormats2KHR p_vkGetPhysicalDeviceSurfaceFormats2KHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceFormats2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats) {
    if (!p_vkGetPhysicalDeviceSurfaceFormats2KHR) p_vkGetPhysicalDeviceSurfaceFormats2KHR = (PFN_vkGetPhysicalDeviceSurfaceFormats2KHR)ps5_vk_proc("vkGetPhysicalDeviceSurfaceFormats2KHR");
    return p_vkGetPhysicalDeviceSurfaceFormats2KHR(physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats);
}
static PFN_vkGetPhysicalDeviceDisplayProperties2KHR p_vkGetPhysicalDeviceDisplayProperties2KHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties) {
    if (!p_vkGetPhysicalDeviceDisplayProperties2KHR) p_vkGetPhysicalDeviceDisplayProperties2KHR = (PFN_vkGetPhysicalDeviceDisplayProperties2KHR)ps5_vk_proc("vkGetPhysicalDeviceDisplayProperties2KHR");
    return p_vkGetPhysicalDeviceDisplayProperties2KHR(physicalDevice, pPropertyCount, pProperties);
}
static PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR p_vkGetPhysicalDeviceDisplayPlaneProperties2KHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPlaneProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties) {
    if (!p_vkGetPhysicalDeviceDisplayPlaneProperties2KHR) p_vkGetPhysicalDeviceDisplayPlaneProperties2KHR = (PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR)ps5_vk_proc("vkGetPhysicalDeviceDisplayPlaneProperties2KHR");
    return p_vkGetPhysicalDeviceDisplayPlaneProperties2KHR(physicalDevice, pPropertyCount, pProperties);
}
static PFN_vkGetDisplayModeProperties2KHR p_vkGetDisplayModeProperties2KHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetDisplayModeProperties2KHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties) {
    if (!p_vkGetDisplayModeProperties2KHR) p_vkGetDisplayModeProperties2KHR = (PFN_vkGetDisplayModeProperties2KHR)ps5_vk_proc("vkGetDisplayModeProperties2KHR");
    return p_vkGetDisplayModeProperties2KHR(physicalDevice, display, pPropertyCount, pProperties);
}
static PFN_vkGetDisplayPlaneCapabilities2KHR p_vkGetDisplayPlaneCapabilities2KHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetDisplayPlaneCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities) {
    if (!p_vkGetDisplayPlaneCapabilities2KHR) p_vkGetDisplayPlaneCapabilities2KHR = (PFN_vkGetDisplayPlaneCapabilities2KHR)ps5_vk_proc("vkGetDisplayPlaneCapabilities2KHR");
    return p_vkGetDisplayPlaneCapabilities2KHR(physicalDevice, pDisplayPlaneInfo, pCapabilities);
}
static PFN_vkGetImageMemoryRequirements2KHR p_vkGetImageMemoryRequirements2KHR;
VKAPI_ATTR void VKAPI_CALL vkGetImageMemoryRequirements2KHR(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
    if (!p_vkGetImageMemoryRequirements2KHR) p_vkGetImageMemoryRequirements2KHR = (PFN_vkGetImageMemoryRequirements2KHR)ps5_vk_proc("vkGetImageMemoryRequirements2KHR");
    p_vkGetImageMemoryRequirements2KHR(device, pInfo, pMemoryRequirements);
}
static PFN_vkGetBufferMemoryRequirements2KHR p_vkGetBufferMemoryRequirements2KHR;
VKAPI_ATTR void VKAPI_CALL vkGetBufferMemoryRequirements2KHR(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
    if (!p_vkGetBufferMemoryRequirements2KHR) p_vkGetBufferMemoryRequirements2KHR = (PFN_vkGetBufferMemoryRequirements2KHR)ps5_vk_proc("vkGetBufferMemoryRequirements2KHR");
    p_vkGetBufferMemoryRequirements2KHR(device, pInfo, pMemoryRequirements);
}
static PFN_vkGetImageSparseMemoryRequirements2KHR p_vkGetImageSparseMemoryRequirements2KHR;
VKAPI_ATTR void VKAPI_CALL vkGetImageSparseMemoryRequirements2KHR(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) {
    if (!p_vkGetImageSparseMemoryRequirements2KHR) p_vkGetImageSparseMemoryRequirements2KHR = (PFN_vkGetImageSparseMemoryRequirements2KHR)ps5_vk_proc("vkGetImageSparseMemoryRequirements2KHR");
    p_vkGetImageSparseMemoryRequirements2KHR(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}
static PFN_vkCreateSamplerYcbcrConversionKHR p_vkCreateSamplerYcbcrConversionKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateSamplerYcbcrConversionKHR(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion) {
    if (!p_vkCreateSamplerYcbcrConversionKHR) p_vkCreateSamplerYcbcrConversionKHR = (PFN_vkCreateSamplerYcbcrConversionKHR)ps5_vk_proc("vkCreateSamplerYcbcrConversionKHR");
    return p_vkCreateSamplerYcbcrConversionKHR(device, pCreateInfo, pAllocator, pYcbcrConversion);
}
static PFN_vkDestroySamplerYcbcrConversionKHR p_vkDestroySamplerYcbcrConversionKHR;
VKAPI_ATTR void VKAPI_CALL vkDestroySamplerYcbcrConversionKHR(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroySamplerYcbcrConversionKHR) p_vkDestroySamplerYcbcrConversionKHR = (PFN_vkDestroySamplerYcbcrConversionKHR)ps5_vk_proc("vkDestroySamplerYcbcrConversionKHR");
    p_vkDestroySamplerYcbcrConversionKHR(device, ycbcrConversion, pAllocator);
}
static PFN_vkBindBufferMemory2KHR p_vkBindBufferMemory2KHR;
VKAPI_ATTR VkResult VKAPI_CALL vkBindBufferMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) {
    if (!p_vkBindBufferMemory2KHR) p_vkBindBufferMemory2KHR = (PFN_vkBindBufferMemory2KHR)ps5_vk_proc("vkBindBufferMemory2KHR");
    return p_vkBindBufferMemory2KHR(device, bindInfoCount, pBindInfos);
}
static PFN_vkBindImageMemory2KHR p_vkBindImageMemory2KHR;
VKAPI_ATTR VkResult VKAPI_CALL vkBindImageMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) {
    if (!p_vkBindImageMemory2KHR) p_vkBindImageMemory2KHR = (PFN_vkBindImageMemory2KHR)ps5_vk_proc("vkBindImageMemory2KHR");
    return p_vkBindImageMemory2KHR(device, bindInfoCount, pBindInfos);
}
static PFN_vkGetDescriptorSetLayoutSupportKHR p_vkGetDescriptorSetLayoutSupportKHR;
VKAPI_ATTR void VKAPI_CALL vkGetDescriptorSetLayoutSupportKHR(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) {
    if (!p_vkGetDescriptorSetLayoutSupportKHR) p_vkGetDescriptorSetLayoutSupportKHR = (PFN_vkGetDescriptorSetLayoutSupportKHR)ps5_vk_proc("vkGetDescriptorSetLayoutSupportKHR");
    p_vkGetDescriptorSetLayoutSupportKHR(device, pCreateInfo, pSupport);
}
static PFN_vkCmdDrawIndirectCountKHR p_vkCmdDrawIndirectCountKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
    if (!p_vkCmdDrawIndirectCountKHR) p_vkCmdDrawIndirectCountKHR = (PFN_vkCmdDrawIndirectCountKHR)ps5_vk_proc("vkCmdDrawIndirectCountKHR");
    p_vkCmdDrawIndirectCountKHR(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
static PFN_vkCmdDrawIndexedIndirectCountKHR p_vkCmdDrawIndexedIndirectCountKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
    if (!p_vkCmdDrawIndexedIndirectCountKHR) p_vkCmdDrawIndexedIndirectCountKHR = (PFN_vkCmdDrawIndexedIndirectCountKHR)ps5_vk_proc("vkCmdDrawIndexedIndirectCountKHR");
    p_vkCmdDrawIndexedIndirectCountKHR(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
static PFN_vkGetSemaphoreCounterValueKHR p_vkGetSemaphoreCounterValueKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetSemaphoreCounterValueKHR(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) {
    if (!p_vkGetSemaphoreCounterValueKHR) p_vkGetSemaphoreCounterValueKHR = (PFN_vkGetSemaphoreCounterValueKHR)ps5_vk_proc("vkGetSemaphoreCounterValueKHR");
    return p_vkGetSemaphoreCounterValueKHR(device, semaphore, pValue);
}
static PFN_vkWaitSemaphoresKHR p_vkWaitSemaphoresKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkWaitSemaphoresKHR(VkDevice device, const VkSemaphoreWaitInfo* pWaitInfo, uint64_t timeout) {
    if (!p_vkWaitSemaphoresKHR) p_vkWaitSemaphoresKHR = (PFN_vkWaitSemaphoresKHR)ps5_vk_proc("vkWaitSemaphoresKHR");
    return p_vkWaitSemaphoresKHR(device, pWaitInfo, timeout);
}
static PFN_vkSignalSemaphoreKHR p_vkSignalSemaphoreKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkSignalSemaphoreKHR(VkDevice device, const VkSemaphoreSignalInfo* pSignalInfo) {
    if (!p_vkSignalSemaphoreKHR) p_vkSignalSemaphoreKHR = (PFN_vkSignalSemaphoreKHR)ps5_vk_proc("vkSignalSemaphoreKHR");
    return p_vkSignalSemaphoreKHR(device, pSignalInfo);
}
static PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR p_vkGetPhysicalDeviceFragmentShadingRatesKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceFragmentShadingRatesKHR(VkPhysicalDevice physicalDevice, uint32_t* pFragmentShadingRateCount, VkPhysicalDeviceFragmentShadingRateKHR* pFragmentShadingRates) {
    if (!p_vkGetPhysicalDeviceFragmentShadingRatesKHR) p_vkGetPhysicalDeviceFragmentShadingRatesKHR = (PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR)ps5_vk_proc("vkGetPhysicalDeviceFragmentShadingRatesKHR");
    return p_vkGetPhysicalDeviceFragmentShadingRatesKHR(physicalDevice, pFragmentShadingRateCount, pFragmentShadingRates);
}
static PFN_vkCmdSetFragmentShadingRateKHR p_vkCmdSetFragmentShadingRateKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdSetFragmentShadingRateKHR(VkCommandBuffer commandBuffer, const VkExtent2D* pFragmentSize, const VkFragmentShadingRateCombinerOpKHR combinerOps[2]) {
    if (!p_vkCmdSetFragmentShadingRateKHR) p_vkCmdSetFragmentShadingRateKHR = (PFN_vkCmdSetFragmentShadingRateKHR)ps5_vk_proc("vkCmdSetFragmentShadingRateKHR");
    p_vkCmdSetFragmentShadingRateKHR(commandBuffer, pFragmentSize, combinerOps);
}
static PFN_vkCmdSetRenderingAttachmentLocationsKHR p_vkCmdSetRenderingAttachmentLocationsKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdSetRenderingAttachmentLocationsKHR(VkCommandBuffer commandBuffer, const VkRenderingAttachmentLocationInfo* pLocationInfo) {
    if (!p_vkCmdSetRenderingAttachmentLocationsKHR) p_vkCmdSetRenderingAttachmentLocationsKHR = (PFN_vkCmdSetRenderingAttachmentLocationsKHR)ps5_vk_proc("vkCmdSetRenderingAttachmentLocationsKHR");
    p_vkCmdSetRenderingAttachmentLocationsKHR(commandBuffer, pLocationInfo);
}
static PFN_vkCmdSetRenderingInputAttachmentIndicesKHR p_vkCmdSetRenderingInputAttachmentIndicesKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdSetRenderingInputAttachmentIndicesKHR(VkCommandBuffer commandBuffer, const VkRenderingInputAttachmentIndexInfo* pInputAttachmentIndexInfo) {
    if (!p_vkCmdSetRenderingInputAttachmentIndicesKHR) p_vkCmdSetRenderingInputAttachmentIndicesKHR = (PFN_vkCmdSetRenderingInputAttachmentIndicesKHR)ps5_vk_proc("vkCmdSetRenderingInputAttachmentIndicesKHR");
    p_vkCmdSetRenderingInputAttachmentIndicesKHR(commandBuffer, pInputAttachmentIndexInfo);
}
static PFN_vkWaitForPresentKHR p_vkWaitForPresentKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkWaitForPresentKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout) {
    if (!p_vkWaitForPresentKHR) p_vkWaitForPresentKHR = (PFN_vkWaitForPresentKHR)ps5_vk_proc("vkWaitForPresentKHR");
    return p_vkWaitForPresentKHR(device, swapchain, presentId, timeout);
}
static PFN_vkGetBufferDeviceAddressKHR p_vkGetBufferDeviceAddressKHR;
VKAPI_ATTR VkDeviceAddress VKAPI_CALL vkGetBufferDeviceAddressKHR(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) {
    if (!p_vkGetBufferDeviceAddressKHR) p_vkGetBufferDeviceAddressKHR = (PFN_vkGetBufferDeviceAddressKHR)ps5_vk_proc("vkGetBufferDeviceAddressKHR");
    return p_vkGetBufferDeviceAddressKHR(device, pInfo);
}
static PFN_vkGetBufferOpaqueCaptureAddressKHR p_vkGetBufferOpaqueCaptureAddressKHR;
VKAPI_ATTR uint64_t VKAPI_CALL vkGetBufferOpaqueCaptureAddressKHR(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) {
    if (!p_vkGetBufferOpaqueCaptureAddressKHR) p_vkGetBufferOpaqueCaptureAddressKHR = (PFN_vkGetBufferOpaqueCaptureAddressKHR)ps5_vk_proc("vkGetBufferOpaqueCaptureAddressKHR");
    return p_vkGetBufferOpaqueCaptureAddressKHR(device, pInfo);
}
static PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR p_vkGetDeviceMemoryOpaqueCaptureAddressKHR;
VKAPI_ATTR uint64_t VKAPI_CALL vkGetDeviceMemoryOpaqueCaptureAddressKHR(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) {
    if (!p_vkGetDeviceMemoryOpaqueCaptureAddressKHR) p_vkGetDeviceMemoryOpaqueCaptureAddressKHR = (PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR)ps5_vk_proc("vkGetDeviceMemoryOpaqueCaptureAddressKHR");
    return p_vkGetDeviceMemoryOpaqueCaptureAddressKHR(device, pInfo);
}
static PFN_vkCreateDeferredOperationKHR p_vkCreateDeferredOperationKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDeferredOperationKHR(VkDevice device, const VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation) {
    if (!p_vkCreateDeferredOperationKHR) p_vkCreateDeferredOperationKHR = (PFN_vkCreateDeferredOperationKHR)ps5_vk_proc("vkCreateDeferredOperationKHR");
    return p_vkCreateDeferredOperationKHR(device, pAllocator, pDeferredOperation);
}
static PFN_vkDestroyDeferredOperationKHR p_vkDestroyDeferredOperationKHR;
VKAPI_ATTR void VKAPI_CALL vkDestroyDeferredOperationKHR(VkDevice device, VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyDeferredOperationKHR) p_vkDestroyDeferredOperationKHR = (PFN_vkDestroyDeferredOperationKHR)ps5_vk_proc("vkDestroyDeferredOperationKHR");
    p_vkDestroyDeferredOperationKHR(device, operation, pAllocator);
}
static PFN_vkGetDeferredOperationMaxConcurrencyKHR p_vkGetDeferredOperationMaxConcurrencyKHR;
VKAPI_ATTR uint32_t VKAPI_CALL vkGetDeferredOperationMaxConcurrencyKHR(VkDevice device, VkDeferredOperationKHR operation) {
    if (!p_vkGetDeferredOperationMaxConcurrencyKHR) p_vkGetDeferredOperationMaxConcurrencyKHR = (PFN_vkGetDeferredOperationMaxConcurrencyKHR)ps5_vk_proc("vkGetDeferredOperationMaxConcurrencyKHR");
    return p_vkGetDeferredOperationMaxConcurrencyKHR(device, operation);
}
static PFN_vkGetDeferredOperationResultKHR p_vkGetDeferredOperationResultKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetDeferredOperationResultKHR(VkDevice device, VkDeferredOperationKHR operation) {
    if (!p_vkGetDeferredOperationResultKHR) p_vkGetDeferredOperationResultKHR = (PFN_vkGetDeferredOperationResultKHR)ps5_vk_proc("vkGetDeferredOperationResultKHR");
    return p_vkGetDeferredOperationResultKHR(device, operation);
}
static PFN_vkDeferredOperationJoinKHR p_vkDeferredOperationJoinKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkDeferredOperationJoinKHR(VkDevice device, VkDeferredOperationKHR operation) {
    if (!p_vkDeferredOperationJoinKHR) p_vkDeferredOperationJoinKHR = (PFN_vkDeferredOperationJoinKHR)ps5_vk_proc("vkDeferredOperationJoinKHR");
    return p_vkDeferredOperationJoinKHR(device, operation);
}
static PFN_vkGetPipelineExecutablePropertiesKHR p_vkGetPipelineExecutablePropertiesKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPipelineExecutablePropertiesKHR(VkDevice device, const VkPipelineInfoKHR* pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties) {
    if (!p_vkGetPipelineExecutablePropertiesKHR) p_vkGetPipelineExecutablePropertiesKHR = (PFN_vkGetPipelineExecutablePropertiesKHR)ps5_vk_proc("vkGetPipelineExecutablePropertiesKHR");
    return p_vkGetPipelineExecutablePropertiesKHR(device, pPipelineInfo, pExecutableCount, pProperties);
}
static PFN_vkGetPipelineExecutableStatisticsKHR p_vkGetPipelineExecutableStatisticsKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPipelineExecutableStatisticsKHR(VkDevice device, const VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics) {
    if (!p_vkGetPipelineExecutableStatisticsKHR) p_vkGetPipelineExecutableStatisticsKHR = (PFN_vkGetPipelineExecutableStatisticsKHR)ps5_vk_proc("vkGetPipelineExecutableStatisticsKHR");
    return p_vkGetPipelineExecutableStatisticsKHR(device, pExecutableInfo, pStatisticCount, pStatistics);
}
static PFN_vkGetPipelineExecutableInternalRepresentationsKHR p_vkGetPipelineExecutableInternalRepresentationsKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPipelineExecutableInternalRepresentationsKHR(VkDevice device, const VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations) {
    if (!p_vkGetPipelineExecutableInternalRepresentationsKHR) p_vkGetPipelineExecutableInternalRepresentationsKHR = (PFN_vkGetPipelineExecutableInternalRepresentationsKHR)ps5_vk_proc("vkGetPipelineExecutableInternalRepresentationsKHR");
    return p_vkGetPipelineExecutableInternalRepresentationsKHR(device, pExecutableInfo, pInternalRepresentationCount, pInternalRepresentations);
}
static PFN_vkMapMemory2KHR p_vkMapMemory2KHR;
VKAPI_ATTR VkResult VKAPI_CALL vkMapMemory2KHR(VkDevice device, const VkMemoryMapInfo* pMemoryMapInfo, void** ppData) {
    if (!p_vkMapMemory2KHR) p_vkMapMemory2KHR = (PFN_vkMapMemory2KHR)ps5_vk_proc("vkMapMemory2KHR");
    return p_vkMapMemory2KHR(device, pMemoryMapInfo, ppData);
}
static PFN_vkUnmapMemory2KHR p_vkUnmapMemory2KHR;
VKAPI_ATTR VkResult VKAPI_CALL vkUnmapMemory2KHR(VkDevice device, const VkMemoryUnmapInfo* pMemoryUnmapInfo) {
    if (!p_vkUnmapMemory2KHR) p_vkUnmapMemory2KHR = (PFN_vkUnmapMemory2KHR)ps5_vk_proc("vkUnmapMemory2KHR");
    return p_vkUnmapMemory2KHR(device, pMemoryUnmapInfo);
}
static PFN_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR p_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceVideoEncodeQualityLevelInfoKHR* pQualityLevelInfo, VkVideoEncodeQualityLevelPropertiesKHR* pQualityLevelProperties) {
    if (!p_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR) p_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR = (PFN_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR)ps5_vk_proc("vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR");
    return p_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR(physicalDevice, pQualityLevelInfo, pQualityLevelProperties);
}
static PFN_vkGetEncodedVideoSessionParametersKHR p_vkGetEncodedVideoSessionParametersKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetEncodedVideoSessionParametersKHR(VkDevice device, const VkVideoEncodeSessionParametersGetInfoKHR* pVideoSessionParametersInfo, VkVideoEncodeSessionParametersFeedbackInfoKHR* pFeedbackInfo, size_t* pDataSize, void* pData) {
    if (!p_vkGetEncodedVideoSessionParametersKHR) p_vkGetEncodedVideoSessionParametersKHR = (PFN_vkGetEncodedVideoSessionParametersKHR)ps5_vk_proc("vkGetEncodedVideoSessionParametersKHR");
    return p_vkGetEncodedVideoSessionParametersKHR(device, pVideoSessionParametersInfo, pFeedbackInfo, pDataSize, pData);
}
static PFN_vkCmdEncodeVideoKHR p_vkCmdEncodeVideoKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdEncodeVideoKHR(VkCommandBuffer commandBuffer, const VkVideoEncodeInfoKHR* pEncodeInfo) {
    if (!p_vkCmdEncodeVideoKHR) p_vkCmdEncodeVideoKHR = (PFN_vkCmdEncodeVideoKHR)ps5_vk_proc("vkCmdEncodeVideoKHR");
    p_vkCmdEncodeVideoKHR(commandBuffer, pEncodeInfo);
}
static PFN_vkCmdSetEvent2KHR p_vkCmdSetEvent2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdSetEvent2KHR(VkCommandBuffer commandBuffer, VkEvent event, const VkDependencyInfo* pDependencyInfo) {
    if (!p_vkCmdSetEvent2KHR) p_vkCmdSetEvent2KHR = (PFN_vkCmdSetEvent2KHR)ps5_vk_proc("vkCmdSetEvent2KHR");
    p_vkCmdSetEvent2KHR(commandBuffer, event, pDependencyInfo);
}
static PFN_vkCmdResetEvent2KHR p_vkCmdResetEvent2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdResetEvent2KHR(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2 stageMask) {
    if (!p_vkCmdResetEvent2KHR) p_vkCmdResetEvent2KHR = (PFN_vkCmdResetEvent2KHR)ps5_vk_proc("vkCmdResetEvent2KHR");
    p_vkCmdResetEvent2KHR(commandBuffer, event, stageMask);
}
static PFN_vkCmdWaitEvents2KHR p_vkCmdWaitEvents2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdWaitEvents2KHR(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, const VkDependencyInfo* pDependencyInfos) {
    if (!p_vkCmdWaitEvents2KHR) p_vkCmdWaitEvents2KHR = (PFN_vkCmdWaitEvents2KHR)ps5_vk_proc("vkCmdWaitEvents2KHR");
    p_vkCmdWaitEvents2KHR(commandBuffer, eventCount, pEvents, pDependencyInfos);
}
static PFN_vkCmdPipelineBarrier2KHR p_vkCmdPipelineBarrier2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdPipelineBarrier2KHR(VkCommandBuffer commandBuffer, const VkDependencyInfo* pDependencyInfo) {
    if (!p_vkCmdPipelineBarrier2KHR) p_vkCmdPipelineBarrier2KHR = (PFN_vkCmdPipelineBarrier2KHR)ps5_vk_proc("vkCmdPipelineBarrier2KHR");
    p_vkCmdPipelineBarrier2KHR(commandBuffer, pDependencyInfo);
}
static PFN_vkCmdWriteTimestamp2KHR p_vkCmdWriteTimestamp2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdWriteTimestamp2KHR(VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query) {
    if (!p_vkCmdWriteTimestamp2KHR) p_vkCmdWriteTimestamp2KHR = (PFN_vkCmdWriteTimestamp2KHR)ps5_vk_proc("vkCmdWriteTimestamp2KHR");
    p_vkCmdWriteTimestamp2KHR(commandBuffer, stage, queryPool, query);
}
static PFN_vkQueueSubmit2KHR p_vkQueueSubmit2KHR;
VKAPI_ATTR VkResult VKAPI_CALL vkQueueSubmit2KHR(VkQueue queue, uint32_t submitCount, const VkSubmitInfo2* pSubmits, VkFence fence) {
    if (!p_vkQueueSubmit2KHR) p_vkQueueSubmit2KHR = (PFN_vkQueueSubmit2KHR)ps5_vk_proc("vkQueueSubmit2KHR");
    return p_vkQueueSubmit2KHR(queue, submitCount, pSubmits, fence);
}
static PFN_vkCmdBindIndexBuffer3KHR p_vkCmdBindIndexBuffer3KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdBindIndexBuffer3KHR(VkCommandBuffer commandBuffer, const VkBindIndexBuffer3InfoKHR* pInfo) {
    if (!p_vkCmdBindIndexBuffer3KHR) p_vkCmdBindIndexBuffer3KHR = (PFN_vkCmdBindIndexBuffer3KHR)ps5_vk_proc("vkCmdBindIndexBuffer3KHR");
    p_vkCmdBindIndexBuffer3KHR(commandBuffer, pInfo);
}
static PFN_vkCmdBindVertexBuffers3KHR p_vkCmdBindVertexBuffers3KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdBindVertexBuffers3KHR(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBindVertexBuffer3InfoKHR* pBindingInfos) {
    if (!p_vkCmdBindVertexBuffers3KHR) p_vkCmdBindVertexBuffers3KHR = (PFN_vkCmdBindVertexBuffers3KHR)ps5_vk_proc("vkCmdBindVertexBuffers3KHR");
    p_vkCmdBindVertexBuffers3KHR(commandBuffer, firstBinding, bindingCount, pBindingInfos);
}
static PFN_vkCmdDrawIndirect2KHR p_vkCmdDrawIndirect2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndirect2KHR(VkCommandBuffer commandBuffer, const VkDrawIndirect2InfoKHR* pInfo) {
    if (!p_vkCmdDrawIndirect2KHR) p_vkCmdDrawIndirect2KHR = (PFN_vkCmdDrawIndirect2KHR)ps5_vk_proc("vkCmdDrawIndirect2KHR");
    p_vkCmdDrawIndirect2KHR(commandBuffer, pInfo);
}
static PFN_vkCmdDrawIndexedIndirect2KHR p_vkCmdDrawIndexedIndirect2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndexedIndirect2KHR(VkCommandBuffer commandBuffer, const VkDrawIndirect2InfoKHR* pInfo) {
    if (!p_vkCmdDrawIndexedIndirect2KHR) p_vkCmdDrawIndexedIndirect2KHR = (PFN_vkCmdDrawIndexedIndirect2KHR)ps5_vk_proc("vkCmdDrawIndexedIndirect2KHR");
    p_vkCmdDrawIndexedIndirect2KHR(commandBuffer, pInfo);
}
static PFN_vkCmdDispatchIndirect2KHR p_vkCmdDispatchIndirect2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdDispatchIndirect2KHR(VkCommandBuffer commandBuffer, const VkDispatchIndirect2InfoKHR* pInfo) {
    if (!p_vkCmdDispatchIndirect2KHR) p_vkCmdDispatchIndirect2KHR = (PFN_vkCmdDispatchIndirect2KHR)ps5_vk_proc("vkCmdDispatchIndirect2KHR");
    p_vkCmdDispatchIndirect2KHR(commandBuffer, pInfo);
}
static PFN_vkCmdCopyMemoryKHR p_vkCmdCopyMemoryKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyMemoryKHR(VkCommandBuffer commandBuffer, const VkCopyDeviceMemoryInfoKHR* pCopyMemoryInfo) {
    if (!p_vkCmdCopyMemoryKHR) p_vkCmdCopyMemoryKHR = (PFN_vkCmdCopyMemoryKHR)ps5_vk_proc("vkCmdCopyMemoryKHR");
    p_vkCmdCopyMemoryKHR(commandBuffer, pCopyMemoryInfo);
}
static PFN_vkCmdCopyMemoryToImageKHR p_vkCmdCopyMemoryToImageKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyMemoryToImageKHR(VkCommandBuffer commandBuffer, const VkCopyDeviceMemoryImageInfoKHR* pCopyMemoryInfo) {
    if (!p_vkCmdCopyMemoryToImageKHR) p_vkCmdCopyMemoryToImageKHR = (PFN_vkCmdCopyMemoryToImageKHR)ps5_vk_proc("vkCmdCopyMemoryToImageKHR");
    p_vkCmdCopyMemoryToImageKHR(commandBuffer, pCopyMemoryInfo);
}
static PFN_vkCmdCopyImageToMemoryKHR p_vkCmdCopyImageToMemoryKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyImageToMemoryKHR(VkCommandBuffer commandBuffer, const VkCopyDeviceMemoryImageInfoKHR* pCopyMemoryInfo) {
    if (!p_vkCmdCopyImageToMemoryKHR) p_vkCmdCopyImageToMemoryKHR = (PFN_vkCmdCopyImageToMemoryKHR)ps5_vk_proc("vkCmdCopyImageToMemoryKHR");
    p_vkCmdCopyImageToMemoryKHR(commandBuffer, pCopyMemoryInfo);
}
static PFN_vkCmdUpdateMemoryKHR p_vkCmdUpdateMemoryKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdUpdateMemoryKHR(VkCommandBuffer commandBuffer, const VkDeviceAddressRangeKHR* pDstRange, VkAddressCommandFlagsKHR dstFlags, VkDeviceSize dataSize, const void* pData) {
    if (!p_vkCmdUpdateMemoryKHR) p_vkCmdUpdateMemoryKHR = (PFN_vkCmdUpdateMemoryKHR)ps5_vk_proc("vkCmdUpdateMemoryKHR");
    p_vkCmdUpdateMemoryKHR(commandBuffer, pDstRange, dstFlags, dataSize, pData);
}
static PFN_vkCmdFillMemoryKHR p_vkCmdFillMemoryKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdFillMemoryKHR(VkCommandBuffer commandBuffer, const VkDeviceAddressRangeKHR* pDstRange, VkAddressCommandFlagsKHR dstFlags, uint32_t data) {
    if (!p_vkCmdFillMemoryKHR) p_vkCmdFillMemoryKHR = (PFN_vkCmdFillMemoryKHR)ps5_vk_proc("vkCmdFillMemoryKHR");
    p_vkCmdFillMemoryKHR(commandBuffer, pDstRange, dstFlags, data);
}
static PFN_vkCmdCopyQueryPoolResultsToMemoryKHR p_vkCmdCopyQueryPoolResultsToMemoryKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyQueryPoolResultsToMemoryKHR(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, const VkStridedDeviceAddressRangeKHR* pDstRange, VkAddressCommandFlagsKHR dstFlags, VkQueryResultFlags queryResultFlags) {
    if (!p_vkCmdCopyQueryPoolResultsToMemoryKHR) p_vkCmdCopyQueryPoolResultsToMemoryKHR = (PFN_vkCmdCopyQueryPoolResultsToMemoryKHR)ps5_vk_proc("vkCmdCopyQueryPoolResultsToMemoryKHR");
    p_vkCmdCopyQueryPoolResultsToMemoryKHR(commandBuffer, queryPool, firstQuery, queryCount, pDstRange, dstFlags, queryResultFlags);
}
static PFN_vkCmdDrawIndirectCount2KHR p_vkCmdDrawIndirectCount2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndirectCount2KHR(VkCommandBuffer commandBuffer, const VkDrawIndirectCount2InfoKHR* pInfo) {
    if (!p_vkCmdDrawIndirectCount2KHR) p_vkCmdDrawIndirectCount2KHR = (PFN_vkCmdDrawIndirectCount2KHR)ps5_vk_proc("vkCmdDrawIndirectCount2KHR");
    p_vkCmdDrawIndirectCount2KHR(commandBuffer, pInfo);
}
static PFN_vkCmdDrawIndexedIndirectCount2KHR p_vkCmdDrawIndexedIndirectCount2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndexedIndirectCount2KHR(VkCommandBuffer commandBuffer, const VkDrawIndirectCount2InfoKHR* pInfo) {
    if (!p_vkCmdDrawIndexedIndirectCount2KHR) p_vkCmdDrawIndexedIndirectCount2KHR = (PFN_vkCmdDrawIndexedIndirectCount2KHR)ps5_vk_proc("vkCmdDrawIndexedIndirectCount2KHR");
    p_vkCmdDrawIndexedIndirectCount2KHR(commandBuffer, pInfo);
}
static PFN_vkCmdBeginConditionalRendering2EXT p_vkCmdBeginConditionalRendering2EXT;
VKAPI_ATTR void VKAPI_CALL vkCmdBeginConditionalRendering2EXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfo2EXT* pConditionalRenderingBegin) {
    if (!p_vkCmdBeginConditionalRendering2EXT) p_vkCmdBeginConditionalRendering2EXT = (PFN_vkCmdBeginConditionalRendering2EXT)ps5_vk_proc("vkCmdBeginConditionalRendering2EXT");
    p_vkCmdBeginConditionalRendering2EXT(commandBuffer, pConditionalRenderingBegin);
}
static PFN_vkCmdBindTransformFeedbackBuffers2EXT p_vkCmdBindTransformFeedbackBuffers2EXT;
VKAPI_ATTR void VKAPI_CALL vkCmdBindTransformFeedbackBuffers2EXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBindTransformFeedbackBuffer2InfoEXT* pBindingInfos) {
    if (!p_vkCmdBindTransformFeedbackBuffers2EXT) p_vkCmdBindTransformFeedbackBuffers2EXT = (PFN_vkCmdBindTransformFeedbackBuffers2EXT)ps5_vk_proc("vkCmdBindTransformFeedbackBuffers2EXT");
    p_vkCmdBindTransformFeedbackBuffers2EXT(commandBuffer, firstBinding, bindingCount, pBindingInfos);
}
static PFN_vkCmdBeginTransformFeedback2EXT p_vkCmdBeginTransformFeedback2EXT;
VKAPI_ATTR void VKAPI_CALL vkCmdBeginTransformFeedback2EXT(VkCommandBuffer commandBuffer, uint32_t firstCounterRange, uint32_t counterRangeCount, const VkBindTransformFeedbackBuffer2InfoEXT* pCounterInfos) {
    if (!p_vkCmdBeginTransformFeedback2EXT) p_vkCmdBeginTransformFeedback2EXT = (PFN_vkCmdBeginTransformFeedback2EXT)ps5_vk_proc("vkCmdBeginTransformFeedback2EXT");
    p_vkCmdBeginTransformFeedback2EXT(commandBuffer, firstCounterRange, counterRangeCount, pCounterInfos);
}
static PFN_vkCmdEndTransformFeedback2EXT p_vkCmdEndTransformFeedback2EXT;
VKAPI_ATTR void VKAPI_CALL vkCmdEndTransformFeedback2EXT(VkCommandBuffer commandBuffer, uint32_t firstCounterRange, uint32_t counterRangeCount, const VkBindTransformFeedbackBuffer2InfoEXT* pCounterInfos) {
    if (!p_vkCmdEndTransformFeedback2EXT) p_vkCmdEndTransformFeedback2EXT = (PFN_vkCmdEndTransformFeedback2EXT)ps5_vk_proc("vkCmdEndTransformFeedback2EXT");
    p_vkCmdEndTransformFeedback2EXT(commandBuffer, firstCounterRange, counterRangeCount, pCounterInfos);
}
static PFN_vkCmdDrawIndirectByteCount2EXT p_vkCmdDrawIndirectByteCount2EXT;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndirectByteCount2EXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, const VkBindTransformFeedbackBuffer2InfoEXT* pCounterInfo, uint32_t counterOffset, uint32_t vertexStride) {
    if (!p_vkCmdDrawIndirectByteCount2EXT) p_vkCmdDrawIndirectByteCount2EXT = (PFN_vkCmdDrawIndirectByteCount2EXT)ps5_vk_proc("vkCmdDrawIndirectByteCount2EXT");
    p_vkCmdDrawIndirectByteCount2EXT(commandBuffer, instanceCount, firstInstance, pCounterInfo, counterOffset, vertexStride);
}
static PFN_vkCmdDrawMeshTasksIndirect2EXT p_vkCmdDrawMeshTasksIndirect2EXT;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawMeshTasksIndirect2EXT(VkCommandBuffer commandBuffer, const VkDrawIndirect2InfoKHR* pInfo) {
    if (!p_vkCmdDrawMeshTasksIndirect2EXT) p_vkCmdDrawMeshTasksIndirect2EXT = (PFN_vkCmdDrawMeshTasksIndirect2EXT)ps5_vk_proc("vkCmdDrawMeshTasksIndirect2EXT");
    p_vkCmdDrawMeshTasksIndirect2EXT(commandBuffer, pInfo);
}
static PFN_vkCmdDrawMeshTasksIndirectCount2EXT p_vkCmdDrawMeshTasksIndirectCount2EXT;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawMeshTasksIndirectCount2EXT(VkCommandBuffer commandBuffer, const VkDrawIndirectCount2InfoKHR* pInfo) {
    if (!p_vkCmdDrawMeshTasksIndirectCount2EXT) p_vkCmdDrawMeshTasksIndirectCount2EXT = (PFN_vkCmdDrawMeshTasksIndirectCount2EXT)ps5_vk_proc("vkCmdDrawMeshTasksIndirectCount2EXT");
    p_vkCmdDrawMeshTasksIndirectCount2EXT(commandBuffer, pInfo);
}
static PFN_vkCmdWriteMarkerToMemoryAMD p_vkCmdWriteMarkerToMemoryAMD;
VKAPI_ATTR void VKAPI_CALL vkCmdWriteMarkerToMemoryAMD(VkCommandBuffer commandBuffer, const VkMemoryMarkerInfoAMD* pInfo) {
    if (!p_vkCmdWriteMarkerToMemoryAMD) p_vkCmdWriteMarkerToMemoryAMD = (PFN_vkCmdWriteMarkerToMemoryAMD)ps5_vk_proc("vkCmdWriteMarkerToMemoryAMD");
    p_vkCmdWriteMarkerToMemoryAMD(commandBuffer, pInfo);
}
static PFN_vkCreateAccelerationStructure2KHR p_vkCreateAccelerationStructure2KHR;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateAccelerationStructure2KHR(VkDevice device, const VkAccelerationStructureCreateInfo2KHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureKHR* pAccelerationStructure) {
    if (!p_vkCreateAccelerationStructure2KHR) p_vkCreateAccelerationStructure2KHR = (PFN_vkCreateAccelerationStructure2KHR)ps5_vk_proc("vkCreateAccelerationStructure2KHR");
    return p_vkCreateAccelerationStructure2KHR(device, pCreateInfo, pAllocator, pAccelerationStructure);
}
static PFN_vkCmdCopyBuffer2KHR p_vkCmdCopyBuffer2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2* pCopyBufferInfo) {
    if (!p_vkCmdCopyBuffer2KHR) p_vkCmdCopyBuffer2KHR = (PFN_vkCmdCopyBuffer2KHR)ps5_vk_proc("vkCmdCopyBuffer2KHR");
    p_vkCmdCopyBuffer2KHR(commandBuffer, pCopyBufferInfo);
}
static PFN_vkCmdCopyImage2KHR p_vkCmdCopyImage2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyImage2KHR(VkCommandBuffer commandBuffer, const VkCopyImageInfo2* pCopyImageInfo) {
    if (!p_vkCmdCopyImage2KHR) p_vkCmdCopyImage2KHR = (PFN_vkCmdCopyImage2KHR)ps5_vk_proc("vkCmdCopyImage2KHR");
    p_vkCmdCopyImage2KHR(commandBuffer, pCopyImageInfo);
}
static PFN_vkCmdCopyBufferToImage2KHR p_vkCmdCopyBufferToImage2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyBufferToImage2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) {
    if (!p_vkCmdCopyBufferToImage2KHR) p_vkCmdCopyBufferToImage2KHR = (PFN_vkCmdCopyBufferToImage2KHR)ps5_vk_proc("vkCmdCopyBufferToImage2KHR");
    p_vkCmdCopyBufferToImage2KHR(commandBuffer, pCopyBufferToImageInfo);
}
static PFN_vkCmdCopyImageToBuffer2KHR p_vkCmdCopyImageToBuffer2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyImageToBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) {
    if (!p_vkCmdCopyImageToBuffer2KHR) p_vkCmdCopyImageToBuffer2KHR = (PFN_vkCmdCopyImageToBuffer2KHR)ps5_vk_proc("vkCmdCopyImageToBuffer2KHR");
    p_vkCmdCopyImageToBuffer2KHR(commandBuffer, pCopyImageToBufferInfo);
}
static PFN_vkCmdBlitImage2KHR p_vkCmdBlitImage2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdBlitImage2KHR(VkCommandBuffer commandBuffer, const VkBlitImageInfo2* pBlitImageInfo) {
    if (!p_vkCmdBlitImage2KHR) p_vkCmdBlitImage2KHR = (PFN_vkCmdBlitImage2KHR)ps5_vk_proc("vkCmdBlitImage2KHR");
    p_vkCmdBlitImage2KHR(commandBuffer, pBlitImageInfo);
}
static PFN_vkCmdResolveImage2KHR p_vkCmdResolveImage2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdResolveImage2KHR(VkCommandBuffer commandBuffer, const VkResolveImageInfo2* pResolveImageInfo) {
    if (!p_vkCmdResolveImage2KHR) p_vkCmdResolveImage2KHR = (PFN_vkCmdResolveImage2KHR)ps5_vk_proc("vkCmdResolveImage2KHR");
    p_vkCmdResolveImage2KHR(commandBuffer, pResolveImageInfo);
}
static PFN_vkCmdTraceRaysIndirect2KHR p_vkCmdTraceRaysIndirect2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdTraceRaysIndirect2KHR(VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress) {
    if (!p_vkCmdTraceRaysIndirect2KHR) p_vkCmdTraceRaysIndirect2KHR = (PFN_vkCmdTraceRaysIndirect2KHR)ps5_vk_proc("vkCmdTraceRaysIndirect2KHR");
    p_vkCmdTraceRaysIndirect2KHR(commandBuffer, indirectDeviceAddress);
}
static PFN_vkGetDeviceBufferMemoryRequirementsKHR p_vkGetDeviceBufferMemoryRequirementsKHR;
VKAPI_ATTR void VKAPI_CALL vkGetDeviceBufferMemoryRequirementsKHR(VkDevice device, const VkDeviceBufferMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
    if (!p_vkGetDeviceBufferMemoryRequirementsKHR) p_vkGetDeviceBufferMemoryRequirementsKHR = (PFN_vkGetDeviceBufferMemoryRequirementsKHR)ps5_vk_proc("vkGetDeviceBufferMemoryRequirementsKHR");
    p_vkGetDeviceBufferMemoryRequirementsKHR(device, pInfo, pMemoryRequirements);
}
static PFN_vkGetDeviceImageMemoryRequirementsKHR p_vkGetDeviceImageMemoryRequirementsKHR;
VKAPI_ATTR void VKAPI_CALL vkGetDeviceImageMemoryRequirementsKHR(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
    if (!p_vkGetDeviceImageMemoryRequirementsKHR) p_vkGetDeviceImageMemoryRequirementsKHR = (PFN_vkGetDeviceImageMemoryRequirementsKHR)ps5_vk_proc("vkGetDeviceImageMemoryRequirementsKHR");
    p_vkGetDeviceImageMemoryRequirementsKHR(device, pInfo, pMemoryRequirements);
}
static PFN_vkGetDeviceImageSparseMemoryRequirementsKHR p_vkGetDeviceImageSparseMemoryRequirementsKHR;
VKAPI_ATTR void VKAPI_CALL vkGetDeviceImageSparseMemoryRequirementsKHR(VkDevice device, const VkDeviceImageMemoryRequirements* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) {
    if (!p_vkGetDeviceImageSparseMemoryRequirementsKHR) p_vkGetDeviceImageSparseMemoryRequirementsKHR = (PFN_vkGetDeviceImageSparseMemoryRequirementsKHR)ps5_vk_proc("vkGetDeviceImageSparseMemoryRequirementsKHR");
    p_vkGetDeviceImageSparseMemoryRequirementsKHR(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}
static PFN_vkCmdBindIndexBuffer2KHR p_vkCmdBindIndexBuffer2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdBindIndexBuffer2KHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType) {
    if (!p_vkCmdBindIndexBuffer2KHR) p_vkCmdBindIndexBuffer2KHR = (PFN_vkCmdBindIndexBuffer2KHR)ps5_vk_proc("vkCmdBindIndexBuffer2KHR");
    p_vkCmdBindIndexBuffer2KHR(commandBuffer, buffer, offset, size, indexType);
}
static PFN_vkGetRenderingAreaGranularityKHR p_vkGetRenderingAreaGranularityKHR;
VKAPI_ATTR void VKAPI_CALL vkGetRenderingAreaGranularityKHR(VkDevice device, const VkRenderingAreaInfo* pRenderingAreaInfo, VkExtent2D* pGranularity) {
    if (!p_vkGetRenderingAreaGranularityKHR) p_vkGetRenderingAreaGranularityKHR = (PFN_vkGetRenderingAreaGranularityKHR)ps5_vk_proc("vkGetRenderingAreaGranularityKHR");
    p_vkGetRenderingAreaGranularityKHR(device, pRenderingAreaInfo, pGranularity);
}
static PFN_vkGetDeviceImageSubresourceLayoutKHR p_vkGetDeviceImageSubresourceLayoutKHR;
VKAPI_ATTR void VKAPI_CALL vkGetDeviceImageSubresourceLayoutKHR(VkDevice device, const VkDeviceImageSubresourceInfo* pInfo, VkSubresourceLayout2* pLayout) {
    if (!p_vkGetDeviceImageSubresourceLayoutKHR) p_vkGetDeviceImageSubresourceLayoutKHR = (PFN_vkGetDeviceImageSubresourceLayoutKHR)ps5_vk_proc("vkGetDeviceImageSubresourceLayoutKHR");
    p_vkGetDeviceImageSubresourceLayoutKHR(device, pInfo, pLayout);
}
static PFN_vkGetImageSubresourceLayout2KHR p_vkGetImageSubresourceLayout2KHR;
VKAPI_ATTR void VKAPI_CALL vkGetImageSubresourceLayout2KHR(VkDevice device, VkImage image, const VkImageSubresource2* pSubresource, VkSubresourceLayout2* pLayout) {
    if (!p_vkGetImageSubresourceLayout2KHR) p_vkGetImageSubresourceLayout2KHR = (PFN_vkGetImageSubresourceLayout2KHR)ps5_vk_proc("vkGetImageSubresourceLayout2KHR");
    p_vkGetImageSubresourceLayout2KHR(device, image, pSubresource, pLayout);
}
static PFN_vkWaitForPresent2KHR p_vkWaitForPresent2KHR;
VKAPI_ATTR VkResult VKAPI_CALL vkWaitForPresent2KHR(VkDevice device, VkSwapchainKHR swapchain, const VkPresentWait2InfoKHR* pPresentWait2Info) {
    if (!p_vkWaitForPresent2KHR) p_vkWaitForPresent2KHR = (PFN_vkWaitForPresent2KHR)ps5_vk_proc("vkWaitForPresent2KHR");
    return p_vkWaitForPresent2KHR(device, swapchain, pPresentWait2Info);
}
static PFN_vkCreatePipelineBinariesKHR p_vkCreatePipelineBinariesKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkCreatePipelineBinariesKHR(VkDevice device, const VkPipelineBinaryCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineBinaryHandlesInfoKHR* pBinaries) {
    if (!p_vkCreatePipelineBinariesKHR) p_vkCreatePipelineBinariesKHR = (PFN_vkCreatePipelineBinariesKHR)ps5_vk_proc("vkCreatePipelineBinariesKHR");
    return p_vkCreatePipelineBinariesKHR(device, pCreateInfo, pAllocator, pBinaries);
}
static PFN_vkDestroyPipelineBinaryKHR p_vkDestroyPipelineBinaryKHR;
VKAPI_ATTR void VKAPI_CALL vkDestroyPipelineBinaryKHR(VkDevice device, VkPipelineBinaryKHR pipelineBinary, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyPipelineBinaryKHR) p_vkDestroyPipelineBinaryKHR = (PFN_vkDestroyPipelineBinaryKHR)ps5_vk_proc("vkDestroyPipelineBinaryKHR");
    p_vkDestroyPipelineBinaryKHR(device, pipelineBinary, pAllocator);
}
static PFN_vkGetPipelineKeyKHR p_vkGetPipelineKeyKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPipelineKeyKHR(VkDevice device, const VkPipelineCreateInfoKHR* pPipelineCreateInfo, VkPipelineBinaryKeyKHR* pPipelineKey) {
    if (!p_vkGetPipelineKeyKHR) p_vkGetPipelineKeyKHR = (PFN_vkGetPipelineKeyKHR)ps5_vk_proc("vkGetPipelineKeyKHR");
    return p_vkGetPipelineKeyKHR(device, pPipelineCreateInfo, pPipelineKey);
}
static PFN_vkGetPipelineBinaryDataKHR p_vkGetPipelineBinaryDataKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPipelineBinaryDataKHR(VkDevice device, const VkPipelineBinaryDataInfoKHR* pInfo, VkPipelineBinaryKeyKHR* pPipelineBinaryKey, size_t* pPipelineBinaryDataSize, void* pPipelineBinaryData) {
    if (!p_vkGetPipelineBinaryDataKHR) p_vkGetPipelineBinaryDataKHR = (PFN_vkGetPipelineBinaryDataKHR)ps5_vk_proc("vkGetPipelineBinaryDataKHR");
    return p_vkGetPipelineBinaryDataKHR(device, pInfo, pPipelineBinaryKey, pPipelineBinaryDataSize, pPipelineBinaryData);
}
static PFN_vkReleaseCapturedPipelineDataKHR p_vkReleaseCapturedPipelineDataKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkReleaseCapturedPipelineDataKHR(VkDevice device, const VkReleaseCapturedPipelineDataInfoKHR* pInfo, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkReleaseCapturedPipelineDataKHR) p_vkReleaseCapturedPipelineDataKHR = (PFN_vkReleaseCapturedPipelineDataKHR)ps5_vk_proc("vkReleaseCapturedPipelineDataKHR");
    return p_vkReleaseCapturedPipelineDataKHR(device, pInfo, pAllocator);
}
static PFN_vkReleaseSwapchainImagesKHR p_vkReleaseSwapchainImagesKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkReleaseSwapchainImagesKHR(VkDevice device, const VkReleaseSwapchainImagesInfoKHR* pReleaseInfo) {
    if (!p_vkReleaseSwapchainImagesKHR) p_vkReleaseSwapchainImagesKHR = (PFN_vkReleaseSwapchainImagesKHR)ps5_vk_proc("vkReleaseSwapchainImagesKHR");
    return p_vkReleaseSwapchainImagesKHR(device, pReleaseInfo);
}
static PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR p_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesKHR* pProperties) {
    if (!p_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR) p_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR = (PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR)ps5_vk_proc("vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR");
    return p_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR(physicalDevice, pPropertyCount, pProperties);
}
static PFN_vkCmdSetLineStippleKHR p_vkCmdSetLineStippleKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdSetLineStippleKHR(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) {
    if (!p_vkCmdSetLineStippleKHR) p_vkCmdSetLineStippleKHR = (PFN_vkCmdSetLineStippleKHR)ps5_vk_proc("vkCmdSetLineStippleKHR");
    p_vkCmdSetLineStippleKHR(commandBuffer, lineStippleFactor, lineStipplePattern);
}
static PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR p_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceCalibrateableTimeDomainsKHR(VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainKHR* pTimeDomains) {
    if (!p_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR) p_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR = (PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR)ps5_vk_proc("vkGetPhysicalDeviceCalibrateableTimeDomainsKHR");
    return p_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR(physicalDevice, pTimeDomainCount, pTimeDomains);
}
static PFN_vkGetCalibratedTimestampsKHR p_vkGetCalibratedTimestampsKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetCalibratedTimestampsKHR(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoKHR* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) {
    if (!p_vkGetCalibratedTimestampsKHR) p_vkGetCalibratedTimestampsKHR = (PFN_vkGetCalibratedTimestampsKHR)ps5_vk_proc("vkGetCalibratedTimestampsKHR");
    return p_vkGetCalibratedTimestampsKHR(device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
}
static PFN_vkCmdBindDescriptorSets2KHR p_vkCmdBindDescriptorSets2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdBindDescriptorSets2KHR(VkCommandBuffer commandBuffer, const VkBindDescriptorSetsInfo* pBindDescriptorSetsInfo) {
    if (!p_vkCmdBindDescriptorSets2KHR) p_vkCmdBindDescriptorSets2KHR = (PFN_vkCmdBindDescriptorSets2KHR)ps5_vk_proc("vkCmdBindDescriptorSets2KHR");
    p_vkCmdBindDescriptorSets2KHR(commandBuffer, pBindDescriptorSetsInfo);
}
static PFN_vkCmdPushConstants2KHR p_vkCmdPushConstants2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdPushConstants2KHR(VkCommandBuffer commandBuffer, const VkPushConstantsInfo* pPushConstantsInfo) {
    if (!p_vkCmdPushConstants2KHR) p_vkCmdPushConstants2KHR = (PFN_vkCmdPushConstants2KHR)ps5_vk_proc("vkCmdPushConstants2KHR");
    p_vkCmdPushConstants2KHR(commandBuffer, pPushConstantsInfo);
}
static PFN_vkCmdPushDescriptorSet2KHR p_vkCmdPushDescriptorSet2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdPushDescriptorSet2KHR(VkCommandBuffer commandBuffer, const VkPushDescriptorSetInfo* pPushDescriptorSetInfo) {
    if (!p_vkCmdPushDescriptorSet2KHR) p_vkCmdPushDescriptorSet2KHR = (PFN_vkCmdPushDescriptorSet2KHR)ps5_vk_proc("vkCmdPushDescriptorSet2KHR");
    p_vkCmdPushDescriptorSet2KHR(commandBuffer, pPushDescriptorSetInfo);
}
static PFN_vkCmdPushDescriptorSetWithTemplate2KHR p_vkCmdPushDescriptorSetWithTemplate2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdPushDescriptorSetWithTemplate2KHR(VkCommandBuffer commandBuffer, const VkPushDescriptorSetWithTemplateInfo* pPushDescriptorSetWithTemplateInfo) {
    if (!p_vkCmdPushDescriptorSetWithTemplate2KHR) p_vkCmdPushDescriptorSetWithTemplate2KHR = (PFN_vkCmdPushDescriptorSetWithTemplate2KHR)ps5_vk_proc("vkCmdPushDescriptorSetWithTemplate2KHR");
    p_vkCmdPushDescriptorSetWithTemplate2KHR(commandBuffer, pPushDescriptorSetWithTemplateInfo);
}
static PFN_vkCmdSetDescriptorBufferOffsets2EXT p_vkCmdSetDescriptorBufferOffsets2EXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDescriptorBufferOffsets2EXT(VkCommandBuffer commandBuffer, const VkSetDescriptorBufferOffsetsInfoEXT* pSetDescriptorBufferOffsetsInfo) {
    if (!p_vkCmdSetDescriptorBufferOffsets2EXT) p_vkCmdSetDescriptorBufferOffsets2EXT = (PFN_vkCmdSetDescriptorBufferOffsets2EXT)ps5_vk_proc("vkCmdSetDescriptorBufferOffsets2EXT");
    p_vkCmdSetDescriptorBufferOffsets2EXT(commandBuffer, pSetDescriptorBufferOffsetsInfo);
}
static PFN_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT p_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT;
VKAPI_ATTR void VKAPI_CALL vkCmdBindDescriptorBufferEmbeddedSamplers2EXT(VkCommandBuffer commandBuffer, const VkBindDescriptorBufferEmbeddedSamplersInfoEXT* pBindDescriptorBufferEmbeddedSamplersInfo) {
    if (!p_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT) p_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT = (PFN_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT)ps5_vk_proc("vkCmdBindDescriptorBufferEmbeddedSamplers2EXT");
    p_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT(commandBuffer, pBindDescriptorBufferEmbeddedSamplersInfo);
}
static PFN_vkCmdCopyMemoryIndirectKHR p_vkCmdCopyMemoryIndirectKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyMemoryIndirectKHR(VkCommandBuffer commandBuffer, const VkCopyMemoryIndirectInfoKHR* pCopyMemoryIndirectInfo) {
    if (!p_vkCmdCopyMemoryIndirectKHR) p_vkCmdCopyMemoryIndirectKHR = (PFN_vkCmdCopyMemoryIndirectKHR)ps5_vk_proc("vkCmdCopyMemoryIndirectKHR");
    p_vkCmdCopyMemoryIndirectKHR(commandBuffer, pCopyMemoryIndirectInfo);
}
static PFN_vkCmdCopyMemoryToImageIndirectKHR p_vkCmdCopyMemoryToImageIndirectKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyMemoryToImageIndirectKHR(VkCommandBuffer commandBuffer, const VkCopyMemoryToImageIndirectInfoKHR* pCopyMemoryToImageIndirectInfo) {
    if (!p_vkCmdCopyMemoryToImageIndirectKHR) p_vkCmdCopyMemoryToImageIndirectKHR = (PFN_vkCmdCopyMemoryToImageIndirectKHR)ps5_vk_proc("vkCmdCopyMemoryToImageIndirectKHR");
    p_vkCmdCopyMemoryToImageIndirectKHR(commandBuffer, pCopyMemoryToImageIndirectInfo);
}
static PFN_vkGetDeviceFaultReportsKHR p_vkGetDeviceFaultReportsKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetDeviceFaultReportsKHR(VkDevice device, uint64_t timeout, uint32_t* pFaultCounts, VkDeviceFaultInfoKHR* pFaultInfo) {
    if (!p_vkGetDeviceFaultReportsKHR) p_vkGetDeviceFaultReportsKHR = (PFN_vkGetDeviceFaultReportsKHR)ps5_vk_proc("vkGetDeviceFaultReportsKHR");
    return p_vkGetDeviceFaultReportsKHR(device, timeout, pFaultCounts, pFaultInfo);
}
static PFN_vkGetDeviceFaultDebugInfoKHR p_vkGetDeviceFaultDebugInfoKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetDeviceFaultDebugInfoKHR(VkDevice device, VkDeviceFaultDebugInfoKHR* pDebugInfo) {
    if (!p_vkGetDeviceFaultDebugInfoKHR) p_vkGetDeviceFaultDebugInfoKHR = (PFN_vkGetDeviceFaultDebugInfoKHR)ps5_vk_proc("vkGetDeviceFaultDebugInfoKHR");
    return p_vkGetDeviceFaultDebugInfoKHR(device, pDebugInfo);
}
static PFN_vkCmdEndRendering2KHR p_vkCmdEndRendering2KHR;
VKAPI_ATTR void VKAPI_CALL vkCmdEndRendering2KHR(VkCommandBuffer commandBuffer, const VkRenderingEndInfoKHR* pRenderingEndInfo) {
    if (!p_vkCmdEndRendering2KHR) p_vkCmdEndRendering2KHR = (PFN_vkCmdEndRendering2KHR)ps5_vk_proc("vkCmdEndRendering2KHR");
    p_vkCmdEndRendering2KHR(commandBuffer, pRenderingEndInfo);
}
static PFN_vkCreateDebugReportCallbackEXT p_vkCreateDebugReportCallbackEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {
    if (!p_vkCreateDebugReportCallbackEXT) p_vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)ps5_vk_proc("vkCreateDebugReportCallbackEXT");
    return p_vkCreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
}
static PFN_vkDestroyDebugReportCallbackEXT p_vkDestroyDebugReportCallbackEXT;
VKAPI_ATTR void VKAPI_CALL vkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyDebugReportCallbackEXT) p_vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)ps5_vk_proc("vkDestroyDebugReportCallbackEXT");
    p_vkDestroyDebugReportCallbackEXT(instance, callback, pAllocator);
}
static PFN_vkDebugReportMessageEXT p_vkDebugReportMessageEXT;
VKAPI_ATTR void VKAPI_CALL vkDebugReportMessageEXT(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage) {
    if (!p_vkDebugReportMessageEXT) p_vkDebugReportMessageEXT = (PFN_vkDebugReportMessageEXT)ps5_vk_proc("vkDebugReportMessageEXT");
    p_vkDebugReportMessageEXT(instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
}
static PFN_vkDebugMarkerSetObjectTagEXT p_vkDebugMarkerSetObjectTagEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkDebugMarkerSetObjectTagEXT(VkDevice device, const VkDebugMarkerObjectTagInfoEXT* pTagInfo) {
    if (!p_vkDebugMarkerSetObjectTagEXT) p_vkDebugMarkerSetObjectTagEXT = (PFN_vkDebugMarkerSetObjectTagEXT)ps5_vk_proc("vkDebugMarkerSetObjectTagEXT");
    return p_vkDebugMarkerSetObjectTagEXT(device, pTagInfo);
}
static PFN_vkDebugMarkerSetObjectNameEXT p_vkDebugMarkerSetObjectNameEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkDebugMarkerSetObjectNameEXT(VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo) {
    if (!p_vkDebugMarkerSetObjectNameEXT) p_vkDebugMarkerSetObjectNameEXT = (PFN_vkDebugMarkerSetObjectNameEXT)ps5_vk_proc("vkDebugMarkerSetObjectNameEXT");
    return p_vkDebugMarkerSetObjectNameEXT(device, pNameInfo);
}
static PFN_vkCmdDebugMarkerBeginEXT p_vkCmdDebugMarkerBeginEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) {
    if (!p_vkCmdDebugMarkerBeginEXT) p_vkCmdDebugMarkerBeginEXT = (PFN_vkCmdDebugMarkerBeginEXT)ps5_vk_proc("vkCmdDebugMarkerBeginEXT");
    p_vkCmdDebugMarkerBeginEXT(commandBuffer, pMarkerInfo);
}
static PFN_vkCmdDebugMarkerEndEXT p_vkCmdDebugMarkerEndEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer) {
    if (!p_vkCmdDebugMarkerEndEXT) p_vkCmdDebugMarkerEndEXT = (PFN_vkCmdDebugMarkerEndEXT)ps5_vk_proc("vkCmdDebugMarkerEndEXT");
    p_vkCmdDebugMarkerEndEXT(commandBuffer);
}
static PFN_vkCmdDebugMarkerInsertEXT p_vkCmdDebugMarkerInsertEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) {
    if (!p_vkCmdDebugMarkerInsertEXT) p_vkCmdDebugMarkerInsertEXT = (PFN_vkCmdDebugMarkerInsertEXT)ps5_vk_proc("vkCmdDebugMarkerInsertEXT");
    p_vkCmdDebugMarkerInsertEXT(commandBuffer, pMarkerInfo);
}
static PFN_vkCmdBindTransformFeedbackBuffersEXT p_vkCmdBindTransformFeedbackBuffersEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) {
    if (!p_vkCmdBindTransformFeedbackBuffersEXT) p_vkCmdBindTransformFeedbackBuffersEXT = (PFN_vkCmdBindTransformFeedbackBuffersEXT)ps5_vk_proc("vkCmdBindTransformFeedbackBuffersEXT");
    p_vkCmdBindTransformFeedbackBuffersEXT(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
}
static PFN_vkCmdBeginTransformFeedbackEXT p_vkCmdBeginTransformFeedbackEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) {
    if (!p_vkCmdBeginTransformFeedbackEXT) p_vkCmdBeginTransformFeedbackEXT = (PFN_vkCmdBeginTransformFeedbackEXT)ps5_vk_proc("vkCmdBeginTransformFeedbackEXT");
    p_vkCmdBeginTransformFeedbackEXT(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}
static PFN_vkCmdEndTransformFeedbackEXT p_vkCmdEndTransformFeedbackEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) {
    if (!p_vkCmdEndTransformFeedbackEXT) p_vkCmdEndTransformFeedbackEXT = (PFN_vkCmdEndTransformFeedbackEXT)ps5_vk_proc("vkCmdEndTransformFeedbackEXT");
    p_vkCmdEndTransformFeedbackEXT(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}
static PFN_vkCmdBeginQueryIndexedEXT p_vkCmdBeginQueryIndexedEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) {
    if (!p_vkCmdBeginQueryIndexedEXT) p_vkCmdBeginQueryIndexedEXT = (PFN_vkCmdBeginQueryIndexedEXT)ps5_vk_proc("vkCmdBeginQueryIndexedEXT");
    p_vkCmdBeginQueryIndexedEXT(commandBuffer, queryPool, query, flags, index);
}
static PFN_vkCmdEndQueryIndexedEXT p_vkCmdEndQueryIndexedEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index) {
    if (!p_vkCmdEndQueryIndexedEXT) p_vkCmdEndQueryIndexedEXT = (PFN_vkCmdEndQueryIndexedEXT)ps5_vk_proc("vkCmdEndQueryIndexedEXT");
    p_vkCmdEndQueryIndexedEXT(commandBuffer, queryPool, query, index);
}
static PFN_vkCmdDrawIndirectByteCountEXT p_vkCmdDrawIndirectByteCountEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) {
    if (!p_vkCmdDrawIndirectByteCountEXT) p_vkCmdDrawIndirectByteCountEXT = (PFN_vkCmdDrawIndirectByteCountEXT)ps5_vk_proc("vkCmdDrawIndirectByteCountEXT");
    p_vkCmdDrawIndirectByteCountEXT(commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
}
static PFN_vkCreateCuModuleNVX p_vkCreateCuModuleNVX;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateCuModuleNVX(VkDevice device, const VkCuModuleCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule) {
    if (!p_vkCreateCuModuleNVX) p_vkCreateCuModuleNVX = (PFN_vkCreateCuModuleNVX)ps5_vk_proc("vkCreateCuModuleNVX");
    return p_vkCreateCuModuleNVX(device, pCreateInfo, pAllocator, pModule);
}
static PFN_vkCreateCuFunctionNVX p_vkCreateCuFunctionNVX;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateCuFunctionNVX(VkDevice device, const VkCuFunctionCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction) {
    if (!p_vkCreateCuFunctionNVX) p_vkCreateCuFunctionNVX = (PFN_vkCreateCuFunctionNVX)ps5_vk_proc("vkCreateCuFunctionNVX");
    return p_vkCreateCuFunctionNVX(device, pCreateInfo, pAllocator, pFunction);
}
static PFN_vkDestroyCuModuleNVX p_vkDestroyCuModuleNVX;
VKAPI_ATTR void VKAPI_CALL vkDestroyCuModuleNVX(VkDevice device, VkCuModuleNVX module, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyCuModuleNVX) p_vkDestroyCuModuleNVX = (PFN_vkDestroyCuModuleNVX)ps5_vk_proc("vkDestroyCuModuleNVX");
    p_vkDestroyCuModuleNVX(device, module, pAllocator);
}
static PFN_vkDestroyCuFunctionNVX p_vkDestroyCuFunctionNVX;
VKAPI_ATTR void VKAPI_CALL vkDestroyCuFunctionNVX(VkDevice device, VkCuFunctionNVX function, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyCuFunctionNVX) p_vkDestroyCuFunctionNVX = (PFN_vkDestroyCuFunctionNVX)ps5_vk_proc("vkDestroyCuFunctionNVX");
    p_vkDestroyCuFunctionNVX(device, function, pAllocator);
}
static PFN_vkCmdCuLaunchKernelNVX p_vkCmdCuLaunchKernelNVX;
VKAPI_ATTR void VKAPI_CALL vkCmdCuLaunchKernelNVX(VkCommandBuffer commandBuffer, const VkCuLaunchInfoNVX* pLaunchInfo) {
    if (!p_vkCmdCuLaunchKernelNVX) p_vkCmdCuLaunchKernelNVX = (PFN_vkCmdCuLaunchKernelNVX)ps5_vk_proc("vkCmdCuLaunchKernelNVX");
    p_vkCmdCuLaunchKernelNVX(commandBuffer, pLaunchInfo);
}
static PFN_vkGetImageViewHandleNVX p_vkGetImageViewHandleNVX;
VKAPI_ATTR uint32_t VKAPI_CALL vkGetImageViewHandleNVX(VkDevice device, const VkImageViewHandleInfoNVX* pInfo) {
    if (!p_vkGetImageViewHandleNVX) p_vkGetImageViewHandleNVX = (PFN_vkGetImageViewHandleNVX)ps5_vk_proc("vkGetImageViewHandleNVX");
    return p_vkGetImageViewHandleNVX(device, pInfo);
}
static PFN_vkGetImageViewHandle64NVX p_vkGetImageViewHandle64NVX;
VKAPI_ATTR uint64_t VKAPI_CALL vkGetImageViewHandle64NVX(VkDevice device, const VkImageViewHandleInfoNVX* pInfo) {
    if (!p_vkGetImageViewHandle64NVX) p_vkGetImageViewHandle64NVX = (PFN_vkGetImageViewHandle64NVX)ps5_vk_proc("vkGetImageViewHandle64NVX");
    return p_vkGetImageViewHandle64NVX(device, pInfo);
}
static PFN_vkGetImageViewAddressNVX p_vkGetImageViewAddressNVX;
VKAPI_ATTR VkResult VKAPI_CALL vkGetImageViewAddressNVX(VkDevice device, VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties) {
    if (!p_vkGetImageViewAddressNVX) p_vkGetImageViewAddressNVX = (PFN_vkGetImageViewAddressNVX)ps5_vk_proc("vkGetImageViewAddressNVX");
    return p_vkGetImageViewAddressNVX(device, imageView, pProperties);
}
static PFN_vkGetDeviceCombinedImageSamplerIndexNVX p_vkGetDeviceCombinedImageSamplerIndexNVX;
VKAPI_ATTR uint64_t VKAPI_CALL vkGetDeviceCombinedImageSamplerIndexNVX(VkDevice device, uint64_t imageViewIndex, uint64_t samplerIndex) {
    if (!p_vkGetDeviceCombinedImageSamplerIndexNVX) p_vkGetDeviceCombinedImageSamplerIndexNVX = (PFN_vkGetDeviceCombinedImageSamplerIndexNVX)ps5_vk_proc("vkGetDeviceCombinedImageSamplerIndexNVX");
    return p_vkGetDeviceCombinedImageSamplerIndexNVX(device, imageViewIndex, samplerIndex);
}
static PFN_vkCmdDrawIndirectCountAMD p_vkCmdDrawIndirectCountAMD;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
    if (!p_vkCmdDrawIndirectCountAMD) p_vkCmdDrawIndirectCountAMD = (PFN_vkCmdDrawIndirectCountAMD)ps5_vk_proc("vkCmdDrawIndirectCountAMD");
    p_vkCmdDrawIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
static PFN_vkCmdDrawIndexedIndirectCountAMD p_vkCmdDrawIndexedIndirectCountAMD;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
    if (!p_vkCmdDrawIndexedIndirectCountAMD) p_vkCmdDrawIndexedIndirectCountAMD = (PFN_vkCmdDrawIndexedIndirectCountAMD)ps5_vk_proc("vkCmdDrawIndexedIndirectCountAMD");
    p_vkCmdDrawIndexedIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
static PFN_vkGetShaderInfoAMD p_vkGetShaderInfoAMD;
VKAPI_ATTR VkResult VKAPI_CALL vkGetShaderInfoAMD(VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo) {
    if (!p_vkGetShaderInfoAMD) p_vkGetShaderInfoAMD = (PFN_vkGetShaderInfoAMD)ps5_vk_proc("vkGetShaderInfoAMD");
    return p_vkGetShaderInfoAMD(device, pipeline, shaderStage, infoType, pInfoSize, pInfo);
}
static PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV p_vkGetPhysicalDeviceExternalImageFormatPropertiesNV;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceExternalImageFormatPropertiesNV(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties) {
    if (!p_vkGetPhysicalDeviceExternalImageFormatPropertiesNV) p_vkGetPhysicalDeviceExternalImageFormatPropertiesNV = (PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV)ps5_vk_proc("vkGetPhysicalDeviceExternalImageFormatPropertiesNV");
    return p_vkGetPhysicalDeviceExternalImageFormatPropertiesNV(physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties);
}
static PFN_vkCmdBeginConditionalRenderingEXT p_vkCmdBeginConditionalRenderingEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) {
    if (!p_vkCmdBeginConditionalRenderingEXT) p_vkCmdBeginConditionalRenderingEXT = (PFN_vkCmdBeginConditionalRenderingEXT)ps5_vk_proc("vkCmdBeginConditionalRenderingEXT");
    p_vkCmdBeginConditionalRenderingEXT(commandBuffer, pConditionalRenderingBegin);
}
static PFN_vkCmdEndConditionalRenderingEXT p_vkCmdEndConditionalRenderingEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer) {
    if (!p_vkCmdEndConditionalRenderingEXT) p_vkCmdEndConditionalRenderingEXT = (PFN_vkCmdEndConditionalRenderingEXT)ps5_vk_proc("vkCmdEndConditionalRenderingEXT");
    p_vkCmdEndConditionalRenderingEXT(commandBuffer);
}
static PFN_vkCmdSetViewportWScalingNV p_vkCmdSetViewportWScalingNV;
VKAPI_ATTR void VKAPI_CALL vkCmdSetViewportWScalingNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings) {
    if (!p_vkCmdSetViewportWScalingNV) p_vkCmdSetViewportWScalingNV = (PFN_vkCmdSetViewportWScalingNV)ps5_vk_proc("vkCmdSetViewportWScalingNV");
    p_vkCmdSetViewportWScalingNV(commandBuffer, firstViewport, viewportCount, pViewportWScalings);
}
static PFN_vkReleaseDisplayEXT p_vkReleaseDisplayEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkReleaseDisplayEXT(VkPhysicalDevice physicalDevice, VkDisplayKHR display) {
    if (!p_vkReleaseDisplayEXT) p_vkReleaseDisplayEXT = (PFN_vkReleaseDisplayEXT)ps5_vk_proc("vkReleaseDisplayEXT");
    return p_vkReleaseDisplayEXT(physicalDevice, display);
}
static PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT p_vkGetPhysicalDeviceSurfaceCapabilities2EXT;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilities2EXT(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities) {
    if (!p_vkGetPhysicalDeviceSurfaceCapabilities2EXT) p_vkGetPhysicalDeviceSurfaceCapabilities2EXT = (PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT)ps5_vk_proc("vkGetPhysicalDeviceSurfaceCapabilities2EXT");
    return p_vkGetPhysicalDeviceSurfaceCapabilities2EXT(physicalDevice, surface, pSurfaceCapabilities);
}
static PFN_vkDisplayPowerControlEXT p_vkDisplayPowerControlEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkDisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo) {
    if (!p_vkDisplayPowerControlEXT) p_vkDisplayPowerControlEXT = (PFN_vkDisplayPowerControlEXT)ps5_vk_proc("vkDisplayPowerControlEXT");
    return p_vkDisplayPowerControlEXT(device, display, pDisplayPowerInfo);
}
static PFN_vkRegisterDeviceEventEXT p_vkRegisterDeviceEventEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkRegisterDeviceEventEXT(VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) {
    if (!p_vkRegisterDeviceEventEXT) p_vkRegisterDeviceEventEXT = (PFN_vkRegisterDeviceEventEXT)ps5_vk_proc("vkRegisterDeviceEventEXT");
    return p_vkRegisterDeviceEventEXT(device, pDeviceEventInfo, pAllocator, pFence);
}
static PFN_vkRegisterDisplayEventEXT p_vkRegisterDisplayEventEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkRegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) {
    if (!p_vkRegisterDisplayEventEXT) p_vkRegisterDisplayEventEXT = (PFN_vkRegisterDisplayEventEXT)ps5_vk_proc("vkRegisterDisplayEventEXT");
    return p_vkRegisterDisplayEventEXT(device, display, pDisplayEventInfo, pAllocator, pFence);
}
static PFN_vkGetSwapchainCounterEXT p_vkGetSwapchainCounterEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkGetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) {
    if (!p_vkGetSwapchainCounterEXT) p_vkGetSwapchainCounterEXT = (PFN_vkGetSwapchainCounterEXT)ps5_vk_proc("vkGetSwapchainCounterEXT");
    return p_vkGetSwapchainCounterEXT(device, swapchain, counter, pCounterValue);
}
static PFN_vkGetRefreshCycleDurationGOOGLE p_vkGetRefreshCycleDurationGOOGLE;
VKAPI_ATTR VkResult VKAPI_CALL vkGetRefreshCycleDurationGOOGLE(VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties) {
    if (!p_vkGetRefreshCycleDurationGOOGLE) p_vkGetRefreshCycleDurationGOOGLE = (PFN_vkGetRefreshCycleDurationGOOGLE)ps5_vk_proc("vkGetRefreshCycleDurationGOOGLE");
    return p_vkGetRefreshCycleDurationGOOGLE(device, swapchain, pDisplayTimingProperties);
}
static PFN_vkGetPastPresentationTimingGOOGLE p_vkGetPastPresentationTimingGOOGLE;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPastPresentationTimingGOOGLE(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings) {
    if (!p_vkGetPastPresentationTimingGOOGLE) p_vkGetPastPresentationTimingGOOGLE = (PFN_vkGetPastPresentationTimingGOOGLE)ps5_vk_proc("vkGetPastPresentationTimingGOOGLE");
    return p_vkGetPastPresentationTimingGOOGLE(device, swapchain, pPresentationTimingCount, pPresentationTimings);
}
static PFN_vkCmdSetDiscardRectangleEXT p_vkCmdSetDiscardRectangleEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles) {
    if (!p_vkCmdSetDiscardRectangleEXT) p_vkCmdSetDiscardRectangleEXT = (PFN_vkCmdSetDiscardRectangleEXT)ps5_vk_proc("vkCmdSetDiscardRectangleEXT");
    p_vkCmdSetDiscardRectangleEXT(commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
}
static PFN_vkCmdSetDiscardRectangleEnableEXT p_vkCmdSetDiscardRectangleEnableEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDiscardRectangleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable) {
    if (!p_vkCmdSetDiscardRectangleEnableEXT) p_vkCmdSetDiscardRectangleEnableEXT = (PFN_vkCmdSetDiscardRectangleEnableEXT)ps5_vk_proc("vkCmdSetDiscardRectangleEnableEXT");
    p_vkCmdSetDiscardRectangleEnableEXT(commandBuffer, discardRectangleEnable);
}
static PFN_vkCmdSetDiscardRectangleModeEXT p_vkCmdSetDiscardRectangleModeEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDiscardRectangleModeEXT(VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode) {
    if (!p_vkCmdSetDiscardRectangleModeEXT) p_vkCmdSetDiscardRectangleModeEXT = (PFN_vkCmdSetDiscardRectangleModeEXT)ps5_vk_proc("vkCmdSetDiscardRectangleModeEXT");
    p_vkCmdSetDiscardRectangleModeEXT(commandBuffer, discardRectangleMode);
}
static PFN_vkSetHdrMetadataEXT p_vkSetHdrMetadataEXT;
VKAPI_ATTR void VKAPI_CALL vkSetHdrMetadataEXT(VkDevice device, uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata) {
    if (!p_vkSetHdrMetadataEXT) p_vkSetHdrMetadataEXT = (PFN_vkSetHdrMetadataEXT)ps5_vk_proc("vkSetHdrMetadataEXT");
    p_vkSetHdrMetadataEXT(device, swapchainCount, pSwapchains, pMetadata);
}
static PFN_vkSetDebugUtilsObjectNameEXT p_vkSetDebugUtilsObjectNameEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkSetDebugUtilsObjectNameEXT(VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo) {
    if (!p_vkSetDebugUtilsObjectNameEXT) p_vkSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)ps5_vk_proc("vkSetDebugUtilsObjectNameEXT");
    return p_vkSetDebugUtilsObjectNameEXT(device, pNameInfo);
}
static PFN_vkSetDebugUtilsObjectTagEXT p_vkSetDebugUtilsObjectTagEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkSetDebugUtilsObjectTagEXT(VkDevice device, const VkDebugUtilsObjectTagInfoEXT* pTagInfo) {
    if (!p_vkSetDebugUtilsObjectTagEXT) p_vkSetDebugUtilsObjectTagEXT = (PFN_vkSetDebugUtilsObjectTagEXT)ps5_vk_proc("vkSetDebugUtilsObjectTagEXT");
    return p_vkSetDebugUtilsObjectTagEXT(device, pTagInfo);
}
static PFN_vkQueueBeginDebugUtilsLabelEXT p_vkQueueBeginDebugUtilsLabelEXT;
VKAPI_ATTR void VKAPI_CALL vkQueueBeginDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) {
    if (!p_vkQueueBeginDebugUtilsLabelEXT) p_vkQueueBeginDebugUtilsLabelEXT = (PFN_vkQueueBeginDebugUtilsLabelEXT)ps5_vk_proc("vkQueueBeginDebugUtilsLabelEXT");
    p_vkQueueBeginDebugUtilsLabelEXT(queue, pLabelInfo);
}
static PFN_vkQueueEndDebugUtilsLabelEXT p_vkQueueEndDebugUtilsLabelEXT;
VKAPI_ATTR void VKAPI_CALL vkQueueEndDebugUtilsLabelEXT(VkQueue queue) {
    if (!p_vkQueueEndDebugUtilsLabelEXT) p_vkQueueEndDebugUtilsLabelEXT = (PFN_vkQueueEndDebugUtilsLabelEXT)ps5_vk_proc("vkQueueEndDebugUtilsLabelEXT");
    p_vkQueueEndDebugUtilsLabelEXT(queue);
}
static PFN_vkQueueInsertDebugUtilsLabelEXT p_vkQueueInsertDebugUtilsLabelEXT;
VKAPI_ATTR void VKAPI_CALL vkQueueInsertDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) {
    if (!p_vkQueueInsertDebugUtilsLabelEXT) p_vkQueueInsertDebugUtilsLabelEXT = (PFN_vkQueueInsertDebugUtilsLabelEXT)ps5_vk_proc("vkQueueInsertDebugUtilsLabelEXT");
    p_vkQueueInsertDebugUtilsLabelEXT(queue, pLabelInfo);
}
static PFN_vkCmdBeginDebugUtilsLabelEXT p_vkCmdBeginDebugUtilsLabelEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) {
    if (!p_vkCmdBeginDebugUtilsLabelEXT) p_vkCmdBeginDebugUtilsLabelEXT = (PFN_vkCmdBeginDebugUtilsLabelEXT)ps5_vk_proc("vkCmdBeginDebugUtilsLabelEXT");
    p_vkCmdBeginDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
}
static PFN_vkCmdEndDebugUtilsLabelEXT p_vkCmdEndDebugUtilsLabelEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer) {
    if (!p_vkCmdEndDebugUtilsLabelEXT) p_vkCmdEndDebugUtilsLabelEXT = (PFN_vkCmdEndDebugUtilsLabelEXT)ps5_vk_proc("vkCmdEndDebugUtilsLabelEXT");
    p_vkCmdEndDebugUtilsLabelEXT(commandBuffer);
}
static PFN_vkCmdInsertDebugUtilsLabelEXT p_vkCmdInsertDebugUtilsLabelEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) {
    if (!p_vkCmdInsertDebugUtilsLabelEXT) p_vkCmdInsertDebugUtilsLabelEXT = (PFN_vkCmdInsertDebugUtilsLabelEXT)ps5_vk_proc("vkCmdInsertDebugUtilsLabelEXT");
    p_vkCmdInsertDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
}
static PFN_vkCreateDebugUtilsMessengerEXT p_vkCreateDebugUtilsMessengerEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger) {
    if (!p_vkCreateDebugUtilsMessengerEXT) p_vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)ps5_vk_proc("vkCreateDebugUtilsMessengerEXT");
    return p_vkCreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
}
static PFN_vkDestroyDebugUtilsMessengerEXT p_vkDestroyDebugUtilsMessengerEXT;
VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyDebugUtilsMessengerEXT) p_vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)ps5_vk_proc("vkDestroyDebugUtilsMessengerEXT");
    p_vkDestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
}
static PFN_vkSubmitDebugUtilsMessageEXT p_vkSubmitDebugUtilsMessageEXT;
VKAPI_ATTR void VKAPI_CALL vkSubmitDebugUtilsMessageEXT(VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) {
    if (!p_vkSubmitDebugUtilsMessageEXT) p_vkSubmitDebugUtilsMessageEXT = (PFN_vkSubmitDebugUtilsMessageEXT)ps5_vk_proc("vkSubmitDebugUtilsMessageEXT");
    p_vkSubmitDebugUtilsMessageEXT(instance, messageSeverity, messageTypes, pCallbackData);
}
static PFN_vkCreateGpaSessionAMD p_vkCreateGpaSessionAMD;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateGpaSessionAMD(VkDevice device, const VkGpaSessionCreateInfoAMD* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkGpaSessionAMD* pGpaSession) {
    if (!p_vkCreateGpaSessionAMD) p_vkCreateGpaSessionAMD = (PFN_vkCreateGpaSessionAMD)ps5_vk_proc("vkCreateGpaSessionAMD");
    return p_vkCreateGpaSessionAMD(device, pCreateInfo, pAllocator, pGpaSession);
}
static PFN_vkDestroyGpaSessionAMD p_vkDestroyGpaSessionAMD;
VKAPI_ATTR void VKAPI_CALL vkDestroyGpaSessionAMD(VkDevice device, VkGpaSessionAMD gpaSession, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyGpaSessionAMD) p_vkDestroyGpaSessionAMD = (PFN_vkDestroyGpaSessionAMD)ps5_vk_proc("vkDestroyGpaSessionAMD");
    p_vkDestroyGpaSessionAMD(device, gpaSession, pAllocator);
}
static PFN_vkSetGpaDeviceClockModeAMD p_vkSetGpaDeviceClockModeAMD;
VKAPI_ATTR VkResult VKAPI_CALL vkSetGpaDeviceClockModeAMD(VkDevice device, VkGpaDeviceClockModeInfoAMD* pInfo) {
    if (!p_vkSetGpaDeviceClockModeAMD) p_vkSetGpaDeviceClockModeAMD = (PFN_vkSetGpaDeviceClockModeAMD)ps5_vk_proc("vkSetGpaDeviceClockModeAMD");
    return p_vkSetGpaDeviceClockModeAMD(device, pInfo);
}
static PFN_vkGetGpaDeviceClockInfoAMD p_vkGetGpaDeviceClockInfoAMD;
VKAPI_ATTR VkResult VKAPI_CALL vkGetGpaDeviceClockInfoAMD(VkDevice device, VkGpaDeviceGetClockInfoAMD* pInfo) {
    if (!p_vkGetGpaDeviceClockInfoAMD) p_vkGetGpaDeviceClockInfoAMD = (PFN_vkGetGpaDeviceClockInfoAMD)ps5_vk_proc("vkGetGpaDeviceClockInfoAMD");
    return p_vkGetGpaDeviceClockInfoAMD(device, pInfo);
}
static PFN_vkCmdBeginGpaSessionAMD p_vkCmdBeginGpaSessionAMD;
VKAPI_ATTR VkResult VKAPI_CALL vkCmdBeginGpaSessionAMD(VkCommandBuffer commandBuffer, VkGpaSessionAMD gpaSession) {
    if (!p_vkCmdBeginGpaSessionAMD) p_vkCmdBeginGpaSessionAMD = (PFN_vkCmdBeginGpaSessionAMD)ps5_vk_proc("vkCmdBeginGpaSessionAMD");
    return p_vkCmdBeginGpaSessionAMD(commandBuffer, gpaSession);
}
static PFN_vkCmdEndGpaSessionAMD p_vkCmdEndGpaSessionAMD;
VKAPI_ATTR VkResult VKAPI_CALL vkCmdEndGpaSessionAMD(VkCommandBuffer commandBuffer, VkGpaSessionAMD gpaSession) {
    if (!p_vkCmdEndGpaSessionAMD) p_vkCmdEndGpaSessionAMD = (PFN_vkCmdEndGpaSessionAMD)ps5_vk_proc("vkCmdEndGpaSessionAMD");
    return p_vkCmdEndGpaSessionAMD(commandBuffer, gpaSession);
}
static PFN_vkCmdBeginGpaSampleAMD p_vkCmdBeginGpaSampleAMD;
VKAPI_ATTR VkResult VKAPI_CALL vkCmdBeginGpaSampleAMD(VkCommandBuffer commandBuffer, VkGpaSessionAMD gpaSession, const VkGpaSampleBeginInfoAMD* pGpaSampleBeginInfo, uint32_t* pSampleID) {
    if (!p_vkCmdBeginGpaSampleAMD) p_vkCmdBeginGpaSampleAMD = (PFN_vkCmdBeginGpaSampleAMD)ps5_vk_proc("vkCmdBeginGpaSampleAMD");
    return p_vkCmdBeginGpaSampleAMD(commandBuffer, gpaSession, pGpaSampleBeginInfo, pSampleID);
}
static PFN_vkCmdEndGpaSampleAMD p_vkCmdEndGpaSampleAMD;
VKAPI_ATTR void VKAPI_CALL vkCmdEndGpaSampleAMD(VkCommandBuffer commandBuffer, VkGpaSessionAMD gpaSession, uint32_t sampleID) {
    if (!p_vkCmdEndGpaSampleAMD) p_vkCmdEndGpaSampleAMD = (PFN_vkCmdEndGpaSampleAMD)ps5_vk_proc("vkCmdEndGpaSampleAMD");
    p_vkCmdEndGpaSampleAMD(commandBuffer, gpaSession, sampleID);
}
static PFN_vkGetGpaSessionStatusAMD p_vkGetGpaSessionStatusAMD;
VKAPI_ATTR VkResult VKAPI_CALL vkGetGpaSessionStatusAMD(VkDevice device, VkGpaSessionAMD gpaSession) {
    if (!p_vkGetGpaSessionStatusAMD) p_vkGetGpaSessionStatusAMD = (PFN_vkGetGpaSessionStatusAMD)ps5_vk_proc("vkGetGpaSessionStatusAMD");
    return p_vkGetGpaSessionStatusAMD(device, gpaSession);
}
static PFN_vkGetGpaSessionResultsAMD p_vkGetGpaSessionResultsAMD;
VKAPI_ATTR VkResult VKAPI_CALL vkGetGpaSessionResultsAMD(VkDevice device, VkGpaSessionAMD gpaSession, uint32_t sampleID, size_t* pSizeInBytes, void* pData) {
    if (!p_vkGetGpaSessionResultsAMD) p_vkGetGpaSessionResultsAMD = (PFN_vkGetGpaSessionResultsAMD)ps5_vk_proc("vkGetGpaSessionResultsAMD");
    return p_vkGetGpaSessionResultsAMD(device, gpaSession, sampleID, pSizeInBytes, pData);
}
static PFN_vkResetGpaSessionAMD p_vkResetGpaSessionAMD;
VKAPI_ATTR VkResult VKAPI_CALL vkResetGpaSessionAMD(VkDevice device, VkGpaSessionAMD gpaSession) {
    if (!p_vkResetGpaSessionAMD) p_vkResetGpaSessionAMD = (PFN_vkResetGpaSessionAMD)ps5_vk_proc("vkResetGpaSessionAMD");
    return p_vkResetGpaSessionAMD(device, gpaSession);
}
static PFN_vkCmdCopyGpaSessionResultsAMD p_vkCmdCopyGpaSessionResultsAMD;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyGpaSessionResultsAMD(VkCommandBuffer commandBuffer, VkGpaSessionAMD gpaSession) {
    if (!p_vkCmdCopyGpaSessionResultsAMD) p_vkCmdCopyGpaSessionResultsAMD = (PFN_vkCmdCopyGpaSessionResultsAMD)ps5_vk_proc("vkCmdCopyGpaSessionResultsAMD");
    p_vkCmdCopyGpaSessionResultsAMD(commandBuffer, gpaSession);
}
static PFN_vkWriteSamplerDescriptorsEXT p_vkWriteSamplerDescriptorsEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkWriteSamplerDescriptorsEXT(VkDevice device, uint32_t samplerCount, const VkSamplerCreateInfo* pSamplers, const VkHostAddressRangeEXT* pDescriptors) {
    if (!p_vkWriteSamplerDescriptorsEXT) p_vkWriteSamplerDescriptorsEXT = (PFN_vkWriteSamplerDescriptorsEXT)ps5_vk_proc("vkWriteSamplerDescriptorsEXT");
    return p_vkWriteSamplerDescriptorsEXT(device, samplerCount, pSamplers, pDescriptors);
}
static PFN_vkWriteResourceDescriptorsEXT p_vkWriteResourceDescriptorsEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkWriteResourceDescriptorsEXT(VkDevice device, uint32_t resourceCount, const VkResourceDescriptorInfoEXT* pResources, const VkHostAddressRangeEXT* pDescriptors) {
    if (!p_vkWriteResourceDescriptorsEXT) p_vkWriteResourceDescriptorsEXT = (PFN_vkWriteResourceDescriptorsEXT)ps5_vk_proc("vkWriteResourceDescriptorsEXT");
    return p_vkWriteResourceDescriptorsEXT(device, resourceCount, pResources, pDescriptors);
}
static PFN_vkCmdBindSamplerHeapEXT p_vkCmdBindSamplerHeapEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdBindSamplerHeapEXT(VkCommandBuffer commandBuffer, const VkBindHeapInfoEXT* pBindInfo) {
    if (!p_vkCmdBindSamplerHeapEXT) p_vkCmdBindSamplerHeapEXT = (PFN_vkCmdBindSamplerHeapEXT)ps5_vk_proc("vkCmdBindSamplerHeapEXT");
    p_vkCmdBindSamplerHeapEXT(commandBuffer, pBindInfo);
}
static PFN_vkCmdBindResourceHeapEXT p_vkCmdBindResourceHeapEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdBindResourceHeapEXT(VkCommandBuffer commandBuffer, const VkBindHeapInfoEXT* pBindInfo) {
    if (!p_vkCmdBindResourceHeapEXT) p_vkCmdBindResourceHeapEXT = (PFN_vkCmdBindResourceHeapEXT)ps5_vk_proc("vkCmdBindResourceHeapEXT");
    p_vkCmdBindResourceHeapEXT(commandBuffer, pBindInfo);
}
static PFN_vkCmdPushDataEXT p_vkCmdPushDataEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdPushDataEXT(VkCommandBuffer commandBuffer, const VkPushDataInfoEXT* pPushDataInfo) {
    if (!p_vkCmdPushDataEXT) p_vkCmdPushDataEXT = (PFN_vkCmdPushDataEXT)ps5_vk_proc("vkCmdPushDataEXT");
    p_vkCmdPushDataEXT(commandBuffer, pPushDataInfo);
}
static PFN_vkGetImageOpaqueCaptureDataEXT p_vkGetImageOpaqueCaptureDataEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkGetImageOpaqueCaptureDataEXT(VkDevice device, uint32_t imageCount, const VkImage* pImages, VkHostAddressRangeEXT* pDatas) {
    if (!p_vkGetImageOpaqueCaptureDataEXT) p_vkGetImageOpaqueCaptureDataEXT = (PFN_vkGetImageOpaqueCaptureDataEXT)ps5_vk_proc("vkGetImageOpaqueCaptureDataEXT");
    return p_vkGetImageOpaqueCaptureDataEXT(device, imageCount, pImages, pDatas);
}
static PFN_vkGetPhysicalDeviceDescriptorSizeEXT p_vkGetPhysicalDeviceDescriptorSizeEXT;
VKAPI_ATTR VkDeviceSize VKAPI_CALL vkGetPhysicalDeviceDescriptorSizeEXT(VkPhysicalDevice physicalDevice, VkDescriptorType descriptorType) {
    if (!p_vkGetPhysicalDeviceDescriptorSizeEXT) p_vkGetPhysicalDeviceDescriptorSizeEXT = (PFN_vkGetPhysicalDeviceDescriptorSizeEXT)ps5_vk_proc("vkGetPhysicalDeviceDescriptorSizeEXT");
    return p_vkGetPhysicalDeviceDescriptorSizeEXT(physicalDevice, descriptorType);
}
static PFN_vkRegisterCustomBorderColorEXT p_vkRegisterCustomBorderColorEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkRegisterCustomBorderColorEXT(VkDevice device, const VkSamplerCustomBorderColorCreateInfoEXT* pBorderColor, VkBool32 requestIndex, uint32_t* pIndex) {
    if (!p_vkRegisterCustomBorderColorEXT) p_vkRegisterCustomBorderColorEXT = (PFN_vkRegisterCustomBorderColorEXT)ps5_vk_proc("vkRegisterCustomBorderColorEXT");
    return p_vkRegisterCustomBorderColorEXT(device, pBorderColor, requestIndex, pIndex);
}
static PFN_vkUnregisterCustomBorderColorEXT p_vkUnregisterCustomBorderColorEXT;
VKAPI_ATTR void VKAPI_CALL vkUnregisterCustomBorderColorEXT(VkDevice device, uint32_t index) {
    if (!p_vkUnregisterCustomBorderColorEXT) p_vkUnregisterCustomBorderColorEXT = (PFN_vkUnregisterCustomBorderColorEXT)ps5_vk_proc("vkUnregisterCustomBorderColorEXT");
    p_vkUnregisterCustomBorderColorEXT(device, index);
}
static PFN_vkGetTensorOpaqueCaptureDataARM p_vkGetTensorOpaqueCaptureDataARM;
VKAPI_ATTR VkResult VKAPI_CALL vkGetTensorOpaqueCaptureDataARM(VkDevice device, uint32_t tensorCount, const VkTensorARM* pTensors, VkHostAddressRangeEXT* pDatas) {
    if (!p_vkGetTensorOpaqueCaptureDataARM) p_vkGetTensorOpaqueCaptureDataARM = (PFN_vkGetTensorOpaqueCaptureDataARM)ps5_vk_proc("vkGetTensorOpaqueCaptureDataARM");
    return p_vkGetTensorOpaqueCaptureDataARM(device, tensorCount, pTensors, pDatas);
}
static PFN_vkCmdSetSampleLocationsEXT p_vkCmdSetSampleLocationsEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo) {
    if (!p_vkCmdSetSampleLocationsEXT) p_vkCmdSetSampleLocationsEXT = (PFN_vkCmdSetSampleLocationsEXT)ps5_vk_proc("vkCmdSetSampleLocationsEXT");
    p_vkCmdSetSampleLocationsEXT(commandBuffer, pSampleLocationsInfo);
}
static PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT p_vkGetPhysicalDeviceMultisamplePropertiesEXT;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceMultisamplePropertiesEXT(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties) {
    if (!p_vkGetPhysicalDeviceMultisamplePropertiesEXT) p_vkGetPhysicalDeviceMultisamplePropertiesEXT = (PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT)ps5_vk_proc("vkGetPhysicalDeviceMultisamplePropertiesEXT");
    p_vkGetPhysicalDeviceMultisamplePropertiesEXT(physicalDevice, samples, pMultisampleProperties);
}
static PFN_vkGetImageDrmFormatModifierPropertiesEXT p_vkGetImageDrmFormatModifierPropertiesEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkGetImageDrmFormatModifierPropertiesEXT(VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties) {
    if (!p_vkGetImageDrmFormatModifierPropertiesEXT) p_vkGetImageDrmFormatModifierPropertiesEXT = (PFN_vkGetImageDrmFormatModifierPropertiesEXT)ps5_vk_proc("vkGetImageDrmFormatModifierPropertiesEXT");
    return p_vkGetImageDrmFormatModifierPropertiesEXT(device, image, pProperties);
}
static PFN_vkCreateValidationCacheEXT p_vkCreateValidationCacheEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateValidationCacheEXT(VkDevice device, const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache) {
    if (!p_vkCreateValidationCacheEXT) p_vkCreateValidationCacheEXT = (PFN_vkCreateValidationCacheEXT)ps5_vk_proc("vkCreateValidationCacheEXT");
    return p_vkCreateValidationCacheEXT(device, pCreateInfo, pAllocator, pValidationCache);
}
static PFN_vkDestroyValidationCacheEXT p_vkDestroyValidationCacheEXT;
VKAPI_ATTR void VKAPI_CALL vkDestroyValidationCacheEXT(VkDevice device, VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyValidationCacheEXT) p_vkDestroyValidationCacheEXT = (PFN_vkDestroyValidationCacheEXT)ps5_vk_proc("vkDestroyValidationCacheEXT");
    p_vkDestroyValidationCacheEXT(device, validationCache, pAllocator);
}
static PFN_vkMergeValidationCachesEXT p_vkMergeValidationCachesEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkMergeValidationCachesEXT(VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches) {
    if (!p_vkMergeValidationCachesEXT) p_vkMergeValidationCachesEXT = (PFN_vkMergeValidationCachesEXT)ps5_vk_proc("vkMergeValidationCachesEXT");
    return p_vkMergeValidationCachesEXT(device, dstCache, srcCacheCount, pSrcCaches);
}
static PFN_vkGetValidationCacheDataEXT p_vkGetValidationCacheDataEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkGetValidationCacheDataEXT(VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData) {
    if (!p_vkGetValidationCacheDataEXT) p_vkGetValidationCacheDataEXT = (PFN_vkGetValidationCacheDataEXT)ps5_vk_proc("vkGetValidationCacheDataEXT");
    return p_vkGetValidationCacheDataEXT(device, validationCache, pDataSize, pData);
}
static PFN_vkCmdBindShadingRateImageNV p_vkCmdBindShadingRateImageNV;
VKAPI_ATTR void VKAPI_CALL vkCmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) {
    if (!p_vkCmdBindShadingRateImageNV) p_vkCmdBindShadingRateImageNV = (PFN_vkCmdBindShadingRateImageNV)ps5_vk_proc("vkCmdBindShadingRateImageNV");
    p_vkCmdBindShadingRateImageNV(commandBuffer, imageView, imageLayout);
}
static PFN_vkCmdSetViewportShadingRatePaletteNV p_vkCmdSetViewportShadingRatePaletteNV;
VKAPI_ATTR void VKAPI_CALL vkCmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes) {
    if (!p_vkCmdSetViewportShadingRatePaletteNV) p_vkCmdSetViewportShadingRatePaletteNV = (PFN_vkCmdSetViewportShadingRatePaletteNV)ps5_vk_proc("vkCmdSetViewportShadingRatePaletteNV");
    p_vkCmdSetViewportShadingRatePaletteNV(commandBuffer, firstViewport, viewportCount, pShadingRatePalettes);
}
static PFN_vkCmdSetCoarseSampleOrderNV p_vkCmdSetCoarseSampleOrderNV;
VKAPI_ATTR void VKAPI_CALL vkCmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders) {
    if (!p_vkCmdSetCoarseSampleOrderNV) p_vkCmdSetCoarseSampleOrderNV = (PFN_vkCmdSetCoarseSampleOrderNV)ps5_vk_proc("vkCmdSetCoarseSampleOrderNV");
    p_vkCmdSetCoarseSampleOrderNV(commandBuffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
}
static PFN_vkCreateAccelerationStructureNV p_vkCreateAccelerationStructureNV;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateAccelerationStructureNV(VkDevice device, const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure) {
    if (!p_vkCreateAccelerationStructureNV) p_vkCreateAccelerationStructureNV = (PFN_vkCreateAccelerationStructureNV)ps5_vk_proc("vkCreateAccelerationStructureNV");
    return p_vkCreateAccelerationStructureNV(device, pCreateInfo, pAllocator, pAccelerationStructure);
}
static PFN_vkDestroyAccelerationStructureNV p_vkDestroyAccelerationStructureNV;
VKAPI_ATTR void VKAPI_CALL vkDestroyAccelerationStructureNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyAccelerationStructureNV) p_vkDestroyAccelerationStructureNV = (PFN_vkDestroyAccelerationStructureNV)ps5_vk_proc("vkDestroyAccelerationStructureNV");
    p_vkDestroyAccelerationStructureNV(device, accelerationStructure, pAllocator);
}
static PFN_vkGetAccelerationStructureMemoryRequirementsNV p_vkGetAccelerationStructureMemoryRequirementsNV;
VKAPI_ATTR void VKAPI_CALL vkGetAccelerationStructureMemoryRequirementsNV(VkDevice device, const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
    if (!p_vkGetAccelerationStructureMemoryRequirementsNV) p_vkGetAccelerationStructureMemoryRequirementsNV = (PFN_vkGetAccelerationStructureMemoryRequirementsNV)ps5_vk_proc("vkGetAccelerationStructureMemoryRequirementsNV");
    p_vkGetAccelerationStructureMemoryRequirementsNV(device, pInfo, pMemoryRequirements);
}
static PFN_vkBindAccelerationStructureMemoryNV p_vkBindAccelerationStructureMemoryNV;
VKAPI_ATTR VkResult VKAPI_CALL vkBindAccelerationStructureMemoryNV(VkDevice device, uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos) {
    if (!p_vkBindAccelerationStructureMemoryNV) p_vkBindAccelerationStructureMemoryNV = (PFN_vkBindAccelerationStructureMemoryNV)ps5_vk_proc("vkBindAccelerationStructureMemoryNV");
    return p_vkBindAccelerationStructureMemoryNV(device, bindInfoCount, pBindInfos);
}
static PFN_vkCmdBuildAccelerationStructureNV p_vkCmdBuildAccelerationStructureNV;
VKAPI_ATTR void VKAPI_CALL vkCmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset) {
    if (!p_vkCmdBuildAccelerationStructureNV) p_vkCmdBuildAccelerationStructureNV = (PFN_vkCmdBuildAccelerationStructureNV)ps5_vk_proc("vkCmdBuildAccelerationStructureNV");
    p_vkCmdBuildAccelerationStructureNV(commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
}
static PFN_vkCmdCopyAccelerationStructureNV p_vkCmdCopyAccelerationStructureNV;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode) {
    if (!p_vkCmdCopyAccelerationStructureNV) p_vkCmdCopyAccelerationStructureNV = (PFN_vkCmdCopyAccelerationStructureNV)ps5_vk_proc("vkCmdCopyAccelerationStructureNV");
    p_vkCmdCopyAccelerationStructureNV(commandBuffer, dst, src, mode);
}
static PFN_vkCmdTraceRaysNV p_vkCmdTraceRaysNV;
VKAPI_ATTR void VKAPI_CALL vkCmdTraceRaysNV(VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth) {
    if (!p_vkCmdTraceRaysNV) p_vkCmdTraceRaysNV = (PFN_vkCmdTraceRaysNV)ps5_vk_proc("vkCmdTraceRaysNV");
    p_vkCmdTraceRaysNV(commandBuffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
}
static PFN_vkCreateRayTracingPipelinesNV p_vkCreateRayTracingPipelinesNV;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateRayTracingPipelinesNV(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) {
    if (!p_vkCreateRayTracingPipelinesNV) p_vkCreateRayTracingPipelinesNV = (PFN_vkCreateRayTracingPipelinesNV)ps5_vk_proc("vkCreateRayTracingPipelinesNV");
    return p_vkCreateRayTracingPipelinesNV(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}
static PFN_vkGetRayTracingShaderGroupHandlesKHR p_vkGetRayTracingShaderGroupHandlesKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetRayTracingShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) {
    if (!p_vkGetRayTracingShaderGroupHandlesKHR) p_vkGetRayTracingShaderGroupHandlesKHR = (PFN_vkGetRayTracingShaderGroupHandlesKHR)ps5_vk_proc("vkGetRayTracingShaderGroupHandlesKHR");
    return p_vkGetRayTracingShaderGroupHandlesKHR(device, pipeline, firstGroup, groupCount, dataSize, pData);
}
static PFN_vkGetRayTracingShaderGroupHandlesNV p_vkGetRayTracingShaderGroupHandlesNV;
VKAPI_ATTR VkResult VKAPI_CALL vkGetRayTracingShaderGroupHandlesNV(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) {
    if (!p_vkGetRayTracingShaderGroupHandlesNV) p_vkGetRayTracingShaderGroupHandlesNV = (PFN_vkGetRayTracingShaderGroupHandlesNV)ps5_vk_proc("vkGetRayTracingShaderGroupHandlesNV");
    return p_vkGetRayTracingShaderGroupHandlesNV(device, pipeline, firstGroup, groupCount, dataSize, pData);
}
static PFN_vkGetAccelerationStructureHandleNV p_vkGetAccelerationStructureHandleNV;
VKAPI_ATTR VkResult VKAPI_CALL vkGetAccelerationStructureHandleNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData) {
    if (!p_vkGetAccelerationStructureHandleNV) p_vkGetAccelerationStructureHandleNV = (PFN_vkGetAccelerationStructureHandleNV)ps5_vk_proc("vkGetAccelerationStructureHandleNV");
    return p_vkGetAccelerationStructureHandleNV(device, accelerationStructure, dataSize, pData);
}
static PFN_vkCmdWriteAccelerationStructuresPropertiesNV p_vkCmdWriteAccelerationStructuresPropertiesNV;
VKAPI_ATTR void VKAPI_CALL vkCmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) {
    if (!p_vkCmdWriteAccelerationStructuresPropertiesNV) p_vkCmdWriteAccelerationStructuresPropertiesNV = (PFN_vkCmdWriteAccelerationStructuresPropertiesNV)ps5_vk_proc("vkCmdWriteAccelerationStructuresPropertiesNV");
    p_vkCmdWriteAccelerationStructuresPropertiesNV(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}
static PFN_vkCompileDeferredNV p_vkCompileDeferredNV;
VKAPI_ATTR VkResult VKAPI_CALL vkCompileDeferredNV(VkDevice device, VkPipeline pipeline, uint32_t shader) {
    if (!p_vkCompileDeferredNV) p_vkCompileDeferredNV = (PFN_vkCompileDeferredNV)ps5_vk_proc("vkCompileDeferredNV");
    return p_vkCompileDeferredNV(device, pipeline, shader);
}
static PFN_vkGetMemoryHostPointerPropertiesEXT p_vkGetMemoryHostPointerPropertiesEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkGetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties) {
    if (!p_vkGetMemoryHostPointerPropertiesEXT) p_vkGetMemoryHostPointerPropertiesEXT = (PFN_vkGetMemoryHostPointerPropertiesEXT)ps5_vk_proc("vkGetMemoryHostPointerPropertiesEXT");
    return p_vkGetMemoryHostPointerPropertiesEXT(device, handleType, pHostPointer, pMemoryHostPointerProperties);
}
static PFN_vkCmdWriteBufferMarkerAMD p_vkCmdWriteBufferMarkerAMD;
VKAPI_ATTR void VKAPI_CALL vkCmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) {
    if (!p_vkCmdWriteBufferMarkerAMD) p_vkCmdWriteBufferMarkerAMD = (PFN_vkCmdWriteBufferMarkerAMD)ps5_vk_proc("vkCmdWriteBufferMarkerAMD");
    p_vkCmdWriteBufferMarkerAMD(commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
}
static PFN_vkCmdWriteBufferMarker2AMD p_vkCmdWriteBufferMarker2AMD;
VKAPI_ATTR void VKAPI_CALL vkCmdWriteBufferMarker2AMD(VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) {
    if (!p_vkCmdWriteBufferMarker2AMD) p_vkCmdWriteBufferMarker2AMD = (PFN_vkCmdWriteBufferMarker2AMD)ps5_vk_proc("vkCmdWriteBufferMarker2AMD");
    p_vkCmdWriteBufferMarker2AMD(commandBuffer, stage, dstBuffer, dstOffset, marker);
}
static PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT p_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainKHR* pTimeDomains) {
    if (!p_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT) p_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT = (PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT)ps5_vk_proc("vkGetPhysicalDeviceCalibrateableTimeDomainsEXT");
    return p_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(physicalDevice, pTimeDomainCount, pTimeDomains);
}
static PFN_vkGetCalibratedTimestampsEXT p_vkGetCalibratedTimestampsEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkGetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoKHR* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) {
    if (!p_vkGetCalibratedTimestampsEXT) p_vkGetCalibratedTimestampsEXT = (PFN_vkGetCalibratedTimestampsEXT)ps5_vk_proc("vkGetCalibratedTimestampsEXT");
    return p_vkGetCalibratedTimestampsEXT(device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
}
static PFN_vkCmdDrawMeshTasksNV p_vkCmdDrawMeshTasksNV;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask) {
    if (!p_vkCmdDrawMeshTasksNV) p_vkCmdDrawMeshTasksNV = (PFN_vkCmdDrawMeshTasksNV)ps5_vk_proc("vkCmdDrawMeshTasksNV");
    p_vkCmdDrawMeshTasksNV(commandBuffer, taskCount, firstTask);
}
static PFN_vkCmdDrawMeshTasksIndirectNV p_vkCmdDrawMeshTasksIndirectNV;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
    if (!p_vkCmdDrawMeshTasksIndirectNV) p_vkCmdDrawMeshTasksIndirectNV = (PFN_vkCmdDrawMeshTasksIndirectNV)ps5_vk_proc("vkCmdDrawMeshTasksIndirectNV");
    p_vkCmdDrawMeshTasksIndirectNV(commandBuffer, buffer, offset, drawCount, stride);
}
static PFN_vkCmdDrawMeshTasksIndirectCountNV p_vkCmdDrawMeshTasksIndirectCountNV;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
    if (!p_vkCmdDrawMeshTasksIndirectCountNV) p_vkCmdDrawMeshTasksIndirectCountNV = (PFN_vkCmdDrawMeshTasksIndirectCountNV)ps5_vk_proc("vkCmdDrawMeshTasksIndirectCountNV");
    p_vkCmdDrawMeshTasksIndirectCountNV(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
static PFN_vkCmdSetExclusiveScissorEnableNV p_vkCmdSetExclusiveScissorEnableNV;
VKAPI_ATTR void VKAPI_CALL vkCmdSetExclusiveScissorEnableNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkBool32* pExclusiveScissorEnables) {
    if (!p_vkCmdSetExclusiveScissorEnableNV) p_vkCmdSetExclusiveScissorEnableNV = (PFN_vkCmdSetExclusiveScissorEnableNV)ps5_vk_proc("vkCmdSetExclusiveScissorEnableNV");
    p_vkCmdSetExclusiveScissorEnableNV(commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissorEnables);
}
static PFN_vkCmdSetExclusiveScissorNV p_vkCmdSetExclusiveScissorNV;
VKAPI_ATTR void VKAPI_CALL vkCmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors) {
    if (!p_vkCmdSetExclusiveScissorNV) p_vkCmdSetExclusiveScissorNV = (PFN_vkCmdSetExclusiveScissorNV)ps5_vk_proc("vkCmdSetExclusiveScissorNV");
    p_vkCmdSetExclusiveScissorNV(commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
}
static PFN_vkCmdSetCheckpointNV p_vkCmdSetCheckpointNV;
VKAPI_ATTR void VKAPI_CALL vkCmdSetCheckpointNV(VkCommandBuffer commandBuffer, const void* pCheckpointMarker) {
    if (!p_vkCmdSetCheckpointNV) p_vkCmdSetCheckpointNV = (PFN_vkCmdSetCheckpointNV)ps5_vk_proc("vkCmdSetCheckpointNV");
    p_vkCmdSetCheckpointNV(commandBuffer, pCheckpointMarker);
}
static PFN_vkGetQueueCheckpointDataNV p_vkGetQueueCheckpointDataNV;
VKAPI_ATTR void VKAPI_CALL vkGetQueueCheckpointDataNV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData) {
    if (!p_vkGetQueueCheckpointDataNV) p_vkGetQueueCheckpointDataNV = (PFN_vkGetQueueCheckpointDataNV)ps5_vk_proc("vkGetQueueCheckpointDataNV");
    p_vkGetQueueCheckpointDataNV(queue, pCheckpointDataCount, pCheckpointData);
}
static PFN_vkGetQueueCheckpointData2NV p_vkGetQueueCheckpointData2NV;
VKAPI_ATTR void VKAPI_CALL vkGetQueueCheckpointData2NV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData) {
    if (!p_vkGetQueueCheckpointData2NV) p_vkGetQueueCheckpointData2NV = (PFN_vkGetQueueCheckpointData2NV)ps5_vk_proc("vkGetQueueCheckpointData2NV");
    p_vkGetQueueCheckpointData2NV(queue, pCheckpointDataCount, pCheckpointData);
}
static PFN_vkSetSwapchainPresentTimingQueueSizeEXT p_vkSetSwapchainPresentTimingQueueSizeEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkSetSwapchainPresentTimingQueueSizeEXT(VkDevice device, VkSwapchainKHR swapchain, uint32_t size) {
    if (!p_vkSetSwapchainPresentTimingQueueSizeEXT) p_vkSetSwapchainPresentTimingQueueSizeEXT = (PFN_vkSetSwapchainPresentTimingQueueSizeEXT)ps5_vk_proc("vkSetSwapchainPresentTimingQueueSizeEXT");
    return p_vkSetSwapchainPresentTimingQueueSizeEXT(device, swapchain, size);
}
static PFN_vkGetSwapchainTimingPropertiesEXT p_vkGetSwapchainTimingPropertiesEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkGetSwapchainTimingPropertiesEXT(VkDevice device, VkSwapchainKHR swapchain, VkSwapchainTimingPropertiesEXT* pSwapchainTimingProperties, uint64_t* pSwapchainTimingPropertiesCounter) {
    if (!p_vkGetSwapchainTimingPropertiesEXT) p_vkGetSwapchainTimingPropertiesEXT = (PFN_vkGetSwapchainTimingPropertiesEXT)ps5_vk_proc("vkGetSwapchainTimingPropertiesEXT");
    return p_vkGetSwapchainTimingPropertiesEXT(device, swapchain, pSwapchainTimingProperties, pSwapchainTimingPropertiesCounter);
}
static PFN_vkGetSwapchainTimeDomainPropertiesEXT p_vkGetSwapchainTimeDomainPropertiesEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkGetSwapchainTimeDomainPropertiesEXT(VkDevice device, VkSwapchainKHR swapchain, VkSwapchainTimeDomainPropertiesEXT* pSwapchainTimeDomainProperties, uint64_t* pTimeDomainsCounter) {
    if (!p_vkGetSwapchainTimeDomainPropertiesEXT) p_vkGetSwapchainTimeDomainPropertiesEXT = (PFN_vkGetSwapchainTimeDomainPropertiesEXT)ps5_vk_proc("vkGetSwapchainTimeDomainPropertiesEXT");
    return p_vkGetSwapchainTimeDomainPropertiesEXT(device, swapchain, pSwapchainTimeDomainProperties, pTimeDomainsCounter);
}
static PFN_vkGetPastPresentationTimingEXT p_vkGetPastPresentationTimingEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPastPresentationTimingEXT(VkDevice device, const VkPastPresentationTimingInfoEXT* pPastPresentationTimingInfo, VkPastPresentationTimingPropertiesEXT* pPastPresentationTimingProperties) {
    if (!p_vkGetPastPresentationTimingEXT) p_vkGetPastPresentationTimingEXT = (PFN_vkGetPastPresentationTimingEXT)ps5_vk_proc("vkGetPastPresentationTimingEXT");
    return p_vkGetPastPresentationTimingEXT(device, pPastPresentationTimingInfo, pPastPresentationTimingProperties);
}
static PFN_vkInitializePerformanceApiINTEL p_vkInitializePerformanceApiINTEL;
VKAPI_ATTR VkResult VKAPI_CALL vkInitializePerformanceApiINTEL(VkDevice device, const VkInitializePerformanceApiInfoINTEL* pInitializeInfo) {
    if (!p_vkInitializePerformanceApiINTEL) p_vkInitializePerformanceApiINTEL = (PFN_vkInitializePerformanceApiINTEL)ps5_vk_proc("vkInitializePerformanceApiINTEL");
    return p_vkInitializePerformanceApiINTEL(device, pInitializeInfo);
}
static PFN_vkUninitializePerformanceApiINTEL p_vkUninitializePerformanceApiINTEL;
VKAPI_ATTR void VKAPI_CALL vkUninitializePerformanceApiINTEL(VkDevice device) {
    if (!p_vkUninitializePerformanceApiINTEL) p_vkUninitializePerformanceApiINTEL = (PFN_vkUninitializePerformanceApiINTEL)ps5_vk_proc("vkUninitializePerformanceApiINTEL");
    p_vkUninitializePerformanceApiINTEL(device);
}
static PFN_vkCmdSetPerformanceMarkerINTEL p_vkCmdSetPerformanceMarkerINTEL;
VKAPI_ATTR VkResult VKAPI_CALL vkCmdSetPerformanceMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo) {
    if (!p_vkCmdSetPerformanceMarkerINTEL) p_vkCmdSetPerformanceMarkerINTEL = (PFN_vkCmdSetPerformanceMarkerINTEL)ps5_vk_proc("vkCmdSetPerformanceMarkerINTEL");
    return p_vkCmdSetPerformanceMarkerINTEL(commandBuffer, pMarkerInfo);
}
static PFN_vkCmdSetPerformanceStreamMarkerINTEL p_vkCmdSetPerformanceStreamMarkerINTEL;
VKAPI_ATTR VkResult VKAPI_CALL vkCmdSetPerformanceStreamMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo) {
    if (!p_vkCmdSetPerformanceStreamMarkerINTEL) p_vkCmdSetPerformanceStreamMarkerINTEL = (PFN_vkCmdSetPerformanceStreamMarkerINTEL)ps5_vk_proc("vkCmdSetPerformanceStreamMarkerINTEL");
    return p_vkCmdSetPerformanceStreamMarkerINTEL(commandBuffer, pMarkerInfo);
}
static PFN_vkCmdSetPerformanceOverrideINTEL p_vkCmdSetPerformanceOverrideINTEL;
VKAPI_ATTR VkResult VKAPI_CALL vkCmdSetPerformanceOverrideINTEL(VkCommandBuffer commandBuffer, const VkPerformanceOverrideInfoINTEL* pOverrideInfo) {
    if (!p_vkCmdSetPerformanceOverrideINTEL) p_vkCmdSetPerformanceOverrideINTEL = (PFN_vkCmdSetPerformanceOverrideINTEL)ps5_vk_proc("vkCmdSetPerformanceOverrideINTEL");
    return p_vkCmdSetPerformanceOverrideINTEL(commandBuffer, pOverrideInfo);
}
static PFN_vkAcquirePerformanceConfigurationINTEL p_vkAcquirePerformanceConfigurationINTEL;
VKAPI_ATTR VkResult VKAPI_CALL vkAcquirePerformanceConfigurationINTEL(VkDevice device, const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration) {
    if (!p_vkAcquirePerformanceConfigurationINTEL) p_vkAcquirePerformanceConfigurationINTEL = (PFN_vkAcquirePerformanceConfigurationINTEL)ps5_vk_proc("vkAcquirePerformanceConfigurationINTEL");
    return p_vkAcquirePerformanceConfigurationINTEL(device, pAcquireInfo, pConfiguration);
}
static PFN_vkReleasePerformanceConfigurationINTEL p_vkReleasePerformanceConfigurationINTEL;
VKAPI_ATTR VkResult VKAPI_CALL vkReleasePerformanceConfigurationINTEL(VkDevice device, VkPerformanceConfigurationINTEL configuration) {
    if (!p_vkReleasePerformanceConfigurationINTEL) p_vkReleasePerformanceConfigurationINTEL = (PFN_vkReleasePerformanceConfigurationINTEL)ps5_vk_proc("vkReleasePerformanceConfigurationINTEL");
    return p_vkReleasePerformanceConfigurationINTEL(device, configuration);
}
static PFN_vkQueueSetPerformanceConfigurationINTEL p_vkQueueSetPerformanceConfigurationINTEL;
VKAPI_ATTR VkResult VKAPI_CALL vkQueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration) {
    if (!p_vkQueueSetPerformanceConfigurationINTEL) p_vkQueueSetPerformanceConfigurationINTEL = (PFN_vkQueueSetPerformanceConfigurationINTEL)ps5_vk_proc("vkQueueSetPerformanceConfigurationINTEL");
    return p_vkQueueSetPerformanceConfigurationINTEL(queue, configuration);
}
static PFN_vkGetPerformanceParameterINTEL p_vkGetPerformanceParameterINTEL;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPerformanceParameterINTEL(VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue) {
    if (!p_vkGetPerformanceParameterINTEL) p_vkGetPerformanceParameterINTEL = (PFN_vkGetPerformanceParameterINTEL)ps5_vk_proc("vkGetPerformanceParameterINTEL");
    return p_vkGetPerformanceParameterINTEL(device, parameter, pValue);
}
static PFN_vkSetLocalDimmingAMD p_vkSetLocalDimmingAMD;
VKAPI_ATTR void VKAPI_CALL vkSetLocalDimmingAMD(VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable) {
    if (!p_vkSetLocalDimmingAMD) p_vkSetLocalDimmingAMD = (PFN_vkSetLocalDimmingAMD)ps5_vk_proc("vkSetLocalDimmingAMD");
    p_vkSetLocalDimmingAMD(device, swapChain, localDimmingEnable);
}
static PFN_vkGetBufferDeviceAddressEXT p_vkGetBufferDeviceAddressEXT;
VKAPI_ATTR VkDeviceAddress VKAPI_CALL vkGetBufferDeviceAddressEXT(VkDevice device, const VkBufferDeviceAddressInfo* pInfo) {
    if (!p_vkGetBufferDeviceAddressEXT) p_vkGetBufferDeviceAddressEXT = (PFN_vkGetBufferDeviceAddressEXT)ps5_vk_proc("vkGetBufferDeviceAddressEXT");
    return p_vkGetBufferDeviceAddressEXT(device, pInfo);
}
static PFN_vkGetPhysicalDeviceToolPropertiesEXT p_vkGetPhysicalDeviceToolPropertiesEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceToolPropertiesEXT(VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolProperties* pToolProperties) {
    if (!p_vkGetPhysicalDeviceToolPropertiesEXT) p_vkGetPhysicalDeviceToolPropertiesEXT = (PFN_vkGetPhysicalDeviceToolPropertiesEXT)ps5_vk_proc("vkGetPhysicalDeviceToolPropertiesEXT");
    return p_vkGetPhysicalDeviceToolPropertiesEXT(physicalDevice, pToolCount, pToolProperties);
}
static PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV p_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties) {
    if (!p_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV) p_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV = (PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV)ps5_vk_proc("vkGetPhysicalDeviceCooperativeMatrixPropertiesNV");
    return p_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(physicalDevice, pPropertyCount, pProperties);
}
static PFN_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV p_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(VkPhysicalDevice physicalDevice, uint32_t* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations) {
    if (!p_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV) p_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV = (PFN_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV)ps5_vk_proc("vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV");
    return p_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(physicalDevice, pCombinationCount, pCombinations);
}
static PFN_vkCreateHeadlessSurfaceEXT p_vkCreateHeadlessSurfaceEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateHeadlessSurfaceEXT(VkInstance instance, const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
    if (!p_vkCreateHeadlessSurfaceEXT) p_vkCreateHeadlessSurfaceEXT = (PFN_vkCreateHeadlessSurfaceEXT)ps5_vk_proc("vkCreateHeadlessSurfaceEXT");
    return p_vkCreateHeadlessSurfaceEXT(instance, pCreateInfo, pAllocator, pSurface);
}
static PFN_vkCmdSetLineStippleEXT p_vkCmdSetLineStippleEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) {
    if (!p_vkCmdSetLineStippleEXT) p_vkCmdSetLineStippleEXT = (PFN_vkCmdSetLineStippleEXT)ps5_vk_proc("vkCmdSetLineStippleEXT");
    p_vkCmdSetLineStippleEXT(commandBuffer, lineStippleFactor, lineStipplePattern);
}
static PFN_vkResetQueryPoolEXT p_vkResetQueryPoolEXT;
VKAPI_ATTR void VKAPI_CALL vkResetQueryPoolEXT(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) {
    if (!p_vkResetQueryPoolEXT) p_vkResetQueryPoolEXT = (PFN_vkResetQueryPoolEXT)ps5_vk_proc("vkResetQueryPoolEXT");
    p_vkResetQueryPoolEXT(device, queryPool, firstQuery, queryCount);
}
static PFN_vkCmdSetCullModeEXT p_vkCmdSetCullModeEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetCullModeEXT(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) {
    if (!p_vkCmdSetCullModeEXT) p_vkCmdSetCullModeEXT = (PFN_vkCmdSetCullModeEXT)ps5_vk_proc("vkCmdSetCullModeEXT");
    p_vkCmdSetCullModeEXT(commandBuffer, cullMode);
}
static PFN_vkCmdSetFrontFaceEXT p_vkCmdSetFrontFaceEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetFrontFaceEXT(VkCommandBuffer commandBuffer, VkFrontFace frontFace) {
    if (!p_vkCmdSetFrontFaceEXT) p_vkCmdSetFrontFaceEXT = (PFN_vkCmdSetFrontFaceEXT)ps5_vk_proc("vkCmdSetFrontFaceEXT");
    p_vkCmdSetFrontFaceEXT(commandBuffer, frontFace);
}
static PFN_vkCmdSetPrimitiveTopologyEXT p_vkCmdSetPrimitiveTopologyEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetPrimitiveTopologyEXT(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) {
    if (!p_vkCmdSetPrimitiveTopologyEXT) p_vkCmdSetPrimitiveTopologyEXT = (PFN_vkCmdSetPrimitiveTopologyEXT)ps5_vk_proc("vkCmdSetPrimitiveTopologyEXT");
    p_vkCmdSetPrimitiveTopologyEXT(commandBuffer, primitiveTopology);
}
static PFN_vkCmdSetViewportWithCountEXT p_vkCmdSetViewportWithCountEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetViewportWithCountEXT(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) {
    if (!p_vkCmdSetViewportWithCountEXT) p_vkCmdSetViewportWithCountEXT = (PFN_vkCmdSetViewportWithCountEXT)ps5_vk_proc("vkCmdSetViewportWithCountEXT");
    p_vkCmdSetViewportWithCountEXT(commandBuffer, viewportCount, pViewports);
}
static PFN_vkCmdSetScissorWithCountEXT p_vkCmdSetScissorWithCountEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetScissorWithCountEXT(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) {
    if (!p_vkCmdSetScissorWithCountEXT) p_vkCmdSetScissorWithCountEXT = (PFN_vkCmdSetScissorWithCountEXT)ps5_vk_proc("vkCmdSetScissorWithCountEXT");
    p_vkCmdSetScissorWithCountEXT(commandBuffer, scissorCount, pScissors);
}
static PFN_vkCmdBindVertexBuffers2EXT p_vkCmdBindVertexBuffers2EXT;
VKAPI_ATTR void VKAPI_CALL vkCmdBindVertexBuffers2EXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) {
    if (!p_vkCmdBindVertexBuffers2EXT) p_vkCmdBindVertexBuffers2EXT = (PFN_vkCmdBindVertexBuffers2EXT)ps5_vk_proc("vkCmdBindVertexBuffers2EXT");
    p_vkCmdBindVertexBuffers2EXT(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);
}
static PFN_vkCmdSetDepthTestEnableEXT p_vkCmdSetDepthTestEnableEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) {
    if (!p_vkCmdSetDepthTestEnableEXT) p_vkCmdSetDepthTestEnableEXT = (PFN_vkCmdSetDepthTestEnableEXT)ps5_vk_proc("vkCmdSetDepthTestEnableEXT");
    p_vkCmdSetDepthTestEnableEXT(commandBuffer, depthTestEnable);
}
static PFN_vkCmdSetDepthWriteEnableEXT p_vkCmdSetDepthWriteEnableEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthWriteEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) {
    if (!p_vkCmdSetDepthWriteEnableEXT) p_vkCmdSetDepthWriteEnableEXT = (PFN_vkCmdSetDepthWriteEnableEXT)ps5_vk_proc("vkCmdSetDepthWriteEnableEXT");
    p_vkCmdSetDepthWriteEnableEXT(commandBuffer, depthWriteEnable);
}
static PFN_vkCmdSetDepthCompareOpEXT p_vkCmdSetDepthCompareOpEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthCompareOpEXT(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) {
    if (!p_vkCmdSetDepthCompareOpEXT) p_vkCmdSetDepthCompareOpEXT = (PFN_vkCmdSetDepthCompareOpEXT)ps5_vk_proc("vkCmdSetDepthCompareOpEXT");
    p_vkCmdSetDepthCompareOpEXT(commandBuffer, depthCompareOp);
}
static PFN_vkCmdSetDepthBoundsTestEnableEXT p_vkCmdSetDepthBoundsTestEnableEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthBoundsTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) {
    if (!p_vkCmdSetDepthBoundsTestEnableEXT) p_vkCmdSetDepthBoundsTestEnableEXT = (PFN_vkCmdSetDepthBoundsTestEnableEXT)ps5_vk_proc("vkCmdSetDepthBoundsTestEnableEXT");
    p_vkCmdSetDepthBoundsTestEnableEXT(commandBuffer, depthBoundsTestEnable);
}
static PFN_vkCmdSetStencilTestEnableEXT p_vkCmdSetStencilTestEnableEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetStencilTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) {
    if (!p_vkCmdSetStencilTestEnableEXT) p_vkCmdSetStencilTestEnableEXT = (PFN_vkCmdSetStencilTestEnableEXT)ps5_vk_proc("vkCmdSetStencilTestEnableEXT");
    p_vkCmdSetStencilTestEnableEXT(commandBuffer, stencilTestEnable);
}
static PFN_vkCmdSetStencilOpEXT p_vkCmdSetStencilOpEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetStencilOpEXT(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) {
    if (!p_vkCmdSetStencilOpEXT) p_vkCmdSetStencilOpEXT = (PFN_vkCmdSetStencilOpEXT)ps5_vk_proc("vkCmdSetStencilOpEXT");
    p_vkCmdSetStencilOpEXT(commandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);
}
static PFN_vkCopyMemoryToImageEXT p_vkCopyMemoryToImageEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkCopyMemoryToImageEXT(VkDevice device, const VkCopyMemoryToImageInfo* pCopyMemoryToImageInfo) {
    if (!p_vkCopyMemoryToImageEXT) p_vkCopyMemoryToImageEXT = (PFN_vkCopyMemoryToImageEXT)ps5_vk_proc("vkCopyMemoryToImageEXT");
    return p_vkCopyMemoryToImageEXT(device, pCopyMemoryToImageInfo);
}
static PFN_vkCopyImageToMemoryEXT p_vkCopyImageToMemoryEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkCopyImageToMemoryEXT(VkDevice device, const VkCopyImageToMemoryInfo* pCopyImageToMemoryInfo) {
    if (!p_vkCopyImageToMemoryEXT) p_vkCopyImageToMemoryEXT = (PFN_vkCopyImageToMemoryEXT)ps5_vk_proc("vkCopyImageToMemoryEXT");
    return p_vkCopyImageToMemoryEXT(device, pCopyImageToMemoryInfo);
}
static PFN_vkCopyImageToImageEXT p_vkCopyImageToImageEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkCopyImageToImageEXT(VkDevice device, const VkCopyImageToImageInfo* pCopyImageToImageInfo) {
    if (!p_vkCopyImageToImageEXT) p_vkCopyImageToImageEXT = (PFN_vkCopyImageToImageEXT)ps5_vk_proc("vkCopyImageToImageEXT");
    return p_vkCopyImageToImageEXT(device, pCopyImageToImageInfo);
}
static PFN_vkTransitionImageLayoutEXT p_vkTransitionImageLayoutEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkTransitionImageLayoutEXT(VkDevice device, uint32_t transitionCount, const VkHostImageLayoutTransitionInfo* pTransitions) {
    if (!p_vkTransitionImageLayoutEXT) p_vkTransitionImageLayoutEXT = (PFN_vkTransitionImageLayoutEXT)ps5_vk_proc("vkTransitionImageLayoutEXT");
    return p_vkTransitionImageLayoutEXT(device, transitionCount, pTransitions);
}
static PFN_vkGetImageSubresourceLayout2EXT p_vkGetImageSubresourceLayout2EXT;
VKAPI_ATTR void VKAPI_CALL vkGetImageSubresourceLayout2EXT(VkDevice device, VkImage image, const VkImageSubresource2* pSubresource, VkSubresourceLayout2* pLayout) {
    if (!p_vkGetImageSubresourceLayout2EXT) p_vkGetImageSubresourceLayout2EXT = (PFN_vkGetImageSubresourceLayout2EXT)ps5_vk_proc("vkGetImageSubresourceLayout2EXT");
    p_vkGetImageSubresourceLayout2EXT(device, image, pSubresource, pLayout);
}
static PFN_vkReleaseSwapchainImagesEXT p_vkReleaseSwapchainImagesEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkReleaseSwapchainImagesEXT(VkDevice device, const VkReleaseSwapchainImagesInfoKHR* pReleaseInfo) {
    if (!p_vkReleaseSwapchainImagesEXT) p_vkReleaseSwapchainImagesEXT = (PFN_vkReleaseSwapchainImagesEXT)ps5_vk_proc("vkReleaseSwapchainImagesEXT");
    return p_vkReleaseSwapchainImagesEXT(device, pReleaseInfo);
}
static PFN_vkGetGeneratedCommandsMemoryRequirementsNV p_vkGetGeneratedCommandsMemoryRequirementsNV;
VKAPI_ATTR void VKAPI_CALL vkGetGeneratedCommandsMemoryRequirementsNV(VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
    if (!p_vkGetGeneratedCommandsMemoryRequirementsNV) p_vkGetGeneratedCommandsMemoryRequirementsNV = (PFN_vkGetGeneratedCommandsMemoryRequirementsNV)ps5_vk_proc("vkGetGeneratedCommandsMemoryRequirementsNV");
    p_vkGetGeneratedCommandsMemoryRequirementsNV(device, pInfo, pMemoryRequirements);
}
static PFN_vkCmdPreprocessGeneratedCommandsNV p_vkCmdPreprocessGeneratedCommandsNV;
VKAPI_ATTR void VKAPI_CALL vkCmdPreprocessGeneratedCommandsNV(VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) {
    if (!p_vkCmdPreprocessGeneratedCommandsNV) p_vkCmdPreprocessGeneratedCommandsNV = (PFN_vkCmdPreprocessGeneratedCommandsNV)ps5_vk_proc("vkCmdPreprocessGeneratedCommandsNV");
    p_vkCmdPreprocessGeneratedCommandsNV(commandBuffer, pGeneratedCommandsInfo);
}
static PFN_vkCmdExecuteGeneratedCommandsNV p_vkCmdExecuteGeneratedCommandsNV;
VKAPI_ATTR void VKAPI_CALL vkCmdExecuteGeneratedCommandsNV(VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) {
    if (!p_vkCmdExecuteGeneratedCommandsNV) p_vkCmdExecuteGeneratedCommandsNV = (PFN_vkCmdExecuteGeneratedCommandsNV)ps5_vk_proc("vkCmdExecuteGeneratedCommandsNV");
    p_vkCmdExecuteGeneratedCommandsNV(commandBuffer, isPreprocessed, pGeneratedCommandsInfo);
}
static PFN_vkCmdBindPipelineShaderGroupNV p_vkCmdBindPipelineShaderGroupNV;
VKAPI_ATTR void VKAPI_CALL vkCmdBindPipelineShaderGroupNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex) {
    if (!p_vkCmdBindPipelineShaderGroupNV) p_vkCmdBindPipelineShaderGroupNV = (PFN_vkCmdBindPipelineShaderGroupNV)ps5_vk_proc("vkCmdBindPipelineShaderGroupNV");
    p_vkCmdBindPipelineShaderGroupNV(commandBuffer, pipelineBindPoint, pipeline, groupIndex);
}
static PFN_vkCreateIndirectCommandsLayoutNV p_vkCreateIndirectCommandsLayoutNV;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateIndirectCommandsLayoutNV(VkDevice device, const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout) {
    if (!p_vkCreateIndirectCommandsLayoutNV) p_vkCreateIndirectCommandsLayoutNV = (PFN_vkCreateIndirectCommandsLayoutNV)ps5_vk_proc("vkCreateIndirectCommandsLayoutNV");
    return p_vkCreateIndirectCommandsLayoutNV(device, pCreateInfo, pAllocator, pIndirectCommandsLayout);
}
static PFN_vkDestroyIndirectCommandsLayoutNV p_vkDestroyIndirectCommandsLayoutNV;
VKAPI_ATTR void VKAPI_CALL vkDestroyIndirectCommandsLayoutNV(VkDevice device, VkIndirectCommandsLayoutNV indirectCommandsLayout, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyIndirectCommandsLayoutNV) p_vkDestroyIndirectCommandsLayoutNV = (PFN_vkDestroyIndirectCommandsLayoutNV)ps5_vk_proc("vkDestroyIndirectCommandsLayoutNV");
    p_vkDestroyIndirectCommandsLayoutNV(device, indirectCommandsLayout, pAllocator);
}
static PFN_vkCmdSetDepthBias2EXT p_vkCmdSetDepthBias2EXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthBias2EXT(VkCommandBuffer commandBuffer, const VkDepthBiasInfoEXT* pDepthBiasInfo) {
    if (!p_vkCmdSetDepthBias2EXT) p_vkCmdSetDepthBias2EXT = (PFN_vkCmdSetDepthBias2EXT)ps5_vk_proc("vkCmdSetDepthBias2EXT");
    p_vkCmdSetDepthBias2EXT(commandBuffer, pDepthBiasInfo);
}
static PFN_vkAcquireDrmDisplayEXT p_vkAcquireDrmDisplayEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkAcquireDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display) {
    if (!p_vkAcquireDrmDisplayEXT) p_vkAcquireDrmDisplayEXT = (PFN_vkAcquireDrmDisplayEXT)ps5_vk_proc("vkAcquireDrmDisplayEXT");
    return p_vkAcquireDrmDisplayEXT(physicalDevice, drmFd, display);
}
static PFN_vkGetDrmDisplayEXT p_vkGetDrmDisplayEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkGetDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, uint32_t connectorId, VkDisplayKHR* display) {
    if (!p_vkGetDrmDisplayEXT) p_vkGetDrmDisplayEXT = (PFN_vkGetDrmDisplayEXT)ps5_vk_proc("vkGetDrmDisplayEXT");
    return p_vkGetDrmDisplayEXT(physicalDevice, drmFd, connectorId, display);
}
static PFN_vkCreatePrivateDataSlotEXT p_vkCreatePrivateDataSlotEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkCreatePrivateDataSlotEXT(VkDevice device, const VkPrivateDataSlotCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlot* pPrivateDataSlot) {
    if (!p_vkCreatePrivateDataSlotEXT) p_vkCreatePrivateDataSlotEXT = (PFN_vkCreatePrivateDataSlotEXT)ps5_vk_proc("vkCreatePrivateDataSlotEXT");
    return p_vkCreatePrivateDataSlotEXT(device, pCreateInfo, pAllocator, pPrivateDataSlot);
}
static PFN_vkDestroyPrivateDataSlotEXT p_vkDestroyPrivateDataSlotEXT;
VKAPI_ATTR void VKAPI_CALL vkDestroyPrivateDataSlotEXT(VkDevice device, VkPrivateDataSlot privateDataSlot, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyPrivateDataSlotEXT) p_vkDestroyPrivateDataSlotEXT = (PFN_vkDestroyPrivateDataSlotEXT)ps5_vk_proc("vkDestroyPrivateDataSlotEXT");
    p_vkDestroyPrivateDataSlotEXT(device, privateDataSlot, pAllocator);
}
static PFN_vkSetPrivateDataEXT p_vkSetPrivateDataEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkSetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t data) {
    if (!p_vkSetPrivateDataEXT) p_vkSetPrivateDataEXT = (PFN_vkSetPrivateDataEXT)ps5_vk_proc("vkSetPrivateDataEXT");
    return p_vkSetPrivateDataEXT(device, objectType, objectHandle, privateDataSlot, data);
}
static PFN_vkGetPrivateDataEXT p_vkGetPrivateDataEXT;
VKAPI_ATTR void VKAPI_CALL vkGetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlot privateDataSlot, uint64_t* pData) {
    if (!p_vkGetPrivateDataEXT) p_vkGetPrivateDataEXT = (PFN_vkGetPrivateDataEXT)ps5_vk_proc("vkGetPrivateDataEXT");
    p_vkGetPrivateDataEXT(device, objectType, objectHandle, privateDataSlot, pData);
}
static PFN_vkQueueSetPerfHintQCOM p_vkQueueSetPerfHintQCOM;
VKAPI_ATTR VkResult VKAPI_CALL vkQueueSetPerfHintQCOM(VkQueue queue, const VkPerfHintInfoQCOM* pPerfHintInfo) {
    if (!p_vkQueueSetPerfHintQCOM) p_vkQueueSetPerfHintQCOM = (PFN_vkQueueSetPerfHintQCOM)ps5_vk_proc("vkQueueSetPerfHintQCOM");
    return p_vkQueueSetPerfHintQCOM(queue, pPerfHintInfo);
}
static PFN_vkCmdDispatchTileQCOM p_vkCmdDispatchTileQCOM;
VKAPI_ATTR void VKAPI_CALL vkCmdDispatchTileQCOM(VkCommandBuffer commandBuffer, const VkDispatchTileInfoQCOM* pDispatchTileInfo) {
    if (!p_vkCmdDispatchTileQCOM) p_vkCmdDispatchTileQCOM = (PFN_vkCmdDispatchTileQCOM)ps5_vk_proc("vkCmdDispatchTileQCOM");
    p_vkCmdDispatchTileQCOM(commandBuffer, pDispatchTileInfo);
}
static PFN_vkCmdBeginPerTileExecutionQCOM p_vkCmdBeginPerTileExecutionQCOM;
VKAPI_ATTR void VKAPI_CALL vkCmdBeginPerTileExecutionQCOM(VkCommandBuffer commandBuffer, const VkPerTileBeginInfoQCOM* pPerTileBeginInfo) {
    if (!p_vkCmdBeginPerTileExecutionQCOM) p_vkCmdBeginPerTileExecutionQCOM = (PFN_vkCmdBeginPerTileExecutionQCOM)ps5_vk_proc("vkCmdBeginPerTileExecutionQCOM");
    p_vkCmdBeginPerTileExecutionQCOM(commandBuffer, pPerTileBeginInfo);
}
static PFN_vkCmdEndPerTileExecutionQCOM p_vkCmdEndPerTileExecutionQCOM;
VKAPI_ATTR void VKAPI_CALL vkCmdEndPerTileExecutionQCOM(VkCommandBuffer commandBuffer, const VkPerTileEndInfoQCOM* pPerTileEndInfo) {
    if (!p_vkCmdEndPerTileExecutionQCOM) p_vkCmdEndPerTileExecutionQCOM = (PFN_vkCmdEndPerTileExecutionQCOM)ps5_vk_proc("vkCmdEndPerTileExecutionQCOM");
    p_vkCmdEndPerTileExecutionQCOM(commandBuffer, pPerTileEndInfo);
}
static PFN_vkGetDescriptorSetLayoutSizeEXT p_vkGetDescriptorSetLayoutSizeEXT;
VKAPI_ATTR void VKAPI_CALL vkGetDescriptorSetLayoutSizeEXT(VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize* pLayoutSizeInBytes) {
    if (!p_vkGetDescriptorSetLayoutSizeEXT) p_vkGetDescriptorSetLayoutSizeEXT = (PFN_vkGetDescriptorSetLayoutSizeEXT)ps5_vk_proc("vkGetDescriptorSetLayoutSizeEXT");
    p_vkGetDescriptorSetLayoutSizeEXT(device, layout, pLayoutSizeInBytes);
}
static PFN_vkGetDescriptorSetLayoutBindingOffsetEXT p_vkGetDescriptorSetLayoutBindingOffsetEXT;
VKAPI_ATTR void VKAPI_CALL vkGetDescriptorSetLayoutBindingOffsetEXT(VkDevice device, VkDescriptorSetLayout layout, uint32_t binding, VkDeviceSize* pOffset) {
    if (!p_vkGetDescriptorSetLayoutBindingOffsetEXT) p_vkGetDescriptorSetLayoutBindingOffsetEXT = (PFN_vkGetDescriptorSetLayoutBindingOffsetEXT)ps5_vk_proc("vkGetDescriptorSetLayoutBindingOffsetEXT");
    p_vkGetDescriptorSetLayoutBindingOffsetEXT(device, layout, binding, pOffset);
}
static PFN_vkGetDescriptorEXT p_vkGetDescriptorEXT;
VKAPI_ATTR void VKAPI_CALL vkGetDescriptorEXT(VkDevice device, const VkDescriptorGetInfoEXT* pDescriptorInfo, size_t dataSize, void* pDescriptor) {
    if (!p_vkGetDescriptorEXT) p_vkGetDescriptorEXT = (PFN_vkGetDescriptorEXT)ps5_vk_proc("vkGetDescriptorEXT");
    p_vkGetDescriptorEXT(device, pDescriptorInfo, dataSize, pDescriptor);
}
static PFN_vkCmdBindDescriptorBuffersEXT p_vkCmdBindDescriptorBuffersEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdBindDescriptorBuffersEXT(VkCommandBuffer commandBuffer, uint32_t bufferCount, const VkDescriptorBufferBindingInfoEXT* pBindingInfos) {
    if (!p_vkCmdBindDescriptorBuffersEXT) p_vkCmdBindDescriptorBuffersEXT = (PFN_vkCmdBindDescriptorBuffersEXT)ps5_vk_proc("vkCmdBindDescriptorBuffersEXT");
    p_vkCmdBindDescriptorBuffersEXT(commandBuffer, bufferCount, pBindingInfos);
}
static PFN_vkCmdSetDescriptorBufferOffsetsEXT p_vkCmdSetDescriptorBufferOffsetsEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDescriptorBufferOffsetsEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, const uint32_t* pBufferIndices, const VkDeviceSize* pOffsets) {
    if (!p_vkCmdSetDescriptorBufferOffsetsEXT) p_vkCmdSetDescriptorBufferOffsetsEXT = (PFN_vkCmdSetDescriptorBufferOffsetsEXT)ps5_vk_proc("vkCmdSetDescriptorBufferOffsetsEXT");
    p_vkCmdSetDescriptorBufferOffsetsEXT(commandBuffer, pipelineBindPoint, layout, firstSet, setCount, pBufferIndices, pOffsets);
}
static PFN_vkCmdBindDescriptorBufferEmbeddedSamplersEXT p_vkCmdBindDescriptorBufferEmbeddedSamplersEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdBindDescriptorBufferEmbeddedSamplersEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set) {
    if (!p_vkCmdBindDescriptorBufferEmbeddedSamplersEXT) p_vkCmdBindDescriptorBufferEmbeddedSamplersEXT = (PFN_vkCmdBindDescriptorBufferEmbeddedSamplersEXT)ps5_vk_proc("vkCmdBindDescriptorBufferEmbeddedSamplersEXT");
    p_vkCmdBindDescriptorBufferEmbeddedSamplersEXT(commandBuffer, pipelineBindPoint, layout, set);
}
static PFN_vkGetBufferOpaqueCaptureDescriptorDataEXT p_vkGetBufferOpaqueCaptureDescriptorDataEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkGetBufferOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkBufferCaptureDescriptorDataInfoEXT* pInfo, void* pData) {
    if (!p_vkGetBufferOpaqueCaptureDescriptorDataEXT) p_vkGetBufferOpaqueCaptureDescriptorDataEXT = (PFN_vkGetBufferOpaqueCaptureDescriptorDataEXT)ps5_vk_proc("vkGetBufferOpaqueCaptureDescriptorDataEXT");
    return p_vkGetBufferOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
}
static PFN_vkGetImageOpaqueCaptureDescriptorDataEXT p_vkGetImageOpaqueCaptureDescriptorDataEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkGetImageOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageCaptureDescriptorDataInfoEXT* pInfo, void* pData) {
    if (!p_vkGetImageOpaqueCaptureDescriptorDataEXT) p_vkGetImageOpaqueCaptureDescriptorDataEXT = (PFN_vkGetImageOpaqueCaptureDescriptorDataEXT)ps5_vk_proc("vkGetImageOpaqueCaptureDescriptorDataEXT");
    return p_vkGetImageOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
}
static PFN_vkGetImageViewOpaqueCaptureDescriptorDataEXT p_vkGetImageViewOpaqueCaptureDescriptorDataEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkGetImageViewOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageViewCaptureDescriptorDataInfoEXT* pInfo, void* pData) {
    if (!p_vkGetImageViewOpaqueCaptureDescriptorDataEXT) p_vkGetImageViewOpaqueCaptureDescriptorDataEXT = (PFN_vkGetImageViewOpaqueCaptureDescriptorDataEXT)ps5_vk_proc("vkGetImageViewOpaqueCaptureDescriptorDataEXT");
    return p_vkGetImageViewOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
}
static PFN_vkGetSamplerOpaqueCaptureDescriptorDataEXT p_vkGetSamplerOpaqueCaptureDescriptorDataEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkGetSamplerOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkSamplerCaptureDescriptorDataInfoEXT* pInfo, void* pData) {
    if (!p_vkGetSamplerOpaqueCaptureDescriptorDataEXT) p_vkGetSamplerOpaqueCaptureDescriptorDataEXT = (PFN_vkGetSamplerOpaqueCaptureDescriptorDataEXT)ps5_vk_proc("vkGetSamplerOpaqueCaptureDescriptorDataEXT");
    return p_vkGetSamplerOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
}
static PFN_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT p_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkAccelerationStructureCaptureDescriptorDataInfoEXT* pInfo, void* pData) {
    if (!p_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT) p_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT = (PFN_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT)ps5_vk_proc("vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT");
    return p_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT(device, pInfo, pData);
}
static PFN_vkCmdSetFragmentShadingRateEnumNV p_vkCmdSetFragmentShadingRateEnumNV;
VKAPI_ATTR void VKAPI_CALL vkCmdSetFragmentShadingRateEnumNV(VkCommandBuffer commandBuffer, VkFragmentShadingRateNV shadingRate, const VkFragmentShadingRateCombinerOpKHR combinerOps[2]) {
    if (!p_vkCmdSetFragmentShadingRateEnumNV) p_vkCmdSetFragmentShadingRateEnumNV = (PFN_vkCmdSetFragmentShadingRateEnumNV)ps5_vk_proc("vkCmdSetFragmentShadingRateEnumNV");
    p_vkCmdSetFragmentShadingRateEnumNV(commandBuffer, shadingRate, combinerOps);
}
static PFN_vkGetDeviceFaultInfoEXT p_vkGetDeviceFaultInfoEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkGetDeviceFaultInfoEXT(VkDevice device, VkDeviceFaultCountsEXT* pFaultCounts, VkDeviceFaultInfoEXT* pFaultInfo) {
    if (!p_vkGetDeviceFaultInfoEXT) p_vkGetDeviceFaultInfoEXT = (PFN_vkGetDeviceFaultInfoEXT)ps5_vk_proc("vkGetDeviceFaultInfoEXT");
    return p_vkGetDeviceFaultInfoEXT(device, pFaultCounts, pFaultInfo);
}
static PFN_vkCmdSetVertexInputEXT p_vkCmdSetVertexInputEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetVertexInputEXT(VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions) {
    if (!p_vkCmdSetVertexInputEXT) p_vkCmdSetVertexInputEXT = (PFN_vkCmdSetVertexInputEXT)ps5_vk_proc("vkCmdSetVertexInputEXT");
    p_vkCmdSetVertexInputEXT(commandBuffer, vertexBindingDescriptionCount, pVertexBindingDescriptions, vertexAttributeDescriptionCount, pVertexAttributeDescriptions);
}
static PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI p_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI;
VKAPI_ATTR VkResult VKAPI_CALL vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(VkDevice device, VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize) {
    if (!p_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI) p_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI = (PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI)ps5_vk_proc("vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI");
    return p_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(device, renderpass, pMaxWorkgroupSize);
}
static PFN_vkCmdSubpassShadingHUAWEI p_vkCmdSubpassShadingHUAWEI;
VKAPI_ATTR void VKAPI_CALL vkCmdSubpassShadingHUAWEI(VkCommandBuffer commandBuffer) {
    if (!p_vkCmdSubpassShadingHUAWEI) p_vkCmdSubpassShadingHUAWEI = (PFN_vkCmdSubpassShadingHUAWEI)ps5_vk_proc("vkCmdSubpassShadingHUAWEI");
    p_vkCmdSubpassShadingHUAWEI(commandBuffer);
}
static PFN_vkCmdBindInvocationMaskHUAWEI p_vkCmdBindInvocationMaskHUAWEI;
VKAPI_ATTR void VKAPI_CALL vkCmdBindInvocationMaskHUAWEI(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) {
    if (!p_vkCmdBindInvocationMaskHUAWEI) p_vkCmdBindInvocationMaskHUAWEI = (PFN_vkCmdBindInvocationMaskHUAWEI)ps5_vk_proc("vkCmdBindInvocationMaskHUAWEI");
    p_vkCmdBindInvocationMaskHUAWEI(commandBuffer, imageView, imageLayout);
}
static PFN_vkGetMemoryRemoteAddressNV p_vkGetMemoryRemoteAddressNV;
VKAPI_ATTR VkResult VKAPI_CALL vkGetMemoryRemoteAddressNV(VkDevice device, const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress) {
    if (!p_vkGetMemoryRemoteAddressNV) p_vkGetMemoryRemoteAddressNV = (PFN_vkGetMemoryRemoteAddressNV)ps5_vk_proc("vkGetMemoryRemoteAddressNV");
    return p_vkGetMemoryRemoteAddressNV(device, pMemoryGetRemoteAddressInfo, pAddress);
}
static PFN_vkGetPipelinePropertiesEXT p_vkGetPipelinePropertiesEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPipelinePropertiesEXT(VkDevice device, const VkPipelineInfoKHR* pPipelineInfo, VkBaseOutStructure* pPipelineProperties) {
    if (!p_vkGetPipelinePropertiesEXT) p_vkGetPipelinePropertiesEXT = (PFN_vkGetPipelinePropertiesEXT)ps5_vk_proc("vkGetPipelinePropertiesEXT");
    return p_vkGetPipelinePropertiesEXT(device, pPipelineInfo, pPipelineProperties);
}
static PFN_vkCmdSetPatchControlPointsEXT p_vkCmdSetPatchControlPointsEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints) {
    if (!p_vkCmdSetPatchControlPointsEXT) p_vkCmdSetPatchControlPointsEXT = (PFN_vkCmdSetPatchControlPointsEXT)ps5_vk_proc("vkCmdSetPatchControlPointsEXT");
    p_vkCmdSetPatchControlPointsEXT(commandBuffer, patchControlPoints);
}
static PFN_vkCmdSetRasterizerDiscardEnableEXT p_vkCmdSetRasterizerDiscardEnableEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetRasterizerDiscardEnableEXT(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) {
    if (!p_vkCmdSetRasterizerDiscardEnableEXT) p_vkCmdSetRasterizerDiscardEnableEXT = (PFN_vkCmdSetRasterizerDiscardEnableEXT)ps5_vk_proc("vkCmdSetRasterizerDiscardEnableEXT");
    p_vkCmdSetRasterizerDiscardEnableEXT(commandBuffer, rasterizerDiscardEnable);
}
static PFN_vkCmdSetDepthBiasEnableEXT p_vkCmdSetDepthBiasEnableEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthBiasEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) {
    if (!p_vkCmdSetDepthBiasEnableEXT) p_vkCmdSetDepthBiasEnableEXT = (PFN_vkCmdSetDepthBiasEnableEXT)ps5_vk_proc("vkCmdSetDepthBiasEnableEXT");
    p_vkCmdSetDepthBiasEnableEXT(commandBuffer, depthBiasEnable);
}
static PFN_vkCmdSetLogicOpEXT p_vkCmdSetLogicOpEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp) {
    if (!p_vkCmdSetLogicOpEXT) p_vkCmdSetLogicOpEXT = (PFN_vkCmdSetLogicOpEXT)ps5_vk_proc("vkCmdSetLogicOpEXT");
    p_vkCmdSetLogicOpEXT(commandBuffer, logicOp);
}
static PFN_vkCmdSetPrimitiveRestartEnableEXT p_vkCmdSetPrimitiveRestartEnableEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetPrimitiveRestartEnableEXT(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) {
    if (!p_vkCmdSetPrimitiveRestartEnableEXT) p_vkCmdSetPrimitiveRestartEnableEXT = (PFN_vkCmdSetPrimitiveRestartEnableEXT)ps5_vk_proc("vkCmdSetPrimitiveRestartEnableEXT");
    p_vkCmdSetPrimitiveRestartEnableEXT(commandBuffer, primitiveRestartEnable);
}
static PFN_vkCmdSetColorWriteEnableEXT p_vkCmdSetColorWriteEnableEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetColorWriteEnableEXT(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkBool32* pColorWriteEnables) {
    if (!p_vkCmdSetColorWriteEnableEXT) p_vkCmdSetColorWriteEnableEXT = (PFN_vkCmdSetColorWriteEnableEXT)ps5_vk_proc("vkCmdSetColorWriteEnableEXT");
    p_vkCmdSetColorWriteEnableEXT(commandBuffer, attachmentCount, pColorWriteEnables);
}
static PFN_vkCmdDrawMultiEXT p_vkCmdDrawMultiEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawMultiEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride) {
    if (!p_vkCmdDrawMultiEXT) p_vkCmdDrawMultiEXT = (PFN_vkCmdDrawMultiEXT)ps5_vk_proc("vkCmdDrawMultiEXT");
    p_vkCmdDrawMultiEXT(commandBuffer, drawCount, pVertexInfo, instanceCount, firstInstance, stride);
}
static PFN_vkCmdDrawMultiIndexedEXT p_vkCmdDrawMultiIndexedEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset) {
    if (!p_vkCmdDrawMultiIndexedEXT) p_vkCmdDrawMultiIndexedEXT = (PFN_vkCmdDrawMultiIndexedEXT)ps5_vk_proc("vkCmdDrawMultiIndexedEXT");
    p_vkCmdDrawMultiIndexedEXT(commandBuffer, drawCount, pIndexInfo, instanceCount, firstInstance, stride, pVertexOffset);
}
static PFN_vkCreateMicromapEXT p_vkCreateMicromapEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateMicromapEXT(VkDevice device, const VkMicromapCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkMicromapEXT* pMicromap) {
    if (!p_vkCreateMicromapEXT) p_vkCreateMicromapEXT = (PFN_vkCreateMicromapEXT)ps5_vk_proc("vkCreateMicromapEXT");
    return p_vkCreateMicromapEXT(device, pCreateInfo, pAllocator, pMicromap);
}
static PFN_vkDestroyMicromapEXT p_vkDestroyMicromapEXT;
VKAPI_ATTR void VKAPI_CALL vkDestroyMicromapEXT(VkDevice device, VkMicromapEXT micromap, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyMicromapEXT) p_vkDestroyMicromapEXT = (PFN_vkDestroyMicromapEXT)ps5_vk_proc("vkDestroyMicromapEXT");
    p_vkDestroyMicromapEXT(device, micromap, pAllocator);
}
static PFN_vkCmdBuildMicromapsEXT p_vkCmdBuildMicromapsEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdBuildMicromapsEXT(VkCommandBuffer commandBuffer, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos) {
    if (!p_vkCmdBuildMicromapsEXT) p_vkCmdBuildMicromapsEXT = (PFN_vkCmdBuildMicromapsEXT)ps5_vk_proc("vkCmdBuildMicromapsEXT");
    p_vkCmdBuildMicromapsEXT(commandBuffer, infoCount, pInfos);
}
static PFN_vkBuildMicromapsEXT p_vkBuildMicromapsEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkBuildMicromapsEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos) {
    if (!p_vkBuildMicromapsEXT) p_vkBuildMicromapsEXT = (PFN_vkBuildMicromapsEXT)ps5_vk_proc("vkBuildMicromapsEXT");
    return p_vkBuildMicromapsEXT(device, deferredOperation, infoCount, pInfos);
}
static PFN_vkCopyMicromapEXT p_vkCopyMicromapEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkCopyMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapInfoEXT* pInfo) {
    if (!p_vkCopyMicromapEXT) p_vkCopyMicromapEXT = (PFN_vkCopyMicromapEXT)ps5_vk_proc("vkCopyMicromapEXT");
    return p_vkCopyMicromapEXT(device, deferredOperation, pInfo);
}
static PFN_vkCopyMicromapToMemoryEXT p_vkCopyMicromapToMemoryEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkCopyMicromapToMemoryEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapToMemoryInfoEXT* pInfo) {
    if (!p_vkCopyMicromapToMemoryEXT) p_vkCopyMicromapToMemoryEXT = (PFN_vkCopyMicromapToMemoryEXT)ps5_vk_proc("vkCopyMicromapToMemoryEXT");
    return p_vkCopyMicromapToMemoryEXT(device, deferredOperation, pInfo);
}
static PFN_vkCopyMemoryToMicromapEXT p_vkCopyMemoryToMicromapEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkCopyMemoryToMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToMicromapInfoEXT* pInfo) {
    if (!p_vkCopyMemoryToMicromapEXT) p_vkCopyMemoryToMicromapEXT = (PFN_vkCopyMemoryToMicromapEXT)ps5_vk_proc("vkCopyMemoryToMicromapEXT");
    return p_vkCopyMemoryToMicromapEXT(device, deferredOperation, pInfo);
}
static PFN_vkWriteMicromapsPropertiesEXT p_vkWriteMicromapsPropertiesEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkWriteMicromapsPropertiesEXT(VkDevice device, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, size_t dataSize, void* pData, size_t stride) {
    if (!p_vkWriteMicromapsPropertiesEXT) p_vkWriteMicromapsPropertiesEXT = (PFN_vkWriteMicromapsPropertiesEXT)ps5_vk_proc("vkWriteMicromapsPropertiesEXT");
    return p_vkWriteMicromapsPropertiesEXT(device, micromapCount, pMicromaps, queryType, dataSize, pData, stride);
}
static PFN_vkCmdCopyMicromapEXT p_vkCmdCopyMicromapEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapInfoEXT* pInfo) {
    if (!p_vkCmdCopyMicromapEXT) p_vkCmdCopyMicromapEXT = (PFN_vkCmdCopyMicromapEXT)ps5_vk_proc("vkCmdCopyMicromapEXT");
    p_vkCmdCopyMicromapEXT(commandBuffer, pInfo);
}
static PFN_vkCmdCopyMicromapToMemoryEXT p_vkCmdCopyMicromapToMemoryEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyMicromapToMemoryEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapToMemoryInfoEXT* pInfo) {
    if (!p_vkCmdCopyMicromapToMemoryEXT) p_vkCmdCopyMicromapToMemoryEXT = (PFN_vkCmdCopyMicromapToMemoryEXT)ps5_vk_proc("vkCmdCopyMicromapToMemoryEXT");
    p_vkCmdCopyMicromapToMemoryEXT(commandBuffer, pInfo);
}
static PFN_vkCmdCopyMemoryToMicromapEXT p_vkCmdCopyMemoryToMicromapEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyMemoryToMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMemoryToMicromapInfoEXT* pInfo) {
    if (!p_vkCmdCopyMemoryToMicromapEXT) p_vkCmdCopyMemoryToMicromapEXT = (PFN_vkCmdCopyMemoryToMicromapEXT)ps5_vk_proc("vkCmdCopyMemoryToMicromapEXT");
    p_vkCmdCopyMemoryToMicromapEXT(commandBuffer, pInfo);
}
static PFN_vkCmdWriteMicromapsPropertiesEXT p_vkCmdWriteMicromapsPropertiesEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdWriteMicromapsPropertiesEXT(VkCommandBuffer commandBuffer, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) {
    if (!p_vkCmdWriteMicromapsPropertiesEXT) p_vkCmdWriteMicromapsPropertiesEXT = (PFN_vkCmdWriteMicromapsPropertiesEXT)ps5_vk_proc("vkCmdWriteMicromapsPropertiesEXT");
    p_vkCmdWriteMicromapsPropertiesEXT(commandBuffer, micromapCount, pMicromaps, queryType, queryPool, firstQuery);
}
static PFN_vkGetDeviceMicromapCompatibilityEXT p_vkGetDeviceMicromapCompatibilityEXT;
VKAPI_ATTR void VKAPI_CALL vkGetDeviceMicromapCompatibilityEXT(VkDevice device, const VkMicromapVersionInfoEXT* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) {
    if (!p_vkGetDeviceMicromapCompatibilityEXT) p_vkGetDeviceMicromapCompatibilityEXT = (PFN_vkGetDeviceMicromapCompatibilityEXT)ps5_vk_proc("vkGetDeviceMicromapCompatibilityEXT");
    p_vkGetDeviceMicromapCompatibilityEXT(device, pVersionInfo, pCompatibility);
}
static PFN_vkGetMicromapBuildSizesEXT p_vkGetMicromapBuildSizesEXT;
VKAPI_ATTR void VKAPI_CALL vkGetMicromapBuildSizesEXT(VkDevice device, VkAccelerationStructureBuildTypeKHR buildType, const VkMicromapBuildInfoEXT* pBuildInfo, VkMicromapBuildSizesInfoEXT* pSizeInfo) {
    if (!p_vkGetMicromapBuildSizesEXT) p_vkGetMicromapBuildSizesEXT = (PFN_vkGetMicromapBuildSizesEXT)ps5_vk_proc("vkGetMicromapBuildSizesEXT");
    p_vkGetMicromapBuildSizesEXT(device, buildType, pBuildInfo, pSizeInfo);
}
static PFN_vkCmdDrawClusterHUAWEI p_vkCmdDrawClusterHUAWEI;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawClusterHUAWEI(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
    if (!p_vkCmdDrawClusterHUAWEI) p_vkCmdDrawClusterHUAWEI = (PFN_vkCmdDrawClusterHUAWEI)ps5_vk_proc("vkCmdDrawClusterHUAWEI");
    p_vkCmdDrawClusterHUAWEI(commandBuffer, groupCountX, groupCountY, groupCountZ);
}
static PFN_vkCmdDrawClusterIndirectHUAWEI p_vkCmdDrawClusterIndirectHUAWEI;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawClusterIndirectHUAWEI(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) {
    if (!p_vkCmdDrawClusterIndirectHUAWEI) p_vkCmdDrawClusterIndirectHUAWEI = (PFN_vkCmdDrawClusterIndirectHUAWEI)ps5_vk_proc("vkCmdDrawClusterIndirectHUAWEI");
    p_vkCmdDrawClusterIndirectHUAWEI(commandBuffer, buffer, offset);
}
static PFN_vkSetDeviceMemoryPriorityEXT p_vkSetDeviceMemoryPriorityEXT;
VKAPI_ATTR void VKAPI_CALL vkSetDeviceMemoryPriorityEXT(VkDevice device, VkDeviceMemory memory, float priority) {
    if (!p_vkSetDeviceMemoryPriorityEXT) p_vkSetDeviceMemoryPriorityEXT = (PFN_vkSetDeviceMemoryPriorityEXT)ps5_vk_proc("vkSetDeviceMemoryPriorityEXT");
    p_vkSetDeviceMemoryPriorityEXT(device, memory, priority);
}
static PFN_vkCmdSetDispatchParametersARM p_vkCmdSetDispatchParametersARM;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDispatchParametersARM(VkCommandBuffer commandBuffer, const VkDispatchParametersARM* pDispatchParameters) {
    if (!p_vkCmdSetDispatchParametersARM) p_vkCmdSetDispatchParametersARM = (PFN_vkCmdSetDispatchParametersARM)ps5_vk_proc("vkCmdSetDispatchParametersARM");
    p_vkCmdSetDispatchParametersARM(commandBuffer, pDispatchParameters);
}
static PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE p_vkGetDescriptorSetLayoutHostMappingInfoVALVE;
VKAPI_ATTR void VKAPI_CALL vkGetDescriptorSetLayoutHostMappingInfoVALVE(VkDevice device, const VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping) {
    if (!p_vkGetDescriptorSetLayoutHostMappingInfoVALVE) p_vkGetDescriptorSetLayoutHostMappingInfoVALVE = (PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE)ps5_vk_proc("vkGetDescriptorSetLayoutHostMappingInfoVALVE");
    p_vkGetDescriptorSetLayoutHostMappingInfoVALVE(device, pBindingReference, pHostMapping);
}
static PFN_vkGetDescriptorSetHostMappingVALVE p_vkGetDescriptorSetHostMappingVALVE;
VKAPI_ATTR void VKAPI_CALL vkGetDescriptorSetHostMappingVALVE(VkDevice device, VkDescriptorSet descriptorSet, void** ppData) {
    if (!p_vkGetDescriptorSetHostMappingVALVE) p_vkGetDescriptorSetHostMappingVALVE = (PFN_vkGetDescriptorSetHostMappingVALVE)ps5_vk_proc("vkGetDescriptorSetHostMappingVALVE");
    p_vkGetDescriptorSetHostMappingVALVE(device, descriptorSet, ppData);
}
static PFN_vkCmdCopyMemoryIndirectNV p_vkCmdCopyMemoryIndirectNV;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyMemoryIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride) {
    if (!p_vkCmdCopyMemoryIndirectNV) p_vkCmdCopyMemoryIndirectNV = (PFN_vkCmdCopyMemoryIndirectNV)ps5_vk_proc("vkCmdCopyMemoryIndirectNV");
    p_vkCmdCopyMemoryIndirectNV(commandBuffer, copyBufferAddress, copyCount, stride);
}
static PFN_vkCmdCopyMemoryToImageIndirectNV p_vkCmdCopyMemoryToImageIndirectNV;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyMemoryToImageIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride, VkImage dstImage, VkImageLayout dstImageLayout, const VkImageSubresourceLayers* pImageSubresources) {
    if (!p_vkCmdCopyMemoryToImageIndirectNV) p_vkCmdCopyMemoryToImageIndirectNV = (PFN_vkCmdCopyMemoryToImageIndirectNV)ps5_vk_proc("vkCmdCopyMemoryToImageIndirectNV");
    p_vkCmdCopyMemoryToImageIndirectNV(commandBuffer, copyBufferAddress, copyCount, stride, dstImage, dstImageLayout, pImageSubresources);
}
static PFN_vkCmdDecompressMemoryNV p_vkCmdDecompressMemoryNV;
VKAPI_ATTR void VKAPI_CALL vkCmdDecompressMemoryNV(VkCommandBuffer commandBuffer, uint32_t decompressRegionCount, const VkDecompressMemoryRegionNV* pDecompressMemoryRegions) {
    if (!p_vkCmdDecompressMemoryNV) p_vkCmdDecompressMemoryNV = (PFN_vkCmdDecompressMemoryNV)ps5_vk_proc("vkCmdDecompressMemoryNV");
    p_vkCmdDecompressMemoryNV(commandBuffer, decompressRegionCount, pDecompressMemoryRegions);
}
static PFN_vkCmdDecompressMemoryIndirectCountNV p_vkCmdDecompressMemoryIndirectCountNV;
VKAPI_ATTR void VKAPI_CALL vkCmdDecompressMemoryIndirectCountNV(VkCommandBuffer commandBuffer, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t stride) {
    if (!p_vkCmdDecompressMemoryIndirectCountNV) p_vkCmdDecompressMemoryIndirectCountNV = (PFN_vkCmdDecompressMemoryIndirectCountNV)ps5_vk_proc("vkCmdDecompressMemoryIndirectCountNV");
    p_vkCmdDecompressMemoryIndirectCountNV(commandBuffer, indirectCommandsAddress, indirectCommandsCountAddress, stride);
}
static PFN_vkGetPipelineIndirectMemoryRequirementsNV p_vkGetPipelineIndirectMemoryRequirementsNV;
VKAPI_ATTR void VKAPI_CALL vkGetPipelineIndirectMemoryRequirementsNV(VkDevice device, const VkComputePipelineCreateInfo* pCreateInfo, VkMemoryRequirements2* pMemoryRequirements) {
    if (!p_vkGetPipelineIndirectMemoryRequirementsNV) p_vkGetPipelineIndirectMemoryRequirementsNV = (PFN_vkGetPipelineIndirectMemoryRequirementsNV)ps5_vk_proc("vkGetPipelineIndirectMemoryRequirementsNV");
    p_vkGetPipelineIndirectMemoryRequirementsNV(device, pCreateInfo, pMemoryRequirements);
}
static PFN_vkCmdUpdatePipelineIndirectBufferNV p_vkCmdUpdatePipelineIndirectBufferNV;
VKAPI_ATTR void VKAPI_CALL vkCmdUpdatePipelineIndirectBufferNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) {
    if (!p_vkCmdUpdatePipelineIndirectBufferNV) p_vkCmdUpdatePipelineIndirectBufferNV = (PFN_vkCmdUpdatePipelineIndirectBufferNV)ps5_vk_proc("vkCmdUpdatePipelineIndirectBufferNV");
    p_vkCmdUpdatePipelineIndirectBufferNV(commandBuffer, pipelineBindPoint, pipeline);
}
static PFN_vkGetPipelineIndirectDeviceAddressNV p_vkGetPipelineIndirectDeviceAddressNV;
VKAPI_ATTR VkDeviceAddress VKAPI_CALL vkGetPipelineIndirectDeviceAddressNV(VkDevice device, const VkPipelineIndirectDeviceAddressInfoNV* pInfo) {
    if (!p_vkGetPipelineIndirectDeviceAddressNV) p_vkGetPipelineIndirectDeviceAddressNV = (PFN_vkGetPipelineIndirectDeviceAddressNV)ps5_vk_proc("vkGetPipelineIndirectDeviceAddressNV");
    return p_vkGetPipelineIndirectDeviceAddressNV(device, pInfo);
}
static PFN_vkCmdSetDepthClampEnableEXT p_vkCmdSetDepthClampEnableEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthClampEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClampEnable) {
    if (!p_vkCmdSetDepthClampEnableEXT) p_vkCmdSetDepthClampEnableEXT = (PFN_vkCmdSetDepthClampEnableEXT)ps5_vk_proc("vkCmdSetDepthClampEnableEXT");
    p_vkCmdSetDepthClampEnableEXT(commandBuffer, depthClampEnable);
}
static PFN_vkCmdSetPolygonModeEXT p_vkCmdSetPolygonModeEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetPolygonModeEXT(VkCommandBuffer commandBuffer, VkPolygonMode polygonMode) {
    if (!p_vkCmdSetPolygonModeEXT) p_vkCmdSetPolygonModeEXT = (PFN_vkCmdSetPolygonModeEXT)ps5_vk_proc("vkCmdSetPolygonModeEXT");
    p_vkCmdSetPolygonModeEXT(commandBuffer, polygonMode);
}
static PFN_vkCmdSetRasterizationSamplesEXT p_vkCmdSetRasterizationSamplesEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetRasterizationSamplesEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits rasterizationSamples) {
    if (!p_vkCmdSetRasterizationSamplesEXT) p_vkCmdSetRasterizationSamplesEXT = (PFN_vkCmdSetRasterizationSamplesEXT)ps5_vk_proc("vkCmdSetRasterizationSamplesEXT");
    p_vkCmdSetRasterizationSamplesEXT(commandBuffer, rasterizationSamples);
}
static PFN_vkCmdSetSampleMaskEXT p_vkCmdSetSampleMaskEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetSampleMaskEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits samples, const VkSampleMask* pSampleMask) {
    if (!p_vkCmdSetSampleMaskEXT) p_vkCmdSetSampleMaskEXT = (PFN_vkCmdSetSampleMaskEXT)ps5_vk_proc("vkCmdSetSampleMaskEXT");
    p_vkCmdSetSampleMaskEXT(commandBuffer, samples, pSampleMask);
}
static PFN_vkCmdSetAlphaToCoverageEnableEXT p_vkCmdSetAlphaToCoverageEnableEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetAlphaToCoverageEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable) {
    if (!p_vkCmdSetAlphaToCoverageEnableEXT) p_vkCmdSetAlphaToCoverageEnableEXT = (PFN_vkCmdSetAlphaToCoverageEnableEXT)ps5_vk_proc("vkCmdSetAlphaToCoverageEnableEXT");
    p_vkCmdSetAlphaToCoverageEnableEXT(commandBuffer, alphaToCoverageEnable);
}
static PFN_vkCmdSetAlphaToOneEnableEXT p_vkCmdSetAlphaToOneEnableEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetAlphaToOneEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable) {
    if (!p_vkCmdSetAlphaToOneEnableEXT) p_vkCmdSetAlphaToOneEnableEXT = (PFN_vkCmdSetAlphaToOneEnableEXT)ps5_vk_proc("vkCmdSetAlphaToOneEnableEXT");
    p_vkCmdSetAlphaToOneEnableEXT(commandBuffer, alphaToOneEnable);
}
static PFN_vkCmdSetLogicOpEnableEXT p_vkCmdSetLogicOpEnableEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetLogicOpEnableEXT(VkCommandBuffer commandBuffer, VkBool32 logicOpEnable) {
    if (!p_vkCmdSetLogicOpEnableEXT) p_vkCmdSetLogicOpEnableEXT = (PFN_vkCmdSetLogicOpEnableEXT)ps5_vk_proc("vkCmdSetLogicOpEnableEXT");
    p_vkCmdSetLogicOpEnableEXT(commandBuffer, logicOpEnable);
}
static PFN_vkCmdSetColorBlendEnableEXT p_vkCmdSetColorBlendEnableEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetColorBlendEnableEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkBool32* pColorBlendEnables) {
    if (!p_vkCmdSetColorBlendEnableEXT) p_vkCmdSetColorBlendEnableEXT = (PFN_vkCmdSetColorBlendEnableEXT)ps5_vk_proc("vkCmdSetColorBlendEnableEXT");
    p_vkCmdSetColorBlendEnableEXT(commandBuffer, firstAttachment, attachmentCount, pColorBlendEnables);
}
static PFN_vkCmdSetColorBlendEquationEXT p_vkCmdSetColorBlendEquationEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetColorBlendEquationEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendEquationEXT* pColorBlendEquations) {
    if (!p_vkCmdSetColorBlendEquationEXT) p_vkCmdSetColorBlendEquationEXT = (PFN_vkCmdSetColorBlendEquationEXT)ps5_vk_proc("vkCmdSetColorBlendEquationEXT");
    p_vkCmdSetColorBlendEquationEXT(commandBuffer, firstAttachment, attachmentCount, pColorBlendEquations);
}
static PFN_vkCmdSetColorWriteMaskEXT p_vkCmdSetColorWriteMaskEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetColorWriteMaskEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorComponentFlags* pColorWriteMasks) {
    if (!p_vkCmdSetColorWriteMaskEXT) p_vkCmdSetColorWriteMaskEXT = (PFN_vkCmdSetColorWriteMaskEXT)ps5_vk_proc("vkCmdSetColorWriteMaskEXT");
    p_vkCmdSetColorWriteMaskEXT(commandBuffer, firstAttachment, attachmentCount, pColorWriteMasks);
}
static PFN_vkCmdSetTessellationDomainOriginEXT p_vkCmdSetTessellationDomainOriginEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetTessellationDomainOriginEXT(VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin) {
    if (!p_vkCmdSetTessellationDomainOriginEXT) p_vkCmdSetTessellationDomainOriginEXT = (PFN_vkCmdSetTessellationDomainOriginEXT)ps5_vk_proc("vkCmdSetTessellationDomainOriginEXT");
    p_vkCmdSetTessellationDomainOriginEXT(commandBuffer, domainOrigin);
}
static PFN_vkCmdSetRasterizationStreamEXT p_vkCmdSetRasterizationStreamEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetRasterizationStreamEXT(VkCommandBuffer commandBuffer, uint32_t rasterizationStream) {
    if (!p_vkCmdSetRasterizationStreamEXT) p_vkCmdSetRasterizationStreamEXT = (PFN_vkCmdSetRasterizationStreamEXT)ps5_vk_proc("vkCmdSetRasterizationStreamEXT");
    p_vkCmdSetRasterizationStreamEXT(commandBuffer, rasterizationStream);
}
static PFN_vkCmdSetConservativeRasterizationModeEXT p_vkCmdSetConservativeRasterizationModeEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetConservativeRasterizationModeEXT(VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode) {
    if (!p_vkCmdSetConservativeRasterizationModeEXT) p_vkCmdSetConservativeRasterizationModeEXT = (PFN_vkCmdSetConservativeRasterizationModeEXT)ps5_vk_proc("vkCmdSetConservativeRasterizationModeEXT");
    p_vkCmdSetConservativeRasterizationModeEXT(commandBuffer, conservativeRasterizationMode);
}
static PFN_vkCmdSetExtraPrimitiveOverestimationSizeEXT p_vkCmdSetExtraPrimitiveOverestimationSizeEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetExtraPrimitiveOverestimationSizeEXT(VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize) {
    if (!p_vkCmdSetExtraPrimitiveOverestimationSizeEXT) p_vkCmdSetExtraPrimitiveOverestimationSizeEXT = (PFN_vkCmdSetExtraPrimitiveOverestimationSizeEXT)ps5_vk_proc("vkCmdSetExtraPrimitiveOverestimationSizeEXT");
    p_vkCmdSetExtraPrimitiveOverestimationSizeEXT(commandBuffer, extraPrimitiveOverestimationSize);
}
static PFN_vkCmdSetDepthClipEnableEXT p_vkCmdSetDepthClipEnableEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthClipEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClipEnable) {
    if (!p_vkCmdSetDepthClipEnableEXT) p_vkCmdSetDepthClipEnableEXT = (PFN_vkCmdSetDepthClipEnableEXT)ps5_vk_proc("vkCmdSetDepthClipEnableEXT");
    p_vkCmdSetDepthClipEnableEXT(commandBuffer, depthClipEnable);
}
static PFN_vkCmdSetSampleLocationsEnableEXT p_vkCmdSetSampleLocationsEnableEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetSampleLocationsEnableEXT(VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable) {
    if (!p_vkCmdSetSampleLocationsEnableEXT) p_vkCmdSetSampleLocationsEnableEXT = (PFN_vkCmdSetSampleLocationsEnableEXT)ps5_vk_proc("vkCmdSetSampleLocationsEnableEXT");
    p_vkCmdSetSampleLocationsEnableEXT(commandBuffer, sampleLocationsEnable);
}
static PFN_vkCmdSetColorBlendAdvancedEXT p_vkCmdSetColorBlendAdvancedEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetColorBlendAdvancedEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendAdvancedEXT* pColorBlendAdvanced) {
    if (!p_vkCmdSetColorBlendAdvancedEXT) p_vkCmdSetColorBlendAdvancedEXT = (PFN_vkCmdSetColorBlendAdvancedEXT)ps5_vk_proc("vkCmdSetColorBlendAdvancedEXT");
    p_vkCmdSetColorBlendAdvancedEXT(commandBuffer, firstAttachment, attachmentCount, pColorBlendAdvanced);
}
static PFN_vkCmdSetProvokingVertexModeEXT p_vkCmdSetProvokingVertexModeEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetProvokingVertexModeEXT(VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode) {
    if (!p_vkCmdSetProvokingVertexModeEXT) p_vkCmdSetProvokingVertexModeEXT = (PFN_vkCmdSetProvokingVertexModeEXT)ps5_vk_proc("vkCmdSetProvokingVertexModeEXT");
    p_vkCmdSetProvokingVertexModeEXT(commandBuffer, provokingVertexMode);
}
static PFN_vkCmdSetLineRasterizationModeEXT p_vkCmdSetLineRasterizationModeEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetLineRasterizationModeEXT(VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode) {
    if (!p_vkCmdSetLineRasterizationModeEXT) p_vkCmdSetLineRasterizationModeEXT = (PFN_vkCmdSetLineRasterizationModeEXT)ps5_vk_proc("vkCmdSetLineRasterizationModeEXT");
    p_vkCmdSetLineRasterizationModeEXT(commandBuffer, lineRasterizationMode);
}
static PFN_vkCmdSetLineStippleEnableEXT p_vkCmdSetLineStippleEnableEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetLineStippleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable) {
    if (!p_vkCmdSetLineStippleEnableEXT) p_vkCmdSetLineStippleEnableEXT = (PFN_vkCmdSetLineStippleEnableEXT)ps5_vk_proc("vkCmdSetLineStippleEnableEXT");
    p_vkCmdSetLineStippleEnableEXT(commandBuffer, stippledLineEnable);
}
static PFN_vkCmdSetDepthClipNegativeOneToOneEXT p_vkCmdSetDepthClipNegativeOneToOneEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthClipNegativeOneToOneEXT(VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne) {
    if (!p_vkCmdSetDepthClipNegativeOneToOneEXT) p_vkCmdSetDepthClipNegativeOneToOneEXT = (PFN_vkCmdSetDepthClipNegativeOneToOneEXT)ps5_vk_proc("vkCmdSetDepthClipNegativeOneToOneEXT");
    p_vkCmdSetDepthClipNegativeOneToOneEXT(commandBuffer, negativeOneToOne);
}
static PFN_vkCmdSetViewportWScalingEnableNV p_vkCmdSetViewportWScalingEnableNV;
VKAPI_ATTR void VKAPI_CALL vkCmdSetViewportWScalingEnableNV(VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable) {
    if (!p_vkCmdSetViewportWScalingEnableNV) p_vkCmdSetViewportWScalingEnableNV = (PFN_vkCmdSetViewportWScalingEnableNV)ps5_vk_proc("vkCmdSetViewportWScalingEnableNV");
    p_vkCmdSetViewportWScalingEnableNV(commandBuffer, viewportWScalingEnable);
}
static PFN_vkCmdSetViewportSwizzleNV p_vkCmdSetViewportSwizzleNV;
VKAPI_ATTR void VKAPI_CALL vkCmdSetViewportSwizzleNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportSwizzleNV* pViewportSwizzles) {
    if (!p_vkCmdSetViewportSwizzleNV) p_vkCmdSetViewportSwizzleNV = (PFN_vkCmdSetViewportSwizzleNV)ps5_vk_proc("vkCmdSetViewportSwizzleNV");
    p_vkCmdSetViewportSwizzleNV(commandBuffer, firstViewport, viewportCount, pViewportSwizzles);
}
static PFN_vkCmdSetCoverageToColorEnableNV p_vkCmdSetCoverageToColorEnableNV;
VKAPI_ATTR void VKAPI_CALL vkCmdSetCoverageToColorEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable) {
    if (!p_vkCmdSetCoverageToColorEnableNV) p_vkCmdSetCoverageToColorEnableNV = (PFN_vkCmdSetCoverageToColorEnableNV)ps5_vk_proc("vkCmdSetCoverageToColorEnableNV");
    p_vkCmdSetCoverageToColorEnableNV(commandBuffer, coverageToColorEnable);
}
static PFN_vkCmdSetCoverageToColorLocationNV p_vkCmdSetCoverageToColorLocationNV;
VKAPI_ATTR void VKAPI_CALL vkCmdSetCoverageToColorLocationNV(VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation) {
    if (!p_vkCmdSetCoverageToColorLocationNV) p_vkCmdSetCoverageToColorLocationNV = (PFN_vkCmdSetCoverageToColorLocationNV)ps5_vk_proc("vkCmdSetCoverageToColorLocationNV");
    p_vkCmdSetCoverageToColorLocationNV(commandBuffer, coverageToColorLocation);
}
static PFN_vkCmdSetCoverageModulationModeNV p_vkCmdSetCoverageModulationModeNV;
VKAPI_ATTR void VKAPI_CALL vkCmdSetCoverageModulationModeNV(VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode) {
    if (!p_vkCmdSetCoverageModulationModeNV) p_vkCmdSetCoverageModulationModeNV = (PFN_vkCmdSetCoverageModulationModeNV)ps5_vk_proc("vkCmdSetCoverageModulationModeNV");
    p_vkCmdSetCoverageModulationModeNV(commandBuffer, coverageModulationMode);
}
static PFN_vkCmdSetCoverageModulationTableEnableNV p_vkCmdSetCoverageModulationTableEnableNV;
VKAPI_ATTR void VKAPI_CALL vkCmdSetCoverageModulationTableEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable) {
    if (!p_vkCmdSetCoverageModulationTableEnableNV) p_vkCmdSetCoverageModulationTableEnableNV = (PFN_vkCmdSetCoverageModulationTableEnableNV)ps5_vk_proc("vkCmdSetCoverageModulationTableEnableNV");
    p_vkCmdSetCoverageModulationTableEnableNV(commandBuffer, coverageModulationTableEnable);
}
static PFN_vkCmdSetCoverageModulationTableNV p_vkCmdSetCoverageModulationTableNV;
VKAPI_ATTR void VKAPI_CALL vkCmdSetCoverageModulationTableNV(VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, const float* pCoverageModulationTable) {
    if (!p_vkCmdSetCoverageModulationTableNV) p_vkCmdSetCoverageModulationTableNV = (PFN_vkCmdSetCoverageModulationTableNV)ps5_vk_proc("vkCmdSetCoverageModulationTableNV");
    p_vkCmdSetCoverageModulationTableNV(commandBuffer, coverageModulationTableCount, pCoverageModulationTable);
}
static PFN_vkCmdSetShadingRateImageEnableNV p_vkCmdSetShadingRateImageEnableNV;
VKAPI_ATTR void VKAPI_CALL vkCmdSetShadingRateImageEnableNV(VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable) {
    if (!p_vkCmdSetShadingRateImageEnableNV) p_vkCmdSetShadingRateImageEnableNV = (PFN_vkCmdSetShadingRateImageEnableNV)ps5_vk_proc("vkCmdSetShadingRateImageEnableNV");
    p_vkCmdSetShadingRateImageEnableNV(commandBuffer, shadingRateImageEnable);
}
static PFN_vkCmdSetRepresentativeFragmentTestEnableNV p_vkCmdSetRepresentativeFragmentTestEnableNV;
VKAPI_ATTR void VKAPI_CALL vkCmdSetRepresentativeFragmentTestEnableNV(VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable) {
    if (!p_vkCmdSetRepresentativeFragmentTestEnableNV) p_vkCmdSetRepresentativeFragmentTestEnableNV = (PFN_vkCmdSetRepresentativeFragmentTestEnableNV)ps5_vk_proc("vkCmdSetRepresentativeFragmentTestEnableNV");
    p_vkCmdSetRepresentativeFragmentTestEnableNV(commandBuffer, representativeFragmentTestEnable);
}
static PFN_vkCmdSetCoverageReductionModeNV p_vkCmdSetCoverageReductionModeNV;
VKAPI_ATTR void VKAPI_CALL vkCmdSetCoverageReductionModeNV(VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode) {
    if (!p_vkCmdSetCoverageReductionModeNV) p_vkCmdSetCoverageReductionModeNV = (PFN_vkCmdSetCoverageReductionModeNV)ps5_vk_proc("vkCmdSetCoverageReductionModeNV");
    p_vkCmdSetCoverageReductionModeNV(commandBuffer, coverageReductionMode);
}
static PFN_vkCreateTensorARM p_vkCreateTensorARM;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateTensorARM(VkDevice device, const VkTensorCreateInfoARM* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkTensorARM* pTensor) {
    if (!p_vkCreateTensorARM) p_vkCreateTensorARM = (PFN_vkCreateTensorARM)ps5_vk_proc("vkCreateTensorARM");
    return p_vkCreateTensorARM(device, pCreateInfo, pAllocator, pTensor);
}
static PFN_vkDestroyTensorARM p_vkDestroyTensorARM;
VKAPI_ATTR void VKAPI_CALL vkDestroyTensorARM(VkDevice device, VkTensorARM tensor, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyTensorARM) p_vkDestroyTensorARM = (PFN_vkDestroyTensorARM)ps5_vk_proc("vkDestroyTensorARM");
    p_vkDestroyTensorARM(device, tensor, pAllocator);
}
static PFN_vkCreateTensorViewARM p_vkCreateTensorViewARM;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateTensorViewARM(VkDevice device, const VkTensorViewCreateInfoARM* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkTensorViewARM* pView) {
    if (!p_vkCreateTensorViewARM) p_vkCreateTensorViewARM = (PFN_vkCreateTensorViewARM)ps5_vk_proc("vkCreateTensorViewARM");
    return p_vkCreateTensorViewARM(device, pCreateInfo, pAllocator, pView);
}
static PFN_vkDestroyTensorViewARM p_vkDestroyTensorViewARM;
VKAPI_ATTR void VKAPI_CALL vkDestroyTensorViewARM(VkDevice device, VkTensorViewARM tensorView, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyTensorViewARM) p_vkDestroyTensorViewARM = (PFN_vkDestroyTensorViewARM)ps5_vk_proc("vkDestroyTensorViewARM");
    p_vkDestroyTensorViewARM(device, tensorView, pAllocator);
}
static PFN_vkGetTensorMemoryRequirementsARM p_vkGetTensorMemoryRequirementsARM;
VKAPI_ATTR void VKAPI_CALL vkGetTensorMemoryRequirementsARM(VkDevice device, const VkTensorMemoryRequirementsInfoARM* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
    if (!p_vkGetTensorMemoryRequirementsARM) p_vkGetTensorMemoryRequirementsARM = (PFN_vkGetTensorMemoryRequirementsARM)ps5_vk_proc("vkGetTensorMemoryRequirementsARM");
    p_vkGetTensorMemoryRequirementsARM(device, pInfo, pMemoryRequirements);
}
static PFN_vkBindTensorMemoryARM p_vkBindTensorMemoryARM;
VKAPI_ATTR VkResult VKAPI_CALL vkBindTensorMemoryARM(VkDevice device, uint32_t bindInfoCount, const VkBindTensorMemoryInfoARM* pBindInfos) {
    if (!p_vkBindTensorMemoryARM) p_vkBindTensorMemoryARM = (PFN_vkBindTensorMemoryARM)ps5_vk_proc("vkBindTensorMemoryARM");
    return p_vkBindTensorMemoryARM(device, bindInfoCount, pBindInfos);
}
static PFN_vkGetDeviceTensorMemoryRequirementsARM p_vkGetDeviceTensorMemoryRequirementsARM;
VKAPI_ATTR void VKAPI_CALL vkGetDeviceTensorMemoryRequirementsARM(VkDevice device, const VkDeviceTensorMemoryRequirementsARM* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
    if (!p_vkGetDeviceTensorMemoryRequirementsARM) p_vkGetDeviceTensorMemoryRequirementsARM = (PFN_vkGetDeviceTensorMemoryRequirementsARM)ps5_vk_proc("vkGetDeviceTensorMemoryRequirementsARM");
    p_vkGetDeviceTensorMemoryRequirementsARM(device, pInfo, pMemoryRequirements);
}
static PFN_vkCmdCopyTensorARM p_vkCmdCopyTensorARM;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyTensorARM(VkCommandBuffer commandBuffer, const VkCopyTensorInfoARM* pCopyTensorInfo) {
    if (!p_vkCmdCopyTensorARM) p_vkCmdCopyTensorARM = (PFN_vkCmdCopyTensorARM)ps5_vk_proc("vkCmdCopyTensorARM");
    p_vkCmdCopyTensorARM(commandBuffer, pCopyTensorInfo);
}
static PFN_vkGetPhysicalDeviceExternalTensorPropertiesARM p_vkGetPhysicalDeviceExternalTensorPropertiesARM;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceExternalTensorPropertiesARM(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalTensorInfoARM* pExternalTensorInfo, VkExternalTensorPropertiesARM* pExternalTensorProperties) {
    if (!p_vkGetPhysicalDeviceExternalTensorPropertiesARM) p_vkGetPhysicalDeviceExternalTensorPropertiesARM = (PFN_vkGetPhysicalDeviceExternalTensorPropertiesARM)ps5_vk_proc("vkGetPhysicalDeviceExternalTensorPropertiesARM");
    p_vkGetPhysicalDeviceExternalTensorPropertiesARM(physicalDevice, pExternalTensorInfo, pExternalTensorProperties);
}
static PFN_vkGetTensorOpaqueCaptureDescriptorDataARM p_vkGetTensorOpaqueCaptureDescriptorDataARM;
VKAPI_ATTR VkResult VKAPI_CALL vkGetTensorOpaqueCaptureDescriptorDataARM(VkDevice device, const VkTensorCaptureDescriptorDataInfoARM* pInfo, void* pData) {
    if (!p_vkGetTensorOpaqueCaptureDescriptorDataARM) p_vkGetTensorOpaqueCaptureDescriptorDataARM = (PFN_vkGetTensorOpaqueCaptureDescriptorDataARM)ps5_vk_proc("vkGetTensorOpaqueCaptureDescriptorDataARM");
    return p_vkGetTensorOpaqueCaptureDescriptorDataARM(device, pInfo, pData);
}
static PFN_vkGetTensorViewOpaqueCaptureDescriptorDataARM p_vkGetTensorViewOpaqueCaptureDescriptorDataARM;
VKAPI_ATTR VkResult VKAPI_CALL vkGetTensorViewOpaqueCaptureDescriptorDataARM(VkDevice device, const VkTensorViewCaptureDescriptorDataInfoARM* pInfo, void* pData) {
    if (!p_vkGetTensorViewOpaqueCaptureDescriptorDataARM) p_vkGetTensorViewOpaqueCaptureDescriptorDataARM = (PFN_vkGetTensorViewOpaqueCaptureDescriptorDataARM)ps5_vk_proc("vkGetTensorViewOpaqueCaptureDescriptorDataARM");
    return p_vkGetTensorViewOpaqueCaptureDescriptorDataARM(device, pInfo, pData);
}
static PFN_vkGetShaderModuleIdentifierEXT p_vkGetShaderModuleIdentifierEXT;
VKAPI_ATTR void VKAPI_CALL vkGetShaderModuleIdentifierEXT(VkDevice device, VkShaderModule shaderModule, VkShaderModuleIdentifierEXT* pIdentifier) {
    if (!p_vkGetShaderModuleIdentifierEXT) p_vkGetShaderModuleIdentifierEXT = (PFN_vkGetShaderModuleIdentifierEXT)ps5_vk_proc("vkGetShaderModuleIdentifierEXT");
    p_vkGetShaderModuleIdentifierEXT(device, shaderModule, pIdentifier);
}
static PFN_vkGetShaderModuleCreateInfoIdentifierEXT p_vkGetShaderModuleCreateInfoIdentifierEXT;
VKAPI_ATTR void VKAPI_CALL vkGetShaderModuleCreateInfoIdentifierEXT(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, VkShaderModuleIdentifierEXT* pIdentifier) {
    if (!p_vkGetShaderModuleCreateInfoIdentifierEXT) p_vkGetShaderModuleCreateInfoIdentifierEXT = (PFN_vkGetShaderModuleCreateInfoIdentifierEXT)ps5_vk_proc("vkGetShaderModuleCreateInfoIdentifierEXT");
    p_vkGetShaderModuleCreateInfoIdentifierEXT(device, pCreateInfo, pIdentifier);
}
static PFN_vkGetPhysicalDeviceOpticalFlowImageFormatsNV p_vkGetPhysicalDeviceOpticalFlowImageFormatsNV;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceOpticalFlowImageFormatsNV(VkPhysicalDevice physicalDevice, const VkOpticalFlowImageFormatInfoNV* pOpticalFlowImageFormatInfo, uint32_t* pFormatCount, VkOpticalFlowImageFormatPropertiesNV* pImageFormatProperties) {
    if (!p_vkGetPhysicalDeviceOpticalFlowImageFormatsNV) p_vkGetPhysicalDeviceOpticalFlowImageFormatsNV = (PFN_vkGetPhysicalDeviceOpticalFlowImageFormatsNV)ps5_vk_proc("vkGetPhysicalDeviceOpticalFlowImageFormatsNV");
    return p_vkGetPhysicalDeviceOpticalFlowImageFormatsNV(physicalDevice, pOpticalFlowImageFormatInfo, pFormatCount, pImageFormatProperties);
}
static PFN_vkCreateOpticalFlowSessionNV p_vkCreateOpticalFlowSessionNV;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateOpticalFlowSessionNV(VkDevice device, const VkOpticalFlowSessionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkOpticalFlowSessionNV* pSession) {
    if (!p_vkCreateOpticalFlowSessionNV) p_vkCreateOpticalFlowSessionNV = (PFN_vkCreateOpticalFlowSessionNV)ps5_vk_proc("vkCreateOpticalFlowSessionNV");
    return p_vkCreateOpticalFlowSessionNV(device, pCreateInfo, pAllocator, pSession);
}
static PFN_vkDestroyOpticalFlowSessionNV p_vkDestroyOpticalFlowSessionNV;
VKAPI_ATTR void VKAPI_CALL vkDestroyOpticalFlowSessionNV(VkDevice device, VkOpticalFlowSessionNV session, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyOpticalFlowSessionNV) p_vkDestroyOpticalFlowSessionNV = (PFN_vkDestroyOpticalFlowSessionNV)ps5_vk_proc("vkDestroyOpticalFlowSessionNV");
    p_vkDestroyOpticalFlowSessionNV(device, session, pAllocator);
}
static PFN_vkBindOpticalFlowSessionImageNV p_vkBindOpticalFlowSessionImageNV;
VKAPI_ATTR VkResult VKAPI_CALL vkBindOpticalFlowSessionImageNV(VkDevice device, VkOpticalFlowSessionNV session, VkOpticalFlowSessionBindingPointNV bindingPoint, VkImageView view, VkImageLayout layout) {
    if (!p_vkBindOpticalFlowSessionImageNV) p_vkBindOpticalFlowSessionImageNV = (PFN_vkBindOpticalFlowSessionImageNV)ps5_vk_proc("vkBindOpticalFlowSessionImageNV");
    return p_vkBindOpticalFlowSessionImageNV(device, session, bindingPoint, view, layout);
}
static PFN_vkCmdOpticalFlowExecuteNV p_vkCmdOpticalFlowExecuteNV;
VKAPI_ATTR void VKAPI_CALL vkCmdOpticalFlowExecuteNV(VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, const VkOpticalFlowExecuteInfoNV* pExecuteInfo) {
    if (!p_vkCmdOpticalFlowExecuteNV) p_vkCmdOpticalFlowExecuteNV = (PFN_vkCmdOpticalFlowExecuteNV)ps5_vk_proc("vkCmdOpticalFlowExecuteNV");
    p_vkCmdOpticalFlowExecuteNV(commandBuffer, session, pExecuteInfo);
}
static PFN_vkAntiLagUpdateAMD p_vkAntiLagUpdateAMD;
VKAPI_ATTR void VKAPI_CALL vkAntiLagUpdateAMD(VkDevice device, const VkAntiLagDataAMD* pData) {
    if (!p_vkAntiLagUpdateAMD) p_vkAntiLagUpdateAMD = (PFN_vkAntiLagUpdateAMD)ps5_vk_proc("vkAntiLagUpdateAMD");
    p_vkAntiLagUpdateAMD(device, pData);
}
static PFN_vkCreateShadersEXT p_vkCreateShadersEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateShadersEXT(VkDevice device, uint32_t createInfoCount, const VkShaderCreateInfoEXT* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkShaderEXT* pShaders) {
    if (!p_vkCreateShadersEXT) p_vkCreateShadersEXT = (PFN_vkCreateShadersEXT)ps5_vk_proc("vkCreateShadersEXT");
    return p_vkCreateShadersEXT(device, createInfoCount, pCreateInfos, pAllocator, pShaders);
}
static PFN_vkDestroyShaderEXT p_vkDestroyShaderEXT;
VKAPI_ATTR void VKAPI_CALL vkDestroyShaderEXT(VkDevice device, VkShaderEXT shader, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyShaderEXT) p_vkDestroyShaderEXT = (PFN_vkDestroyShaderEXT)ps5_vk_proc("vkDestroyShaderEXT");
    p_vkDestroyShaderEXT(device, shader, pAllocator);
}
static PFN_vkGetShaderBinaryDataEXT p_vkGetShaderBinaryDataEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkGetShaderBinaryDataEXT(VkDevice device, VkShaderEXT shader, size_t* pDataSize, void* pData) {
    if (!p_vkGetShaderBinaryDataEXT) p_vkGetShaderBinaryDataEXT = (PFN_vkGetShaderBinaryDataEXT)ps5_vk_proc("vkGetShaderBinaryDataEXT");
    return p_vkGetShaderBinaryDataEXT(device, shader, pDataSize, pData);
}
static PFN_vkCmdBindShadersEXT p_vkCmdBindShadersEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdBindShadersEXT(VkCommandBuffer commandBuffer, uint32_t stageCount, const VkShaderStageFlagBits* pStages, const VkShaderEXT* pShaders) {
    if (!p_vkCmdBindShadersEXT) p_vkCmdBindShadersEXT = (PFN_vkCmdBindShadersEXT)ps5_vk_proc("vkCmdBindShadersEXT");
    p_vkCmdBindShadersEXT(commandBuffer, stageCount, pStages, pShaders);
}
static PFN_vkCmdSetDepthClampRangeEXT p_vkCmdSetDepthClampRangeEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthClampRangeEXT(VkCommandBuffer commandBuffer, VkDepthClampModeEXT depthClampMode, const VkDepthClampRangeEXT* pDepthClampRange) {
    if (!p_vkCmdSetDepthClampRangeEXT) p_vkCmdSetDepthClampRangeEXT = (PFN_vkCmdSetDepthClampRangeEXT)ps5_vk_proc("vkCmdSetDepthClampRangeEXT");
    p_vkCmdSetDepthClampRangeEXT(commandBuffer, depthClampMode, pDepthClampRange);
}
static PFN_vkGetFramebufferTilePropertiesQCOM p_vkGetFramebufferTilePropertiesQCOM;
VKAPI_ATTR VkResult VKAPI_CALL vkGetFramebufferTilePropertiesQCOM(VkDevice device, VkFramebuffer framebuffer, uint32_t* pPropertiesCount, VkTilePropertiesQCOM* pProperties) {
    if (!p_vkGetFramebufferTilePropertiesQCOM) p_vkGetFramebufferTilePropertiesQCOM = (PFN_vkGetFramebufferTilePropertiesQCOM)ps5_vk_proc("vkGetFramebufferTilePropertiesQCOM");
    return p_vkGetFramebufferTilePropertiesQCOM(device, framebuffer, pPropertiesCount, pProperties);
}
static PFN_vkGetDynamicRenderingTilePropertiesQCOM p_vkGetDynamicRenderingTilePropertiesQCOM;
VKAPI_ATTR VkResult VKAPI_CALL vkGetDynamicRenderingTilePropertiesQCOM(VkDevice device, const VkRenderingInfo* pRenderingInfo, VkTilePropertiesQCOM* pProperties) {
    if (!p_vkGetDynamicRenderingTilePropertiesQCOM) p_vkGetDynamicRenderingTilePropertiesQCOM = (PFN_vkGetDynamicRenderingTilePropertiesQCOM)ps5_vk_proc("vkGetDynamicRenderingTilePropertiesQCOM");
    return p_vkGetDynamicRenderingTilePropertiesQCOM(device, pRenderingInfo, pProperties);
}
static PFN_vkGetPhysicalDeviceCooperativeVectorPropertiesNV p_vkGetPhysicalDeviceCooperativeVectorPropertiesNV;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceCooperativeVectorPropertiesNV(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeVectorPropertiesNV* pProperties) {
    if (!p_vkGetPhysicalDeviceCooperativeVectorPropertiesNV) p_vkGetPhysicalDeviceCooperativeVectorPropertiesNV = (PFN_vkGetPhysicalDeviceCooperativeVectorPropertiesNV)ps5_vk_proc("vkGetPhysicalDeviceCooperativeVectorPropertiesNV");
    return p_vkGetPhysicalDeviceCooperativeVectorPropertiesNV(physicalDevice, pPropertyCount, pProperties);
}
static PFN_vkConvertCooperativeVectorMatrixNV p_vkConvertCooperativeVectorMatrixNV;
VKAPI_ATTR VkResult VKAPI_CALL vkConvertCooperativeVectorMatrixNV(VkDevice device, const VkConvertCooperativeVectorMatrixInfoNV* pInfo) {
    if (!p_vkConvertCooperativeVectorMatrixNV) p_vkConvertCooperativeVectorMatrixNV = (PFN_vkConvertCooperativeVectorMatrixNV)ps5_vk_proc("vkConvertCooperativeVectorMatrixNV");
    return p_vkConvertCooperativeVectorMatrixNV(device, pInfo);
}
static PFN_vkCmdConvertCooperativeVectorMatrixNV p_vkCmdConvertCooperativeVectorMatrixNV;
VKAPI_ATTR void VKAPI_CALL vkCmdConvertCooperativeVectorMatrixNV(VkCommandBuffer commandBuffer, uint32_t infoCount, const VkConvertCooperativeVectorMatrixInfoNV* pInfos) {
    if (!p_vkCmdConvertCooperativeVectorMatrixNV) p_vkCmdConvertCooperativeVectorMatrixNV = (PFN_vkCmdConvertCooperativeVectorMatrixNV)ps5_vk_proc("vkCmdConvertCooperativeVectorMatrixNV");
    p_vkCmdConvertCooperativeVectorMatrixNV(commandBuffer, infoCount, pInfos);
}
static PFN_vkSetLatencySleepModeNV p_vkSetLatencySleepModeNV;
VKAPI_ATTR VkResult VKAPI_CALL vkSetLatencySleepModeNV(VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepModeInfoNV* pSleepModeInfo) {
    if (!p_vkSetLatencySleepModeNV) p_vkSetLatencySleepModeNV = (PFN_vkSetLatencySleepModeNV)ps5_vk_proc("vkSetLatencySleepModeNV");
    return p_vkSetLatencySleepModeNV(device, swapchain, pSleepModeInfo);
}
static PFN_vkLatencySleepNV p_vkLatencySleepNV;
VKAPI_ATTR VkResult VKAPI_CALL vkLatencySleepNV(VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepInfoNV* pSleepInfo) {
    if (!p_vkLatencySleepNV) p_vkLatencySleepNV = (PFN_vkLatencySleepNV)ps5_vk_proc("vkLatencySleepNV");
    return p_vkLatencySleepNV(device, swapchain, pSleepInfo);
}
static PFN_vkSetLatencyMarkerNV p_vkSetLatencyMarkerNV;
VKAPI_ATTR void VKAPI_CALL vkSetLatencyMarkerNV(VkDevice device, VkSwapchainKHR swapchain, const VkSetLatencyMarkerInfoNV* pLatencyMarkerInfo) {
    if (!p_vkSetLatencyMarkerNV) p_vkSetLatencyMarkerNV = (PFN_vkSetLatencyMarkerNV)ps5_vk_proc("vkSetLatencyMarkerNV");
    p_vkSetLatencyMarkerNV(device, swapchain, pLatencyMarkerInfo);
}
static PFN_vkGetLatencyTimingsNV p_vkGetLatencyTimingsNV;
VKAPI_ATTR void VKAPI_CALL vkGetLatencyTimingsNV(VkDevice device, VkSwapchainKHR swapchain, VkGetLatencyMarkerInfoNV* pLatencyMarkerInfo) {
    if (!p_vkGetLatencyTimingsNV) p_vkGetLatencyTimingsNV = (PFN_vkGetLatencyTimingsNV)ps5_vk_proc("vkGetLatencyTimingsNV");
    p_vkGetLatencyTimingsNV(device, swapchain, pLatencyMarkerInfo);
}
static PFN_vkQueueNotifyOutOfBandNV p_vkQueueNotifyOutOfBandNV;
VKAPI_ATTR void VKAPI_CALL vkQueueNotifyOutOfBandNV(VkQueue queue, const VkOutOfBandQueueTypeInfoNV* pQueueTypeInfo) {
    if (!p_vkQueueNotifyOutOfBandNV) p_vkQueueNotifyOutOfBandNV = (PFN_vkQueueNotifyOutOfBandNV)ps5_vk_proc("vkQueueNotifyOutOfBandNV");
    p_vkQueueNotifyOutOfBandNV(queue, pQueueTypeInfo);
}
static PFN_vkCreateDataGraphPipelinesARM p_vkCreateDataGraphPipelinesARM;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDataGraphPipelinesARM(VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkDataGraphPipelineCreateInfoARM* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) {
    if (!p_vkCreateDataGraphPipelinesARM) p_vkCreateDataGraphPipelinesARM = (PFN_vkCreateDataGraphPipelinesARM)ps5_vk_proc("vkCreateDataGraphPipelinesARM");
    return p_vkCreateDataGraphPipelinesARM(device, deferredOperation, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}
static PFN_vkCreateDataGraphPipelineSessionARM p_vkCreateDataGraphPipelineSessionARM;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDataGraphPipelineSessionARM(VkDevice device, const VkDataGraphPipelineSessionCreateInfoARM* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDataGraphPipelineSessionARM* pSession) {
    if (!p_vkCreateDataGraphPipelineSessionARM) p_vkCreateDataGraphPipelineSessionARM = (PFN_vkCreateDataGraphPipelineSessionARM)ps5_vk_proc("vkCreateDataGraphPipelineSessionARM");
    return p_vkCreateDataGraphPipelineSessionARM(device, pCreateInfo, pAllocator, pSession);
}
static PFN_vkGetDataGraphPipelineSessionBindPointRequirementsARM p_vkGetDataGraphPipelineSessionBindPointRequirementsARM;
VKAPI_ATTR VkResult VKAPI_CALL vkGetDataGraphPipelineSessionBindPointRequirementsARM(VkDevice device, const VkDataGraphPipelineSessionBindPointRequirementsInfoARM* pInfo, uint32_t* pBindPointRequirementCount, VkDataGraphPipelineSessionBindPointRequirementARM* pBindPointRequirements) {
    if (!p_vkGetDataGraphPipelineSessionBindPointRequirementsARM) p_vkGetDataGraphPipelineSessionBindPointRequirementsARM = (PFN_vkGetDataGraphPipelineSessionBindPointRequirementsARM)ps5_vk_proc("vkGetDataGraphPipelineSessionBindPointRequirementsARM");
    return p_vkGetDataGraphPipelineSessionBindPointRequirementsARM(device, pInfo, pBindPointRequirementCount, pBindPointRequirements);
}
static PFN_vkGetDataGraphPipelineSessionMemoryRequirementsARM p_vkGetDataGraphPipelineSessionMemoryRequirementsARM;
VKAPI_ATTR void VKAPI_CALL vkGetDataGraphPipelineSessionMemoryRequirementsARM(VkDevice device, const VkDataGraphPipelineSessionMemoryRequirementsInfoARM* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
    if (!p_vkGetDataGraphPipelineSessionMemoryRequirementsARM) p_vkGetDataGraphPipelineSessionMemoryRequirementsARM = (PFN_vkGetDataGraphPipelineSessionMemoryRequirementsARM)ps5_vk_proc("vkGetDataGraphPipelineSessionMemoryRequirementsARM");
    p_vkGetDataGraphPipelineSessionMemoryRequirementsARM(device, pInfo, pMemoryRequirements);
}
static PFN_vkBindDataGraphPipelineSessionMemoryARM p_vkBindDataGraphPipelineSessionMemoryARM;
VKAPI_ATTR VkResult VKAPI_CALL vkBindDataGraphPipelineSessionMemoryARM(VkDevice device, uint32_t bindInfoCount, const VkBindDataGraphPipelineSessionMemoryInfoARM* pBindInfos) {
    if (!p_vkBindDataGraphPipelineSessionMemoryARM) p_vkBindDataGraphPipelineSessionMemoryARM = (PFN_vkBindDataGraphPipelineSessionMemoryARM)ps5_vk_proc("vkBindDataGraphPipelineSessionMemoryARM");
    return p_vkBindDataGraphPipelineSessionMemoryARM(device, bindInfoCount, pBindInfos);
}
static PFN_vkDestroyDataGraphPipelineSessionARM p_vkDestroyDataGraphPipelineSessionARM;
VKAPI_ATTR void VKAPI_CALL vkDestroyDataGraphPipelineSessionARM(VkDevice device, VkDataGraphPipelineSessionARM session, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyDataGraphPipelineSessionARM) p_vkDestroyDataGraphPipelineSessionARM = (PFN_vkDestroyDataGraphPipelineSessionARM)ps5_vk_proc("vkDestroyDataGraphPipelineSessionARM");
    p_vkDestroyDataGraphPipelineSessionARM(device, session, pAllocator);
}
static PFN_vkCmdDispatchDataGraphARM p_vkCmdDispatchDataGraphARM;
VKAPI_ATTR void VKAPI_CALL vkCmdDispatchDataGraphARM(VkCommandBuffer commandBuffer, VkDataGraphPipelineSessionARM session, const VkDataGraphPipelineDispatchInfoARM* pInfo) {
    if (!p_vkCmdDispatchDataGraphARM) p_vkCmdDispatchDataGraphARM = (PFN_vkCmdDispatchDataGraphARM)ps5_vk_proc("vkCmdDispatchDataGraphARM");
    p_vkCmdDispatchDataGraphARM(commandBuffer, session, pInfo);
}
static PFN_vkGetDataGraphPipelineAvailablePropertiesARM p_vkGetDataGraphPipelineAvailablePropertiesARM;
VKAPI_ATTR VkResult VKAPI_CALL vkGetDataGraphPipelineAvailablePropertiesARM(VkDevice device, const VkDataGraphPipelineInfoARM* pPipelineInfo, uint32_t* pPropertiesCount, VkDataGraphPipelinePropertyARM* pProperties) {
    if (!p_vkGetDataGraphPipelineAvailablePropertiesARM) p_vkGetDataGraphPipelineAvailablePropertiesARM = (PFN_vkGetDataGraphPipelineAvailablePropertiesARM)ps5_vk_proc("vkGetDataGraphPipelineAvailablePropertiesARM");
    return p_vkGetDataGraphPipelineAvailablePropertiesARM(device, pPipelineInfo, pPropertiesCount, pProperties);
}
static PFN_vkGetDataGraphPipelinePropertiesARM p_vkGetDataGraphPipelinePropertiesARM;
VKAPI_ATTR VkResult VKAPI_CALL vkGetDataGraphPipelinePropertiesARM(VkDevice device, const VkDataGraphPipelineInfoARM* pPipelineInfo, uint32_t propertiesCount, VkDataGraphPipelinePropertyQueryResultARM* pProperties) {
    if (!p_vkGetDataGraphPipelinePropertiesARM) p_vkGetDataGraphPipelinePropertiesARM = (PFN_vkGetDataGraphPipelinePropertiesARM)ps5_vk_proc("vkGetDataGraphPipelinePropertiesARM");
    return p_vkGetDataGraphPipelinePropertiesARM(device, pPipelineInfo, propertiesCount, pProperties);
}
static PFN_vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM p_vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pQueueFamilyDataGraphPropertyCount, VkQueueFamilyDataGraphPropertiesARM* pQueueFamilyDataGraphProperties) {
    if (!p_vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM) p_vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM = (PFN_vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM)ps5_vk_proc("vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM");
    return p_vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM(physicalDevice, queueFamilyIndex, pQueueFamilyDataGraphPropertyCount, pQueueFamilyDataGraphProperties);
}
static PFN_vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM p_vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM;
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceQueueFamilyDataGraphProcessingEngineInfoARM* pQueueFamilyDataGraphProcessingEngineInfo, VkQueueFamilyDataGraphProcessingEnginePropertiesARM* pQueueFamilyDataGraphProcessingEngineProperties) {
    if (!p_vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM) p_vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM = (PFN_vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM)ps5_vk_proc("vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM");
    p_vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM(physicalDevice, pQueueFamilyDataGraphProcessingEngineInfo, pQueueFamilyDataGraphProcessingEngineProperties);
}
static PFN_vkGetPhysicalDeviceQueueFamilyDataGraphEngineOperationPropertiesARM p_vkGetPhysicalDeviceQueueFamilyDataGraphEngineOperationPropertiesARM;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceQueueFamilyDataGraphEngineOperationPropertiesARM(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, const VkQueueFamilyDataGraphPropertiesARM* pQueueFamilyDataGraphProperties, VkBaseOutStructure* pProperties) {
    if (!p_vkGetPhysicalDeviceQueueFamilyDataGraphEngineOperationPropertiesARM) p_vkGetPhysicalDeviceQueueFamilyDataGraphEngineOperationPropertiesARM = (PFN_vkGetPhysicalDeviceQueueFamilyDataGraphEngineOperationPropertiesARM)ps5_vk_proc("vkGetPhysicalDeviceQueueFamilyDataGraphEngineOperationPropertiesARM");
    return p_vkGetPhysicalDeviceQueueFamilyDataGraphEngineOperationPropertiesARM(physicalDevice, queueFamilyIndex, pQueueFamilyDataGraphProperties, pProperties);
}
static PFN_vkCmdSetAttachmentFeedbackLoopEnableEXT p_vkCmdSetAttachmentFeedbackLoopEnableEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetAttachmentFeedbackLoopEnableEXT(VkCommandBuffer commandBuffer, VkImageAspectFlags aspectMask) {
    if (!p_vkCmdSetAttachmentFeedbackLoopEnableEXT) p_vkCmdSetAttachmentFeedbackLoopEnableEXT = (PFN_vkCmdSetAttachmentFeedbackLoopEnableEXT)ps5_vk_proc("vkCmdSetAttachmentFeedbackLoopEnableEXT");
    p_vkCmdSetAttachmentFeedbackLoopEnableEXT(commandBuffer, aspectMask);
}
static PFN_vkCmdBindTileMemoryQCOM p_vkCmdBindTileMemoryQCOM;
VKAPI_ATTR void VKAPI_CALL vkCmdBindTileMemoryQCOM(VkCommandBuffer commandBuffer, const VkTileMemoryBindInfoQCOM* pTileMemoryBindInfo) {
    if (!p_vkCmdBindTileMemoryQCOM) p_vkCmdBindTileMemoryQCOM = (PFN_vkCmdBindTileMemoryQCOM)ps5_vk_proc("vkCmdBindTileMemoryQCOM");
    p_vkCmdBindTileMemoryQCOM(commandBuffer, pTileMemoryBindInfo);
}
static PFN_vkCmdDecompressMemoryEXT p_vkCmdDecompressMemoryEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdDecompressMemoryEXT(VkCommandBuffer commandBuffer, const VkDecompressMemoryInfoEXT* pDecompressMemoryInfoEXT) {
    if (!p_vkCmdDecompressMemoryEXT) p_vkCmdDecompressMemoryEXT = (PFN_vkCmdDecompressMemoryEXT)ps5_vk_proc("vkCmdDecompressMemoryEXT");
    p_vkCmdDecompressMemoryEXT(commandBuffer, pDecompressMemoryInfoEXT);
}
static PFN_vkCmdDecompressMemoryIndirectCountEXT p_vkCmdDecompressMemoryIndirectCountEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdDecompressMemoryIndirectCountEXT(VkCommandBuffer commandBuffer, VkMemoryDecompressionMethodFlagsEXT decompressionMethod, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t maxDecompressionCount, uint32_t stride) {
    if (!p_vkCmdDecompressMemoryIndirectCountEXT) p_vkCmdDecompressMemoryIndirectCountEXT = (PFN_vkCmdDecompressMemoryIndirectCountEXT)ps5_vk_proc("vkCmdDecompressMemoryIndirectCountEXT");
    p_vkCmdDecompressMemoryIndirectCountEXT(commandBuffer, decompressionMethod, indirectCommandsAddress, indirectCommandsCountAddress, maxDecompressionCount, stride);
}
static PFN_vkCreateExternalComputeQueueNV p_vkCreateExternalComputeQueueNV;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateExternalComputeQueueNV(VkDevice device, const VkExternalComputeQueueCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkExternalComputeQueueNV* pExternalQueue) {
    if (!p_vkCreateExternalComputeQueueNV) p_vkCreateExternalComputeQueueNV = (PFN_vkCreateExternalComputeQueueNV)ps5_vk_proc("vkCreateExternalComputeQueueNV");
    return p_vkCreateExternalComputeQueueNV(device, pCreateInfo, pAllocator, pExternalQueue);
}
static PFN_vkDestroyExternalComputeQueueNV p_vkDestroyExternalComputeQueueNV;
VKAPI_ATTR void VKAPI_CALL vkDestroyExternalComputeQueueNV(VkDevice device, VkExternalComputeQueueNV externalQueue, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyExternalComputeQueueNV) p_vkDestroyExternalComputeQueueNV = (PFN_vkDestroyExternalComputeQueueNV)ps5_vk_proc("vkDestroyExternalComputeQueueNV");
    p_vkDestroyExternalComputeQueueNV(device, externalQueue, pAllocator);
}
static PFN_vkGetExternalComputeQueueDataNV p_vkGetExternalComputeQueueDataNV;
VKAPI_ATTR void VKAPI_CALL vkGetExternalComputeQueueDataNV(VkExternalComputeQueueNV externalQueue, VkExternalComputeQueueDataParamsNV* params, void* pData) {
    if (!p_vkGetExternalComputeQueueDataNV) p_vkGetExternalComputeQueueDataNV = (PFN_vkGetExternalComputeQueueDataNV)ps5_vk_proc("vkGetExternalComputeQueueDataNV");
    p_vkGetExternalComputeQueueDataNV(externalQueue, params, pData);
}
static PFN_vkGetClusterAccelerationStructureBuildSizesNV p_vkGetClusterAccelerationStructureBuildSizesNV;
VKAPI_ATTR void VKAPI_CALL vkGetClusterAccelerationStructureBuildSizesNV(VkDevice device, const VkClusterAccelerationStructureInputInfoNV* pInfo, VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo) {
    if (!p_vkGetClusterAccelerationStructureBuildSizesNV) p_vkGetClusterAccelerationStructureBuildSizesNV = (PFN_vkGetClusterAccelerationStructureBuildSizesNV)ps5_vk_proc("vkGetClusterAccelerationStructureBuildSizesNV");
    p_vkGetClusterAccelerationStructureBuildSizesNV(device, pInfo, pSizeInfo);
}
static PFN_vkCmdBuildClusterAccelerationStructureIndirectNV p_vkCmdBuildClusterAccelerationStructureIndirectNV;
VKAPI_ATTR void VKAPI_CALL vkCmdBuildClusterAccelerationStructureIndirectNV(VkCommandBuffer commandBuffer, const VkClusterAccelerationStructureCommandsInfoNV* pCommandInfos) {
    if (!p_vkCmdBuildClusterAccelerationStructureIndirectNV) p_vkCmdBuildClusterAccelerationStructureIndirectNV = (PFN_vkCmdBuildClusterAccelerationStructureIndirectNV)ps5_vk_proc("vkCmdBuildClusterAccelerationStructureIndirectNV");
    p_vkCmdBuildClusterAccelerationStructureIndirectNV(commandBuffer, pCommandInfos);
}
static PFN_vkGetPartitionedAccelerationStructuresBuildSizesNV p_vkGetPartitionedAccelerationStructuresBuildSizesNV;
VKAPI_ATTR void VKAPI_CALL vkGetPartitionedAccelerationStructuresBuildSizesNV(VkDevice device, const VkPartitionedAccelerationStructureInstancesInputNV* pInfo, VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo) {
    if (!p_vkGetPartitionedAccelerationStructuresBuildSizesNV) p_vkGetPartitionedAccelerationStructuresBuildSizesNV = (PFN_vkGetPartitionedAccelerationStructuresBuildSizesNV)ps5_vk_proc("vkGetPartitionedAccelerationStructuresBuildSizesNV");
    p_vkGetPartitionedAccelerationStructuresBuildSizesNV(device, pInfo, pSizeInfo);
}
static PFN_vkCmdBuildPartitionedAccelerationStructuresNV p_vkCmdBuildPartitionedAccelerationStructuresNV;
VKAPI_ATTR void VKAPI_CALL vkCmdBuildPartitionedAccelerationStructuresNV(VkCommandBuffer commandBuffer, const VkBuildPartitionedAccelerationStructureInfoNV* pBuildInfo) {
    if (!p_vkCmdBuildPartitionedAccelerationStructuresNV) p_vkCmdBuildPartitionedAccelerationStructuresNV = (PFN_vkCmdBuildPartitionedAccelerationStructuresNV)ps5_vk_proc("vkCmdBuildPartitionedAccelerationStructuresNV");
    p_vkCmdBuildPartitionedAccelerationStructuresNV(commandBuffer, pBuildInfo);
}
static PFN_vkGetGeneratedCommandsMemoryRequirementsEXT p_vkGetGeneratedCommandsMemoryRequirementsEXT;
VKAPI_ATTR void VKAPI_CALL vkGetGeneratedCommandsMemoryRequirementsEXT(VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoEXT* pInfo, VkMemoryRequirements2* pMemoryRequirements) {
    if (!p_vkGetGeneratedCommandsMemoryRequirementsEXT) p_vkGetGeneratedCommandsMemoryRequirementsEXT = (PFN_vkGetGeneratedCommandsMemoryRequirementsEXT)ps5_vk_proc("vkGetGeneratedCommandsMemoryRequirementsEXT");
    p_vkGetGeneratedCommandsMemoryRequirementsEXT(device, pInfo, pMemoryRequirements);
}
static PFN_vkCmdPreprocessGeneratedCommandsEXT p_vkCmdPreprocessGeneratedCommandsEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdPreprocessGeneratedCommandsEXT(VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoEXT* pGeneratedCommandsInfo, VkCommandBuffer stateCommandBuffer) {
    if (!p_vkCmdPreprocessGeneratedCommandsEXT) p_vkCmdPreprocessGeneratedCommandsEXT = (PFN_vkCmdPreprocessGeneratedCommandsEXT)ps5_vk_proc("vkCmdPreprocessGeneratedCommandsEXT");
    p_vkCmdPreprocessGeneratedCommandsEXT(commandBuffer, pGeneratedCommandsInfo, stateCommandBuffer);
}
static PFN_vkCmdExecuteGeneratedCommandsEXT p_vkCmdExecuteGeneratedCommandsEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdExecuteGeneratedCommandsEXT(VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoEXT* pGeneratedCommandsInfo) {
    if (!p_vkCmdExecuteGeneratedCommandsEXT) p_vkCmdExecuteGeneratedCommandsEXT = (PFN_vkCmdExecuteGeneratedCommandsEXT)ps5_vk_proc("vkCmdExecuteGeneratedCommandsEXT");
    p_vkCmdExecuteGeneratedCommandsEXT(commandBuffer, isPreprocessed, pGeneratedCommandsInfo);
}
static PFN_vkCreateIndirectCommandsLayoutEXT p_vkCreateIndirectCommandsLayoutEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateIndirectCommandsLayoutEXT(VkDevice device, const VkIndirectCommandsLayoutCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutEXT* pIndirectCommandsLayout) {
    if (!p_vkCreateIndirectCommandsLayoutEXT) p_vkCreateIndirectCommandsLayoutEXT = (PFN_vkCreateIndirectCommandsLayoutEXT)ps5_vk_proc("vkCreateIndirectCommandsLayoutEXT");
    return p_vkCreateIndirectCommandsLayoutEXT(device, pCreateInfo, pAllocator, pIndirectCommandsLayout);
}
static PFN_vkDestroyIndirectCommandsLayoutEXT p_vkDestroyIndirectCommandsLayoutEXT;
VKAPI_ATTR void VKAPI_CALL vkDestroyIndirectCommandsLayoutEXT(VkDevice device, VkIndirectCommandsLayoutEXT indirectCommandsLayout, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyIndirectCommandsLayoutEXT) p_vkDestroyIndirectCommandsLayoutEXT = (PFN_vkDestroyIndirectCommandsLayoutEXT)ps5_vk_proc("vkDestroyIndirectCommandsLayoutEXT");
    p_vkDestroyIndirectCommandsLayoutEXT(device, indirectCommandsLayout, pAllocator);
}
static PFN_vkCreateIndirectExecutionSetEXT p_vkCreateIndirectExecutionSetEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateIndirectExecutionSetEXT(VkDevice device, const VkIndirectExecutionSetCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectExecutionSetEXT* pIndirectExecutionSet) {
    if (!p_vkCreateIndirectExecutionSetEXT) p_vkCreateIndirectExecutionSetEXT = (PFN_vkCreateIndirectExecutionSetEXT)ps5_vk_proc("vkCreateIndirectExecutionSetEXT");
    return p_vkCreateIndirectExecutionSetEXT(device, pCreateInfo, pAllocator, pIndirectExecutionSet);
}
static PFN_vkDestroyIndirectExecutionSetEXT p_vkDestroyIndirectExecutionSetEXT;
VKAPI_ATTR void VKAPI_CALL vkDestroyIndirectExecutionSetEXT(VkDevice device, VkIndirectExecutionSetEXT indirectExecutionSet, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyIndirectExecutionSetEXT) p_vkDestroyIndirectExecutionSetEXT = (PFN_vkDestroyIndirectExecutionSetEXT)ps5_vk_proc("vkDestroyIndirectExecutionSetEXT");
    p_vkDestroyIndirectExecutionSetEXT(device, indirectExecutionSet, pAllocator);
}
static PFN_vkUpdateIndirectExecutionSetPipelineEXT p_vkUpdateIndirectExecutionSetPipelineEXT;
VKAPI_ATTR void VKAPI_CALL vkUpdateIndirectExecutionSetPipelineEXT(VkDevice device, VkIndirectExecutionSetEXT indirectExecutionSet, uint32_t executionSetWriteCount, const VkWriteIndirectExecutionSetPipelineEXT* pExecutionSetWrites) {
    if (!p_vkUpdateIndirectExecutionSetPipelineEXT) p_vkUpdateIndirectExecutionSetPipelineEXT = (PFN_vkUpdateIndirectExecutionSetPipelineEXT)ps5_vk_proc("vkUpdateIndirectExecutionSetPipelineEXT");
    p_vkUpdateIndirectExecutionSetPipelineEXT(device, indirectExecutionSet, executionSetWriteCount, pExecutionSetWrites);
}
static PFN_vkUpdateIndirectExecutionSetShaderEXT p_vkUpdateIndirectExecutionSetShaderEXT;
VKAPI_ATTR void VKAPI_CALL vkUpdateIndirectExecutionSetShaderEXT(VkDevice device, VkIndirectExecutionSetEXT indirectExecutionSet, uint32_t executionSetWriteCount, const VkWriteIndirectExecutionSetShaderEXT* pExecutionSetWrites) {
    if (!p_vkUpdateIndirectExecutionSetShaderEXT) p_vkUpdateIndirectExecutionSetShaderEXT = (PFN_vkUpdateIndirectExecutionSetShaderEXT)ps5_vk_proc("vkUpdateIndirectExecutionSetShaderEXT");
    p_vkUpdateIndirectExecutionSetShaderEXT(device, indirectExecutionSet, executionSetWriteCount, pExecutionSetWrites);
}
static PFN_vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV p_vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixFlexibleDimensionsPropertiesNV* pProperties) {
    if (!p_vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV) p_vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV = (PFN_vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV)ps5_vk_proc("vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV");
    return p_vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV(physicalDevice, pPropertyCount, pProperties);
}
static PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM p_vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM;
VKAPI_ATTR VkResult VKAPI_CALL vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pCounterCount, VkPerformanceCounterARM* pCounters, VkPerformanceCounterDescriptionARM* pCounterDescriptions) {
    if (!p_vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM) p_vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM = (PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM)ps5_vk_proc("vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM");
    return p_vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM(physicalDevice, queueFamilyIndex, pCounterCount, pCounters, pCounterDescriptions);
}
static PFN_vkEnumeratePhysicalDeviceShaderInstrumentationMetricsARM p_vkEnumeratePhysicalDeviceShaderInstrumentationMetricsARM;
VKAPI_ATTR VkResult VKAPI_CALL vkEnumeratePhysicalDeviceShaderInstrumentationMetricsARM(VkPhysicalDevice physicalDevice, uint32_t* pDescriptionCount, VkShaderInstrumentationMetricDescriptionARM* pDescriptions) {
    if (!p_vkEnumeratePhysicalDeviceShaderInstrumentationMetricsARM) p_vkEnumeratePhysicalDeviceShaderInstrumentationMetricsARM = (PFN_vkEnumeratePhysicalDeviceShaderInstrumentationMetricsARM)ps5_vk_proc("vkEnumeratePhysicalDeviceShaderInstrumentationMetricsARM");
    return p_vkEnumeratePhysicalDeviceShaderInstrumentationMetricsARM(physicalDevice, pDescriptionCount, pDescriptions);
}
static PFN_vkCreateShaderInstrumentationARM p_vkCreateShaderInstrumentationARM;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateShaderInstrumentationARM(VkDevice device, const VkShaderInstrumentationCreateInfoARM* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderInstrumentationARM* pInstrumentation) {
    if (!p_vkCreateShaderInstrumentationARM) p_vkCreateShaderInstrumentationARM = (PFN_vkCreateShaderInstrumentationARM)ps5_vk_proc("vkCreateShaderInstrumentationARM");
    return p_vkCreateShaderInstrumentationARM(device, pCreateInfo, pAllocator, pInstrumentation);
}
static PFN_vkDestroyShaderInstrumentationARM p_vkDestroyShaderInstrumentationARM;
VKAPI_ATTR void VKAPI_CALL vkDestroyShaderInstrumentationARM(VkDevice device, VkShaderInstrumentationARM instrumentation, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyShaderInstrumentationARM) p_vkDestroyShaderInstrumentationARM = (PFN_vkDestroyShaderInstrumentationARM)ps5_vk_proc("vkDestroyShaderInstrumentationARM");
    p_vkDestroyShaderInstrumentationARM(device, instrumentation, pAllocator);
}
static PFN_vkCmdBeginShaderInstrumentationARM p_vkCmdBeginShaderInstrumentationARM;
VKAPI_ATTR void VKAPI_CALL vkCmdBeginShaderInstrumentationARM(VkCommandBuffer commandBuffer, VkShaderInstrumentationARM instrumentation) {
    if (!p_vkCmdBeginShaderInstrumentationARM) p_vkCmdBeginShaderInstrumentationARM = (PFN_vkCmdBeginShaderInstrumentationARM)ps5_vk_proc("vkCmdBeginShaderInstrumentationARM");
    p_vkCmdBeginShaderInstrumentationARM(commandBuffer, instrumentation);
}
static PFN_vkCmdEndShaderInstrumentationARM p_vkCmdEndShaderInstrumentationARM;
VKAPI_ATTR void VKAPI_CALL vkCmdEndShaderInstrumentationARM(VkCommandBuffer commandBuffer) {
    if (!p_vkCmdEndShaderInstrumentationARM) p_vkCmdEndShaderInstrumentationARM = (PFN_vkCmdEndShaderInstrumentationARM)ps5_vk_proc("vkCmdEndShaderInstrumentationARM");
    p_vkCmdEndShaderInstrumentationARM(commandBuffer);
}
static PFN_vkGetShaderInstrumentationValuesARM p_vkGetShaderInstrumentationValuesARM;
VKAPI_ATTR VkResult VKAPI_CALL vkGetShaderInstrumentationValuesARM(VkDevice device, VkShaderInstrumentationARM instrumentation, uint32_t* pMetricBlockCount, void* pMetricValues, VkShaderInstrumentationValuesFlagsARM flags) {
    if (!p_vkGetShaderInstrumentationValuesARM) p_vkGetShaderInstrumentationValuesARM = (PFN_vkGetShaderInstrumentationValuesARM)ps5_vk_proc("vkGetShaderInstrumentationValuesARM");
    return p_vkGetShaderInstrumentationValuesARM(device, instrumentation, pMetricBlockCount, pMetricValues, flags);
}
static PFN_vkClearShaderInstrumentationMetricsARM p_vkClearShaderInstrumentationMetricsARM;
VKAPI_ATTR void VKAPI_CALL vkClearShaderInstrumentationMetricsARM(VkDevice device, VkShaderInstrumentationARM instrumentation) {
    if (!p_vkClearShaderInstrumentationMetricsARM) p_vkClearShaderInstrumentationMetricsARM = (PFN_vkClearShaderInstrumentationMetricsARM)ps5_vk_proc("vkClearShaderInstrumentationMetricsARM");
    p_vkClearShaderInstrumentationMetricsARM(device, instrumentation);
}
static PFN_vkCmdEndRendering2EXT p_vkCmdEndRendering2EXT;
VKAPI_ATTR void VKAPI_CALL vkCmdEndRendering2EXT(VkCommandBuffer commandBuffer, const VkRenderingEndInfoKHR* pRenderingEndInfo) {
    if (!p_vkCmdEndRendering2EXT) p_vkCmdEndRendering2EXT = (PFN_vkCmdEndRendering2EXT)ps5_vk_proc("vkCmdEndRendering2EXT");
    p_vkCmdEndRendering2EXT(commandBuffer, pRenderingEndInfo);
}
static PFN_vkCmdBeginCustomResolveEXT p_vkCmdBeginCustomResolveEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdBeginCustomResolveEXT(VkCommandBuffer commandBuffer, const VkBeginCustomResolveInfoEXT* pBeginCustomResolveInfo) {
    if (!p_vkCmdBeginCustomResolveEXT) p_vkCmdBeginCustomResolveEXT = (PFN_vkCmdBeginCustomResolveEXT)ps5_vk_proc("vkCmdBeginCustomResolveEXT");
    p_vkCmdBeginCustomResolveEXT(commandBuffer, pBeginCustomResolveInfo);
}
static PFN_vkGetPhysicalDeviceQueueFamilyDataGraphOpticalFlowImageFormatsARM p_vkGetPhysicalDeviceQueueFamilyDataGraphOpticalFlowImageFormatsARM;
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceQueueFamilyDataGraphOpticalFlowImageFormatsARM(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, const VkQueueFamilyDataGraphPropertiesARM* pQueueFamilyDataGraphProperties, const VkDataGraphOpticalFlowImageFormatInfoARM* pOpticalFlowImageFormatInfo, uint32_t* pFormatCount, VkDataGraphOpticalFlowImageFormatPropertiesARM* pImageFormatProperties) {
    if (!p_vkGetPhysicalDeviceQueueFamilyDataGraphOpticalFlowImageFormatsARM) p_vkGetPhysicalDeviceQueueFamilyDataGraphOpticalFlowImageFormatsARM = (PFN_vkGetPhysicalDeviceQueueFamilyDataGraphOpticalFlowImageFormatsARM)ps5_vk_proc("vkGetPhysicalDeviceQueueFamilyDataGraphOpticalFlowImageFormatsARM");
    return p_vkGetPhysicalDeviceQueueFamilyDataGraphOpticalFlowImageFormatsARM(physicalDevice, queueFamilyIndex, pQueueFamilyDataGraphProperties, pOpticalFlowImageFormatInfo, pFormatCount, pImageFormatProperties);
}
static PFN_vkCmdSetComputeOccupancyPriorityNV p_vkCmdSetComputeOccupancyPriorityNV;
VKAPI_ATTR void VKAPI_CALL vkCmdSetComputeOccupancyPriorityNV(VkCommandBuffer commandBuffer, const VkComputeOccupancyPriorityParametersNV* pParameters) {
    if (!p_vkCmdSetComputeOccupancyPriorityNV) p_vkCmdSetComputeOccupancyPriorityNV = (PFN_vkCmdSetComputeOccupancyPriorityNV)ps5_vk_proc("vkCmdSetComputeOccupancyPriorityNV");
    p_vkCmdSetComputeOccupancyPriorityNV(commandBuffer, pParameters);
}
static PFN_vkCmdSetPrimitiveRestartIndexEXT p_vkCmdSetPrimitiveRestartIndexEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdSetPrimitiveRestartIndexEXT(VkCommandBuffer commandBuffer, uint32_t primitiveRestartIndex) {
    if (!p_vkCmdSetPrimitiveRestartIndexEXT) p_vkCmdSetPrimitiveRestartIndexEXT = (PFN_vkCmdSetPrimitiveRestartIndexEXT)ps5_vk_proc("vkCmdSetPrimitiveRestartIndexEXT");
    p_vkCmdSetPrimitiveRestartIndexEXT(commandBuffer, primitiveRestartIndex);
}
static PFN_vkCreateAccelerationStructureKHR p_vkCreateAccelerationStructureKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateAccelerationStructureKHR(VkDevice device, const VkAccelerationStructureCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureKHR* pAccelerationStructure) {
    if (!p_vkCreateAccelerationStructureKHR) p_vkCreateAccelerationStructureKHR = (PFN_vkCreateAccelerationStructureKHR)ps5_vk_proc("vkCreateAccelerationStructureKHR");
    return p_vkCreateAccelerationStructureKHR(device, pCreateInfo, pAllocator, pAccelerationStructure);
}
static PFN_vkDestroyAccelerationStructureKHR p_vkDestroyAccelerationStructureKHR;
VKAPI_ATTR void VKAPI_CALL vkDestroyAccelerationStructureKHR(VkDevice device, VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator) {
    if (!p_vkDestroyAccelerationStructureKHR) p_vkDestroyAccelerationStructureKHR = (PFN_vkDestroyAccelerationStructureKHR)ps5_vk_proc("vkDestroyAccelerationStructureKHR");
    p_vkDestroyAccelerationStructureKHR(device, accelerationStructure, pAllocator);
}
static PFN_vkCmdBuildAccelerationStructuresKHR p_vkCmdBuildAccelerationStructuresKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdBuildAccelerationStructuresKHR(VkCommandBuffer commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) {
    if (!p_vkCmdBuildAccelerationStructuresKHR) p_vkCmdBuildAccelerationStructuresKHR = (PFN_vkCmdBuildAccelerationStructuresKHR)ps5_vk_proc("vkCmdBuildAccelerationStructuresKHR");
    p_vkCmdBuildAccelerationStructuresKHR(commandBuffer, infoCount, pInfos, ppBuildRangeInfos);
}
static PFN_vkCmdBuildAccelerationStructuresIndirectKHR p_vkCmdBuildAccelerationStructuresIndirectKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress* pIndirectDeviceAddresses, const uint32_t* pIndirectStrides, const uint32_t* const* ppMaxPrimitiveCounts) {
    if (!p_vkCmdBuildAccelerationStructuresIndirectKHR) p_vkCmdBuildAccelerationStructuresIndirectKHR = (PFN_vkCmdBuildAccelerationStructuresIndirectKHR)ps5_vk_proc("vkCmdBuildAccelerationStructuresIndirectKHR");
    p_vkCmdBuildAccelerationStructuresIndirectKHR(commandBuffer, infoCount, pInfos, pIndirectDeviceAddresses, pIndirectStrides, ppMaxPrimitiveCounts);
}
static PFN_vkBuildAccelerationStructuresKHR p_vkBuildAccelerationStructuresKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkBuildAccelerationStructuresKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) {
    if (!p_vkBuildAccelerationStructuresKHR) p_vkBuildAccelerationStructuresKHR = (PFN_vkBuildAccelerationStructuresKHR)ps5_vk_proc("vkBuildAccelerationStructuresKHR");
    return p_vkBuildAccelerationStructuresKHR(device, deferredOperation, infoCount, pInfos, ppBuildRangeInfos);
}
static PFN_vkCopyAccelerationStructureKHR p_vkCopyAccelerationStructureKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkCopyAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureInfoKHR* pInfo) {
    if (!p_vkCopyAccelerationStructureKHR) p_vkCopyAccelerationStructureKHR = (PFN_vkCopyAccelerationStructureKHR)ps5_vk_proc("vkCopyAccelerationStructureKHR");
    return p_vkCopyAccelerationStructureKHR(device, deferredOperation, pInfo);
}
static PFN_vkCopyAccelerationStructureToMemoryKHR p_vkCopyAccelerationStructureToMemoryKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkCopyAccelerationStructureToMemoryKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) {
    if (!p_vkCopyAccelerationStructureToMemoryKHR) p_vkCopyAccelerationStructureToMemoryKHR = (PFN_vkCopyAccelerationStructureToMemoryKHR)ps5_vk_proc("vkCopyAccelerationStructureToMemoryKHR");
    return p_vkCopyAccelerationStructureToMemoryKHR(device, deferredOperation, pInfo);
}
static PFN_vkCopyMemoryToAccelerationStructureKHR p_vkCopyMemoryToAccelerationStructureKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkCopyMemoryToAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) {
    if (!p_vkCopyMemoryToAccelerationStructureKHR) p_vkCopyMemoryToAccelerationStructureKHR = (PFN_vkCopyMemoryToAccelerationStructureKHR)ps5_vk_proc("vkCopyMemoryToAccelerationStructureKHR");
    return p_vkCopyMemoryToAccelerationStructureKHR(device, deferredOperation, pInfo);
}
static PFN_vkWriteAccelerationStructuresPropertiesKHR p_vkWriteAccelerationStructuresPropertiesKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkWriteAccelerationStructuresPropertiesKHR(VkDevice device, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, size_t dataSize, void* pData, size_t stride) {
    if (!p_vkWriteAccelerationStructuresPropertiesKHR) p_vkWriteAccelerationStructuresPropertiesKHR = (PFN_vkWriteAccelerationStructuresPropertiesKHR)ps5_vk_proc("vkWriteAccelerationStructuresPropertiesKHR");
    return p_vkWriteAccelerationStructuresPropertiesKHR(device, accelerationStructureCount, pAccelerationStructures, queryType, dataSize, pData, stride);
}
static PFN_vkCmdCopyAccelerationStructureKHR p_vkCmdCopyAccelerationStructureKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo) {
    if (!p_vkCmdCopyAccelerationStructureKHR) p_vkCmdCopyAccelerationStructureKHR = (PFN_vkCmdCopyAccelerationStructureKHR)ps5_vk_proc("vkCmdCopyAccelerationStructureKHR");
    p_vkCmdCopyAccelerationStructureKHR(commandBuffer, pInfo);
}
static PFN_vkCmdCopyAccelerationStructureToMemoryKHR p_vkCmdCopyAccelerationStructureToMemoryKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) {
    if (!p_vkCmdCopyAccelerationStructureToMemoryKHR) p_vkCmdCopyAccelerationStructureToMemoryKHR = (PFN_vkCmdCopyAccelerationStructureToMemoryKHR)ps5_vk_proc("vkCmdCopyAccelerationStructureToMemoryKHR");
    p_vkCmdCopyAccelerationStructureToMemoryKHR(commandBuffer, pInfo);
}
static PFN_vkCmdCopyMemoryToAccelerationStructureKHR p_vkCmdCopyMemoryToAccelerationStructureKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) {
    if (!p_vkCmdCopyMemoryToAccelerationStructureKHR) p_vkCmdCopyMemoryToAccelerationStructureKHR = (PFN_vkCmdCopyMemoryToAccelerationStructureKHR)ps5_vk_proc("vkCmdCopyMemoryToAccelerationStructureKHR");
    p_vkCmdCopyMemoryToAccelerationStructureKHR(commandBuffer, pInfo);
}
static PFN_vkGetAccelerationStructureDeviceAddressKHR p_vkGetAccelerationStructureDeviceAddressKHR;
VKAPI_ATTR VkDeviceAddress VKAPI_CALL vkGetAccelerationStructureDeviceAddressKHR(VkDevice device, const VkAccelerationStructureDeviceAddressInfoKHR* pInfo) {
    if (!p_vkGetAccelerationStructureDeviceAddressKHR) p_vkGetAccelerationStructureDeviceAddressKHR = (PFN_vkGetAccelerationStructureDeviceAddressKHR)ps5_vk_proc("vkGetAccelerationStructureDeviceAddressKHR");
    return p_vkGetAccelerationStructureDeviceAddressKHR(device, pInfo);
}
static PFN_vkCmdWriteAccelerationStructuresPropertiesKHR p_vkCmdWriteAccelerationStructuresPropertiesKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) {
    if (!p_vkCmdWriteAccelerationStructuresPropertiesKHR) p_vkCmdWriteAccelerationStructuresPropertiesKHR = (PFN_vkCmdWriteAccelerationStructuresPropertiesKHR)ps5_vk_proc("vkCmdWriteAccelerationStructuresPropertiesKHR");
    p_vkCmdWriteAccelerationStructuresPropertiesKHR(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}
static PFN_vkGetDeviceAccelerationStructureCompatibilityKHR p_vkGetDeviceAccelerationStructureCompatibilityKHR;
VKAPI_ATTR void VKAPI_CALL vkGetDeviceAccelerationStructureCompatibilityKHR(VkDevice device, const VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) {
    if (!p_vkGetDeviceAccelerationStructureCompatibilityKHR) p_vkGetDeviceAccelerationStructureCompatibilityKHR = (PFN_vkGetDeviceAccelerationStructureCompatibilityKHR)ps5_vk_proc("vkGetDeviceAccelerationStructureCompatibilityKHR");
    p_vkGetDeviceAccelerationStructureCompatibilityKHR(device, pVersionInfo, pCompatibility);
}
static PFN_vkGetAccelerationStructureBuildSizesKHR p_vkGetAccelerationStructureBuildSizesKHR;
VKAPI_ATTR void VKAPI_CALL vkGetAccelerationStructureBuildSizesKHR(VkDevice device, VkAccelerationStructureBuildTypeKHR buildType, const VkAccelerationStructureBuildGeometryInfoKHR* pBuildInfo, const uint32_t* pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo) {
    if (!p_vkGetAccelerationStructureBuildSizesKHR) p_vkGetAccelerationStructureBuildSizesKHR = (PFN_vkGetAccelerationStructureBuildSizesKHR)ps5_vk_proc("vkGetAccelerationStructureBuildSizesKHR");
    p_vkGetAccelerationStructureBuildSizesKHR(device, buildType, pBuildInfo, pMaxPrimitiveCounts, pSizeInfo);
}
static PFN_vkCmdTraceRaysKHR p_vkCmdTraceRaysKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdTraceRaysKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth) {
    if (!p_vkCmdTraceRaysKHR) p_vkCmdTraceRaysKHR = (PFN_vkCmdTraceRaysKHR)ps5_vk_proc("vkCmdTraceRaysKHR");
    p_vkCmdTraceRaysKHR(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, width, height, depth);
}
static PFN_vkCreateRayTracingPipelinesKHR p_vkCreateRayTracingPipelinesKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateRayTracingPipelinesKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) {
    if (!p_vkCreateRayTracingPipelinesKHR) p_vkCreateRayTracingPipelinesKHR = (PFN_vkCreateRayTracingPipelinesKHR)ps5_vk_proc("vkCreateRayTracingPipelinesKHR");
    return p_vkCreateRayTracingPipelinesKHR(device, deferredOperation, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}
static PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR p_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR;
VKAPI_ATTR VkResult VKAPI_CALL vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) {
    if (!p_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR) p_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR = (PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR)ps5_vk_proc("vkGetRayTracingCaptureReplayShaderGroupHandlesKHR");
    return p_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(device, pipeline, firstGroup, groupCount, dataSize, pData);
}
static PFN_vkCmdTraceRaysIndirectKHR p_vkCmdTraceRaysIndirectKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress) {
    if (!p_vkCmdTraceRaysIndirectKHR) p_vkCmdTraceRaysIndirectKHR = (PFN_vkCmdTraceRaysIndirectKHR)ps5_vk_proc("vkCmdTraceRaysIndirectKHR");
    p_vkCmdTraceRaysIndirectKHR(commandBuffer, pRaygenShaderBindingTable, pMissShaderBindingTable, pHitShaderBindingTable, pCallableShaderBindingTable, indirectDeviceAddress);
}
static PFN_vkGetRayTracingShaderGroupStackSizeKHR p_vkGetRayTracingShaderGroupStackSizeKHR;
VKAPI_ATTR VkDeviceSize VKAPI_CALL vkGetRayTracingShaderGroupStackSizeKHR(VkDevice device, VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader) {
    if (!p_vkGetRayTracingShaderGroupStackSizeKHR) p_vkGetRayTracingShaderGroupStackSizeKHR = (PFN_vkGetRayTracingShaderGroupStackSizeKHR)ps5_vk_proc("vkGetRayTracingShaderGroupStackSizeKHR");
    return p_vkGetRayTracingShaderGroupStackSizeKHR(device, pipeline, group, groupShader);
}
static PFN_vkCmdSetRayTracingPipelineStackSizeKHR p_vkCmdSetRayTracingPipelineStackSizeKHR;
VKAPI_ATTR void VKAPI_CALL vkCmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize) {
    if (!p_vkCmdSetRayTracingPipelineStackSizeKHR) p_vkCmdSetRayTracingPipelineStackSizeKHR = (PFN_vkCmdSetRayTracingPipelineStackSizeKHR)ps5_vk_proc("vkCmdSetRayTracingPipelineStackSizeKHR");
    p_vkCmdSetRayTracingPipelineStackSizeKHR(commandBuffer, pipelineStackSize);
}
static PFN_vkCmdDrawMeshTasksEXT p_vkCmdDrawMeshTasksEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawMeshTasksEXT(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
    if (!p_vkCmdDrawMeshTasksEXT) p_vkCmdDrawMeshTasksEXT = (PFN_vkCmdDrawMeshTasksEXT)ps5_vk_proc("vkCmdDrawMeshTasksEXT");
    p_vkCmdDrawMeshTasksEXT(commandBuffer, groupCountX, groupCountY, groupCountZ);
}
static PFN_vkCmdDrawMeshTasksIndirectEXT p_vkCmdDrawMeshTasksIndirectEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawMeshTasksIndirectEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
    if (!p_vkCmdDrawMeshTasksIndirectEXT) p_vkCmdDrawMeshTasksIndirectEXT = (PFN_vkCmdDrawMeshTasksIndirectEXT)ps5_vk_proc("vkCmdDrawMeshTasksIndirectEXT");
    p_vkCmdDrawMeshTasksIndirectEXT(commandBuffer, buffer, offset, drawCount, stride);
}
static PFN_vkCmdDrawMeshTasksIndirectCountEXT p_vkCmdDrawMeshTasksIndirectCountEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdDrawMeshTasksIndirectCountEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) {
    if (!p_vkCmdDrawMeshTasksIndirectCountEXT) p_vkCmdDrawMeshTasksIndirectCountEXT = (PFN_vkCmdDrawMeshTasksIndirectCountEXT)ps5_vk_proc("vkCmdDrawMeshTasksIndirectCountEXT");
    p_vkCmdDrawMeshTasksIndirectCountEXT(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
#endif
