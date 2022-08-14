//
// Created by https://github.com/AlexBychek
//

#include <Renderer.h>

#include <string>

Renderer::Renderer( HWND* consoleWindow, const GameType& gameType )
        : mPConsoleWindow(consoleWindow )
        , gameType_( gameType )
{
    bitmapInfo.bmiHeader.biSize        = sizeof(bitmapInfo.bmiHeader);
    bitmapInfo.bmiHeader.biWidth       = PMATRIX_WIDTH;
    bitmapInfo.bmiHeader.biHeight      = -PMATRIX_HEIGHT;
    bitmapInfo.bmiHeader.biPlanes      = 1;
    bitmapInfo.bmiHeader.biBitCount    = 32;
    bitmapInfo.bmiHeader.biCompression = BI_RGB;

    int bitmapMemorySize = (PMATRIX_WIDTH * PMATRIX_HEIGHT) * BYTES_PER_PIXEL;
    m_p_bitmapMemory = VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
}

Renderer::~Renderer()
{
    VirtualFree(m_p_bitmapMemory, 0, MEM_RELEASE);      // free memory for pixel bitmap
}

void Renderer::draw()
{
    int pitch;
    uint8_t* p_row;

    pitch = PMATRIX_WIDTH * BYTES_PER_PIXEL;
    p_row = reinterpret_cast<uint8_t*>(m_p_bitmapMemory);
    for (int y = 0; y < PMATRIX_HEIGHT; ++y)
    {
        uint32_t* p_pixel = reinterpret_cast<uint32_t*>(p_row);
        for (int x = 0; x < PMATRIX_WIDTH; ++x)
        {
            uint8_t blue  = GetBValue(mainDigitalMatrix[y * PMATRIX_WIDTH + x] );
            uint8_t green = GetGValue(mainDigitalMatrix[y * PMATRIX_WIDTH + x] );
            uint8_t red   = GetRValue(mainDigitalMatrix[y * PMATRIX_WIDTH + x] );
            uint8_t pixelPadding = 0;

            *p_pixel = ((pixelPadding << 24) | (red << 16) | (green << 8) | blue);
            ++p_pixel;
        }
        p_row += pitch;
    }

    deviceContext = GetDC(*mPConsoleWindow);

    SetTextColor(deviceContext, RGB(0, 0, 0));
    SetBkColor(deviceContext, RGB(169, 169, 169));

    StretchDIBits(
            deviceContext,
            OFFSET_LEFT, OFFSET_TOP,
            PMATRIX_WIDTH, PMATRIX_HEIGHT,
            0, 0,
            PMATRIX_WIDTH, PMATRIX_HEIGHT,
            m_p_bitmapMemory, &bitmapInfo,
            DIB_RGB_COLORS,
            SRCCOPY
    );

    HFONT hFont;
    hFont = static_cast<HFONT>(GetStockObject(SYSTEM_FIXED_FONT));
    SelectObject(deviceContext, hFont);

    std::string sTimer  = "Timer = " + std::to_string( statistic_->time );

    if (gameType_ == BREAKOUT)
    {
        std::string sScores = "Scores = " + std::to_string( statistic_->scores );
        std::string sLevel  = "Level = " + std::to_string( statistic_->levelNo );

        TextOut(deviceContext, PMATRIX_WIDTH / 4, 55, sTimer.c_str(), sTimer.size());
        TextOut(deviceContext, PMATRIX_WIDTH / 4, 75, sScores.c_str(), sScores.size());
        TextOut(deviceContext, PMATRIX_WIDTH / 2, 55, sLevel.c_str(), sLevel.size());
    }
    else
    {
        std::string sScores = "player 1 = " + std::to_string( statistic_->player1Scores );
        std::string sLevel  = "player 2 = " + std::to_string( statistic_->player2Scores );

        TextOut(deviceContext, PMATRIX_WIDTH / 2, 55, sTimer.c_str(), sTimer.size());
        TextOut(deviceContext, PMATRIX_WIDTH / 4, 75, sScores.c_str(), sScores.size());
        TextOut(deviceContext, (PMATRIX_WIDTH - 10) - PMATRIX_WIDTH / 4, 75, sLevel.c_str(), sLevel.size());
    }

    DeleteObject(hFont);

    ReleaseDC(*mPConsoleWindow, deviceContext);
}

void Renderer::setStatistic( std::shared_ptr<Statistic> statistic )
{
    statistic_ = statistic;
}
