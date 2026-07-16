export module render;

#pragma once

export class Render {
    public:
     virtual ~Render() = default;
     virtual void draw() = 0;
};
