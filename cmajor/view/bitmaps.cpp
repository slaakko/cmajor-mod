// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.view.bitmaps;

import util;

namespace cmajor::view {

class Bitmaps
{
public:
    static Bitmaps& Instance();
    ~Bitmaps();
    wing::Bitmap* GetBitmap(const std::string& bitmapName);
    void Clear();
private:
    std::vector<std::unique_ptr<wing::Bitmap>> bitmaps;
    std::map<std::string, wing::Bitmap*> bitmapMap;
};

Bitmaps& Bitmaps::Instance()
{
    static Bitmaps instance;
    return instance;
}

Bitmaps::~Bitmaps()
{
    Clear();
}

wing::Bitmap* Bitmaps::GetBitmap(const std::string& bitmapName)
{
    auto it = bitmapMap.find(bitmapName);
    if (it != bitmapMap.cend())
    {
        return it->second;
    }
    else
    {
        std::u16string u16bitmapName = util::ToUtf16(bitmapName);
        std::unique_ptr<wing::Bitmap> bitmapPtr(wing::Bitmap::FromResource(wing::Instance(), (const WCHAR*)u16bitmapName.c_str()));
        wing::Bitmap* bitmap = bitmapPtr.get();
        bitmapMap[bitmapName] = bitmap;
        bitmaps.push_back(std::move(bitmapPtr)); 
        return bitmap;
    }
}

void Bitmaps::Clear()
{
    bitmaps.clear();
    bitmapMap.clear();
}

wing::Bitmap* GetBitmap(const std::string& bitmapName)
{
    return Bitmaps::Instance().GetBitmap(bitmapName);
}

void ClearBitmaps()
{
    Bitmaps::Instance().Clear();
}

} // namespace cmajor::view
