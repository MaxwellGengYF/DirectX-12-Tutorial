#pragma once
#include <stdafx.h>
#include <unordered_map>
#include <unordered_set>
class Resource;
class ResourceStateTracker  {
private:
    struct State {
        D3D12_RESOURCE_STATES lastState;
        D3D12_RESOURCE_STATES curState;
        bool uavBarrier;
        bool isWrite;
    };
    std::unordered_map<Resource const *, State> stateMap;
	std::unordered_set<Resource const*> writeStateMap;
    std::vector<D3D12_RESOURCE_BARRIER> states;
    void ExecuteStateMap();
    void RestoreStateMap();
    void MarkWritable(Resource const *res, bool writable);

public:
	std::unordered_set<Resource const*> const& WriteStateMap() const { return writeStateMap; }
    ResourceStateTracker();
    ~ResourceStateTracker();
    void RecordState(
        Resource const *resource,
        D3D12_RESOURCE_STATES state);
    void RecordState(Resource const *resource);
    void UpdateState(ID3D12GraphicsCommandList* cmdList);
	void RestoreState(ID3D12GraphicsCommandList* cmdList);
};
