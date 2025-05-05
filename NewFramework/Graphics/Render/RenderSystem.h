#pragma once

#include "NewFramework/Graphics/Texture.h"
#include "NewFramework/Math/Matrix.h"
#include "Uncategorized/SpriteInformation.h"
#include "Uncategorized/types.h"

class CEventManager;
enum class eBufferTarget;
enum class eChangeState;
enum class eDataUsage;
enum class eRenderPrimitive;
struct SVertexElement;

// TODO: very basic skeleton. lot of stuff needs to be implemented before getting to this.
class CRenderSystem {
public:
    static void LoadTexture(const std::string& sName);
    static void UnloadTexture(const std::string& sName);
    static void BindTexture(CTexture* pTexture, int a2);
    static CTexture* CreateTextureFromMemory(uchar* pPixelData, uint uWidth, uint uHeight);
    static void CreateTextureFromMemory(CTexture* pTexture, uchar* pPixelData);
    static void UpdateTexture(CTexture* pTexture);
    static void UpdateTextureSubImage(
        CTexture* pTexture, int a2, int a3, uint a4, uint a5, ePixelDataFormat a6, ePixelDataType a7, void* a8, bool a9);
    static void BeginScissor(float a2, float a3, float a4, float a5);
    static void BeginScissorScaledToFrameBuffer(float a2, float a3, float a4, float a5);
    static void EndScissor();
    static void SetVsyncEnabled(bool bEnabled);
    static void CreateFrameBuffer(const std::string& a1, CTexture* pTexture, bool a3);
    static void DestroyFrameBuffer(const std::string& a1);
    static void SetFrameBuffer(const std::string& a1, const CVec2& a2, float a4, bool a3);
    static void RecreateBuffers();
    static void ResizeViewport(const CVec2i& a2, const CVec2i& a3);
    static void ChangeState(eChangeState a1, uint a2);
    static void AddShader(const std::string& a1, const std::string& a2 = "", const std::string& a3 = "");
    static void SetShader(ulong a1);
    static void SetShaderParameter(ulong a1, const std::string& a2, void* a3, uint a4, uint a5);
    static void SetShaderParameter(ulong a1, ulong a2, void* a3, uint a4, uint a5);
    static void SetCamera(const RectStruct& rect);
    static void SetCameraMatrix(const CMatrix& matrix);
    static void PushCameraMatrix();
    static void PopCameraMatrix();
    static void ResetCamera();
    static void CreateBuffer(uint* pBuffer);
    static void DestroyBuffer(uint a1);
    static void BindBuffer(eBufferTarget a1, uint a2);
    static void BufferData(
        uint a1, eBufferTarget a2, uint a3, const void* a4, eDataUsage a5, CEventManager* pEventManager);
    static void VBOSetup(
        uint a1, eBufferTarget a2, uint a3, const void* a4, eDataUsage a5, CEventManager* pEventManager);
    static void DrawIndexed(
        eRenderPrimitive a1, const SVertexElement* a2, uint a3, uint a4, ulong a5, const void* a6);
};
