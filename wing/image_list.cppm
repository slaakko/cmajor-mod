// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.image.list;

import wing.graphics;
import std.core;

export namespace wing {

class ImageList
{
public:
    ImageList();
    ImageList(const ImageList&) = delete;
    ImageList operator=(const ImageList&) = delete;
    void AddImage(const std::string& imageName);
    void AddDisabledImage(const std::string& imageName);
    void AddImage(const std::string& imageName, Bitmap* bitmap);
    int GetImageIndex(const std::string& imageName) const;
    int GetDisabledImageIndex(const std::string& imageName) const;
    Bitmap* GetImage(int imageIndex) const;
private:
    std::map<std::string, int> imageIndexMap;
    std::vector<std::unique_ptr<Bitmap>> images;
};

} // wing
