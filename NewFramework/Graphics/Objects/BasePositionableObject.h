#pragma once

#include <NewFramework/Math/MathematicTypes/VectorDataStructures.h>
#include <NewFramework/Math/Matrix.h>

#include <boost/intrusive/list_hook.hpp>
#include <boost/intrusive/list.hpp>
#include <boost/container/list.hpp>

#include <array>

struct _BasePositionableObjectDetail {
    struct SBasePositionableObjectListTag {};
};
enum struct SPRITE_ALIGNMENT : int32_t {};
struct SGameTime {
    float fTime;
};

class CBasePositionableObject
    : public boost::intrusive::list_base_hook<
        boost::intrusive::tag<_BasePositionableObjectDetail::SBasePositionableObjectListTag>,
        boost::intrusive::link_mode<boost::intrusive::link_mode_type::auto_unlink>, void>
{
public:
    static size_t m_uUpdateTransforms;
    CBasePositionableObject() = default;

    void _AddChild(CBasePositionableObject* child, bool first);
    void AddChild(CBasePositionableObject* child);
    void RemoveChild(CBasePositionableObject* child);
    void FlagDirty();
    CVec3 GetAbsoluteXYZ();
    void _SetParent(CBasePositionableObject* parent, bool addBefore);

    virtual ~CBasePositionableObject() = default;
    virtual void Draw(bool recursive);
    virtual void Process(const SGameTime&);
    virtual void Process_r(const SGameTime&);
    virtual void HitTest(CVec2, float);
    virtual void AddFirstChild(CBasePositionableObject* child);
    virtual void AddChildBefore(CBasePositionableObject* child, CBasePositionableObject* before);
    virtual void AddChildAfter(CBasePositionableObject* child, CBasePositionableObject* after);
    virtual void DeleteChildren();
    virtual void SetXYZ(CVec3 vec);
    virtual void SetXY(CVec2 vec);
    virtual void SetXY(float x, float y);
    virtual void SetX(float x);
    virtual void SetY(float y);
    virtual void SetZ(float z);
    virtual void MoveXYZ(CVec3);
    virtual void MoveXY(CVec2);
    virtual void MoveX(float x);
    virtual void MoveY(float y);
    virtual void MoveZ(float z);
    virtual void SetScale(float x, float y);
    virtual void SetScale(CVec2 vec);
    virtual void SetScaleX(float x);
    virtual void SetScaleY(float y);
    virtual void SetAngle(float angle);
    virtual void SetWH(CVec2 vec);
    virtual void SetW(float w);
    virtual void SetH(float h);
    virtual void SetAlignmentX(SPRITE_ALIGNMENT);
    virtual void SetAlignmentY(SPRITE_ALIGNMENT);
    virtual void SetAlignmentXY(SPRITE_ALIGNMENT x, SPRITE_ALIGNMENT y);
    virtual CVec3 GetXYZ();
    virtual CVec2 GetXY();
    virtual float GetX();
    virtual float GetY();
    virtual float GetZ();
    virtual CVec2 GetWH();
    virtual float GetW();
    virtual float GetH();
    virtual void UpdateLocalTransform();
    virtual void UpdateTransform(bool cascadeUp);
    virtual void UpdateTransform_r();
    virtual void ForceUpdate();
    virtual void SetScreenTransition(float, bool);
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
    std::array<SPRITE_ALIGNMENT, 2> mAlignment{};
    CVec3 mLocation{};
    float mRotAngle{};
    CVec2 mScale{};
};
