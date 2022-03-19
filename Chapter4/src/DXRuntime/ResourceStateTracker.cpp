#include <DXRuntime/ResourceStateTracker.h>
#include <Resource/Resource.h>
namespace detail {
static bool IsWriteState(D3D12_RESOURCE_STATES state) {
	switch (state) {
		case D3D12_RESOURCE_STATE_UNORDERED_ACCESS:
		case D3D12_RESOURCE_STATE_COPY_DEST:
			return true;
		default:
			return false;
	}
}
}// namespace detail
ResourceStateTracker::ResourceStateTracker() {
}
ResourceStateTracker::~ResourceStateTracker() = default;
void ResourceStateTracker::RecordState(Resource const* resource, D3D12_RESOURCE_STATES state) {
	auto initState = resource->GetInitState();
	auto ite = stateMap.try_emplace(resource);
	bool isWrite = detail::IsWriteState(state);
	if (ite.second) {
		if (isWrite) {
			writeStateMap.emplace(resource);
		}
		ite.first->second = State{
			.lastState = initState,
			.curState = state,
			.uavBarrier = (state == D3D12_RESOURCE_STATE_UNORDERED_ACCESS && initState == state),
			.isWrite = isWrite};
	} else {
		auto&& st = ite.first->second;
		st.uavBarrier = (state == D3D12_RESOURCE_STATE_UNORDERED_ACCESS && st.lastState == state);
		st.curState = state;
		if (isWrite != st.isWrite) {
			st.isWrite = isWrite;
			MarkWritable(resource, isWrite);
		}
	}
}
void ResourceStateTracker::RecordState(Resource const* resource) {
	RecordState(resource, resource->GetInitState());
}
void ResourceStateTracker::ExecuteStateMap() {
	for (auto&& i : stateMap) {
		if (i.second.uavBarrier) {
			D3D12_RESOURCE_BARRIER& uavBarrier = states.emplace_back();
			uavBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
			uavBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			uavBarrier.UAV.pResource = i.first->GetResource();
			i.second.uavBarrier = false;
		} else if (i.second.curState != i.second.lastState) {
			D3D12_RESOURCE_BARRIER& transBarrier = states.emplace_back();
			transBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			transBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			transBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			transBarrier.Transition.pResource = i.first->GetResource();
			transBarrier.Transition.StateBefore = i.second.lastState;
			transBarrier.Transition.StateAfter = i.second.curState;
		}
		i.second.lastState = i.second.curState;
	}
}
void ResourceStateTracker::RestoreStateMap() {
	for (auto&& i : stateMap) {
		i.second.curState = i.first->GetInitState();
		bool isWrite = detail::IsWriteState(i.second.curState);
		if (isWrite != i.second.isWrite) {
			MarkWritable(i.first, isWrite);
		}
		bool useUavBarrier =
			(i.second.lastState == D3D12_RESOURCE_STATE_UNORDERED_ACCESS && i.second.curState == D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

		if (useUavBarrier) {
			D3D12_RESOURCE_BARRIER& uavBarrier = states.emplace_back();
			uavBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
			uavBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			uavBarrier.UAV.pResource = i.first->GetResource();
		} else if (i.second.curState != i.second.lastState) {
			D3D12_RESOURCE_BARRIER& transBarrier = states.emplace_back();
			transBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			transBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			transBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			transBarrier.Transition.pResource = i.first->GetResource();
			transBarrier.Transition.StateBefore = i.second.lastState;
			transBarrier.Transition.StateAfter = i.second.curState;
		}
	}
	stateMap.clear();
}

void ResourceStateTracker::UpdateState(ID3D12GraphicsCommandList* cmdList) {
	ExecuteStateMap();
	if (!states.empty()) {
		cmdList->ResourceBarrier(
			states.size(),
			states.data());
		states.clear();
	}
}
void ResourceStateTracker::RestoreState(ID3D12GraphicsCommandList* cmdList) {
	RestoreStateMap();
	if (!states.empty()) {
		cmdList->ResourceBarrier(
			states.size(),
			states.data());
		states.clear();
	}
}

void ResourceStateTracker::MarkWritable(Resource const* res, bool writable) {
	if (writable) {
		writeStateMap.emplace(res);
	} else {
		writeStateMap.erase(res);
	}
}
