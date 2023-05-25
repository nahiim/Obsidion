
#pragma once


#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <string>

class AssetLoader
{
public:

    AssetLoader(){}

    AssetLoader(AAssetManager* assetManager) : m_assetManager(assetManager)
    {}


    char* loadAsset(const std::string& filename)
    {
        AAsset* asset = AAssetManager_open(m_assetManager, filename.c_str(), AASSET_MODE_BUFFER);
        if (!asset)
        {
            throw std::runtime_error("Failed to open asset: " + filename);
        }
        off_t assetSize = AAsset_getLength(asset);
        char* buffer = new char[assetSize + 1];
        AAsset_read(asset, buffer, assetSize);
        buffer[assetSize] = '\0';

        AAsset_close(asset);

        return buffer;
    }


private:
    AAssetManager* m_assetManager{};
};