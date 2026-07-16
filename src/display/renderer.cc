export module renderer;

#pragma once

export class Renderer {
    public:
     virtual ~Renderer() = default;
     virtual void draw() = 0;
};
