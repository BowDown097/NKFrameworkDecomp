#include "BasePositionableObject.h"

#include "NewFramework/Platform/Shared/Logging.h"

size_t CBasePositionableObject::m_uUpdateTransforms = 0;

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
	UpdateTransform(false);
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

void CBasePositionableObject::AddFirstChild(CBasePositionableObject* child) {
	_AddChild(child, true);
}

void CBasePositionableObject::AddChildBefore(CBasePositionableObject* child, CBasePositionableObject* before) {
	if (child == nullptr) {
		return;
	}
	if (mDefaultDirtyState != false) {
		child->SetDefaultDirtyState(mDefaultDirtyState);
	}
	if (child->mParent != nullptr) {
		child->mParent->RemoveChild(child);
	}
	child->mParent = this;
	child->FlagDirty();
	auto it = mChildren.begin();
	while (it != mChildren.end()) {
		if (*it == before) {
			mChildren.insert(it, child);
			break;
		}
		++it;
	}
}
void CBasePositionableObject::AddChildAfter(CBasePositionableObject* child, CBasePositionableObject* after) {
	if (child == nullptr) {
		return;
	}
	if (mDefaultDirtyState != false) {
		child->SetDefaultDirtyState(mDefaultDirtyState);
	}
	if (child->mParent != nullptr) {
		child->mParent->RemoveChild(child);
	}
	child->mParent = this;
	child->FlagDirty();
	auto it = mChildren.end();
	do {
		--it;
		if (*it == after) {
			mChildren.insert(it, child);
			break;
		}
	} while (it != mChildren.begin());
}

void CBasePositionableObject::DeleteChildren() {
	_DeleteChildrenInternal();
	mChildren.clear();
}

void CBasePositionableObject::SetXYZ(const CVec3 vec) {
	mLocation = vec;
	FlagDirty();
}

void CBasePositionableObject::SetXY(CVec2 vec) {
	mLocation.mX = vec.mX;
	mLocation.mY = vec.mY;
	FlagDirty();
}

void CBasePositionableObject::SetXY(float x, float y) {
	mLocation.mX = x;
	mLocation.mY = y;
	FlagDirty();
}

void CBasePositionableObject::SetX(float x) {
	mLocation.mX = x;
	FlagDirty();
}

void CBasePositionableObject::SetY(float y) {
	mLocation.mY = y;
	FlagDirty();
}

void CBasePositionableObject::SetZ(float z) {
	mLocation.mZ = z;
	FlagDirty();
}

void CBasePositionableObject::MoveXYZ(CVec3 vec) {
	mLocation += vec;
	FlagDirty();
}

void CBasePositionableObject::HitTest(CVec2 point, float) {
	LOG_ERROR("Fallen into blank base");
	ENFORCE_LINE(154);
}

void CBasePositionableObject::MoveXY(CVec2 vec) {
	mLocation.mX += vec.mX;
	mLocation.mY += vec.mY;
	FlagDirty();
}

void CBasePositionableObject::MoveX(float x) {
	mLocation.mX += x;
	FlagDirty();
}

void CBasePositionableObject::MoveY(float y) {
	mLocation.mY += y;
	FlagDirty();
}

void CBasePositionableObject::MoveZ(float z) {
	mLocation.mZ += z;
	FlagDirty();
}

void CBasePositionableObject::SetScale(float x, float y) {
	mScale.mX = x;
	mScale.mY = y;
	FlagDirty();
}

void CBasePositionableObject::SetScale(CVec2 vec) {
	mScale = vec;
	FlagDirty();
}

void CBasePositionableObject::SetScaleX(float x) {
	mScale.mX = x;
	FlagDirty();
}

void CBasePositionableObject::SetScaleY(float y) {
	mScale.mY = y;
	FlagDirty();
}

void CBasePositionableObject::SetAngle(float angle) {
	mRotAngle = angle;
	FlagDirty();
}

void CBasePositionableObject::SetWH(CVec2 vec) {
	mSize = vec;
	mSizeHalf = vec * 0.5f;
}

void CBasePositionableObject::SetW(float w) {
	mSize.mX = w;
	mSizeHalf = mSize * 0.5f;
}

void CBasePositionableObject::SetH(float h) {
	mSize.mY = h;
	mSizeHalf = mSize * 0.5f;
}

void CBasePositionableObject::SetAlignmentX(SPRITE_ALIGNMENT alignment) {
	mAlignment[0] = alignment;
}

void CBasePositionableObject::SetAlignmentY(SPRITE_ALIGNMENT alignment) {
	mAlignment[1] = alignment;
}

void CBasePositionableObject::SetAlignmentXY(SPRITE_ALIGNMENT x, SPRITE_ALIGNMENT y) {
	mAlignment[0] = x;
	mAlignment[1] = y;
}

CVec3 CBasePositionableObject::GetXYZ() {
	return mLocation;
}

CVec2 CBasePositionableObject::GetXY() {
	return mLocation;
}

float CBasePositionableObject::GetX() {
	return mLocation.mX;
}

float CBasePositionableObject::GetY() {
	return mLocation.mY;
}

float CBasePositionableObject::GetZ() {
	return mLocation.mZ;
}

CVec2 CBasePositionableObject::GetWH() {
	return mSize;
}

float CBasePositionableObject::GetW() {
	return mSize.mX;
}

float CBasePositionableObject::GetH() {
	return mSize.mY;
}

void CBasePositionableObject::UpdateLocalTransform() {
	if (!mDirtyFlag) {
		return;
	}

	m_uUpdateTransforms++;
	mTransform.Reset(mLocation, mRotAngle);
	mTransform.Scale(mScale);
	mDirtyFlag = mDefaultDirtyState;
}

void CBasePositionableObject::UpdateTransform(bool cascadeUp) {
	if (!mDirtyFlag) {
		return;
	}
	UpdateLocalTransform();
	if (mParent == nullptr) {
		return;
	}
	if (mParent->mDirtyFlag != false && cascadeUp) {
		mParent->UpdateTransform(cascadeUp);
	}

	mTransform = mParent->mTransform * mTransform;
}

void CBasePositionableObject::UpdateTransform_r() {
	UpdateTransform(false);
	for (auto child : mChildren) {
		child->UpdateTransform_r();
	}
}

void CBasePositionableObject::ForceUpdate() {}

void CBasePositionableObject::SetScreenTransition(float value, bool clamp) {
	float v = value;
	if (clamp && value <= 0.0f) {
		v = 0.0f;
	}

	// I think it calls this? could be wrong too lazy to figure it out rn
	SetScaleX(v);
}

void CBasePositionableObject::SetDefaultDirtyState(bool state) {
	mDefaultDirtyState = state;
	for (auto child : mChildren) {
		child->SetDefaultDirtyState(state);
	}
}

void CBasePositionableObject::_DeleteChildrenInternal() {}
void CBasePositionableObject::BaseDrawChildren() {
	for (auto child : mChildren) {
		child->Draw(true);
	}
}

CVec3 CBasePositionableObject::GetAbsoluteXYZ() {
	UpdateTransform(false);
	return mTransform * CVec3{};
}

void CBasePositionableObject::AddChild(CBasePositionableObject* child) {
	_AddChild(child, false);
}

void CBasePositionableObject::_SetParent(CBasePositionableObject* parent, bool addBefore) {
	if (mParent == parent) {
		return;
	}
	if (mParent != nullptr) {
		mParent->RemoveChild(this);
	}
	if (parent != nullptr) {
		if (addBefore) {
			parent->AddFirstChild(this);
		} else {
			parent->AddChild(this);
		}
		if (parent->mDefaultDirtyState != false) {
			SetDefaultDirtyState(true);
		}
	}
	FlagDirty();
}
