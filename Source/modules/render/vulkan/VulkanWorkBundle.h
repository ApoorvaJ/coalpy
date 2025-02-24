#pragma once
#include <stdint.h>
#include <vector>
#include "WorkBundleDb.h"
#include "VulkanFencePool.h"
#include "VulkanQueues.h"
#include "VulkanGpuMemPools.h"
#include "VulkanReadbackBufferPool.h"

namespace coalpy
{
namespace render
{

class CommandList;
class VulkanDevice;
class VulkanGpuDescriptorSetPool;
struct VulkanList;
struct AbiCopyCmd;
struct AbiUploadCmd;

struct VulkanResourceDownloadState
{
    ResourceDownloadKey downloadKey;
    WorkType queueType = WorkType::Graphics;
    VulkanFenceHandle fenceValue;
    VulkanReadbackMemBlock memoryBlock;
    uint64_t requestedSize;
    ResourceHandle resource;
    size_t rowPitch = 0;
    int width  = 0;
    int height = 0;
    int depth  = 0;
};

using VulkanDownloadResourceMap = std::unordered_map<ResourceDownloadKey, VulkanResourceDownloadState>;

class VulkanWorkBundle
{
public:
    VulkanWorkBundle(VulkanDevice& device) : m_device(device) {}
    bool load(const WorkBundle& workBundle);
    VulkanFenceHandle execute(CommandList** commandLists, int commandListsCount);
    void getDownloadResourceMap(VulkanDownloadResourceMap& downloadMap);

private:
    void buildComputeCmd(const unsigned char* data, const AbiComputeCmd* computeCmd, const CommandInfo& cmdInfo, VulkanList& outList);
    void buildUploadCmd(const unsigned char* data, const AbiUploadCmd* uploadCmd, const CommandInfo& cmdInfo, VulkanList& outList);
    void buildCopyCmd(const unsigned char* data, const AbiCopyCmd* copyCmd, const CommandInfo& cmdInfo, VulkanList& outList);
    void buildDownloadCmd(const unsigned char* data, const AbiDownloadCmd* downloadCmd,const CommandInfo& cmdInfo, VulkanFenceHandle fenceValue, VulkanList& outList);
    void buildCopyAppendConsumeCounter(const unsigned char* data, const AbiCopyAppendConsumeCounter* abiCmd, const CommandInfo& cmdInfo, VulkanList& outList);
    void buildClearAppendConsumeCounter(const unsigned char* data, const AbiClearAppendConsumeCounter* abiCmd, const CommandInfo& cmdInfo, VulkanList& outList);
        
    void buildCommandList(int listIndex, const CommandList* cmdList, WorkType workType, VulkanList& list, std::vector<VulkanEventHandle>& events, VulkanFenceHandle fenceValue);

    WorkBundle m_workBundle;
    VulkanDevice& m_device;
    VulkanGpuDescriptorSetPool* m_descriptors = nullptr;
    VulkanGpuMemoryBlock m_uploadMemBlock;
    std::vector<VulkanResourceDownloadState> m_downloadStates;
};

}
}
