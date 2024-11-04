#pragma once

#include <NewFramework/Math/MathematicTypes/VectorDataStructures.h>
#include <NewFramework/Math/Matrix.h>

#include <boost/intrusive/list_hook.hpp>
#include <boost/intrusive/list.hpp>
#include <boost/container/list.hpp>

struct _BasePositionableObjectDetail {
    struct SBasePositionableObjectListTag {};
};
struct SGameTime;
class CBasePositionableObject
    : public boost::intrusive::list_base_hook<
        boost::intrusive::tag<_BasePositionableObjectDetail::SBasePositionableObjectListTag>,
        boost::intrusive::link_mode<boost::intrusive::link_mode_type::auto_unlink>, void>
{
public:
    CBasePositionableObject() = default;

    void _AddChild(CBasePositionableObject* child, bool first);
    void RemoveChild(CBasePositionableObject* child);
    void FlagDirty();

    virtual ~CBasePositionableObject() = default;
    virtual void Draw(bool recursive);
    virtual void Process(const SGameTime&);
    virtual void Process_r(const SGameTime&);
    virtual void HitTest(CVec2, float);
    virtual void AddFirstChild();
    virtual void AddChildBefore();
    virtual void AddChildAfter();
    virtual void DeleteChildren();
    virtual void SetXYZ();
    virtual void SetXY(CVec2 vec);
    virtual void SetXY(float x, float y);
    virtual void SetX();
    virtual void SetY();
    virtual void SetZ();
    virtual void MoveXYZ();
    virtual void MoveXY();
    virtual void MoveX();
    virtual void MoveY();
    virtual void MoveZ();
    virtual void SetScale(float x, float y);
    virtual void SetScale(CVec2 vec);
    virtual void SetScaleX();
    virtual void SetScaleY();
    virtual void SetAngle();
    virtual void SetWH();
    virtual void SetH();
    virtual void SetAlignmentX();
    virtual void SetAlignmentY();
    virtual void SetAlignmentXY();
    virtual void GetXYZ();
    virtual void GetXY();
    virtual void GetX();
    virtual void GetY();
    virtual void GetZ();
    virtual void GetWH();
    virtual void GetW();
    virtual void GetH();
    virtual void UpdateLocalTransform();
    virtual void UpdateTransform();
    virtual void UpdateTransform_r();
    virtual void ForceUpdate();
    virtual void SetScreenTransition();
    virtual void SetDefaultDirtyState(bool state);
    virtual void _DeleteChildrenInternal();
    virtual void BaseDrawChildren();

    bool mVisible = true;
    CBasePositionableObject* mParent = nullptr;
    boost::container::list<CBasePositionableObject*> mChildren{};
    CVec2 mSize{};
    CVec2 mSizeHalf{};
    CMatrix mTransform{};
    bool mDirtyFlag = false;
    bool mDefaultDirtyState = false;
    CVec2 mAlignment{};
    CVec3 mLocation{};
    float mRotAngle{};
    CVec2 mScale{};
};