#include "RenderHelp.h"
#include <wincodec.h>
#include <iostream>

#pragma comment(lib, "windowscodecs.lib")  // WIC ���̺귯��
#pragma comment(lib, "msimg32.lib")        // AlphaBlend �Լ��� ���Ե� ���̺귯��

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

namespace renderHelp
{
    struct WICInitializer
    {
        WICInitializer() = default;

        ~WICInitializer()
        {
            DeleteAllBitmaps();

          /*  try
            {
                if (m_pFactory)
                {
                    m_pFactory->Release();
                    m_pFactory = nullptr;
                }
            }
            catch (const std::exception& e)
            {
                std::cout << e.what() << std::endl;
            }*/
    
            
            CoUninitialize();
        }

        bool Initialize()
        {         
            m_LastError = CoCreateInstance(
                CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pFactory));

            if (FAILED(m_LastError))
            {
                m_pFactory = nullptr;
                return false;
            }

            return true;
        }

        bool LoadImageFromFile(__in LPCWSTR filename, __out HBITMAP& hBitmap)
        {
            hBitmap = nullptr;
            if (m_pFactory == nullptr)
            {
                m_LastError = E_FAIL;
                return false;
            }

            m_LastError = m_pFactory->CreateDecoderFromFilename(
                filename, nullptr, GENERIC_READ,
                WICDecodeMetadataCacheOnLoad, &m_pDecoder);

            if (FAILED(m_LastError))
            {
                return false;
            }

            m_LastError = m_pDecoder->GetFrame(0, &m_pFrame);
            if (FAILED(m_LastError))
            {
                return false;
            }

            m_LastError = m_pFactory->CreateFormatConverter(&m_pConverter);
            if (FAILED(m_LastError))
            {
                return false;
            }

            m_LastError = m_pConverter->Initialize(m_pFrame, GUID_WICPixelFormat32bppPBGRA,
                WICBitmapDitherTypeNone, nullptr, 0.0f,
                WICBitmapPaletteTypeCustom);

            if (FAILED(m_LastError))
            {
                return false;
            }

            UINT width = 0, height = 0;
            m_LastError = m_pFrame->GetSize(&width, &height);

            if (FAILED(m_LastError))
            {
                return false;
            }

            BITMAPINFO bmi = { 0 };
            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth = width; // ��Ʈ�� �ʺ�
            bmi.bmiHeader.biHeight = -static_cast<LONG>(height); // top-down DIB
            bmi.bmiHeader.biPlanes = 1; // ��Ʈ�� ��� ��
            bmi.bmiHeader.biBitCount = 32; // 32bpp
            bmi.bmiHeader.biCompression = BI_RGB; // ���� ����

            void* pvImageBits = nullptr;
            HDC hdc = GetDC(nullptr);
            hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pvImageBits, nullptr, 0);
            ReleaseDC(nullptr, hdc);

            if (FAILED(m_LastError))
            {
                return false;
            }

            // ��Ʈ�ʿ� �ȼ� ���� (32bppPBGRA�� ��ȯ)
            m_LastError = m_pConverter->CopyPixels(nullptr, width * 4, width * height * 4, (BYTE*)pvImageBits);

            if (FAILED(m_LastError))
            {
                return false;
            }

            return true;
        }
     
        HRESULT GetLastError() const
        {
            return m_LastError;
        }

        void Clean()
        {
            if (m_pDecoder) m_pDecoder->Release();
            if (m_pFrame) m_pFrame->Release();
            if (m_pConverter) m_pConverter->Release();
        }

        BitmapInfo* CreateBitmapInfo(HBITMAP hBitmap)
        {
            BitmapInfo* pNewBitmap = new BitmapInfo(hBitmap);

            // ������ ��Ʈ�� ��带 �����ؾ� �Ѵ�.
            // ��带 ��ũ ���Ѽ� ������ ���ô�.

            return pNewBitmap;
        }

        void DeleteAllBitmaps()
        {
            
        }

        HRESULT m_LastError = S_OK;

        IWICImagingFactory* m_pFactory = nullptr;
        IWICBitmapDecoder* m_pDecoder = nullptr;
        IWICBitmapFrameDecode* m_pFrame = nullptr;
        IWICFormatConverter* m_pConverter = nullptr;

        // WICInitializer�� ������ �� ������ �����մϴ�.
        WICInitializer(const WICInitializer&) = delete;
        WICInitializer& operator=(const WICInitializer&) = delete;
    }GWICInitializer;
  

    BitmapInfo* CreateBitmapInfo(LPCWSTR filename)
    {
        static bool bCoInit = GWICInitializer.Initialize();
        if (false == bCoInit)
        {
            return nullptr;
        }

        HBITMAP hBitmap = nullptr;
        BitmapInfo* pBitmapInfo = nullptr;
        if (GWICInitializer.LoadImageFromFile(filename, hBitmap))
        {
            pBitmapInfo = GWICInitializer.CreateBitmapInfo(hBitmap);
        }

        GWICInitializer.Clean();

        return pBitmapInfo;
    }

    HDC Create32BitMemoryDC(HDC hdcScreen, int width, int height, HBITMAP& hBitmapOut)
    {
        // BITMAPINFO ����ü �ʱ�ȭ
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height;    // top-down DIB (���� ���)
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;         // 32��Ʈ (���� ����)
        bmi.bmiHeader.biCompression = BI_RGB;     // ���� ����

        // DIBSection ���� (DIB_RGB_COLORS: ���� ���̺� ������� ����)
        void* pBits = nullptr;
        hBitmapOut = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);
        if (!hBitmapOut)
        {
            return nullptr;
        }

        // ��ũ�� DC�� ȣȯ�Ǵ� �޸� DC ���� ��, ������ 32��Ʈ ��Ʈ���� ����
        HDC hdcMem = CreateCompatibleDC(hdcScreen);
        if (!hdcMem)
        {
            DeleteObject(hBitmapOut);
            hBitmapOut = nullptr;
            return nullptr;
        }

        SelectObject(hdcMem, hBitmapOut);
        return hdcMem;
    }

    // �¿� ���� �̹���
    BitmapInfo* CreateFlippedBitmap(BitmapInfo* pBitmapInfo)
    {
        if (nullptr == pBitmapInfo)
            return nullptr;

        HBITMAP hBitmap = pBitmapInfo->GetBitmapHandle();
        if (nullptr == hBitmap)
            return nullptr;

        int width = pBitmapInfo->GetWidth();
        int height = pBitmapInfo->GetHeight();

        HDC hdcScreen = GetDC(nullptr);
        HDC hdcSrc = CreateCompatibleDC(hdcScreen);
        SelectObject(hdcSrc, hBitmap);

        HBITMAP hFlippedBitmap = nullptr;
        HDC hdcDest = Create32BitMemoryDC(hdcScreen, width, height, hFlippedBitmap);

        // �¿� ���� �׸���
        StretchBlt(hdcDest, 0, 0, width, height, hdcSrc, width - 1, 0, -width, height, SRCCOPY);

        DeleteDC(hdcSrc);
        DeleteDC(hdcDest);
        ReleaseDC(nullptr, hdcScreen);

        BitmapInfo* pNewBitmapInfo = GWICInitializer.CreateBitmapInfo(hFlippedBitmap);
        return pNewBitmapInfo;
    }
}
