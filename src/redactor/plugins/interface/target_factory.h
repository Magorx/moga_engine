#pragma once


#include "redactor/plugin_std/std.hpp"


class TargetFactory : public P::RenderTargetFactory {
    virtual P::RenderTarget *spawn       (const P::Vec2s &size, const P::RGBA &color = {0, 0, 0, 255}) const override;
    virtual P::RenderTarget *from_pixels (const P::Vec2s &size, const P::RGBA *data)            const override;
    virtual P::RenderTarget *from_file   (const char *path)                              const override;
};
