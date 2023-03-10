#pragma once

#ifdef ANDROID
#include <android/asset_manager.h>
#else
#include <sstream>
#include <fstream>
#endif

#include "log.h"

#ifdef ANDROID
class Asset {
    private:
        off_t           count   {0};
        AAsset*         file    {nullptr};
        char*           content {nullptr};
        AAssetManager*  manager;
    public:
               // copy-move disabled
        Asset(const Asset&) = delete;
        Asset(Asset&&) = delete;
        Asset& operator=(const Asset&) = delete;
        Asset& operator=(Asset&&) = delete;

        Asset(AAssetManager* manager_input): manager{manager_input} {}

        bool open(const char* path) {

            LOGI("Asset create %s", path)
            file  = AAssetManager_open(manager, path, AASSET_MODE_BUFFER);

            if (file==nullptr) {

                LOGE("Could not open asset, %s", path)
                return false;
            } else {
                
                count   =   AAsset_getLength(file);
                content =   new char[count+1];
                LOGI("Asset opened OK %d bytes", (int)count)
                return true;
            }
        }

        ~Asset() {}

        const char* read() {
            
            if (AAsset_read(file, content, count)<=0) LOGE("Asset::get Could not read asset");
            content[count]='\0';
            return content;
        }

        void close() { delete[] content; count=0; AAsset_close(file); }
        off_t size() const {return count;}
};
#else
class Asset {
    private:
        size_t              count   {0};
        std::ifstream       file;
        std::stringstream   content;
        std::string         buf;
    public:
               // copy-move disabled
        Asset(const Asset&) = delete;
        Asset(Asset&&) = delete;
        Asset& operator=(const Asset&) = delete;
        Asset& operator=(Asset&&) = delete;

        Asset() {}

        bool open(const char* path) {

            LOGI("Asset create %s", path)
            file.open(path, std::ios_base::binary);

            if (!file.is_open()) {

                LOGE("Could not open asset, %s", path)
                return false;
            } else {
                content<<file.rdbuf();
                count = content.str().size();
                LOGI("Asset opened OK %d bytes", (int)count)
                return true;
            }
        }

        ~Asset() { file.close(); }

        const char* read() {
            
            if (count<=0) {LOGE("Asset::get Could not read asset"); return nullptr; }
            buf = content.str();
            return buf.c_str();
        }

        size_t size() const {return count;}
};
#endif