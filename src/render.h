#pragma once

#include <vector>
#include "filter.h"
#include "scene.h"
#include "log.h"
#include "types.h"

class Render {
private:
    std::vector<Filter*> filters;
    dims viewport;
    Scene* scene;
public:

    Render(dims viewport): 
            viewport{viewport} {

        scene = new Scene(viewport, "../shader/out.frag", "../shader/out.vert");
    }

    void render() { 

        if (filters.size()==0) return;
        GLubyte* ptr = scene->map_frame();
        for (auto f: filters) f->apply(ptr);
        scene->unmap_frame();
        scene->draw();
    }

    void attach_filters(std::vector<Filter*> filters_) { filters=filters_; }
    ~Render() { LOGI("Destroy render") delete scene; scene=nullptr; }
};