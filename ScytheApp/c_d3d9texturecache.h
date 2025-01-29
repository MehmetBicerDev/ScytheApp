#pragma once
#include <d3d9.h>
#include <netincludes.h>





namespace scythe
{
	class c_d3d9texturecache
	{
	public:
		c_d3d9texturecache(IDirect3DDevice9* device) : d3dDevice(device){}


        ~c_d3d9texturecache() {
            // Release all cached textures
            for (auto& pair : textureCache) {
                if (pair.second) {
                    pair.second->Release();
                }
            }
        }

        IDirect3DTexture9* load_texture(const std::string& texture_name, const std::string& path);
        IDirect3DTexture9* create_texture_from_bytes(const std::string& texture_name, int width, int height, const std::vector<unsigned char>& data);
        IDirect3DTexture9* get_cached_texture(const std::string& path) const;
    private:
        IDirect3DDevice9* d3dDevice;
        std::unordered_map<std::string, IDirect3DTexture9*> textureCache;
	};
}