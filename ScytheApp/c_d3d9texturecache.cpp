#include "c_d3d9texturecache.h"
#include <d3dx9.h>

#pragma comment(lib, "d3dx9")
IDirect3DTexture9* scythe::c_d3d9texturecache::load_texture(const std::string& texture_name, const std::string& path)
{
    if (textureCache.find(texture_name) != textureCache.end()) {
        return textureCache[texture_name];  // Return cached texture
    }

    // Load the texture from file
    IDirect3DTexture9* texture = nullptr;
    if (FAILED(D3DXCreateTextureFromFileA(d3dDevice, path.c_str(), &texture))) {
        throw std::runtime_error("Failed to load texture: " + path);
    }

    textureCache[texture_name] = texture;  // Cache the texture
    return texture;
}

IDirect3DTexture9* scythe::c_d3d9texturecache::create_texture_from_bytes(const std::string& texture_name, int width, int height, const std::vector<unsigned char>& data)
{
    if (data.size() < width * height * 4) {
        throw std::runtime_error("Insufficient data size for the texture.");
    }

    // Create an empty texture
    IDirect3DTexture9* texture = nullptr;
    if (FAILED(d3dDevice->CreateTexture(width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture, nullptr))) {
        throw std::runtime_error("Failed to create texture from bytes.");
    }

    // Lock the texture and copy the data
    D3DLOCKED_RECT lockedRect;
    if (SUCCEEDED(texture->LockRect(0, &lockedRect, nullptr, 0))) {
        unsigned char* dest = static_cast<unsigned char*>(lockedRect.pBits);
        int pitch = lockedRect.Pitch;

        for (int y = 0; y < height; ++y) {
            memcpy(dest + y * pitch, &data[y * width * 4], width * 4);
        }

        texture->UnlockRect(0);
    }
    textureCache[texture_name] = texture;  // Cache the texture

    return texture;
}

IDirect3DTexture9* scythe::c_d3d9texturecache::get_cached_texture(const std::string& path) const
{
    auto it = textureCache.find(path);
    return (it != textureCache.end()) ? it->second : nullptr;
}
