#include "BasePositionableObject.h"

#include "NewFramework/Platform/Shared/Logging.h"

void CBasePositionableObject::_AddChild(CBasePositionableObject* child, bool first) {
	if (child == nullptr) {
		return;
	}
	if (mDefaultDirtyState) {
		child->SetDefaultDirtyState(mDefaultDirtyState);
	}
	if (child->mParent != nullptr) {
		child->mParent->RemoveChild(child);
	}
	child->mParent = this;
	child->FlagDirty();
	if (first) {
		mChildren.push_front(child);
	} else {
		mChildren.push_back(child);
	}
}

void CBasePositionableObject::RemoveChild(CBasePositionableObject* child) {
	if (child == nullptr) {
		return;
	}

	child->mParent = nullptr;
	mChildren.remove(child);
	child->unlink();
}


void CBasePositionableObject::FlagDirty() {
	mDirtyFlag = true;
	for (auto child : mChildren) {
		child->FlagDirty();
	}
}


void CBasePositionableObject::Draw(bool recursive) {
	if (!mVisible) {
		return;
	}
	UpdateTransform();
	if (!recursive) {
		return;
	}
	BaseDrawChildren();
}

void CBasePositionableObject::Process(const SGameTime&) {}

void CBasePositionableObject::Process_r(const SGameTime& gameTime) {
	Process(gameTime);
	for (auto child : mChildren) {
		child->Process_r(gameTime);
	}
}

void CBasePositionableObject::AddFirstChild() {

}






















































































void CBasePositionableObject::HitTest(CVec2 point, float) {
	LOG_ERROR("Fallen into blank base HitTest(CVec2, float)");
	ENFORCE_LINE(154);
}
